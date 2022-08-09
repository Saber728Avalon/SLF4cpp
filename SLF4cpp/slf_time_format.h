#pragma once
#include <vector>
#include "slf_string.h"
#include "slf_cast.h"

namespace SLF4cpp
{
	class slf_time_foramt
	{
	public:

		enum NodeType
		{
			NodeTypeYear,
			NodeTypeMonth,
			NodeTypeDay,
			NodeTypeHour,
			NodeTypeMinute,
			NodeTypeSecond,
			NodeTypeMillisecond,
			NodeTypeLevel,
			NodeTypeTID,

			NodeTypeCount
		};

		class time_format_node
		{
		public:
			time_format_node()
			{
				nodeType = NodeTypeCount;
				nStart = -1;
				nLen = -1;
			}
			NodeType nodeType;
			int nStart;
			int nLen;
		};

		bool find(const slf_string::PROTOTYPE *pObj, int nObjLen, const slf_string::PROTOTYPE *pFind, int nFindLen, int &nStart, int &nEnd)
		{
			if (NULL == pObj || NULL == pFind)
			{
				return false;
			}

			int nTmpAll = nObjLen - nFindLen;
			if (nTmpAll < 0)
			{
				return false;
			}
			for (int i = 0; i < nTmpAll; i++)
			{
				if (pObj[i] == pFind[0])
				{
					
					for (int j = 0; j < nFindLen; j++)
					{
						if (pFind[j] != pObj[i + j])
						{
							return false;
						}
					}
					nStart = i;
					nEnd = i + nFindLen - 1;
					return true;
				}
			}
			return false;
		}

		void format_time(slf_string &strLog)
		{
			int nValue = 0;
			slf_string::PROTOTYPE chBufTime[64];
			slf_string::PROTOTYPE chNoBuf[64];

			slf_mem_copy((char*)chBufTime, m_strPattern.c_str(), m_strPattern.length());
			struct tm timeInfo = slf_time_stamp();
			for (auto iter = m_vectNode.begin(); iter != m_vectNode.end(); iter++)
			{
				switch (iter->nodeType)
				{
				case NodeTypeYear:
					nValue = timeInfo.tm_year;
					break;
				}
				int nLen = unsigned_integer(nValue, chNoBuf, 64);
				int nSubLen = iter->nLen - nLen;
				for (int i = 0; i < nSubLen; i++)
				{

				}
			}
			
		}

		//yyyy-MM-dd HH:mm:ss.SSS lv tid
		int analyze(slf_string::PROTOTYPE *pPrefix)
		{
			slf_string strTmp = pPrefix;
			int nStart = 0;
			int nEnd = 0;
			time_format_node tmpNode;
			m_strPattern = pPrefix;
			slf_string::PROTOTYPE slfBufYear[] = { 'y', 'y', 'y', 'y', '\0' };
			slf_string::PROTOTYPE slfBufMonth[] = { 'm', 'm', '\0' };
			slf_string::PROTOTYPE slfBufDay[] = { 'd', 'd', '\0' };
			slf_string::PROTOTYPE slfBufHour[] = { 'H', 'H', '\0' };
			slf_string::PROTOTYPE slfBufMinute[] = { 'm', 'm', '\0' };
			slf_string::PROTOTYPE slfBufSecond[] = { 's', 's', '\0' };
			slf_string::PROTOTYPE slfBufMillsec[] = { 'S', 'S', 'S', '\0' };
			slf_string::PROTOTYPE slfBufLevel[] = { 'l', 'v', '\0' };
			slf_string::PROTOTYPE slfBufTID[] = { 't', 'i', 'd', '\0' };

			

			if (find(strTmp.c_str(), strTmp.length(), slfBufYear, sizeof(slfBufYear), nStart, nEnd))
			{
				tmpNode.nodeType = NodeTypeYear;
				tmpNode.nStart = nStart;
				tmpNode.nLen = nEnd - nStart;
				m_vectNode.push_back(tmpNode);
			}
			if (find(strTmp.c_str(), strTmp.length(), slfBufMonth, sizeof(slfBufMonth), nStart, nEnd))
			{
				tmpNode.nodeType = NodeTypeMonth;
				tmpNode.nStart = nStart;
				tmpNode.nLen = nEnd - nStart;
				m_vectNode.push_back(tmpNode);
			}
			if (find(strTmp.c_str(), strTmp.length(), slfBufDay, sizeof(slfBufDay), nStart, nEnd))
			{
				tmpNode.nodeType = NodeTypeDay;
				tmpNode.nStart = nStart;
				tmpNode.nLen = nEnd - nStart;
				m_vectNode.push_back(tmpNode);
			}
			if (find(strTmp.c_str(), strTmp.length(), slfBufHour, sizeof(slfBufHour), nStart, nEnd))
			{
				tmpNode.nodeType = NodeTypeHour;
				tmpNode.nStart = nStart;
				tmpNode.nLen = nEnd - nStart;
				m_vectNode.push_back(tmpNode);
			}
			if (find(strTmp.c_str(), strTmp.length(), slfBufMinute, sizeof(slfBufMinute), nStart, nEnd))
			{
				tmpNode.nodeType = NodeTypeMinute;
				tmpNode.nStart = nStart;
				tmpNode.nLen = nEnd - nStart;
				m_vectNode.push_back(tmpNode);
			}
			if (find(strTmp.c_str(), strTmp.length(), slfBufSecond, sizeof(slfBufSecond), nStart, nEnd))
			{
				tmpNode.nodeType = NodeTypeSecond;
				tmpNode.nStart = nStart;
				tmpNode.nLen = nEnd - nStart;
				m_vectNode.push_back(tmpNode);
			}
			if (find(strTmp.c_str(), strTmp.length(), slfBufMillsec, sizeof(slfBufMillsec), nStart, nEnd))
			{
				tmpNode.nodeType = NodeTypeMillisecond;
				tmpNode.nStart = nStart;
				tmpNode.nLen = nEnd - nStart;
				m_vectNode.push_back(tmpNode);
			}
			if (find(strTmp.c_str(), strTmp.length(), slfBufLevel, sizeof(slfBufLevel), nStart, nEnd))
			{
				tmpNode.nodeType = NodeTypeLevel;
				tmpNode.nStart = nStart;
				tmpNode.nLen = nEnd - nStart;
				m_vectNode.push_back(tmpNode);
			}
			if (find(strTmp.c_str(), strTmp.length(), slfBufTID, sizeof(slfBufTID), nStart, nEnd))
			{
				tmpNode.nodeType = NodeTypeTID;
				tmpNode.nStart = nStart;
				tmpNode.nLen = nEnd - nStart;
				m_vectNode.push_back(tmpNode);
			}
		}

	private:
		std::vector<time_format_node> m_vectNode;
		slf_string m_strPattern;
	};
}

