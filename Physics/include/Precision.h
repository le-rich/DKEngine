#ifndef PRECISION_HEADER

#define PRECISION_HEADER

#include <math.h>
#include <float.h>

namespace AE86 {
	/**
	 * Defines a real number precision. The point of this is so that we
	 * can choose to use doubles instead of floats and only have to change
	 * this one typedef as opposed to everything that uses floats in our AE86
	 * namespace.
	*/

#if 0  // 1 = FLOATING POINT PRECISION, 0 = DOUBLE PRECISION
	typedef float real;

#define REAL_MAX FLT_MAX

#define real_abs fabsf
#define real_sqrt sqrtf
#define real_pow powf

#define real_sin sinf
#define real_cos cosf
#define real_exp expf
#else 
	typedef double real;

#define REAL_MAX DBL_MAX

#define R_PI 3.14159265358979323846 // pi

#define real_abs fabs
#define real_sqrt sqrt
#define real_pow pow

#define real_sin sin
#define real_cos cos
#define real_exp exp
#endif

}

#endif