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

	// negate operator
	inline vec<N> operator-() const {
		vec<N> res;
		for (size_t i = 0; i < N; i++) {
			res[i] = -data[i];
		}
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

	// dehomogenize
	inline vec<N - 1> dehomogenize() const {
		vec<N - 1> res;
		for (size_t i = 0; i < N - 1; i++) {
			res[i] = data[i] / data[N - 1];
		}
		return res;
	}
};

struct mat4x4 {
	std::array<vec<4>, 4> data;

	inline vec<4>& operator[](size_t i) { return data[i]; }
	inline vec<4> operator[](size_t i) const { return data[i]; }

	// mul by scalar
	inline mat4x4 operator*(float s) const {
		mat4x4 res{};
		for (size_t i = 0; i < 4; i++) {
			res[i] = data[i] * s;
		}
		return res;
	}

	// mul by vector
	inline vec<4> operator*(const vec<4>& v) const {
		vec<4> res{};
		for (size_t i = 0; i < 4; i++) {
			res[i] = data[i] * v;
		}
		return res;
	}
	inline vec<4> operator*(vec<4>& v) const {
		vec<4> res{};
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
		mat4x4 res{};
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

	// inverse
	inline mat4x4 inverse() const {
		mat4x4 matrix;
		matrix[0][0] = data[0][0]; matrix[0][1] = data[1][0]; matrix[0][2] = data[2][0]; matrix[0][3] = 0.0f;
		matrix[1][0] = data[0][1]; matrix[1][1] = data[1][1]; matrix[1][2] = data[2][1]; matrix[1][3] = 0.0f;
		matrix[2][0] = data[0][2]; matrix[2][1] = data[1][2]; matrix[2][2] = data[2][2]; matrix[2][3] = 0.0f;
		matrix[3][0] = -(data[3][0] * matrix[0][0] + data[3][1] * matrix[1][0] + data[3][2] * matrix[2][0]);
		matrix[3][1] = -(data[3][0] * matrix[0][1] + data[3][1] * matrix[1][1] + data[3][2] * matrix[2][1]);
		matrix[3][2] = -(data[3][0] * matrix[0][2] + data[3][1] * matrix[1][2] + data[3][2] * matrix[2][2]);
		matrix[3][3] = 1.0f;
		return matrix;
	}

	// lookAt
	static mat4x4 lookAt(vec<3> eye, vec<3> center, vec<3> up) {
		vec<3> f = (center - eye).normalize();
		vec<3> u = up.normalize();
		vec<3> s = f ^ u;
		u = s ^ f;

		mat4x4 res = identity();
		for (size_t i = 0; i < 3; i++) {
			res[0][i] = s[i];
			res[1][i] = u[i];
			res[2][i] = -f[i];
		}
		res[3][0] = -s * eye;
		res[3][1] = -u * eye;
		res[3][2] = f * eye;
		return res;
	}

	static mat4x4 rotate_z(float theta) {
		mat4x4 res = identity();
		res[0][0] = cosf(theta);
		res[0][1] = sinf(theta);
		res[1][0] = -sinf(theta);
		res[1][1] = cosf(theta);
		return res;
	}

	static mat4x4 rotate_x(float theta) {
		mat4x4 res = identity();
		res[1][1] = cosf(theta);
		res[1][2] = sinf(theta);
		res[2][1] = -sinf(theta);
		res[2][2] = cosf(theta);
		return res;
	}

	static mat4x4 rotate_y(float theta) {
		mat4x4 res = identity();
		res[0][0] = cosf(theta);
		res[0][2] = -sinf(theta);
		res[2][0] = sinf(theta);
		res[2][2] = cosf(theta);
		return res;
	}
};

inline void MultiplyMatrixVector(vec<3>& i, vec<3>& o, mat4x4& m)
{
	o[0] = i[0] * m[0][0] + i[1] * m[1][0] + i[2] * m[2][0] + m[3][0];
	o[1] = i[0] * m[0][1] + i[1] * m[1][1] + i[2] * m[2][1] + m[3][1];
	o[2] = i[0] * m[0][2] + i[1] * m[1][2] + i[2] * m[2][2] + m[3][2];
	float w = i[0] * m[0][3] + i[1] * m[1][3] + i[2] * m[2][3] + m[3][3];

	if (w != 0.0f)
	{
		o[0] /= w; o[1] /= w; o[2] /= w;
	}
}
