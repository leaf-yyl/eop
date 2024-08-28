




#include "eol_timer.h"

namespace eol {

EolTimer::EolTimer()
{
}

EolTimer::~EolTimer()
{
}

void EolTimer::start()
{
    if(!m_is_on_timing) {
        m_is_on_timing = true;
        m_start_tp = std::chrono::high_resolution_clock::now();
    }
}

void EolTimer::stop()
{
    if(m_is_on_timing) {
        m_is_on_timing = false;
        auto tp = std::chrono::high_resolution_clock::now();
        m_last_time = std::chrono::duration_cast<std::chrono::nanoseconds>(tp - m_start_tp).count();
        m_count++;
        m_total_time += m_last_time;
    }
}

int EolTimer::total_count()
{
    return m_count;
}

float EolTimer::last_elapsed()
{
    return m_last_time / 1000000.0f;
}

float EolTimer::total_elapsed()
{
    return m_total_time / 1000000.0f;
}

} // namespace dl
