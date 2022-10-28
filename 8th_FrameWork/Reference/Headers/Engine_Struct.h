#pragma once

namespace Engine
{
#pragma region TRANSFORM
	typedef struct tag_Transform
	{
		_float4		vScale;
		_float4x4		matMyWorld;
		_float4x4		matWorld;
		_float4x4		matBonus;

	}TRANSFORM;

	typedef struct tag_View_Transform
	{
		_float4		vEye;
		_float4		vLookAt;
		_float4		vUp;

		_float4x4		matView; // 뷰 행렬 

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
	typedef struct tagLineIndices32
	{
		unsigned long _1, _2;
	}LINEINDICES32;

	typedef struct tagLineIndices16
	{
		unsigned short _1, _2;
	}LINEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long _1, _2, _3;
	}FACEINDICES32;

	typedef struct tagFaceIndices16
	{
		unsigned short _1, _2, _3;
	}FACEINDICES16;

	typedef struct tagModelMaterial
	{
		class CTexture* pTextures[AI_TEXTURE_TYPE_MAX] = {};
		_float4			vColor;
		_uint			iType;
		string			strName;
	}MODEL_MATERIAL;

	typedef struct tagVertex_Normal_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT4		vColor;
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertex_Normal_Texture_Declaration
	{
		static const unsigned int		iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXNORTEX_DECLARATION;

	typedef struct tagVertex_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexUV;
	}VTXTEX;

	typedef struct ENGINE_DLL tagVertex_Texture_Declaration
	{
		static const unsigned int		iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXTEX_DECLARATION;

	typedef struct tagVertex_Point
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vPSize;
	}VTXPOINT;

	typedef struct tagVertex_Instance
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vTranslation;
	}VTXINSTANCE;

	typedef struct tagVertexRect_Instance
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vTranslation;
		XMFLOAT4			vColor;
	}VTXRECTINSTANCE;

	typedef struct tagVertex_Default
	{
		XMFLOAT3		vPosition;
	}VTXDEFAULT;

	typedef struct tagVertexTri_Instance
	{
		XMFLOAT3		vPosition[3];
		XMFLOAT4		vColor;
	}VTXTRIINSTANCE;

	typedef struct ENGINE_DLL tagVertex_Point_Instance_Declaration
	{
		static const unsigned int		iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	}VTXPOINT_INSTANCE_DECLARATION;

	typedef struct ENGINE_DLL tagVertex_Model_Instance_Declaration
	{
		static const unsigned int		iNumElements = 8;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXMODEL_INSTANCE_DECLARATION;

	typedef struct ENGINE_DLL tagVertex_Rect_Instance_Declaration
	{
		static const unsigned int		iNumElements = 7;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXRECT_INSTANCE_DECLARATION;

	typedef struct ENGINE_DLL tagVertex_Tri_Instance_Declaration
	{
		static const unsigned int		iNumElements = 5;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXTRI_INSTANCE_DECLARATION;


	typedef struct tagVertex_Model
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;
	}VTXMODEL;

	typedef struct ENGINE_DLL tagtagVertex_Model_Declaration
	{
		static const unsigned int		iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXMODEL_DECLARATION;

	typedef struct tagVertex_AnimModel
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;
		XMUINT4			vBlendIndex;
		XMFLOAT4		vBlendWeight;
	}VTXANIM;

	typedef struct ENGINE_DLL tagVertex_AnimModel_Declaration
	{
		static const unsigned int		iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXANIM_DECLARATION;

	typedef struct tagVertex_Cube_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexUV;
	}VTXCUBETEX;

	typedef struct tagVertex_DecalCube
	{
		tagVertex_DecalCube()
		{}

		tagVertex_DecalCube(_float f1, _float f2, _float f3, _float f4, _float f5)
			: vPosition(f1, f2, f3)
			, vTexUV(f1, f2)
		{}
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexUV;
	}VTXDECALCUBE;

	typedef struct ENGINE_DLL tagVertex_Cube_Texture_Declaration
	{
		static const unsigned int		iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXCUBETEX_DECLARATION;

	typedef struct ENGINE_DLL tagVertex_DecalCube_Declaration
	{
		static const unsigned int		iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXDECALCUBE_DECLARATION;

	//typedef		struct tag_Vertex_Normal
	//{
	//	_float4 vPosition;
	//	_float4 vNormal;

	//	void Set_Normal(const tag_Vertex_Normal& Adj1, const tag_Vertex_Normal& Adj2)
	//	{
	//		_float4 AdjVec1(Adj1.vPosition), AdjVec2(Adj2.vPosition);
	//		_float4 u, v;

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
	//	_float4 vPosition;
	//	_float4 vNormal;
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
	//		_float4 AdjVec1(Adj1.vPosition), AdjVec2(Adj2.vPosition);
	//		_float4 u, v;

	//		u = AdjVec1 - vPosition;
	//		v = AdjVec2 - vPosition;

	//		u.Cross(v);
	//		u.Normalize();

	//		vNormal = u;
	//	}
	//}VERTEX_TEX_NORMAL;

	//typedef struct tag_Vertex_CubeMap
	//{
	//	_float4 vPosition;
	//	_float4 vNormal;
	//	_float4	vTexUV;

	//	tag_Vertex_CubeMap(const _float4& vPos)
	//		: vPosition(vPos)
	//		, vTexUV(vPos)
	//	{
	//		//vTexUV.Normalize();
	//	}

	//	void Set_Normal(const tag_Vertex_CubeMap& Adj1, const tag_Vertex_CubeMap& Adj2)
	//	{
	//		_float4 AdjVec1(Adj1.vPosition), AdjVec2(Adj2.vPosition);
	//		_float4 u, v;

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
		HWND		hWnd;
		HINSTANCE	hInst;
		WINMODE		WinMode;
	}GRAPHICDESC;


	typedef struct tag_SoundDesc
	{
		unsigned int iChannelNumbers[CH_GROUP_END];

	}SOUNDDESC;

	struct _uint3
	{
		_uint3(_uint _a, _uint _b, _uint _c)
			: _1(_a)
			, _2(_b)
			, _3(_c)
		{}
		_uint _1, _2, _3;
	};

	typedef struct tagKeyFrame
	{
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vPosition;
		float			fTime;
	} KEYFRAME;

	struct BONE_DATA
	{
		char		szName[MIN_STR] = "";
		_float4x4	OffsetMatrix;
	};

	struct CHANNEL_DESC
	{
		string	strName;
		_uint	iNumKeyFrames = 0;
		KEYFRAME* pKeyFrames = nullptr;
	};

	struct ANIM_DESC
	{
		_uint	iNumChannels = 0;
		_float	fDuration = 0.f;
		_float	fTickPerSecond = 0.f;
		CHANNEL_DESC* pChannels = nullptr;
	};

	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END };

		TYPE			eType = TYPE_END;

		XMFLOAT4		vDirection;

		XMFLOAT4		vPosition;
		float			fRange;

		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;

		//라이트 수명
		float			fLightAcc = 0.f;
		float			fLightTime = 999999.f;
		class CGameObject* pOwner = nullptr;
		XMFLOAT4		vOffset = { 0.f, 0.f, 0.f, 1.f };

	}LIGHTDESC;
}
