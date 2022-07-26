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

	typedef struct XMMATRIX_DERIVED : public XMMATRIX
	{
		XMMATRIX_DERIVED& operator = (const XMMATRIX& matrix)
		{
			memcpy(this, &matrix, sizeof(XMMATRIX));
			return *this;
		}

		XMMATRIX_DERIVED operator * (const XMMATRIX& matrix)
		{
			XMMATRIX_DERIVED	matResult = *this * matrix;
			return matResult;
		}

		XMMATRIX_DERIVED& operator *= (const XMMATRIX& matrix)
		{
			return *this *= matrix;
		}

		XMMATRIX_DERIVED& Inverse()
		{
			return *this = XMMatrixInverse(nullptr, *this);
		}

		XMMATRIX_DERIVED& Identity()
		{
			return *this = XMMatrixIdentity();
		}

		XMMATRIX_DERIVED& Transpose()
		{
			return *this = XMMatrixTranspose(*this);
		}


	}_float4x4;


	typedef struct XMFLOAT4_DERIVED : public XMFLOAT4
	{
		XMFLOAT4_DERIVED()
			: XMFLOAT4()
		{}

		XMFLOAT4_DERIVED(const _float _x,
			const _float _y,
			const _float _z,
			const _float _w = 1.f)
			: XMFLOAT4(x, y, z, w)
		{}

		XMFLOAT4_DERIVED& operator = (const XMFLOAT4& _other)
		{
			x = _other.x;
			y = _other.z;
			z = _other.z;
			w = _other.w;


			return *this;
		}

		XMFLOAT4_DERIVED& operator = (const _float& _f)
		{
			x = _f;
			y = _f;
			z = _f;


			return *this;
		}

		XMFLOAT4_DERIVED operator + (const XMFLOAT4& _other)
		{
			XMFLOAT4_DERIVED	vOutput;
			vOutput.x = x + _other.x;
			vOutput.y = y + _other.y;
			vOutput.z = z + _other.z;
			vOutput.w = w;

			return vOutput;
		}

		XMFLOAT4_DERIVED operator + (const _float& _f)
		{
			XMFLOAT4_DERIVED	vOutput;
			vOutput.x = x + _f;
			vOutput.y = y + _f;
			vOutput.z = z + _f;
			vOutput.w = w;

			return vOutput;
		}

		XMFLOAT4_DERIVED operator - (const XMFLOAT4& _other)
		{
			XMFLOAT4_DERIVED	vOutput;
			vOutput.x = x - _other.x;
			vOutput.y = y - _other.y;
			vOutput.z = z - _other.z;
			vOutput.w = w;

			return vOutput;
		}

		XMFLOAT4_DERIVED operator - (const _float& _f)
		{
			XMFLOAT4_DERIVED	vOutput;
			vOutput.x = x - _f;
			vOutput.y = y - _f;
			vOutput.z = z - _f;
			vOutput.w = w;

			return vOutput;
		}

		XMFLOAT4_DERIVED operator * (const XMFLOAT4& _other)
		{
			XMFLOAT4_DERIVED	vOutput;
			vOutput.x = x * _other.x;
			vOutput.y = y * _other.y;
			vOutput.z = z * _other.z;
			vOutput.w = w;

			return vOutput;
		}

		XMFLOAT4_DERIVED operator * (const _float& _f)
		{
			XMFLOAT4_DERIVED	vOutput;
			vOutput.x = x * _f;
			vOutput.y = y * _f;
			vOutput.z = z * _f;
			vOutput.w = w;

			return vOutput;
		}

		XMFLOAT4_DERIVED operator / (const XMFLOAT4& _other)
		{
			XMFLOAT4_DERIVED	vOutput;
			vOutput.x = x / _other.x;
			vOutput.y = y / _other.y;
			vOutput.z = z / _other.z;
			vOutput.w = w;

			return vOutput;
		}

		XMFLOAT4_DERIVED operator / (const _float& _f)
		{
			XMFLOAT4_DERIVED	vOutput;
			vOutput.x = x / _f;
			vOutput.y = y / _f;
			vOutput.z = z / _f;
			vOutput.w = w;

			return vOutput;
		}

		XMFLOAT4_DERIVED& operator += (const XMFLOAT4& _other)
		{
			x += _other.x;
			y += _other.y;
			z += _other.z;

			return *this;
		}

		XMFLOAT4_DERIVED& operator += (const _float& _f)
		{
			x += _f;
			y += _f;
			z += _f;

			return *this;
		}

		XMFLOAT4_DERIVED& operator -= (const XMFLOAT4& _other)
		{
			x -= _other.x;
			y -= _other.y;
			z -= _other.z;

			return *this;
		}

		XMFLOAT4_DERIVED& operator -= (const _float& _f)
		{
			x -= _f;
			y -= _f;
			z -= _f;

			return *this;
		}

		XMFLOAT4_DERIVED& operator *= (const XMFLOAT4& _other)
		{
			x *= _other.x;
			y *= _other.y;
			z *= _other.z;

			return *this;
		}

		XMFLOAT4_DERIVED& operator *= (const _float& _f)
		{
			x *= _f;
			y *= _f;
			z *= _f;

			return *this;
		}

		XMFLOAT4_DERIVED& operator /= (const XMFLOAT4& _other)
		{
			x /= _other.x;
			y /= _other.y;
			z /= _other.z;

			return *this;
		}

		XMFLOAT4_DERIVED& operator /= (const _float& _f)
		{
			x /= _f;
			y /= _f;
			z /= _f;

			return *this;
		}

		_bool operator == (const XMFLOAT4& _other)
		{
			if (x == _other.x &&
				y == _other.y &&
				z == _other.z &&
				w == _other.w)
				return true;

			return false;
		}

		_bool operator != (const XMFLOAT4& _other)
		{
			return !(*this == _other);
		}

		/* transform */
		XMFLOAT4_DERIVED operator * (const XMMATRIX& _otherMatrix)
		{
			XMVECTOR	vec = { x, y, z, w };

			XMVECTOR	vTransform = XMVector4Transform(vec, _otherMatrix);

			XMFLOAT4_DERIVED	vResult = XMFLOAT4_DERIVED(vTransform.m128_f32[0],
				vTransform.m128_f32[1],
				vTransform.m128_f32[2],
				vTransform.m128_f32[3]);

			return vResult;
		}

		XMFLOAT4_DERIVED& operator *= (const XMMATRIX& _otherMatrix)
		{
			XMVECTOR	vec = { x, y, z, w };

			XMVECTOR	vTransform = XMVector4Transform(vec, _otherMatrix);

			XMFLOAT4_DERIVED	vResult = XMFLOAT4_DERIVED(vTransform.m128_f32[0],
				vTransform.m128_f32[1],
				vTransform.m128_f32[2],
				vTransform.m128_f32[3]);

			return *this = vResult;
		}


		_float Length()
		{
			FXMVECTOR	vec = { x, y, z, w };

			return XMVector4Length(vec).m128_f32[0];
		}

		_float	Dot(const XMFLOAT4& _other)
		{
			FXMVECTOR	Myvec = { x, y, z, w };
			FXMVECTOR	Othervec = { _other.x, _other.y, _other.z, w };

			return XMVector4Dot(Myvec, Othervec).m128_f32[0];
		}

		XMFLOAT4_DERIVED	Cross(const XMFLOAT4& _other)
		{
			FXMVECTOR	Myvec = { x, y, z, 0.f };
			FXMVECTOR	Othervec = { _other.x, _other.y, _other.z, 0.f };
			FXMVECTOR	vCross = XMVector3Cross(Myvec, Othervec);

			XMFLOAT4_DERIVED	vResult = XMFLOAT4_DERIVED(vCross.m128_f32[0],
				vCross.m128_f32[1],
				vCross.m128_f32[2],
				0.f);

			return vResult;
		}

		XMFLOAT4_DERIVED& Normalize()
		{
			FXMVECTOR	Myvec = { x, y, z, w };

			FXMVECTOR	vNormalize = XMVector4Normalize(Myvec);

			XMFLOAT4_DERIVED	vResult = XMFLOAT4_DERIVED(vNormalize.m128_f32[0],
				vNormalize.m128_f32[1],
				vNormalize.m128_f32[2],
				0.f);

			return *this = vResult;
		}

		FXMVECTOR	XMVector()
		{
			FXMVECTOR	Myvec = { x, y, z, w };
			return Myvec;
		}

	}_float4;
}
