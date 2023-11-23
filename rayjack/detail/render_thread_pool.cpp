#include "render_thread_pool.h"
#include "color.h"
#include "render_thread.h"
#include <stdexcept>

RenderThreadPool::RenderThreadPool(const Camera& cam, const int numThreads) : m_cam(cam) {
    // Create the threads
    for (int i = 0; i < numThreads; i++)
        m_threads.push_back(RenderThread(cam));
}

void RenderThreadPool::setupRender() {
    // Set which threads will render which scanlines
    RenderThread::Work work;

    int row = m_cam.imageHeight - 1;
    bool added = false;

    while (row >= 0) {
        // Setup the task
        work.scanline = row--;

        // Pick which thread to add it to
        added = m_threads[row % m_threads.size()].addWork(work);
        if (!added)
            throw std::runtime_error("Can't add work");
    }
}

void RenderThreadPool::startRender(const HittableList& world) {
    // Start all threads
    bool started = false;
    for (RenderThread& thread : m_threads) {
        started = thread.run(world);
        if (!started)
            throw std::runtime_error("Can't start render thread");
    }
}

uint64_t RenderThreadPool::totalPixelCount() const {
    uint64_t total = 0;
    for (const RenderThread& thread : m_threads)
        total += thread.totalPixelCount();

    return total;
}

uint64_t RenderThreadPool::pixelsCompleted() const {
    uint64_t completed = 0;
    for (const RenderThread& thread : m_threads)
        completed += thread.pixelsCompleted();

    return completed;
}

// Checks if the render is done by looking at the task counts
bool RenderThreadPool::renderCompleted() const {
    uint16_t works = 0;
    for (const RenderThread& thread : m_threads)
        works += thread.worksCompleted();

    // the total number of tasks is equal to the height of the render
    return (works >= m_cam.imageHeight);
}

void RenderThreadPool::terminateThreads() {
    // Cleanly tell the threads to shutdown and wait for them all
    for (RenderThread& thread : m_threads)
        thread.requestStop();

    for (RenderThread& thread : m_threads)
        thread.stopAndTryJoin();
}

// Write the outputs to a byte buffer
void RenderThreadPool::retrieveRender(uint8_t* imageData) {
    RenderThread::Output output;
    bool gotScanline = false;

    // Loop through all the threads
    for (RenderThread& thread : m_threads) {
        // And loop through all of their output queues
        gotScanline = thread.retrieveOutput(output);
        while (gotScanline) {
            writeScanline(imageData, output.data, 3 * output.scanline * m_cam.imageWidth);
            gotScanline = thread.retrieveOutput(output);
        }
    }
}