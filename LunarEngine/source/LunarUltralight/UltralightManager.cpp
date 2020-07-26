#include "../../public/LunarUltralight/UltralightManager.h"

namespace LunarGUI {
    using namespace ultralight;

    //static variables
    int UltralightManager::windowWidth = 1280;
    int UltralightManager::windowHeight = 720;
    bool UltralightManager::dimsChanged = false;

    UltralightManager::UltralightManager() {
        ultralight::Config config;
        config.device_scale = 1.0;
        config.resource_path = "./res/";
        config.use_gpu_renderer = false;
        

        ultralight::Platform::instance().set_config(config);
        ultralight::Platform::instance().set_font_loader(ultralight::GetPlatformFontLoader());
        ultralight::Platform::instance().set_file_system(ultralight::GetPlatformFileSystem("./html/"));
        ultralight::Platform::instance().set_logger(this);

        renderer = ultralight::Renderer::Create();

        std::cout << "ultralight manager starting" << std::endl;
    }

    UltralightManager::~UltralightManager() {
        //DestroyAllLayouts();
    }

    void UltralightManager::PassWindow(GLFWwindow* window) {
        this->window = window;
        
        _cursor_ibeam = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
        _cursor_crosshair = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
        _cursor_hand = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
        _cursor_hresize = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
        _cursor_vresize = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
    }

    void UltralightManager::SetLayout(std::string layout) {
        for (auto& _layout : layouts) {
            if (_layout.active) {
                _layout.active = false;
                for (auto& pane : _layout._panes) {
                    pane._view->get()->set_needs_paint(false);
                    pane._view->get()->Unfocus();

                    vkUnmapMemory(device, pane.uniformBuffersMemory[1]);
                    vkUnmapMemory(device, pane.uniformBuffersMemory[0]);
                }
            }
        }

        this->currLayout = layout;

        for (auto& _layout : layouts) {
            if (_layout.layoutID == layout) {
                _layout.active = true;
                for (auto& pane : _layout._panes) {
                    pane._view->get()->set_needs_paint(true);
                    pane._view->get()->Focus();

                    vkMapMemory(device, pane.uniformBuffersMemory[1], 0, sizeof(pane.ubo), 0, &pane.dataOne);
                    vkMapMemory(device, pane.uniformBuffersMemory[0], 0, sizeof(pane.ubo), 0, &pane.data);
                }
            }
        }
    }

    void UltralightManager::PassResources(LunarRenderer::LunarLayerConstruction *construction) {
        this->device = construction->device;
        this->swapChainExtent = construction->swapChainExtent;
        this->renderPass = construction->renderPass;
        this->swapChainImages = construction->swapChainImages;
        this->physicalDevice = construction->physicalDevice;
        this->commandPool = construction->commandPool;
        this->graphicsQueue = construction->graphicsQueue;
        this->swapChainFrameBuffers = construction->swapChainFrameBuffers;

        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.pImmutableSamplers = nullptr;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
            throw std::runtime_error("failed to create descriptor set layout");

        BuildShader();
    }

    void UltralightManager::ReAttachComponents(LunarRenderer::LunarLayerConstruction* construction) {
        this->device = construction->device;
        this->swapChainExtent = construction->swapChainExtent;
        this->renderPass = construction->renderPass;
        this->swapChainImages = construction->swapChainImages;
        this->physicalDevice = construction->physicalDevice;
        this->commandPool = construction->commandPool;
        this->graphicsQueue = construction->graphicsQueue;
        this->swapChainFrameBuffers = construction->swapChainFrameBuffers;

        BuildShader();
        RecreateDims(construction->width, construction->height);
    }

    void UltralightManager::PrepareGeometry() {
        std::vector<ULVertex> verts = {
            {{-0.5f, -0.5f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f}, {1.0f, 0.0f}},
            {{0.5f, 0.5f},  {1.0f, 1.0f}},
            {{-0.5f, 0.5f}, {0.0f, 1.0f}}
        };
    }

    //virtuals
    void UltralightManager::LogMessage(LogLevel log_level, const String16& message) {

    }   

    void UltralightManager::OnFinishLoading(ultralight::View* caller) {
        std::cout << "done" << std::endl;
    }

    Layout* UltralightManager::GetLayout(std::string layoutID) {
        for (auto& layout : layouts) {
            if (layout.layoutID == layoutID)
                return &layout;
        }
    }

    void UltralightManager::DestroyLayout(Layout* targetLayout) {
        if (!targetLayout->destroyed) {
            targetLayout->destroyed = true;
            vkDestroyBuffer(device, targetLayout->vertexBuffer, nullptr);
            vkFreeMemory(device, targetLayout->vertexBufferMemory, nullptr);

            vkDestroyBuffer(device, targetLayout->indexBuffer, nullptr);
            vkFreeMemory(device, targetLayout->indexBufferMemory, nullptr);

            for (auto _pane : targetLayout->_panes) {
                vkDestroySampler(device, _pane.textureSampler, nullptr);
                vkDestroyImageView(device, _pane.textureImageView, nullptr);

                vkDestroyImage(device, _pane.textureImage, nullptr);
                vkFreeMemory(device, _pane.textureImageMemory, nullptr);
            }
        }
    }

    void UltralightManager::DestroyAllLayouts() {
        for (auto layout : layouts) {
            DestroyLayout(&layout);
        }
    }
}