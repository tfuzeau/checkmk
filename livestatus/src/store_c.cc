// +------------------------------------------------------------------+
// |             ____ _               _        __  __ _  __           |
// |            / ___| |__   ___  ___| | __   |  \/  | |/ /           |
// |           | |   | '_ \ / _ \/ __| |/ /   | |\/| | ' /            |
// |           | |___| | | |  __/ (__|   <    | |  | | . \            |
// |            \____|_| |_|\___|\___|_|\_\___|_|  |_|_|\_\           |
// |                                                                  |
// | Copyright Mathias Kettner 2014             mk@mathias-kettner.de |
// +------------------------------------------------------------------+
//
// This file is part of Check_MK.
// The official homepage is at http://mathias-kettner.de/check_mk.
//
// check_mk is free software;  you can redistribute it and/or modify it
// under the  terms of the  GNU General Public License  as published by
// the Free Software Foundation in version 2.  check_mk is  distributed
// in the hope that it will be useful, but WITHOUT ANY WARRANTY;  with-
// out even the implied warranty of  MERCHANTABILITY  or  FITNESS FOR A
// PARTICULAR PURPOSE. See the  GNU General Public License for more de-
// ails.  You should have  received  a copy of the  GNU  General Public
// License along with GNU Make; see the file  COPYING.  If  not,  write
// to the Free Software Foundation, Inc., 51 Franklin St,  Fifth Floor,
// Boston, MA 02110-1301 USA.

#include "store_c.h"
#include "ClientQueue.h"
#include "InputBuffer.h"
#include "OutputBuffer.h"
#include "Store.h"
#include "TimeperiodsCache.h"

Store *g_store = nullptr;
ClientQueue *g_client_queue = nullptr;
TimeperiodsCache *g_timeperiods_cache = nullptr;

void store_init() {
    g_store = new Store();
    g_client_queue = new ClientQueue();
    g_timeperiods_cache = new TimeperiodsCache();
}

void store_deinit() {
    if (g_store != nullptr) {
        delete g_store;
        g_store = nullptr;
    }
    if (g_client_queue != nullptr) {
        delete g_client_queue;
        g_client_queue = nullptr;
    }
    if (g_timeperiods_cache != nullptr) {
        delete g_timeperiods_cache;
        g_timeperiods_cache = nullptr;
    }
}

void queue_add_connection(int cc) { g_client_queue->addConnection(cc); }

int queue_pop_connection() { return g_client_queue->popConnection(); }

void queue_terminate() { return g_client_queue->terminate(); }

void store_register_comment(nebstruct_comment_data *d) {
    g_store->registerComment(d);
}

void store_register_downtime(nebstruct_downtime_data *d) {
    g_store->registerDowntime(d);
}

int store_answer_request(void *ib, void *ob) {
    return static_cast<int>(g_store->answerRequest(
        static_cast<InputBuffer *>(ib), static_cast<OutputBuffer *>(ob)));
}

void *create_outputbuffer() { return new OutputBuffer(); }

void flush_output_buffer(void *ob, int fd, int *termination_flag) {
    static_cast<OutputBuffer *>(ob)->flush(fd, termination_flag);
}

void delete_outputbuffer(void *ob) { delete static_cast<OutputBuffer *>(ob); }

void *create_inputbuffer(int fd, const int *termination_flag) {
    return new InputBuffer(fd, termination_flag);
}

void delete_inputbuffer(void *ib) { delete static_cast<InputBuffer *>(ib); }

void update_timeperiods_cache(time_t now) { g_timeperiods_cache->update(now); }

void log_timeperiods_cache() { g_timeperiods_cache->logCurrentTimeperiods(); }
