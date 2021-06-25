#ifndef EVENTCLOCK_EVENTCLOCK_H
#define EVENTCLOCK_EVENTCLOCK_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <chrono>

namespace TimeTicks {
    using Hour = std::chrono::hours;
    using Minute = std::chrono::minutes;
    using Seconds = std::chrono::seconds;
    using Microseconds = std::chrono::microseconds;
    using Nanoseconds = std::chrono::nanoseconds;
};

template<typename TimeTicksType>
class EventClock {
public:
    using EventType = std::string;
    struct EventStatus {
        bool isRecording = false;
        std::chrono::time_point<std::chrono::steady_clock> startTime;
        TimeTicksType accumulateDuration;
    };

private:
    std::unordered_map<EventType, unsigned int> _eventAccumulateDuration;
    std::unordered_map<EventType, EventStatus> _eventStatus;

    std::string _getAbbreviation(TimeTicks::Hour);
    std::string _getAbbreviation(TimeTicks::Minute);
    std::string _getAbbreviation(TimeTicks::Seconds);
    std::string _getAbbreviation(TimeTicks::Microseconds);
    std::string _getAbbreviation(TimeTicks::Nanoseconds);

public:
    void startClock(const EventType &event);
    void stopClock(const EventType &event);
    TimeTicksType getEventDuration(const EventType &event);

    void printEvent(const EventClock::EventType &event, std::ostream &os = std::cout);
};


template<typename TimeTicksType>
void EventClock<TimeTicksType>::startClock(const EventClock::EventType &event) {
    EventStatus &eventStatus = _eventStatus[event];

    if(eventStatus.isRecording) {
        std::cerr << event << " is recoding." << std::endl;
        return;
    }

    eventStatus.isRecording = true;
    eventStatus.startTime = std::chrono::steady_clock::now();
}

template<typename TimeTicksType>
void EventClock<TimeTicksType>::stopClock(const EventClock::EventType &event) {
    EventStatus &eventStatus = _eventStatus[event];

    if(!eventStatus.isRecording) {
        std::cerr << event << " is not recoding." << std::endl;
        return;
    }

    auto currTime = std::chrono::steady_clock::now();
    eventStatus.accumulateDuration += std::chrono::duration_cast<TimeTicksType>(currTime - eventStatus.startTime);
    eventStatus.isRecording = false;
}

template<typename TimeTicksType>
TimeTicksType EventClock<TimeTicksType>::getEventDuration(const EventClock::EventType &event) {
    EventStatus &eventStatus = _eventStatus[event];

    if(eventStatus.isRecording) {
        std::cerr << event << " is recoding." << std::endl;
        return -1;
    }

    return eventStatus.accumulateDuration;
}

template<typename TimeTicksType>
void EventClock<TimeTicksType>::printEvent(const EventClock::EventType &event, std::ostream &os) {
    os << "The accumulate time of event \"" << event << "\" is "
       << _eventStatus[event].accumulateDuration.count() << ' ' << _getAbbreviation(TimeTicksType()) << "(s)"
       << std::endl;
}

template<typename TimeTicksType>
std::string EventClock<TimeTicksType>::_getAbbreviation(TimeTicks::Hour) {
    return "hour";
}

template<typename TimeTicksType>
std::string EventClock<TimeTicksType>::_getAbbreviation(TimeTicks::Minute) {
    return "min";
}

template<typename TimeTicksType>
std::string EventClock<TimeTicksType>::_getAbbreviation(TimeTicks::Seconds) {
    return "second";
}

template<typename TimeTicksType>
std::string EventClock<TimeTicksType>::_getAbbreviation(TimeTicks::Microseconds) {
    return "microsecond";
}

template<typename TimeTicksType>
std::string EventClock<TimeTicksType>::_getAbbreviation(TimeTicks::Nanoseconds) {
    return "nanosecond";
}



#endif //EVENTCLOCK_EVENTCLOCK_H
