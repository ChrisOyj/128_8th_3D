#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CModel;
class CAnimator;
END

BEGIN(Client)
class CCustomizingPreview
	: public CGameObject
{
	DECLARE_PROTOTYPE(CCustomizingPreview);

private:
	CCustomizingPreview();
	virtual ~CCustomizingPreview();

public:
	static CCustomizingPreview* Create(wstring wstrFilePath);

public:
	HRESULT	Add_ModelParts(wstring wstrFilePath, MODEL_PART_TYPE eType);
	HRESULT	Add_ModelParts(wstring wstrFilePath, MODEL_PART_TYPE eType, string strRefBone);
	UNIT_MODEL_DATA&	Get_ModelData() { return m_tPlayerModelData; }
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	CModel* m_pModelCom = nullptr;
	CAnimator* m_pAnimator = nullptr;
	vector<_uint>		m_vecVaildAnimIndices;
	_uint				m_iCurVaildAnimIndex = 0;
	UNIT_MODEL_DATA		m_tPlayerModelData;
	_bool				m_bOverall = false;

private:
	HRESULT		SetUp_Model(wstring wstrFilePath);

private:


private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

};
END
