#include "stdafx.h"
#include "CUtility_Transform.h"
#include "Transform.h"

void CUtility_Transform::Turn_ByAngle(CTransform* pTransform, _float4 vAxis, _float fAngle)
{
	_float4 vLook = pTransform->Get_MyWorld(WORLD_LOOK);
	_float4 vRight = pTransform->Get_MyWorld(WORLD_RIGHT);
	_float4 vUp = pTransform->Get_MyWorld(WORLD_UP);

	_float4x4 matRot;

	matRot = XMMatrixRotationAxis(vAxis.XMLoad(), ToRadian(fAngle));

	vLook = vLook.MultiplyNormal(matRot);
	vRight = vRight.MultiplyNormal(matRot);
	vUp = vUp.MultiplyNormal(matRot);

	pTransform->Set_World(WORLD_LOOK, vLook.Normalize());
	pTransform->Set_World(WORLD_RIGHT, vRight.Normalize());
	pTransform->Set_World(WORLD_UP, vUp.Normalize());
}
