#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{	
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

        vector3 cross(const vector3& other) const
        {
            D3DXVECTOR3 a(x, y, z);
            D3DXVECTOR3 b(other.x, other.y, other.z);
            D3DXVECTOR3 result;
            D3DXVec3Cross(&result, &a, &b);
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

        static ColorValue red() { return ColorValue(255, 0, 0); }
        static ColorValue green() { return ColorValue(0, 255, 0); }
        static ColorValue blue() { return ColorValue(0, 0, 255); }
        static ColorValue white() { return ColorValue(255, 255, 255); }
        static ColorValue black() { return ColorValue(0, 0, 0); }
        static ColorValue yellow() { return ColorValue(255, 255, 0); }
        static ColorValue cyan() { return ColorValue(0, 255, 255); }
        static ColorValue magenta() { return ColorValue(255, 0, 255); }
        static ColorValue transparent() { return ColorValue(0, 0, 0, 0); }
    };

	typedef struct tagVertexColor
	{
		_vec3		vPosition;		
		_ulong		dwColor;

	}VTXCOL;

	const _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTexture
	{
		_vec3		vPosition;
		_vec2		vTexUV;

	}VTXTEX;
	const _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_TEX1;


	typedef struct tagVertexCubeTexture
	{
		_vec3		vPosition;
		_vec3		vTexUV;

	}VTXCUBE;

	const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0); // 텍스처의 UV 좌표 값을 FLOAT형 3개로 표현하겠다는 매크로(괄호안의 숫자 0의 의미는 본래 버텍스에 텍스쳐 UV값이 여러개가 올 수 있는데 그중 0번째 값을 지정하겠다는 의미)

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
