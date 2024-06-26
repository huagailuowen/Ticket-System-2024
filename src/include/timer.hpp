#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
class Timer {
  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    long totduration;
    const char *name;
  public:
    Timer(const char *s) : name(s), totduration(0) {}
    void start() {
        start_time = std::chrono::system_clock::now();
    }

    void stop() {
        auto end_time = std::chrono::system_clock::now();
        totduration += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    }

    double duration() {
        return (double)totduration * std::chrono::nanoseconds::period::num / std::chrono::nanoseconds::period::den;
    }

    ~Timer() {
        fprintf(stderr, "%s took %.6lf seconds\n", name, duration());
    }
};

#endif // TIMER_HPP