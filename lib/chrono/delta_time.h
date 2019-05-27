#pragma once

#include "time.h"

class DeltaTime
{
public:

    const Time& update()
    {
        m_dt = Time::now() - m_last_update;
        m_last_update = Time::now();
        return m_dt;
    }

private:

    Time m_dt;
    Time m_last_update;
};