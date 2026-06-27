#include "glp/profiler.h"


Profiler::Profiler() {
    mQueries.clear();
    mQueries.reserve(32);
}

Profiler::~Profiler() {
    if (!mQueries.empty()) {
        glDeleteQueries(static_cast<GLsizei>(mQueries.size()), mQueries.data());
    }
}

GLuint Profiler::getNextQuery() {
    if (mNextQuery >= mQueries.size())
    {
        GLuint q;
        glGenQueries(1, &q);
        mQueries.push_back(q);
    }

    return mQueries[mNextQuery++];
}

void Profiler::beginFrame() {
    mNextQuery = 0;
    mResults.clear();
    mPending.clear();
}

void Profiler::endFrame() {
    mResults.clear();

    for (const auto& region : mPending) {
        GLuint64 start = 0, end = 0;

        glGetQueryObjectui64v(region.startQuery, GL_QUERY_RESULT, &start);
        glGetQueryObjectui64v(region.endQuery, GL_QUERY_RESULT, &end);

        mResults.push_back({
            region.name,
            static_cast<double>(end-start) / 1e6, //ns to ms
            region.depth
        });
    }

    mPending.clear();
}

void Profiler::push(const std::string& name)
{
    GLuint startQuery = getNextQuery();
    glQueryCounter(startQuery, GL_TIMESTAMP);

    mStack.push({
        name,
        startQuery,
        static_cast<int>(mStack.size())
    });
}

void Profiler::pop()
{
    if (mStack.empty())
        return;

    ActiveRegion region = mStack.top();
    mStack.pop();

    GLuint endQuery = getNextQuery();
    glQueryCounter(endQuery, GL_TIMESTAMP);

    mPending.push_back({
        region.name,
        region.startQuery,
        endQuery,
        region.depth
    });
}