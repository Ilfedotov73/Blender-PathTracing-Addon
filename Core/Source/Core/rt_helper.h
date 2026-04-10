#pragma once

#include <cstdlib>
#include <limits>
#include <vector>
#include <cmath>

namespace render_core {
	const float INF = std::numeric_limits<float>::infinity();
	const float PI = 3.1415926535897932385f;

	inline float degrees_to_radians(float degrees) { return degrees * PI / 180.0f; }

	inline float random_float() { return std::rand() / (RAND_MAX + 1.0f); } 
	
	inline float random_float(float min, float max) { return min + (max - min) * random_float(); } 
	
	inline int random_int(int min, int max) { return int(random_float(min, max + 1)); }

	inline float determinant(std::vector<std::vector<float>> matrix)
	{
		int n = matrix.size();
		float det = 1.0f;

		for (int i = 0; i < n; ++i) {
			int pivot = i;
			for (int j = i + 1; j < n; ++j) {
				if (std::abs(matrix[j][i]) > abs(matrix[pivot][i])) { pivot = j; }
			}
			std::swap(matrix[i], matrix[pivot]);
			if (std::abs(matrix[i][i]) < 1e-9) { return 0.0f; }
			det *= matrix[i][i];

			for (int j = i + 1; j < n; ++j) {
				double factor = matrix[j][i] / matrix[i][i];
				for (int k = i + 1; k < n; ++k) {
					matrix[j][k] -= factor * matrix[i][k];
				}
			}
		}
		return det;
	}
}
