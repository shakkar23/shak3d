#pragma once

#include <array>
#include <cmath>


template<size_t N>
struct vec {
	std::array<float, N> data;

	inline float& operator[](size_t i) { return data[i]; }
	inline float operator[](size_t i) const { return data[i]; }

	// mul by scalar
	inline vec<N> operator*(float s) const {
		vec<N> res;
		for (size_t i = 0; i < N; i++) {
			res[i] = data[i] * s;
		}
		return res;
	}

	// add
	inline vec<N> operator+(const vec<N>& v) const {
		vec<N> res;
		for (size_t i = 0; i < N; i++) {
			res[i] = data[i] + v[i];
		}
		return res;
	}

	// sub
	inline vec<N> operator-(const vec<N>& v) const {
		vec<N> res;
		for (size_t i = 0; i < N; i++) {
			res[i] = data[i] - v[i];
		}
		return res;
	}

	// dot product
	inline float operator*(const vec<N>& v) const {
		float res = 0;
		for (size_t i = 0; i < N; i++) {
			res += data[i] * v[i];
		}
		return res;
	}

	// cross product
	inline vec<N> operator^(const vec<N>& v) const {
		static_assert(N == 3, "Cross product only defined for 3D vectors");
		vec<N> res;
		res[0] = data[1] * v[2] - data[2] * v[1];
		res[1] = data[2] * v[0] - data[0] * v[2];
		res[2] = data[0] * v[1] - data[1] * v[0];
		return res;
	}

	// length
	inline float length() const {
		return sqrt((*this) * (*this));
	}

	// normalize
	inline vec<N> normalize() const {
		return (*this) * (1.0f / length());
	}

	// homogenize
	inline vec<N + 1> homogenize() const {
		vec<N + 1> res;
		for (size_t i = 0; i < N; i++) {
			res[i] = data[i];
		}
		res[N] = 1;
		return res;
	}

};

struct mat4x4 {
	std::array<vec<4>, 4> data;

	inline vec<4>& operator[](size_t i) { return data[i]; }
	inline vec<4> operator[](size_t i) const { return data[i]; }

	// mul by scalar
	inline mat4x4 operator*(float s) const {
		mat4x4 res;
		for (size_t i = 0; i < 4; i++) {
			res[i] = data[i] * s;
		}
		return res;
	}

	// mul by vector
	inline vec<4> operator*(const vec<4>& v) const {
		vec<4> res;
		for (size_t i = 0; i < 4; i++) {
			res[i] = data[i] * v;
		}
		return res;
	}

	// mul by matrix
	inline mat4x4 operator*(const mat4x4& m) const {
		mat4x4 res;
		for (size_t i = 0; i < 4; i++) {
			for (size_t j = 0; j < 4; j++) {
				res[i][j] = data[i][0] * m[0][j] + data[i][1] * m[1][j] + data[i][2] * m[2][j] + data[i][3] * m[3][j];
			}
		}
		return res;
	}

	// identity
	static mat4x4 identity() {
		mat4x4 res;
		for (size_t i = 0; i < 4; i++) {
			for (size_t j = 0; j < 4; j++) {
				res[i][j] = (i == j) ? 1 : 0;
			}
		}
		return res;
	}

	// translation
	inline mat4x4 translate(const vec<3>& v) {
		mat4x4 res = identity();
		for (size_t i = 0; i < 3; i++) {
			res[3][i] = v[i];
		}
		return res;
	}
};