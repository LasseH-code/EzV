#include "ezv_base.h"

namespace ezv
{
	bool EzV::createRenderPass(EzVRenderPassCreateInfo* createInfo)
	{
		VulkanRenderpass* temp = new VulkanRenderpass();
		VkAttachmentReference attachmentReference;// = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
		VkAttachmentDescription attachmentDescription = {};

		VkSubpassDescription subpass = {};

		VkSampleCountFlagBits samples = (VkSampleCountFlagBits) createInfo->m_samples;

		switch (createInfo->m_renderPassType)
		{
		case RENDER_PASS_TYPE_CUSTOM:
			temp->m_renderpass = *createInfo->p_customRenderPass;
			break;
		case RENDER_PASS_TYPE_COLOR_LOAD:
			attachmentDescription.format = swapchain->format;
			attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
			attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			attachmentReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; // For rn; COMPUTE is also an option f.e.
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &attachmentReference;
			break;
		case RENDER_PASS_TYPE_COLOR_NORMAL:
			attachmentDescription.format = swapchain->format;
			attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			attachmentReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; // For rn; COMPUTE is also an option f.e.
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &attachmentReference;
			break;
		case RENDER_PASS_TYPE_COLOR_CLEAR:
			attachmentDescription.format = swapchain->format;
			attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			attachmentReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; // For rn; COMPUTE is also an option f.e.
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &attachmentReference;
			break;
		case RENDER_PASS_TYPE_CUSTOM_SUBPASSES:
			break;
		default:
			LOG_ERROR("createInfo->m_renderPassType is out of range: ", createInfo->m_renderPassType);
		}

		VkRenderPassCreateInfo renderpassCreateInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
		renderpassCreateInfo.attachmentCount = 1;
		renderpassCreateInfo.pAttachments = &attachmentDescription;
		renderpassCreateInfo.subpassCount = 1;
		renderpassCreateInfo.pSubpasses = &subpass;
		VKA(vkCreateRenderPass(context->logicalDevice, &renderpassCreateInfo, 0, &temp->m_renderpass));
		renderpasses.push_back(*temp);
		temp = 0;
		return true;
	}

	void EzV::destroyRenderPasses()
	{
		//VulkanRenderpass& temp = renderpasses.data();
		if (renderpasses.size() > 0)
		{
			while (renderpasses.size() > 0)
			{
				VK(vkDestroyRenderPass(context->logicalDevice, renderpasses.back().m_renderpass, 0)); // change away from a pointer
				renderpasses.pop_back();
			}
			LOG_WARN("Renderpass destroyed successfully!");
		}
	}
}