#pragma once
#include <sstream>

namespace spacetheory {

        class point {
        public:
            int x, y;

            point(const int x = 0, const int y = 0) { this->x = x; this->y = y; }
            point(const point& pt) { this->x = pt.x; this->y = pt.y; }
            virtual ~point() {}

            void clear() { x = y = 0; }

            void set(const int x, const int y)
            {
                this->x = x;
                this->y = y;
            }

            double distance(const int x, const int y)
            {
                const double x_distance = (double) this->x - (double) x;
                const double y_distance = (double) this->y - (double) y;
                return sqrt(x_distance*x_distance + y_distance*y_distance);
            }

            double distance(const point& pt)
            {
                const double x_distance = (double) x - (double) pt.x;
                const double y_distance = (double) y - (double) pt.y;
                return sqrt(x_distance*x_distance + y_distance*y_distance);
            }

            void offset(const int x, const int y)
            {
                this->x += x;
                this->y += y;
            }

            void offset(const point& pt)
            {
                this->x += pt.x;
                this->y += pt.y;
            }

            bool empty() const
            {
                if(x == 0 && y == 0) return true;
                else return false;
            }

            std::wstring str() const
            {
                std::wstringstream ss;
                ss << x << L", " << y;
                return ss.str();
            }

            point operator+(const point& pt) { return point(x + pt.x, y + pt.y); }
            void operator+=(const point& pt) { offset(pt); }
            point operator-(const point& pt) { return point(x - pt.x, y - pt.y); }
            void operator-=(const point& pt) { offset(-pt.x, -pt.y); }
            bool operator==(const point& pt) { return (x == pt.x && y == pt.y); }
            bool operator!=(const point& pt) { return (x != pt.x || y != pt.y); }
        };

}