#pragma once

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

    // explicit RenderThread(const RenderContext &render_context);
    explicit RenderThread(RenderThread&& other);

    bool run();

    // Queue a task to turn, returns false if failed to queue
    bool addWork(const Work& work);

    // Try to get a result, returns false if no result was placed in `result`
    bool retriveOutput(Output& output);

private:
    /*== Data ==*/
    // RenderContext _r_ctx;

    // Threading
    std::thread m_thread;
    std::atomic_bool m_running;

    // Work queuing
    std::mutex m_work_queue_mutex;
    std::queue<Work> m_work_queue;

    // Outputs
    std::mutex m_output_queue_mutex;
    std::queue<Output> m_output_queue;
    std::atomic_uint16_t m_completed_work_count;

    // Metering
    std::atomic_uint64_t m_total_queued_pixels; // How many pixels in total will be worked on
    std::atomic_uint64_t m_pixels_completed;    // How many pixels have been completed thus far

    // == Thread functions
    void threadMainLoop(); // Main loop of the thread
    std::vector<color>
    renderScanline(const Work& work); // Called in the main loop, where rendering actually takes place

    // Attempts to get a task from the work queue
    // return `true` if a task was pulled off.  It's then placed into `task`.
    // if `false`, then one wasn't pulled off.  `task` shouldn't be modified then
    bool getNextTask(Work& work);

    // Place the result of a render into a queue for the thread owner to receive
    bool storeResult(const uint16_t scanline, const std::vector<color>& data);
};