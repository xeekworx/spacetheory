#pragma once
#include <sstream>

namespace spacetheory {

		class size {
		public:
			int w, h;

			size(const int w = 0, const int h = 0) : w(w), h(h) {}
			size(const size& sz) : w(sz.w), h(sz.h) {}
			virtual ~size() {}

			void clear() { w = h = 0; }

			void set(const int w, const int h)
			{
				this->w = w;
				this->h = h;
			}

			void offset(const int w, const int h)
			{
				this->w += h;
				this->h += h;
			}

			void offset(const size& sz)
			{
				this->w += sz.w;
				this->h += sz.h;
			}

			bool empty() const
			{
				if(w == 0 && h == 0) return true;
				else return false;
			}

			std::wstring str() const
			{
				std::wstringstream ss;
				ss << w << L" x " << h;
				return ss.str();
			}

			bool operator>(const size& sz) { return this->w > sz.w && this->h > sz.h; }
			bool operator<(const size& sz) { return this->w < sz.w && this->h < sz.h; }
			bool operator>=(const size& sz) { return this->w >= sz.w && this->h >= sz.h; }
			bool operator<=(const size& sz) { return this->w <= sz.w && this->h <= sz.h; }
			size operator+(size sz) { return size(w + sz.w, h + sz.h); }
			void operator+=(size sz) { offset(sz); }
			size operator-(size sz) { return size(w - sz.w, h - sz.h); }
			void operator-=(size sz) { offset(-sz.w, -sz.h); }
			bool operator==(size sz) { return (w == sz.w && h == sz.h); }
			bool operator!=(size sz) { return (w != sz.w || h != sz.h); }
		};

}