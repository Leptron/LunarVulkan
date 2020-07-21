#include "../../public/LunarUltralight/UltralightManager.h"

namespace LunarGUI {
    void UltralightManager::ConstructView(std::string contents, glm::vec2 dims, glm::vec2 pos, std::string tagName) {
        
        std::vector<ULVertex> verts = {
            {{-0.5f, -0.5f}, {0.0f, 1.0f}},
            {{0.5f, -0.5f}, {1.0f, 1.0f}},
            {{0.5f, 0.5f},  {1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f}}
        };

        std::vector<uint32_t> indices = {
            0, 1, 2, 2, 3, 0
        };

        glm::mat4 model;
        glm::mat4 projection;

        model = glm::translate(model, glm::vec3(pos, 0.0f));
        model = glm::translate(model, glm::vec3(0.5f * dims.x, 0.5f * dims.y, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * dims.x, -0.5f * dims.y, 0.0f));
        model = glm::scale(model, glm::vec3(dims, 1.0f));

        projection = glm::ortho(0.0f, static_cast<float>(windowWidth), static_cast<float>(windowHeight), 0.0f, -1.0f, 1.0f);

        Pane _pane = {};
        _pane.verts = verts;
        _pane.x = pos.x;
        _pane.y = pos.y;
        _pane.paneID = tagName;

        _pane.verts = verts;
        _pane.indices = indices;
        _pane.indexCount = static_cast<uint32_t>(indices.size());

        ultralight::RefPtr<ultralight::View> _tmpView;

        _tmpView = renderer->CreateView(dims.x, dims.y, false, nullptr);
        _tmpView->set_needs_paint(true);
        _tmpView->set_view_listener(this);
        _tmpView->set_load_listener(this);
        _tmpView->LoadHTML(contents.c_str());


        renderer->Update();
        renderer->Render();

        //get pixels
        ultralight::BitmapSurface* bitmap_surface = (ultralight::BitmapSurface*)_tmpView->surface();
        //getting the bitmmap
        ultralight::RefPtr<ultralight::Bitmap> bmp = bitmap_surface->bitmap();
        bmp->SwapRedBlueChannels();

        void* pixels = bmp->LockPixels();

        Image _image = {};
        _image.pixels = pixels;
        _image.height = static_cast<int>(bmp->height());
        _image.width = static_cast<int>(bmp->width());
        _image.size = bmp->size();

        std::cout << sizeof(pixels) << std::endl;

        CreateTextureImage(_image, _pane);

        VkDeviceSize imageSize = bmp->size();
        createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _pane.imageStagingBuffer, _pane.imageStagingBufferMemory);


        bmp->UnlockPixels();
        _tmpView->set_needs_paint(false);

        _viewList.push_back(_tmpView);

        _pane._view = &_viewList[_viewList.size() - 1];
        _pane.ubo = {
            model, projection
        };

        int xPos = pos.x;
        int fxPos = xPos;

        int xDiv = dims.x / 2;

        xPos -= xDiv;
        fxPos += xDiv;

        int yPos = pos.y;
        int fyPos = yPos;

        int yDiv = dims.y / 2;

        yPos -= yDiv;
        fyPos += yDiv;

        _pane.tX = xPos;
        _pane.tY = yPos;
        _pane.fX = fxPos;
        _pane.fY = fyPos;

        //create the uniform buffers
        VkDeviceSize bufferSize = sizeof(ULUBO);

        _pane.uniformBuffers.resize(swapChainImages.size());
        _pane.uniformBuffersMemory.resize(swapChainImages.size());

        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _pane.uniformBuffers[0], _pane.uniformBuffersMemory[0]);
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _pane.uniformBuffers[1], _pane.uniformBuffersMemory[1]);

        _tmpLayout._panes.push_back(_pane);

        std::cout << "created a pane" << std::endl;
    }

    void UltralightManager::ConstructLayout(std::string layoutID) {
        Layout _layout = {};
        _layout.destroyed = false;
        int indexBase = 0;

        for (auto& pane : _tmpLayout._panes) {
            pane.indexBase = indexBase;
            _layout._panes.push_back(pane);
            indexBase += pane.indexCount;
        }

        _tmpLayout._panes.clear();
        _tmpLayout = {};

        VkDescriptorPool descriptorPool;

        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = 2;
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = 2;

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size());

        if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
            throw std::runtime_error("Failed to create a descriptor pool");

        descriptorPools.push_back(descriptorPool);

        _layout.poolIndex = descriptorPools.size() - 1;
        _layout.descriptorPool = descriptorPool;
        _layout.layoutID = layoutID;

        allocateBufferMemory(_layout);
        createDescirptorSets(_layout);
        layouts.push_back(_layout);

        std::cout << "created a layout" << std::endl;
    }
}