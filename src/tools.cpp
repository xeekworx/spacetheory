#include <SDL.h>
#include <glad\glad.h>
#include "tools.h"
#include <sstream>
#include <vector>

using namespace spacetheory;

std::string tools::friendly_duration(const clock::time_point& start, const clock::time_point& end, const bool abbreviate)
{
    std::stringstream result;

    using day_t = std::chrono::duration<long, std::ratio<3600 * 24>>;

    auto dur = end - start;
    auto d = std::chrono::duration_cast<day_t>(dur);
    auto h = std::chrono::duration_cast<std::chrono::hours>(dur -= d);
    auto m = std::chrono::duration_cast<std::chrono::minutes>(dur -= h);
    auto s = std::chrono::duration_cast<std::chrono::seconds>(dur -= m);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur -= s);

    struct tmp { long long count; std::string name; };
    std::vector<tmp> dur_vec;

    dur_vec.push_back({ d.count(), abbreviate ? "day" : "day" });
    dur_vec.push_back({ h.count(), abbreviate ? "hr" : "hour" });
    dur_vec.push_back({ m.count(), abbreviate ? "min" : "minute" });
    dur_vec.push_back({ s.count(), abbreviate ? "sec" : "second" });
    dur_vec.push_back({ ms.count(), abbreviate ? "ms" : "millisecond" });

    bool first = true;
    for (const auto &x : dur_vec) {
        if (x.count <= 0) continue;
        if (!first) { // Only put a separator in front of other names
            result << " ";
        }
        result << x.count << " " << x.name;
        // Name plurality:
        if ((!abbreviate || x.name == "day") && x.count > 1) result << "s";
        first = false;
    }

    return result.str();
}

std::string sdltools::SDL_GLattrToString(const SDL_GLattr attr)
{
    switch (attr) {
    case SDL_GL_RED_SIZE:
        return "SDL_GL_RED_SIZE";
    case SDL_GL_GREEN_SIZE:
        return "SDL_GL_GREEN_SIZE";
    case SDL_GL_BLUE_SIZE:
        return "SDL_GL_BLUE_SIZE";
    case SDL_GL_ALPHA_SIZE:
        return "SDL_GL_ALPHA_SIZE";
    case SDL_GL_BUFFER_SIZE:
        return "SDL_GL_BUFFER_SIZE";
    case SDL_GL_DOUBLEBUFFER:
        return "SDL_GL_DOUBLEBUFFER";
    case SDL_GL_DEPTH_SIZE:
        return "SDL_GL_DEPTH_SIZE";
    case SDL_GL_STENCIL_SIZE:
        return "SDL_GL_STENCIL_SIZE";
    case SDL_GL_ACCUM_RED_SIZE:
        return "SDL_GL_ACCUM_RED_SIZE";
    case SDL_GL_ACCUM_GREEN_SIZE:
        return "SDL_GL_ACCUM_GREEN_SIZE";
    case SDL_GL_ACCUM_BLUE_SIZE:
        return "SDL_GL_ACCUM_BLUE_SIZE";
    case SDL_GL_ACCUM_ALPHA_SIZE:
        return "SDL_GL_ACCUM_ALPHA_SIZE";
    case SDL_GL_STEREO:
        return "SDL_GL_STEREO";
    case SDL_GL_MULTISAMPLEBUFFERS:
        return "SDL_GL_MULTISAMPLEBUFFERS";
    case SDL_GL_MULTISAMPLESAMPLES:
        return "SDL_GL_MULTISAMPLESAMPLES";
    case SDL_GL_ACCELERATED_VISUAL:
        return "SDL_GL_ACCELERATED_VISUA";
    case SDL_GL_RETAINED_BACKING:
        return "SDL_GL_RETAINED_BACKING";
    case SDL_GL_CONTEXT_MAJOR_VERSION:
        return "SDL_GL_CONTEXT_MAJOR_VERSION";
    case SDL_GL_CONTEXT_MINOR_VERSION:
        return "SDL_GL_CONTEXT_MINOR_VERSION";
    case SDL_GL_CONTEXT_EGL:
        return "SDL_GL_CONTEXT_EG";
    case SDL_GL_CONTEXT_FLAGS:
        return "SDL_GL_CONTEXT_FLAGS";
    case SDL_GL_CONTEXT_PROFILE_MASK:
        return "SDL_GL_CONTEXT_PROFILE_MASK";
    case SDL_GL_SHARE_WITH_CURRENT_CONTEXT:
        return "SDL_GL_SHARE_WITH_CURRENT_CONTEXT";
    case SDL_GL_FRAMEBUFFER_SRGB_CAPABLE:
        return "SDL_GL_FRAMEBUFFER_SRGB_CAPABLE";
    case SDL_GL_CONTEXT_RELEASE_BEHAVIOR:
        return "SDL_GL_CONTEXT_RELEASE_BEHAVIOR";
    default:
        return "UNKNOWN";
    }
}

std::string sdltools::SDL_GLprofileToString(const int profile_mask)
{
    switch (profile_mask) {
    case 0x0000:
        return "TO BE DETERMINED";
    case SDL_GL_CONTEXT_PROFILE_CORE:
        return "SDL_GL_CONTEXT_PROFILE_CORE";
    case SDL_GL_CONTEXT_PROFILE_COMPATIBILITY:
        return "SDL_GL_CONTEXT_PROFILE_COMPATIBILITY";
    case SDL_GL_CONTEXT_PROFILE_ES:
        return "SDL_GL_CONTEXT_PROFILE_ES";
    default:
        return "UNKNOWN";
    }
}

std::string sdltools::SDL_GLcontextFlagToString(const int flag)
{
    std::vector<std::string> flags;
    if (flag & SDL_GL_CONTEXT_DEBUG_FLAG) flags.push_back("SDL_GL_CONTEXT_DEBUG_FLAG");
    if (flag & SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG) flags.push_back("SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG");
    if (flag & SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG) flags.push_back("SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG");
    if (flag & SDL_GL_CONTEXT_RESET_ISOLATION_FLAG) flags.push_back("SDL_GL_CONTEXT_RESET_ISOLATION_FLAG");

    std::string result;
    for (auto i = flags.begin(); i != flags.end(); ++i) {
        if (i != flags.begin()) result += " | ";
        result += (*i);
    }

    return result;
}

std::string sdltools::SDL_GLcontextReleaseFlagToString(const int behavior)
{
    switch (behavior) {
    case SDL_GL_CONTEXT_RELEASE_BEHAVIOR_NONE:
        return "SDL_GL_CONTEXT_RELEASE_BEHAVIOR_NONE";
    case SDL_GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH:
        return "SDL_GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH";
    default:
        return "UNKNOWN";
    }
}

std::string gltools::GL_ErrorToString(GLenum error)
{
    switch (error) {
    case GL_NO_ERROR:
        return "No error has been recorded";
    case GL_INVALID_ENUM:
        return "An unacceptable value is specified for an enumerated argument";
    case GL_INVALID_VALUE:
        return "A numeric argument is out of range";
    case GL_INVALID_OPERATION:
        return "The specified operation is not allowed in the current state";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "The framebuffer object is not complete";
    case GL_OUT_OF_MEMORY:
        return "There is not enough memory left to execute the command";
    case GL_STACK_UNDERFLOW:
        return "An attempt has been made to perform an operation that would cause an internal stack to underflow.";
    case GL_STACK_OVERFLOW:
        return "An attempt has been made to perform an operation that would cause an internal stack to overflow.";
    default:
        return "An unknown error has occurred.";
    }
}