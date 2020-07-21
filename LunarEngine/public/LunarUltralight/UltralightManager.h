#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Ultralight/Ultralight.h>
#include <Ultralight/platform/Logger.h>
#include <AppCore/Platform.h>
#include <JavaScriptCore/JavaScript.h>
#include <AppCore/JSHelpers.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <optional>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <array>

#include "../LunarVkDatatypes.h"
#include "../LunarImage.h"
#include "../LunarUser/LunarInputData.h"
#include "UltralightData.h"

typedef struct pixel
{
    char b;
    char g;
    char r;
} pixel_t;

namespace LunarGUI {

    class UltralightManager : public ultralight::Logger, public ultralight::LoadListener, public ultralight::ViewListener {
    public:
        UltralightManager();
        ~UltralightManager();

        void PassWindow(GLFWwindow* window);
        void PassResources(LunarRenderer::LunarLayerConstruction *construction);
        Layout* GetLayout(std::string layoutID);
        void DestroyLayout(Layout* targetLayout);
        void DestroyAllLayouts();

        //construct
        void ConstructView(std::string contents, glm::vec2 dims, glm::vec2 pos, std::string tagName="index");
        void ConstructLayout(std::string layoutID);
        void PrepareGeometry();

        //rebuild
        void ReAttachComponents(LunarRenderer::LunarLayerConstruction* construction);

        //dual purpose, used for rebuilding and building
        void BuildShader();

        //setter
        void SetLayout(std::string layout);

        virtual void LogMessage(ultralight::LogLevel log_level, const ultralight::String16& message) override;
        virtual void OnFinishLoading(ultralight::View* caller);
    private:
        ultralight::RefPtr<ultralight::Renderer> renderer;
        std::vector<ultralight::RefPtr<ultralight::View>> _viewList;
        std::vector<Layout> layouts;
        std::string currLayout;

        GLFWwindow* window;
        static int  windowWidth;
        static int  windowHeight;
        static bool dimsChanged;

        //temporary layout
        Layout _tmpLayout = {};
    private:
        //vulkan stuff
        VkExtent2D swapChainExtent;
        VkDevice device;
        VkCommandPool commandPool;
        VkQueue graphicsQueue;
        VkDescriptorSetLayout descriptorSetLayout;

        VkRenderPass renderPass;
        std::vector<VkImage> swapChainImages;
        VkPhysicalDevice physicalDevice;
        std::vector<VkDescriptorPool> descriptorPools;
        std::vector<VkFramebuffer> swapChainFrameBuffers;
    private:
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        void allocateBufferMemory(Layout& m_buf);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize Dsize);
        void createDescirptorSets(Layout& m_buf);

        void makeMaterial();
        //material helpers
        VkShaderModule createShaderModule(const std::vector<char>& code);
        std::vector<char> readFile(const std::string& filename);
    private:
        //material
        bool created = false;
        VkPipeline ultralightPipeline;
        VkPipelineLayout ultralightPipelineLayout;
    private:
        //view's image helpers
        void CreateTextureImage(Image imageData, LunarGUI::Pane& _pane);
        void CreateTextureImageView(LunarGUI::Pane& _pane);
        void CreateTextureImageSampler(LunarGUI::Pane& _pane);

        void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    public:
        //rendering commands
        void RenderViews(int frameIndex);
        void RecreateDims(int width, int height);
        void UpdateUBOS(int frameIndex);
        void FlushCommandBuffer(VkCommandBuffer& commandBuffer, int index);

        void RenderDT(float deltaTime, int frameIndex);
    private:
        float currAccumulator = 0;
        float renderTime = 1 / 120;

    public:
        void PassMouseInput(LunarInput::MousePos pos);
        void OnChangeCursor(ultralight::View* caller, ultralight::Cursor cursor);
        void UpdateKeyEvent();
        
        static bool updatePos;
        static LunarInput::MousePos _cPos;

        static bool updateKey;
        static bool secondKey;
        static ultralight::KeyEvent evt;
        static ultralight::KeyEvent sEvt;
    private:
        GLFWcursor* _cursor_ibeam = nullptr;
        GLFWcursor* _cursor_crosshair;
        GLFWcursor* _cursor_hand;
        GLFWcursor* _cursor_hresize;
        GLFWcursor* _cursor_vresize;
    private:
        virtual void OnDOMReady(ultralight::View* caller,
            uint64_t frame_id,
            bool is_main_frame,
            const ultralight::String& url) override;
    public:
        ultralight::JSValue LogMSG(const ultralight::JSObject& object, const ultralight::JSArgs& args);
    };
}