#include "utils/Profiler.h"

namespace ark 
{

    ScopedTimer::ScopedTimer(std::string name)
        : m_name(std::move(name)),
        m_t0(std::chrono::high_resolution_clock::now())
    {}

    ScopedTimer::~ScopedTimer() 
    {
        using namespace std::chrono;
        auto t1 = high_resolution_clock::now();
        double ms = duration<double, std::milli>(t1 - m_t0).count();
        Profiler::instance().push({ m_name, ms });
    }

} // namespace ark