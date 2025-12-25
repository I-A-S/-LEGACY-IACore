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

// IACore-OSS; The Core Library for All IA Open Source Projects
// Copyright (C) 2025 IAS (ias@iasoft.dev)

#include <IACore/SIMD.hpp>
#include <IACore/IATest.hpp>

using namespace IACore;

IAT_BEGIN_BLOCK(Core, IntVec4)

BOOL TestConstructors()
{
    IntVec4 vBroadcast(10);
    ALIGN(16) UINT32 storeBuf[4];
    vBroadcast.Store(storeBuf);

    IAT_CHECK_EQ(storeBuf[0], 10U);
    IAT_CHECK_EQ(storeBuf[3], 10U);

    IntVec4 vComp(1, 2, 3, 4);
    vComp.Store(storeBuf);
    IAT_CHECK_EQ(storeBuf[0], 1U);
    IAT_CHECK_EQ(storeBuf[3], 4U);

    ALIGN(16) UINT32 srcBuf[4] = {100, 200, 300, 400};
    IntVec4 vLoad = IntVec4::Load(srcBuf);
    vLoad.Store(storeBuf);
    IAT_CHECK_EQ(storeBuf[1], 200U);

    return TRUE;
}

BOOL TestArithmetic()
{
    IntVec4 v1(10, 20, 30, 40);
    IntVec4 v2(1, 2, 3, 4);

    IntVec4 vAdd = v1 + v2;
    ALIGN(16) UINT32 res[4];
    vAdd.Store(res);
    IAT_CHECK_EQ(res[0], 11U);
    IAT_CHECK_EQ(res[3], 44U);

    IntVec4 vSub = v1 - v2;
    vSub.Store(res);
    IAT_CHECK_EQ(res[0], 9U);

    IntVec4 vMul = v1 * v2;
    vMul.Store(res);
    IAT_CHECK_EQ(res[0], 10U);
    IAT_CHECK_EQ(res[2], 90U);
    IAT_CHECK_EQ(res[3], 160U);

    return TRUE;
}

BOOL TestBitwise()
{
    IntVec4 vAllOnes(0xFFFFFFFF);
    IntVec4 vZero((UINT32) 0);
    IntVec4 vPattern(0xAAAAAAAA);

    ALIGN(16) UINT32 res[4];

    (vAllOnes & vPattern).Store(res);
    IAT_CHECK_EQ(res[0], 0xAAAAAAAAU);

    (vZero | vPattern).Store(res);
    IAT_CHECK_EQ(res[0], 0xAAAAAAAAU);

    (vAllOnes ^ vPattern).Store(res);
    IAT_CHECK_EQ(res[0], 0x55555555U);

    (~vPattern).Store(res);
    IAT_CHECK_EQ(res[0], 0x55555555U);

    IntVec4 vShift(1);
    (vShift << 1).Store(res);
    IAT_CHECK_EQ(res[0], 2U);

    IntVec4 vShiftRight(4);
    (vShiftRight >> 1).Store(res);
    IAT_CHECK_EQ(res[0], 2U);

    return TRUE;
}

BOOL TestSaturation()
{
    UINT32 max = 0xFFFFFFFF;
    IntVec4 vHigh(max - 10);
    IntVec4 vAdd(20);

    ALIGN(16) UINT32 res[4];

    vHigh.SatAdd(vAdd).Store(res);
    IAT_CHECK_EQ(res[0], max);

    IntVec4 vLow(10);
    IntVec4 vSub(20);
    vLow.SatSub(vSub).Store(res);
    IAT_CHECK_EQ(res[0], 0U);

    return TRUE;
}

BOOL TestAdvancedOps()
{
    IntVec4 v(0, 50, 100, 150);
    ALIGN(16) UINT32 res[4];

    v.Clamp(40, 110).Store(res);
    IAT_CHECK_EQ(res[0], 40U);
    IAT_CHECK_EQ(res[1], 50U);
    IAT_CHECK_EQ(res[2], 100U);
    IAT_CHECK_EQ(res[3], 110U);

    IntVec4 A(2);
    IntVec4 B(10);
    IntVec4 C(5);
    A.MultAdd(B, C).Store(res);
    IAT_CHECK_EQ(res[0], 25U);

    return TRUE;
}

IAT_BEGIN_TEST_LIST()
IAT_ADD_TEST(TestConstructors);
IAT_ADD_TEST(TestArithmetic);
IAT_ADD_TEST(TestBitwise);
IAT_ADD_TEST(TestSaturation);
IAT_ADD_TEST(TestAdvancedOps);
IAT_END_TEST_LIST()

IAT_END_BLOCK()

IAT_REGISTER_ENTRY(Core, IntVec4)