#pragma once

#include <iostream>
#include <cmath>

#include "rt_helper.h"

namespace render_core {
	class vec3
	{
	public:
		float e[3];

		vec3() : e{ 0.0f,0.0f,0.0f } {}
		vec3(float e0, float e1, float e2) : e{ e0, e1, e2 } {}

		float x() const { return e[0]; }
		float y() const { return e[1]; }
		float z() const { return e[2]; }

		vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

		float  operator[](int i) const { return e[i]; }
		float& operator[](int i) { return e[i]; }

		vec3& operator+=(const vec3& v)
		{
			e[0] += v.e[0];
			e[1] += v.e[1];
			e[2] += v.e[2];
			return *this;
		}

		vec3& operator*=(float t)
		{
			e[0] *= t;
			e[1] *= t;
			e[2] *= t;
			return *this;
		}

		vec3& operator/=(float t) { return *this *= 1.0f / t; }

		float length() const { return std::sqrt(length_squared()); }

		float length_squared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

		static vec3 random() { return vec3(random_float(), random_float(), random_float()); }
		static vec3 random(float min, float max) { return vec3(random_float(min, max), random_float(min, max), random_float(min, max)); }

		bool near_zero() const
		{
			float s = 1e-8f; 
			return(std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
		}

	};

	using point3 = vec3;

	inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
		return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
	}

	inline vec3 operator+(const vec3& u, const vec3& v)
	{
		return vec3(
			u.e[0] + v.e[0],
			u.e[1] + v.e[1],
			u.e[2] + v.e[2]
		);
	}

	inline vec3 operator-(const vec3& u, const vec3& v)
	{
		return vec3(
			u.e[0] - v.e[0],
			u.e[1] - v.e[1],
			u.e[2] - v.e[2]
		);
	}

	inline vec3 operator*(const vec3& u, const vec3& v)
	{
		return vec3(
			u.e[0] * v.e[0],
			u.e[1] * v.e[1],
			u.e[2] * v.e[2]
		);
	}

	inline vec3 operator*(float t, const vec3& v)
	{
		return vec3(
			t * v.e[0],
			t * v.e[1],
			t * v.e[2]
		);
	}

	inline vec3 operator*(const vec3& v, float t) { return t * v; }

	inline vec3 operator/(const vec3& v, float t) { return (1 / t) * v; }

	inline float dot(const vec3& u, const vec3& v)
	{
		return u.e[0] * v.e[0]
			+ u.e[1] * v.e[1]
			+ u.e[2] * v.e[2];
	}

	inline vec3 cross(const vec3& u, const vec3& v)
	{
		return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
			u.e[2] * v.e[0] - u.e[0] * v.e[2],
			u.e[0] * v.e[1] - u.e[1] * v.e[0]);
	}

	inline vec3 unitv(const vec3& v) { return v / v.length(); }

	inline float distance(const vec3& u, const vec3& v)
	{
		float dx = u.x() - v.x();
		float dy = u.y() - v.y();
		float dz = u.z() - v.z();
		return sqrt(dx * dx + dy * dy + dz * dz);
	}

	inline vec3 random_unit_vector()
	{
		for (;;) {
			vec3 p = vec3::random(-1.0f, 1.0f);                                 
			float lensq = p.length_squared();
			if (1e-160f < lensq && lensq <= 1.0f) { return p / sqrt(lensq); }  
		}
	}

	inline vec3 random_on_hemisphere(const vec3& normal)
	{
		vec3 rndv_in_unit_sp = random_unit_vector();
		if (dot(rndv_in_unit_sp, normal) > 0.0f) { return rndv_in_unit_sp; } 

		else { return -rndv_in_unit_sp; }
	}

	inline vec3 reflect(const vec3& v, const vec3& n) { return v - 2.0f * dot(v, n) * n; }

	inline vec3 refract(const vec3& uv, const vec3& n, float etai_over_etat)
	{
		float cos_theta = std::fmin(dot(-uv, n), 1.0f);				 

		vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);	
		vec3 r_out_paral = -std::sqrt(std::fabs(1.0f - r_out_perp.length_squared())) * n; 
		return r_out_perp + r_out_paral;
	}

	inline vec3 random_in_unit_disk()
	{
		for (;;) {
			vec3 p = vec3(random_float(-1, 1), random_float(-1.0f, 1.0f), 0.0f);
			if (p.length_squared() < 1.0f) { return p; }
		}
	}
}