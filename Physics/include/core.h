#ifndef CORE_HEADER

#define CORE_HEADER

#include "precision.h"

namespace AE86 {
	/**
	 * Holds a vector in three dimensions. Four data members are allocated
	 * to ensure alignment in an array.
	*/
	class Vector3 {
	public:
		/** Holds the value along the x axis. */
		real x;

		/** Holds the value along the y axis. */
		real y;

		/** Holds the value along the z axis. */
		real z;

		/** Gets the magnitude of this vector. */
		real magnitude() const {
			return real_sqrt(squareMagnitude());
		}

		/** Gets the squared magnitude of this vector */
		real squareMagnitude() const {
			return x * x + y * y + z * z;
		}

		/** Turns a non-zero vector into a vector of unit length. */
		void normalize() {
			real l = magnitude();
			if (l > 0) {
				(*this) *= ((real)1) / l;
			}
		}

		/* Multiplies this vector by the given scalar. */
		void operator *= (const real value) {
			x *= value;
			y *= value;
			z *= value;
		}

		/* Returns a copy of this vector scaled the given value. */
		Vector3 operator*(const real value) const {
			return Vector3(x * value, y * value, z * value);
		}

		/** Adds the given vector to this. */
		void operator+=(const Vector3& v) {
			x += v.x;
			y += v.y;
			z += v.z;
		}

		/** Returns the value of the given vector added to this. */
		Vector3 operator+(const Vector3& v) const {
			return Vector3(x + v.x, y + v.y, z + v.z);
		}

		/** Subtracts the given vector from this. */
		void operator-=(const Vector3& v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
		}

		/**
		 * Returns the value of the given vector subtracted from this.
		*/
		Vector3 operator-(const Vector3& v) const {
			return Vector3(x - v.x, y - v.y, z - v.z);
		}

		/**
		 * Adds the given vector to this, scaled by the given amount.
		*/
		void addScaledVector(const Vector3& vector, real scale) {
			x += vector.x * scale;
			y += vector.y * scale;
			z += vector.z * scale;
		}

		/**
		 * Calculates and returns a component-wise product of this
		 * vector with the given vector.
		*/
		Vector3 componentProduct(const Vector3& vector) const {
			return Vector3(x * vector.x, y * vector.y, z * vector.z);
		}

		/**
		 * Performs a component-wise product with the given vector and
		 * sets this vector to its result.
		 */
		void componentProductUpdate(const Vector3& vector) {
			x *= vector.x;
			y *= vector.y;
			z *= vector.z;
		}

		/**
		 * Calculates and returns the scalar product of this vector
		 * with the given vector.
		 */
		real scalarProduct(const Vector3& vector) const {
			return x * vector.x + y * vector.y + z * vector.z;
		}

		/**
		 * Calculate and returns the scalar product of this vector
		 * with the given vector.
		 */
		real operator *(const Vector3& vector) const {
			return x * vector.x + y * vector.y + z * vector.z;
		}

		/**
		 * Calculates and returns the vector product of this vector
		 * with the given vector.
		 */
		Vector3 vectorProduct(const Vector3& vector) const {
			return Vector3(y * vector.z - z * vector.y,
				z * vector.x - x * vector.z,
				x * vector.y - y * vector.x);
		}

		/**
		 * Updates this vector to be the vector product of its current
		 * value and the given vector.
		 */
		void operator &=(const Vector3& vector) {
			*this = vectorProduct(vector);
		}

		/**
		 * Calculates and returns the vector product of this vector
		 * with the given vector.
		 */
		Vector3 operator%(const Vector3& vector) const {
			return Vector3(y * vector.z - z * vector.y,
				z * vector.x - x * vector.z,
				x * vector.y - y * vector.x);
		}

		/** Zeroes the components of this vector. */
		void clear() {
			x, y, z = 0, 0, 0;
		}

	private:
		/** Padding to ensure four word alignment. On many machines, four floating-point values
		 *  sit cleaner in memory (memory is optimized for four sets of words). The speed ups from
		 *  this padding can be noticeable.
		*/
		real pad;

	public:
		Vector3() : x(0), y(0), z(0), pad(0) {}

		/**
		 * The explicit constructor creates a vector with the given
		 * components.
		 */
		Vector3(const real x, const real y, const real z) : x(x), y(y), z(z), pad(0) {}
		/** Flips all the components of the vector. */
		void invert()
		{
			x = -x;
			y = -y;
			z = -z;
		}
	};

	/**
	 * Holds a 3 x 3 row major matrix representing a transformation in
	 * 3D space that does not include a translational component. This
	 * matrix is not padded to produce an aligned structure.
	 */
	class Matrix3 {
	public:
		/**
		 *
		 * Holds the tensor matrix data in array form.
		 */
		real data[9];
		/**
		 * Creates a new matrix.
		 */
		Matrix3() : data{} {
		}
		/**
		 * Creates a new matrix with the given three vectors making
		 * up its columns.
		 */
		Matrix3(const Vector3& compOne, const Vector3& compTwo,
			const Vector3& compThree) : data{}
		{
			setComponents(compOne, compTwo, compThree);
		}
		/**
		 * Creates a new matrix with explicit coefficients.
		 */
		Matrix3(real c0, real c1, real c2, real c3, real c4, real c5,
			real c6, real c7, real c8) : data{}
		{
			data[0] = c0; data[1] = c1; data[2] = c2;
			data[3] = c3; data[4] = c4; data[5] = c5;
			data[6] = c6; data[7] = c7; data[8] = c8;
		}
		/**
		 * Sets the matrix values from the given three vector components.
		 * These are arranged as the three columns of the vector.
		 */
		void setComponents(const Vector3& compOne, const Vector3& compTwo,
			const Vector3& compThree)
		{
			// first row
			data[0] = compOne.x;
			data[1] = compTwo.x;
			data[2] = compThree.x;
			// second row
			data[3] = compOne.y;
			data[4] = compTwo.y;
			data[5] = compThree.y;
			// third row
			data[6] = compOne.z;
			data[7] = compTwo.z;
			data[8] = compThree.z;
		}
		/**
		 * Transform the given vector by this matrix.
		 */
		Vector3 operator*(const Vector3& vector) const {
			return Vector3(
				vector.x * data[0] + vector.y * data[1] + vector.z * data[2],
				vector.x * data[3] + vector.y * data[4] + vector.z * data[5],
				vector.x * data[6] + vector.y * data[7] + vector.z * data[8]
			);
		}
		/**
		 * Returns a matrix, which is this one multiplied by the other given
		 * matrix.
		 */
		Matrix3 operator*(const Matrix3& o) const {
			return Matrix3(
				data[0] * o.data[0] + data[1] * o.data[3] + data[2] * o.data[6],
				data[0] * o.data[1] + data[1] * o.data[4] + data[2] * o.data[7],
				data[0] * o.data[2] + data[1] * o.data[5] + data[2] * o.data[8],
				data[3] * o.data[0] + data[4] * o.data[3] + data[5] * o.data[6],
				data[3] * o.data[1] + data[4] * o.data[4] + data[5] * o.data[7],
				data[3] * o.data[2] + data[4] * o.data[5] + data[5] * o.data[8],
				data[6] * o.data[0] + data[7] * o.data[3] + data[8] * o.data[6],
				data[6] * o.data[1] + data[7] * o.data[4] + data[8] * o.data[7],
				data[6] * o.data[2] + data[7] * o.data[5] + data[8] * o.data[8]
			);
		}
		/**
		 * Multiplies this matrix in place by the other given matrix.
		 */
		void operator*=(const Matrix3& o) {
			real t1;
			real t2;
			real t3;
			t1 = data[0] * o.data[0] + data[1] * o.data[3] + data[2] * o.data[6];
			t2 = data[0] * o.data[1] + data[1] * o.data[4] + data[2] * o.data[7];
			t3 = data[0] * o.data[2] + data[1] * o.data[5] + data[2] * o.data[8];
			data[0] = t1;
			data[1] = t2;
			data[2] = t3;
			t1 = data[3] * o.data[0] + data[4] * o.data[3] + data[5] * o.data[6];
			t2 = data[3] * o.data[1] + data[4] * o.data[4] + data[5] * o.data[7];
			t3 = data[3] * o.data[2] + data[4] * o.data[5] + data[5] * o.data[8];
			data[3] = t1;
			data[4] = t2;
			data[5] = t3;
			t1 = data[6] * o.data[0] + data[7] * o.data[3] + data[8] * o.data[6];
			t2 = data[6] * o.data[1] + data[7] * o.data[4] + data[8] * o.data[7];
			t3 = data[6] * o.data[2] + data[7] * o.data[5] + data[8] * o.data[8];
			data[6] = t1;
			data[7] = t2;
			data[8] = t3;
		}
		/**
		 * Transform the given vector by this matrix.
		 */
		Vector3 transform(const Vector3& vector) const {
			return (*this) * vector;
		}
	};

	/**
	 * Holds a transform matrix, consisting of a rotatino matrix and
	 * a position. The matrix has 12 elements, and it is assumed that the
	 * remaining four are (0, 0, 0, 1), producing a homogeneous matrix.
	 */
	class Matrix4 {
	public:
		/**
		 * Holds the transform matrix data in array form.
		 */
		real data[12];
		/**
		 * Creates an identity matrix.
		 */
		Matrix4()
		{
			data[1] = data[2] = data[3] = data[4] = data[6] =
				data[7] = data[8] = data[9] = data[11] = 0;
			data[0] = data[5] = data[10] = 1;
		}
		/**
		 * Transform the given vector by this matrix.
		 */
		Vector3 operator*(const Vector3& vector) const {
			return Vector3(
				vector.x * data[0] +
				vector.y * data[1] +
				vector.z * data[2] + data[3],
				vector.x * data[4] +
				vector.y * data[5] +
				vector.z * data[6] + data[7],
				vector.x * data[8] +
				vector.y * data[9] +
				vector.z * data[10] + data[11]
			);
		}
		Vector3 transform(const Vector3& vector) const {
			return (*this) * vector;
		}
		/**
		 * Returns a matrix, which is this one multiplied by the other given
		 * matrix.
		 */
		Matrix4 operator*(const Matrix4& o) const {
			Matrix4 result = Matrix4();
			result.data[0] = (o.data[0] * data[0]) + (o.data[4] * data[1]) + (o.data[8] * data[2]);
			result.data[4] = (o.data[0] * data[4]) + (o.data[4] * data[5]) + (o.data[8] * data[6]);
			result.data[8] = (o.data[0] * data[8]) + (o.data[4] * data[9]) + (o.data[8] * data[10]);
			result.data[1] = (o.data[1] * data[0]) + (o.data[5] * data[1]) + (o.data[9] * data[2]);
			result.data[5] = (o.data[1] * data[4]) + (o.data[5] * data[5]) + (o.data[9] * data[6]);
			result.data[9] = (o.data[1] * data[8]) + (o.data[5] * data[9]) + (o.data[9] * data[10]);
			result.data[2] = (o.data[2] * data[0]) + (o.data[6] * data[1]) + (o.data[10] * data[2]);
			result.data[6] = (o.data[2] * data[4]) + (o.data[6] * data[5]) + (o.data[10] * data[6]);
			result.data[10] = (o.data[2] * data[8]) + (o.data[6] * data[9]) + (o.data[10] * data[10]);
			result.data[3] = (o.data[3] * data[0]) + (o.data[7] * data[1]) + (o.data[11] * data[2]) + data[3];
			result.data[7] = (o.data[3] * data[4]) + (o.data[7] * data[5]) + (o.data[11] * data[6]) + data[7];
			result.data[11] = (o.data[3] * data[8]) + (o.data[7] * data[9]) + (o.data[11] * data[10]) + data[11];
			return result;
		}
	};
}



#endif