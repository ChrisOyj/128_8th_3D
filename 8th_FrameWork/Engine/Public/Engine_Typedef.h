#pragma once

namespace Engine
{
	typedef unsigned char				_ubyte;
	typedef signed char					_byte;

	typedef unsigned short				_ushort;
	typedef signed short				_short;

	typedef unsigned int				_uint;
	typedef signed int					_int;

	typedef unsigned long				_ulong;
	typedef signed long					_long;

	typedef unsigned long long			_ulonglong;
	typedef signed long long			_longlong;

	typedef float						_float;
	typedef double						_double;

	typedef bool						_bool;

	typedef wchar_t						_tchar;

	typedef size_t						_hashcode;

	typedef XMFLOAT2					_float2;
	typedef XMFLOAT3					_float3;
	typedef XMFLOAT4					_float4;
	typedef XMFLOAT4X4					_float4x4;




	//typedef struct XMFLOAT4X4_DERIVED : public XMFLOAT4X4
	//{
	//	XMFLOAT4X4_DERIVED& operator = (const XMFLOAT4X4& matrix)
	//	{
	//		memcpy(this, &matrix, sizeof(XMFLOAT4X4));
	//		return *this;
	//	}

	//	XMFLOAT4X4_DERIVED()
	//	{
	//		*this = XMMatrixMultiply();
	//	}

	//	

	//	XMFLOAT4X4_DERIVED	operator * (const XMFLOAT4X4& matrix)
	//	{
	//		XMFLOAT4X4_DERIVED matResult;
	//		XMFLOAT4X4	matTemp = *this;

	//		matResult = matTemp * matrix;


	//		return matResult;
	//	}

	//	XMFLOAT4X4_DERIVED&	Identity()
	//	{
	//		XMFLOAT4X4Identity(this);
	//		return *this;
	//	}
	//	XMFLOAT4X4_DERIVED&	Inverse()
	//	{
	//		XMFLOAT4X4Inverse(this, nullptr, this);
	//		return *this;
	//	}

	//	XMFLOAT4X4_DERIVED	Transpose()
	//	{
	//		XMFLOAT4X4_DERIVED	mat;
	//		XMFLOAT4X4Transpose(&mat, this);
	//		return mat;
	//	}

	//}_float4x4;

	//typedef struct XMFLOAT3_DERIVED : public XMFLOAT3
	//{
	//	XMFLOAT3_DERIVED()
	//	{}

	//	XMFLOAT3_DERIVED(_float _x, _float _y, _float _z)
	//		: XMFLOAT3(_x, _y, _z)
	//	{}

	//	XMFLOAT3_DERIVED&	operator = (const XMFLOAT3& vec3)
	//	{
	//		memcpy(this, &vec3, sizeof(XMFLOAT3));
	//		return *this;
	//	}

	//	XMFLOAT3_DERIVED	operator + (const XMFLOAT3_DERIVED& vec3)
	//	{
	//		return XMFLOAT3_DERIVED(x + vec3.x, y + vec3.y, z + vec3.z);
	//	}

	//	XMFLOAT3_DERIVED	operator - (const XMFLOAT3_DERIVED& vec3)
	//	{
	//		return XMFLOAT3_DERIVED(x - vec3.x, y - vec3.y, z - vec3.z);
	//	}

	//	XMFLOAT3_DERIVED&	operator -= (const XMFLOAT3_DERIVED& vec3)
	//	{
	//		x -= vec3.x;
	//		y -= vec3.y;
	//		z -= vec3.z;

	//		return *this;
	//	}

	//	XMFLOAT3_DERIVED&	operator -= (const float& _f)
	//	{
	//		x -= _f;
	//		y -= _f;
	//		z -= _f;

	//		return *this;
	//	}

	//	/*XMFLOAT3_DERIVED&	operator += (const float& _f)
	//	{
	//		x += _f;
	//		y += _f;
	//		z += _f;

	//		return *this;
	//	}*/

	//	XMFLOAT3_DERIVED	operator * (const float& _f)
	//	{
	//		return XMFLOAT3_DERIVED(x * _f, y * _f, z * _f);
	//	}

	//	XMFLOAT3_DERIVED&	operator *= (const float& _f)
	//	{
	//		x *= _f;
	//		y *= _f;
	//		z *= _f;

	//		return *this;
	//	}

	//	bool operator == (const float& _f)
	//	{
	//		if (x == _f && y == _f && z == _f)
	//			return true;

	//		return false;
	//	}

	//	bool operator != (const float& _f)
	//	{
	//		return !(*this == _f);
	//	}

	//	_float  Length() {
	//		return XMVector3Length((XMVECTOR)(*this));
	//	}

	//	XMFLOAT3_DERIVED&  Normalize() {
	//		XMVector3NormalizeEst(*this);
	//		return *this;
	//	}

	//	_float Dot(const XMFLOAT3_DERIVED& _other)
	//	{
	//		return D3DXVec3Dot(this, &_other);
	//	}

	//	XMFLOAT3_DERIVED& Cross(const XMFLOAT3_DERIVED& _other)
	//	{
	//		D3DXVec3Cross(this, this, &_other);
	//		return *this;
	//	}

	//	XMFLOAT3_DERIVED& Multiply(const XMFLOAT4X4_DERIVED& _float4x4, bool bNormal = false)
	//	{
	//		if (bNormal)
	//		{
	//			D3DXVec3TransformNormal(this, this, &_float4x4);
	//		}
	//		else
	//		{
	//			D3DXVec3TransformCoord(this, this, &_float4x4);
	//		}

	//		return *this;
	//	}

	//}_float3;

	//typedef struct D3DXVECTOR4_DERIVED : public D3DXVECTOR4
	//{
	//	D3DXVECTOR4_DERIVED()
	//	{}

	//	D3DXVECTOR4_DERIVED(_float _x, _float _y, _float _z, _float _w)
	//		: D3DXVECTOR4(_x, _y, _z, _w)
	//	{}



	//	_float  Length() {
	//		return D3DXVec4Length(this);
	//	}

	//	D3DXVECTOR4_DERIVED&  Normalize() {
	//		D3DXVec4Normalize(this, this);
	//		return *this;
	//	}

	//	_float Dot(const D3DXVECTOR4_DERIVED& _other)
	//	{
	//		return D3DXVec4Dot(this, &_other);
	//	}

	//	D3DXVECTOR4_DERIVED& Cross(const D3DXVECTOR4_DERIVED& V1, const D3DXVECTOR4_DERIVED& V2)
	//	{
	//		D3DXVec4Cross(this, this, &V1, &V2);
	//		return *this;
	//	}

	//	D3DXVECTOR4_DERIVED& Multiply(const XMFLOAT4X4_DERIVED& _float4x4)
	//	{
	//		D3DXVec4Transform(this, this, &_float4x4);

	//		return *this;
	//	}

	//	D3DXVECTOR4_DERIVED	operator * (const XMFLOAT4X4_DERIVED& _float4x4)
	//	{
	//		D3DXVECTOR4_DERIVED vResult;
	//		D3DXVec4Transform(&vResult, this, &_float4x4);
	//		return vResult;
	//	}

	//	D3DXVECTOR4_DERIVED	operator *= (const XMFLOAT4X4_DERIVED& _float4x4)
	//	{
	//		D3DXVec4Transform(this, this, &_float4x4);
	//		return *this;
	//	}

	//}_float4;


	
}
