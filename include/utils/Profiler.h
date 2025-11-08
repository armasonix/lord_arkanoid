#pragma once
#include <chrono>
#include <string>
#include <deque>

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
    explicit ScopedTimer(std::string name) : m_name(std::move(name)), m_t0(std::chrono::high_resolution_clock::now()) {}
    ~ScopedTimer() 
    {
        using namespace std::chrono;
        auto t1 = high_resolution_clock::now();
        double ms = duration<double, std::milli>(t1 - m_t0).count();
        Profiler::instance().push({ m_name, ms });
    }
private:
    std::string m_name;
    std::chrono::high_resolution_clock::time_point m_t0;
};

#define PROFILE_SCOPE(name) ScopedTimer _scoped_timer_##__LINE__{name}