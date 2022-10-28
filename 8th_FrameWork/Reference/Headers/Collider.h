#pragma once
#include "Component.h"

BEGIN(Engine)

class CHierarchyNode;

enum COL_TYPE {CT_SPHERE, CT_BOX, CT_END};

class ENGINE_DLL CCollider abstract
	: public CComponent
{
protected:
	CCollider(_uint iGroupID);
	CCollider(const CCollider& origin);
	virtual ~CCollider();

public:
	COL_TYPE				Get_ColType() { return m_eColType; }
	_uint					Get_ColIndex()	const	{ return m_iColIndex; }
	const _uint&			Get_ID()		const	{ return m_iID; }

	void					Set_ColIndex(const _uint& iColIndex)		{ m_iColIndex = iColIndex; }
	void					Set_Col(_bool bCol);

	void					Set_RefBone(CHierarchyNode* pBone) { m_pRefBone = pBone; }

#ifdef _DEBUG
public:
	virtual HRESULT Render() { return S_OK; };
#endif // _DEBUG

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

protected:
	_int			m_iColCount = 0;
	COL_TYPE		m_eColType = CT_END;

	CHierarchyNode* m_pRefBone = nullptr;
	_float4x4		m_matTransformation;

	_bool			m_isCloned = false;
	_bool			m_bCol = false;


private:
	static _uint	g_iNextID;

	_uint			m_iID = 0;
	_uint			m_iColIndex = 0;

#ifdef _DEBUG
protected:
	BasicEffect* m_pEffect = nullptr;
	PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;
	ComPtr<ID3D11InputLayout> m_pInputLayout = nullptr;
#endif // _DEBUG


};

END