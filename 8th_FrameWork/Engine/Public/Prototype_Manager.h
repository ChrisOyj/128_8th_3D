#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CGameObject;
class CComponent;

class CPrototype_Manager final
{
	DECLARE_SINGLETON(CPrototype_Manager);

private:
	CPrototype_Manager();
	virtual ~CPrototype_Manager();

public:
	HRESULT		Initialize();

public:
	CGameObject* Clone_GameObject(_hashcode	hcClassName);
	CComponent* Clone_Component(_hashcode	hcClassName);

	template <typename T>
	T* Clone_GameObject()
	{
		T* pTargetObject = Find_GameObject_Prototype<T>();

		if (!pTargetObject)
		{
			wstring strMsg = L"Failed to Find_GameObject_Prototype : CPrototype_Manager - ";
			string strTemp = typeid(T).name();
			wstring wstrName(strTemp.begin(), strTemp.end());
			strMsg += wstrName;
			Call_MsgBox(strMsg.c_str());
			return nullptr;
		}

		return pTargetObject->Clone();
	}

	template <typename T>
	T* Find_GameObject_Prototype()
	{
		auto iter = m_GameObject_Prototypes.find(HASHCODE(T));
		if (iter == m_GameObject_Prototypes.end())
			return nullptr;
		return static_cast<T*>(iter->second);
	}
	
	template <typename T>
	HRESULT	Add_GameObject_Prototype(T* pGameObject)
	{
		if (Find_GameObject_Prototype<T>())
			return E_FAIL;

		m_GameObject_Prototypes.emplace(HASHCODE(T), pGameObject);
		
		return S_OK;
	}


	template <typename T>
	T* Clone_Component()
	{
		T* pTargetObject = Find_Component_Prototype<T>();

		if (!pTargetObject)
		{
			wstring strMsg = L"Failed to Find_Component_Prototype : CPrototype_Manager - ";
			string strTemp = typeid(T).name();
			wstring wstrName(strTemp.begin(), strTemp.end());
			strMsg += wstrName;
			Call_MsgBox(strMsg.c_str());
			return nullptr;
		}

		return pTargetObject->Clone();
	}

	template <typename T>
	T* Find_Component_Prototype()
	{
		auto iter = m_Component_Prototypes.find(HASHCODE(T));
		if (iter == m_Component_Prototypes.end())
			return nullptr;
		return static_cast<T*>(iter->second);
	}

	template <typename T>
	HRESULT	Add_Component_Prototype(T* pComponent)
	{
		if (Find_Component_Prototype<T>())
			return E_FAIL;

		m_Component_Prototypes.emplace(HASHCODE(T), pComponent);

		return S_OK;
	}


public:
	void	Delete_GameObject_Prototypes();
	void	Delete_Component_Prototypes();

private:
	map<_hashcode, CGameObject*>	m_GameObject_Prototypes;
	map<_hashcode, CComponent*>		m_Component_Prototypes;

private:
	void			Release();

	friend class CLevel_Manager;
	void			Clear() { Release(); }

};

END