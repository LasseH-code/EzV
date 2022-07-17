#include "ezv_base.h"

namespace ezv
{
	bool EzV::createRenderPass()
	{
		renderpass = new VulkanRenderpass();

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
		VKA(vkCreateRenderPass(context->logicalDevice, &createInfo, 0, &renderpass->renderpass));
		return true;
	}

	void EzV::destroyRenderPass()
	{
		if (renderpass && renderpass->renderpass)
		{
			VK(vkDestroyRenderPass(context->logicalDevice, renderpass->renderpass, 0));
			renderpass = 0;
			LOG_WARN("Renderpass destroyed successfully!");
		}
	}
}