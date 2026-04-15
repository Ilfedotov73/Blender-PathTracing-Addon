#include <time.h>
#include <memory>
#include <iostream>

#include "Core/rt_obj_loader.h"
#include "Core/camera.h"
#include "Core/material.h"
#include "Core/vec3.h"

void test_scene_1(render_core::hittable_list WORLD)
{
	render_core::camera cam;

	cam.ASPECT_RATIO = 2.0f;
	cam.IMAGE_WIDTH = 400.0f;
	cam.SAMPLES_PER_PIXEL = 25.0f;
	cam.MAX_DEPTH = 15.0f;
	cam.VFOV = 40.0f;
	cam.FOCUS_ANGLE = 0.0f;
	cam.FOCUS_DIST = 10.0f;
	cam.TIME_LIMIT_PER_PIXEL = 0.0f;

	cam.background = render_core::color(0.83f, 0.25f, 0.96f);
	cam.LOOKFROM = render_core::point3(8.0f, 5.0f, 5.0f);
	cam.LOOKAT = render_core::point3(0.0f, 0.0f, 0.0f);
	cam.VUP = render_core::vec3(0.0f, 1.0f, 0.0f);

	cam.ADAPTING_RENDERING = false;

	std::cerr << "Start test №1:" << '\n';
	std::cerr << "Settings: " << '\n';
	std::cerr << "* Aspect_ratio: " << cam.ASPECT_RATIO << '\n';
	std::cerr << "* Image_width: " << cam.IMAGE_WIDTH<< '\n';
	std::cerr << "* Samples_per_pixel: " << cam.SAMPLES_PER_PIXEL<< '\n';
	std::cerr << "* Max_depth: " << cam.MAX_DEPTH<< '\n';
	std::cerr << "* Vfov: " << cam.VFOV<< '\n';
	std::cerr << "* Focus_angle: " << cam.FOCUS_ANGLE<< '\n';
	std::cerr << "* Focus_dist: " << cam.FOCUS_DIST << '\n';
	std::cerr << "* Time_limit_per_pixel: " << cam.TIME_LIMIT_PER_PIXEL << '\n';

	std::cerr << "Start rendering:" << '\n';

	time_t start, stop;

	start = clock();
	cam.render(WORLD);
	stop = clock();

	double elapsed_time = double(start - stop) / CLOCKS_PER_SEC;
	std::cout << "Время выполнения: " << elapsed_time << " секунд\n";
}

void test_scene_2(render_core::hittable_list WORLD)
{
	render_core::camera cam;

	cam.ASPECT_RATIO = 2.0f;
	cam.IMAGE_WIDTH = 600.0f;
	cam.SAMPLES_PER_PIXEL = 50.0f;
	cam.MAX_DEPTH = 30.0f;
	cam.VFOV = 40.0f;
	cam.FOCUS_ANGLE = 0.0f;
	cam.FOCUS_DIST = 10.0f;
	cam.TIME_LIMIT_PER_PIXEL = 0.0f;

	cam.background = render_core::color(0.83f, 0.25f, 0.96f);
	cam.LOOKFROM = render_core::point3(8.0f, 5.0f, 5.0f);
	cam.LOOKAT = render_core::point3(0.0f, 0.0f, 0.0f);
	cam.VUP = render_core::vec3(0.0f, 1.0f, 0.0f);

	cam.ADAPTING_RENDERING = false;

	std::cerr << "Start test №1:" << '\n';
	std::cerr << "Settings: " << '\n';
	std::cerr << "* Aspect_ratio: " << cam.ASPECT_RATIO << '\n';
	std::cerr << "* Image_width: " << cam.IMAGE_WIDTH << '\n';
	std::cerr << "* Samples_per_pixel: " << cam.SAMPLES_PER_PIXEL << '\n';
	std::cerr << "* Max_depth: " << cam.MAX_DEPTH << '\n';
	std::cerr << "* Vfov: " << cam.VFOV << '\n';
	std::cerr << "* Focus_angle: " << cam.FOCUS_ANGLE << '\n';
	std::cerr << "* Focus_dist: " << cam.FOCUS_DIST << '\n';
	std::cerr << "* Time_limit_per_pixel: " << cam.TIME_LIMIT_PER_PIXEL << '\n';

	std::cerr << "Start rendering:" << '\n';

	time_t start, stop;

	start = clock();
	cam.render(WORLD);
	stop = clock();

	double elapsed_time = double(start - stop) / CLOCKS_PER_SEC;
	std::cout << "Время выполнения: " << elapsed_time << " секунд\n";
}

int main()
{
	std::cerr << "Start Benchmark" << '\n';
	std::cerr << "Load scene:" << '\n';

	render_core::hittable_list WORLD;
	render_core::obj_loader loader;
	try {
		loader.load_models("TestScene.obj", "C:\Dev\Blender-PathTracing-Addon\Tests");
		WORLD.add(loader.get_sides());


	}
	catch (std::exception e) {
		std::cerr << "Error: " << e.what() << '\n';
	}

	test_scene_1(WORLD);
	test_scene_2(WORLD);

	std::cerr << "Benchmark stop!" << '\n';
}