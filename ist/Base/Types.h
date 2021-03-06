﻿#ifndef ist_Base_Types_h
#define ist_Base_Types_h

#define GLM_FORCE_SSE2
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/simd_vec4.hpp>
#include <glm/gtx/simd_mat4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Half/half.h"
#include "Serialize.h"
#include "MemberPtr.h"

namespace ist {

typedef signed char         int8;
typedef signed short        int16;
typedef signed int          int32;
typedef signed long long    int64;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;
typedef __m128i             uint128;
typedef half                float16;
typedef float               float32;
typedef double              float64;

typedef glm::vec2       vec2;
typedef glm::vec3       vec3;
typedef glm::vec4       vec4;
typedef glm::ivec2      ivec2;
typedef glm::ivec3      ivec3;
typedef glm::ivec4      ivec4;
typedef glm::uvec2      uvec2;
typedef glm::uvec3      uvec3;
typedef glm::uvec4      uvec4;
typedef glm::mat2       mat2;
typedef glm::mat3       mat3;
typedef glm::mat4       mat4;
typedef glm::simdVec4   simdvec4;
//typedef glm::simdVec8   simdvec8;
typedef glm::simdMat4   simdmat4;

} // namespace ist

istSerializeRaw(ist::vec2);
istSerializeRaw(ist::vec3);
istSerializeRaw(ist::vec4);
istSerializeRaw(ist::ivec2);
istSerializeRaw(ist::ivec3);
istSerializeRaw(ist::ivec4);
istSerializeRaw(ist::uvec2);
istSerializeRaw(ist::uvec3);
istSerializeRaw(ist::uvec4);
istSerializeRaw(ist::mat2);
istSerializeRaw(ist::mat3);
istSerializeRaw(ist::mat4);
istSerializeRaw(ist::simdvec4);
istSerializeRaw(ist::simdmat4);

#include "New.h"

#endif // ist_Base_Types_h
