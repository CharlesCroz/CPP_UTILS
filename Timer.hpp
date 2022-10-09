//
// Created by charles on 10/04/2022.
//

#ifndef TP2REGIONGROWING_TIMER_H
#define TP2REGIONGROWING_TIMER_H

#include <chrono>

class Timer {
public:
    Timer() = default;
    Timer(const Timer &) = default;
    Timer(Timer &&) noexcept = default;
    Timer &operator=(const Timer &) = default;
    Timer &operator=(Timer &&) noexcept = default;
    ~Timer() = default;

    Timer &start();
    Timer &stop() ;

    Timer &display_ms();
    Timer &display_ms(const std::string &message);

    long count_ms() const;
private:
    std::chrono::time_point<std::chrono::system_clock> start_point;
    std::chrono::time_point<std::chrono::system_clock> stop_point;
};

// Function declarations

Timer &Timer::start() {
    start_point = std::chrono::high_resolution_clock::now();
    return *this;
}

Timer &Timer::stop() {
    stop_point = std::chrono::high_resolution_clock::now();
    return *this;
}

long Timer::count_ms() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(stop_point - start_point).count();
}

Timer &Timer::display_ms() {
    std::cout << "\t---- Execution in " << count_ms() << " ms.\n";
    return *this;
}

Timer &Timer::display_ms(const std::string &message) {
    std::cout << message << "\n";
    return display_ms();
}

#endif //TP2REGIONGROWING_TIMER_H
