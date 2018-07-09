#define _CRT_SECURE_NO_WARNINGS

#include "glfw/context.c"
#include "glfw/init.c"
#include "glfw/input.c"
#include "glfw/monitor.c"
#include "glfw/vulkan.c"
#include "glfw/window.c"

// Windows Part
#ifdef _GLFW_WIN32

    #include "glfw/win32_init.c"
    #include "glfw/win32_joystick.c"
    #include "glfw/win32_monitor.c"
    #include "glfw/win32_time.c"
    #include "glfw/win32_thread.c"
    #include "glfw/win32_window.c"
    #include "glfw/wgl_context.c"
    #include "glfw/egl_context.c"
    #include "glfw/osmesa_context.c"
#endif

// Todo : Add other stuff