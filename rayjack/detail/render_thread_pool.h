#pragma once

#include "camera.h"
#include "hittable_list.h"
#include "render_thread.h"
#include <vector>

class RenderThreadPool {

private:
    // Render Context
    Camera m_cam;
    std::vector<RenderThread> m_threads;

public:
    explicit RenderThreadPool(const Camera& cam, const int numThreads);

    void setupRender();
    void startRender(const HittableList& world);
    void terminateThreads();

    uint64_t totalPixelCount() const;
    uint64_t pixelsCompleted() const;
    bool renderCompleted() const;
    void retrieveRender(uint8_t* imageData);
};
