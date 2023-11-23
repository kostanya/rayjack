#pragma once

#include "camera.h"
#include "hittable_list.h"
#include "utils.h"
#include <atomic>
#include <mutex>
#include <queue>
#include <stdint.h>
#include <thread>
#include <vector>

class RenderThread {

public:
    struct Work {
        uint16_t scanline = 0;
    };

    struct Output {
        uint16_t scanline = 0;
        std::vector<color> data = {};
    };

    explicit RenderThread(const Camera& cam);
    explicit RenderThread(RenderThread&& other);

    bool run(const HittableList& world);

    // Queue a task to turn, returns false if failed to queue
    bool addWork(const Work& work);

    // Try to get a result, returns false if no result was placed in `result`
    bool retrieveOutput(Output& output);

    inline uint16_t worksCompleted() const { return m_completedWorkCount.load(); }

    inline uint64_t totalPixelCount() const { return m_totalQueuedPixels.load(); }

    inline uint64_t pixelsCompleted() const { return m_completedPixelsCount.load(); }

    inline void requestStop() { m_stopRequested.store(true); }

    inline void stopAndTryJoin() {
        requestStop();
        if (m_thread.joinable())
            m_thread.join();
    }

    inline bool running() const { return m_running.load(); }

private:
    // Render Context
    Camera m_cam;

    // Threading
    std::thread m_thread;
    std::atomic_bool m_running;
    std::atomic_bool m_stopRequested;

    // Work queuing
    std::mutex m_workQueueMutex;
    std::queue<Work> m_workQueue;

    // Outputs
    std::mutex m_outputQueueMutex;
    std::queue<Output> m_outputQueue;
    std::atomic_uint16_t m_completedWorkCount;

    // Metering
    std::atomic_uint64_t m_totalQueuedPixels;    // How many pixels in total will be worked on
    std::atomic_uint64_t m_completedPixelsCount; // How many pixels have been completed thus far

    // Main loop of the thread
    void threadMainLoop(const HittableList& world);

    // Called in the main loop, where rendering actually takes place
    std::vector<color> renderScanline(const Work& work, const HittableList& world);

    // Attempts to get a task from the work queue
    // return `true` if a task was pulled off.  It's then placed into `task`.
    // if `false`, then one wasn't pulled off.  `task` shouldn't be modified then
    bool getNextWork(Work& work);

    // Place the result of a render into a queue for the thread owner to receive
    bool storeOutput(const uint16_t scanline, const std::vector<color>& data);
};