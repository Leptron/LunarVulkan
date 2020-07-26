#include "../../public/LunarUltralight/UltralightManager.h"

namespace LunarGUI {
	void UltralightManager::RenderViews(int frameIndex) {
		renderer->Update();
		renderer->Render();

		for (auto& layout : layouts) {
			if (layout.active) {
				for (auto& pane : layout._panes) {
						vkDeviceWaitIdle(device);
						ultralight::BitmapSurface* bitmap_surface = (ultralight::BitmapSurface*)pane._view->get()->surface();
						ultralight::RefPtr<ultralight::Bitmap> bmp = bitmap_surface->bitmap();
						//bmp->SwapRedBlueChannels();	

						void* pixels = bmp->LockPixels();

						VkDeviceSize imageSize = bmp->size();

						void* data;
						vkMapMemory(device, pane.imageStagingBufferMemory, 0, imageSize, 0, &data);
						memcpy(data, pixels, static_cast<uint32_t>(imageSize));
						vkUnmapMemory(device, pane.imageStagingBufferMemory);

						transitionImageLayout(pane.textureImage, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
						CopyBufferToImage(pane.imageStagingBuffer, pane.textureImage, static_cast<uint32_t>(bmp->width()), static_cast<uint32_t>(bmp->height()));
						transitionImageLayout(pane.textureImage, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

						bmp->UnlockPixels();
				}
			}
		}

		UpdateUBOS(frameIndex);
	}

	void UltralightManager::RecreateDims(int width, int height) {
		glm::mat4 projection = glm::ortho(0.0f, (float)swapChainExtent.width, (float)swapChainExtent.height, 0.0f, 0.1f, 100.0f);

		for (auto layout : layouts) {
			for (auto pane : layout._panes) {
				pane.ubo.projection = projection;
			}
		}

		UpdateUBOS(0);
	}

	void UltralightManager::UpdateUBOS(int frameIndex) {
		vkDeviceWaitIdle(device);
		for (auto& layout : layouts) {
			if (layout.active) {
				for (auto& pane : layout._panes) {
					memcpy(pane.data, &pane.ubo, sizeof(pane.ubo));
					memcpy(pane.dataOne, &pane.ubo, sizeof(pane.ubo));
				}
			}
		}
	}

	void UltralightManager::FlushCommandBuffer(VkCommandBuffer& commandBuffer, int index) {

		for (auto layout : layouts) {
			if (layout.active) {
				VkDeviceSize offsets[] = { 0 };

				vkCmdBindVertexBuffers(commandBuffer, 0, 1, &layout.vertexBuffer, offsets);
				vkCmdBindIndexBuffer(commandBuffer, layout.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
				for (auto pane : layout._panes) {
					VkPipeline pipe = ultralightPipeline;
					VkPipelineLayout layout = ultralightPipelineLayout;

					vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipe);
					vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &pane.descriptorSets[index], 0, nullptr);
					vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(pane.indices.size()), 1, 0, pane.indexBase, 0);
				}
			}
		}
	}

	void UltralightManager::RenderDT(float deltaTime, int frameIndex) {
		currAccumulator += deltaTime;
		UpdateKeyEvent();
		
		if (currAccumulator >= renderTime) {
			PassMouseInput(_cPos);

			currAccumulator = 0;

			RenderViews(frameIndex);
			updatePos = false;
		}
	}
}