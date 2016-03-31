// mlsMFCCepasSamAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mlsMFCCepasSamApp.h"
#include "mlsMFCCepasSamAppDlg.h"
#include "mlsManager.h"
#include "mlsException.h"
#include "mlsCepas.h"
#include "mlsSettlement.h"
#include "mlsLtaSAM.h"
#include "mlsDebug.h"
#include "mlsDateTime.h"

using namespace mlsMFCDll;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
int iStartYear =1995;
CString WORK_READER_NAME = "Magneton CL SmartCard Reader 0";
//CString WORK_READER_NAME = "STYL STY90x PICC Reader 0";
CString strPATHFILE = "CE_00000000_YYYYMMDD_HHMMSS_000000.bath";
UINT8 PATHFILE[39];
int iFileSequenceID = 1;
//Manager readers thread 
HWND mainhwnd;

mlsManager mlsManagerObj;

CCriticalSection criticalsection;

BOOL blCardDetectThreadRun;

CEvent EvCardDetect;

CEvent waitThreadStop;
mlsCard_str *card_str = new mlsCard_str();
mlsCard_str *cardSam_str = new mlsCard_str();
/*TODO mls@dev01 to review*/
CepasMDBatchFile *cepasMDBatchFile = NULL;


void readMaxFileSeQuenceID(UINT8 * sequenceId)
{
	WIN32_FIND_DATA f;
	HANDLE h = FindFirstFile("./*.bath", &f);
	UINT8 idTmp[6];
	if(h != INVALID_HANDLE_VALUE)
	{
		do
		{
			int temp = strlen(f.cFileName);
			if(temp==39)
			{
				memcpy(idTmp,(UInt8*)(&(f.cFileName[28])),6);
				if(compareTwoNumber(sequenceId,6,idTmp,6)<0)
					memcpy(sequenceId,idTmp,6);
			}
		} while(FindNextFile(h, &f));
	}
}

void createNameofMBBatchFileFormat(SYSTEMTIME currentTime)
{
	memcpy(PATHFILE,strPATHFILE.GetBuffer(),39);
	memcpy(PATHFILE+3,DEVICEID,8);
	convertNumbertoArrChar(currentTime.wYear,PATHFILE+12,4);
	convertNumbertoArrChar(currentTime.wMonth,PATHFILE+12+4,2);
	convertNumbertoArrChar(currentTime.wDay,PATHFILE+12+6,2);
	convertNumbertoArrChar(currentTime.wHour,PATHFILE+12+1+8,2);
	convertNumbertoArrChar(currentTime.wMinute,PATHFILE+12+1+10,2);
	convertNumbertoArrChar(currentTime.wSecond,PATHFILE+12+1+12,2);

	//serial id
	UINT8 sequenceID[6];
	memset(sequenceID,'0',6);
	readMaxFileSeQuenceID(sequenceID);
	sumArrayCharAndInt(sequenceID,6,1);
	memcpy((&PATHFILE[28]),sequenceID,6);
	//sumArrayCharAndInt((&PATHFILE[28]),6,iFileSequenceID);
}
CString convertIntTwoNumber(int input)
{
	CString output;
	if(input <10)
		output.Format(_T(" 0%d"),input);
	else
		output.Format(_T(" %d"),input);
	return output;
}

void mlsConvertJulianDatetoTime(int iSecondSum,int*Hour,int*Minute,int*Second)
{
	/*todo:mls@dev01 to review*/
	
	(*Hour) = 0;
	(*Minute) = 0;
	while(true)
	{
		if(iSecondSum > 3600)
		{
			(*Hour)++;
			iSecondSum = iSecondSum - 3600;
		}
		else{
			break;
		}
	}
	while(true)
	{
		if(iSecondSum > 60)
		{
			(*Minute)++;
			iSecondSum = iSecondSum - 60;
		}
		else{
			break;
		}
	}
	(*Second) = iSecondSum;
}
CString mlsTransactionType(int iType)
{
	/*todo:mls@dev01 to review*/
	switch(iType)
	{
	case 0x07:
		return "ERP Congestion Based Charging";
	case 0x08:
		return "ERP Distance Based Charging";
	case 0x09:
		return "EPS Time Based Charging";
	case 0x11:
		return "Purse Disable";
	case 0x30:
		return "Rail Payment";
	case 0x31:
		return "Bus Payment";
	case 0x66:
		return "Cashback";
	case 0x75:
		return "Add Value";
	case 0x76:
		return "Partial Refund";
	case 0x83:
		return "AutoLoad Disable";
	case 0x84:
		return "Purse and AutoLoad Disable";
	case 0x85:
		return "Rail Payment with AutoLoad Disable";
	case 0x86:
		return "Bus Payment with AutoLoad Disable";
	case 0x87:
		return "Partial Refund with AutoLoad Disable";
	case 0xA0:
		return "NTA Payment & AutoLoad";
	case 0xF0:
		return "Modify Purse";
	default:
		return "";
	}
}
// CAboutDlg dialog used for App About
UINT WorkerThread(LPVOID pParam )
{
	while (1);//do nothing

	return 0;   // thread completed successfully
}
UINT CardDetectUsingHKThread(LPVOID pParam )
{
	mlsManager* pManager = (mlsManager*)&mlsManagerObj;
	HWND hwnd = (HWND)pParam;
	mlsErrorCode_tp errorCode;

	try
	{
		mlsReader* pReader = pManager->GetReaderByName(CString(WORK_READER_NAME));

		//mlsReader* pReader = pManager->GetReaderByIndex(0);

		errorCode = pReader->Open();

		pReader->setCardDetectTimeOutMs(100);//500ms

		pReader->SetNotifyEvent(&EvCardDetect);

		pReader->StartReaderHouseKeeping();

		while (blCardDetectThreadRun== TRUE)
		{
			WaitForSingleObject(EvCardDetect,INFINITE);

			if (pReader->IsCardAvailable())
			{
				PostMessage(hwnd,UWM_CARDPRESENT,0,0);
				//pReader->Connect(); //test
			}
			else
			{
				PostMessage(hwnd,UWM_CARDABSENT,0,0);
				//pReader->Disconnect();//test
			}
		}
		//pReader->Close();//test

		pReader->StopReaderHouseKeeping();

		SetEvent(waitThreadStop);

	}
	catch(mlsException& exc)
	{
		AfxMessageBox(exc.what(),MB_OK,0);
	}

	catch (...)
	{
		AfxMessageBox("Unhandle exception",MB_OK,0);		
	}

	return 0;
}
int unhexify(unsigned char *obuf, const char *ibuf)
{
	unsigned char c, c2;
	int len = strlen(ibuf) / 2;
	//assert(!(strlen(ibuf) %1)); // must be even number of bytes

	while (*ibuf != 0)
	{
		c = *ibuf++;
		if( c >= '0' && c <= '9' )
			c -= '0';
		else if( c >= 'a' && c <= 'f' )
			c -= 'a' - 10;
		else if( c >= 'A' && c <= 'F' )
			c -= 'A' - 10;
		/*else
			assert( 0 );*/

		c2 = *ibuf++;
		if( c2 >= '0' && c2 <= '9' )
			c2 -= '0';
		else if( c2 >= 'a' && c2 <= 'f' )
			c2 -= 'a' - 10;
		else if( c2 >= 'A' && c2 <= 'F' )
			c2 -= 'A' - 10;
		/*else
			assert( 0 );*/

		*obuf++ = ( c << 4 ) | c2;
	}

	return len;
}
CString convertBytetoBitString(unsigned char temp)
{
	CString strResult = "";
	char resultTemp;
	for(int i = 7;i >=0;i--)
	{
		if((temp >> i)&1)
			resultTemp = '1';
		else
			resultTemp = '0';
		strResult =strResult+ resultTemp;
	}
	return strResult;
}

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CmlsMFCCepasSamAppDlg dialog




CmlsMFCCepasSamAppDlg::CmlsMFCCepasSamAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CmlsMFCCepasSamAppDlg::IDD, pParent)
	, mv_InputValue(_T(""))
	, m_cstrCepasVerion(_T(""))
	, m_cstrPurseStatus(_T(""))
	, m_cstrPurseBalance(_T(""))
	, m_cstrAutoLoadAmount(_T(""))
	, m_cstrCAN(_T(""))
	, m_cstrCSN(_T(""))
	, m_cstrPurseCreationDate(_T(""))
	, m_cstrPurseExpiryDate(_T(""))
	, m_cstrLastCreditTransactionTRP(_T(""))
	, m_cstrLastCreditTransactionHeader(_T(""))
	, m_cstrNoofRecordinTransactionlofile(_T(""))
	, m_cstrIssuerSpecificDataLength(_T(""))
	, m_cstrLastTransactionTRP(_T(""))
	, m_cstrLastTransactionRecord(_T(""))
	, m_cstrIssuerSpecificData(_T(""))
	, m_cstrLastTransactionDebitOptionByte(_T(""))
	, m_cstrCEPASVesionDetail(_T(""))
	, m_cstrPurseStatusDetail(_T(""))
	, m_cstrPurseBalanceDetail(_T(""))
	, m_cstrAutoLoadAmountDetail(_T(""))
	, m_cstrPurseExpiryDateDetail(_T(""))
	, m_cstrPurseCreationDateDetail(_T(""))
	, m_cstrTransactionTypeDetail(_T(""))
	, m_cstrTransactionAmountDetail(_T(""))
	, m_cstrTransactionDaTime(_T(""))
	, m_cstrNoofRecordDetail(_T(""))
	, m_cstrIssuerSpecificDataLengthDetail(_T(""))
	, m_cstrLastTransactionDebitOptionDetail(_T(""))
	, m_cstrTransactionUserData(_T(""))
	, m_cstrValue(_T(""))
	, m_cstrPurseBanlance(_T(""))
	, m_cstrCommandStatus(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmlsMFCCepasSamAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CONTACTLESS, mc_ComboContactless);
	DDX_Control(pDX, IDC_COMBO_CONTACT, mc_ComboContact);
	DDX_Text(pDX, IDC_EDIT2, m_cstrCepasVerion);
	DDX_Text(pDX, IDC_EDIT3, m_cstrPurseStatus);
	DDX_Text(pDX, IDC_EDIT4, m_cstrPurseBalance);
	DDX_Text(pDX, IDC_EDIT5, m_cstrAutoLoadAmount);
	DDX_Text(pDX, IDC_EDIT8, m_cstrCAN);
	DDX_Text(pDX, IDC_EDIT7, m_cstrCSN);
	DDX_Text(pDX, IDC_EDIT9, m_cstrPurseCreationDate);
	DDX_Text(pDX, IDC_EDIT10, m_cstrPurseExpiryDate);
	DDX_Text(pDX, IDC_EDIT6, m_cstrLastCreditTransactionTRP);
	DDX_Text(pDX, IDC_EDIT11, m_cstrLastCreditTransactionHeader);
	DDX_Text(pDX, IDC_EDIT12, m_cstrNoofRecordinTransactionlofile);
	DDX_Text(pDX, IDC_EDIT13, m_cstrIssuerSpecificDataLength);
	DDX_Text(pDX, IDC_EDIT14, m_cstrLastTransactionTRP);
	DDX_Text(pDX, IDC_EDIT19, m_cstrLastTransactionRecord);
	DDX_Text(pDX, IDC_EDIT16, m_cstrIssuerSpecificData);
	DDX_Text(pDX, IDC_EDIT15, m_cstrLastTransactionDebitOptionByte);
	DDX_Text(pDX, IDC_EDIT17, m_cstrCEPASVesionDetail);
	DDX_Text(pDX, IDC_EDIT18, m_cstrPurseStatusDetail);
	DDX_Text(pDX, IDC_EDIT20, m_cstrPurseBalanceDetail);
	DDX_Text(pDX, IDC_EDIT21, m_cstrAutoLoadAmountDetail);
	DDX_Text(pDX, IDC_EDIT22, m_cstrPurseExpiryDateDetail);
	DDX_Text(pDX, IDC_EDIT23, m_cstrPurseCreationDateDetail);
	DDX_Text(pDX, IDC_EDIT25, m_cstrTransactionTypeDetail);
	DDX_Text(pDX, IDC_EDIT26, m_cstrTransactionAmountDetail);
	DDX_Text(pDX, IDC_EDIT27, m_cstrTransactionDaTime);
	DDX_Text(pDX, IDC_EDIT24, m_cstrNoofRecordDetail);
	DDX_Text(pDX, IDC_EDIT28, m_cstrIssuerSpecificDataLengthDetail);
	DDX_Text(pDX, IDC_EDIT29, m_cstrLastTransactionDebitOptionDetail);
	DDX_Control(pDX, IDC_EDIT19, m_ctlLastTransactionRecord);
	DDX_Text(pDX, IDC_EDIT30, m_cstrTransactionUserData);
	DDX_Control(pDX, IDC_BUTTON3, m_btDebit);
	DDX_Text(pDX, IDC_EDIT1, m_cstrValue);
	DDX_Text(pDX, IDC_EDIT35, m_cstrPurseBanlanceValue);
	DDX_Text(pDX, IDC_EDIT36, m_cstrCommandStatus);
	DDX_Control(pDX, IDC_LSTTRANLOG, m_clstTransactionLog);
}

BEGIN_MESSAGE_MAP(CmlsMFCCepasSamAppDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(UWM_CARDPRESENT, OnCardPresent)
	ON_MESSAGE(UWM_CARDABSENT, OnCardAbsent)
	ON_BN_CLICKED(ID_BUTTON_CREATE, &CmlsMFCCepasSamAppDlg::OnBnClickedButtonCreate)
	ON_BN_CLICKED(IDC_BUTTON_FORMAT, &CmlsMFCCepasSamAppDlg::OnBnClickedButtonFormat)
	ON_BN_CLICKED(ID_BUTTON_EXIT, &CmlsMFCCepasSamAppDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON3, &CmlsMFCCepasSamAppDlg::OnBnClickedBtnDebitCard)
	ON_CBN_SELCHANGE(IDC_COMBO_CONTACTLESS, &CmlsMFCCepasSamAppDlg::OnCbnSelchangeComboContactless)
END_MESSAGE_MAP()


// CmlsMFCCepasSamAppDlg message handlers

BOOL CmlsMFCCepasSamAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	mainhwnd = GetSafeHwnd();

#if defined (MLSLTASAM_MUTUALAUTHENTICATE_TEST)	
	mlsCard_str handleSAM;
	unsigned char abKey[16] = {0x00,0x01,0x1f,0x88,0x81,0x01,0x94,0x9b,
							   0x2e,0xb9,0xb1,0x4c,0xfd,0xe4,0x22,0x0f};
	mlsLtaSAM_MutualAuthenticateProcess(&handleSAM, abKey);
	OnCancel();
	return TRUE;
#endif

	UINT32 nrReader = mlsManagerObj.GetNrReader();

	if ( nrReader == 0)
	{
		MessageBox("No Reader found, please attach a PCSC Reader then OK", "Error");

		Sleep(2000);
		mlsManagerObj.Initialise();

		nrReader = mlsManagerObj.GetNrReader();

		if (nrReader == 0)
		{
			MessageBox("No reader found, The program will be exited", "Error");
			OnCancel();
			return TRUE;
		}
	}

	CString strSelReader;
	CStringArray astrListReaders;
	astrListReaders.SetSize(128);
	nrReader = mlsManagerObj.ListAllReader(astrListReaders);
	astrListReaders.SetSize(nrReader);
	CString strReaderName;
	for(int i=0;i<astrListReaders.GetSize();i++)
	{
		strReaderName = astrListReaders.GetAt(i);
#if 0
		if (strReaderName.Find(/*"STYL STY90x PICC Reader"*//*"Contactless Smartcard Reader"*/"Magneton CL SmartCard Reader")!=-1)
#endif
		{
			mc_ComboContactless.AddString(strReaderName);
		}
		//else if (strReaderName.Find("STYL STY90x PICC Reader")!=-1){
			mc_ComboContact.AddString(strReaderName);
		//}
	}
	mc_ComboContact.SetCurSel(0);
	mc_ComboContactless.SetCurSel(0);
	if(mc_ComboContactless.GetCount() <= 0)
	{
		MessageBox("No reader found, The program will be exited", "Error");
		OnCancel();
		return TRUE;
	}
	mc_ComboContactless.GetLBText(0,WORK_READER_NAME);


	mv_InputValue = "100";
	m_btDebit.EnableWindow(FALSE);
	m_cstrValue = "100";
	//init column of transactionlog record grid
	InitDataGrid();
	UpdateData(false);


	/*start Card Detect thread; there are 2 ways to detect card present: 
	1. call CardDetect directly 2.
	2. start Reader HouseKeeping thread and set notify event, based on 
	this event, the application process acordingly
	*/
	blCardDetectThreadRun = TRUE;


	AfxBeginThread(CardDetectUsingHKThread, mainhwnd);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CmlsMFCCepasSamAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CmlsMFCCepasSamAppDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CmlsMFCCepasSamAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CmlsMFCCepasSamAppDlg::OnCardPresent(WPARAM wParam, LPARAM lParam)
{
	
	mlsErrorCode_tp errorCode;
	CByteArray UID;
	mlsUtil uti;
	CString strUDI;
	CString strBalance;
	LONG32 outVal;
	char cValue[10];
	CString str;


	criticalsection.Lock();

	//Beep(1000,20);

	//mc_buttonFormat.EnableWindow(true);
	///************************************************************************/
	////set the contactess reader by the reader name in contactless reader combobox
	if (mc_ComboContactless.GetCurSel()==CB_ERR)
	{
		mv_Inform = "No contactless reader select!!!";
		UpdateData(false);
		criticalsection.Unlock();
		return 0;
	}
	mc_ComboContactless.GetLBText(mc_ComboContactless.GetCurSel(),WORK_READER_NAME);
	mlsReader* pReaderContactless= mlsManagerObj.GetReaderByName(WORK_READER_NAME);
	//mlsReader* pReaderContactless= mlsManagerObj.GetReaderByIndex(0);
	//mls@dev03 connect with protocol T = 1;
	ULONG proto = CARD_CONNECT_PROTOCOL_T1;
	errorCode = pReaderContactless->Connect(proto);

	if (errorCode!=SUCCESS)
	{
		criticalsection.Unlock();
		return 0;
	}

	if ((pReaderContactless->GetCardInfo().GetCardType()!= CARD_TYPE_EZCEPAS))
	{
		mv_Inform = "Invalid Card!";
		UpdateData(false);
		criticalsection.Unlock();
		return 0;
	}

	card_str->cardhandle = pReaderContactless->GetCardInfo().GetCardHandle();
	card_str->iorequest = pReaderContactless->GetCardInfo().GetPCIRequest();
	mlsReadPurseRecord readPurseRecord;
	unsigned short status;
	status = mlsCepasReadPurseNoAuthen(card_str,0x03,0x5f,&readPurseRecord);
	if(status == MLSCEPAS_SUCCESS)
	{
		displayReadPurseRecord(readPurseRecord);
		detailReadPurseRecord(readPurseRecord);
		//display transaction log record
		DisplayArrayTranLogRecord(readPurseRecord.ucNoOfRecord);
		//change state of button Read Transaction Log
		m_btDebit.EnableWindow(TRUE);
		UpdateData(FALSE);
	}
	
	///**********************************************************************/

	criticalsection.Unlock();

	//pReaderContact->Disconnect();

	//pReaderContact->Close();

	return 0;
}
//show data of read purse record
void CmlsMFCCepasSamAppDlg::displayReadPurseRecord(mlsReadPurseRecord readPurseRecord)
{
	unsigned char temp[100];
	CString cstrTemp = "",str1= "";
	//show CepasVerion data
	str1.Format(_T(" %02X"),readPurseRecord.ucCEPASVersion);
	m_cstrCepasVerion = str1;
	//show Purse status data
	str1.Format(_T(" %02X"),readPurseRecord.ucPurseStatus);
	m_cstrPurseStatus = str1;
	//show Purse balance data
	m_cstrPurseBalance = ByteArrayToHexString(readPurseRecord.arrPurseBalance,3);
	//show Auto Load Amount data
	m_cstrAutoLoadAmount = ByteArrayToHexString(readPurseRecord.arrAutoLoadAmount,3);
	//show CAN data
	m_cstrCAN = ByteArrayToHexString(readPurseRecord.arrCAN,8);
	//show CSN data
	m_cstrCSN = ByteArrayToHexString(readPurseRecord.arrCSN,8);
	//show Purse Expiry Date data
	m_cstrPurseExpiryDate = ByteArrayToHexString(readPurseRecord.arrPurseExpiryDate,2);
	//show Purse Creation Date data
	m_cstrPurseCreationDate = ByteArrayToHexString(readPurseRecord.arrPurseCreationDate,2);
	//show Last Credit Transaction TRP data
	m_cstrLastCreditTransactionTRP = ByteArrayToHexString(readPurseRecord.arrLastCreditTransactionTRP,4);
	//show Last Credit Transaction Header data
	m_cstrLastCreditTransactionHeader = ByteArrayToHexString(readPurseRecord.arrLastCreditTransactionHeader,8);
	//show No. of Record in transaction log file
	str1.Format(_T(" %02X"),readPurseRecord.ucNoOfRecord);
	m_cstrNoofRecordinTransactionlofile = str1;
	//show Issuer Specific data length
	str1.Format(_T(" %02X"),readPurseRecord.ucIssureSpecificDataLength);
	m_cstrIssuerSpecificDataLength = str1;
	// show last transaction TRP data
	m_cstrLastTransactionTRP = ByteArrayToHexString(readPurseRecord.arrLastTransactionTRP,4);
	//show last transaction record data
	str1 = ByteArrayToHexString(readPurseRecord.arrLastTransactionRecord,16);
	str1 = multilineEdit((unsigned char*)(LPCTSTR)str1,str1.GetLength(),33);
	m_cstrLastTransactionRecord = str1;
	//show issuer specific data

	if (readPurseRecord.ucIssureSpecificDataLength <= sizeof(readPurseRecord.arrIssurespecificData))
	{
		str1 = ByteArrayToHexString(readPurseRecord.arrIssurespecificData,readPurseRecord.ucIssureSpecificDataLength);
		str1 = multilineEdit((unsigned char*)(LPCTSTR)str1,str1.GetLength(),33);
		m_cstrIssuerSpecificData = str1;
	}
	else
	{
		m_cstrIssuerSpecificData="---";
	}
	//show last transaction debit option byte
	str1.Format(_T(" %02X"),readPurseRecord.ucLastTransactionDebitOptionsByte);
	m_cstrLastTransactionDebitOptionByte = str1;

}
void CmlsMFCCepasSamAppDlg::test(CString text)
{
	CString result = "sdfsd";
}
CString CmlsMFCCepasSamAppDlg::multilineEdit(unsigned char * text,int length,int number)
{
	CString result;
	for(int i = 0 ;i < length;i++)
	{
		result = result + (char)text[i];
		if((i + 1) % number == 0)
			result = result + "\r\n";
	}
	return result;
}
//show detail data of read purse record
void CmlsMFCCepasSamAppDlg::detailReadPurseRecord(mlsReadPurseRecord readPurseRecord)
{
	unsigned char temp[100];
	CString cstrTemp = "",str1= "";
	int iValueTemp;
	float fValue;
	int iYear;
	int iMonth;
	int iDay;
	mlsDateTime_t dateTime;

	//show CEPASVertion detail data
	str1.Format(_T(" %d"),readPurseRecord.ucCEPASVersion);
	m_cstrCEPASVesionDetail = str1;
	// show Purse Status detail data
	str1 = convertBytetoBitString(readPurseRecord.ucPurseStatus);
	//str1.Format(_T(" %d"),readPurseRecord.ucPurseStatus);
	m_cstrPurseStatusDetail = str1;
	//show Purse balance detail data
	mlsConvertThreeByteToInt(&iValueTemp,readPurseRecord.arrPurseBalance,0);
	fValue = (float)iValueTemp/100;
	//mls@dev01 str1.Format(_T(" %d"),iValueTemp);
	//mls@dev01 m_cstrPurseBalanceDetail = str1;
	str1.Format(_T(" %.2f"),fValue);
	m_cstrPurseBalanceDetail = str1;
	//show AutoLoad Amount detail data
	mlsConvertThreeByteToInt(&iValueTemp,readPurseRecord.arrAutoLoadAmount,0);
	str1.Format(_T(" %d"),iValueTemp);
	m_cstrAutoLoadAmountDetail = str1;

	//show Purse Expiry detail data
	mlsConvertTwoByteToInt(&iValueTemp,readPurseRecord.arrPurseExpiryDate);
	mlsUnixTimeToGregorianTime((iValueTemp + 9131) * 86400, &dateTime);
	m_cstrPurseExpiryDateDetail = "";
	m_cstrPurseExpiryDateDetail.AppendFormat("%02d", dateTime.dayOfMonth);
	m_cstrPurseExpiryDateDetail.AppendFormat("/%02d", dateTime.month);
	m_cstrPurseExpiryDateDetail.AppendFormat("/%04d", dateTime.year);

	//show Purse Creation Date detail data
	mlsConvertTwoByteToInt(&iValueTemp,readPurseRecord.arrPurseCreationDate);
	mlsUnixTimeToGregorianTime((iValueTemp + 9131) * 86400, &dateTime);
	m_cstrPurseCreationDateDetail = "";
	m_cstrPurseCreationDateDetail.AppendFormat("%02d", dateTime.dayOfMonth);
	m_cstrPurseCreationDateDetail.AppendFormat("/%02d", dateTime.month);
	m_cstrPurseCreationDateDetail.AppendFormat("/%04d", dateTime.year);

	//show transaction type detail data
	//str1.Format(_T(" %d"),readPurseRecord.arrLastTransactionRecord[0]);
	m_cstrTransactionTypeDetail = mlsTransactionType(readPurseRecord.arrLastTransactionRecord[0]);
	//show transaction amount detail data
	mlsConvertToIntKeepNegative(&iValueTemp,readPurseRecord.arrLastTransactionRecord,1);
	//mls@dev02 mlsConvertThreeByteToInt(&iValueTemp,readPurseRecord.arrLastTransactionRecord,1);

	fValue = (float)iValueTemp/100;
	//mls@dev01 str1.Format(_T(" %d"),iValueTemp);
	//mls@dev01 m_cstrTransactionAmountDetail = str1;

	str1.Format(_T(" %.2f"),(fValue));
	m_cstrTransactionAmountDetail = str1;

	//show transaction data time detail data
	mlsConvertFourByteToInt(&iValueTemp,readPurseRecord.arrLastTransactionRecord,4);
	mlsUnixTimeToGregorianTime(iValueTemp + 9131 * 86400, &dateTime);

	m_cstrTransactionDaTime = "";
	m_cstrTransactionDaTime.AppendFormat("%02d", dateTime.dayOfMonth);
	m_cstrTransactionDaTime.AppendFormat("/%02d", dateTime.month);
	m_cstrTransactionDaTime.AppendFormat("/%04d", dateTime.year);
	m_cstrTransactionDaTime.AppendFormat(" %02d", dateTime.hour);
	m_cstrTransactionDaTime.AppendFormat(":%02d", dateTime.minute);
	m_cstrTransactionDaTime.AppendFormat(":%02d", dateTime.second);

	//show transaction user data
	str1 = ByteArrayToHexString(&(readPurseRecord.arrLastTransactionRecord[8]),8);
	str1 = multilineEdit((unsigned char*)(LPCTSTR)str1,str1.GetLength(),24);
	m_cstrTransactionUserData = str1;
	//show number of record detail
	str1.Format(_T(" %d"),readPurseRecord.ucNoOfRecord);
	m_cstrNoofRecordDetail = str1;

	//show issuer specific data length detail 
	str1.Format(_T(" %d"),readPurseRecord.ucIssureSpecificDataLength);
	m_cstrIssuerSpecificDataLengthDetail = str1;
	str1.Format(_T(" %d"),readPurseRecord.ucLastTransactionDebitOptionsByte);
	m_cstrLastTransactionDebitOptionDetail = str1;

}
void CmlsMFCCepasSamAppDlg::clearReadPurseRecord()
{
	//clear text box of read purse record
	m_cstrCepasVerion = "";
	m_cstrPurseStatus = "";
	m_cstrPurseBalance = "";
	m_cstrAutoLoadAmount = "";
	m_cstrCAN = "";
	m_cstrCSN = "";
	m_cstrPurseExpiryDate = "";
	m_cstrPurseCreationDate = "";
	m_cstrLastCreditTransactionTRP = "";
	m_cstrLastCreditTransactionHeader = "";
	m_cstrNoofRecordinTransactionlofile = "";
	m_cstrIssuerSpecificDataLength = "";
	m_cstrLastTransactionTRP = "";
	m_cstrLastTransactionRecord = "";
	m_cstrIssuerSpecificData = "";
	m_cstrLastTransactionDebitOptionByte = "";
	//clear text box detail of read purse record
	m_cstrCEPASVesionDetail = "";
	m_cstrPurseStatusDetail = "";
	m_cstrPurseBalanceDetail = "";
	m_cstrAutoLoadAmountDetail = "";
	m_cstrPurseExpiryDateDetail = "";
	m_cstrPurseCreationDateDetail = "";
	m_cstrTransactionTypeDetail = "";
	m_cstrTransactionAmountDetail = "";
	m_cstrTransactionDaTime = "";
	m_cstrNoofRecordDetail = "";
	m_cstrIssuerSpecificDataLengthDetail = "";
	m_cstrLastTransactionDebitOptionDetail = "";
	m_cstrTransactionUserData = "";
	//status
	m_cstrCommandStatus = "";


}
//convert byte array to hexa string
CString CmlsMFCCepasSamAppDlg::ByteArrayToHexString(unsigned char *byteArr,int length)
{	
	//char* cHex = new char();
	CString str,str1 = "";
	for(int i=0;i<length;i++){
		str1.Format(_T(" %02X"),byteArr[i]);
		str +=str1;
	}
	//delete cHex;	
	return str;
}
LRESULT CmlsMFCCepasSamAppDlg::OnCardAbsent(WPARAM wParam, LPARAM lParam)
{
	//Beep(500,100);
	mv_Balance="";
	mv_Inform="Card Absent!";
	clearReadPurseRecord();
	m_btDebit.EnableWindow(FALSE);
	m_clstTransactionLog.DeleteAllItems();
	UpdateData(FALSE);
	return 0;
}

void CmlsMFCCepasSamAppDlg::OnBnClickedButtonCreate()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString temp;
	m_cstrLastTransactionRecord = "dfadfasdf";
	m_cstrLastTransactionRecord +="\r\n";
	m_cstrLastTransactionRecord += "sfsdf";
	UpdateData(FALSE);
}

void CmlsMFCCepasSamAppDlg::OnBnClickedButtonFormat()
{
	// TODO: Add your control notification handler code here
}

void CmlsMFCCepasSamAppDlg::OnBnClickedButtonExit()
{
	// TODO: Add your control notification handler code here
}

LONG CmlsMFCCepasSamAppDlg::getHandleSAMCard()
{
	mlsErrorCode_tp errorCode;
	CString CONTACT_READER_NAME;
	if (mc_ComboContact.GetCurSel()==CB_ERR)
	{
		m_cstrCommandStatus="No contact reader select!!!";
		UpdateData(false);
		return 0;
	}
	mc_ComboContact.GetLBText(mc_ComboContact.GetCurSel(),CONTACT_READER_NAME);
	mlsReader* pReaderContact = mlsManagerObj.GetReaderByName(CONTACT_READER_NAME);

	/*****************************************************************/
	//Connect to SAM and create mlsSAM object
	errorCode = pReaderContact->Open();

	if (errorCode!=SUCCESS && errorCode != READER_OPENED)
	{
		return 0;
	}

	//mls@dev01 errorCode = pReaderContact->Connect();
	ULONG proto = CARD_CONNECT_PROTOCOL_T0;
	errorCode = pReaderContact->Connect(proto);
	if (errorCode!=SUCCESS)
	{
		m_cstrCommandStatus = "Can not connect to the SAM card!";
		UpdateData(false);
		return 0;
	}
	cardSam_str->cardhandle = pReaderContact->GetCardInfo().GetCardHandle();
	cardSam_str->iorequest = pReaderContact->GetCardInfo().GetPCIRequest();
	return 1;
	//pSAMCard = (mlsSAM*)pReaderContact->GetCard();
}
void CmlsMFCCepasSamAppDlg::OnBnClickedBtnDebitCard()
{
	// TODO: Add your control notification handler code here
	unsigned short status;
	CString strTemp = "";
#ifndef MLS_TESTSETTLEMENT
	if(getHandleSAMCard())
	{
#endif
		SYSTEMTIME currentTime;
		SAM_CEPAS_t sam = ezSAM;
		unsigned char ucPf=EZLINK_PURSE_FILE_NR;
		unsigned char ucTRP[4]={0x00,0x00,0x00,0x00};
		unsigned char ucDebitOption=EZLINK_DEBIT_OPTION_DEDUCTPURSE|\
									EZLINK_DEBIT_OPTION_DISABLEEXPIREYCHECKING_MASK ;
		unsigned char arrTransactionUserData[8]={'W','A','Y','-','D','B','T',' '};
		KeyIndex debitKey1;
		mlsDebugLogDateTimeInfo(">>>>>>>>>START DEBIT SESSION>>>>>>>>>>>\n");
		debitKey1.ucKeyFileID = EZLINK_DEBIT_KEY_SFI;
		debitKey1.ucKeyNo = EZLINK_DEBIT_KEY_NR;
		KeyIndex signingKey;

		signingKey.ucKeyFileID = EZLINK_SIGNING_KEY_SFI;
		signingKey.ucKeyNo = EZLINK_SIGNING_KEY_NR;
		long plAmout;
		int iPurseBalance;
		int IssuerId = 0x04;
		UpdateData(TRUE);
		plAmout = strtol(m_cstrValue,NULL,10);
		if(cepasMDBatchFile == NULL)
		{
			mlsDebugLogSrcInfo("Creating Settlement Batch File\n");
			cepasMDBatchFile = (CepasMDBatchFile *)malloc(sizeof(CepasMDBatchFile));
			GetLocalTime(&currentTime);
			mlsCepasCreateMDBatchFile(currentTime,cepasMDBatchFile);
			createNameofMBBatchFileFormat(currentTime);
		}else{
			if(checkFileSizeOrRecordNumber(cepasMDBatchFile)){
				mlsDebugLogSrcInfo("Old Settlement Batch file is full, create a new Batch File\n");
				freeMemoryOfCepasMDBatchFile(cepasMDBatchFile);
				cepasMDBatchFile = (CepasMDBatchFile *)malloc(sizeof(CepasMDBatchFile));
				GetLocalTime(&currentTime);
				iFileSequenceID++;
				mlsCepasCreateMDBatchFile(currentTime,cepasMDBatchFile);
				createNameofMBBatchFileFormat(currentTime);
			}
		}
		MDTransactionRecord *mdTransRecord = (MDTransactionRecord *)malloc(sizeof(MDTransactionRecord));

#ifndef MLS_TESTSETTLEMENT
		status = mlsCepasDebitSAM(card_str,cardSam_str,ucPf,ucTRP,ucDebitOption,
								  arrTransactionUserData,debitKey1,signingKey,
								  plAmout,&iPurseBalance);
#else

		status = mlsCepasDebitSAMAndCreTransRec(
			card_str,
			cardSam_str, 
			sam,
			ucPf,
			ucTRP,
			ucDebitOption,
			arrTransactionUserData,
			debitKey1,
			signingKey,
			plAmout,
			&iPurseBalance,
			IssuerId,
			mdTransRecord);
#endif

		if(status == MLSCEPAS_SUCCESS)
		{
			mlsDebugLogSrcInfo("Debit OK\n");			
			mlsAddRecordtoMBBatchFile(cepasMDBatchFile,mdTransRecord);
			mlsWriteAllMDBatchFile((CHAR *)PATHFILE,cepasMDBatchFile);
			strTemp.Format(_T(" %d"),iPurseBalance);
			m_cstrCommandStatus = "Success";
		}else{
			m_cstrCommandStatus = getStringError(status);
		}

		//update UI after debit; ReadPurseRecord
		mlsReadPurseRecord readPurseRecord;
		unsigned short statusReadPurse;
		statusReadPurse = mlsCepasReadPurseNoAuthen(card_str,0x03,0x5f,&readPurseRecord);
		if(statusReadPurse == MLSCEPAS_SUCCESS)
		{
			displayReadPurseRecord(readPurseRecord);
			detailReadPurseRecord(readPurseRecord);
			//display transaction log record
			DisplayArrayTranLogRecord(readPurseRecord.ucNoOfRecord);
		}
			

		UpdateData(FALSE);
#ifndef MLS_TESTSETTLEMENT
	}
	else
	{
		mlsDebugLog("getHandleSAMCard ERROR - check your SAM interface\n");
	}
#endif
}

void CmlsMFCCepasSamAppDlg::InitDataGrid()
{
	CRect rect;
	m_clstTransactionLog.GetClientRect(&rect);
	int nColInterval = rect.Width()/6;

	m_clstTransactionLog.InsertColumn(0, _T("S/No"), LVCFMT_LEFT, nColInterval*0.5);
	m_clstTransactionLog.InsertColumn(1, _T("Date"), LVCFMT_LEFT, nColInterval*1);
	m_clstTransactionLog.InsertColumn(2, _T("Time"), LVCFMT_LEFT, nColInterval*1);
	m_clstTransactionLog.InsertColumn(3, _T("Txn. Type"), LVCFMT_LEFT, nColInterval*1);
	m_clstTransactionLog.InsertColumn(4, _T("Amount"), LVCFMT_LEFT, nColInterval*1);
	m_clstTransactionLog.InsertColumn(5, _T("Description"), LVCFMT_LEFT, nColInterval*1.4);
	m_clstTransactionLog.SetAltRowColors(true,RGB(240,224,230),RGB(153,153,102));
	m_clstTransactionLog.SetGridMode(true,RGB(0,0,0),RGB(255,255,255));
}
void CmlsMFCCepasSamAppDlg::DisplayArrayTranLogRecord(int iRecordNumber)
{
	/*unsigned char arrData[368] = {0x76 ,0x00 ,0x00 ,0x32 ,0x1C ,0x1F ,0x40 ,0xD5 ,0x53 ,0x56 ,0x43 ,0x20 ,0x31 ,0x33 ,0x39 ,0x20,
		0x31 ,0xFF ,0xFF ,0x73 ,0x1C ,0x1F ,0x3C ,0x9E ,0x53 ,0x56 ,0x43 ,0x20 ,0x31 ,0x33 ,0x39 ,0x20,
		0x30 ,0xFF ,0xFF ,0x9C ,0x1B ,0x5D ,0x8E ,0xBE ,0x43 ,0x54 ,0x48 ,0x2D ,0x54 ,0x41 ,0x50 ,0x20,
		0x30 ,0xFF ,0xFF ,0xBC ,0x1B ,0x5D ,0x39 ,0x80 ,0x4F ,0x52 ,0x43 ,0x2D ,0x43 ,0x54 ,0x48 ,0x20,
		0x30 ,0xFF ,0xFF ,0xB0 ,0x1B ,0x5D ,0x11 ,0xFC ,0x54 ,0x41 ,0x50 ,0x2D ,0x4F ,0x52 ,0x43 ,0x20,
		0x30 ,0xFF ,0xFF ,0x6F ,0x1B ,0x5C ,0x5E ,0xFD ,0x48 ,0x42 ,0x46 ,0x2D ,0x42 ,0x44 ,0x4C ,0x20,
		0x30 ,0xFF ,0xFF ,0x97 ,0x1B ,0x5C ,0x1B ,0x89 ,0x4D ,0x52 ,0x42 ,0x2D ,0x48 ,0x42 ,0x46 ,0x20,
		0x30 ,0xFF ,0xFF ,0x68 ,0x1B ,0x5C ,0x14 ,0x26 ,0x42 ,0x4E ,0x4C ,0x2D ,0x4D ,0x52 ,0x42 ,0x20,
		0x31 ,0xFF ,0xFF ,0xBB ,0x1B ,0x5B ,0xED ,0xF7 ,0x53 ,0x56 ,0x43 ,0x20 ,0x31 ,0x39 ,0x34 ,0x20,
		0x76 ,0x00 ,0x00 ,0x2A ,0x1B ,0x5B ,0xB6 ,0xFA ,0x53 ,0x56 ,0x43 ,0x20 ,0x31 ,0x39 ,0x34 ,0x20,
		0x31 ,0xFF ,0xFF ,0xC3 ,0x1B ,0x5B ,0xB5 ,0x06 ,0x53 ,0x56 ,0x43 ,0x20 ,0x31 ,0x39 ,0x34 ,0x20,
		0x30 ,0xFF ,0xFF ,0x5E ,0x1B ,0x5B ,0xB3 ,0xED ,0x42 ,0x44 ,0x4C ,0x2D ,0x42 ,0x4E ,0x4C ,0x20,
		0x75 ,0x00 ,0x03 ,0xE8 ,0x1B ,0x5B ,0xA7 ,0x78 ,0x42 ,0x44 ,0x4C ,0x20 ,0x47 ,0x54 ,0x4D ,0x00,
		0x30 ,0xFF ,0xFF ,0xA6 ,0x1B ,0x5A ,0xFF ,0x3B ,0x4F ,0x52 ,0x43 ,0x2D ,0x42 ,0x44 ,0x4C ,0x20,
		0x30 ,0xFF ,0xFF ,0xBC ,0x1B ,0x5A ,0xE2 ,0x06 ,0x43 ,0x54 ,0x48 ,0x2D ,0x4F ,0x52 ,0x43 ,0x20,
		0x30 ,0xFF ,0xFF ,0x9C ,0x1B ,0x5A ,0xAA ,0xD9 ,0x42 ,0x44 ,0x4C ,0x2D ,0x43 ,0x54 ,0x48 ,0x20,
		0x30 ,0xFF ,0xFF ,0x7E ,0x1B ,0x59 ,0xBE ,0xE3 ,0x42 ,0x44 ,0x4C ,0x2D ,0x41 ,0x4C ,0x4A ,0x20,
		0x30 ,0xFF ,0xFF ,0x9C ,0x1B ,0x55 ,0xC1 ,0xBB ,0x44 ,0x42 ,0x4E ,0x2D ,0x42 ,0x44 ,0x4C ,0x20,
		0x30 ,0xFF ,0xFF ,0xA6 ,0x1B ,0x54 ,0x72 ,0xD3 ,0x4F ,0x52 ,0x43 ,0x2D ,0x42 ,0x44 ,0x4C ,0x20,
		0x30 ,0xFF ,0xFF ,0xBC ,0x1B ,0x54 ,0x5A ,0x21 ,0x43 ,0x54 ,0x48 ,0x2D ,0x4F ,0x52 ,0x43 ,0x20,
		0x30 ,0xFF ,0xFF ,0x9C ,0x1B ,0x54 ,0x48 ,0x7D ,0x42 ,0x44 ,0x4C ,0x2D ,0x43 ,0x54 ,0x48 ,0x20,
		0x75 ,0x00 ,0x03 ,0xE8 ,0x1B ,0x54 ,0x44 ,0x0C ,0x42 ,0x44 ,0x4C ,0x20 ,0x50 ,0x53 ,0x43 ,0x20,
		0xF0 ,0x01 ,0x00 ,0x00 ,0x1B ,0x54 ,0x44 ,0x0C ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};*/
	//unsigned char arrTemp[16];
	m_clstTransactionLog.DeleteAllItems();
	for(int i = 0;i < iRecordNumber;i++)
	{
		unsigned char pbResponseBuffer[16];
		short length;
		mlsCepasReadTransactionLog(card_str,0x03,i,0x10,pbResponseBuffer,&length);
		//memcpy(arrTemp,arrData+i*16,16);
		if(length != NULL && length == 16)
			InsertItems(pbResponseBuffer,i);
		//arrData = arrData + 16;
	}
}
void CmlsMFCCepasSamAppDlg::InsertItems(BYTE *data,int iNumber)
{
	/*todo:mls@dev01 to review*/
	// Delete the current contents
	//mc_ListView.DeleteAllItems();

	// Use the LV_ITEM structure to insert the items
	LVITEM lvi;
	CString strItem;
	int iSecondSum;
	int iValueTemp;
	int iYear;
	int iMonth;
	int iDay;
	int iHour;
	int iMinute;
	int iSecond;
	int i = 0;
	BYTE arrUserData[9];
	CString strTemp;
	// Insert the first item
	lvi.mask =  LVIF_IMAGE | LVIF_TEXT;
	lvi.iItem = iNumber;
	lvi.iSubItem = 0;
	strItem.Format(_T(" %d"),iNumber + 1);
	lvi.pszText = (LPTSTR)(LPCTSTR)strItem;
	//lvi.iImage = i%8;		// There are 8 images in the image list
	m_clstTransactionLog.InsertItem(&lvi);

	//// Set subitem 1
	mlsDateTime_t txnTime;
	mlsConvertFourByteToInt(&iSecondSum,data,4);
	mlsUnixTimeToGregorianTime(iSecondSum + 9131 * 86400, &txnTime);
	strItem = "";
	strItem.AppendFormat("%02d", txnTime.dayOfMonth);
	strItem.AppendFormat("/%02d", txnTime.month);
	strItem.AppendFormat("/%04d", txnTime.year);
	lvi.iSubItem =1;
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_clstTransactionLog.SetItem(&lvi);

	strItem = "";
	strItem.AppendFormat("%02d", txnTime.hour);
	strItem.AppendFormat(":%02d", txnTime.minute);
	strItem.AppendFormat(":%02d", txnTime.second);
	lvi.iSubItem =2;
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_clstTransactionLog.SetItem(&lvi);


	//// Set subitem 3
	lvi.iItem = iNumber;
	lvi.iSubItem = 3;
	strItem = mlsTransactionType(data[0]);
	lvi.pszText = (LPTSTR)(LPCTSTR)strItem;
	m_clstTransactionLog.SetItem(&lvi);
	//// Set subitem 4
	mlsConvertToIntKeepNegative(&iValueTemp,data,1);
	double dTemp = abs(iValueTemp);
	dTemp = dTemp/100;
	strItem.Format(_T(" %.2f"),dTemp);
	if(iValueTemp < 0)
		strItem = "-S$"+strItem;
	else
		if(iValueTemp > 0)
			strItem = "+S$"+strItem;
		else
			strItem = " S$"+strItem;

	lvi.iSubItem =4;
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_clstTransactionLog.SetItem(&lvi);

	//// Set subitem 5
	memcpy(arrUserData,data+8,8);
	arrUserData[8]='\0';
	CString strUserDataTemp((char*)arrUserData);
	lvi.iSubItem =5;
	lvi.pszText = (LPTSTR)(LPCTSTR)(strUserDataTemp);
	m_clstTransactionLog.SetItem(&lvi);

}


void CmlsMFCCepasSamAppDlg::OnCbnSelchangeComboContactless()
{
	blCardDetectThreadRun = FALSE;

	SetEvent(EvCardDetect);

	WaitForSingleObject(waitThreadStop, INFINITE);

	mlsReader* pReaderContactless = mlsManagerObj.GetReaderByName(CString(WORK_READER_NAME));
	pReaderContactless->Close();

	mc_ComboContactless.GetLBText(mc_ComboContactless.GetCurSel(),WORK_READER_NAME);
	
	//restart dialog
	mv_Inform="Reader change!";
	clearReadPurseRecord();
	m_btDebit.EnableWindow(FALSE);
	m_clstTransactionLog.DeleteAllItems();
	UpdateData(FALSE);

	//start card detect thread again
	blCardDetectThreadRun = TRUE;
	AfxBeginThread(CardDetectUsingHKThread, mainhwnd);
}
