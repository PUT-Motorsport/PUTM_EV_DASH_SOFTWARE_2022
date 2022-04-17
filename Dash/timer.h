#pragma once

#include <chrono>

class Timer {
public:
    Timer() {}

    void start() {
        m_start = std::chrono::high_resolution_clock::now();
    }

    auto get() {
        return std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - m_start).count();
    }

private:

    std::chrono::system_clock::time_point m_start;

public:
    Timer(Timer const &) = delete;
    Timer(Timer && obj) = delete;
};
