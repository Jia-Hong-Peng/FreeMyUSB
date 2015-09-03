
// CleanDeltaDlg.h : header file
//

#pragma once


// CCleanDeltaDlg dialog
class CCleanDeltaDlg : public CDialogEx
{
// Construction
public:
	CCleanDeltaDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CLEANDELTA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();			
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	BOOL StopService(CString strServiceName);
	static BOOL CALLBACK CCleanDeltaDlg::EnumWindowsProc(HWND hwnd, LPARAM lParam);
	void CCleanDeltaDlg::killProcessByTitle(CString strTarget);
	void CCleanDeltaDlg::Kill(HWND hwnd);
	afx_msg void OnBnClickedButton2();

	void CCleanDeltaDlg::killProcessByFileName(CString strFilename);
	afx_msg void OnBnClickedButton3();
	
};
