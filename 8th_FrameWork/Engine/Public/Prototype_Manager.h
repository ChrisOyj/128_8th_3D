#pragma once
#include "Engine_Defines.h"

//���� �������� ������Ʈ �����ÿ��� �� Ŭ���ʿ��� �̸� Add_Prototype�� ���ִ� ������ �ʿ���!!
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
	//Tool ������, hashcode�� �̿��� Clone �Լ���
	CObj*			Get_Clone(_hashcode hashcode);

	template <typename T>
	T*				Get_Clone()
	{
		T* pPrototype = Find_Prototype<T>();

		//������ �׶� �߰�
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