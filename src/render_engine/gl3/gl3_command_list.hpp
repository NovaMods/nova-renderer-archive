/*!
 * \author ddubois
 * \date 31-Mar-19.
 */

#ifndef NOVA_RENDERER_GL_2_COMMAND_LIST_HPP
#define NOVA_RENDERER_GL_2_COMMAND_LIST_HPP
#include <nova_renderer/command_list.hpp>

#include "glad/glad.h"

namespace nova::renderer::rhi {
    enum class Gl3CommandType {
        BufferCopy,
        ExecuteCommandLists,
        BeginRenderpass,
        EndRenderpass,
        BindPipeline,
        BindMaterial,
        BindVertexBuffers,
        BindIndexBuffer,
        DrawIndexedMesh,
    };

    struct Gl3BufferCopyCommand {
        GLuint destination_buffer;
        uint64_t destination_offset;

        GLuint source_buffer;
        uint64_t source_offset;

        uint64_t num_bytes;
    };

    struct Gl3ExecuteCommandListsCommand {
        std::vector<CommandList*> lists_to_execute;
    };

    struct Gl3BeginRenderpassCommand {
        GLuint framebuffer;
    };

    struct Gl3BindPipelineCommand {};

    struct Gl3BindMaterialCommand {};

    struct Gl3BindVertexBuffersCommand {};

    struct Gl3BindIndexBufferCommand {};

    struct Gl3DrawIndexedMeshCommand {};

    struct Gl3Command {
        Gl3CommandType type;

        union {
            Gl3BufferCopyCommand buffer_copy;
            Gl3ExecuteCommandListsCommand execute_command_lists;
            Gl3BeginRenderpassCommand begin_renderpass;
            Gl3BindPipelineCommand bind_pipeline;
            Gl3BindMaterialCommand bind_material;
            Gl3BindVertexBuffersCommand bind_vertex_buffers;
            Gl3BindIndexBufferCommand bind_index_buffer;
            Gl3DrawIndexedMeshCommand draw_indexed_mesh;
        };

        ~Gl3Command();
    };

    /*!
     * \brief Command list implementation for OpenGL 3.1
     *
     * This class is fun because OpenGL has no notion of a command list - it's synchronous af. Thus, this command list
     * is a custom thing that records commands into host memory. When this command list is submitted to a "queue", Nova
     * runs the OpenGL commands. This lets command lists be recorded from multiple threads, but submitting a command
     * list to OpenGL is _way_ more expensive than submitting a DirectX or Vulkan command list
     *
     * OpenGL also has a really fun property where it can't be multithreaded, especially since Nova has to use OpenGL
     * 2.1. The OpenGL render engine will chew through OpenGL commands in a separate thread, and in that way mimic the
     * async nature of modern APIs, but still... it'll be rough
     *
     * On the other hand, OpenGL has no concept of a resource barrier...
     */
    class Gl3CommandList final : public CommandList {
    public:
        Gl3CommandList();

		Gl3CommandList(Gl3CommandList&& old) noexcept = default;
		Gl3CommandList& operator=(Gl3CommandList&& old) noexcept = default;

		Gl3CommandList(const Gl3CommandList& other) = delete;
		Gl3CommandList& operator=(const Gl3CommandList& other) = delete;

        void resource_barriers([[maybe_unused]] PipelineStageFlags stages_before_barrier,
                              [[maybe_unused]] PipelineStageFlags stages_after_barrier,
                              [[maybe_unused]] const std::vector<ResourceBarrier>& barriers) override final;

        void copy_buffer(Buffer* destination_buffer,
                         uint64_t destination_offset,
                         Buffer* source_buffer,
                         uint64_t source_offset,
                         uint64_t num_bytes) override final;

        void execute_command_lists(const std::vector<CommandList*>& lists) override final;

        void begin_renderpass([[maybe_unused]] Renderpass* renderpass, Framebuffer* framebuffer) override final;

        void end_renderpass() override final;

        void bind_pipeline() override final;

        void bind_material() override final;

        void bind_vertex_buffers() override final;

        void bind_index_buffer() override final;

        void draw_indexed_mesh() override final;

        ~Gl3CommandList() override final;

        /*!
         * \brief Provides access to the actual command list, so that the GL2 render engine can process the commands
         */
        std::vector<Gl3Command> get_commands() const;

    private:
        std::vector<Gl3Command> commands;
    };
} // namespace nova::renderer

#endif // NOVA_RENDERER_GL_2_COMMAND_LIST_HPP