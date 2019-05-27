#include "rate.h"

Rate::Rate(const float hz)
:
m_loop_time(hertz(hz)),
m_loop_start_time(Time::now())
{

}

void Rate::sleep()
{
    while (Time::now() - m_loop_start_time < m_loop_time);

    m_loop_start_time = Time::now();
}