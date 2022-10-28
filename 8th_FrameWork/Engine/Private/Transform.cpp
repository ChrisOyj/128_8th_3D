#include "..\Public\Transform.h"
#include "GameObject.h"

#include "GameInstance.h"

#include "CShader.h"

CTransform::CTransform(_uint iGroupID)
	: CComponent(iGroupID)
{
	ZeroMemory(&m_tTransform, sizeof(TRANSFORM));
	
}


CTransform::~CTransform()
{
}

CTransform* CTransform::Create(_uint iGroupID)
{
	CTransform* pTransform = new CTransform(iGroupID);

	if (FAILED(pTransform->Initialize_Prototype()))
	{
		SAFE_DELETE(pTransform);
		Call_MsgBox(L"Failed to Initialize_Prototype : CTransform");
		return nullptr;
	}

	return pTransform;
}

_float4x4	CTransform::Get_WorldMatrix(const _byte& matrixFlag)
{
	_float4x4	WorldMat = m_tTransform.matWorld;

	if (matrixFlag & MATRIX_NOSCALE)
	{
		(*((_float4*)&WorldMat.m[WORLD_RIGHT])).Normalize();
		(*((_float4*)&WorldMat.m[WORLD_UP])).Normalize();
		(*((_float4*)&WorldMat.m[WORLD_LOOK])).Normalize();
	}

	if (matrixFlag & MATRIX_NOTURN)
	{
		(*((_float4*)&WorldMat.m[WORLD_RIGHT])) = _float4(1.f, 0.f, 0.f);
		(*((_float4*)&WorldMat.m[WORLD_UP])) = _float4(0.f, 1.f, 0.f);
		(*((_float4*)&WorldMat.m[WORLD_LOOK])) = _float4(0.f, 0.f, 1.f);
	}

	if (matrixFlag & MARTIX_NOTRANS)
	{
		(*((_float4*)&WorldMat.m[WORLD_POS])) = _float4(0.f, 0.f, 0.f);
	}

	if (matrixFlag & MATRIX_IDENTITY)
	{
		WorldMat.Identity();
	}

	return WorldMat;
}


_float4 CTransform::Get_World(WORLD eType)
{
	_float4 vResult = *((_float4*)&m_tTransform.matWorld.m[eType]);

	return vResult;
}

_float4 CTransform::Get_MyWorld(WORLD eType)
{
	_float4 vResult = *((_float4*)&m_tTransform.matMyWorld.m[eType]);

	return vResult;
}

void CTransform::Set_World(WORLD eType, const _float4 & vCol)
{
	*((_float4*)&m_tTransform.matMyWorld.m[eType]) = vCol;
}

void CTransform::Set_RealWorld(WORLD eType, const _float4 & vCol)
{
	*((_float4*)&m_tTransform.matWorld.m[eType]) = vCol;

}

void CTransform::Set_MyMatrix(_float4x4 matWorld)
{
	m_tTransform.matMyWorld = matWorld;
}

void CTransform::Set_Look(const _float4& vLook)
{
	_float4 _vLook = vLook;
	Set_World(WORLD_LOOK, _vLook.Normalize());

	_float4 vUp = { 0.f, 1.f, 0.f };
	if ((vLook.y < 1.1f && vLook.y > 0.9f) ||
		(vLook.y > -1.1f && vLook.y < -0.9f)
		)
		vUp = _float4(0.f, 0.f, 1.f, 0.f);

	vUp.Normalize();
	_float4 vRight = vUp.Cross(vLook);
	Set_World(WORLD_RIGHT, vRight.Normalize());
	
	vUp = _vLook.Cross(vRight);
	Set_World(WORLD_UP, vUp.Normalize());
}

void CTransform::Set_Right(const _float4& vRight)
{
	_float4 _vRight = vRight;
	Set_World(WORLD_RIGHT, _vRight.Normalize());

	_float4 vUp = { 0.f, 1.f, 0.f, 0.f };

	if (vRight.y < 1.1f && vRight.y > 0.9f)
		vUp = _float4(0.f, 0.f, 1.f, 0.f);
	else if (vRight.y > -1.1f && vRight.y < -0.9f)
		vUp = _float4(0.f, 0.f, 1.f, 0.f);

	vUp.Normalize();
	_float4 vLook = _vRight.Cross(vUp);
	Set_World(WORLD_LOOK, vLook.Normalize());

	vUp = vLook.Cross(_vRight);
	Set_World(WORLD_UP, vUp.Normalize());
}

void CTransform::Set_Up(const _float4& vUp)
{
	_float4 _vUp = vUp;
	Set_World(WORLD_UP, _vUp.Normalize());

	_float4 vLook = { 0.f, 0.f, 1.f, 0.f };

	if (vUp.z < 1.1f && vUp.z > 0.9f)
		vLook = _float4(0.f, 1.f, 1.f, 0.f);
	else if (vUp.z > -1.1f && vUp.z < -0.9f)
		vLook = _float4(0.f, 1.f, 1.f, 0.f);

	vLook.Normalize();
	_float4 vRight = _vUp.Cross(vLook);
	Set_World(WORLD_RIGHT, vRight.Normalize());

	vLook = vRight.Cross(_vUp);
	Set_World(WORLD_LOOK, vLook.Normalize());
}

void CTransform::Set_Rect()
{
	_float4	vRight;
	_float4 vUp;
	_float4	vLook;

	vRight = Get_World(WORLD_RIGHT);
	vUp =	Get_World(WORLD_UP);
	vLook = Get_World(WORLD_LOOK);

	Set_World(WORLD_RIGHT,	vRight  * -1);
	Set_World(WORLD_UP,		vUp);
	Set_World(WORLD_LOOK,	vLook * -1);
}

void	CTransform::Set_Scale(const _float4& vScale)
{
	m_tTransform.vScale = vScale;

	(*((_float4*)&m_tTransform.matMyWorld.m[WORLD_RIGHT])).Normalize() *= m_tTransform.vScale.x;
	(*((_float4*)&m_tTransform.matMyWorld.m[WORLD_UP])).Normalize() *= m_tTransform.vScale.y;
	(*((_float4*)&m_tTransform.matMyWorld.m[WORLD_LOOK])).Normalize() *= m_tTransform.vScale.z;
}


void CTransform::Set_Y(const _float& fY)
{
	(*((_float4*)&m_tTransform.matMyWorld.m[WORLD_POS])).y = fY;
}

void CTransform::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	_float4x4		WorldMatrixTP = m_tTransform.matWorld;
	WorldMatrixTP.Transpose();
	pShader->Set_RawValue(pConstantName, &WorldMatrixTP, sizeof(_float4x4));
}

void CTransform::OnCollisionEnter(CGameObject* pGameObject, const _uint& iColType, _float4 vColPoint)
{
}

void CTransform::Set_LerpLook(_float4 vLook, _float fMaxLerpTime)
{
	m_bLerp = true;
	m_vOriginLook = Get_World(WORLD_LOOK);
	m_vTargetLook = vLook.Normalize();

	m_fTimeAcc = 0.f;

	m_fLerpTime = fMaxLerpTime;

	_float fRatio;
	fRatio = m_vOriginLook.Dot(m_vTargetLook);
	fRatio *= -1.f;
	fRatio += 1.f;
	fRatio *= 0.5f;

	m_fLerpTime *= fRatio;

	//-1에서 1사이로 값이 나옴
	//-1일 때 1이 되어야함
	//1일 때 0이 되어야함


}


HRESULT CTransform::Initialize_Prototype()
{
	m_tTransform.vScale = _float4(1.f, 1.f, 1.f, 1.f);
	m_tTransform.matMyWorld.Identity();
	m_tTransform.matWorld.Identity();
	m_tTransform.matBonus.Identity();

	return S_OK;
}

HRESULT CTransform::Initialize()
{
	return S_OK;
}

void CTransform::Start()
{
	__super::Start();

	list<CComponent*>	pShdaerlist = m_pOwner->Get_Component<CShader>();
	if (pShdaerlist.empty())
		return;
	static_cast<CShader*>(pShdaerlist.front())->CallBack_SetRawValues +=
		bind(&CTransform::Set_ShaderResource, this, placeholders::_1, "g_WorldMatrix");

}

void CTransform::Tick()
{
	//m_pOwner->CallBack_CollisionEnter +=
		//bind(&CTransform::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);
}

void CTransform::Late_Tick()
{
	if (m_bLerp)
	{
		m_fTimeAcc += fDT;

		if (m_fTimeAcc > m_fLerpTime)
		{
			m_bLerp = false;
			Set_Look(m_vTargetLook);
		}
		else
		{
			_float fRatio = m_fTimeAcc / m_fLerpTime;
			_float4 vLook = XMVectorLerp(m_vOriginLook.XMLoad(), m_vTargetLook.XMLoad(), fRatio);
			Set_Look(vLook);
		}
	}
	


	Make_WorldMatrix();
}

void CTransform::Release()
{
}

void CTransform::OnEnable()
{
	__super::OnEnable();
}

void CTransform::OnDisable()
{
}

void CTransform::Make_WorldMatrix()
{
	Rescale_WorldMatrix();

	_float4x4	parentMat;
	XMStoreFloat4x4(&parentMat, XMMatrixIdentity());
	CGameObject* pParent = m_pOwner->Get_Parent();

	if (pParent)
		parentMat = pParent->Get_Transform()->Get_WorldMatrix(m_cParentFlag);

	m_tTransform.matWorld = m_tTransform.matMyWorld * parentMat * m_tTransform.matBonus;

}

void CTransform::Rescale_WorldMatrix()
{
	(*((_float4*)&m_tTransform.matMyWorld.m[WORLD_RIGHT])).Normalize() *= m_tTransform.vScale.x;
	(*((_float4*)&m_tTransform.matMyWorld.m[WORLD_UP])).Normalize() *= m_tTransform.vScale.y;
	(*((_float4*)&m_tTransform.matMyWorld.m[WORLD_LOOK])).Normalize() *= m_tTransform.vScale.z;
}
