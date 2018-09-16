#ifndef _DJN_ASSERT_H_
#define _DJN_ASSERT_H_

#ifdef DJN_USE_ASSERT
	#include <assert.h>
	#define DJN_ASSERT(expr) assert(expr)
#else
	#define DJN_ASSERT(expr)
#endif

#endif
