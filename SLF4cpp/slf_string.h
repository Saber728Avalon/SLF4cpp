#pragma once
#include <iostream>
#include "slf_memory.h"

namespace SLF4cpp
{
	template<typename Elem>
	class char_traits
	{
	public:
		static inline int length(const Elem *pSrc)
		{
			int i = 0;
			for (; pSrc[i] != '\0'; i++);
			return i;
		}

		static inline void copy(Elem *pDst, const Elem *pSrc, const int nLen)
		{
			slf_mem_copy(pDst, pSrc, sizeof(Elem) * nLen);
		}

		static inline  bool eq(const Elem *pSrc1, const int nSrc1, const Elem *pSrc2, const int nSrc2)
		{
			if (nSrc1 != nSrc2 || NULL == pSrc1 || NULL == pSrc2)
			{
				return false;
			}
			for (int i = 0; i < nSrc1; i++)
			{
				if (pSrc1[i] != pSrc2[i])
				{
					return false;
				}
			}
			return true;
		}
	};


	template<typename Elem, class _Traits = char_traits<Elem>>
	class basic_string : public memory_alloc_elem<Elem>
	{
#define DEF_BUF_SIZE 32
#define DEF_ALIGN_SIZE 32

	public:
		typedef Elem PROTOTYPE;

		basic_string()
		{
			m_pBuffer = string_allloc(DEF_BUF_SIZE);
			if (NULL == m_pBuffer)
			{
				return;
			}
			
			m_nCap = DEF_BUF_SIZE;
			m_nPos = 0;
		}
		basic_string(uint32_t unSize)
		{
			unSize = align(unSize);
			m_pBuffer = string_allloc(DEF_BUF_SIZE);
			if (NULL == m_pBuffer)
			{
				return;
			}

			m_nCap = unSize;
			m_nPos = 0;
		}

		basic_string(const basic_string &src)
		{
			int nSrcSize = src.length();
			nSrcSize = align(nSrcSize);
			m_pBuffer = string_allloc(nSrcSize);
			if (NULL == m_pBuffer)
			{
				//to do
			}
			_Traits::copy(m_pBuffer, src.c_str(), src.length());
			m_nCap = nSrcSize;
			m_nPos = src.length();
		}

		basic_string(const Elem *pSrc)
		{
			int nLen = _Traits::length(pSrc);
			int nCapLen = align(nLen);
			m_pBuffer = string_allloc(nCapLen);
			if (NULL == m_pBuffer)
			{
				return;
			}
			_Traits::copy(m_pBuffer, pSrc, nLen);
			m_nCap = nCapLen;
			m_nPos = nLen;
		}

		~basic_string()
		{
			deallocate(m_pBuffer);
		}

		basic_string& operator = (basic_string &src)
		{
			int nSrcSize = src.length();
			int nAlignSize = align(nSrcSize);
			Elem *pBuffer = string_allloc(nAlignSize);
			_Traits::copy(pBuffer, src.c_str(), nSrcSize);
			if (NULL != m_pBuffer)
			{
				deallocate(m_pBuffer);
			}
			m_nPos = nSrcSize;
			m_pBuffer = pBuffer;
			m_nCap = nAlignSize;
			return *this;
		}
		basic_string& operator = (const Elem *pSrc)
		{
			int nSrcSize = _Traits::length(pSrc);
			int nAlignSize = align(nSrcSize);
			Elem *pBuffer = string_allloc(nAlignSize);
			_Traits::copy(pBuffer, pSrc, nSrcSize);
			if (NULL != m_pBuffer)
			{
				deallocate(m_pBuffer);
			}
			m_nPos = nSrcSize;
			m_nCap = nAlignSize;
			m_pBuffer = pBuffer;
			return *this;
		}
		basic_string& operator += (basic_string &src)
		{
			append(src);
			return *this;
		}

		basic_string& operator + (basic_string &src)
		{
			int nLen = src.length();
			appendCopy(src.c_str(), nLen);
			return *this;
		}
	
		bool operator == (basic_string &src)
		{
			return _Traits::eq(m_pBuffer, m_nPos, src.c_str(), src.length());
		}
		bool operator == (Elem *pSrc)
		{
			int nSize = _Traits::length(pSrc);
			return _Traits::eq(m_pBuffer, m_nPos, pSrc, nSize);
		}


		Elem operator [] (int n) const
		{
			if (n > m_nPos)
			{
				n = m_nPos;
			}
			return m_pBuffer[n];
		}
		Elem &operator [] (int n)
		{
			if (n > m_nPos)
			{
				n = m_nPos;
			}
			return m_pBuffer[n];
		}


		const Elem * c_str() const
		{
			return m_pBuffer;
		}

		basic_string& append(const Elem *pData)
		{
			int nLen = _Traits::length(pData);
			append(pData, nLen);
			m_pBuffer[m_nPos] = '\0';
			return *this;
		}

		basic_string& append(const Elem *pData, int nCount)
		{
			if (need_new(nCount))
			{
				appendCopy(pData, nCount);
			}
			else
			{
				_Traits::copy(&(m_pBuffer[m_nPos]), pData, nCount);
				m_nPos += nCount;
			}
			m_pBuffer[m_nPos] = '\0';
			return *this;
		}

		basic_string& append(const basic_string &strSrc)
		{
			int nLen = strSrc.length();
			if (need_new(nLen))
			{
				appendCopy(strSrc.c_str(), nLen);
			}
			else
			{
				_Traits::copy(&(m_pBuffer[m_nPos]), strSrc.c_str(), nLen);
				m_nPos += nLen;
			}
			m_pBuffer[m_nPos] = '\0';
			return *this;
		}

		basic_string& append(const Elem  e)
		{
			if (need_new(m_nPos))
			{
				appendCopy(&e, 1);
			}
			else
			{
				m_pBuffer[m_nPos] = e;
				m_nPos += 1;
			}
			m_pBuffer[m_nPos] = '\0';
			return *this;
		}

		void replace(int nPos, int nCount, const basic_string &strSrc)
		{
			int nLen = strSrc.length() + m_nPos;
			nLen = align(nLen);
			Elem * pNewBuffer = string_allloc(nLen);
			if (NULL == pNewBuffer)
			{
				// to do assert
			}
			_Traits::copy(pNewBuffer, m_pBuffer, nPos);
			_Traits::copy(pNewBuffer + nPos, strSrc.c_str(), strSrc.length());
			_Traits::copy(pNewBuffer + nPos + strSrc.length(), (Elem *)&(m_pBuffer[nPos + nCount + 1]), m_nPos - nPos);

			deallocate(m_pBuffer);
			m_pBuffer = pNewBuffer;
			m_nCap = nLen;
			m_nPos = _Traits::length(m_pBuffer);
			return ;
		}

		inline bool need_new(int nSize)
		{
			return m_nPos + nSize + 1 > m_nCap;
		}

		int length() const
		{
			return m_nPos;
		}

		int cap()
		{
			return m_nCap;
		}

	private:
		inline int align(int nSize)
		{
			return ((nSize + DEF_ALIGN_SIZE + 31) & 0xFFFFFFF0);
		}

		Elem *string_allloc(int nLen)
		{
			try
			{
				Elem *pMem = allocate(nLen);
				if (NULL != pMem)
				{
					slf_mem_set(pMem, '\0', nLen * sizeof(Elem));
				}
				return pMem;
			}
			catch (...)
			{
			}
			return NULL;
		}

		basic_string& appendCopy(const Elem *pAppendData, int nAppendSize)
		{
			int nSize = m_nCap + nAppendSize;
			nSize = align(nSize);
			Elem * pNewBuffer = string_allloc(nSize);
			if (NULL == pNewBuffer)
			{
				// to do assert
				return *this;
			}

			_Traits::copy(pNewBuffer, m_pBuffer, m_nPos);
			_Traits::copy(&(pNewBuffer[m_nPos]), pAppendData, nAppendSize);
			deallocate(m_pBuffer);
			m_pBuffer = pNewBuffer;
			m_nCap = nSize;
			m_nPos = m_nPos + nAppendSize;
			return *this;
		}

		void free_new_alloc(int nSize)
		{
			if (NULL != m_pBuffer)
			{
				deallocate(m_pBuffer);
			}
			nSize = align(nSize);
			m_pBuffer = string_allloc(nSize);
			if (NULL == m_pBuffer)
			{
				return;
			}
			m_nCap = nSize;
			m_nPos = 0;
			return;
		}

	private:

		Elem *m_pBuffer;
		int m_nCap;
		int m_nPos;
	};

#if defined(SLF4CPP_USE_CHAR)
	typedef basic_string<char, char_traits<char>> slf_string;
#elif defined(SLF4CPP_USE_UTF16)
	typedef basic_string<wchar_t, char_traits<wchar_t>> slf_string;
#elif defined(SLF4CPP_USE_UTF32)
	typedef basic_string<uint32_t, char_traits<uint32_t>> slf_string;
#endif
}