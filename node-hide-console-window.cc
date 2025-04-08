#define NAPI_VERSION 8 // Specifies a sufficiently recent N-API version (e.g., 8 is okay for Node 16+)
#include <napi.h>     // Include the node-addon-api header instead of node.h

#ifdef _WIN32
#include <windows.h> // Keep the include of Windows.h
#else
// You might want to insert a compilation error or a NOP for non-Windows platforms
#error This module only supports Windows
#endif

// The logic to get the window remains unchanged
HWND GetTargetWindow()
{
    HWND consoleWindow = GetConsoleWindow();
    // If there is no console window (e.g., executed from a GUI without a console), do nothing or return NULL
    if (consoleWindow == NULL) {
        return NULL;
    }
    HWND parentWindow = GetParent(consoleWindow);

    // If there is no parent (console is the "owner"), use the console window itself.
    // This handles scenarios like direct execution from cmd.exe.
    if (parentWindow == NULL)
    {
        return consoleWindow;
    }

    // Otherwise (e.g., perhaps launched by another process like VS Code),
    // try to use the parent window.
    // Note: this logic might not cover all edge cases perfectly.
    return parentWindow;
}

// HideConsole function rewritten for node-addon-api
Napi::Value HideConsole(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env(); // Get the N-API environment

#ifdef _WIN32
    HWND hWnd = GetTargetWindow();
    if (hWnd != NULL) {
        ShowWindow(hWnd, SW_HIDE); // 0 = SW_HIDE
    }
#endif

    return env.Undefined(); // Returns undefined to JavaScript
}

// ShowConsole function rewritten for node-addon-api
Napi::Value ShowConsole(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env(); // Get the N-API environment

#ifdef _WIN32
    HWND hWnd = GetTargetWindow();
    if (hWnd != NULL) {
        ShowWindow(hWnd, SW_SHOW); // 1 = SW_SHOW
    }
#endif

    return env.Undefined(); // Returns undefined to JavaScript
}

// Initialization function rewritten for node-addon-api
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    // Exports the functions with the desired names
    exports.Set(Napi::String::New(env, "hideConsole"), Napi::Function::New(env, HideConsole));
    exports.Set(Napi::String::New(env, "showConsole"), Napi::Function::New(env, ShowConsole));

    // Returns the modified exports object
    return exports;
}

// Registers the module using the node-addon-api macro
// The first argument must match "target_name" in binding.gyp
NODE_API_MODULE(node_hide_console_window, Init)