#pragma once

typedef unsigned char uchar;

#include "rt_stb_image.h"

namespace render_core {

	class texture
	{
	public:
		virtual ~texture() = default;
		virtual color value(float u, float v, const point3& p) const = 0;
	};

	class solid_color : public texture
	{
	private:
		color albedo;
	public:
		solid_color(const color& albedo) : albedo(albedo) {}
		solid_color(const float red, float green, float blue) : solid_color(color(red, green, blue)) {}
		color value(float u, float v, const point3& p) const override { return albedo; }
	};

	class checker_texture : public texture
	{
	private:
		float inv_scale;			
		std::shared_ptr<texture> even;
		std::shared_ptr<texture> odd;
	public:
		checker_texture(float scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd)
			:inv_scale(1.0f / scale), even(even), odd(odd) {
		}
		checker_texture(float scale, const color& c1, const color& c2)
			:checker_texture(scale, std::make_shared<solid_color>(c1), std::make_shared<solid_color>(c2)) {
		}

		color value(float u, float v, const point3& p) const override
		{
			int x = int(floor(inv_scale * p.x()));
			int y = int(floor(inv_scale * p.y()));
			int z = int(floor(inv_scale * p.z()));

			bool isEven = (x + y + z) % 2 == 0;
			return isEven ? even->value(u, v, p) : odd->value(u, v, p);
		}
	};

	class image_texture : public texture
	{
	private:
		render_core::rt_image image;
	public:
		image_texture(const char* filename) : image(filename) {}
		color value(float u, float v, const point3& p) const override
		{
			if (image.height() <= 0) { return color(0.0f, 1.0f, 1.0f); }

			u = interval(0.0f, 1.0f).clamp(u);
			v = 1.0f - interval(0.0f, 1.0f).clamp(v);

			int i = int(u * image.width());
			int j = int(v * image.height());
			const uchar* pixel = image.pixel_data(i, j);

			float color_scale = 1.0f / 255.0f;
			return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
		}
	};
}