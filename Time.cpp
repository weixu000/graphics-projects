#include <chrono>

#include "Time.h"

float Time::_now = 0.0f;
float Time::_delta = 0.0f;

using namespace std::chrono;

void Time::tick() {
    auto next = duration_cast<duration<float>>(steady_clock::now().time_since_epoch()).count();
    _delta = next - _now;
    _now = next;
}

void Time::reset() {
    _now = duration_cast<duration<float>>(steady_clock::now().time_since_epoch()).count();
}
