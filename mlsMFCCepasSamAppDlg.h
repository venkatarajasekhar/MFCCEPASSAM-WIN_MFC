// mlsMFCCepasSamAppDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "mlsCepas.h"
#include "afxcmn.h"
#include "ColorListCtrl.h"
#define UWM_CARDPRESENT (WM_USER + 100)
#define UWM_CARDABSENT (WM_USER + 101)
// CmlsMFCCepasSamAppDlg dialog
class CmlsMFCCepasSamAppDlg : public CDialog
{
// Construction
public:
	CmlsMFCCepasSamAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MLSMFCCEPASSAMAPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
public:
	CString mv_Balance;
	CString mv_Inform;
	//	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnCardPresent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCardAbsent(WPARAM wParam, LPARAM lParam);
	void InitDataGrid();
	void DisplayArrayTranLogRecord(int iRecordNumber);
	void InsertItems(BYTE *data,int iNumber);
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
	afx_msg void OnBnClickedButtonCreate();
	afx_msg void OnBnClickedButtonFormat();
	afx_msg void OnBnClickedButtonExit();
	void displayReadPurseRecord(mlsReadPurseRecord readPurseRecord);
	void detailReadPurseRecord(mlsReadPurseRecord readPurseRecord);
	CString ByteArrayToHexString(unsigned char *byteArr,int length);
	void clearReadPurseRecord();
	CString multilineEdit(unsigned char * text,int length,int number);
	LONG getHandleSAMCard();
	void test(CString text);
	CComboBox mc_ComboContactless;
	CComboBox mc_ComboContact;
	//BOOL mv_radioOp;
	CString mv_InputValue;
	//CButton mc_buttonFormat;
	CString m_cstrCepasVerion;
	CString m_cstrPurseStatus;
	CString m_cstrPurseBalance;
	CString m_cstrPurseBanlanceValue;
	CString m_cstrAutoLoadAmount;
	CString m_cstrCAN;
	CString m_cstrCSN;
	CString m_cstrPurseCreationDate;
	CString m_cstrPurseExpiryDate;
	CString m_cstrLastCreditTransactionTRP;
	CString m_cstrLastCreditTransactionHeader;
	CString m_cstrNoofRecordinTransactionlofile;
	CString m_cstrIssuerSpecificDataLength;
	CString m_cstrLastTransactionTRP;
	CString m_cstrLastTransactionRecord;
	CString m_cstrIssuerSpecificData;
	CString m_cstrLastTransactionDebitOptionByte;
	CString m_cstrCEPASVesionDetail;
	CString m_cstrPurseStatusDetail;
	CString m_cstrPurseBalanceDetail;
	CString m_cstrAutoLoadAmountDetail;
	CString m_cstrPurseExpiryDateDetail;
	CString m_cstrPurseCreationDateDetail;
	CString m_cstrTransactionTypeDetail;
	CString m_cstrTransactionAmountDetail;
	CString m_cstrTransactionDaTime;
	CString m_cstrNoofRecordDetail;
	CString m_cstrIssuerSpecificDataLengthDetail;
	CString m_cstrLastTransactionDebitOptionDetail;
	CEdit m_cstrRecordLength;
	CEdit m_ctlLastTransactionRecord;
public:
	CString m_cstrTransactionUserData;
public:
	CButton m_btDebit;
public:
	CString m_cstrValue;
public:
	afx_msg void OnBnClickedBtnDebitCard();
public:
	CString m_cstrPurseBanlance;
public:
	CString m_cstrCommandStatus;
public:
	CColorListCtrl m_clstTransactionLog;
public:
	afx_msg void OnCbnSelchangeComboContactless();
};
UINT CardDetectThread(LPVOID pParam );
UINT CardDetectUsingHKThread(LPVOID pParam );
UINT WorkerThread(LPVOID pParam );