// Copyright 2017-2017 the openage authors. See copying.md for legal info.

#include "gui_event_dispatcher.h"

#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <cassert>
#include <sys/select.h>

#include <QCoreApplication>
#include <QDebug>
#include <QSocketNotifier>

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

bool EventDispatcher::processEvents(QEventLoop::ProcessEventsFlags flags) {
	bool had_events = false;
	qInfo() << flags;

	while (!this->interrupted) {
		if (!(flags & QEventLoop::ExcludeSocketNotifiers))
		{
			fd_set read_fd_set;
			fd_set write_fd_set;
			fd_set except_fd_set;
			struct timeval timeout = {};
			fd_set* fd_sets[3] = {&read_fd_set, &write_fd_set, &except_fd_set};
			static_assert(QSocketNotifier::Read == 0, "QSocketNotifier::Type enum incompatible");
			static_assert(QSocketNotifier::Write == 1, "QSocketNotifier::Type enum incompatible");
			static_assert(QSocketNotifier::Exception == 2, "QSocketNotifier::Type enum incompatible");

			FD_ZERO(&read_fd_set);
			FD_ZERO(&write_fd_set);
			FD_ZERO(&except_fd_set);

			for (auto notifier : this->socket_notifiers)
				switch (notifier->type()) {
				case QSocketNotifier::Read:
				case QSocketNotifier::Write:
				case QSocketNotifier::Exception:
					FD_SET(notifier->socket(), fd_sets[notifier->type()]);
					break;
				default:
					assert(false);
					break;
				}

			emit this->aboutToBlock();
			if (select(FD_SETSIZE, &read_fd_set, &write_fd_set, &except_fd_set, &timeout) < 0)
			{
				emit this->awake();
				qCritical() << "socket notifier failed";
			}
			else
			{
				emit this->awake();
				QEvent event(QEvent::SockAct);
				for (auto notifier : this->socket_notifiers)
					switch (notifier->type()) {
					case QSocketNotifier::Read:
					case QSocketNotifier::Write:
					case QSocketNotifier::Exception:
						if (FD_ISSET(notifier->socket(), fd_sets[notifier->type()])) {
							qInfo() << "evt";
							QCoreApplication::sendEvent(notifier, &event);
							had_events = true;
						}
						break;
					default:
						assert(false);
						break;
					}
			}
		}

		QCoreApplication::sendPostedEvents();

		if (!(flags & QEventLoop::WaitForMoreEvents))
			break;
	}

	return had_events;
}

void EventDispatcher::registerSocketNotifier(QSocketNotifier *notifier) {
	//assert(false);
	qWarning() << notifier->socket() << notifier->type();
	this->socket_notifiers.push_back(notifier);
}

void EventDispatcher::registerTimer(int timer_id, int interval, Qt::TimerType timer_type, QObject *object) {
//	auto found_it = std::find_if(std::begin(this->timer_infos), std::end(this->timer_infos), [timer_id](const TimerInfo &info) { return info.timerId == timer_id; });
//
//	if (found_it != std::end(this->timer_infos))
//		*found_it = TimerInfo{timer_id, interval, timer_type};
//	else
//		this->timer_infos.push_back(TimerInfo{timer_id, interval, timer_type});
}

QList<QAbstractEventDispatcher::TimerInfo> EventDispatcher::registeredTimers(QObject *object) const {
//	const auto found_it = this->timer_infos.find(object);
//	return found_it != std::end(this->timer_infos) ? *found_it : QList<TimerInfo>{};
}

int EventDispatcher::remainingTime(int timer_id) {
	assert(false);
}

void EventDispatcher::unregisterSocketNotifier(QSocketNotifier *notifier) {
//	assert(false);
	this->socket_notifiers.erase(std::remove(std::begin(this->socket_notifiers), std::end(this->socket_notifiers), notifier), std::end(this->socket_notifiers));
}

bool EventDispatcher::unregisterTimer(int timer_id) {
//	const auto found_it = std::find_if(std::begin(this->timer_infos), std::end(this->timer_infos), [timer_id](const TimerInfo &info) { return info.timerId == timer_id; });
//	const bool found = found_it != std::end(this->timer_infos);
//
//	if (found)
//		this->timer_infos.erase(found_it);
//
//	return found;
}

bool EventDispatcher::unregisterTimers(QObject *object) {
	assert(false);
}

void EventDispatcher::wakeUp() {
	//assert(false);
}

bool EventDispatcher::hasPendingEvents() {
	assert(false);
}

} // namespace qtsdl
