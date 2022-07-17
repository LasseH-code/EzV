#include "ezv_base.h"
#ifdef EZV_SUPPORT_SDL
namespace ezv
{
    EzV::EzV(EzVCreateInfo* cs, SDL_Window* window)
    {
        DWORD* flags = &cs->m_creationFlags;
        
        Uint32 sdl_inits = 0;
        if (IS_FLAG_SET(*flags, INIT_VIDEO_SDL)) sdl_inits |= SDL_INIT_VIDEO;
        
        if (sdl_inits != 0 && SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            LOG_CRIT("Error initializing SDL: ", SDL_GetError());
            return;
        }
#ifdef EZV_INFO_OUTPUT
        if (sdl_inits != 0) LOG_INFO("SDL initialized successfully!");
#endif // EZV_INFO_OUTPUT
        
        if (IS_FLAG_SET(*flags, CREATE_WINDOW_SDL) && !createSDLWindow(cs->p_sdlWindowCS, &window))
        {
            LOG_ERROR("An error occured whilst creating a SDL window. aborting");
            return;
        }
#ifdef EZV_INFO_OUTPUT
        if (IS_FLAG_SET(*flags, CREATE_WINDOW_SDL)) LOG_INFO("SDL Window created successfully!");
#endif // EZV_INFO_OUTPUT
        
        if (IS_FLAG_SET(*flags, QUERY_INSTANCE_EXTENSIONS_SDL) && !querySDLInstanceExtensions(cs, &window))
        {
            LOG_ERROR("An error occured whilst querying for instance Extensions. aborting");
            return;
        }
#ifdef EZV_INFO_OUTPUT
        if (IS_FLAG_SET(*flags, QUERY_INSTANCE_EXTENSIONS_SDL)) LOG_INFO("Query for InstanceExtensions successful!");
#endif // EZV_INFO_OUTPUT
        
        *cs->p_returnVal = createVulkanInstance(cs);
        
        
        if (IS_FLAG_SET(*flags, CREATE_SURFACE_KHR_SDL) && !createSurfaceSDL(&window))
        {
            LOG_ERROR("Couldn't create VkSurfaceKHR via SDL. aborting swapchain creation...");
            destroySwapchain();
            return;
        }
#ifdef EZV_INFO_OUTPUT
        if (IS_FLAG_SET(*flags, CREATE_SURFACE_KHR_SDL)) LOG_INFO("Surface created successfully!");
#endif // EZV_INFO_OUTPUT
        
        if (IS_FLAG_SET(*flags, CREATE_SWAPCHAIN_KHR) && !createSwapchain(cs->m_usageFlags))
        {
            LOG_ERROR("Couldn't establish swapchain. aborting swapchain creation");
            destroySwapchain();
            return;
        }
#ifdef EZV_INFO_OUTPUT
        if (IS_FLAG_SET(*flags, CREATE_SWAPCHAIN_KHR)) LOG_INFO("Swapchain created successfully!");
        LOG_INFO("Initialization done");
#endif // EZV_INFO_OUTPUT
    }
    
    bool EzV::createSDLWindow(EzVSDLWindowCreateInfo* cs, SDL_Window** window)
    {
        *window = SDL_CreateWindow(cs->title, cs->x, cs->y, cs->w, cs->h, SDL_WINDOW_VULKAN | cs->windowFlags);
        if (!*window)
        {
            LOG_CRIT("Error creating SDL window: ", SDL_GetError());
            return 1;
        }
        //LOG_DEBUG("window: ", *window);
        return(*window);
    }
    
    bool EzV::createSurfaceSDL(SDL_Window** window)
    {
        if (!SDL_Vulkan_CreateSurface(*window, context->instance, &swapchain->surface))
        {
            LOG_ERROR("Error creating Vulkan Surface via SDL");
            return false;
        }
        return true;
    }
    
    bool EzV::querySDLInstanceExtensions(EzVCreateInfo* cs, SDL_Window** window)
    {
        if (!cs)
        {
            LOG_ERROR("Error whilst querying for Instance Extensions. continuing");
            return false;
        }
        return (querySDLInstanceExtensions(&cs->m_instanceExtensionCount, &cs->p_instanceExtensions, window));
    }
    
    bool EzV::querySDLInstanceExtensions(uint32_t* instanceExtensionCount, const char*** instanceExtensions,  SDL_Window** window)
    {
        if (!window)
        {
            LOG_ERROR("Error whilst querying for Instance Extensions. continuing");
            return false;
        }
        SDL_Vulkan_GetInstanceExtensions(*window, instanceExtensionCount, 0);
        //if (instanceExtensions) delete[] *instanceExtensions;
        *instanceExtensions = new const char* [*instanceExtensionCount];
        SDL_Vulkan_GetInstanceExtensions(*window, instanceExtensionCount, *instanceExtensions);
        return true;
    }
}
#endif