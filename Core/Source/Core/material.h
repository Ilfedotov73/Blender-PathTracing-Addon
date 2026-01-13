#pragma once

#include "ray.h"
#include "hittable.h"
#include "texture.h"
#include "rt_helper.h"

namespace render_core {
	class material
	{
	public:
		virtual ~material() = default;

		virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
			return false;
		}
		virtual color emitted(float u, float v, const point3& p) const { return color(0.0f, 0.0f, 0.0f); }
	};

	class lambertian : public material
	{
	private:
		std::shared_ptr<texture> tex;
	public:
		lambertian(const color& albedo) : tex(std::make_shared<solid_color>(albedo)) {}
		lambertian(std::shared_ptr<texture> tex) : tex(tex) {}

		bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
		{
			vec3 scatter_dir = (rec.normal + random_unit_vector()); 

			if (scatter_dir.near_zero()) { scatter_dir = rec.normal; }

			scattered = ray(rec.p, scatter_dir, r_in.time());		
			attenuation = tex->value(rec.u, rec.v, rec.p);          
			return true;
		}
	};

	class metal : public material
	{
	private:
		color albedo;
		float fuzz;
	public:
		metal(const color& albedo, float fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1.0f) {}

		bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
		{
			vec3 reflected = reflect(r_in.direction(), rec.normal);			
			reflected = unitv(reflected) + (fuzz * random_unit_vector());	

			scattered = ray(rec.p, reflected, r_in.time());					
			attenuation = albedo;											

			return (dot(scattered.direction(), rec.normal) > 0);			
		}
	};

	class dielectric : public material
	{
	private:
		float refraction_index; 

		static float reflectance(float cosine, float refraction_index)
		{
			float r0 = (1.0f - refraction_index) / (1.0f + refraction_index);

			r0 *= r0;
			return r0 + (1 - r0) * std::pow((1.0f - cosine), 5.0f);
		}
	public:
		dielectric(float refraction_index) : refraction_index(refraction_index) {}

		bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
		{
			attenuation = color(1.0f, 1.0f, 1.0f); 

			float ri = rec.front_face ? (1.0f / refraction_index) : refraction_index; 

			vec3 unit_direction = unitv(r_in.direction());
			float cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0f);
			float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);

			bool cannot_refract = ri * sin_theta > 1.0; 

			vec3 direction;
			if (cannot_refract || reflectance(cos_theta, ri) > random_float())
				direction = reflect(unit_direction, rec.normal);
			else { direction = refract(unit_direction, rec.normal, ri); }

			scattered = ray(rec.p, direction, r_in.time());	 
			return true;
		}
	};

	class diffuse_light : public material
	{
	private:
		std::shared_ptr<texture> tex;
	public:
		diffuse_light(std::shared_ptr<texture> tex) : tex(tex) {}
		diffuse_light(const color& emit) : tex(std::make_shared<solid_color>(emit)) {}

		color emitted(float u, float v, const point3& p) const override { return tex->value(u, v, p); }
	};
}