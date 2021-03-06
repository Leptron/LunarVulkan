#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <optional>
#include <set>
#include <algorithm>
#include <fstream>
#include <glm/glm.hpp>
#include <array>
#include <tuple>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <cstdint>

#include "LunarVkDatatypes.h"
#include "LunarBuffer.hpp"
#include "LunarLayerManager.h"

namespace LunarRenderer {
    const int MAX_FRAMES_IN_FLIGHT = 2;

#if defined(__ANDROID__)
#define VK_CHECK_RESULT(f)																				\
{																										\
	VkResult res = (f);																					\
	if (res != VK_SUCCESS)																				\
	{																									\
		LOGE("Fatal : VkResult is \" %s \" in %s at line %d", vks::tools::errorString(res).c_str(), __FILE__, __LINE__); \
		assert(res == VK_SUCCESS);																		\
	}																									\
}
#else
#define VK_CHECK_RESULT(f)																				\
{																										\
	VkResult res = (f);																					\
	if (res != VK_SUCCESS)																				\
	{																									\
		std::cout << "Fatal : VkResult is \"" << "not good" << "\" in " << __FILE__ << " at line " << __LINE__ << std::endl; \
		assert(res == VK_SUCCESS);																		\
	}																									\
}
#endif


    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    struct QueueFamilyIndicies {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct EngineRelinquish {
        VkDevice device;
        GLFWwindow* window;
    };
    
    class LunarRenderer {
    public:
        LunarRenderer();
        ~LunarRenderer();

        // Initialization
        void InitResources();
        void MainLoop();

        void AttachManager(LayerManager* manager);

        void LayerLoop();
        EngineRelinquish RelinquishEngine();

        bool framebufferResized = false;

        //layer prototypes
        LunarLayerConstruction globConstruct();
        void CreateCommandBuffers();
        int drawFrame();
        void cleanup();

        //static bool recreateGUI;
    private:
        LunarGUI::UltralightManager* _guiManager;

        //create the window
        void initWindow();

        //initialize vulkan
        void initVulkan();
        //create instance
        void createInstance();
        //validaton and debugging layer funcs
        bool checkValidationLayerSupport();
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        std::vector<const char*> getRequiredExtensions();
        void setupDebugMessenger();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        //physical device + logical device
        void pickPhysicalDevice();
        bool isDeviceSuitable(VkPhysicalDevice device);
        QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice device);
        
        void createLogicalDevice();

        //presentation
        void createSurface();
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        void createSwapChain();
        void createImageViews();

        //graphics pipeline (eventual layer implementation)
        void createRenderPass();


        //drawing
        void createFrameBuffer();
        void createCommandPool();
        
        
        void createSyncObjects();

        //recreate swap chain
        void recreateSwapChain();
        void cleanupSwapChain();

        //variables and handles
        //drawing
        std::vector<VkFramebuffer> swapChainFrameBuffers;
        std::vector<VkCommandBuffer> commandBuffers;
        VkCommandPool commandPool;

        //semaphores
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;
        size_t currentFrame = 0;

        std::vector<GeometryLayer> layers;
        

        //pipeline
        VkPipeline graphicsPipeline;
        VkRenderPass renderPass;
        VkPipelineLayout pipelineLayout;
        //surface + presentation
        VkSurfaceKHR surface;
        VkQueue presentQueue;
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
        //Queue
        VkQueue graphicsQueue;
        //physical device
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;
        //validation layers
        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        //extensions
        const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        #ifdef NDEBUG
            const bool enableValidationLayers = true;
        #else
            const bool enableValidationLayers = true;
        #endif

        VkDebugUtilsMessengerEXT debugMessenger;
        //GLFW window and width / height
        GLFWwindow* window;
        uint32_t width = 1280;
        uint32_t height = 720;

        //vulkan handler
        //vulkan instance
        VkInstance instance;

        LayerManager* manager;

    //LAYER STUFF
    public:
        void CreateMaterial(std::string mName, int target);

        int AddGeometry(Geometry addGeometry);
        int FlushGeometry();
        void FlushToCommandBuffer();
    private:
        void createVertexBuffer(std::vector<Vertex> vertices, GeometryLayer& geoLayer);
        void createIndexBuffer(std::vector<uint32_t> indices, GeometryLayer& geoLayer);
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        //layer helpers
        std::vector<Geometry> tmpGeometryBuffer;
        void cleanAllLayers();

        std::vector<VkCommandBuffer> layerCommandBuffer;

        //command buffer
        VkCommandBuffer getLayerCommandBuffer(bool begin);
        void layerFlushCommandBuffer(VkCommandBuffer commandBuffer);

        int geomIndex = 0;
        int currGeomLayer = 0;

        VkPhysicalDeviceMemoryProperties deviceMemoryProperties; 

        //material
        void cleanMaterials();
    private:
        //graphics pipeline for layers
        std::vector<LunarMaterial> materialIndex;
        std::vector<std::tuple<std::string, int>> materialNames;

    public:
    private:
        
        void createDescriptorSetLayout();
        void createUniformBuffers();
        void updateUniformBuffer(uint32_t currentImage);
        void createDescriptorPool();
        void createDescriptorSets();

        int totalGeom = 0;

        VkDescriptorSetLayout descriptorSetLayout;
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
    private:
        //gui stuff
        LunarGUI::UltralightManager* _manager;
    };
}