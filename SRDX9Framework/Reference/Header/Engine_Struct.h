#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{	
#pragma region vector2
    struct vector2
    {
        float x;
        float y;

        vector2()
            : x(0.f), y(0.f)
        {
        }

        vector2(float _x, float _y)
            : x(_x), y(_y)
        {
        }

        vector2(const D3DXVECTOR2& v)
            : x(v.x), y(v.y)
        {
        }

        vector2& operator=(const vector2& rhs)
        {
            x = rhs.x;
            y = rhs.y;
            return *this;
        }

        vector2& operator=(const D3DXVECTOR2& rhs)
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
            D3DXVECTOR2 v(x, y);
            return D3DXVec2Length(&v);
        }

        float lengthSq() const
        {
            D3DXVECTOR2 v(x, y);
            return D3DXVec2LengthSq(&v);
        }

        vector2 normalized() const
        {
            D3DXVECTOR2 v(x, y);
            D3DXVECTOR2 result;
            D3DXVec2Normalize(&result, &v);
            return vector2(result);
        }

        float dot(const vector2& other) const
        {
            D3DXVECTOR2 a(x, y);
            D3DXVECTOR2 b(other.x, other.y);
            return D3DXVec2Dot(&a, &b);
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

        operator D3DXVECTOR2() const
        {
            return D3DXVECTOR2(x, y);
        }

        D3DXVECTOR2 vector() const
        {
            return D3DXVECTOR2(x, y);
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

        vector2Int(int _x, int _y)
            : x(_x), y(_y)
        {
        }

        vector2Int(const vector2& v)
            : x(static_cast<int>(v.x)), y(static_cast<int>(v.y))
        {
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

        vector2 to_float() const
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

        vector3(const D3DXVECTOR3& v)
            : x(v.x), y(v.y), z(v.z)
        {
        }

        vector3& operator=(const vector3& rhs)
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            return *this;
        }

        vector3& operator=(const D3DXVECTOR3& rhs)
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

        static vector3 backward()
        {
            return vector3(0.f, 0.f, -1.f);
        }

        float length() const
        {
            D3DXVECTOR3 v(x, y, z);
            return D3DXVec3Length(&v);
        }

        float lengthSq() const
        {
            D3DXVECTOR3 v(x, y, z);
            return D3DXVec3LengthSq(&v);
        }

        vector3 normalized() const
        {
            D3DXVECTOR3 v(x, y, z);
            D3DXVECTOR3 result;
            D3DXVec3Normalize(&result, &v);
            return vector3(result);
        }

        float dot(const vector3& other) const
        {
            D3DXVECTOR3 a(x, y, z);
            D3DXVECTOR3 b(other.x, other.y, other.z);
            return D3DXVec3Dot(&a, &b);
        }

        static float dot(const vector3& a, const vector3& b)
        {
            D3DXVECTOR3 da(a.x, a.y, a.z);
            D3DXVECTOR3 db(b.x, b.y, b.z);
            return D3DXVec3Dot(&da, &db);
        }

        vector3 cross(const vector3& other) const
        {
            D3DXVECTOR3 a(x, y, z);
            D3DXVECTOR3 b(other.x, other.y, other.z);
            D3DXVECTOR3 result;
            D3DXVec3Cross(&result, &a, &b);
            return vector3(result);
        }

        static vector3 Cross(const vector3& a, const vector3& b)
        {
            D3DXVECTOR3 va(a.x, a.y, a.z);
            D3DXVECTOR3 vb(b.x, b.y, b.z);
            D3DXVECTOR3 result;
            D3DXVec3Cross(&result, &va, &vb);
            return vector3(result);
        }

        operator D3DXVECTOR3() const
        {
            return D3DXVECTOR3(x, y, z);
        }

        D3DXVECTOR3 vector() const
        {
            return D3DXVECTOR3(x, y, z);
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

        quaternion(const D3DXQUATERNION& q)
            : x(q.x), y(q.y), z(q.z), w(q.w)
        {
        }

        D3DXQUATERNION dQuaternion() const
        {
            return D3DXQUATERNION(x, y, z, w);
        }

        quaternion& operator=(const quaternion& rhs)
        {
            x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
            return *this;
        }

        quaternion& operator=(const D3DXQUATERNION& rhs)
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
            D3DXQUATERNION a(x, y, z, w);
            D3DXQUATERNION b(rhs.x, rhs.y, rhs.z, rhs.w);
            D3DXQUATERNION result;
            D3DXQuaternionMultiply(&result, &a, &b);
            return quaternion(result);
        }

        quaternion& operator*=(const quaternion& rhs)
        {
            *this = *this * rhs;
            return *this;
        }

        quaternion normalized() const
        {
            D3DXQUATERNION q(x, y, z, w);
            D3DXQuaternionNormalize(&q, &q);
            return quaternion(q);
        }

        quaternion conjugated() const
        {
            return quaternion(-x, -y, -z, w);
        }

        float dot(const quaternion& rhs) const
        {
            D3DXQUATERNION a(x, y, z, w);
            D3DXQUATERNION b(rhs.x, rhs.y, rhs.z, rhs.w);
            return D3DXQuaternionDot(&a, &b);
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
            D3DXQUATERNION q;

            D3DXQuaternionRotationYawPitchRoll(
                &q,
                D3DXToRadian(_yaw),
                D3DXToRadian(_pitch),
                D3DXToRadian(_roll)
            );

            return quaternion(q);
        }

        static vector3 to_euler(const D3DXQUATERNION& _q)
        {
            vector3 euler;

            euler.y = atan2f(2.f * (_q.w * _q.y + _q.x * _q.z), 1.f - 2.f * (_q.y * _q.y + _q.z * _q.z));

            float sinp = 2.f * (_q.w * _q.x - _q.z * _q.y);
            if (fabs(sinp) >= 1)
                euler.x = D3DX_PI / 2 * (sinp > 0 ? 1 : -1);
            else
                euler.x = asinf(sinp);

            euler.z = atan2f(2.f * (_q.w * _q.z + _q.x * _q.y), 1.f - 2.f * (_q.x * _q.x + _q.z * _q.z));

            return vector3(D3DXToDegree(euler.x), D3DXToDegree(euler.y), D3DXToDegree(euler.z));
        }

        D3DXMATRIX to_matrix() const
        {
            D3DXMATRIX mat;
            D3DXQUATERNION q(x, y, z, w);
            D3DXMatrixRotationQuaternion(&mat, &q);
            return mat;
        }

        operator D3DXQUATERNION() const
        {
            return D3DXQUATERNION(x, y, z, w);
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

        D3DCOLOR dColor() const { return D3DCOLOR_ARGB(a, r, g, b); }

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

        operator D3DCOLOR() const
        {
            return D3DCOLOR_ARGB(a, r, g, b);
        }

        D3DCOLOR value() const
        {
            return D3DCOLOR_ARGB(a, r, g, b);
        }

        static ColorValue red() { return ColorValue(255, 0, 0, 255); }
        static ColorValue green() { return ColorValue(0, 255, 0, 255); }
        static ColorValue blue() { return ColorValue(0, 0, 255, 255); }
        static ColorValue white() { return ColorValue(255, 255, 255, 255); }
        static ColorValue black() { return ColorValue(0, 0, 0, 255); }
        static ColorValue yellow() { return ColorValue(255, 255, 0, 255); }
        static ColorValue cyan() { return ColorValue(0, 255, 255, 255); }
        static ColorValue magenta() { return ColorValue(255, 0, 255, 255); }
        static ColorValue transparent() { return ColorValue(0, 0, 0, 0); }
    };
#pragma endregion

	typedef struct tagVertexColor
	{
		_vec3		vPosition;		
		_ulong		dwColor;

	}VTXCOL;

	const _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTexture
	{
		_vec3		vPosition;
        D3DCOLOR color;
		_vec2		vTexUV;

	}VTXTEX;

    typedef struct tagVertexLightTexture
    {
        _vec3 vPosition;
        _vec3 vNormalVector;
        _vec3 vTexUV;

    }VTLTEX;

	const _ulong FVF_TEX = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    const _ulong FVF_LTEX = (D3DFVF_XYZ | D3DFVF_NORMAL);

	typedef struct tagVertexCubeTexture
	{
		_vec3		vPosition;
		_vec3		vTexUV;

	}VTXCUBE;

	const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

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
}

#endif // Engine_Struct_h__
