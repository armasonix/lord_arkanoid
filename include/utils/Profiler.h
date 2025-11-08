#pragma once
#include <chrono>
#include <string>
#include <deque>

namespace ark 
{

struct Sample 
{
    std::string name;
    double ms;
};

class Profiler 
{
public:
    static Profiler& instance() { static Profiler p; return p; }
    void push(const Sample& s) 
    {
        m_samples.push_front(s);
        if (m_samples.size() > 64) m_samples.pop_back();
    }
    const std::deque<Sample>& samples() const { return m_samples; }
private:
    std::deque<Sample> m_samples;
};

class ScopedTimer 
{
public:
    explicit ScopedTimer(std::string name);
    ~ScopedTimer();
private:
    std::string m_name;
    std::chrono::high_resolution_clock::time_point m_t0;
};

} // namespace ark

#define ARK_CONCAT_INNER(a,b) a##b
#define ARK_CONCAT(a,b) ARK_CONCAT_INNER(a,b)
#define PROFILE_SCOPE(name) ::ark::ScopedTimer ARK_CONCAT(_ark_scoped_timer_, __COUNTER__){name}