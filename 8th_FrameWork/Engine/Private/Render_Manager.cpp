#include "..\Public\Render_Manager.h"

#include "GameObject.h"
#include "Transform.h"
#include "GameInstance.h"
#include "CModel_Renderer.h"

#include "Camera_Manager.h"

#include "CTarget_Manager.h"
#include "CLight_Manager.h"
#include "CRenderTarget.h"
#include "CLight.h"

#include "CShader.h"
#include "CMesh_Rect.h"
#include "Texture.h"

IMPLEMENT_SINGLETON(CRender_Manager)

#define	SHADER_DEFERRED	0
#define	SHADER_BLUR	1
#define	SHADER_GLOW	2
#define	SHADER_SHADOW	3


CRender_Manager::CRender_Manager()
{
}

CRender_Manager::~CRender_Manager()
{
	Release();
}

HRESULT CRender_Manager::Add_Renderer(RENDER_GROUP eGroup, CRenderer* pRenderer)
{
	if (eGroup >= RENDER_END ||
		nullptr == pRenderer)
		return E_FAIL;

	if (eGroup == RENDER_ALPHA)
	{
		m_pAlphaRenderers.push_back(make_pair(0.f, pRenderer));
		return S_OK;
	}

	m_Renderers[eGroup].push_back(pRenderer);

	return S_OK;
}

CShader* CRender_Manager::Get_DeferredShader()
{
	return m_vecShader[SHADER_DEFERRED];
}

void CRender_Manager::Bake_StaticShadow(vector<CGameObject*>& vecObjs, _float fDistance)
{
	m_ShadowViewMatrix.Identity();
	_float4 vLook = _float4(-1.f, -2.f, 1.f, 0.f).Normalize();

	*((_float4*)&m_ShadowViewMatrix.m[2]) = vLook;

	_float4 vUp = { 0.f, 1.f, 0.f, 0.f };
	_float4 vRight = vUp.Cross(vLook);
	*((_float4*)&m_ShadowViewMatrix.m[0]) = vRight;
	vUp = vLook.Cross(vRight);
	*((_float4*)&m_ShadowViewMatrix.m[1]) = vUp;

	_float4 vPos = vLook * -1.f * fDistance;
	vPos.w = 1.f;

	*((_float4*)&m_ShadowViewMatrix.m[3]) = vPos;
	m_ShadowViewMatrix.Inverse();
	m_ShadowViewMatrix.Transpose();




	m_pTarget_Manager->Begin_Shadow(TEXT("MRT_StaticShadowAcc"));

	//조명 위치에서 뷰,투영 매트릭스
	CShader_Manager::Get_Instance()->Set_RawValue_All("g_ProjMatrix", &m_ShadowProjMatrix, sizeof(_float4x4));


	for (auto& elem : vecObjs)
	{
		GET_COMPONENT_FROM(elem, CRenderer)->Bake_Shadow(m_ShadowViewMatrix);
	}


	m_pTarget_Manager->End_Shadow();

}

HRESULT CRender_Manager::Initialize()
{
	m_pTarget_Manager = CTarget_Manager::Get_Instance();
	m_pLight_Manager = CLight_Manager::Get_Instance();

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	_uint		iNumViewports = 1;

	DEVICE_CONTEXT->RSGetViewports(&iNumViewports, &ViewPortDesc);


#pragma region SHADOW DSV
	ComPtr < ID3D11Texture2D> pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = 8000;
	//TextureDesc.Width = 1280;
	_float fRatio = 8000.f / 1280.f;
	TextureDesc.Height = (_uint)(ViewPortDesc.Height * fRatio);
	//TextureDesc.Height = 720;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;


	if (FAILED(DEVICE->CreateTexture2D(&TextureDesc, nullptr, pDepthStencilTexture.GetAddressOf())))
		return E_FAIL;

	if (FAILED(DEVICE->CreateDepthStencilView(pDepthStencilTexture.Get(), nullptr, m_pShadowDSV.GetAddressOf())))
		return E_FAIL;



	ZeroMemory(&m_ShadowViewPort, sizeof(D3D11_VIEWPORT));
	m_ShadowViewPort.TopLeftX = 0;
	m_ShadowViewPort.TopLeftY = 0;
	m_ShadowViewPort.Width = (_float)TextureDesc.Width;
	m_ShadowViewPort.Height = (_float)TextureDesc.Height;
	m_ShadowViewPort.MinDepth = 0.f;
	m_ShadowViewPort.MaxDepth = 1.f;

	m_pTarget_Manager->Set_ShadowDSV(m_pShadowDSV, m_ShadowViewPort);



	m_ShadowViewMatrix.Identity();
	_float4 vLook = _float4(-1.f, -2.f, 1.f, 0.f).Normalize();

	*((_float4*)&m_ShadowViewMatrix.m[2]) = vLook;

	_float4 vUp = { 0.f, 1.f, 0.f, 0.f };
	_float4 vRight = vUp.Cross(vLook);
	*((_float4*)&m_ShadowViewMatrix.m[0]) = vRight;
	vUp = vLook.Cross(vRight);
	*((_float4*)&m_ShadowViewMatrix.m[1]) = vUp;


	*((_float4*)&m_ShadowViewMatrix.m[3]) = _float4(200.f, 400.f, -200.f, 1.f);
	m_ShadowViewMatrix.Inverse();
	m_ShadowViewMatrix.Transpose();

	m_ShadowProjMatrix.Identity();
	m_ShadowProjMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.f),
		1280.f / 720.f,
		0.2f, 1500.f);
	m_ShadowProjMatrix.Transpose();



#pragma endregion

	/* 렌더타겟들을 추가한다. */

#pragma region RENDERTARGETS

	/* For.Target_Diffuse */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Diffuse"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.027f, 0.1215f, 0.3254f, 0.f))))
		return E_FAIL;

	/* For.Target_Diffuse */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_SkyBox"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.5f, 0.9f, 1.f, 0.f))))
		return E_FAIL;

	/* For.Target_Normal*/
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Normal"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 1.f, 0.f, 1.f))))
		return E_FAIL;

	/* For.Target_Depth */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Depth"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 1.f, 0.f, 1.f))))
		return E_FAIL;

	/* For.Target_Shade */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Shade"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/* For.Target_Specular */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Specular"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_Flag */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Flag"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_Shadow */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Shadow"), TextureDesc.Width, TextureDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_StaticShadow"), TextureDesc.Width, TextureDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_ShadowBlur"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;


	/* For.Target_Forward */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Forward"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;


	/* For.Target_EffectDiffuse */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_EffectDiffuse"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	/*Glow*/
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Glowable"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Glow"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_GlowFlag"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	/* For.Target_EffectFlag */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_EffectFlag"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	/* For.Target_Depth */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_EffectDepth"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 1.f, 0.f, 1.f))))
		return E_FAIL;


	/* For.Blur_Flag */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_DownScale"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_HorizonBlur"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_VerticalBlur"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Outline_Flag */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Outline"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Bloom */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Bloom"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_BloomBlur"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.FinalBlend */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_FinalBlend"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.EffectBlur */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_EffectBlur"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Distortion */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Distortion"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Distortion */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_SSD"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* SkyBox */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SkyBox"), TEXT("Target_SkyBox"))))
		return E_FAIL;

	/* For.MRT_Deferred */
	/* 조명연산이 필요한 객체들을 그릴 때, 추후 빛연산에 필요한 정보를 저장받기위한 렌더타겟들 (Diffuse + Normal ) */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Depth"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Flag"))))
		return E_FAIL;

	/* For.MRT_Effect */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_Forward"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_Depth"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_EffectFlag"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_EffectDiffuse"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_GlowFlag"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Distortion"), TEXT("Target_Distortion"))))
		return E_FAIL;

	/* For.MRT_LightAcc */
	/* 조명연산한 결과를 저장해놓기위한 타겟 (Shade + Specular) */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		return E_FAIL;

	/*For Blur*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_DownScaleAcc"), TEXT("Target_DownScale"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_VerticalBlurAcc"), TEXT("Target_VerticalBlur"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_HorizonBlurAcc"), TEXT("Target_HorizonBlur"))))
		return E_FAIL;

	/*For Glow*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GlowableAcc"), TEXT("Target_Glowable"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GlowAcc"), TEXT("Target_Glow"))))
		return E_FAIL;

	/*For Outline*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_OutlineAcc"), TEXT("Target_Outline"))))
		return E_FAIL;

	/*For Shadow*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_ShadowAcc"), TEXT("Target_Shadow"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_StaticShadowAcc"), TEXT("Target_StaticShadow"))))
		return E_FAIL;

	/*For Forward*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_ForwardAcc"), TEXT("Target_Forward"))))
		return E_FAIL;

	/*For Bloom*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BloomAcc"), TEXT("Target_Bloom"))))
		return E_FAIL;

	/*For Bloom*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BloomBlurAcc"), TEXT("Target_BloomBlur"))))
		return E_FAIL;

	/*For Distortion*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_FinalBlendAcc"), TEXT("Target_FinalBlend"))))
		return E_FAIL;

	/*For EffectBlur*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_EffectBlurAcc"), TEXT("Target_EffectBlur"))))
		return E_FAIL;

	/*For Decal*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSDAcc"), TEXT("Target_Forward"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSDAcc"), TEXT("Target_EffectDiffuse"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSDAcc"), TEXT("Target_EffectFlag"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSDAcc"), TEXT("Target_GlowFlag"))))
		return E_FAIL;
	
	/*SHadow Blur*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_ShadowBlurAcc"), TEXT("Target_ShadowBlur"))))
		return E_FAIL;

	/*if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSDAcc"), TEXT("Target_EffectDiffuse"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSDAcc"), TEXT("Target_EffectFlag"))))
		return E_FAIL;*/

#ifdef _DEBUG	
	_float fSize = 100.f;
	_float fStep = fSize * 0.5f;

	_float fPosX = fStep;
	_float fPosY = fStep;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Diffuse"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Normal"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Depth"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Flag"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;

	fPosX += fSize;
	fPosY = fStep;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Shade"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Specular"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;

	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_StaticShadow"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;

	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Shadow"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;

	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_ShadowBlur"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	

	/*Forward*/
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Forward"), 75.f, 645.f, 150.f, 150.f)))
		return E_FAIL;

	fPosX = 1280.f - fStep;
	fPosY = fStep;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_VerticalBlur"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Outline"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Distortion"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;

	fPosX -= fSize;
	fPosY = fStep;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_EffectFlag"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	fPosY += fSize;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Glow"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	fPosY += fSize;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_BloomBlur"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	fPosY += fSize;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_SSD"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	
#endif // _DEBUG

	
#pragma endregion



	_matrix			WorldMatrix = XMMatrixIdentity();
	WorldMatrix.r[0] = XMVectorSet(ViewPortDesc.Width, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, ViewPortDesc.Height, 0.f, 0.f);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(WorldMatrix));

	WorldMatrix = XMMatrixIdentity();
	WorldMatrix.r[0] = XMVectorSet(ViewPortDesc.Width * 0.5f, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, ViewPortDesc.Height * 0.5f, 0.f, 0.f);

	XMStoreFloat4x4(&m_DownScaleWorldMatrix, XMMatrixTranspose(WorldMatrix));

	WorldMatrix = XMMatrixIdentity();
	WorldMatrix.r[0] = XMVectorSet(ViewPortDesc.Width * 0.25f, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, ViewPortDesc.Height * 0.25f, 0.f, 0.f);

	XMStoreFloat4x4(&m_SuperDownScaleWorldMatrix, XMMatrixTranspose(WorldMatrix));

	WorldMatrix = XMMatrixIdentity();
	WorldMatrix.r[0] = XMVectorSet(ViewPortDesc.Width * 2.f, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, ViewPortDesc.Height * 2.f, 0.f, 0.f);

	XMStoreFloat4x4(&m_UpScaleWorldMatrix, XMMatrixTranspose(WorldMatrix));

	WorldMatrix = XMMatrixIdentity();
	WorldMatrix.r[0] = XMVectorSet(ViewPortDesc.Width * 4.f, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, ViewPortDesc.Height * 4.f, 0.f, 0.f);

	XMStoreFloat4x4(&m_SuperUpScaleWorldMatrix, XMMatrixTranspose(WorldMatrix));


	WorldMatrix = XMMatrixIdentity();
	WorldMatrix.r[0] = XMVectorSet(TextureDesc.Width, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, TextureDesc.Height, 0.f, 0.f);

	XMStoreFloat4x4(&m_ShadowWorldMatrix, XMMatrixTranspose(WorldMatrix));



	if (FAILED(CShader_Manager::Get_Instance()->Load_EffectFile(TEXT("../Bin/Shaderfiles/Shader_Deferred.hlsl"))))
		return E_FAIL;

	if (FAILED(CShader_Manager::Get_Instance()->Load_EffectFile(TEXT("../Bin/Shaderfiles/Shader_Blur.hlsl"))))
		return E_FAIL;

	if (FAILED(CShader_Manager::Get_Instance()->Load_EffectFile(TEXT("../Bin/Shaderfiles/Shader_Glow.hlsl"))))
		return E_FAIL;

	if (FAILED(CShader_Manager::Get_Instance()->Load_EffectFile(TEXT("../Bin/Shaderfiles/Shader_Shadow.hlsl"))))
		return E_FAIL;

	m_vecShader.push_back( CShader::Create(0,0, VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));
	if (nullptr == m_vecShader[SHADER_DEFERRED])
		return E_FAIL;

	m_vecShader.push_back(CShader::Create(0, SHADER_BLUR, VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));
	if (nullptr == m_vecShader[SHADER_BLUR])
		return E_FAIL;

	m_vecShader.push_back(CShader::Create(0, SHADER_GLOW, VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));
	if (nullptr == m_vecShader[SHADER_GLOW])
		return E_FAIL;

	m_vecShader.push_back(CShader::Create(0, SHADER_SHADOW, VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));
	if (nullptr == m_vecShader[SHADER_SHADOW])
		return E_FAIL;

	m_pMeshRect = CMesh_Rect::Create(0);
	if (nullptr == m_pMeshRect)
		return E_FAIL;

	m_pBlackTexture = CTexture::Create(0, L"../bin/resources/textures/black.png", 1);
	if (nullptr == m_pBlackTexture)
		return E_FAIL;

	return S_OK;
}

void CRender_Manager::Update()
{
	SetUp_SRV();
}

HRESULT CRender_Manager::Render()
{
	if (FAILED(CCamera_Manager::Get_Instance()->SetUp_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_SkyBox"))))
		return E_FAIL;

	if (FAILED(Render_Group(RENDER_PRIORITY)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	/* Shadow Baking */
	if (FAILED(Bake_Shadow()))
		return E_FAIL;

	/* Deferred MRT */
	if (FAILED(CCamera_Manager::Get_Instance()->SetUp_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_Deferred"))))
		return E_FAIL;

	if (FAILED(Render_Group(RENDER_NONALPHA)))
		return E_FAIL;

	

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	
	//Ortho

	if (FAILED(CCamera_Manager::Get_Instance()->SetUp_ShaderResources(true)))
		return E_FAIL;


	if (FAILED(Render_Lights()))
		return E_FAIL;

	if (FAILED(Render_ShadowBlur()))
		return E_FAIL;
	
	//Lights, Shadow, DOF
	if (FAILED(Render_ForwardBlend()))
		return E_FAIL;

	//Blur For Outline
	if (FAILED(Render_Blur()))
		return E_FAIL;

	if (FAILED(Render_Outline()))
		return E_FAIL;


	if (FAILED(CCamera_Manager::Get_Instance()->SetUp_ShaderResources()))
		return E_FAIL;

	//포워드에다가 전부 합친 후 거따 이펙트를 그린다

	if (FAILED(Render_SSD()))
		return E_FAIL;

	if (FAILED(Render_AlphaGroup()))
		return E_FAIL;

	

	if (FAILED(Render_Distortion()))
		return E_FAIL;

	

	if (FAILED(CCamera_Manager::Get_Instance()->SetUp_ShaderResources(true)))
		return E_FAIL;

	if (FAILED(Render_Glow()))
		return E_FAIL;



	if (FAILED(Render_Bloom()))
		return E_FAIL;

	//Blur
	if (FAILED(Render_EffectBlur()))
		return E_FAIL;

	if (FAILED(Render_FinalBlend()))
		return E_FAIL;

	if (FAILED(Render_PostEffect()))
		return E_FAIL;

	Sort_UIGroup();

	if (FAILED(Render_Group(RENDER_UI)))
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(Render_Debug()))
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

void CRender_Manager::Release()
{
	for (auto& elem : m_vecShader)
		SAFE_DELETE(elem);
	SAFE_DELETE(m_pMeshRect);
	SAFE_DELETE(m_pBlackTexture);

	m_pShadowDSV.Reset();

}
#ifdef _DEBUG


HRESULT CRender_Manager::Render_Debug()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (!m_bDebugRender)
		return S_OK;

	m_pTarget_Manager->Render_Debug(TEXT("MRT_Deferred"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_LightAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_OutlineAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_VerticalBlurAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_ShadowAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_ForwardAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_GlowAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_BloomBlurAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_Distortion"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_StaticShadowAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_ShadowBlurAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	//m_pTarget_Manager->Render_Debug(TEXT("MRT_SSDAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);



	return S_OK;
}
void CRender_Manager::Tick_Debug()
{
	if (KEY(F7, TAP))
	{
		m_bDebugRender = !m_bDebugRender;
	}
}
#endif // _DEBUG

HRESULT CRender_Manager::Render_Lights()
{
	/* 셰이드 타겟을 장치에 바인드한다. */
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_LightAcc"))))
		return E_FAIL;

	/* 모든 빛은 이 노멀텍스쳐(타겟)과 연산이 이뤄지면 되기때문에.
	모든 빛마다 각각 던질피룡가 없다. */
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_NormalTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Normal")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_DepthTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Depth")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_FlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Flag")))))
		return E_FAIL;

	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));


	_float4x4		ViewMatrixInv, ProjMatrixInv;

	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(GAMEINSTANCE->Get_CurViewMatrix().Inverse().XMLoad()));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(GAMEINSTANCE->Get_CurProjMatrix().Inverse().XMLoad()));

	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4));
	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4));

	_float4 vCamPos = GAMEINSTANCE->Get_ViewPos();
	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_vCamPosition", &vCamPos, sizeof(_float4));

	m_pLight_Manager->Render_Lights(m_vecShader[SHADER_DEFERRED], m_pMeshRect);

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}
HRESULT CRender_Manager::Render_ShadowBlur()
{
	//그림자 추출 + 그리기
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_DownScaleAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_SHADOW]->Set_ShaderResourceView("g_ShadowTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Shadow")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_SHADOW]->Set_ShaderResourceView("g_DepthTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Depth")))))
		return E_FAIL;



	_float4x4		ViewMatrixInv, ProjMatrixInv;
	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(GAMEINSTANCE->Get_CurViewMatrix().Inverse().XMLoad()));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(GAMEINSTANCE->Get_CurProjMatrix().Inverse().XMLoad()));
	m_vecShader[SHADER_SHADOW]->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4));
	m_vecShader[SHADER_SHADOW]->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4));
	_float4 vCamPos = GAMEINSTANCE->Get_ViewPos();
	m_vecShader[SHADER_SHADOW]->Set_RawValue("g_vCamPosition", &vCamPos, sizeof(_float4));
	m_vecShader[SHADER_SHADOW]->Set_RawValue("g_LightViewMatrix", &m_ShadowViewMatrix, sizeof(_float4x4));
	m_vecShader[SHADER_SHADOW]->Set_RawValue("g_LightVeiwProjMatrix", &m_ShadowProjMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_SHADOW]->Begin(1);
	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	//블러
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_HorizonBlurAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_DownScale")))))
		return E_FAIL;

	/* 모든 빛들은 셰이드 타겟을 꽉 채우고 지굑투영으로 그려지면 되기때문에 빛마다 다른 상태를 줄 필요가 없다. */
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_BLUR]->Begin(2);

	m_pMeshRect->Render();

	//3. 수직
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_ShadowBlurAcc"))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_HorizonBlur")))))
		return E_FAIL;
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_BLUR]->Begin(1);

	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	_uint iNumBlur = 1;
	
	for (_uint i = 0; i < iNumBlur; ++i)
	{
		//블러
		if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_HorizonBlurAcc"))))
			return E_FAIL;

		if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_ShadowBlur")))))
			return E_FAIL;

		/* 모든 빛들은 셰이드 타겟을 꽉 채우고 지굑투영으로 그려지면 되기때문에 빛마다 다른 상태를 줄 필요가 없다. */
		m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

		m_vecShader[SHADER_BLUR]->Begin(2);

		m_pMeshRect->Render();

		//3. 수직
		if (FAILED(m_pTarget_Manager->End_MRT()))
			return E_FAIL;


		if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_ShadowBlurAcc"))))
			return E_FAIL;
		if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_HorizonBlur")))))
			return E_FAIL;
		m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

		m_vecShader[SHADER_BLUR]->Begin(1);

		m_pMeshRect->Render();

		if (FAILED(m_pTarget_Manager->End_MRT()))
			return E_FAIL;

	}

	

	




	return S_OK;
}
HRESULT CRender_Manager::Render_AlphaGroup()
{
	Sort_AlphaList();

	//0번 렌더타겟에 Forward 두기
	if (FAILED(m_pTarget_Manager->Begin_Effects(TEXT("MRT_Effect"))))
		return E_FAIL;


	for (auto& elem : m_pAlphaRenderers)
	{
		if (FAILED(elem.second->Render()))
			return E_FAIL;
	}

	m_pAlphaRenderers.clear();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	return S_OK;
}

HRESULT CRender_Manager::Render_Distortion()
{
	//0번 렌더타겟에 Forward 두기
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_Distortion"))))
		return E_FAIL;

	for (auto& elem : m_Renderers[RENDER_DISTORTION])
	{
		if (FAILED(elem->Render()))
			return E_FAIL;
	}

	m_Renderers[RENDER_DISTORTION].clear();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	return S_OK;
}


HRESULT CRender_Manager::Render_ForwardBlend()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_ForwardAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_SkyTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_SkyBox")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_FlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Flag")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_DiffuseTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Diffuse")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_ShadeTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Shade")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_SpecularTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Specular")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_ShadowTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_ShadowBlur")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_BlurTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_VerticalBlur")))))
		return E_FAIL;

	/* 모든 빛들은 셰이드 타겟을 꽉 채우고 지굑투영으로 그려지면 되기때문에 빛마다 다른 상태를 줄 필요가 없다. */
	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_DEFERRED]->Begin(3);

	/* 사각형 버퍼를 백버퍼위에 그려낸다. */
	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRender_Manager::Render_Outline()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_OutlineAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_FlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Flag")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_DiffuseTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_VerticalBlur")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_DepthTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Depth")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_NormalTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Normal")))))
		return E_FAIL;

	/* 모든 빛들은 셰이드 타겟을 꽉 채우고 지굑투영으로 그려지면 되기때문에 빛마다 다른 상태를 줄 필요가 없다. */
	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_DEFERRED]->Begin(4);

	/* 사각형 버퍼를 백버퍼위에 그려낸다. */
	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRender_Manager::SetUp_SRV()
{
	if (FAILED(m_pBlackTexture->Set_ShaderResourceView(m_vecShader[SHADER_DEFERRED], "g_FogTexture")))
		return E_FAIL;


	return S_OK;
}

HRESULT CRender_Manager::Bake_Shadow()
{
	if (FAILED(m_pTarget_Manager->Begin_Shadow(TEXT("MRT_ShadowAcc"))))
		return E_FAIL;


	//미리구워논거부터 넣고
	if (FAILED(m_vecShader[SHADER_SHADOW]->Set_ShaderResourceView("g_StaticShadowTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_StaticShadow")))))
		return E_FAIL;

	/*직교로 일단 넣기*/
	if (FAILED(CCamera_Manager::Get_Instance()->SetUp_ShaderResources(SHADER_SHADOW, true)))
		return E_FAIL;

	m_vecShader[SHADER_SHADOW]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_vecShader[SHADER_SHADOW]->Begin(0);
	m_pMeshRect->Render();
	





	//조명 위치에서 뷰,투영 매트릭스
	CShader_Manager::Get_Instance()->Set_RawValue_All("g_ProjMatrix", &m_ShadowProjMatrix, sizeof(_float4x4));


	//이미 그렸던 애면 패스는 나중에 생각
	for (auto& elem : m_Renderers[RENDER_SHADOW])
	{
		if (FAILED(elem->Get_ShaderCom()->Set_ShaderResourceView("g_StaticShadowTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_StaticShadow")))))
			return E_FAIL;

		if (FAILED(elem->Bake_Shadow(m_ShadowViewMatrix)))
			return E_FAIL;
	}

	m_Renderers[RENDER_SHADOW].clear();

	if (FAILED(m_pTarget_Manager->End_Shadow()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRender_Manager::Render_Blur()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_DownScaleAcc"))))
		return E_FAIL;

	//1. 다운 샘플링
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Diffuse")))))
		return E_FAIL;
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_vecShader[SHADER_BLUR]->Begin(0);
	m_pMeshRect->Render();

	//2. 수평블러
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_HorizonBlurAcc"))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_FlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Flag")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_DownScale")))))
		return E_FAIL;

	/* 모든 빛들은 셰이드 타겟을 꽉 채우고 지굑투영으로 그려지면 되기때문에 빛마다 다른 상태를 줄 필요가 없다. */
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_BLUR]->Begin(2);
	
	m_pMeshRect->Render();

	//3. 수직
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_VerticalBlurAcc"))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_HorizonBlur")))))
		return E_FAIL;
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_BLUR]->Begin(1);

	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRender_Manager::Render_Group(RENDER_GROUP eGroup)
{
	for (auto& elem : m_Renderers[eGroup])
	{
		if (FAILED(elem->Render()))
			return E_FAIL;
	}
		

	m_Renderers[eGroup].clear();

	return S_OK;
}

void CRender_Manager::Sort_AlphaList()
{
	_float4 vCamPos = CCamera_Manager::Get_Instance()->Get_ViewPos();

	for (auto& elem : m_pAlphaRenderers)
	{
		_float4 vMyPos = elem.second->Get_WorldPosition();
		_float fDistance = (vMyPos - vCamPos).Length();
		elem.first = fDistance;
	}

	m_pAlphaRenderers.sort(greater<pair<_float, CRenderer*>>());
}

void CRender_Manager::Sort_UIGroup()
{
	m_Renderers[RENDER_UI].sort([](CRenderer* p1, CRenderer* p2)
		{
			return p1->Get_FinalPos().z > p2->Get_FinalPos().z;
		});
}

HRESULT CRender_Manager::SetUp_Cascade(_uint iShadowMapWidth, _uint iShadowMapHeight, _uint iArrayCount)
{
	D3D11_TEXTURE2D_DESC texDesc = {
	iShadowMapWidth, iShadowMapHeight, 1, iArrayCount, DXGI_FORMAT_R32_TYPELESS, 1, 0, D3D11_USAGE_DEFAULT,
	D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE, 0, 0
	};

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {
		DXGI_FORMAT_D32_FLOAT, D3D11_DSV_DIMENSION_TEXTURE2DARRAY, 0 };
	dsvDesc.Texture2DArray.FirstArraySlice = 0;
	dsvDesc.Texture2DArray.ArraySize = iArrayCount;
	dsvDesc.Texture2DArray.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC	srvDesc = {
		DXGI_FORMAT_R32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2DARRAY, 0, 0 };
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.ArraySize = iArrayCount;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.MostDetailedMip = 0;

	
	ID3D11Texture2D* pTex = nullptr;

	if (FAILED(DEVICE->CreateTexture2D(&texDesc, NULL, &pTex)))
		return E_FAIL;

	if (FAILED(DEVICE->CreateDepthStencilView(pTex, &dsvDesc, &m_pCascadeDSV)))
		return E_FAIL;

	if (FAILED(DEVICE->CreateShaderResourceView(pTex, &srvDesc, &m_pCascadeSRV)))

	pTex->Release();

	return S_OK;
}

HRESULT CRender_Manager::Render_Glow()
{
	//1. 다운 샘플링 + Glowable 추출
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_DownScaleAcc"))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_GLOW]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_EffectDiffuse")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_GLOW]->Set_ShaderResourceView("g_FlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_EffectFlag")))))
		return E_FAIL;

	m_vecShader[SHADER_GLOW]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	//0. 글로우 할 대상 추출부터
	m_vecShader[SHADER_GLOW]->Begin(0);
	m_pMeshRect->Render();
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	//2. 수평블러
	
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_HorizonBlurAcc"))))
		return E_FAIL;
	

	if (FAILED(m_vecShader[SHADER_GLOW]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_DownScale")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_GLOW]->Set_ShaderResourceView("g_GlowFlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_GlowFlag")))))
		return E_FAIL;

	/* 모든 빛들은 셰이드 타겟을 꽉 채우고 지굑투영으로 그려지면 되기때문에 빛마다 다른 상태를 줄 필요가 없다. */
	m_vecShader[SHADER_GLOW]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	//첫 수평 블러
	m_vecShader[SHADER_GLOW]->Begin(3);

	m_pMeshRect->Render();

	//3. 수직
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_GlowAcc"))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_GLOW]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_HorizonBlur")))))
		return E_FAIL;
	m_vecShader[SHADER_GLOW]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_GLOW]->Begin(1);

	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	//한번 더

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_HorizonBlurAcc"))))
		return E_FAIL;


	if (FAILED(m_vecShader[SHADER_GLOW]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Glow")))))
		return E_FAIL;
	/* 모든 빛들은 셰이드 타겟을 꽉 채우고 지굑투영으로 그려지면 되기때문에 빛마다 다른 상태를 줄 필요가 없다. */
	m_vecShader[SHADER_GLOW]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_GLOW]->Begin(2);

	m_pMeshRect->Render();

	//3. 수직
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_GlowAcc"))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_GLOW]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_HorizonBlur")))))
		return E_FAIL;
	
	m_vecShader[SHADER_GLOW]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_GLOW]->Begin(1);

	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRender_Manager::Render_Bloom()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_BloomAcc"))))
		return E_FAIL;

	//1. Bloom 대상 찾기
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_EffectDiffuse")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_FlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_EffectFlag")))))
		return E_FAIL;
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_vecShader[SHADER_BLUR]->Begin(3);
	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	//2. 블러

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_DownScaleAcc"))))
		return E_FAIL;

	//1. 다운 샘플링
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Bloom")))))
		return E_FAIL;
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_DownScaleWorldMatrix, sizeof(_float4x4));
	m_vecShader[SHADER_BLUR]->Begin(0);
	m_pMeshRect->Render();

	//2. 수평블러
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_HorizonBlurAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_DownScale")))))
		return E_FAIL;

	/* 모든 빛들은 셰이드 타겟을 꽉 채우고 지굑투영으로 그려지면 되기때문에 빛마다 다른 상태를 줄 필요가 없다. */
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_BLUR]->Begin(2);

	m_pMeshRect->Render();

	//3. 수직
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_BloomBlurAcc"))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_HorizonBlur")))))
		return E_FAIL;
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_UpScaleWorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_BLUR]->Begin(1);

	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRender_Manager::Render_EffectBlur()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_DownScaleAcc"))))
		return E_FAIL;

	//1. 다운 샘플링
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Forward")))))
		return E_FAIL;
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_DownScaleWorldMatrix, sizeof(_float4x4));
	m_vecShader[SHADER_BLUR]->Begin(0);
	m_pMeshRect->Render();

	//2. 수평블러
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_HorizonBlurAcc"))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_FlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Flag")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_DownScale")))))
		return E_FAIL;

	/* 모든 빛들은 셰이드 타겟을 꽉 채우고 지굑투영으로 그려지면 되기때문에 빛마다 다른 상태를 줄 필요가 없다. */
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_BLUR]->Begin(2);

	m_pMeshRect->Render();

	//3. 수직
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_EffectBlurAcc"))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_HorizonBlur")))))
		return E_FAIL;
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_UpScaleWorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_BLUR]->Begin(1);

	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRender_Manager::Render_PostEffect()
{
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_Texture", m_pTarget_Manager->Get_SRV(TEXT("Target_FinalBlend")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_EffectFlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_EffectFlag")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_DepthTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Depth")))))
		return E_FAIL;

	//7. Distortion
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_DistortionTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Distortion")))))
		return E_FAIL;

	m_vecShader[SHADER_DEFERRED]->Begin(6);

	m_pMeshRect->Render();

	return S_OK;


}

HRESULT CRender_Manager::Render_SSD()
{
	if (FAILED(m_pTarget_Manager->Begin_SSD(TEXT("MRT_SSDAcc"))))
		return E_FAIL;

	//Depth 타겟 던져줘야함
	

	for (auto& elem : m_Renderers[RENDER_SSD])
	{
		if (FAILED(elem->Get_ShaderCom()->Set_ShaderResourceView("g_DepthTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Depth")))))
			return E_FAIL;

		if (FAILED(elem->Render()))
			return E_FAIL;
	}

	m_Renderers[RENDER_SSD].clear();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	return S_OK;
}

HRESULT CRender_Manager::Render_FinalBlend()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_FinalBlendAcc"))))
		return E_FAIL;

	//1. Forward
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_Texture", m_pTarget_Manager->Get_SRV(TEXT("Target_Forward")))))
		return E_FAIL;

	//2. EffectFlag
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_EffectFlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_EffectFlag")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_EffectDiffuseTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_EffectDiffuse")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_DepthTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Depth")))))
		return E_FAIL;

	//3. Outline
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_OutlineTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Outline")))))
		return E_FAIL;

	//4. Glow
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_GlowTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Glow")))))
		return E_FAIL;

	//5. Bloom
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_BloomTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_BloomBlur")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_BloomOriginTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Bloom")))))
		return E_FAIL;

	//6. Blur
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_EffectBlurTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_EffectBlur")))))
		return E_FAIL;

	

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Begin(5)))
		return E_FAIL;

	/* 사각형 버퍼를 백버퍼위에 그려낸다. */
	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	return S_OK;
}
