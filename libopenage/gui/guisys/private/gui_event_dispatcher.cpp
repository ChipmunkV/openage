// Copyright 2017-2017 the openage authors. See copying.md for legal info.

#include "gui_event_dispatcher.h"

namespace qtsdl {

EventDispatcher::EventDispatcher(QObject *parent)
	:
	QAbstractEventDispatcher{parent},
	interrupted{}
{
}

void EventDispatcher::flush() {
}

void EventDispatcher::interrupt() {
	this->interrupted = true;
}

bool EventDispatcher::processEvents(QEventLoop::ProcessEventsFlags flags) {}

void EventDispatcher::registerSocketNotifier(QSocketNotifier *notifier) {}

void EventDispatcher::registerTimer(int timer_id, int interval, Qt::TimerType timer_type, QObject *object) {}

QList<QAbstractEventDispatcher::TimerInfo> EventDispatcher::registeredTimers(QObject *object) const {
	return this->timer_infos;
}

int EventDispatcher::remainingTime(int timer_id) {}

void EventDispatcher::unregisterSocketNotifier(QSocketNotifier *notifier) {}

bool EventDispatcher::unregisterTimer(int timer_id) {}

bool EventDispatcher::unregisterTimers(QObject *object) {}

void EventDispatcher::wakeUp() {

}

bool EventDispatcher::hasPendingEvents() {

}

} // namespace qtsdl
