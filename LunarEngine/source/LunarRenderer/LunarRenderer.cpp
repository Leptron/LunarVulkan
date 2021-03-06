#include "../../public/LunarRenderer.h"

namespace LunarRenderer {
    //bool LunarRenderer::recreateGUI = false;

    LunarRenderer::LunarRenderer() {

    }

    LunarRenderer::~LunarRenderer() {

    }

    void LunarRenderer::InitResources() {
        initWindow();

        initVulkan();
    }

    void LunarRenderer::recreateSwapChain() {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(device);

        cleanupSwapChain();

        createSwapChain();
        createImageViews();
        createRenderPass();
        
        createFrameBuffer();

        //manager components
        manager->ReAttachComponents(&globConstruct());
        manager->RebuildMaterials(&renderPass);
        manager->RecreateUBO();
        manager->FlushCommandBuffer(commandBuffers);
    }

    void LunarRenderer::AttachManager(LayerManager* manager) {
        this->manager = manager;
    }

    void LunarRenderer::cleanupSwapChain() {
        for (size_t i = 0; i < this->swapChainFrameBuffers.size(); i++) {
            vkDestroyFramebuffer(device, this->swapChainFrameBuffers[i], nullptr);
        }

        vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

        manager->CleanMaterials();
        vkDestroyRenderPass(device, renderPass, nullptr);

        for (size_t i = 0; i < swapChainImageViews.size(); i++) {
            vkDestroyImageView(device, swapChainImageViews[i], nullptr);
        }

        vkDestroySwapchainKHR(device, swapChain, nullptr);

        manager->CleanAllUBO();
    }

    LunarLayerConstruction LunarRenderer::globConstruct() {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);

        LunarLayerConstruction construct = {};

        construct.device = device;
        construct.physicalDevice = physicalDevice;
        construct.renderPass = renderPass;
        construct.swapChainExtent = swapChainExtent;
        construct.swapChainImages = swapChainImages;
        construct.commandPool = commandPool;
        construct.graphicsQueue = graphicsQueue;
        construct.swapChainFrameBuffers = swapChainFrameBuffers;
        construct.width = width;
        construct.height = height;

        return construct;
    }

    void LunarRenderer::cleanup() {
        //call all the various sub cleaning functions
        cleanupSwapChain();
        //semaphores
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(device, inFlightFences[i], nullptr);
        }

        manager->CleanAllStorageBuffers();

        //command pool
        vkDestroyCommandPool(device, commandPool, nullptr);
        //cleanup device
        vkDestroyDevice(device, nullptr);
        //destroy debugging
        if(enableValidationLayers)
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        //destroy surface
        vkDestroySurfaceKHR(instance, surface, nullptr);
        //destroy instance
        vkDestroyInstance(instance, nullptr);
        //destroy window
        glfwDestroyWindow(window);

        //terminate glfw
        glfwTerminate();
    }

    void LunarRenderer::createRenderPass() {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = swapChainImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if(vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
            throw std::runtime_error("failed to create render pass!");
    }
}