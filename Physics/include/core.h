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
	 * Holds a three-degrees-of-freedom orientation. This is done
	 * using the mathematical concept of a Quaternion. It is the
	 * extension of imaginary numbers, with four items of data to
	 * hold three degrees of freedom. These items are coefficients
	 * of a complex number with three imaginary units. This class
	 * is a normalized quaternion (length of 1) as that is a
	 * requirement for a quaternion to be a valid rotation.
	 */
	class Quaternion {
	public:
		union {
			struct {
				/**
				 * Holds the real component of the quaternion.
				 */
				real r;
				/**
				 * Holds the first complex component of the
				 * quaternion.
				 */
				real i;
				/**
				 * Holds the second complex component of the
				 * quaternion.
				 */
				real j;
				/**
				 * Holds the third complex component of the
				 * quaternion.
				 */
				real k;
			};
			/**
			 * Holds the quaternion data in array form.
			 */
			real data[4];
		};

		/**
		 * The default constructor creates a quaternion representing
		 * a zero rotation.
		 */
		Quaternion() : r(1), i(0), j(0), k(0) {}

		/**
		 * The explicit constructor creates a quaternion with the given
		 * components.
		 */
		Quaternion(const real r, const real i, const real j, const real k)
			: r(r), i(i), j(j), k(k)
		{
		}

		/**
		 * Normalizes the quaternion, ensuring unit length and that
		 * it is a valid quaternion rotation.
		 */
		void normalize() {
			real d = r * r + i * i + j * j + k * k;
			// Check for zero-length quaternion.
			// If so, set r = 1, which means no-rotation.
			if (d == 0) {
				r = 1;
				return;
			}
			d = ((real)1.0) / real_sqrt(d);
			r *= d;
			i *= d;
			j *= d;
			k *= d;
		}

		/**
		 * Multiplies the quaternion by the given quaternion.
		 */
		void operator *=(const Quaternion& multiplier) {
			Quaternion q = *this;
			r = q.r * multiplier.r - q.i * multiplier.i -
				q.j * multiplier.j - q.k * multiplier.k;
			i = q.r * multiplier.i + q.i * multiplier.r +
				q.j * multiplier.k - q.k * multiplier.j;
			j = q.r * multiplier.j + q.j * multiplier.r +
				q.k * multiplier.i - q.i * multiplier.k;
			k = q.r * multiplier.k + q.k * multiplier.r +
				q.i * multiplier.j - q.j * multiplier.i;
		}
		void rotateByVector(const Vector3& vector) {
			Quaternion q(0, vector.x, vector.y, vector.z);
			(*this) *= q;
		}

		/**
		 * Adds the given vector this, scaled by a given amount.
		 * Required for orientation updates by angular velocity.
		 */
		void addScaledVector(const Vector3& vector, real scale)
		{
			Quaternion q(0,
				vector.x * scale,
				vector.y * scale,
				vector.z * scale);
			q *= *this;
			r += q.r * ((real)0.5);
			i += q.i * ((real)0.5);
			j += q.j * ((real)0.5);
			k += q.k * ((real)0.5);
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
		* Sets the matrix to be the inverse of the given matrix.
		*/
		void setInverse(const Matrix3& m)
		{
			real t1 = m.data[0] * m.data[4];
			real t2 = m.data[0] * m.data[5];
			real t3 = m.data[1] * m.data[3];
			real t4 = m.data[2] * m.data[3];
			real t5 = m.data[1] * m.data[6];
			real t6 = m.data[2] * m.data[6];
			// Calculate the determinant.
			real det = (t1 * m.data[8] - t2 * m.data[7] - t3 * m.data[8] +
				t4 * m.data[7] + t5 * m.data[5] - t6 * m.data[4]);
			// Make sure the determinant is non-zero.
			if (det == (real)0.0f) return;
			real invd = (real)1.0f / det;
			data[0] = (m.data[4] * m.data[8] - m.data[5] * m.data[7]) * invd;
			data[1] = -(m.data[1] * m.data[8] - m.data[2] * m.data[7]) * invd;
			data[2] = (m.data[1] * m.data[5] - m.data[2] * m.data[4]) * invd;
			data[3] = -(m.data[3] * m.data[8] - m.data[5] * m.data[6]) * invd;
			data[4] = (m.data[0] * m.data[8] - t6) * invd;
			data[5] = -(t2 - t4) * invd;
			data[6] = (m.data[3] * m.data[7] - m.data[4] * m.data[6]) * invd;
			data[7] = -(m.data[0] * m.data[7] - t5) * invd;
			data[8] = (t1 - t3) * invd;
		}

		/** Returns a new matrix containing the inverse of the matrix. */
		Matrix3 inverse() const {
			Matrix3 result;
			result.setInverse(*this);
			return result;
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
		 * Sets the matrix to be the transpose of the given matrix.
		 */
		void setTranspose(const Matrix3& m) {
			data[0] = m.data[0];
			data[1] = m.data[3];
			data[2] = m.data[6];
			data[3] = m.data[1];
			data[4] = m.data[4];
			data[5] = m.data[7];
			data[6] = m.data[2];
			data[7] = m.data[5];
			data[8] = m.data[8];
		}

		/** Returns a new matrix containing the transpose of the matrix. */
		Matrix3 transpose() const {
			Matrix3 result;
			result.setTranspose(*this);
			return result;
		}

		/**
		 * Transform the given vector by this matrix.
		 */
		Vector3 transform(const Vector3& vector) const {
			return (*this) * vector;
		}

		/**
		 * Sets this matrix to be the rotation matrix corresponding to
		 * the given quaternion.
		 */
		void setOrientation(const Quaternion& q) {
			data[0] = 1 - (2 * q.j * q.j + 2 * q.k * q.k);
			data[1] = 2 * q.i * q.j + 2 * q.k * q.r;
			data[2] = 2 * q.i * q.k - 2 * q.j * q.r;
			data[3] = 2 * q.i * q.j - 2 * q.k * q.r;
			data[4] = 1 - (2 * q.i * q.i + 2 * q.k * q.k);
			data[5] = 2 * q.j * q.k + 2 * q.i * q.r;
			data[6] = 2 * q.i * q.k + 2 * q.j * q.r;
			data[7] = 2 * q.j * q.k - 2 * q.i * q.r;
			data[8] = 1 - (2 * q.i * q.i + 2 * q.j * q.j);
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

		/**
		 * Sets this matrix to be the rotatino matrix corresponding to
		 * the given quaternion.
		 */
		void setOrientationAndPos(const Quaternion& q, const Vector3& pos) {
			data[0] = 1 - (2 * q.j * q.j + 2 * q.k * q.k);
			data[1] = 2 * q.i * q.j + 2 * q.k * q.r;
			data[2] = 2 * q.i * q.k - 2 * q.j * q.r;
			data[3] = pos.x;
			data[4] = 2 * q.i * q.j - 2 * q.k * q.r;
			data[5] = 1 - (2 * q.i * q.i + 2 * q.k * q.k);
			data[6] = 2 * q.j * q.k + 2 * q.i * q.r;
			data[7] = pos.y;
			data[8] = 2 * q.i * q.k + 2 * q.j * q.r;
			data[9] = 2 * q.j * q.k - 2 * q.i * q.r;
			data[10] = 1 - (2 * q.i * q.i + 2 * q.j * q.j);
			data[11] = pos.z;
		}

		/**
		 * Returns the determinant of the matrix.
		 */
		real getDeterminant() const;

		/**
		 * Sets the matrix to be the inverse of the given matrix.
		 */
		void setInverse(const Matrix4& m);

		/**
		 * Returns a new matrix containing the inverse of this matrix.
		 */
		Matrix4 inverse() const {
			Matrix4 result;
			result.setInverse(*this);
			return result;
		}

		/**
		 * Inverts the matrix.
		 */
		void invert() {
			setInverse(*this);
		}

	};
}



#endif