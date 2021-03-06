#pragma once
#include <stdint.h>
#include <memory>
#include "rectangle.h"
#include "corner_radius.h"
#include "color.h"

namespace spacetheory {

    class graphics2d {
    private:
        void * m_fbo; // NanoVG Frame Buffer
        float m_width, m_height;
        bool m_ready;
        bool m_antialias;

        struct glstate {
            float viewport[4];
        } m_glstate;

        static void create_nvg_context();
        static void delete_nvg_context();

    public:
        static const color transparent, black, white, red, green, blue;

        graphics2d(const bool antialias = false);
        graphics2d(const uint32_t width, const uint32_t height, const bool antialias = false);
        virtual ~graphics2d();

        inline const bool& is_ready() const { return m_ready; }

        void begin();
        void end();
        void cancel();
            
        void clear(const color& c);

        void scale_percent(const float percent);
        void scale_percent(const float x_percent, const float y_percent);
        void scale_factor(const float factor);
        void scale_factor(const float x_factor, const float y_factor);
        void reset_transform();

        void draw_rect(rectangle& rect, const float border_width, const color& border_color, const color& fill_color = graphics2d::transparent);
        void fill_rect(rectangle& rect, const color& fill_color);

        void draw_roundrect(rectangle& rect, const corner_radius& radius, const float border_width, const color& border_color, const color& fill_color = graphics2d::transparent);
        void fill_roundrect(rectangle& rect, const corner_radius& radius, const color& fill_color);

        void draw(const graphics2d& source, const float x, const float y);
        void test();
    };

}
