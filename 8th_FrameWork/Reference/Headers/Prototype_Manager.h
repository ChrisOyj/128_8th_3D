#pragma once
#include "Engine_Defines.h"

//파일 연동으로 오브젝트 생성시에는 꼭 클라쪽에서 미리 Add_Prototype을 해주는 과정이 필요함!!
BEGIN(Engine)

class CGameObject;
class CComponent;

class ENGINE_DLL CPrototype_Manager final
{
	DECLARE_SINGLETON(CPrototype_Manager);

private:
	CPrototype_Manager();
	virtual ~CPrototype_Manager();

private:
	map<_hashcode, CGameObject*>	m_GameObject_Prototypes;
	map<_hashcode, CComponent*>	m_Component_Prototypes;

public:
	//Tool 연동시, hashcode를 이용한 Clone 함수들
	CGameObject*	Clone_GameObject(_hashcode hashcode);

	template <typename T>
	T* Clone_GameObject()
	{
		T* pPrototype = Find_GameObject_Prototype<T>();

		//없으면 그때 추가
		if (!pPrototype)
		{
			pPrototype = Add_GameObject_Prototype<T>();

			if (!pPrototype)
				return nullptr;
		}

		return pPrototype->Clone();
	}


	CComponent* Clone_Component(_hashcode hashcode);

	template <typename T>
	T* Clone_Component()
	{
		T* pPrototype = Find_Component_Prototype<T>();

		//없으면 그때 추가
		if (!pPrototype)
		{
			pPrototype = Add_Component_Prototype<T>();

			if (!pPrototype)
				return nullptr;
		}

		return pPrototype->Clone();
	}

private:
	CGameObject*			Find_GameObject_Prototype(_hashcode hashcode);
	CComponent*			Find_Component_Prototype(_hashcode hashcode);

	template <typename T>
	T* Find_GameObject_Prototype()
	{
		auto iter = m_GameObject_Prototypes.find(HASHCODE(T));

		if (iter == m_GameObject_Prototypes.end())
			return nullptr;

		return static_cast<T*>(iter->second);
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
	T* Add_GameObject_Prototype()
	{
		T* pPrototype = T::Create_Prototype();

		if (!pPrototype)
			return nullptr;

		m_GameObject_Prototypes.emplace(HASHCODE(T), pPrototype);
		return pPrototype;
	}

	template <typename T>
	T* Add_Component_Prototype()
	{
		T* pPrototype = T::Create_Prototype();

		if (!pPrototype)
			return nullptr;

		m_Component_Prototypes.emplace(HASHCODE(T), pPrototype);
		return pPrototype;
	}

private:
	void			Release();

	friend class CLevel_Manager;
	void			Clear() { Release(); }



};

END