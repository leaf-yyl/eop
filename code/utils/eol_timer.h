




#ifndef EOL_TIMER_H
#define EOL_TIMER_H

#include <chrono>

namespace eol {

class EolTimer
{
public:
    EolTimer();
    ~EolTimer();

    void reset() {
        m_count = 0;
        m_last_time = 0;
        m_total_time = 0;
        m_is_on_timing = false;
    }
    void start();
    void stop();
    int total_count();
    float last_elapsed();
    float total_elapsed();

protected:
    int m_count = 0;
    bool m_is_on_timing = false;
    float m_last_time   = 0.0f;
    float m_total_time  = 0.0f;

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start_tp;
};

} // namespace eol


#endif //DL_TIMER_H
