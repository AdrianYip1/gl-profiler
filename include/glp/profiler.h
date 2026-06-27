#pragma once

#include <string>
#include <vector>
#include <stack>
#include <glad/glad.h>

struct TimingResult {
    std::string name;
    double ms;
    int depth;
};

class Profiler {
public:
    Profiler();
    ~Profiler();

    Profiler(const Profiler&) = delete;
    Profiler& operator=(const Profiler&) = delete;

    void beginFrame();
    void endFrame();

    void push(const std::string& name);
    void pop();

    const std::vector<TimingResult>& results() const { return mResults; }

private:
    struct ActiveRegion {
        std::string name;
        GLuint startQuery;
        int depth;
    };

    struct PendingRegion {
        std::string name;
        GLuint startQuery;
        GLuint endQuery;
        int depth;
    };

private:
    GLuint getNextQuery();

private:
    std::vector<GLuint> mQueries;
    size_t mNextQuery = 0;

    std::stack<ActiveRegion> mStack;
    std::vector<PendingRegion> mPending;
    std::vector<TimingResult> mResults;
};