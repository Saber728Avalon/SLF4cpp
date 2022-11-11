#pragma once
#include <iostream>
#include "slf_utils.h"
#include <malloc.h>

//分配内存
inline void* SLF_MALLCO(size_t size)
{
	return malloc(size);
}
//释放内存
inline void SLF_FREEE(void *p)
{
	free(p);
}

//重载new
static void* operator new(size_t size) throw(std::bad_alloc)
{
	void *pMem = SLF_MALLCO(size);
	if (NULL == pMem)
	{
		// to do assert
	}
	return pMem;
}
//重载new []
static void *operator new[](size_t count) throw(std::bad_alloc)
{
	return operator new (count);
}

//重载delete
//static void operator delete(void *rawMemory) throw(std::bad_alloc)
//{
//	if (rawMemory == 0) return;
//	free(rawMemory);
//}
static void operator delete(void *rawMemory)
{
	if (rawMemory == 0) return;
	SLF_FREEE(rawMemory);
}
//重载delete []
static void operator delete[](void *pMem, size_t count) throw(std::bad_alloc)
{
	return operator delete(pMem);
}
static void operator delete[](void *pMem)
{
	return operator delete(pMem);
}

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
		
		//分配内存
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

		//释放内存
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

		virtual uint32_t GetName() const
		{
			return m_unName;
		}

		// Used must be carefully
		virtual void SetName(uint32_t unName)
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