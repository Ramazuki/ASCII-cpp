#pragma once
#include <chrono>
#include <iostream>
#include <string>

namespace profiling
{

class LogDuration
{
public:
    using Clock = std::chrono::steady_clock;

    explicit LogDuration(std::string id, std::ostream& os = std::cerr)
        : id_(std::move(id))
        , os_(os)
    {
    }

    ~LogDuration()
    {
        const auto elapsed = Clock::now() - start_;
        os_ << id_ << ": "
            << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()
            << " microseconds" << std::endl;
    }

private:
    std::string id_;
    std::ostream& os_;
    Clock::time_point start_ = Clock::now();
};

class DurationMeter
{
public:
    using Clock = std::chrono::steady_clock;

    explicit DurationMeter(long long& result) noexcept
        : result_(result)
    {
    }

    ~DurationMeter()
    {
        const auto elapsed = Clock::now() - start_;
        result_ = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    }

private:
    long long& result_;
    Clock::time_point start_ = Clock::now();
};

} // namespace profiling

// Склейка имён
#define PROFILING_CONCAT_IMPL(a, b) a##b
#define PROFILING_CONCAT(a, b) PROFILING_CONCAT_IMPL(a, b)
#define PROFILING_UNIQUE_NAME(prefix) PROFILING_CONCAT(prefix, __LINE__)

#define LOG_DURATION(id) profiling::LogDuration PROFILING_UNIQUE_NAME(log_duration_)(id)
#define LOG_DURATION_STREAM(id, stream) profiling::LogDuration PROFILING_UNIQUE_NAME(log_duration_)(id, stream)
// Записывает время в переданную переменную
#define MEASURE_DURATION(result) profiling::DurationMeter PROFILING_UNIQUE_NAME(duration_meter_)(result)
