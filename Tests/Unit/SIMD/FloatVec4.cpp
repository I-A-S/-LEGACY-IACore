// IACore-OSS; The Core Library for All IA Open Source Projects
// Copyright (C) 2025 IAS (ias@iasoft.dev)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <IACore/SIMD.hpp>
#include <IACore/IATest.hpp>

using namespace IACore;

IAT_BEGIN_BLOCK(Core, FloatVec4)

BOOL TestFloatArithmetic()
{
    FloatVec4 v1(10.0f, 20.0f, 30.0f, 40.0f);
    FloatVec4 v2(2.0f, 4.0f, 5.0f, 8.0f);

    ALIGN(16) FLOAT32 res[4];

    (v1 / v2).Store(res);
    IAT_CHECK_APPROX(res[0], 5.0f);
    IAT_CHECK_APPROX(res[3], 5.0f);

    (v1 * v2).Store(res);
    IAT_CHECK_APPROX(res[0], 20.0f);

    (v1 + v2).Store(res);
    IAT_CHECK_APPROX(res[0], 12.0f);

    return TRUE;
}

BOOL TestMathHelpers()
{
    ALIGN(16) FLOAT32 res[4];

    FloatVec4 vSq(4.0f, 9.0f, 16.0f, 25.0f);
    vSq.Sqrt().Store(res);
    IAT_CHECK_APPROX(res[0], 2.0f);
    IAT_CHECK_APPROX(res[3], 5.0f);

    FloatVec4 vNeg(-1.0f, -5.0f, 10.0f, -0.0f);
    vNeg.Abs().Store(res);
    IAT_CHECK_APPROX(res[0], 1.0f);
    IAT_CHECK_APPROX(res[2], 10.0f);

    FloatVec4 vClamp(-100.0f, 0.0f, 50.0f, 200.0f);
    vClamp.Clamp(0.0f, 100.0f).Store(res);
    IAT_CHECK_APPROX(res[0], 0.0f);
    IAT_CHECK_APPROX(res[2], 50.0f);
    IAT_CHECK_APPROX(res[3], 100.0f);

    return TRUE;
}

BOOL TestApproxMath()
{
    ALIGN(16) FLOAT32 res[4];
    FloatVec4 v(16.0f, 25.0f, 100.0f, 1.0f);

    v.Rsqrt().Store(res);

    IAT_CHECK_APPROX(res[0], 0.25f);
    IAT_CHECK_APPROX(res[2], 0.1f);

    return TRUE;
}

BOOL TestLinearAlgebra()
{
    FloatVec4 v1(1.0f, 2.0f, 3.0f, 4.0f);
    FloatVec4 v2(1.0f, 0.0f, 1.0f, 0.0f);

    FLOAT32 dot = v1.Dot(v2);
    IAT_CHECK_APPROX(dot, 4.0f);

    FloatVec4 vNorm(10.0f, 0.0f, 0.0f, 0.0f);
    ALIGN(16) FLOAT32 res[4];

    vNorm.Normalize().Store(res);
    IAT_CHECK_APPROX(res[0], 1.0f);
    IAT_CHECK_APPROX(res[1], 0.0f);

    return TRUE;
}

IAT_BEGIN_TEST_LIST()
IAT_ADD_TEST(TestFloatArithmetic);
IAT_ADD_TEST(TestMathHelpers);
IAT_ADD_TEST(TestApproxMath);
IAT_ADD_TEST(TestLinearAlgebra);
IAT_END_TEST_LIST()

IAT_END_BLOCK()

IAT_REGISTER_ENTRY(Core, FloatVec4)