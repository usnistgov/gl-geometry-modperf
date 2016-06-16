#include "timer.hpp"

#include <algorithm>
#include <cstdio>

Timer::Timer(int nSamples, int nBins)
: _nSamples(nSamples), _nBins(nBins), _markBuffer(FRONT), _running(true) {
    for (int i=0; i<NUM_BUFFERS; ++i) {
        _frame[i] = 0;

        for (int j=0; j<NUM_MARK_POINTS; ++j) {
            _markPoints[i][j] = new turf::CPUTimer::Point[nSamples];
            _markHeads[i][j] = 0;
        }
    }
}

Timer::~Timer() {
    for (int i=0; i<NUM_BUFFERS; ++i) {
        for (int j=0; j<NUM_MARK_POINTS; ++j) {
            delete[] _markPoints[i][j];
        }
    }
}

void* Timer::process(void* p) {
    Timer* timer = reinterpret_cast<Timer*>(p);
    turf::CPUTimer::Converter converter;

    FILE* csv = fopen("times.csv", "w");
    fprintf(csv, "frame,mark,binwidth,min,max,mean,bins...\n");

    turf::CPUTimer::Duration durationsMin, durationsMax, durationsMean;
    turf::CPUTimer::Duration* durations =
        new turf::CPUTimer::Duration[timer->_nSamples];
    int* durationsBins = new int[timer->_nBins+1];

    for (;;) {
        {
            turf::LockGuard<turf::Mutex> lock(timer->_mutex);
            timer->_ready.wait(lock);
            if (!timer->_running) break;
        }

        MarkBuffer workBuffer = (timer->_markBuffer==FRONT) ? BACK : FRONT;
        if (!timer->full(workBuffer)) continue;


        for (int i=0; i<NUM_MARK_POINTS-1; ++i) {
            durationsMean = 0;
            for (int j=0; j<timer->_nBins+1; ++j) durationsBins[j] = 0;

            for (int j=0; j<timer->_nSamples; ++j) {
                durations[j] = timer->_markPoints[workBuffer][i+1][j] -
                    timer->_markPoints[workBuffer][0][j];
                durationsMean += durations[i]/timer->_nSamples;

                if (j==0) {
                    durationsMin = durationsMax = durations[j];
                } else {
                    durationsMin = std::min(durationsMin, durations[j]);
                    durationsMax = std::max(durationsMax, durations[j]);
                }
            }

            double const binWidth = (durationsMax-durationsMin) /
              static_cast<double>(timer->_nBins);
            for (int j=0; j<timer->_nSamples; ++j) {
                int const bin = (durations[j]-durationsMin) / binWidth;
                durationsBins[bin]++;
            }


            fprintf(csv, "%d,%d,%g,%ld,%ld,%ld,", timer->_frame[workBuffer],
                    i+1, binWidth, durationsMin, durationsMax, durationsMean);
            for (int j=0; j<timer->_nBins+1; ++j) {
                fprintf(csv, "%d%c", durationsBins[j],
                        (j<timer->_nBins) ? ',' : '\n');
            }
            fflush(csv);
        }


        for (int i=0; i<NUM_MARK_POINTS; ++i) {
            timer->_markHeads[workBuffer][i] = 0;
        }

    }

    fclose(csv);
    delete[] durations;
    delete[] durationsBins;
    return NULL;
}

