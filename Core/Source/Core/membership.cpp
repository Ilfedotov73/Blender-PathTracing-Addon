#include "membership.h"

namespace adaptiv_render {
	float triangular(float x, float mu_a, float mu_b, float mu_c)
	{
		if (mu_a >= mu_b || mu_b >= mu_c) { return 0.0f; }
		if (x <= mu_a || x >= mu_c) { return 0.0f; }

		if (x <= mu_b) { return (x - mu_a) / (mu_b - mu_a); }
		else { return (mu_c - x) / (mu_c - mu_b); }
	}

	float trapezoidal(float x, float mu_a, float mu_b, float mu_c, float mu_d)
	{
		if (mu_a >= mu_b || mu_b > mu_c || mu_c >= mu_d) { return 0.0f; }
		if (x <= mu_a || x >= mu_d) { return 0.0f; }

		if (x <= mu_b) { return (x - mu_a) / (mu_b - mu_a); }
		else if (x <= mu_c) { return 1.0f; }
		else { return (mu_d - x) / (mu_d - mu_c); }
	}

	float z_shaped(float x, float mu_a, float mu_b)
	{
		if (mu_a >= mu_b) { return 0.0f; }
		if (x <= mu_a) { return 1.0f; }
		if (x >= mu_b) { return 0.0f; }

		float mid = (mu_a + mu_b) / 2.0f;
		if (x <= mid) {
			float t = (x - mu_a) / (mu_b - mu_a);
			return 1.0f - 2.0f * t * t;
		}
		else {
			float t = (mu_b - x) / (mu_b - mu_a);
			return 2.0f * t * t;
		}
	}
	
	float s_shaped(float x, float mu_a, float mu_b)
	{
		if (mu_a >= mu_b) { return 0.0f; }
		if (x <= mu_a) { return 0.0f; }
		if (x >= mu_b) { return 1.0f; }

		float mid = (mu_a + mu_b) / 2.0f;
		if (x <= mid) {
			float t = (x - mu_a) / (mu_b - mu_a);
			return 2.0f * t * t;
		}
		else { 
			float t = (mu_b - x) / (mu_b - mu_a);
			return 1.0f - 2.0f * t * t;
		}
	}
	
	float pi_shaped(float x, float mu_a, float mu_b, float mu_c, float mu_d)
	{
		if (mu_a >= mu_b || mu_b >= mu_c || mu_c >= mu_d) { return 0.0f; }
		if (x <= mu_a || x >= mu_d) { return 0.0f; }

		if (x <= mu_b) { return s_shaped(x, mu_a, mu_b); }
		else if (x <= mu_c) { return 1.0f; }
		else { return z_shaped(x, mu_c, mu_d); }
	}
}