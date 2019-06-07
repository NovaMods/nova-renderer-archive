#pragma once
#include "nova_renderer/swapchain.hpp"

namespace nova::renderer::rhi {
    class Gl3Swapchain final : public Swapchain {
    public:
        explicit Gl3Swapchain(uint32_t num_swapchain_images);

        ~Gl3Swapchain() override = default;

        uint32_t acquire_next_swapchain_image(Semaphore* signal_semaphore) override;

        void present(uint32_t image_idx, const std::vector<Semaphore*> wait_semaphores) override;

    private:
        const uint32_t num_frames;

        uint32_t cur_frame = 0;
    };
}
