#pragma once



namespace Engine
{

	static _int random(_int _min, _int _max)
	{
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<_int> disx(_min, _max);

		return disx(gen);
	}

	static _float frandom(_float _min, _float _max)
	{
		random_device rd;
		mt19937 gen(rd());
		uniform_real_distribution<_float> disx(_min, _max);

		return disx(gen);
	}

	template<typename T>
	void Safe_Delete(T& Temp)
	{
		if (Temp)
		{
			delete Temp;
			Temp = nullptr;
		}
	}

	template<typename T>
	unsigned long Safe_Release(T& Temp)
	{
		unsigned long dwRefCnt = 0;

		if (Temp)
		{
			dwRefCnt = Temp->Release();

			if(0 == dwRefCnt)
				Temp = nullptr;
		}

		return dwRefCnt;
	}

	template<typename T>
	unsigned long Safe_AddRef(T& Temp)
	{
		unsigned long dwRefCnt = 0;

		if (Temp)	
			dwRefCnt = Temp->AddRef();		

		return dwRefCnt;
	}


	template<typename T>
	void Safe_Delete_Array(T& Temp)
	{
		if (Temp)
		{
			delete[] Temp;
			Temp = nullptr;
		}
	}

}
