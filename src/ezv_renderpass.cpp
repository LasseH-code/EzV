#include "ezv_base.h"

namespace ezv
{
	bool EzV::createRenderPass()
	{
		VulkanRenderpass* temp = new VulkanRenderpass();

		VkAttachmentDescription attachmentDescription = {};
		attachmentDescription.format = swapchain->format; 
		attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
		attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // normally CLEAR
		attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		VkAttachmentReference attachmentReference = {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; // For rn; COMPUTE is also an option f.e.
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &attachmentReference;

		VkRenderPassCreateInfo createInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
		createInfo.attachmentCount = 1;
		createInfo.pAttachments = &attachmentDescription;
		createInfo.subpassCount = 1;
		createInfo.pSubpasses = &subpass;
		VKA(vkCreateRenderPass(context->logicalDevice, &createInfo, 0, &temp->renderpass));
		renderpasses.push_back(*temp);
		temp = 0;
		return true;
	}

	void EzV::destroyRenderPass()
	{
		//VulkanRenderpass& temp = renderpasses.data();
		if (renderpasses.size() > 0)
		{
			while (renderpasses.size() > 0)
			{
				VK(vkDestroyRenderPass(context->logicalDevice, renderpasses.back().renderpass, 0));
				renderpasses.pop_back();
			}
			LOG_WARN("Renderpass destroyed successfully!");
		}
	}
}