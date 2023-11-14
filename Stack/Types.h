#ifndef TYPES_H
#define TYPES_H

/// @file 
/// @brief Contains types for stack

#include <assert.h>
#include <math.h>
#include <stdio.h>

/// @brief ElemType for stack
typedef int ElemType;

/// @brief Format for printing ElemType
#define ElemTypeFormat "%d"

/// @brief Chosen POISON value for stack
static const ElemType POISON = 0xDEAD;

/// @brief Function for checking if two ElemType values are equal 
/// @param [in]a first value
/// @param [in]b second value
/// @return true if they are equal otherwise false
static inline bool Equal(const ElemType* const a, const ElemType* const b)
{
    return a == b;
}

#endif // TYPES_H