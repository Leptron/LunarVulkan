#include "../../public/LunarUltralight/UltralightManager.h"

namespace LunarGUI {
    void UltralightManager::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
            throw std::runtime_error("Failed to create a buffer");

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
            throw std::runtime_error("failed to allocate buffer memory!");

        vkBindBufferMemory(device, buffer, bufferMemory, 0);
    }

    uint32_t UltralightManager::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;

        throw std::runtime_error("Failed to find a memory type");
    }

    void UltralightManager::allocateBufferMemory(Layout& m_buf) {
        std::vector<ULVertex> vertices;
        std::vector<uint32_t> indices;

        for (auto pane : m_buf._panes) {
            for (auto vert : pane.verts)
                vertices.push_back(vert);
            for (auto index : pane.indices)
                indices.push_back(index);
        }

        m_buf.vertices = vertices;
        m_buf.indices = indices;

        VkDeviceSize vertexBufferSize = sizeof(vertices[0]) * vertices.size();
        VkBuffer vertexStagingBuffer;
        VkDeviceMemory vertexStagingBufferMemory;

        createBuffer(vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexStagingBuffer, vertexStagingBufferMemory);

        void* vData;
        vkMapMemory(device, vertexStagingBufferMemory, 0, vertexBufferSize, 0, &vData);
            memcpy(vData, vertices.data(), (size_t)vertexBufferSize);
        vkUnmapMemory(device, vertexStagingBufferMemory);

        createBuffer(vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, m_buf.vertexBuffer, m_buf.vertexBufferMemory);
        copyBuffer(vertexStagingBuffer, m_buf.vertexBuffer, vertexBufferSize);

        vkDestroyBuffer(device, vertexStagingBuffer, nullptr);
        vkFreeMemory(device, vertexStagingBufferMemory, nullptr);

        VkDeviceSize indexBufferSize = sizeof(indices[0]) * indices.size();

        VkBuffer indexStagingBuffer;
        VkDeviceMemory indexStagingBufferMemory;
        createBuffer(indexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, indexStagingBuffer, indexStagingBufferMemory);

        void* iData;
        vkMapMemory(device, indexStagingBufferMemory, 0, indexBufferSize, 0, &iData);
            memcpy(iData, indices.data(), (size_t)indexBufferSize);
        vkUnmapMemory(device, indexStagingBufferMemory);

        createBuffer(indexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_buf.indexBuffer, m_buf.indexBufferMemory);
        copyBuffer(indexStagingBuffer, m_buf.indexBuffer, indexBufferSize);

        vkDestroyBuffer(device, indexStagingBuffer, nullptr);
        vkFreeMemory(device, indexStagingBufferMemory, nullptr);
    }

    void UltralightManager::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize Dsize) {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

            VkBufferCopy copyRegion = {};
            copyRegion.size = Dsize;
            vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        endSingleTimeCommands(commandBuffer);
    }

    void UltralightManager::createDescirptorSets(Layout& m_buf) {
        for (auto& pane : m_buf._panes) {
            std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), descriptorSetLayout);
            VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{};
            descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            descriptorSetAllocateInfo.descriptorPool = m_buf.descriptorPool;
            descriptorSetAllocateInfo.pSetLayouts = layouts.data();
            descriptorSetAllocateInfo.descriptorSetCount = 2;

            pane.descriptorSets.resize(swapChainImages.size());
            if (vkAllocateDescriptorSets(device, &descriptorSetAllocateInfo, pane.descriptorSets.data()) != VK_SUCCESS)
                throw std::runtime_error("failed to create a descriptor set");

            for (size_t i = 0; i < swapChainImages.size(); i++) {
                VkDescriptorBufferInfo bufferInfo = {};
                bufferInfo.buffer = pane.uniformBuffers[i];
                bufferInfo.offset = 0;
                bufferInfo.range = sizeof(ULUBO);

                VkDescriptorImageInfo imageInfo = {};
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = pane.textureImageView;
                imageInfo.sampler = pane.textureSampler;

                std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

                descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[0].dstSet = pane.descriptorSets[i];
                descriptorWrites[0].dstBinding = 0;
                descriptorWrites[0].dstArrayElement = 0;
                descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrites[0].descriptorCount = 1;
                descriptorWrites[0].pBufferInfo = &bufferInfo;

                descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[1].dstSet = pane.descriptorSets[i];
                descriptorWrites[1].dstBinding = 1;
                descriptorWrites[1].dstArrayElement = 0;
                descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                descriptorWrites[1].descriptorCount = 1;
                descriptorWrites[1].pImageInfo = &imageInfo;


                vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
            }
        }
    }
}