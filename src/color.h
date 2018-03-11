#pragma once
#include <stdint.h>
#include <cstdlib>      // std::strtoul
#include <string>
#include <sstream>      // std::stringstream
#include <ios>          // std::hex
#include <algorithm>    // std::transform
#include "third-party\glm\glm\glm.hpp"
#include "html_colors.h"

namespace spacetheory {

    class color {
    public:
        // There is concern that this might not work the same on non x86 based processors
        // due to this code assuming little-endian.

        static constexpr uint32_t transparent = 0xFFFFFF00;
        static constexpr uint32_t white = 0xFFFFFFFF;
        static constexpr uint32_t black = 0x000000FF;

        uint8_t r, g, b, a;

        color() : color(color::transparent) {}
        color(const color& c) : r(c.r), g(c.g), b(c.b), a(c.a) {}
        color(uint32_t rgba_value) { *this = color::from_rgba(rgba_value); }
        color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
        color(float r, float g, float b, float a = 255.f) : 
            r(static_cast<uint8_t>(std::round(r * 255.f))), 
            g(static_cast<uint8_t>(std::round(g * 255.f))), 
            b(static_cast<uint8_t>(std::round(b * 255.f))), 
            a(static_cast<uint8_t>(std::round(a * 255.f))) {}
        color(const std::string& html) { *this = color::from_html(html); }
        color(const glm::vec4& v) : color(v.r, v.g, v.b, v.a) {}
        color(const glm::vec3& v) : color(v.r, v.g, v.b) {}

        /* rgba_value */ operator uint32_t() { return color::to_rgba(*this); }

        friend bool operator==(const color& c1, const color& c2) {
            return (
                c1.r == c2.r &&
                c1.g == c2.g &&
                c1.b == c2.b &&
                c1.a == c2.a);
        }
        friend bool operator!=(const color& c1, const color& c2) {
            return !(c1 == c2);
        }

        static color from_html(const std::string& html) {
            std::string formatted(html);
            formatted.erase(std::find(formatted.begin(), formatted.end(), '#'));
            if (formatted.length() == 8)
                return color::from_rgba(std::strtoul(formatted.c_str(), 0, 16));
            else if (formatted.length() == 6)
                return color::from_rgb(std::strtoul(formatted.c_str(), 0, 16));
            else
                return color::black;
        }

        static color from_rgba(uint32_t v) {
            return color(
                (uint8_t)((v >> 24) & 0xFF),   // red
                (uint8_t)((v >> 16) & 0xFF),   // green
                (uint8_t)((v >> 8) & 0xFF),    // blue
                (uint8_t)(v & 0xFF)            // alpha
            );
        }

        static color from_rgba(uint32_t v, uint8_t alpha) {
            return color(
                (v >> 24) & 0xFF,   // red
                (v >> 16) & 0xFF,   // green
                (v >> 8) & 0xFF,    // blue
                alpha               // alpha
            );
        }

        static color from_rgb(uint32_t v, uint8_t alpha = 0xFF) {
            return color(
                (v >> 16) & 0xFF,   // red
                (v >> 8) & 0xFF,    // green
                v & 0xFF,           // blue
                alpha
            );
        }

        static color from_abgr(uint32_t v) {
            return color(
                (uint8_t)(v & 0xFF),           // red
                (uint8_t)((v >> 8) & 0xFF),    // green
                (uint8_t)((v >> 16) & 0xFF),   // blue
                (uint8_t)((v >> 24) & 0xFF)    // alpha
            );
        }

        static color from_abgr(uint32_t v, uint8_t alpha) {
            return color(
                v & 0xFF,           // red
                (v >> 8) & 0xFF,    // green
                (v >> 16) & 0xFF,   // blue
                alpha               // alpha
            );
        }

        static uint32_t to_rgba(const color& c) {
            return ((c.a) + (c.b << 8) + (c.g << 16) + (c.r << 24));
        }

        static uint32_t to_rgba(const color& c, uint8_t alpha) {
            return ((alpha)+(c.b << 8) + (c.g << 16) + (c.r << 24));
        }

        static uint32_t to_rgb(const color& c) {
            return ((c.b) + (c.g << 8) + (c.r << 16));
        }

        static uint32_t to_abgr(const color& c) {
            return ((c.r) + (c.g << 8) + (c.b << 16) + (c.a << 24));
        }

        static uint32_t to_abgr(const color& c, uint8_t alpha) {
            return ((c.r) + (c.g << 8) + (c.b << 16) + (alpha << 24));
        }

        static std::string to_html(const color& c, bool uppercase = false) {
            std::stringstream ss;
            ss << "#" << std::hex << to_rgb(c);
            if (uppercase) {
                std::string s(ss.str());
                std::transform(s.begin(), s.end(), s.begin(), ::toupper);
                return s;
            }
            else return ss.str();
        }

        static std::string to_string(const color& c, bool uppercase = false) {
            std::stringstream ss;
            ss << "#" << std::hex << to_rgba(c);
            if (uppercase) {
                std::string s(ss.str());
                std::transform(s.begin(), s.end(), s.begin(), ::toupper);
                return s;
            }
            else return ss.str();
        }

    };

}