{
  "targets": [
    {
      "target_name": "node-hide-console-window",
      "sources": [ "node-hide-console-window.cc" ],
      "include_dirs": [
        # Adds the node-addon-api header directory
        "<!(node -p \"require('node-addon-api').include_dir\")"
      ],
      "dependencies": [
        # Adds the dependency to ensure node-addon-api is built if necessary
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "cflags!": [ "-fno-exceptions" ], # Disables flags that might conflict
      "cflags_cc!": [ "-fno-exceptions" ],
      "xcode_settings": { # Specific settings for macOS (if compiling there)
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "CLANG_CXX_LIBRARY": "libc++",
        "MACOSX_DEPLOYMENT_TARGET": "10.7"
      },
      "msvs_settings": { # Specific settings for Windows (Visual Studio)
        "VCCLCompilerTool": { "ExceptionHandling": 1 } # Enables C++ exceptions (required by node-addon-api)
      },
      "defines": [
         # Defines NAPI_VERSION for consistency, even though defined in the .cc file
         # Uses a reasonable value supported by Node 18/20 (e.g., 8)
        'NAPI_VERSION=8',
        # Standard definitions for Windows, node-gyp usually handles these
        '_HAS_EXCEPTIONS=1' # Required by msvs_settings ExceptionHandling=1
      ]
    }
  ]
}