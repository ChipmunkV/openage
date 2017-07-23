// Copyright 2017-2017 the openage authors. See copying.md for legal info.

#pragma once

#include <atomic>
#include <vector>

#include <QAbstractEventDispatcher>

namespace qtsdl {

class EventDispatcher : public QAbstractEventDispatcher {
public:
	explicit EventDispatcher(QObject *parent = nullptr);

	virtual void flush() override;
	virtual void interrupt() override;
	virtual bool processEvents(QEventLoop::ProcessEventsFlags flags) override;
	virtual void registerSocketNotifier(QSocketNotifier *notifier) override;
	virtual void registerTimer(int timerId, int interval, Qt::TimerType timerType, QObject *object) override;
	virtual QList<TimerInfo> registeredTimers(QObject *object) const override;
	virtual int remainingTime(int timerId) override;
	virtual void unregisterSocketNotifier(QSocketNotifier *notifier) override;
	virtual bool unregisterTimer(int timerId) override;
	virtual bool unregisterTimers(QObject *object) override;
	virtual void wakeUp() override;

	/**
	 * Obsolete function.
	 */
	virtual bool hasPendingEvents() override;

private:
	QList<TimerInfo> timer_infos;
	std::atomic_bool interrupted;
};

} // namespace qtsdl
