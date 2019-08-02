/**
 *    Copyright (C) 2018-present MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the Server Side Public License, version 1,
 *    as published by MongoDB, Inc.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Server Side Public License for more details.
 *
 *    You should have received a copy of the Server Side Public License
 *    along with this program. If not, see
 *    <http://www.mongodb.com/licensing/server-side-public-license>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the Server Side Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#pragma once
#include "mongo/platform/basic.h"

#include "mongo/stdx/condition_variable.h"


namespace mongo {
class Mutex;
class ConditionVariable : private stdx::condition_variable_any {
    using lock_t = stdx::unique_lock<Mutex>;
    friend class ::mongo::Waitable;

public:
    template <class Lock>
    void wait(Lock& lock);

    template <class Lock, class Predicate>
    void wait(Lock& lock, Predicate pred);

    template <class Lock, class Rep, class Period>
    stdx::cv_status wait_for(Lock& lock, const stdx::chrono::duration<Rep, Period>& rel_time);

    template <class Lock, class Rep, class Period, class Predicate>
    bool wait_for(Lock& lock,
                  const stdx::chrono::duration<Rep, Period>& rel_time,
                  Predicate pred);

    template <class Lock, class Clock, class Duration>
    stdx::cv_status wait_until(Lock& lock,
                               const stdx::chrono::time_point<Clock, Duration>& timeout_time);

    template <class Lock, class Clock, class Duration, class Predicate>
    bool wait_until(Lock& lock,
                    const stdx::chrono::time_point<Clock, Duration>& timeout_time,
                    Predicate pred);

    void notify_one() noexcept;
    void notify_all() noexcept;

private:
    stdx::condition_variable_any _condvar;
};

template <class Lock>
void ConditionVariable::wait(Lock& lock) {
    _condvar.wait(lock);
}

template <class Lock, class Predicate>
void ConditionVariable::wait(Lock& lock, Predicate pred) {
    _condvar.wait(lock, pred);
}

template <class Lock, class Rep, class Period>
stdx::cv_status ConditionVariable::wait_for(Lock& lock,
                                            const stdx::chrono::duration<Rep, Period>& rel_time) {
    return _condvar.wait_for(lock, rel_time);
}

template <class Lock, class Rep, class Period, class Predicate>
bool ConditionVariable::wait_for(Lock& lock,
                                 const stdx::chrono::duration<Rep, Period>& rel_time,
                                 Predicate pred) {
    return _condvar.wait_for(lock, rel_time, pred);
}

template <class Lock, class Clock, class Duration>
stdx::cv_status ConditionVariable::wait_until(
    Lock& lock, const stdx::chrono::time_point<Clock, Duration>& timeout_time) {
    return _condvar.wait_until(lock, timeout_time);
}

template <class Lock, class Clock, class Duration, class Predicate>
bool ConditionVariable::wait_until(Lock& lock,
                                   const stdx::chrono::time_point<Clock, Duration>& timeout_time,
                                   Predicate pred) {
    return _condvar.wait_until(lock, timeout_time, pred);
}

}  // namespace mongo
