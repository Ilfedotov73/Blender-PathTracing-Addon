#pragma once

#include "camera.h"
#include "hittable_list.h"

namespace render_core {
	struct scene_data
	{
		const char* obj_filaname,
				  * obj_filedir;

		float	aspect_ratio,
				image_width,
				samples_per_pixel,
				max_depth,
				vfov,
				focus_angle,
				focus_dist;
				
		float	background[3],
				lookfrom[3],
				lookat[3],
				vup[3];
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