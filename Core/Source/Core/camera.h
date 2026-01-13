#pragma once

#include <iostream>
#include <cmath>

#include "rt_helper.h"
#include "vec3.h"
#include "interval.h"
#include "ray.h"
#include "color.h"
#include "hittable.h"
#include "material.h"

namespace render_core {
	class camera
	{
	private:
		int    IMAGE_HEIGHT;        
		float  PIXEL_SAMPLES_SCALE; 

		point3 CAMERA_CENTER;       
		point3 PIXEL_LOC_00;        
		vec3   PIXEL_DELTA_U;       
		vec3   PIXEL_DELTA_V;       
		vec3   U, W, V;				
		vec3   FOCUS_DISK_U, 		
			   FOCUS_DISK_V;

		void initialize()
		{
			IMAGE_HEIGHT = int(IMAGE_WIDTH / ASPECT_RATIO);
			IMAGE_HEIGHT = (IMAGE_HEIGHT < 1) ? 1 : IMAGE_HEIGHT; 

			CAMERA_CENTER = LOOKFROM;

			float theta = degrees_to_radians(VFOV);
			float h = std::tan(theta / 2.0f);

			float VIEWPORT_HEIGHT = 2.0f * h * FOCUS_DIST;
			float VIEWPORT_WIDTH = VIEWPORT_HEIGHT * (float(IMAGE_WIDTH) / IMAGE_HEIGHT);

			W = unitv(LOOKFROM - LOOKAT);
			U = unitv(cross(VUP, W));
			V = cross(W, U);

			vec3   VIEWPORT_U = VIEWPORT_WIDTH * U;		
			vec3   VIEWPORT_V = -VIEWPORT_HEIGHT * V;	

			PIXEL_DELTA_U = VIEWPORT_U / IMAGE_WIDTH;
			PIXEL_DELTA_V = VIEWPORT_V / IMAGE_HEIGHT;

			point3 VIEWPORT_UPPER_LEFT = CAMERA_CENTER - (FOCUS_DIST * W) - VIEWPORT_U / 2.0f - VIEWPORT_V / 2.0f;
			PIXEL_LOC_00 = VIEWPORT_UPPER_LEFT + 0.5f * (PIXEL_DELTA_U + PIXEL_DELTA_V);

			float focus_radius = FOCUS_DIST * std::tan(degrees_to_radians(FOCUS_ANGLE / 2.0f)); 
			FOCUS_DISK_U = U * focus_radius;
			FOCUS_DISK_V = V * focus_radius;

			PIXEL_SAMPLES_SCALE = 1.0f / SAMPLES_PER_PIXEL;
		}

		color ray_color(const ray& r, int max_depth, const hittable& world) const
		{
			if (max_depth <= 0) { return color(0.0f, 0.0f, 0.0f); }

			hit_record rec;
			if (!world.hit(r, interval(0.001f, INF), rec)) { return background; }
			ray   scattered;	
			color attenuation;	
			color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

			if (!rec.mat->scatter(r, rec, attenuation, scattered)) { return color_from_emission; }
			color color_from_scatter = attenuation * ray_color(scattered, max_depth - 1, world);
			return color_from_emission + color_from_scatter;
		}

		ray get_ray(int i, int j) const
		{
			vec3 offset = sample_square(); 

			point3 pixel_sample = PIXEL_LOC_00 + ((i + offset.x()) * PIXEL_DELTA_U) + ((j + offset.y()) * PIXEL_DELTA_V);
			point3 ray_origin = (FOCUS_ANGLE <= 0) ? CAMERA_CENTER : focus_disk_sample();
			point3 ray_direction = pixel_sample - ray_origin; 

			float ray_time = random_float();
			return ray(ray_origin, ray_direction, ray_time);
		}

		vec3 sample_square() const { return vec3(random_float() - 0.5f, random_float() - 0.5f, 0.0f); }

		point3 focus_disk_sample() const
		{
			vec3 lens = random_in_unit_disk();
			return CAMERA_CENTER + (lens[0] * FOCUS_DISK_U) + (lens[1] * FOCUS_DISK_V);
		}

	public:
		float  ASPECT_RATIO = 1.0f;     				
		int    IMAGE_WIDTH = 100;     				
		int	   SAMPLES_PER_PIXEL = 10;      				
		int	   MAX_DEPTH = 10;						
		float  VFOV = 90.0f;					
		point3 LOOKFROM = point3(0.0f, 0.0f, 0.0f);	
		point3 LOOKAT = point3(0.f, 0.f, -1.f);  	  
		vec3   VUP = vec3(0.f, 1.f, 0.f);		

		float FOCUS_ANGLE = 0.0f;					
		float  FOCUS_DIST = 10.0f;			    	
		color background;

		void render(const hittable& world)
		{
			initialize();
			std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";
			for (int j = 0; j < IMAGE_HEIGHT; ++j) {
				std::clog << "\rScanlines remaining: " << (IMAGE_HEIGHT - j) << ' ' << std::flush;
				for (int i = 0; i < IMAGE_WIDTH; ++i) {
					color pixel_color(0.0f, 0.0f, 0.0f);
					for (int sample = 0; sample < SAMPLES_PER_PIXEL; ++sample) {
						ray r = get_ray(i, j);	
						pixel_color += ray_color(r, MAX_DEPTH, world); 
					}
					write_color(std::cout, PIXEL_SAMPLES_SCALE * pixel_color); 
				}
			}
			std::clog << "\rDone.                 \n";
		}
	};
}
