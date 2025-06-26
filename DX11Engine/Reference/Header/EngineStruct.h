#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "EngineTypedef.h"

namespace Engine
{
#pragma region vector2
    struct vector2
    {
        _float x;
        _float y;

        vector2()
            : x(0.f), y(0.f)
        {
        }

        vector2(float _x, float _y)
            : x(_x), y(_y)
        {
        }

        vector2(const XMFLOAT2& v)
            : x(v.x), y(v.y)
        {
        }

        const _float2& toFloat2()
        {
            return XMFLOAT2(x, y);;
        }

        _vector& toXMVector()
        {
            _vector result = XMVectorSet(x, y, 0.f, 0.f);
            return result;
        }

        vector2& operator=(const XMFLOAT2& rhs)
        {
            x = rhs.x;
            y = rhs.y;
            return *this;
        }

        vector2& operator+=(const vector2& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        vector2& operator-=(const vector2& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        vector2& operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        vector2& operator/=(float scalar)
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        bool operator==(const vector2& rhs) const
        {
            return x == rhs.x && y == rhs.y;
        }

        bool operator!=(const vector2& rhs) const
        {
            return !(*this == rhs);
        }

        float length() const
        {
            XMVECTOR v = XMVectorSet(x, y, 0.f, 0.f);
            XMVECTOR len = XMVector2Length(v);
            return XMVectorGetX(len);
        }

        float lengthSq() const
        {
            XMVECTOR v = XMVectorSet(x, y, 0.f, 0.f);
            XMVECTOR lenSq = XMVector2LengthSq(v);
            return XMVectorGetX(lenSq);
        }

        vector2 normalized() const
        {
            XMVECTOR v = XMVectorSet(x, y, 0.f, 0.f);
            XMVECTOR result = XMVector2Normalize(v);
            return vector2(XMVectorGetX(result), XMVectorGetY(result));
        }

        float dot(const vector2& other) const
        {
            XMFLOAT2 a(x, y);
            XMFLOAT2 b(other.x, other.y);

            XMVECTOR va = XMLoadFloat2(&a);
            XMVECTOR vb = XMLoadFloat2(&b);

            XMVECTOR result = XMVector2Dot(va, vb);
            return XMVectorGetX(result);
        }

        static vector2 zero()
        {
            return vector2(0.f, 0.f);
        }

        static vector2 one()
        {
            return vector2(1.f, 1.f);
        }

        static vector2 right()
        {
            return vector2(1.f, 0.f);
        }

        static vector2 left()
        {
            return vector2(-1.f, 0.f);
        }

        static vector2 up()
        {
            return vector2(0.f, 1.f);
        }

        static vector2 down()
        {
            return vector2(0.f, -1.f);
        }

        static float Distance(const vector2& a, const vector2& b)
        {
            return vector2(b.x - a.x, b.y - a.y).length();
        }

        operator XMFLOAT2() const
        {
            return XMFLOAT2(x, y);
        }

        XMFLOAT2 vector() const
        {
            return XMFLOAT2(x, y);
        }
    };

    // 연산자 오버로딩
    inline vector2 operator+(const vector2& lhs, const vector2& rhs)
    {
        return vector2(lhs.x + rhs.x, lhs.y + rhs.y);
    }

    inline vector2 operator-(const vector2& lhs, const vector2& rhs)
    {
        return vector2(lhs.x - rhs.x, lhs.y - rhs.y);
    }

    inline vector2 operator*(const vector2& vec, float scalar)
    {
        return vector2(vec.x * scalar, vec.y * scalar);
    }

    inline vector2 operator*(float scalar, const vector2& vec)
    {
        return vector2(vec.x * scalar, vec.y * scalar);
    }

    inline vector2 operator/(const vector2& vec, float scalar)
    {
        return vector2(vec.x / scalar, vec.y / scalar);
    }

    inline vector2 operator*(const vector2& lhs, const vector2& rhs)
    {
        return vector2(lhs.x * rhs.x, lhs.y * rhs.y);
    }

    inline vector2 operator/(const vector2& lhs, const vector2& rhs)
    {
        return vector2(lhs.x / rhs.x, lhs.y / rhs.y);
    }

    inline vector2 operator-(const vector2& v)
    {
        return vector2(-v.x, -v.y);
    }

#pragma endregion

#pragma region vector2Int
    struct vector2Int
    {
        int x;
        int y;

        vector2Int()
            : x(0), y(0)
        {
        }

        vector2Int(_int _x, _int _y)
            : x(_x), y(_y)
        {
        }

        vector2Int(UINT _x, UINT _y)
            : x((_int)_x), y((_int)_y)
        {
        }

        vector2Int(const vector2& v)
            : x(static_cast<int>(v.x)), y(static_cast<int>(v.y))
        {
        }

        const _float2& toFloat2()
        {
            return _float2(static_cast<_float>(x), static_cast<_float>(y));
        }

        _vector& toXMVector()
        {
            _vector result = XMVectorSet(static_cast<_float>(x), static_cast<_float>(y), 0.f, 0.f);
            return result;
        }

        vector2Int& operator=(const vector2Int& rhs)
        {
            x = rhs.x;
            y = rhs.y;
            return *this;
        }

        vector2Int& operator+=(const vector2Int& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        vector2Int& operator-=(const vector2Int& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        vector2Int& operator*=(int scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        vector2Int& operator/=(int scalar)
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        bool operator==(const vector2Int& rhs) const
        {
            return x == rhs.x && y == rhs.y;
        }

        bool operator!=(const vector2Int& rhs) const
        {
            return !(*this == rhs);
        }

        float length() const
        {
            return sqrtf(static_cast<float>(x * x + y * y));
        }

        int lengthSq() const
        {
            return x * x + y * y;
        }

        vector2 to_vector2() const
        {
            return vector2(static_cast<float>(x), static_cast<float>(y));
        }

        static vector2Int zero()
        {
            return vector2Int(0, 0);
        }

        static vector2Int one()
        {
            return vector2Int(1, 1);
        }

        static vector2Int up()
        {
            return vector2Int(0, 1);
        }

        static vector2Int down()
        {
            return vector2Int(0, -1);
        }

        static vector2Int left()
        {
            return vector2Int(-1, 0);
        }

        static vector2Int right()
        {
            return vector2Int(1, 0);
        }

        static int Distance(const vector2Int& a, const vector2Int& b)
        {
            int dx = b.x - a.x;
            int dy = b.y - a.y;
            return static_cast<int>(sqrtf(static_cast<float>(dx * dx + dy * dy)));
        }

        static int ManhattanDistance(const vector2Int& a, const vector2Int& b)
        {
            return abs(b.x - a.x) + abs(b.y - a.y);
        }
    };

    // 연산자 오버로딩
    inline vector2Int operator+(const vector2Int& lhs, const vector2Int& rhs)
    {
        return vector2Int(lhs.x + rhs.x, lhs.y + rhs.y);
    }

    inline vector2Int operator-(const vector2Int& lhs, const vector2Int& rhs)
    {
        return vector2Int(lhs.x - rhs.x, lhs.y - rhs.y);
    }

    inline vector2Int operator*(const vector2Int& vec, int scalar)
    {
        return vector2Int(vec.x * scalar, vec.y * scalar);
    }

    inline vector2Int operator*(int scalar, const vector2Int& vec)
    {
        return vector2Int(vec.x * scalar, vec.y * scalar);
    }

    inline vector2Int operator/(const vector2Int& vec, int scalar)
    {
        return vector2Int(vec.x / scalar, vec.y / scalar);
    }

    inline vector2Int operator-(const vector2Int& v)
    {
        return vector2Int(-v.x, -v.y);
    }
}
#pragma endregion

#pragma region vector3
    struct vector3
    {
        float x;
        float y;
        float z;

        vector3()
            : x(0.f), y(0.f), z(0.f)
        {
        }

        vector3(float _x, float _y, float _z)
            : x(_x), y(_y), z(_z)
        {
        }

        vector3(const XMVECTOR& v)
            : x(XMVectorGetX(v)), y(XMVectorGetY(v)), z(XMVectorGetZ(v))
        {
        }

        vector3(const XMFLOAT3& v)
            : x(v.x), y(v.y), z(v.z)
        {
        }

        const _float3 toFloat3()
        {
            return _float3(x, y, z);
        }

        _vector toXMVector()
        {
            _vector result = XMVectorSet(x, y, z, 0.f);

            return result;
        }

        struct quaternion to_quaternion() const;

        vector3& operator=(const vector3& _rhs)
        {
            x = _rhs.x;
            y = _rhs.y;
            z = _rhs.z;
            return *this;
        }

        vector3& operator=(const XMFLOAT3& rhs)
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            return *this;
        }

        vector3& operator+=(const vector3& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        vector3& operator-=(const vector3& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }

        vector3& operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        vector3& operator/=(float scalar)
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        bool operator==(const vector3& rhs) const
        {
            return x == rhs.x && y == rhs.y && z == rhs.z;
        }

        bool operator!=(const vector3& rhs) const
        {
            return !(*this == rhs);
        }

        static const float Distance(const vector3& _a, const vector3& _b)
        {
            return vector3(_b.x - _a.x, _b.y - _a.y, _b.z - _a.z).length();
        }

        static vector3 zero()
        {
            return vector3(0.f, 0.f, 0.f);
        }

        static vector3 one()
        {
            return vector3(1.f, 1.f, 1.f);
        }

        static vector3 up()
        {
            return vector3(0.f, 1.f, 0.f);
        }

        static vector3 down()
        {
            return vector3(0.f, -1.f, 0.f);
        }

        static vector3 left()
        {
            return vector3(-1.f, 0.f, 0.f);
        }

        static vector3 right()
        {
            return vector3(1.f, 0.f, 0.f);
        }

        static vector3 forward()
        {
            return vector3(0.f, 0.f, 1.f);
        }

        static vector3 back()
        {
            return vector3(0.f, 0.f, -1.f);
        }

        float length() const
        {
            XMVECTOR v = XMVectorSet(x, y, z, 0.f);
            XMVECTOR len = XMVector3Length(v);
            return XMVectorGetX(len);
        }

        float lengthSq() const
        {
            XMVECTOR v = XMVectorSet(x, y, z, 0.f);
            XMVECTOR len = XMVector3LengthSq(v);
            return XMVectorGetX(len);
        }

        vector3 normalized() const
        {
            XMVECTOR v = XMVectorSet(x, y, z, 0.f);
            XMVECTOR normal = XMVector3Normalize(v);
            return vector3(XMVectorGetX(normal), XMVectorGetY(normal), XMVectorGetZ(normal));
        }

        float dot(const vector3& other) const
        {
            XMVECTOR a = XMVectorSet(x, y, z, 0.f);
            XMVECTOR b = XMVectorSet(other.x, other.y, other.z, 0.f);
            XMVECTOR dot = XMVector3Dot(a, b);
            return XMVectorGetX(dot);
        }

        static float dot(const vector3& a, const vector3& b)
        {
            XMVECTOR va = XMVectorSet(a.x, a.y, a.z, 0.f);
            XMVECTOR vb = XMVectorSet(b.x, b.y, b.z, 0.f);
            XMVECTOR dot = XMVector3Dot(va, vb);
            return XMVectorGetX(dot);
        }

        vector3 cross(const vector3& other) const
        {
            XMVECTOR a = XMVectorSet(x, y, z, 0.f);
            XMVECTOR b = XMVectorSet(other.x, other.y, other.z, 0.f);
            XMVECTOR cross = XMVector3Cross(a, b);
            return vector3(XMVectorGetX(cross), XMVectorGetY(cross), XMVectorGetZ(cross));
        }

        static vector3 Cross(const vector3& a, const vector3& b)
        {
            XMVECTOR va = XMVectorSet(a.x, a.y, a.z, 0.f);
            XMVECTOR vb = XMVectorSet(b.x, b.y, b.z, 0.f);
            XMVECTOR cross = XMVector3Cross(va, vb);
            return vector3(XMVectorGetX(cross), XMVectorGetY(cross), XMVectorGetZ(cross));
        }

        operator XMFLOAT3() const
        {
            return XMFLOAT3(x, y, z);
        }
    };

    inline vector3 operator+(const vector3& lhs, const vector3& rhs)
    {
        return vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    }

    inline vector3 operator-(const vector3& lhs, const vector3& rhs)
    {
        return vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
    }

    inline vector3 operator*(const vector3& vec, float scalar)
    {
        return vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
    }

    inline vector3 operator*(float scalar, const vector3& vec)
    {
        return vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
    }

    inline vector3 operator/(const vector3& vec, float scalar)
    {
        return vector3(vec.x / scalar, vec.y / scalar, vec.z / scalar);
    }

    inline vector3 operator*(const vector3& lhs, const vector3& rhs)
    {
        return vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
    }

    inline vector3 operator/(const vector3& lhs, const vector3& rhs)
    {
        return vector3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
    }

    inline vector3 operator-(const vector3& v)
    {
        return vector3(-v.x, -v.y, -v.z);
    }
#pragma endregion

#pragma region vector3Int
    struct vector3Int
    {
        int x;
        int y;
        int z;

        vector3Int()
            : x(0), y(0), z(0)
        {
        }

        vector3Int(_int _x, _int _y, _int _z)
            : x(_x), y(_y), z(_z)
        {
        }

        vector3Int(UINT _x, UINT _y, UINT _z)
            : x((_int)_x), y((_int)_y), z((_int)_z)
        {
        }

        vector3Int(const vector3& v)
            : x(static_cast<int>(v.x)), y(static_cast<int>(v.y)), z(static_cast<int>(v.z))
        {
        }

        vector3Int& operator=(const vector3Int& rhs)
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            return *this;
        }

        vector3Int& operator+=(const vector3Int& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        vector3Int& operator-=(const vector3Int& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }

        vector3Int& operator*=(int scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        vector3Int& operator/=(int scalar)
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        bool operator==(const vector3Int& rhs) const
        {
            return x == rhs.x && y == rhs.y && z == rhs.z;
        }

        bool operator!=(const vector3Int& rhs) const
        {
            return !(*this == rhs);
        }

        float length() const
        {
            return sqrtf(static_cast<float>(x * x + y * y + z * z));
        }

        int lengthSq() const
        {
            return x * x + y * y + z * z;
        }

        vector3 to_vector3() const
        {
            return vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
        }

        static vector3Int zero()
        {
            return vector3Int(0, 0, 0);
        }

        static vector3Int one()
        {
            return vector3Int(1, 1, 1);
        }

        static vector3Int up()
        {
            return vector3Int(0, 1, 0);
        }

        static vector3Int down()
        {
            return vector3Int(0, -1, 0);
        }

        static vector3Int left()
        {
            return vector3Int(-1, 0, 0);
        }

        static vector3Int right()
        {
            return vector3Int(1, 0, 0);
        }

        static vector3Int forward()
        {
            return vector3Int(0, 0, 1);
        }

        static vector3Int back()
        {
            return vector3Int(0, 0, -1);
        }

        static int Distance(const vector3Int& a, const vector3Int& b)
        {
            int dx = b.x - a.x;
            int dy = b.y - a.y;
            int dz = b.z - a.z;
            return static_cast<int>(sqrtf(static_cast<float>(dx * dx + dy * dy + dz * dz)));
        }

        static int ManhattanDistance(const vector3Int& a, const vector3Int& b)
        {
            return abs(b.x - a.x) + abs(b.y - a.y) + abs(b.z - a.z);
        }
    };

    // 연산자 오버로딩
    inline vector3Int operator+(const vector3Int& lhs, const vector3Int& rhs)
    {
        return vector3Int(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    }

    inline vector3Int operator-(const vector3Int& lhs, const vector3Int& rhs)
    {
        return vector3Int(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
    }

    inline vector3Int operator*(const vector3Int& vec, int scalar)
    {
        return vector3Int(vec.x * scalar, vec.y * scalar, vec.z * scalar);
    }

    inline vector3Int operator*(int scalar, const vector3Int& vec)
    {
        return vector3Int(vec.x * scalar, vec.y * scalar, vec.z * scalar);
    }

    inline vector3Int operator/(const vector3Int& vec, int scalar)
    {
        return vector3Int(vec.x / scalar, vec.y / scalar, vec.z / scalar);
    }

    inline vector3Int operator-(const vector3Int& v)
    {
        return vector3Int(-v.x, -v.y, -v.z);
    }
#pragma endregion

#pragma region quaternion
    struct quaternion
    {
        float x, y, z, w;

        quaternion()
            : x(0.f), y(0.f), z(0.f), w(1.f)
        {
        }

        quaternion(float _x, float _y, float _z, float _w)
            : x(_x), y(_y), z(_z), w(_w)
        {
        }

        quaternion(const XMFLOAT4& q)
            : x(q.x), y(q.y), z(q.z), w(q.w)
        {
        }

        quaternion(_vector vec)
        {
            _float4 temp;
            XMStoreFloat4(&temp, vec);
            x = temp.x; y = temp.y; z = temp.z; w = temp.w;
        }

        const _float4 toFloat4()
        {
            return _float4(x, y, z, w);
        }

        _vector toXMVector()
        {
            _vector result = XMVectorSet(x, y, z, w);

            return result;
        }

        quaternion& operator=(const quaternion& rhs)
        {
            x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
            return *this;
        }

        quaternion& operator=(const XMFLOAT4& rhs)
        {
            x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
            return *this;
        }

        bool operator==(const quaternion& rhs) const
        {
            return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
        }

        bool operator!=(const quaternion& rhs) const
        {
            return !(*this == rhs);
        }

        quaternion operator*(const quaternion& rhs) const
        {
            XMVECTOR q1 = XMVectorSet(x, y, z, w);
            XMVECTOR q2 = XMVectorSet(rhs.x, rhs.y, rhs.z, rhs.w);

            XMVECTOR qResult = XMQuaternionMultiply(q1, q2);

            return quaternion(qResult);
        }

        quaternion& operator*=(const quaternion& rhs)
        {
            *this = *this * rhs;
            return *this;
        }

        quaternion normalized() const
        {
            XMVECTOR q = XMVectorSet(x, y, z, w);

            XMVECTOR qNormalized = XMQuaternionNormalize(q);

            return quaternion(qNormalized);
        }

        quaternion conjugated() const
        {
            return quaternion(-x, -y, -z, w);
        }

        float dot(const quaternion& rhs) const
        {
            XMVECTOR q1 = XMVectorSet(x, y, z, w);
            XMVECTOR q2 = XMVectorSet(rhs.x, rhs.y, rhs.z, rhs.w);

            XMVECTOR result = XMQuaternionDot(q1, q2);

            return XMVectorGetX(result);
        }

        static quaternion identity()
        {
            return quaternion(0.f, 0.f, 0.f, 1.f);
        }

        static quaternion from_euler(const vector3& euler_deg)
        {
            return from_euler(euler_deg.x, euler_deg.y, euler_deg.z);
        }

        static quaternion from_euler(float _pitch, float _yaw, float _roll)
        {
            XMVECTOR q = XMQuaternionRotationRollPitchYaw
            (
                XMConvertToRadians(_pitch),
                XMConvertToRadians(_yaw),
                XMConvertToRadians(_roll)
            );

            return quaternion(q);
        }

        vector3 to_euler() const
        {
            vector3 euler;

            XMFLOAT4 quat;
            XMStoreFloat4(&quat, *this);

            float sinp = 2.f * (quat.w * quat.x - quat.z * quat.y);

            euler.y = atan2f
            (
                2.f * (quat.w * quat.y + quat.x * quat.z),
                1.f - 2.f * (quat.y * quat.y + quat.z * quat.z)
            );

            if (fabs(sinp) >= 1.f)
                euler.x = XM_PIDIV2 * (sinp > 0 ? 1.f : -1.f);
            else
                euler.x = asinf(sinp);

            euler.z = atan2f
            (
                2.f * (quat.w * quat.z + quat.x * quat.y),
                1.f - 2.f * (quat.x * quat.x + quat.z * quat.z)
            );

            return vector3
            (
                XMConvertToDegrees(euler.x),
                XMConvertToDegrees(euler.y),
                XMConvertToDegrees(euler.z)
            );
        }

        XMMATRIX to_matrix() const
        {
            XMVECTOR q = XMVectorSet(x, y, z, w);

            return XMMatrixRotationQuaternion(q);
        }

        operator XMVECTOR() const
        {
            return XMVectorSet(x, y, z, w);
        }
    };

#pragma endregion

#pragma region ColorValue
    struct ColorValue
    {
        BYTE r;
        BYTE g;
        BYTE b;
        BYTE a;

        ColorValue()
            : r(255), g(255), b(255), a(255)
        {
        }

        ColorValue(BYTE _r, BYTE _g, BYTE _b, BYTE _a = 255)
            : r(_r), g(_g), b(_b), a(_a)
        {
        }

        constexpr UINT32 ARGB(BYTE a, BYTE r, BYTE g, BYTE b)
        {
            return (static_cast<UINT32>(a) << 24) |
                (static_cast<UINT32>(r) << 16) |
                (static_cast<UINT32>(g) << 8) |
                static_cast<UINT32>(b);
        }

        XMFLOAT4 dColor() const
        {
            return XMFLOAT4(r, g, b, a);
        }

        //COLORREF rColor() const
        //{
        //    return RGB(r, g, b); 
        //}

        D3DCOLORVALUE dvColor() const
        {
            D3DCOLORVALUE color;
            color.r = float(r / 255.f);
            color.g = float(g / 255.f);
            color.b = float(b / 255.f);
            color.a = float(a / 255.f);
            return color;
        }

        ColorValue(D3DCOLOR d3dColor)
        {
            a = (d3dColor >> 24) & 0xFF;
            r = (d3dColor >> 16) & 0xFF;
            g = (d3dColor >> 8) & 0xFF;
            b = (d3dColor >> 0) & 0xFF;
        }

        ColorValue& operator=(const D3DCOLOR& d3dColor)
        {
            a = (d3dColor >> 24) & 0xFF;
            r = (d3dColor >> 16) & 0xFF;
            g = (d3dColor >> 8) & 0xFF;
            b = (d3dColor >> 0) & 0xFF;
            return *this;
        }

        //operator D3DCOLOR() const
        //{
        //    return D3DCOLOR_ARGB(a, r, g, b);
        //}

        //D3DCOLOR value() const
        //{
        //    return D3DCOLOR_ARGB(a, r, g, b);
        //}

        static ColorValue red() { return ColorValue(255, 0, 0, 255); }
        static ColorValue green() { return ColorValue(0, 255, 0, 255); }
        static ColorValue blue() { return ColorValue(0, 0, 255, 255); }
        static ColorValue white() { return ColorValue(255, 255, 255, 255); }
        static ColorValue black() { return ColorValue(0, 0, 0, 255); }
        static ColorValue yellow() { return ColorValue(255, 255, 0, 255); }
        static ColorValue cyan() { return ColorValue(0, 255, 255, 255); }
        static ColorValue gray() { return ColorValue(128, 128, 128, 255); }
        static ColorValue gray(float _value)
        {
            BYTE v = BYTE(255 * _value);
            return ColorValue(v, v, v, 255);
        }
        static ColorValue magenta() { return ColorValue(255, 0, 255, 255); }
        static ColorValue transparent() { return ColorValue(0, 0, 0, 0); }
    };
#pragma endregion

#pragma region VertexBuffer
    struct VertexTexNormalBuffer
    {
        _float3  position;
        _float3  normal;
        _float2 uv;
    };

    struct VertexTexNormalTangentBuffer
    {
        _float3 position;
        _float3 normal;
        _float2 uv;
        _float3 tangent;
    };
#pragma endregion;

#pragma region ShaderBuffer
    struct MatrixCB
    {
        _matrix world = XMMatrixIdentity();
        _matrix view = XMMatrixIdentity();
        _matrix proj = XMMatrixIdentity();
    };

    struct MaterialCB
    {
        _float4 baseColor;
        _bool useTexture;
        _float3 padding;
    };
#pragma endregion

    typedef struct tagIndex16
    {
        _ushort  _0;
        _ushort  _1;
        _ushort  _2;

    }INDEX16;

    typedef struct tagIndex32
    {
        _ulong	_0;
        _ulong	_1;
        _ulong	_2;

    }INDEX32;

#endif // Engine_Struct_h__
