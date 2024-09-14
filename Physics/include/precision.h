#include <math.h>

namespace AE86 {
	/**
	 * Defines a real number precision. The point of this is so that we
	 * can choose to use doubles instead of floats and only have to change
	 * this one typedef as opposed to everything that uses floats in our AE86
	 * namespace.
	*/
	typedef float real;

	#define real_sqrt sqrtf
	#define real_pow powf
}