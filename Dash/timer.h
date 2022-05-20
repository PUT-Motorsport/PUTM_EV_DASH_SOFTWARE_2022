#pragma once

#include <chrono>
#include <string>

class Timer {
public:

    Timer() : m_started(false), m_start() {}

    void start() {
        m_start = std::chrono::high_resolution_clock::now();
        m_started = true;
    }

    auto isValid() {
        return m_started;
    }

    auto get() {

        if (m_started)
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_start).count();
#ifdef raspberrypi
        return 0ll;
#else
      return 0l;
#endif
    }

    static auto toStr(std::chrono::duration<long, std::ratio<1, 1000>>::rep time) {

        std::string result;

        result += std::to_string(time / 60 / 1000) + ':';
        result += std::to_string(time / 1000) + '.';

        auto miliseconds = std::to_string(time);
        miliseconds.resize(3, '0');

        return result + miliseconds;
    }

    auto getStr() {

        return toStr(get());

    }

private:

    bool m_started;
    std::chrono::system_clock::time_point m_start;

};
