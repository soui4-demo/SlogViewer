﻿#pragma once

#include <helper/SAdapterBase.h>
#include <helper/STime.h>
#include <helper/obj-ref-impl.hpp>
#include <souicoll.h>

namespace SOUI
{
	class SLogInfo : public TObjRefImpl<IObjRef>
	{
	public:
		SLogInfo():iSourceLine(0),iLine(0){}
		int      iLine;
		SStringW strTime;
		DWORD	 dwPid;
		DWORD	 dwTid;
		SStringW strPackage;
		SStringW strLevel;
		int	     iLevel;
		SStringW strTag;
		SStringW strContent;
		SStringW strContentLower;//小写版Content
		SStringW strModule;
		SStringW strSourceFile;
		int      iSourceLine;
		SStringW strFunction;
	};

	#define MAX_LEVEL_LENGTH  50

	enum Levels
	{
		Verbose=0,
		Debug,
		Info,
		Warn,
		Error,
		Assert,

		Level_Count,
	};

	enum Field
	{
		col_invalid=-1,
		col_line_index=0,
		col_time,
		col_pid,
		col_tid,
		col_level,
		col_tag,
		col_module,
		col_source_file,
		col_source_line,
		col_function,
		col_content,
		col_package,
		col_unknown,
	};

	struct ILogParse : public IObjRef
	{
		virtual SStringW GetName() const PURE;
		virtual int GetLevels() const PURE;			
		virtual void GetLevelText(wchar_t szLevels[][MAX_LEVEL_LENGTH]) const PURE;
		virtual BOOL ParseLine(LPCWSTR pszLine,int nLen,SLogInfo **ppLogInfo) const PURE;
		virtual bool IsFieldValid(Field field) const PURE;
		virtual int  GetCodePage() const PURE;
		virtual BOOL TestLogBuffer(LPCSTR pszBuf, int nLength) PURE;
	};
	

	struct SRange
	{
		SRange(int a=0,int b=0):iBegin(a),iEnd(b){}
		int iBegin;
		int iEnd;
	};

	class SFilterKeyInfo
	{
	public:

		void Clear();

		bool IsEmpty() const;

		void SetFilterKeys(const SStringW & szFilter);

		bool TestExclude(const SStringW &strContent) const;

		bool TestInclude(const SStringW &strContent) const;

		int FindKeyRange(const SStringW &strContent, SArray<SRange> &outRange) const;
	private:
		static int SRangeCmp(const void* p1, const void* p2);

		SStringWList m_lstInclude;
		SStringWList m_lstExclude;
	};


	class SLogBuffer
	{
		friend class SLogAdapter;
	public:
		SLogBuffer(ILogParse *pLogParser=NULL);

		~SLogBuffer();

		void Clear();

		void ParseLog(LPWSTR pszBuffer);

		SLogInfo * ParseLine(LPCWSTR pszLine,int nLen);

		SLogBuffer & operator = (const SLogBuffer & src);

		void Append(const SLogBuffer & src);

		void Insert(const SLogBuffer & src);
	protected:
		template<class KEY,class VALUE>
		void CopyMap(SMap<KEY,VALUE> &dest, const SMap<KEY,VALUE> &src)
		{
			SPOSITION pos = src.GetStartPosition();
			while(pos)
			{
				const SMap<KEY,VALUE>::CPair *pPair = src.GetNext(pos);
				dest[pPair->m_key] = pPair->m_value;
			}
		}

		SArray<SLogInfo*> m_lstLogs;	//log列表
		int				  m_nLineCount;//代码总行数

		SMap<SStringW,bool> m_mapTags;	//tag
		SMap<UINT,bool> m_mapPids;		//pid
		SMap<UINT,bool> m_mapTids;		//tid

		CAutoRefPtr<ILogParse> m_logParser;
	};

	class SLogAdapter : public SMcAdapterBase, public SLogBuffer
	{
	public:
		SLogAdapter(void);
		~SLogAdapter(void);

		void SetScintillaWnd(CScintillaWnd *pWnd);

		BOOL Load(const TCHAR *szFileName);
		BOOL LoadMemory(LPWSTR buffer);
	
		void Clear();
		void SetFilter(const SStringT& str);
		void SetLevel(int nCurSel);

		ILogParse * GetLogParse() const {
			return m_logParser;
		}

		int GetTags(SArray<SStringW> &tags) const;
		int GetPids(SArray<UINT> &pids) const;
		int GetTids(SArray<UINT> &tids) const;
		void SetFilterTags(const SArray<SStringW> & tags);
		void SetFilterPids(const SArray<UINT> & lstPid);
		void SetFilterTids(const SArray<UINT> & lstTid);

		void SetLogParserPool(SList<ILogParse*> *pLogParserPool);
		SLogInfo* GetLogInfo(int iItem) const;

	protected:

		const SArray<SLogInfo*> * GetLogList() const;

		void doFilter();

		BOOL OnItemDblClick(EventArgs *e);
	protected:
		virtual void WINAPI getView(int position, SItemPanel * pItem, SXmlNode xmlTemplate);
		virtual SStringW WINAPI GetColumnName(int iCol) const;
		virtual BOOL WINAPI IsColumnVisible(int iCol) const;
		virtual int WINAPI getCount();
	private:
		SArray<SLogInfo*> *m_lstFilterResult;

		SFilterKeyInfo m_filterKeyInfo;

		int		m_filterLevel;
		
		SMap<SStringW,bool> m_filterTags;
		SMap<UINT,bool> m_filterPids;
		SMap<UINT,bool> m_filterTids;

		COLORREF m_crLevels[Level_Count];

		//CAutoRefPtr<IParserFactory> m_parserFactory;
		SList<ILogParse*> *		 m_pLogPaserPool;
		CScintillaWnd	*		 m_pScilexer;
	};

}
