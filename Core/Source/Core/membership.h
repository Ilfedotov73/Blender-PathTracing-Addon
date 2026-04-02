#pragma once

using membership_func_ptr1 = float (*)(float, float, float, float, float);
using membership_func_ptr2 = float (*)(float, float, float, float);
using membership_func_ptr3 = float (*)(float, float, float);

namespace adaptiv_render {
	float triangular(float x, float mu_a, float mu_b, float mu_c);
	float trapezoidal(float x, float mu_a, float mu_b, float mu_c, float mu_d);
	float z_shaped(float x, float mu_a, float mu_b);
	float s_shaped(float x, float mu_a, float mu_b);
	float pi_shaped(float x, float mu_a, float mu_b, float mu_c, float mu_d);

	class fuzzy_set {
	private:
		membership_func_ptr1 mf_func1;
		membership_func_ptr2 mf_func2;
		membership_func_ptr3 mf_func3;
		float mu_a, mu_b, mu_c, mu_d;
	public:
		fuzzy_set() : mu_a(0), mu_b(0), mu_c(0), mu_d(0), mf_func1(nullptr), mf_func2(nullptr), mf_func3(nullptr) {}
		fuzzy_set(float mu_a_, float mu_b_, float mu_c_, float mu_d_, membership_func_ptr1 func) : 
			mu_a(mu_a_), mu_b(mu_b_), mu_c(mu_c_), mu_d(mu_d_), mf_func1(func), mf_func2(nullptr), mf_func3(nullptr) {}
		fuzzy_set(float mu_a_, float mu_b_, float mu_c_, membership_func_ptr2 func) :
			mu_a(mu_a_), mu_b(mu_b_), mu_c(mu_c_), mu_d(-1), mf_func1(nullptr), mf_func2(func), mf_func3(nullptr) {
		}
		fuzzy_set(float mu_a_, float mu_b_, membership_func_ptr3 func) :
			mu_a(mu_a_), mu_b(mu_b_), mu_c(-1), mu_d(-1), mf_func1(nullptr), mf_func2(nullptr), mf_func3(func) {
		}

		~fuzzy_set() = default;

		float calc(float x) const
		{
			if (mf_func1) { return mf_func1(x, mu_a, mu_b, mu_c, mu_d); }
			else if (mf_func2) { return mf_func2(x, mu_a, mu_b, mu_c); }
			else if (mf_func3) { return mf_func3(x, mu_a, mu_b); }
			return -1.0f;
		}
	};
}