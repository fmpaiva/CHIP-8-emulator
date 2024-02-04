#ifndef TIMER_H
#define TIMER_H

#include <cstdint>
#include <thread>

class Timer {
public:
    Timer();
    ~Timer();
    void setValue(uint8_t);
    uint8_t getValue();

private:
    uint8_t m_value{0};
    bool m_running {false};
    std::thread m_timerThread;
    std::mutex m_mutex;

    void timer();
    void start();
    void stop();
};

#endif
