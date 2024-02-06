#include "Timer.h"
#include "Constants.h"
#include <thread>
#include <chrono>
#include <cstdint>
#include <mutex>


Timer::Timer() {
    start();
}

Timer::~Timer() {
    stop();
}

void Timer::start() {    
    if (!m_running) {
        m_running = true;
        m_timerThread = std::thread(&Timer::timer, this); 
    }
}

void Timer::stop() {
    m_running = false;
    if (m_timerThread.joinable()) {
        m_timerThread.join();
    }
}

void Timer::timer() {
    while (true) {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (!m_running) break;
        }
        std::this_thread::sleep_for(std::chrono::duration<double>(Constants::timerFrequency));
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_value > 0)
                --m_value;
        }
    }
}

void Timer::setValue(uint8_t newValue) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_value = newValue;
}

uint8_t Timer::getValue() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_value;
}
