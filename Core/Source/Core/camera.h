#pragma once

#include <iostream>
#include <cmath>
#include <time.h>
#include <unordered_map>
#include <vector>

#include "rt_helper.h"
#include "vec3.h"
#include "interval.h"
#include "ray.h"
#include "color.h"
#include "hittable.h"
#include "material.h"
#include "adaptiv_sampling.h"

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

		mutable int hits_count = 0;
		mutable int sqrt_spp;
		mutable float recip_sqrt_spp;

		float MAX_SHADING_DIST = 1.0f;

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

			//PIXEL_SAMPLES_SCALE = 1.0f / SAMPLES_PER_PIXEL;
		}

		color ray_color(const ray& r, int max_depth, const hittable& world) const
		{
			if (max_depth <= 0) { return color(0.0f, 0.0f, 0.0f); }

			hit_record rec;
			if (!world.hit(r, interval(0.001f, INF), rec)) { return background; }
			if (max_depth == MAX_DEPTH) { ++hits_count; }
			ray   scattered;	
			color attenuation;	
			color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

			if (!rec.mat->scatter(r, rec, attenuation, scattered)) { return color_from_emission; }
			color color_from_scatter = attenuation * ray_color(scattered, max_depth - 1, world);
			return color_from_emission + color_from_scatter;
		}

		ray ssao_get_ray(int i, int j) const
		{
			point3 pixel_center = PIXEL_LOC_00 + (i * PIXEL_DELTA_U) + (j * PIXEL_DELTA_V);
			point3 ray_origin = CAMERA_CENTER;
			point3 ray_direction = pixel_center - ray_origin;

			return ray(ray_origin, ray_direction);
		}

		void make_shading(std::vector<hit_point>& hit_point_table)
		{
			int limit = 50000000;
			int size = IMAGE_WIDTH * IMAGE_HEIGHT;
			for (int i = 0; i < limit;) {
				int key1 = random_int(0, size);
				int key2 = random_int(0, size);

				hit_point& p1 = hit_point_table[key1];
				hit_point& p2 = hit_point_table[key2];

				if (p1.get_is_hit() && p2.get_is_hit()) { 
					vec3 v1 = p1.normal - p1.p;
					vec3 v2 = p2.p - p1.p;
					vec3 v3 = p2.normal - p1.p;

					std::vector<std::vector<float>> matrix = {
						{v1[0], v1[1], v1[2]},
						{v2[0], v2[1], v2[2]},
						{v3[0], v3[1], v3[2]}
					};

					if (determinant(matrix) == 0) { continue; }

					float dist = distance(p1.p, p2.p);
					float intensity = std::min(dist / MAX_SHADING_DIST, 1.0f);

					p1.intens = p1.intens > intensity ? intensity : p1.intens;
					//p2.intens = p2.intens > intensity ? intensity : p2.intens;

					++i;
				}
			}
		}

		ray get_ray(int i, int j, int s_i, int s_j) const
		{
			vec3 offset = sample_square_stratified(s_i, s_j);

			point3 pixel_sample = PIXEL_LOC_00 + ((i + offset.x()) * PIXEL_DELTA_U) + ((j + offset.y()) * PIXEL_DELTA_V);
			point3 ray_origin = (FOCUS_ANGLE <= 0) ? CAMERA_CENTER : focus_disk_sample();
			point3 ray_direction = pixel_sample - ray_origin; 

			float ray_time = random_float();
			return ray(ray_origin, ray_direction, ray_time);
		}

		vec3 sample_square() const { return vec3(random_float() - 0.5f, random_float() - 0.5f, 0.0f); }

		vec3 sample_square_stratified(int s_i, int s_j) const
		{
			float px = ((s_i + random_float()) * recip_sqrt_spp) - 0.5f;
			float py = ((s_j + random_float()) * recip_sqrt_spp) - 0.5f;

			return vec3(px, py, 0);
		}

		point3 focus_disk_sample() const
		{
			vec3 lens = random_in_unit_disk();
			return CAMERA_CENTER + (lens[0] * FOCUS_DISK_U) + (lens[1] * FOCUS_DISK_V);
		}

	public:
		float  ASPECT_RATIO = 1.0f;     				
		int    IMAGE_WIDTH = 100;     			
		float  TIME_LIMIT_PER_PIXEL = 120;
		int	   SAMPLES_PER_PIXEL = 10;      				
		int	   MAX_DEPTH = 10;						
		float  VFOV = 90.0f;					
		point3 LOOKFROM = point3(0.0f, 0.0f, 0.0f);	
		point3 LOOKAT = point3(0.f, 0.f, -1.f);  	  
		vec3   VUP = vec3(0.f, 1.f, 0.f);		
		
		bool   ADAPTING_RENDERING = true;

		float FOCUS_ANGLE = 0.0f;					
		float FOCUS_DIST = 10.0f;			    	
		color background;

		void ssao_render(const hittable& world)
		{
			std::cerr << "Start SSAO rendering" << '\n';
			initialize();
			std::vector<hit_point> hit_point_table;

			for (int j = 0; j < IMAGE_HEIGHT; ++j) {
				for (int i = 0; i < IMAGE_WIDTH; ++i) {
					ray r = ssao_get_ray(i, j);

					hit_record rec;
					if (!world.hit(r, interval(0.001f, INF), rec)) { 
						hit_point_table.push_back(hit_point(false));
						continue; 
					}

					hit_point_table.push_back(hit_point(true, rec.p, rec.normal));
				}
			}

			make_shading(hit_point_table);

			std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";
			int k = 0;
			for (int j = 0; j < IMAGE_HEIGHT; ++j) {
				std::clog << "\rScanlines remaining: " << (IMAGE_HEIGHT - j) << ' ' << std::flush;
				for (int i = 0; i < IMAGE_WIDTH; ++i) {
					float pixel_intens = hit_point_table[k++].intens;
					write_color(std::cout, color(pixel_intens, pixel_intens, pixel_intens));
				}
			}
			std::clog << "\rDone.                 \n";
		}

		void render(const hittable& world)
		{
			float elapsed_time;
			int samples_limit;
			time_t start_time, end_time;

			initialize();
			adaptiv_render::adaptiv_sampling ads_(TIME_LIMIT_PER_PIXEL, &elapsed_time, &hits_count, &samples_limit);
			std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";
			for (int j = 0; j < IMAGE_HEIGHT; ++j) {
				std::clog << "\rScanlines remaining: " << (IMAGE_HEIGHT - j) << ' ' << std::flush;
				for (int i = 0; i < IMAGE_WIDTH; ++i) {
					color pixel_color(0.0f, 0.0f, 0.0f);
					samples_limit = SAMPLES_PER_PIXEL;

					sqrt_spp = int(std::sqrt(samples_limit));
					recip_sqrt_spp = 1.0f / sqrt_spp;

					for (int s_j = 0; s_j < sqrt_spp; ++s_j) {
						for (int s_i = 0; s_i < sqrt_spp; ++s_i) {
							ray r = get_ray(i, j, s_i, s_j);

							start_time = time(NULL);
							pixel_color += ray_color(r, MAX_DEPTH, world);
							end_time = time(NULL);

							if (ADAPTING_RENDERING) {
								elapsed_time = difftime(end_time, start_time);
								ads_.samples_delta_cal();
								if (samples_limit < 0) { samples_limit = 0; }
								else if (samples_limit > SAMPLES_PER_PIXEL) { samples_limit = SAMPLES_PER_PIXEL; }
								
								sqrt_spp = int(std::sqrt(samples_limit));
								recip_sqrt_spp = 1.0f / sqrt_spp;
							}
						}
					}
					hits_count = 0;
					PIXEL_SAMPLES_SCALE = 1.0f / (sqrt_spp * sqrt_spp);
					write_color(std::cout, PIXEL_SAMPLES_SCALE * pixel_color); 
				}
			}
			std::clog << "\rDone.                 \n";
		}
	};
}
