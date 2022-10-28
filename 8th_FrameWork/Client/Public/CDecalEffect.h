#pragma once
#include "CEffect.h"

BEGIN(Engine)
class CHierarchyNode;
END

BEGIN(Client)

class CUnit;

class CDecalEffect
	: public CEffect
{
	DECLARE_PROTOTYPE(CDecalEffect);

private:
	CDecalEffect();
	virtual ~CDecalEffect();

public:
	static CDecalEffect* Create(wstring DecalImagePath, wstring ColorMapPath, wstring wstrCode);

public:
	virtual void	Set_ShaderResourceAlpha(CShader* pShader, const char* pConstantName);

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	list<CGameObject*>	m_EffectsList;

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;



};

END