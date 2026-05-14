#include <time.h>
#include <iostream>]
#include <vector>

#include "Core/Core.h"
#include <Core/rt_obj_loader.h>

void test1(render_core::hittable_list &world, std::vector<float> &timetable);
void test2(render_core::hittable_list &world, std::vector<float> &timetable);
void test3(render_core::hittable_list &world, std::vector<float> &timetable);
void test4(render_core::hittable_list &world, std::vector<float> &timetable);
void test5(render_core::hittable_list &world, std::vector<float> &timetable);
void test6(render_core::hittable_list &world, std::vector<float> &timetable);

int main()
{
	render_core::hittable_list world;

	render_core::obj_loader loader;
	loader.load_models("scene.obj", "C:/Dev/Blender-PathTracing-Addon/Tests");
	world.add(loader.get_sides());

	std::vector<float> timetable;

	test1(world, timetable);
	test2(world, timetable);
	test3(world, timetable);
	test4(world, timetable);
	test5(world, timetable);
	test6(world, timetable);

	int sz = timetable.size();
	std::cerr << "Adaptiv sampling - OFF: " << '\n';
	for (int i = 0; i < sz; i += 2) {
		std::cerr << timetable[i] << '\n';
	}
	std::cerr << "Adaptiv sampling - ON: " << '\n';
	for (int i = 1; i < sz; i += 2) {
		std::cerr << timetable[i] << '\n';
	}
	std::cerr << "Difference: " << '\n'; 
	for (int i = 0; i < sz; i += 2) {
		std::cerr << timetable[i] - timetable[i + 1] << '\n';
	}
	std::cerr << std::endl;
}

void test1(render_core::hittable_list &World, std::vector<float>& timetable)
{
	std::cerr << '\t' << "_____Start test1_____" << "\n";
	std::cerr << "Engine settings: " << '\n';

	render_core::camera cam;

	cam.ASPECT_RATIO = 1.0f;
	cam.IMAGE_WIDTH = 400.0f;
	cam.SAMPLES_PER_PIXEL = 100.0f;
	cam.MAX_DEPTH = 50.0f;
	cam.VFOV = 40.0f;
	cam.LOOKFROM = render_core::point3(7.358891487121582f, 4.958309173583984f, 6.925790786743164f);
	cam.LOOKAT = render_core::point3(-0.6515582203865051f, -0.44527140259742737f, -0.6141703724861145f);
	cam.VUP = render_core::vec3(-0.32401347160339355f, 0.8953956365585327f, -0.305420845746994f);
	cam.FOCUS_ANGLE = 0.0f;
	cam.FOCUS_DIST = 10.0f;

	cam.TIME_LIMIT_PER_PIXEL = 120;
	cam.ADAPTING_RENDERING = false;

	std::cerr << "Samples per pixel: " << cam.SAMPLES_PER_PIXEL << '\n' << "Max ray reflect depth: " << cam.MAX_DEPTH << '\n'
		<< "Time limit per pixel: " << cam.TIME_LIMIT_PER_PIXEL << '\n' << "Adaptiv rendering mode: " << cam.ADAPTING_RENDERING << '\n'
		<< '\n' << cam.IMAGE_WIDTH << 'x' << cam.IMAGE_WIDTH / cam.ASPECT_RATIO << '\n';

	time_t start_time, end_time;

	start_time = time(NULL);
	cam.render(World);
	end_time = time(NULL);

	float render_time = difftime(end_time, start_time);
	timetable.push_back(render_time);
	std::cerr << render_time << '\n';
}
void test2(render_core::hittable_list& World, std::vector<float>& timetable)
{
	std::cerr << '\t' << "_____Start test2_____" << "\n";
	std::cerr << "Engine settings: " << '\n';

	render_core::camera cam;

	cam.ASPECT_RATIO = 1.0f;
	cam.IMAGE_WIDTH = 400.0f;
	cam.SAMPLES_PER_PIXEL = 100.0f;
	cam.MAX_DEPTH = 50.0f;
	cam.VFOV = 40.0f;
	cam.LOOKFROM = render_core::point3(7.358891487121582f, 4.958309173583984f, 6.925790786743164f);
	cam.LOOKAT = render_core::point3(-0.6515582203865051f, -0.44527140259742737f, -0.6141703724861145f);
	cam.VUP = render_core::vec3(-0.32401347160339355f, 0.8953956365585327f, -0.305420845746994f);
	cam.FOCUS_ANGLE = 0.0f;
	cam.FOCUS_DIST = 10.0f;

	cam.TIME_LIMIT_PER_PIXEL = 120;
	cam.ADAPTING_RENDERING = true;

	std::cerr << "Samples per pixel: " << cam.SAMPLES_PER_PIXEL << '\n' << "Max ray reflect depth: " << cam.MAX_DEPTH << '\n'
		<< "Time limit per pixel: " << cam.TIME_LIMIT_PER_PIXEL << '\n' << "Adaptiv rendering mode: " << cam.ADAPTING_RENDERING << '\n'
		<< '\n' << cam.IMAGE_WIDTH << 'x' << cam.IMAGE_WIDTH / cam.ASPECT_RATIO << '\n';

	time_t start_time, end_time;

	start_time = time(NULL);
	cam.render(World);
	end_time = time(NULL);

	float render_time = difftime(end_time, start_time);
	timetable.push_back(render_time);
	std::cerr << render_time << '\n';
}
void test3(render_core::hittable_list& World, std::vector<float>& timetable)
{
	std::cerr << '\t' << "_____Start test3_____" << "\n";
	std::cerr << "Engine settings: " << '\n';

	render_core::camera cam;

	cam.ASPECT_RATIO = 1.0f;
	cam.IMAGE_WIDTH = 600.0f;
	cam.SAMPLES_PER_PIXEL = 150.0f;
	cam.MAX_DEPTH = 50.0f;
	cam.VFOV = 40.0f;
	cam.LOOKFROM = render_core::point3(7.358891487121582f, 4.958309173583984f, 6.925790786743164f);
	cam.LOOKAT = render_core::point3(-0.6515582203865051f, -0.44527140259742737f, -0.6141703724861145f);
	cam.VUP = render_core::vec3(-0.32401347160339355f, 0.8953956365585327f, -0.305420845746994f);
	cam.FOCUS_ANGLE = 0.0f;
	cam.FOCUS_DIST = 10.0f;

	cam.TIME_LIMIT_PER_PIXEL = 60;
	cam.ADAPTING_RENDERING = false;

	std::cerr << "Samples per pixel: " << cam.SAMPLES_PER_PIXEL << '\n' << "Max ray reflect depth: " << cam.MAX_DEPTH << '\n'
		<< "Time limit per pixel: " << cam.TIME_LIMIT_PER_PIXEL << '\n' << "Adaptiv rendering mode: " << cam.ADAPTING_RENDERING << '\n'
		<< '\n' << cam.IMAGE_WIDTH << 'x' << cam.IMAGE_WIDTH / cam.ASPECT_RATIO << '\n';

	time_t start_time, end_time;

	start_time = time(NULL);
	cam.render(World);
	end_time = time(NULL);

	float render_time = difftime(end_time, start_time);
	timetable.push_back(render_time);
	std::cerr << render_time << '\n';
}
void test4(render_core::hittable_list& World, std::vector<float>& timetable)
{
	std::cerr << '\t' << "_____Start test4_____" << "\n";
	std::cerr << "Engine settings: " << '\n';

	render_core::camera cam;

	cam.ASPECT_RATIO = 1.0f;
	cam.IMAGE_WIDTH = 600.0f;
	cam.SAMPLES_PER_PIXEL = 150.0f;
	cam.MAX_DEPTH = 50.0f;
	cam.VFOV = 40.0f;
	cam.LOOKFROM = render_core::point3(7.358891487121582f, 4.958309173583984f, 6.925790786743164f);
	cam.LOOKAT = render_core::point3(-0.6515582203865051f, -0.44527140259742737f, -0.6141703724861145f);
	cam.VUP = render_core::vec3(-0.32401347160339355f, 0.8953956365585327f, -0.305420845746994f);
	cam.FOCUS_ANGLE = 0.0f;
	cam.FOCUS_DIST = 10.0f;

	cam.TIME_LIMIT_PER_PIXEL = 60;
	cam.ADAPTING_RENDERING = true;

	std::cerr << "Samples per pixel: " << cam.SAMPLES_PER_PIXEL << '\n' << "Max ray reflect depth: " << cam.MAX_DEPTH << '\n'
		<< "Time limit per pixel: " << cam.TIME_LIMIT_PER_PIXEL << '\n' << "Adaptiv rendering mode: " << cam.ADAPTING_RENDERING << '\n'
		<< '\n' << cam.IMAGE_WIDTH << 'x' << cam.IMAGE_WIDTH / cam.ASPECT_RATIO << '\n';

	time_t start_time, end_time;

	start_time = time(NULL);
	cam.render(World);
	end_time = time(NULL);

	float render_time = difftime(end_time, start_time);
	timetable.push_back(render_time);
	std::cerr << render_time << '\n';
}
void test5(render_core::hittable_list& World, std::vector<float>& timetable)
{

	std::cerr << '\t' << "_____Start test5_____" << "\n";
	std::cerr << "Engine settings: " << '\n';

	render_core::camera cam;

	cam.ASPECT_RATIO = 1.0f;
	cam.IMAGE_WIDTH = 800.0f;
	cam.SAMPLES_PER_PIXEL = 300.0f;
	cam.MAX_DEPTH = 50.0f;
	cam.VFOV = 40.0f;
	cam.LOOKFROM = render_core::point3(7.358891487121582f, 4.958309173583984f, 6.925790786743164f);
	cam.LOOKAT = render_core::point3(-0.6515582203865051f, -0.44527140259742737f, -0.6141703724861145f);
	cam.VUP = render_core::vec3(-0.32401347160339355f, 0.8953956365585327f, -0.305420845746994f);
	cam.FOCUS_ANGLE = 0.0f;
	cam.FOCUS_DIST = 10.0f;

	cam.TIME_LIMIT_PER_PIXEL = 35;
	cam.ADAPTING_RENDERING = false;

	std::cerr << "Samples per pixel: " << cam.SAMPLES_PER_PIXEL << '\n' << "Max ray reflect depth: " << cam.MAX_DEPTH << '\n'
		<< "Time limit per pixel: " << cam.TIME_LIMIT_PER_PIXEL << '\n' << "Adaptiv rendering mode: " << cam.ADAPTING_RENDERING << '\n'
		<< '\n' << cam.IMAGE_WIDTH << 'x' << cam.IMAGE_WIDTH / cam.ASPECT_RATIO << '\n';

	time_t start_time, end_time;

	start_time = time(NULL);
	cam.render(World);
	end_time = time(NULL);

	float render_time = difftime(end_time, start_time);
	timetable.push_back(render_time);
	std::cerr << render_time << '\n';
}
void test6(render_core::hittable_list& World, std::vector<float>& timetable)
{

	std::cerr << '\t' << "_____Start test6_____" << "\n";
	std::cerr << "Engine settings: " << '\n';

	render_core::camera cam;

	cam.ASPECT_RATIO = 1.0f;
	cam.IMAGE_WIDTH = 800.0f;
	cam.SAMPLES_PER_PIXEL = 300.0f;
	cam.MAX_DEPTH = 50.0f;
	cam.VFOV = 40.0f;
	cam.LOOKFROM = render_core::point3(7.358891487121582f, 4.958309173583984f, 6.925790786743164f);
	cam.LOOKAT = render_core::point3(-0.6515582203865051f, -0.44527140259742737f, -0.6141703724861145f);
	cam.VUP = render_core::vec3(-0.32401347160339355f, 0.8953956365585327f, -0.305420845746994f);
	cam.FOCUS_ANGLE = 0.0f;
	cam.FOCUS_DIST = 10.0f;

	cam.TIME_LIMIT_PER_PIXEL = 35;
	cam.ADAPTING_RENDERING = true;

	std::cerr << "Samples per pixel: " << cam.SAMPLES_PER_PIXEL << '\n' << "Max ray reflect depth: " << cam.MAX_DEPTH << '\n'
		<< "Time limit per pixel: " << cam.TIME_LIMIT_PER_PIXEL << '\n' << "Adaptiv rendering mode: " << cam.ADAPTING_RENDERING << '\n'
		<< '\n' << cam.IMAGE_WIDTH << 'x' << cam.IMAGE_WIDTH / cam.ASPECT_RATIO << '\n';

	time_t start_time, end_time;

	start_time = time(NULL);
	cam.render(World);
	end_time = time(NULL);

	float render_time = difftime(end_time, start_time);
	timetable.push_back(render_time);
	std::cerr << render_time << '\n';
}