/*
    KHR/khrplatform.h header for glad loader.
*/
#ifndef __khrplatform_h_
#define __khrplatform_h_

#if defined(_WIN32)
#define KHRPLATFORM_CALL __stdcall
#else
#define KHRPLATFORM_CALL
#endif

#include <stdint.h>
#include <stddef.h>

typedef int32_t khronos_int32_t;
typedef uint32_t khronos_uint32_t;
typedef int64_t khronos_int64_t;
typedef uint64_t khronos_uint64_t;
typedef float khronos_float_t;

#endif
