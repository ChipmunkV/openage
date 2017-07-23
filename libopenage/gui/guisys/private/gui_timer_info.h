// Copyright 2017-2017 the openage authors. See copying.md for legal info.

#pragma once

#include <vector>
#include <chrono>

#include <QAbstractEventDispatcher>

struct TimerInfo {
    int id;           // - timer identifier
    std::chrono::milliseconds interval;     // - timer interval in milliseconds
    Qt::TimerType timer_type; // - timer type
    std::chrono::steady_clock::time_point timeout;  // - when to actually fire
    QObject *obj;     // - object to receive event
    TimerInfo **activateRef; // - ref from activateTimers
};

class TimerInfoList
{
#if ((_POSIX_MONOTONIC_CLOCK-0 <= 0) && !defined(Q_OS_MAC)) || defined(QT_BOOTSTRAPPED)
    timespec previousTime;
    clock_t previousTicks;
    int ticksPerSecond;
    int msPerTick;

    bool timeChanged(timespec *delta);
    void timerRepair(const timespec &);
#endif

    // state variables used by activateTimers()
    TimerInfo *firstTimerInfo;

    std::vector<TimerInfo> infos;

public:
    TimerInfoList();

    std::chrono::steady_clock::time_point current_time;
    std::chrono::steady_clock::time_point update_current_time();

    // must call updateCurrentTime() first!
    void repairTimersIfNeeded();

    bool timer_wait(timespec&);
    void timer_insert(const TimerInfo&);

    int timerRemainingTime(int timer_id);

    void register_timer(int timerId, int interval_ms, Qt::TimerType timer_type, QObject *object);
    bool unregister_timer(int timer_id);
    bool unregister_timers(QObject *object);
    QList<QAbstractEventDispatcher::TimerInfo> registered_timers(QObject *object) const;

    int activate_timers();
};
