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

#pragma once

#include <IACore/PCH.hpp>

#if defined(__clang__)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wunused-parameter"
#    pragma GCC diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

#include <hwy/highway.h>

#if defined(__clang__)
#    pragma GCC diagnostic pop
#endif

namespace IACore
{
    namespace hn = hwy::HWY_NAMESPACE;

#if HWY_TARGET == HWY_SCALAR
#    pragma message("Warning: Configuration mismatch. IACore is being compiled for SCALAR SIMD (Slow)")
#endif

    class ALIGN(16) IntVec4
    {
      public:
        IntVec4() = default;

        INLINE EXPLICIT IntVec4(IN UINT32 s);
        INLINE EXPLICIT IntVec4(IN PCUINT32 values);
        INLINE EXPLICIT IntVec4(IN UINT32 a, IN UINT32 b, IN UINT32 c, IN UINT32 d);

        INLINE IntVec4 operator+(IN CONST IntVec4 &other) CONST;
        INLINE IntVec4 operator-(IN CONST IntVec4 &other) CONST;
        INLINE IntVec4 operator*(IN CONST IntVec4 &other) CONST;

        INLINE IntVec4 operator&(IN CONST IntVec4 &other) CONST;
        INLINE IntVec4 operator|(IN CONST IntVec4 &other) CONST;
        INLINE IntVec4 operator^(IN CONST IntVec4 &other) CONST;
        INLINE IntVec4 operator~() CONST;

        INLINE IntVec4 operator<<(IN UINT32 amount) CONST;
        INLINE IntVec4 operator>>(IN UINT32 amount) CONST;

        INLINE IntVec4 SatAdd(IN CONST IntVec4 &other) CONST;
        INLINE IntVec4 SatSub(IN CONST IntVec4 &other) CONST;

        INLINE IntVec4 Clamp(IN UINT32 min, IN UINT32 max) CONST;

        INLINE IntVec4 MultAdd(IN CONST IntVec4 &multiplier, IN CONST IntVec4 &addend) CONST;

        INLINE VOID Store(OUT PUINT32 values);
        STATIC INLINE IntVec4 Load(IN PCUINT32 values);

      private:
        using Tag = hn::FixedTag<UINT32, 4>;

        hn::Vec<Tag> m_data;

        INLINE EXPLICIT IntVec4(hn::Vec<Tag> v) : m_data(v)
        {
        }
    };

    class ALIGN(16) FloatVec4
    {
      public:
        FloatVec4() = default;

        INLINE EXPLICIT FloatVec4(IN FLOAT32 s);
        INLINE EXPLICIT FloatVec4(IN PCFLOAT32 values);
        INLINE EXPLICIT FloatVec4(IN FLOAT32 a, IN FLOAT32 b, IN FLOAT32 c, IN FLOAT32 d);

        INLINE FloatVec4 operator+(IN CONST FloatVec4 &other) CONST;
        INLINE FloatVec4 operator-(IN CONST FloatVec4 &other) CONST;
        INLINE FloatVec4 operator*(IN CONST FloatVec4 &other) CONST;
        INLINE FloatVec4 operator/(IN CONST FloatVec4 &other) CONST;

        INLINE FloatVec4 Clamp(IN FLOAT32 min, IN FLOAT32 max) CONST;

        INLINE FloatVec4 Abs() CONST;
        INLINE FloatVec4 Sqrt() CONST;
        INLINE FloatVec4 Rsqrt() CONST;
        INLINE FloatVec4 Normalize() CONST;

        INLINE FLOAT32 Dot(IN CONST FloatVec4 &other) CONST;

        INLINE FloatVec4 MultAdd(IN CONST FloatVec4 &multiplier, IN CONST FloatVec4 &addend) CONST;

        INLINE VOID Store(OUT PFLOAT32 values);
        STATIC INLINE FloatVec4 Load(IN PCFLOAT32 values);

      private:
        using Tag = hn::FixedTag<FLOAT32, 4>;

        hn::Vec<Tag> m_data;

        INLINE EXPLICIT FloatVec4(hn::Vec<Tag> v) : m_data(v)
        {
        }
    };
} // namespace IACore

namespace IACore
{
    IntVec4::IntVec4(IN UINT32 s)
    {
        CONST Tag d;
        m_data = hn::Set(d, s);
    }

    IntVec4::IntVec4(IN PCUINT32 values)
    {
        CONST Tag data;
        m_data = hn::Load(data, values);
    }

    IntVec4::IntVec4(IN UINT32 a, IN UINT32 b, IN UINT32 c, IN UINT32 d)
    {
        CONST Tag data;
        ALIGN(16) UINT32 values[4] = {a, b, c, d};
        m_data = hn::Load(data, values);
    }

    IntVec4 IntVec4::operator+(IN CONST IntVec4 &other) CONST
    {
        return IntVec4(hn::Add(m_data, other.m_data));
    }

    IntVec4 IntVec4::operator-(IN CONST IntVec4 &other) CONST
    {
        return IntVec4(hn::Sub(m_data, other.m_data));
    }

    IntVec4 IntVec4::operator*(IN CONST IntVec4 &other) CONST
    {
        return IntVec4(hn::Mul(m_data, other.m_data));
    }

    IntVec4 IntVec4::operator&(IN CONST IntVec4 &other) CONST
    {
        return IntVec4(hn::And(m_data, other.m_data));
    }

    IntVec4 IntVec4::operator|(IN CONST IntVec4 &other) CONST
    {
        return IntVec4(hn::Or(m_data, other.m_data));
    }

    IntVec4 IntVec4::operator^(IN CONST IntVec4 &other) CONST
    {
        return IntVec4(hn::Xor(m_data, other.m_data));
    }

    IntVec4 IntVec4::operator~() CONST
    {
        return IntVec4(hn::Not(m_data));
    }

    IntVec4 IntVec4::operator<<(IN UINT32 amount) CONST
    {
        return IntVec4(hn::ShiftLeftSame(m_data, amount));
    }

    IntVec4 IntVec4::operator>>(IN UINT32 amount) CONST
    {
        return IntVec4(hn::ShiftRightSame(m_data, amount));
    }

    IntVec4 IntVec4::MultAdd(IN CONST IntVec4 &multiplier, IN CONST IntVec4 &addend) CONST
    {
        return IntVec4(hn::MulAdd(m_data, multiplier.m_data, addend.m_data));
    }

    IntVec4 IntVec4::SatAdd(IN CONST IntVec4 &other) CONST
    {
        return IntVec4(hn::SaturatedAdd(m_data, other.m_data));
    }

    IntVec4 IntVec4::SatSub(IN CONST IntVec4 &other) CONST
    {
        return IntVec4(hn::SaturatedSub(m_data, other.m_data));
    }

    IntVec4 IntVec4::Clamp(IN UINT32 min, IN UINT32 max) CONST
    {
        CONST Tag d;
        auto vMin = hn::Set(d, min);
        auto vMax = hn::Set(d, max);
        return IntVec4(hn::Min(hn::Max(m_data, vMin), vMax));
    }

    VOID IntVec4::Store(OUT PUINT32 values)
    {
        CONST Tag d;
        hn::Store(m_data, d, values);
    }

    IntVec4 IntVec4::Load(IN PCUINT32 values)
    {
        CONST Tag d;
        return IntVec4(hn::Load(d, values));
    }
} // namespace IACore

namespace IACore
{
    FloatVec4::FloatVec4(IN FLOAT32 s)
    {
        const Tag d;
        m_data = hn::Set(d, s);
    }

    FloatVec4::FloatVec4(IN PCFLOAT32 values)
    {
        const Tag d;
        m_data = hn::Load(d, values);
    }

    FloatVec4::FloatVec4(IN FLOAT32 a, IN FLOAT32 b, IN FLOAT32 c, IN FLOAT32 d)
    {
        const Tag data;
        ALIGN(16) FLOAT32 temp[4] = {a, b, c, d};
        m_data = hn::Load(data, temp);
    }

    FloatVec4 FloatVec4::operator+(IN CONST FloatVec4 &other) CONST
    {
        return FloatVec4(hn::Add(m_data, other.m_data));
    }

    FloatVec4 FloatVec4::operator-(IN CONST FloatVec4 &other) CONST
    {
        return FloatVec4(hn::Sub(m_data, other.m_data));
    }

    FloatVec4 FloatVec4::operator*(IN CONST FloatVec4 &other) CONST
    {
        return FloatVec4(hn::Mul(m_data, other.m_data));
    }

    FloatVec4 FloatVec4::operator/(IN CONST FloatVec4 &other) CONST
    {
        return FloatVec4(hn::Div(m_data, other.m_data));
    }

    FloatVec4 FloatVec4::MultAdd(IN CONST FloatVec4 &multiplier, IN CONST FloatVec4 &addend) CONST
    {
        return FloatVec4(hn::MulAdd(m_data, multiplier.m_data, addend.m_data));
    }

    FloatVec4 FloatVec4::Clamp(IN FLOAT32 min, IN FLOAT32 max) CONST
    {
        const Tag d;
        auto vMin = hn::Set(d, min);
        auto vMax = hn::Set(d, max);
        return FloatVec4(hn::Min(hn::Max(m_data, vMin), vMax));
    }

    FloatVec4 FloatVec4::Sqrt() CONST
    {
        return FloatVec4(hn::Sqrt(m_data));
    }

    FloatVec4 FloatVec4::Rsqrt() CONST
    {
        return FloatVec4(hn::ApproximateReciprocalSqrt(m_data));
    }

    FloatVec4 FloatVec4::Abs() CONST
    {
        return FloatVec4(hn::Abs(m_data));
    }

    FLOAT32 FloatVec4::Dot(IN CONST FloatVec4 &other) CONST
    {
        const Tag d;
        auto vMul = hn::Mul(m_data, other.m_data);
        return hn::ReduceSum(d, vMul);
    }

    FloatVec4 FloatVec4::Normalize() CONST
    {
        const Tag d;
        auto vMul = hn::Mul(m_data, m_data);
        auto vLenSq = hn::SumOfLanes(d, vMul);
        auto vInvLen = hn::ApproximateReciprocalSqrt(vLenSq);
        return FloatVec4(hn::Mul(m_data, vInvLen));
    }

    VOID FloatVec4::Store(OUT PFLOAT32 values)
    {
        const Tag d;
        hn::Store(m_data, d, values);
    }

    FloatVec4 FloatVec4::Load(IN PCFLOAT32 values)
    {
        const Tag d;
        return FloatVec4(hn::Load(d, values));
    }
} // namespace IACore