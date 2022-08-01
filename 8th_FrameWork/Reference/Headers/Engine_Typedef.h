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


	typedef struct XMFLOAT4X4_DERIVED : public XMFLOAT4X4
	{
		XMFLOAT4X4_DERIVED()
			: XMFLOAT4X4()
		{}

		XMMATRIX XMLoad()
		{
			return XMLoadFloat4x4(this);
		}

		XMFLOAT4X4_DERIVED& operator = (const XMFLOAT4X4& _matrix)
		{
			XMMATRIX	matrix = XMLoadFloat4x4(&_matrix);
			*this = matrix;

			return *this;
		}

		XMFLOAT4X4_DERIVED& operator = (FXMMATRIX _matrix)
		{
			XMStoreFloat4x4(this, _matrix);

			return *this;
		}

		XMFLOAT4X4_DERIVED operator * (const XMFLOAT4X4& matrix)
		{
			XMMATRIX	matResult = XMMatrixMultiply(XMLoad(), XMLoadFloat4x4(&matrix));
			XMFLOAT4X4_DERIVED	matTemp;
			matTemp = matResult;

			return matTemp;
		}

		XMFLOAT4X4_DERIVED& operator *= (const XMFLOAT4X4& matrix)
		{
			XMMATRIX	matResult = XMMatrixMultiply(XMLoad(), XMLoadFloat4x4(&matrix));
			*this = matResult;

			return *this;
		}

		XMFLOAT4X4_DERIVED& Inverse()
		{
			return *this = XMMatrixInverse(nullptr, XMLoad());
		}

		XMFLOAT4X4_DERIVED& Identity()
		{
			return *this = XMMatrixIdentity();
		}

		XMFLOAT4X4_DERIVED& Transpose()
		{
			return *this = XMMatrixTranspose(XMLoad());
		}


	}_float4x4;


	typedef struct XMFLOAT4_DERIVED : public XMFLOAT4
	{
		XMFLOAT4_DERIVED()
			: XMFLOAT4()
		{}

		XMFLOAT4_DERIVED(const _float& _x,
			const _float& _y,
			const _float& _z,
			const _float& _w = 1.f)
			: XMFLOAT4(_x, _y, _z, _w)
		{}

		XMFLOAT4_DERIVED& operator = (const XMFLOAT4& _other)
		{
			XMVECTOR	vector = XMLoadFloat4(&_other);
			*this = vector;

			return *this;
		}

		XMFLOAT4_DERIVED& operator = (FXMVECTOR _vector)
		{
			XMStoreFloat4(this, _vector);
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

			vOutput = XMLoad() + XMLoadFloat4(&_other);

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

			vOutput = XMLoad() - XMLoadFloat4(&_other);

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

			vOutput = XMLoad() * XMLoadFloat4(&_other);


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

			vOutput = XMLoad() / XMLoadFloat4(&_other);


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
			*this = XMLoad() + XMLoadFloat4(&_other);

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
			*this = XMLoad() - XMLoadFloat4(&_other);

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
			*this = XMLoad() * XMLoadFloat4(&_other);

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
			*this = XMLoad() / XMLoadFloat4(&_other);

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
		XMFLOAT4_DERIVED operator * (const XMFLOAT4X4& _otherMatrix)
		{
			FXMVECTOR	vec = XMLoad();

			XMVECTOR	vTransform = XMVector4Transform(vec, XMLoadFloat4x4(&_otherMatrix));

			XMFLOAT4_DERIVED	vResult;

			XMStoreFloat4(&vResult, vTransform);

			return vResult;
		}

		XMFLOAT4_DERIVED& operator *= (const XMFLOAT4X4& _otherMatrix)
		{
			XMVECTOR	vec = XMLoad();

			XMVECTOR	vTransform = XMVector4Transform(vec, XMLoadFloat4x4(&_otherMatrix));

			XMFLOAT4_DERIVED	vResult;

			XMStoreFloat4(&vResult, vTransform);

			return *this = vResult;
		}


		_float Length()
		{
			FXMVECTOR	vec = XMLoad();

			return XMVector4Length(vec).m128_f32[0];
		}

		_float	Dot(const XMFLOAT4& _other)
		{
			FXMVECTOR	Myvec = XMLoad();
			FXMVECTOR	Othervec = XMLoadFloat4(&_other);

			return XMVector4Dot(Myvec, Othervec).m128_f32[0];
		}

		XMFLOAT4_DERIVED	Cross(const XMFLOAT4& _other)
		{
			FXMVECTOR	Myvec = XMLoad();
			FXMVECTOR	Othervec = XMLoadFloat4(&_other);
			FXMVECTOR	vCross = XMVector3Cross(Myvec, Othervec);

			XMFLOAT4_DERIVED	vResult;

			XMStoreFloat4(&vResult, vCross);

			return vResult;
		}

		XMFLOAT4_DERIVED& Normalize()
		{
			FXMVECTOR	Myvec = XMLoad();

			FXMVECTOR	vNormalize = XMVector4Normalize(Myvec);

			XMFLOAT4_DERIVED	vResult;

			XMStoreFloat4(&vResult, vNormalize);

			return *this = vResult;
		}

		XMVECTOR	XMLoad()
		{
			return XMLoadFloat4(this);
		}

		_bool		Is_Zero()
		{
			if (x == 0.f &&
				y == 0.f &&
				z == 0.f)
			return true;

			return false;
		}

	}_float4;
}
