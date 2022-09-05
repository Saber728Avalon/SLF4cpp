#pragma once
#include <vector>
#include <algorithm>
#include "slf_string.h"
#include "slf_cast.h"
#include "slf_enum.h"


namespace SLF4cpp
{
#define MAX_LEVEL_SIZE 5

	class slf_time_foramt
	{
	public:
		slf_time_foramt()
		{
			m_logLevel[slf_log_level::slf_log_level_debug][0] = 'D';
			m_logLevel[slf_log_level::slf_log_level_debug][1] = 'E';
			m_logLevel[slf_log_level::slf_log_level_debug][2] = 'B';
			m_logLevel[slf_log_level::slf_log_level_debug][3] = 'U';
			m_logLevel[slf_log_level::slf_log_level_debug][4] = 'G';
			m_logLevel[slf_log_level::slf_log_level_debug][5] = '\O';

			m_logLevel[slf_log_level::slf_log_level_info][0] = ' ';
			m_logLevel[slf_log_level::slf_log_level_info][1] = 'I';
			m_logLevel[slf_log_level::slf_log_level_info][2] = 'N';
			m_logLevel[slf_log_level::slf_log_level_info][3] = 'F';
			m_logLevel[slf_log_level::slf_log_level_info][4] = 'O';
			m_logLevel[slf_log_level::slf_log_level_info][5] = '\O';

			m_logLevel[slf_log_level::slf_log_level_warn][0] = ' ';
			m_logLevel[slf_log_level::slf_log_level_warn][1] = 'W';
			m_logLevel[slf_log_level::slf_log_level_warn][2] = 'A';
			m_logLevel[slf_log_level::slf_log_level_warn][3] = 'R';
			m_logLevel[slf_log_level::slf_log_level_warn][4] = 'N';
			m_logLevel[slf_log_level::slf_log_level_warn][5] = '\O';

			m_logLevel[slf_log_level::slf_log_level_error][0] = 'E';
			m_logLevel[slf_log_level::slf_log_level_error][1] = 'R';
			m_logLevel[slf_log_level::slf_log_level_error][2] = 'R';
			m_logLevel[slf_log_level::slf_log_level_error][3] = 'O';
			m_logLevel[slf_log_level::slf_log_level_error][4] = 'R';
			m_logLevel[slf_log_level::slf_log_level_error][5] = '\O';
		}
		~slf_time_foramt(){}
		
		

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

		class NodeFormatType
		{
		public:
			NodeFormatType(slf_string::PROTOTYPE *pName, NodeType type)
			{
				strFormatName = pName;
				this->type = type;
			}
			~NodeFormatType()
			{
			}

			slf_string strFormatName;
			NodeType type;
		};

		class TimeFormatNode
		{
		public:
			TimeFormatNode()
			{
				nodeType = NodeTypeCount;
				nFormatStart = -1;
				nFormatLen = -1;
				nOriginalStart = -1;
				nOriginalLen = -1;
			}
			NodeType nodeType;
			int nFormatStart;//格式字符串开始位置
			int nFormatLen;//格式字符串长度

			int nOriginalStart;//该格式字符串之后，原样字符串的开始位置
			int nOriginalLen;//该格式字符串之后，原样字符串的长度

		};

		static bool TimeFormatNodeCmp(TimeFormatNode node1, TimeFormatNode node2)
		{
			return node1.nFormatStart < node2.nFormatStart;
		}

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
			for (int i = 0; i <= nTmpAll; i++)
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

		slf_string format_time(slf_string &strLog, slf_log_level nLevel)
		{
			int nValue = 0;
			int nStartPos = 0;
			slf_string::PROTOTYPE chBufTime[64] = {0};
			slf_string::PROTOTYPE chNoBuf[64];

			if (0 == m_vectNode.size())
			{
				return strLog;
			}
			
			//把最开始的前缀写入
			auto &nodeFirst = m_vectNode[0];
			slf_mem_copy((char*)chBufTime, m_strPattern.c_str(),  nodeFirst.nFormatStart);
			nStartPos += nodeFirst.nFormatStart;

			struct SLF_MT timeInfo = slf_time_stamp();
			for (int i = 0; i < m_vectNode.size(); i++)
			{
				auto &nodeTmp = m_vectNode[i];
				switch (nodeTmp.nodeType)
				{
				case NodeTypeYear:
					nValue = timeInfo.tm_year;
					break;
				case NodeTypeMonth:
					nValue = timeInfo.tm_mon;
					break;
				case NodeTypeDay:
					nValue = timeInfo.tm_mday;
					break;
				case NodeTypeHour:
					nValue = timeInfo.tm_hour;
					break;
				case NodeTypeMinute:
					nValue = timeInfo.tm_min;
					break;
				case NodeTypeSecond:
					nValue = timeInfo.tm_sec;
					break;
				case NodeTypeMillisecond:
					nValue = timeInfo.tm_millsec;
					break;
				case NodeTypeTID:
					nValue = GetCurrentPID();
					break;
				case NodeTypeLevel:
					{
						slf_mem_copy(&chBufTime[nStartPos],  m_logLevel[nLevel], MAX_LEVEL_SIZE * sizeof(slf_string::PROTOTYPE));
						nStartPos += MAX_LEVEL_SIZE;
						//添加两个项中间的，原样输出字符串
						if ( i < m_vectNode.size() - 1)
						{
							slf_mem_copy(&chBufTime[nStartPos],  m_strPattern.c_str() + nodeTmp.nOriginalStart, nodeTmp.nOriginalLen * sizeof(slf_string::PROTOTYPE));
						}
						else
						{
							slf_mem_copy(&chBufTime[nStartPos],  m_strPattern.c_str() + nodeTmp.nOriginalStart, nodeTmp.nOriginalLen * sizeof(slf_string::PROTOTYPE));
						}
						nStartPos += nodeTmp.nOriginalLen;
					}
					continue;
				default:
					break;
				}
				int nLen = unsigned_integer(nValue, chNoBuf, 64);
				int nSubLen = nodeTmp.nFormatLen - nLen;
				if (nSubLen > 0)
				{
					for (int i = 0; i < nSubLen; i++)
					{
						chBufTime[nStartPos] = '0';
						nStartPos++;
					}
				}

				slf_mem_copy(&chBufTime[nStartPos],  chNoBuf, nLen * sizeof(slf_string::PROTOTYPE));
				nStartPos += nLen;

				//添加两个项中间的，原样输出字符串
				if ( i < m_vectNode.size() - 1)
				{
					slf_mem_copy(&chBufTime[nStartPos],  m_strPattern.c_str() + nodeTmp.nOriginalStart, nodeTmp.nOriginalLen * sizeof(slf_string::PROTOTYPE));
				}
				else
				{
					slf_mem_copy(&chBufTime[nStartPos],  m_strPattern.c_str() + nodeTmp.nOriginalStart, nodeTmp.nOriginalLen * sizeof(slf_string::PROTOTYPE));
				}
				nStartPos += nodeTmp.nOriginalLen;
			}
			slf_string strFormatTime;
			strFormatTime = chBufTime;
			strFormatTime = strFormatTime + strLog;
			return std::move(strFormatTime);
		}

		//yyyy-MM-dd HH:mm:ss.SSS LEVEL tid
		int analyze(slf_string::PROTOTYPE *pPrefix)
		{
			slf_string strTmp = pPrefix;
			int nStart = 0;
			int nEnd = strTmp.length();
			TimeFormatNode tmpNode;
			m_strPattern = pPrefix;
			std::vector<NodeFormatType> vectNodeFormatType;

			//这样避免写不同编码定义字符串会不同
			slf_string::PROTOTYPE slfBufYear[] = { 'y', 'y', 'y', 'y', '\0'};
			slf_string::PROTOTYPE slfBufMonth[] = { 'M', 'M', '\0'};
			slf_string::PROTOTYPE slfBufDay[] = { 'd', 'd', '\0'};
			slf_string::PROTOTYPE slfBufHour[] = { 'H', 'H', '\0'};
			slf_string::PROTOTYPE slfBufMinute[] = { 'm', 'm', '\0'};
			slf_string::PROTOTYPE slfBufSecond[] = { 's', 's', '\0'};
			slf_string::PROTOTYPE slfBufMillsec[] = { 'S', 'S', 'S', '\0'};
			slf_string::PROTOTYPE slfBufLevel[] = { 'l', 'e', 'v', 'e','l', '\0'};
			slf_string::PROTOTYPE slfBufTID[] = { 't', 'i', 'd', '\0'};

			vectNodeFormatType.push_back(NodeFormatType(slfBufYear, NodeTypeYear));
			vectNodeFormatType.push_back(NodeFormatType(slfBufMonth, NodeTypeMonth));
			vectNodeFormatType.push_back(NodeFormatType(slfBufDay, NodeTypeDay));
			vectNodeFormatType.push_back(NodeFormatType(slfBufHour, NodeTypeHour));
			vectNodeFormatType.push_back(NodeFormatType(slfBufMinute, NodeTypeMinute));
			vectNodeFormatType.push_back(NodeFormatType(slfBufSecond, NodeTypeSecond));
			vectNodeFormatType.push_back(NodeFormatType(slfBufMillsec, NodeTypeMillisecond));
			vectNodeFormatType.push_back(NodeFormatType(slfBufLevel, NodeTypeLevel));
			vectNodeFormatType.push_back(NodeFormatType(slfBufTID, NodeTypeTID));

			for (auto iter = vectNodeFormatType.begin(); iter !=vectNodeFormatType.end(); iter++)
			{
				if (find(strTmp.c_str(), strTmp.length(), iter->strFormatName.c_str(), iter->strFormatName.length(), nStart, nEnd))
				{
					tmpNode.nodeType = iter->type;
					tmpNode.nFormatStart = nStart;
					tmpNode.nFormatLen = nEnd - nStart + 1;
					m_vectNode.push_back(tmpNode);
				}
			}
			
			//找出相邻节点中的原样字符串
			//先排序
			std::sort(m_vectNode.begin(), m_vectNode.end(), TimeFormatNodeCmp);
			for (int i = 0; i < m_vectNode.size() - 1; i++)
			{
				auto &cur = m_vectNode[i];
				cur.nOriginalStart = cur.nFormatStart + cur.nFormatLen;
				cur.nOriginalLen = m_vectNode[i + 1].nFormatStart - cur.nOriginalStart;
			}
			auto &cur = m_vectNode[m_vectNode.size() - 1];
			cur.nOriginalStart = cur.nFormatStart + cur.nFormatLen;
			cur.nOriginalLen = m_strPattern.length() - cur.nOriginalStart;
			return 0;
		}

	private:
		std::vector<TimeFormatNode> m_vectNode;
		slf_string m_strPattern;
		slf_string::PROTOTYPE m_logLevel[slf_log_level::slf_log_level_count][MAX_LEVEL_SIZE + 1];
	};
}

