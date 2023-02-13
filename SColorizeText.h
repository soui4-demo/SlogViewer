#pragma once

namespace SOUI
{
	class SColorizeText : public SWindow
	{
		DEF_SOBJECT(SWindow,L"colorizeText")
	public:
		SColorizeText(void);
		~SColorizeText(void);

		void ClearColorizeInfo();
		void AddColorizeInfo(int iBegin,int iEnd,COLORREF cr);
	protected:
		virtual void DrawText(IRenderTarget *pRT,LPCTSTR pszBuf,int cchText,LPRECT pRect,UINT uFormat);

		struct COLORIZEINFO
		{
			int iBegin;
			int iEnd;
			COLORREF cr;
		};

		SArray<COLORIZEINFO> m_lstColorizeInfo;
	};

}
