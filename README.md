# EzV
EzV is a lightweight high-level Vulkan Abstraction optimized for ease of use.  
**DISCLAIMER:** EzV is still being worked on and not close to being done or anything close to optimized yet.
Also the codebase isn't very pretty nor mainainable at the mo. Will fix in the future.
## What you'll need to run this
First off it was made for Windows 10/11. Not tested for Linux/Mac.  
Second off I use my trusty logger in this. I have written my own tiny logger, as a mod of Pilzschaf's Logger with some extra features. Heres the repo: https://github.com/LasseH-code/Lhg
Also for SDL you need that aswell. More infos on that in the FAQ.
## 4 Steps to a Window, a VulkanInstance, and a swapchain
Right now EzV natively only supports SDL. Support for GLFW will follow in the future.
You can however still use GLFW, its just not handled by EzV.
If you want to use SDL you have to use the compile definition EZV_SUPPORT_SDL.
1. Include ezv_base.h
2. Create a SDL_Window* and a EzVSDLWindowCreateInfo struct, with the name, position, size, and window flags (SDL_WINDOW_VULKAN is given to SDL no matter what)
3. Create a EzVCreateInfo struct, and pass it flags for the creation process for EzV, usage flags for the swapchain, a reference to an integer as a return value, the amount of Vulkan Extensions, and the extensions themselves, aswell as a reference to the EzVWindowCreateInfo struct
4. Create a new EzV object and pass it a pointer to the EzVCreateInfo struct, and your SDL_Window*
**DISCLAIMER:** EzV does **NOT** keep the window open on its own.
# FAQ
## Will more options, like choosing a specific GPU, be added in the future?
Yes, they will be added... at some point. It is not my top priority right now. If there is demand though, I'll do it.
## Is EzV a good way to use vulkan in bigger projects?
No, not right now. The options EzV gives you are to restrictive at the moment for it to be a good choice. You could work aroung that, but for now I'd recommend using something else or writing your own. In the future I hope this will be possible though.
## Will there be releases in the form of Libraries?
Likely at some point. For now just use the source files there ain't all to many.
## Why does SDL not work??
I compiled SDL locally in a subfolder. Just change the includes in ezv_base.h to whereever you have SDL installed and that should fix it.
