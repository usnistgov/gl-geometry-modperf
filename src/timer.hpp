#ifndef TIMER_HPP
#define TIMER_HPP

#include <turf/CPUTimer.h>
#include <turf/ConditionVariable.h>

class Timer {
public:
    enum MarkBuffer { FRONT, BACK, NUM_BUFFERS };
    enum MarkPoint { START, CLEAR, DRAW, SWAP, NUM_MARK_POINTS };

    Timer(int nSamples, int nBins);
    ~Timer();

    void mark(MarkPoint markPoint) {
        int const head = _markHeads[_markBuffer][markPoint];
        _markPoints[_markBuffer][markPoint][head] = turf::CPUTimer::get();
        _markHeads[_markBuffer][markPoint]++;
    }

    void swap() {
        _frame[_markBuffer]++;
        if (!full(_markBuffer)) return;

        {
            turf::LockGuard<turf::Mutex> lock(_mutex);
            MarkBuffer prevBuffer = _markBuffer;
            _markBuffer = (_markBuffer==FRONT) ? BACK : FRONT;
            _frame[_markBuffer] = _frame[prevBuffer];
        }

        _ready.wakeOne();
    }

    void stop() {
        {
            turf::LockGuard<turf::Mutex> lock(_mutex);
            _running = false;
        }

        _ready.wakeOne();
    }

    static void* process(void* p);

private:
    const int _nSamples;
    const int _nBins;
    MarkBuffer _markBuffer;
    bool _running;
    int _frame[NUM_BUFFERS];

    typedef turf::CPUTimer::Point* MarkPoint;
    MarkPoint _markPoints[NUM_BUFFERS][NUM_MARK_POINTS];
    int _markHeads[NUM_BUFFERS][NUM_MARK_POINTS];

    turf::Mutex _mutex;
    turf::ConditionVariable _ready;

    bool full(MarkBuffer buffer) const {
        for (int i=0; i<NUM_MARK_POINTS; ++i) {
            if (_markHeads[buffer][i] != _nSamples) return false;
        }
        return true;
    }
};

#endif

