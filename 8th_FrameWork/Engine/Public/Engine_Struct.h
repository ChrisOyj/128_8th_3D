#pragma once

namespace Engine
{
#pragma region TRANSFORM
	typedef struct tag_Transform
	{
		_float3		vScale;
		_float4x4		matMyWorld;
		_float4x4		matWorld;

	}TRANSFORM;

	typedef struct tag_View_Transform
	{
		_float3		vEye;
		_float3		vLookAt;
		_float3		vUp;

		_float4x4		matView; // ºä Çà·Ä 

	}VIEW_TRANSFORM;

	typedef struct tag_Proj_Transform
	{
		_float		fFOV;
		_float		fNear;
		_float		fFar;

		_float4x4		matProj;

	}PROJ_TRANSFORM;

#pragma endregion 

#pragma region VERTEX
	typedef struct tag_Vertex_Texture
	{
		_float3	vPosition;
		_float2	vTexUV;

	}VERTEX_TEX;


	//typedef		struct tag_Vertex_Normal
	//{
	//	_float3 vPosition;
	//	_float3 vNormal;

	//	void Set_Normal(const tag_Vertex_Normal& Adj1, const tag_Vertex_Normal& Adj2)
	//	{
	//		_float3 AdjVec1(Adj1.vPosition), AdjVec2(Adj2.vPosition);
	//		_float3 u, v;

	//		u = AdjVec1 - vPosition;
	//		v = AdjVec2 - vPosition;

	//		u.Cross(v);
	//		u.Normalize();

	//		vNormal = u;
	//	}

	//	void Set_Normal(const tag_Vertex_Normal& VertexNormal)
	//	{
	//		vNormal = VertexNormal.vNormal;
	//	}

	//	tag_Vertex_Normal() = default;

	//}VERTEX_NORMAL;

	//typedef struct tag_Vertex_Normal_Tex
	//{
	//	_float3 vPosition;
	//	_float3 vNormal;
	//	_float2	vTexUV;

	//	tag_Vertex_Normal_Tex()
	//	{}

	//	tag_Vertex_Normal_Tex(const _float& _x, const _float& _y, const _float& _z, const _float& _u, const _float& _v)
	//		: vPosition(_x, _y, _z)
	//		, vTexUV(_u, _v)
	//	{
	//	}

	//	void Set_Normal(const tag_Vertex_Normal_Tex& Adj1, const tag_Vertex_Normal_Tex& Adj2)
	//	{
	//		_float3 AdjVec1(Adj1.vPosition), AdjVec2(Adj2.vPosition);
	//		_float3 u, v;

	//		u = AdjVec1 - vPosition;
	//		v = AdjVec2 - vPosition;

	//		u.Cross(v);
	//		u.Normalize();

	//		vNormal = u;
	//	}
	//}VERTEX_TEX_NORMAL;

	//typedef struct tag_Vertex_CubeMap
	//{
	//	_float3 vPosition;
	//	_float3 vNormal;
	//	_float3	vTexUV;

	//	tag_Vertex_CubeMap(const _float3& vPos)
	//		: vPosition(vPos)
	//		, vTexUV(vPos)
	//	{
	//		//vTexUV.Normalize();
	//	}

	//	void Set_Normal(const tag_Vertex_CubeMap& Adj1, const tag_Vertex_CubeMap& Adj2)
	//	{
	//		_float3 AdjVec1(Adj1.vPosition), AdjVec2(Adj2.vPosition);
	//		_float3 u, v;

	//		u = AdjVec1 - vPosition;
	//		v = AdjVec2 - vPosition;

	//		u.Cross(v);
	//		u.Normalize();

	//		vNormal = u;
	//	}
	//}VERTEX_CUBEMAP;

#pragma endregion

	typedef struct tagGraphicDesc
	{
		enum WINMODE { MODE_FULL, MODE_WIN, MODE_END };

		unsigned int iWinCX, iWinCY;
		HWND	hWnd;
		WINMODE	isWindowMode;
	}GRAPHICDESC;


	struct _uint3
	{
		_uint3(_uint _a, _uint _b, _uint _c)
			: _1(_a)
			, _2(_b)
			, _3(_c)
		{}
		_uint _1, _2, _3;
	};

	


}
