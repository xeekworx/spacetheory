#pragma once

namespace spacetheory {

    struct graphics_setup {
        int glversion_major = 3;
        int glversion_minor = 3;
        bool vsync = false;
        bool msaa = false; // multisample antialiasing
        int msaa_samples = 2;
    };

}