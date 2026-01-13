#include <iostream>
#include <stdlib.h>

#include "Core/Core.h"

int main(int argc, char* argv[])
{
	if (argc > 1) {
		try {
			render_core::scene_data s_data = {
				.obj_filaname = argv[1],
				.obj_filedir = argv[2],

				.aspect_ratio = std::stof(argv[3]),
				.image_width = std::stof(argv[4]),
				.samples_per_pixel = std::stof(argv[5]),
				.max_depth = std::stof(argv[6]),
				.vfov = std::stof(argv[7]),
				.focus_angle = std::stof(argv[8]),
				.focus_dist = std::stof(argv[9]),

				.background = { std::stof(argv[10]), std::stof(argv[11]), std::stof(argv[12]) },
				.lookfrom = { std::stof(argv[13]), std::stof(argv[14]), std::stof(argv[15]) },
				.lookat = { std::stof(argv[16]), std::stof(argv[17]), std::stof(argv[18]) },
				.vup = { std::stof(argv[19]), std::stof(argv[20]), std::stof(argv[21]) }
			};

			render_core::renderer::render_run(s_data);
		}
		catch (std::exception e) {
			std::cerr << "Error: " << e.what() << '\n';
		}
	}
}