//
// Created by charles on 10/04/2022.
//

#ifndef TP2REGIONGROWING_TIMER_H
#define TP2REGIONGROWING_TIMER_H

#include <chrono>

class Timer {
public:
    Timer() = default;

    Timer &start() {
        start_point = std::chrono::high_resolution_clock::now();
        return *this;
    }

    Timer &stop() {
        stop_point = std::chrono::high_resolution_clock::now();
        return *this;
    }

    long count_ms() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(stop_point - start_point).count();
    }

    Timer &display_ms() {
        std::cout << "\t---- Execution in " << count_ms() << " ms.\n";
        return *this;
    }

    Timer &display_ms(const std::string &message) {
        std::cout << message << "\n";
        return display_ms();
    }

private:
    std::chrono::time_point<std::chrono::system_clock> start_point;
    std::chrono::time_point<std::chrono::system_clock> stop_point;
};

#endif //TP2REGIONGROWING_TIMER_H
