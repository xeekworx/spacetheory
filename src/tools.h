#pragma once
// WARNING: ONLY INCLUDE THIS IN SOURCE FILES SO THAT INTERNAL APIS SUCH AS OPENGL
// ISN'T A DEPENDENCY ISSUE TO PROJECTS USING THIS LIBRARY

#include <string>
#include <chrono>

namespace spacetheory {
    namespace tools {
        using clock = std::chrono::high_resolution_clock;

        std::string friendly_duration(const clock::time_point& start, const clock::time_point& end, const bool abbreviate = true);
    }

    namespace sdltools {
        std::string SDL_GLattrToString(const SDL_GLattr attr);
        std::string SDL_GLprofileToString(const int profile_mask);
        std::string SDL_GLcontextFlagToString(const int flag);
        std::string SDL_GLcontextReleaseFlagToString(const int behavior);
    }

    namespace gltools {
        std::string GL_ErrorToString(GLenum error);
    }
}