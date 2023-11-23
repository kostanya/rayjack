#include "render_thread.h"
#include "camera.h"
#include "hit_record.h"
#include "hittable.h"
#include "material.h"
#include "ray.h"

RenderThread::RenderThread(const Camera& cam) : m_cam(cam) {
    m_running.store(false);
    m_stopRequested.store(false);
    m_completedWorkCount.store(0);
    m_totalQueuedPixels.store(0);
    m_completedPixelsCount.store(0);
}

RenderThread::RenderThread(RenderThread&& other)
    : m_cam(other.m_cam), m_thread(move(other.m_thread)), m_workQueue(move(other.m_workQueue)),
      m_outputQueue(move(other.m_outputQueue)) {
    m_running.store(other.m_running.load());
    m_stopRequested.store(other.m_stopRequested.load());
    m_completedWorkCount.store(other.m_completedWorkCount.load());
    m_totalQueuedPixels.store(other.m_totalQueuedPixels.load());
    m_completedPixelsCount.store(other.m_completedPixelsCount.load());
}

// Returns true if the thread was successfuly created and starts running
bool RenderThread::run(const HittableList& world) {
    try {
        // Create the thread object
        m_thread = std::thread(&RenderThread::threadMainLoop, this, world);
    } catch (...) {
        return false;
    }

    return true;
}

bool RenderThread::addWork(const Work& work) {
    try {
        // ayni workun farkli threadler tarafindan pushlanmamasi icin
        // ayrica lock_guard blocktan cikinca otomatik olarak locku salio
        {
            // Get the work
            std::lock_guard guard(m_workQueueMutex);
            m_workQueue.push(work);
        }

        // Store how many pixels need to be completed in total
        m_totalQueuedPixels.store(m_totalQueuedPixels.load() + m_cam.imageWidth);

        return true;
    } catch (...) {
        return false;
    }
}

// WE MUST RETRIEVE OUTPUTS SYNCHRONOUSLY
// OTHERWISE 'output' can be overriden by other threads
bool RenderThread::retrieveOutput(Output& output) {
    std::lock_guard guard(m_outputQueueMutex);

    // Nothing?
    if (m_outputQueue.empty())
        return false;

    // Else, got something, grab it and place it
    output = m_outputQueue.front();
    m_outputQueue.pop();
    return true;
}

void RenderThread::threadMainLoop(const HittableList& world) {
    m_running.store(true);
    m_stopRequested.store(false);

    Work work;
    bool gotWork = false;

    while (m_stopRequested.load() != true) {
        try {

            // Do render here
            gotWork = getNextWork(work);
            if (gotWork)
                storeOutput(work.scanline, renderScanline(work, world));
            else
                std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Take a short nap

        } catch (...) {
            // No errors leave thread
        }
    }

    m_running.store(false);
}

std::vector<color> RenderThread::renderScanline(const Work& work, const HittableList& world) {
    std::vector<color> scanline(m_cam.imageWidth);

    for (uint16_t i = 0; i < m_cam.imageWidth; ++i) {
        // Put the pixel into the scanline
        scanline[i] = m_cam.pixelColor(i, work.scanline, world);

        // Metering
        m_completedPixelsCount.store(m_completedPixelsCount.load() + 1);
    }

    return scanline;
}

bool RenderThread::getNextWork(Work& work) {
    std::lock_guard guard(m_workQueueMutex);

    // Nothing?
    if (m_workQueue.empty())
        return false;

    // There's something, grab it and place it
    work = m_workQueue.front();
    m_workQueue.pop();
    return true;
}

bool RenderThread::storeOutput(const uint16_t scanline, const std::vector<color>& data) {
    try {
        const uint16_t completedWorkNow = m_completedWorkCount.load() + 1;
        Output output;
        output.scanline = scanline;
        output.data = data;

        // Put it onto the output queue (and notify)
        std::lock_guard guard(m_outputQueueMutex);
        m_outputQueue.push(output);
        m_completedWorkCount.store(completedWorkNow);

        return true;
    } catch (...) {
        return false;
    }
}
