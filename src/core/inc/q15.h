#ifndef Q15_H
#define Q15_H

#include <stdint.h>


// Q15 fixed-point type
typedef int32_t Q15_t;

#define Q15_SHIFT      15
#define Q15_ONE        ((uint32_t)1 << Q15_SHIFT)
#define Q15_MAX        ((Q15_t)0x7FFFFFFF)
#define Q15_MIN        ((Q15_t)0x80000000)

// Conversion float ↔ Q15
static inline Q15_t Q15_from_float(float val) {
    return (Q15_t)(val * (float)Q15_ONE);
}

static inline float Q15_to_float(Q15_t val) {
    return (float)val / (float)Q15_ONE;
}

// Conversion int ↔ Q15
static inline Q15_t Q15_from_int(int32_t val) {
    return val << Q15_SHIFT;
}

static inline int32_t Q15_to_int(Q15_t val) {
    return val >> Q15_SHIFT;
}

// Multiplication Q15 * Q15 = Q15
static inline Q15_t Q15_mul(Q15_t a, Q15_t b) {
    int64_t result = ((int64_t)a * b) >> Q15_SHIFT;

    // Overflow
    if (result > Q15_MAX) return Q15_MAX;
    if (result < Q15_MIN) return Q15_MIN;

    return (Q15_t)result;
}

// Division Q15 / Q15 = Q15
static inline Q15_t Q15_div(Q15_t a, Q15_t b) {
    int64_t result = ((int64_t)a << Q15_SHIFT) / b;

    // Overflow
    if (result > Q15_MAX) return Q15_MAX;
    if (result < Q15_MIN) return Q15_MIN;

    return (Q15_t)result;
}

// Range restrictions
static inline Q15_t Q15_clamp(int64_t val, Q15_t min, Q15_t max) {
    if (val > max) return max;
    if (val < min) return min;
    return val;
}

#endif // Q15_H
