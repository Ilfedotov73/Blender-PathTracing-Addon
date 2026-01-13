#include "Core/Core.h"

#include "color.h"
#include "vec3.h"
#include "rt_obj_loader.h"


namespace render_core {
	hittable_list renderer::WORLD;
	camera renderer::CAM;

	void renderer::initialize(scene_data data)
	{
		obj_loader loader;
		loader.load_models(data.obj_filaname, data.obj_filedir);
		WORLD.add(loader.get_sides());

		CAM.ASPECT_RATIO = data.aspect_ratio;
		CAM.IMAGE_WIDTH = data.image_width;
		CAM.SAMPLES_PER_PIXEL = data.samples_per_pixel;
		CAM.MAX_DEPTH = data.max_depth;
		CAM.VFOV = data.vfov;
		CAM.FOCUS_ANGLE = data.focus_angle;
		CAM.FOCUS_DIST = data.focus_dist;

		CAM.background = color(data.background[0], data.background[1], data.background[2]);
		CAM.LOOKFROM = point3(data.lookfrom[0], data.lookfrom[1], data.lookfrom[2]);
		CAM.LOOKAT = vec3(data.lookat[0], data.lookat[1], data.lookat[2]);
		CAM.VUP = vec3(data.vup[0], data.vup[1], data.vup[2]);
	}
	void renderer::render_run(scene_data data)
	{
		initialize(data);
		CAM.render(WORLD);
	}
}