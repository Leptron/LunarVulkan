#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Ultralight/Ultralight.h>

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

namespace LunarGUI {
    struct ULVertex {
        glm::vec2 pos;
        glm::vec2 uv;

        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(ULVertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(ULVertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(ULVertex, uv);

            return attributeDescriptions;
        }
    };

    struct ULUBO {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 projection;
    };

    struct Pane {
        int paneWidth, paneHeight; //set
        int x, y; //set

        int tX, tY;
        int fX, fY;

        std::string paneID; //set
        ultralight::RefPtr<ultralight::View>* _view;  //set

        std::vector<ULVertex> verts; // set
        std::vector<uint32_t> indices; // set
        uint32_t indexCount, indexBase; // set

        std::vector<VkBuffer> uniformBuffers; // set
        std::vector<VkDeviceMemory> uniformBuffersMemory; // set
        std::vector<VkDescriptorSet> descriptorSets; // set

        //projections
        ULUBO ubo; // set
        void* data;
        void* dataOne;

        //texture
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
        VkSampler textureSampler;

        //image buffer
        VkBuffer imageStagingBuffer;
        VkDeviceMemory imageStagingBufferMemory;
    };

    struct Layout {
        bool destroyed;
        bool active = false;

        std::vector<Pane> _panes;

        std::vector<ULVertex> vertices;
        std::vector<uint32_t> indices;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;

        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;

        int poolIndex; //figure it out
        std::string layoutID;

        //descriptor pool
        VkDescriptorPool descriptorPool;
    };
}