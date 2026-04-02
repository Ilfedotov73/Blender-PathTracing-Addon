#include "adaptiv_sampling.h"
#include "membership.h"

namespace adaptiv_render {
	void adaptiv_sampling::samples_delta_cal() {
		hit_aggregated();
		render_time_aggregated();
		int delta = rule_base_check();

		if (delta == -1) { return; }
		*samples_per_pixel = delta;
	}

	int adaptiv_sampling::rule_base_check()
	{
		int delta = *samples_per_pixel;
		if (hits_term_aggregated == fuzzy_terms::_ || time_term_aggregated == fuzzy_terms::_) { return -1; }
		switch (hits_term_aggregated) {
			case fuzzy_terms::Empty_area: {
				switch (time_term_aggregated) {
					case fuzzy_terms::Fast_render: {
						delta -= *samples_per_pixel * 0.03f;
						break;
					}
					case fuzzy_terms::Moderate_render: {
						delta -= *samples_per_pixel * 0.05f;
						break;
					}
					case fuzzy_terms::Slow_render: {
						delta -= *samples_per_pixel * 0.07f;
						break;
					}
					break;
				}
				break;
			}
			case fuzzy_terms::Half_filled_area: {
				switch (time_term_aggregated) {
					case fuzzy_terms::Fast_render: {
						break; // -0%
					}
					case fuzzy_terms::Moderate_render: {
						delta -= *samples_per_pixel * 0.01f;
						break;
					}
					case fuzzy_terms::Slow_render: {
						delta -= *samples_per_pixel * 0.02f;
						break;
					}
					break;
				}
				break;
			}
			case fuzzy_terms::Filled_area: {
				switch (time_term_aggregated) {
					case fuzzy_terms::Fast_render: {
						delta += *samples_per_pixel * 0.07f;
						break; 
					}
					case fuzzy_terms::Moderate_render: {
						delta += *samples_per_pixel * 0.05f;
						break;
					}
					case fuzzy_terms::Slow_render: {
						delta += *samples_per_pixel * 0.03f;
						break;
					}
					break;
				}
				break;
			}
			break;
		}
		return delta;
	}

	// Z-shaped.
	float few_hits(int hits_count, int sampls_per_pixel)
	{
		float mu_a = 0.0f;
		float mu_b = sampls_per_pixel * 0.45f;

		fuzzy_set few(mu_a, mu_b, z_shaped);
		return few.calc(hits_count);
	}

	// Triangular.
	float aver_hits(int hits_count, int samples_per_pixel)
	{
		float mu_a = samples_per_pixel * 0.35f;
		float mu_b = samples_per_pixel * 0.5f;
		float mu_c = samples_per_pixel * 0.75;

		fuzzy_set aver(mu_a, mu_b, mu_c, triangular);
		return aver.calc(hits_count);
	}

	// S-shaped.
	float many_hits(int hits_count, int samples_per_pixel)
	{
		float mu_a = samples_per_pixel * 0.65f;
		float mu_b = samples_per_pixel;

		fuzzy_set many(mu_a, mu_b, s_shaped);
		return many.calc(hits_count);
	}

	// Функции принадлежности:
	//	* Z-shaped для опредления принадлежности к области малой заполненности;
	//	* Triangular для определения принадлежности к области средней заполненности;
	//	* S-shaped для определения принадлеждности к области высокой заполненности.
	
	void adaptiv_sampling::hit_aggregated()
	{
		float mu_few = few_hits(*hits_count, *samples_per_pixel);
		float mu_aver = aver_hits(*hits_count, *samples_per_pixel);
		float mu_many = many_hits(*hits_count, *samples_per_pixel);

		if (mu_few > mu_aver) { 
			hits_term_aggregated = mu_few > mu_many ? fuzzy_terms::Empty_area : fuzzy_terms::Filled_area;
		}
		else { 
			hits_term_aggregated = mu_aver > mu_many ? fuzzy_terms::Half_filled_area : fuzzy_terms::Filled_area;
		}
	}
	
	// Z-shaped
	float fast_time(float elapsed_time, float time_limit_per_pixel)
	{
		float mu_a = 0.0f;
		float mu_b = time_limit_per_pixel * 0.45f;

		fuzzy_set fast(mu_a, mu_b, z_shaped);
		return fast.calc(elapsed_time);
	}

	// Pi-shaped
	float moder_time(float elapset_time, float time_limit_per_pixel)
	{
		float mu_a = time_limit_per_pixel * 0.35f;
		float mu_b = time_limit_per_pixel * 0.45f;
		float mu_c = time_limit_per_pixel * 0.55f;
		float mu_d = time_limit_per_pixel * 0.75f;

		fuzzy_set moder(mu_a, mu_b, mu_c, mu_d, pi_shaped);
		return moder.calc(elapset_time);
	}

	// S-shaped
	float slow_time(float elapset_time, float time_limit_per_pixel)
	{
		float mu_a = time_limit_per_pixel * 0.65f;
		float mu_b = time_limit_per_pixel;

		fuzzy_set slow(mu_a, mu_b, s_shaped);
		return slow.calc(elapset_time);
	}

	// Функции принадлежности:
	// * Z-shaped для определения принадлежности к быстрому времени выполнения;
	// * Pi-shaped для опредления принадлежности к среднему времени выполнения;
	// * S-shaped для опредления принадлежности к медленному времени выполнения.
	void adaptiv_sampling::render_time_aggregated()
	{
		float mu_fast = fast_time(*elapsed_time, time_limit_per_pixel);
		float mu_moder = moder_time(*elapsed_time, time_limit_per_pixel);
		float mu_slow = slow_time(*elapsed_time, time_limit_per_pixel);

		if (mu_fast > mu_moder) { 
			time_term_aggregated = mu_fast > mu_slow ? fuzzy_terms::Fast_render: fuzzy_terms::Slow_render;
		}
		else { 
			time_term_aggregated = mu_moder > mu_slow ? fuzzy_terms::Moderate_render : fuzzy_terms::Slow_render;
		}
	}
}