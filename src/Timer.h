#ifndef TIMER_H
#define TIMER_H

#include <cstdint>
#include <thread>
#include <mutex>

/* CHIP-8 has two timers. Delay timer and sound timer. They're both one byte in size
 * and as long as their value is above 0, it is decremented at a rate of 60 Hz.
 * Sound timer emits a beep while its value is above 0.
 */

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
