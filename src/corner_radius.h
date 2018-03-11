#pragma once
#include "third-party\glm\glm\glm.hpp"
#include <sstream>

namespace spacetheory {

    class corner_radius {
    private:
        glm::vec4 m_data;

    public:
        corner_radius(const glm::vec4& vec)
        {
            set(vec);
        }

        corner_radius(const float topleft, const float topright, const float bottomright, const float bottomleft)
        {
            set(topleft, topright, bottomright, bottomleft);
        }

        corner_radius(const float radius) { set(radius, radius, radius, radius); }

        virtual ~corner_radius() {}

        void clear() { for (glm::vec4::length_type i = 0; i < m_data.length(); ++i) m_data[i] = 0.0f; }

        void set(const glm::vec4 vec)
        {
            m_data = vec;
        }

        void set(const float topleft, const float topright, const float bottomright, const float bottomleft)
        {
            m_data[0] = topleft;
            m_data[1] = topright;
            m_data[2] = bottomright;
            m_data[3] = bottomleft;
        }

        void set(const float radius)
        {
            set(radius, radius, radius, radius);
        }

        void offset(const float topleft, const float topright, const float bottomright, const float bottomleft)
        {
            m_data[0] += topleft;
            m_data[1] += topright;
            m_data[2] += bottomright;
            m_data[3] += bottomleft;
        }

        void offset(const float value)
        {
            offset(value, value, value, value);
        }

        float topleft() const { return m_data[0]; }
        float topright() const { return m_data[1]; }
        float bottomright() const { return m_data[2]; }
        float bottomleft() const { return m_data[3]; }

        bool empty() const
        {
            if (m_data[0] <= 0.0f && m_data[1] <= 0.0f && m_data[2] <= 0.0f && m_data[3] <= 0.0f) return true;
            else return false;
        }

        std::wstring str() const
        {
            std::wstringstream ss;
            ss << m_data[0] << L", " << m_data[1] << L", " << m_data[2] << L", " << m_data[3];
            return ss.str();
        }

        corner_radius& operator=(const corner_radius& cr) { m_data = cr.m_data; return *this; }
        corner_radius& operator=(const glm::vec4& vec) { m_data = vec; return *this; }
        void operator+=(const float value) { offset(value); }
        void operator-=(const float value) { offset(-value); }
        bool operator==(const corner_radius& cr) const { return cr.m_data == m_data; }
        bool operator!=(const corner_radius& cr) const { return cr.m_data != m_data; }
        bool operator==(const glm::vec4& vec) const { return vec == m_data; }
        bool operator!=(const glm::vec4& vec) const { return vec != m_data; }
    };

}