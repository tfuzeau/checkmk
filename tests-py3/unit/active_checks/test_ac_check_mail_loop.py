# -*- encoding: utf-8
# pylint: disable=protected-access,redefined-outer-name
import pytest  # type: ignore
from testlib import import_module  # pylint: disable=import-error


@pytest.fixture(scope="module")
def check_mail_loop():
    return import_module("active_checks/check_mail_loop")


def test_ac_check_mail_main_loop_failed_to_send_mail(check_mail_loop):
    state, info, perf = check_mail_loop.main([
        '--smtp-server',
        'foo',
        '--fetch-server',
        'bar',
        '--fetch-username',
        'baz',
        '--fetch-password',
        'passw',
        '--mail-from',
        'from',
        '--mail-to',
        'to',
    ])
    assert state == 3
    assert info.startswith('Failed to send mail')
    assert perf is None


@pytest.mark.parametrize(
    "warning, critical, expected_mails, fetched_mails, expected_result",
    [
        (None, 3600, {}, {}, (0, 'Did not receive any new mail', [])),
        # No received mails
        (None, 3600, {
            '0-123': (0, 123)
        }, {},
         (2, 'Did not receive any new mail, Lost: 1 (Did not arrive within 3600 seconds)', [])),
        (None, 1000, {
            '0-123': (0, 123)
        }, {},
         (2, 'Did not receive any new mail, Lost: 1 (Did not arrive within 1000 seconds)', [])),
        (None, 1000, {
            '0-123': (0, 123),
            '0-Bar': (0, 'Bar'),
        }, {},
         (2, 'Did not receive any new mail, Lost: 2 (Did not arrive within 1000 seconds)', [])),
        (None, 2**64, {
            '0-123': (0, 123),
        }, {}, (0, 'Did not receive any new mail, Currently waiting for 1 mails', [])),
        (None, 2**64, {
            '0-123': (0, 123),
            '0-Bar': (0, 'Bar'),
        }, {}, (0, 'Did not receive any new mail, Currently waiting for 2 mails', [])),
        # No expected mails
        (None, 3600, {}, {
            '0-123': (0, 123),
        }, (0, 'Did not receive any new mail', [])),
        # Both fetched and expected mails
        (None, 3600, {
            '0-123': (0, 123),
            '0-456': (0, 456),
        }, {
            '0-123': (0, 123),
            '0-456': (0, 456),
        }, (0, 'Received 2 mails within average of 289 seconds', [('duration', 289.5, '', 3600)])),
        (None, 3600, {
            '0-123': (0, 123),
            '0-789': (0, 789),
        }, {
            '0-123': (0, 123),
            '0-456': (0, 456),
        }, (2, 'Mail received within 123 seconds, Lost: 1 (Did not arrive within 3600 seconds)', [
            ('duration', 123, '', 3600)
        ])),
    ])
def test_ac_check_mail_loop(check_mail_loop, warning, critical, expected_mails, fetched_mails,
                            expected_result):
    state, info, perf = check_mail_loop.check_mails(warning, critical, expected_mails,
                                                    fetched_mails)
    e_state, e_info, e_perf = expected_result
    assert state == e_state
    assert info == e_info
    assert perf == e_perf
