#pragma once

namespace adaptiv_render {
	enum class fuzzy_terms{
		Empty_area,
		Half_filled_area,
		Filled_area,

		Fast_render,
		Moderate_render,
		Slow_render,
		_
	};

	class adaptiv_sampling {
	private:
		float time_limit_per_pixel;	// Значение лимита по времени на рендер пикселя.
		float* elapsed_time;		// Указатель на замер времени рендеринга пикселя.
		int* samples_per_pixel;		// Указатель на регулируемый предел сэмплов на пиксель.
		int* hits_count;			// Указатель на текущее количество пересечение.

		void hit_aggregated();
		void render_time_aggregated();
		int rule_base_check();
	public:
		fuzzy_terms hits_term_aggregated = fuzzy_terms::_; // Результат агрегирования для количества пересечений.
		fuzzy_terms time_term_aggregated = fuzzy_terms::_; // Результат агрегирования для времени рендеринга пикселя.

		adaptiv_sampling() : time_limit_per_pixel(0.0f), elapsed_time(nullptr), hits_count(nullptr), samples_per_pixel(nullptr) {}
		adaptiv_sampling(float time_limit_per_pixel_, float* elapsed_time_, int* hits_count_, int* samples_per_pixel_) :
			time_limit_per_pixel(time_limit_per_pixel_), elapsed_time(elapsed_time_), hits_count(hits_count_), samples_per_pixel(samples_per_pixel_) { }
		~adaptiv_sampling() {
			elapsed_time = nullptr;
			samples_per_pixel = nullptr;
			hits_count = nullptr;
		}
		void samples_delta_cal();
	};
}