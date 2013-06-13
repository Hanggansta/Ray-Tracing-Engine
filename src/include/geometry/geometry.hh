// File: geometry.hh
// Date: Thu Jun 13 13:22:54 2013 +0800
// Author: Yuxin Wu <ppwwyyxxc@gmail.com>

#pragma once

#include <vector>
#include <cmath>
#include <limits>

#include "common.hh"

class Geometry {
	public:
		int w, h;

		Geometry(int m_w = 0, int m_h = 0):
			w(m_w), h(m_h) {}

		inline int area() const
		{ return w * h; }

		real_t ratio() const
		{ return (real_t) std::max(w, h) / std::min(w, h); }

		inline bool contain(int x, int y)
		{ return (x >= 0 && x < w && y >= 0 && y < h); }
};

class Vector {
	public:
		real_t x = 0, y = 0, z = 0;

		explicit Vector(real_t m_x = 0, real_t m_y = 0, real_t m_z = 0):
			x(m_x), y(m_y), z(m_z) {}

		Vector(const Vector &p0, const Vector &p1):
			x(p1.x - p0.x), y(p1.y -p0.y), z(p1.z - p0.z) {}

		real_t index(int c) const
		{ return c == 0 ? x : c == 1 ? y : z; }

		real_t& index(int c)
		{ return c == 0 ? x : c == 1 ? y : z; }

		real_t min_comp_abs() const {
			real_t a = fabs(x), b = fabs(y), c = fabs(z);
			::update_min(a, b), ::update_min(a, c);
			return a;
		}

		real_t sqr() const
		{ return x * x + y * y + z * z; }

		real_t mod() const
		{ return sqrt(sqr()); }

		real_t dot(const Vector &v) const
		{ return x * v.x + y * v.y + z * v.z; }

		Vector cross(const Vector &v) const
		{ return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

		Vector& operator = (const Vector& v)
		{ x = v.x, y = v.y, z = v.z; return *this; }

		virtual void normalize() {
			real_t m = 1 / mod();
			*this *= m;		// work?
			m_assert(std::isnormal(m));
		}

		Vector get_normalized() const
		{ Vector ret(*this); ret.normalize(); return ret; }

		bool is_zero(real_t threshold = EPS) const
		{ return fabs(x) < threshold && fabs(y) < threshold && fabs(z) < threshold; }

		bool is_positive(real_t threshold = EPS) const
		{ return x > threshold && y > threshold && z > threshold; }

		void update_min(const Vector &v)
		{ ::update_min(x, v.x); ::update_min(y, v.y); ::update_min(z, v.z); }

		void update_max(const Vector &v)
		{ ::update_max(x, v.x); ::update_max(y, v.y); ::update_max(z, v.z); }

		Vector operator + (const Vector &v) const
		{ return Vector(x + v.x, y + v.y, z + v.z); }

		Vector& operator += (const Vector &v)
		{ x += v.x; y += v.y; z += v.z; return *this; }

		Vector operator - (const Vector &v) const
		{ return Vector(x - v.x, y - v.y, z - v.z); }

		Vector operator - () const
		{ return Vector(-x, -y, -z); }

		Vector& operator -= (const Vector &v)
		{ x -= v.x; y -= v.y; z -= v.z; return *this; }

		Vector operator * (real_t p) const
		{ return Vector(x * p, y * p, z * p); }

		Vector& operator *= (real_t p)
		{ x *= p; y *= p; z *= p; return *this; }

		Vector operator / (real_t p) const
		{ return *this * (1.0 / p); }

		bool operator == (const Vector &v) const
		{ return fabs(x - v.x) < EPS && fabs(y - v.y) < EPS && fabs(z - v.z) < EPS; }

		bool operator != (const Vector &v) const
		{ return fabs(x - v.x) >= EPS || fabs(y - v.y) >= EPS || fabs(z - v.z) >= EPS; }

		friend std::ostream & operator << (std::ostream &os, const Vector& vec)
		{ return os << vec.x << " " << vec.y << " " << vec.z;}

		static Vector max()
		{ return Vector(std::numeric_limits<real_t>::max(), std::numeric_limits<real_t>::max()); }

		static Vector infinity()
		{ return Vector(std::numeric_limits<real_t>::infinity(), std::numeric_limits<real_t>::infinity()); }

		real_t get_max()
		{ return std::max(x, std::max(y, z)); }

		real_t get_abs_max()
		{ return std::max(fabs(x), std::max(fabs(y), fabs(z))); }

		static Vector get_zero()
		{ return Vector(0, 0, 0); }

		inline bool isnoraml() const
		{ return isfinite(x) && isfinite(y) && isfinite(z); }

		// i'm norm
		Vector reflection(const Vector& v) const {
			m_assert(fabs(v.sqr() - 1) < EPS && (sqr() - 1 < EPS));
			return *this * 2 * dot(v) - v;
		}

		// i'm norm
		Vector transmission(const Vector& v_in, real_t density) const {
			Vector ret = Vector::infinity();

			density = 1 / density;
			real_t cos1 = -dot(v_in);
			if (cos1 < EPS) return ret;
			m_assert(cos1 >= 0);
			real_t cos2 = 1 - ::sqr(density) * (1 - ::sqr(cos1));
			if (cos2 < 0) return ret;
			cos2 = sqrt(cos2);

			ret = v_in * density + (*this) * (density * cos1 - cos2);
			return ret.get_normalized();
		}
};


template<typename T>
class Vector2D {
	public:
		T x = 0, y = 0;

		Vector2D<T>(){};

		explicit Vector2D<T>(T m_x, T m_y):
			x(m_x), y(m_y) {}

		Vector2D<T> (const Vector2D<T> &p0, const Vector2D<T> &p1):
			x(p1.x - p0.x), y(p1.y -p0.y) {}

		T dot(const Vector2D<T> &v) const
		{ return x * v.x + y * v.y; }

		T cross(const Vector2D<T> &v) const
		{ return x * v.y - y * v.x; }

		Vector2D<T> operator + (const Vector2D<T> &v) const
		{ return Vector2D<T>(x + v.x, y + v.y); }

		Vector2D<T>& operator += (const Vector2D<T> &v)
		{ x += v.x; y += v.y; return *this; }

		Vector2D<T> operator - (const Vector2D<T> &v) const
		{ return Vector2D<T>(x - v.x, y - v.y); }

		Vector2D<T>& operator -= (const Vector2D<T> &v)
		{ x -= v.x; y -= v.y; return *this; }

		Vector2D<T> operator * (real_t f) const
		{ return Vector2D<T>(x * f, y * f); }

		Vector2D<T> operator / (real_t f) const
		{ return *this * (1.0 / f); }

		bool operator == (const Vector2D<T> &v) const
		{ return fabs(x - v.x) < EPS && fabs(y - v.y) < EPS; }

		// take negative of the second component
		Vector2D<T> operator ! () const
		{ return Vector2D<T>(x, -y); }

		// swap the two component
		Vector2D<T> operator ~ () const
		{ return Vector2D<T>(y, x); }

		bool is_zero() const
		{ return fabs(x) < EPS && fabs(y) < EPS; }

		T sqr() const
		{ return x * x + y * y; }

		real_t mod() const
		{ return hypot(x, y); }

		Vector2D<T> get_normalized() const
		{ real_t m = mod(); m_assert(m > EPS); m = 1 / m; return Vector2D<T>(x * m, y * m); }

		template <typename TT>
		friend std::ostream& operator << (std::ostream& os, const Vector2D<TT>& v);

		void update_min(const Vector2D<T> &v)
		{ ::update_min(x, v.x); ::update_min(y, v.y);}

		void update_max(const Vector2D<T> &v)
		{ ::update_max(x, v.x); ::update_max(y, v.y);}
};

template<typename T>
std::ostream& operator << (std::ostream& os, const Vector2D<T>& v) {
	os << v.x << ' ' << v.y;
	return os;
}

typedef Vector Vec;
typedef Vector2D<int> Coor;

typedef Vector2D<real_t> Vec2D;

typedef std::pair<Coor, Coor> Line2D;
typedef std::vector<Coor> Polygon;

