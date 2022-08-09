#pragma once
#include <iostream>
#include "slf_utils.h"

namespace SLF4cpp
{

#define SLF4CPP_ALLOC_DEF_NAME 0x34464C53
#define SLF4CPP_ALLOC_ELEM_NAME 0x34464C54

	class memory_alloc
	{
	public:
		memory_alloc(uint32_t unName = SLF4CPP_ALLOC_DEF_NAME)
		{
			m_unName = unName;
		}
		memory_alloc(const memory_alloc &x)
		{
			m_unName = x.GetName();
		}

		memory_alloc& operator=(const memory_alloc &x)
		{
			m_unName = x.GetName();
		}

		char *allocate(int nSize)
		{
			nSize = sizeof(uint32_t) + nSize;
			char *pBuffer = new char[nSize];
			if (NULL == pBuffer)
			{
				//to do assert
			}
			uint32_t *pTmpName = (uint32_t *)pBuffer;
			pTmpName[0] = m_unName;
			return pBuffer + sizeof(uint32_t);
		}

		void deallocate(char *pBuf)
		{
			if (NULL == pBuf)
			{
				return;
			}
			uint64_t un64Pointer = (uint64_t)pBuf;
			un64Pointer -= 4;
			char *pRealStart = (char *)un64Pointer;
			uint32_t *pTmpName = (uint32_t *)pRealStart;
			if (pTmpName[0] != m_unName)
			{
				//to do assert
			}
			delete[] pRealStart;
		}

		uint32_t GetName() const
		{
			return m_unName;
		}

		// Used must be carefully
		void SetName(uint32_t unName)
		{
			m_unName = unName;
		}

	private:
		uint32_t m_unName;
	};


	template<typename Elem>
	class memory_alloc_elem : public memory_alloc
	{
	public:
		memory_alloc_elem(uint32_t unName) :
			memory_alloc(unName)
		{
		}
		memory_alloc_elem() :
			memory_alloc(SLF4CPP_ALLOC_ELEM_NAME)
		{
		}

		char *allocate(uint32_t nSize)
		{
			return memory_alloc::allocate(sizeof(Elem) * nSize);
		}

		void deallocate(char *pBuf)
		{
			return memory_alloc::deallocate(pBuf);
		}
	};
}