#pragma once

namespace Engine
{
	class CTag_Finder
	{
	private:
		const CHAR*	m_pStr;

	public:
		CTag_Finder(const CHAR* pStr)
			: m_pStr(pStr)
		{

		}

	public:
		template<typename T>
		bool	operator()(T& Pair)
		{
			if (!strcmp(m_pStr, Pair.first))
				return true;

			return false;
		}
	};


	class CTag_Finder_W
	{
	private:
		const TCHAR*	m_pStr;

	public:
		CTag_Finder_W(const TCHAR* pStr)
			: m_pStr(pStr)
		{

		}

	public:
		template<typename T>
		bool	operator()(T& Pair)
		{
			if (!lstrcmp(m_pStr, Pair.first))
				return true;

			return false;
		}
	};

}

