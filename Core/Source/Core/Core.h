#pragma once

#include "camera.h"
#include "hittable_list.h"

namespace render_core {
	struct scene_data
	{
		const char* objfilename,
				  * mtlsdir;

		float	aspect_ratio,
				image_width,
				samples_per_pixel,
				max_depth,
				vfov,
				focus_angle,
				focus_dist,
				time_limit_per_pixel;
				
		float	background[3],
				lookfrom[3],
				lookat[3],
				vup[3];

		bool	adapting_rendering;
		bool	ssao = false;
	};
	class renderer {
	public:
		static hittable_list WORLD;
		static camera CAM;
		static void render_run(scene_data data);
	private:
		static void initialize(scene_data data);
	};
}