#include <string>
#include <iostream>
#include <stdlib.h>

#include "Core/Core.h"

int main(int argc, char* argv[])
{
	if (argc > 1) {
		try {
			render_core::scene_data s_data = {};

			// obj filename & filedir
			s_data.obj_filaname = argv[1];
			s_data.obj_filedir = argv[2];

			for (int i = 3; i < argc; ++i) {
				std::string arg = argv[i];

				// Render settings
				if (arg == "--asp_ratio") { s_data.aspect_ratio = std::stof(argv[++i]); }
				else if (arg == "--image_width") { s_data.image_width = std::stof(argv[++i]); }
				else if (arg == "--samples") { s_data.samples_per_pixel = std::stof(argv[++i]); }
				else if (arg == "--max_depth") { s_data.max_depth = std::stof(argv[++i]); }
				else if (arg == "--vfov") { s_data.vfov = std::stof(argv[++i]); }
				else if (arg == "--focus_angle") { s_data.focus_angle = std::stof(argv[++i]); }
				else if (arg == "--focus_dist") { s_data.focus_dist = std::stof(argv[++i]); }

				// World settings
				// - background 
				else if (arg == "--b1") { s_data.background[0] = std::stof(argv[++i]); }
				else if (arg == "--b2") { s_data.background[1] = std::stof(argv[++i]); }
				else if (arg == "--b3") { s_data.background[2] = std::stof(argv[++i]); }
			
				// - camera settings
				// lookfrom
				else if (arg == "--cam_pos_x") { s_data.lookfrom[0] = std::stof(argv[++i]); }
				else if (arg == "--cam_pos_y") { s_data.lookfrom[1] = std::stof(argv[++i]); }
				else if (arg == "--cam_pos_z") { s_data.lookfrom[2] = std::stof(argv[++i]); }

				// lookat
				else if (arg == "--cam_dir_x") { s_data.lookat[0] = std::stof(argv[++i]); }
				else if (arg == "--cam_dir_y") { s_data.lookat[1] = std::stof(argv[++i]); }
				else if (arg == "--cam_dir_z") { s_data.lookat[2] = std::stof(argv[++i]); }

				// vup
				else if (arg == "--vup_x") { s_data.vup[0] = std::stof(argv[++i]); }
				else if (arg == "--vup_y") { s_data.vup[1] = std::stof(argv[++i]); }
				else if (arg == "--vup_z") { s_data.vup[2] = std::stof(argv[++i]); }
			}

			render_core::renderer::render_run(s_data);
		}
		catch (std::exception e) {
			std::cerr << "Error: " << e.what() << '\n';
		}
	}
}