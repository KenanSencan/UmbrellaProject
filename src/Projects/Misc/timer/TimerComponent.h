#pragma once
#include <chrono>

enum class TimeUnit
{
    Nanosecond,
    Microsecond,
    Millisecond,
    Second,
    Minute,
    Hour,
    Day,
    Week,
    Year,
    Month
};

//`std::chrono::steady_clock::now()` already increasing every line implicitly.
//So, We don't need to increment m_ElapsedTime manually. All we need to know is StartTime and then try to get CurrentTime. If we subtract two, we will get ElapsedTime.
//TotalDuration and ElapsedTime will be held nanoseconds. Everything will be converted nanoseconds before any operation.  
class TimerComponent
{
public:
    explicit TimerComponent(float duration, TimeUnit unit = TimeUnit::Second);
    ~TimerComponent() = default;

    void Start();
    void Stop();
    void Reset();
    void Restart();

    void Update();

    [[nodiscard]] bool IsFinished() const;
    [[nodiscard]] bool IsRunning() const;

    [[nodiscard]] float GetRemainingTime(TimeUnit unit = TimeUnit::Second) const;
    [[nodiscard]] float GetElapsedTime(TimeUnit unit = TimeUnit::Second) const;

    // Get percentage complete (0.0 to 1.0)
    [[nodiscard]] float GetProgress() const;

    void SetDuration(float newDuration, TimeUnit unit = TimeUnit::Second);

private:
    // Convert between time units
    [[nodiscard]] static float ConvertTimeUnit(float time, TimeUnit fromUnit, TimeUnit toUnit);
    [[nodiscard]] static std::chrono::nanoseconds ConvertToNanos(float time, TimeUnit unit);
    static float NanoSecToTargetUnit(TimeUnit toUnit, double nanoseconds);
    std::chrono::nanoseconds CalculateCurrentElapsedTime(const std::chrono::nanoseconds& currentElapsed) const;

    std::chrono::nanoseconds m_TotalDuration{}; // Total duration in nanoseconds (whatever unit is given, it'll be converted to be nanoseconds firstly)
    std::chrono::steady_clock::time_point m_StartTime; // Point when timer was started (the value at here doesn't matter we will subtract this with end timer) 
    std::chrono::nanoseconds m_ElapsedTime; // Accumulated time when stopped
    bool m_IsRunning;
    bool m_IsFinished{};
};
