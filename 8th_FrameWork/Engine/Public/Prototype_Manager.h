#pragma once
#include "Engine_Defines.h"

//파일 연동으로 오브젝트 생성시에는 꼭 클라쪽에서 미리 Add_Prototype을 해주는 과정이 필요함!!
BEGIN(Engine)

class CObj;

class ENGINE_DLL CPrototype_Manager final
{
	DECLARE_SINGLETON(CPrototype_Manager);

private:
	CPrototype_Manager();
	virtual ~CPrototype_Manager();

private:
	map<_hashcode, CObj*>	m_Prototypes;

public:
	//Tool 연동시, hashcode를 이용한 Clone 함수들
	CObj*			Get_Clone(_hashcode hashcode);

	template <typename T>
	T*				Get_Clone()
	{
		T* pPrototype = Find_Prototype<T>();

		//없으면 그때 추가
		if (!pPrototype)
		{
			pPrototype = Add_Prototype<T>();

			if (!pPrototype)
				return nullptr;
		}

		return pPrototype->Clone();
	}

private:
	CObj*			Find_Prototype(_hashcode hashcode);

	template <typename T>
	T*				Find_Prototype()
	{
		auto iter = m_Prototypes.find(HASHCODE(T));

		if (iter == m_Prototypes.end())
			return nullptr;

		return static_cast<T*>(iter->second);
	}
	template <typename T>
	T*				Add_Prototype()
	{
		T* pPrototype = T::Create_Prototype();

		if (!pPrototype)
			return nullptr;

		m_Prototypes.emplace(HASHCODE(T), pPrototype);
		return pPrototype;
	}

private:
	void			Release();

	friend class CLevel_Manager;
	void			Clear() { Release(); }



};

END