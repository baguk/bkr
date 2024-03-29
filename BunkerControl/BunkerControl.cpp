// BunkerControl.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <ctime>
#include <iostream>
#include "BunkerControl.h"
#include "MainFrm.h"
#include "vzBox_converted.h"
#include "Events.h"

//#include <mmsystem.h>
#include <math.h>
#include <Dbt.h>
#include <Setupapi.h>
//#include <IoAPI.h>
extern "C"
{
#include "Hidsdi.h"
}
#define ENGLISH    1
#define RUSSIAN    2
#if RUS_VERSION
UINT language = RUSSIAN;
#else
UINT language = ENGLISH;
#endif

#define BEEP_PULL        0xFFFFFFFF
#define BEEP_RELEASED    0xFFFFFFFF
#define BEEP_NOTARGET    MB_ICONERROR

// CBunkerControlApp

BEGIN_MESSAGE_MAP(CBunkerControlApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CBunkerControlApp::OnAppAbout)
	ON_COMMAND(ID_LOAD_FIRMWARE, &CBunkerControlApp::OnAppLoadFirmware)	
	ON_COMMAND(ID_CONVERT_FIRMWARE, &CBunkerControlApp::OnAppConvertFirmware)	
	ON_COMMAND(ID_PROGRAM_FIRMWARE, &CBunkerControlApp::OnAppProgramFirmware)	
	ON_COMMAND(ID_RUN_BOOTLOADER, &CBunkerControlApp::OnAppRunBootloader)	
	ON_COMMAND(ID_RESET_BOX, &CBunkerControlApp::OnAppResetBox)
	ON_COMMAND(ID_DRAW_SHOOTERS, &CBunkerControlApp::OnAppDrawShooters)	
	// Standard file based document commands
	//ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	//ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
    // Start Bunker Control setup command
	ON_COMMAND(ID_DTQ_NEW, &CBunkerControlApp::OnDtq)
	ON_COMMAND(ID_DTF_NEW, &CBunkerControlApp::OnDtf)
	ON_COMMAND(ID_DTM_NEW, &CBunkerControlApp::OnDtm)
	ON_COMMAND(ID_DSHOW,   &CBunkerControlApp::OnDshow)
	ON_COMMAND(ID_TSHOW,   &CBunkerControlApp::OnTshow)
	ON_COMMAND(ID_DSHOOTOFF,&CBunkerControlApp::OnDshootoff)
	ON_COMMAND(ID_TSHOOTOFF,&CBunkerControlApp::OnTshootoff)

	ON_COMMAND(ID_TQ_NEW,  &CBunkerControlApp::OnTq)
	ON_COMMAND(ID_TF_NEW,  &CBunkerControlApp::OnTf)
	ON_COMMAND(ID_TM_NEW,  &CBunkerControlApp::OnTm)
	ON_COMMAND(ID_TEST_NEW,&CBunkerControlApp::OnTest)
	ON_COMMAND(ID_TOP_NEW, &CBunkerControlApp::OnTop)
	ON_COMMAND(ID_DOP_NEW, &CBunkerControlApp::OnDop)

	ON_COMMAND(ID_COMPAK_PRESET,  &CBunkerControlApp::OnComPreset)
	ON_COMMAND(ID_COMPAK_CONTROL, &CBunkerControlApp::OnComControl)
	ON_COMMAND(ID_COMPAK_TEST,  &CBunkerControlApp::OnComTest)

END_MESSAGE_MAP()


// CBunkerControlApp construction

CBunkerControlApp::CBunkerControlApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CBunkerControlApp object

CBunkerControlApp theApp;
// This identifier was generated to be statistically unique for your app
// You may change it if you prefer to choose a specific identifier

// {1EFC2765-FF42-46D2-AAC2-BD465DD34AB0}
static const CLSID clsid =
{ 0x1EFC2765, 0xFF42, 0x46D2, { 0xAA, 0xC2, 0xBD, 0x46, 0x5D, 0xD3, 0x4A, 0xB0 } };

const GUID CDECL BASED_CODE _tlid =
		{ 0xDEA0A2C8, 0xB7D2, 0x4BB0, { 0xA6, 0xD8, 0xA0, 0x2F, 0x1F, 0x80, 0x6A, 0x34 } };
//const WORD _wVerMajor = 1;
//const WORD _wVerMinor = 0;

// VZ - start
const wchar_t monthes[12][5] = { L"Jan", L"Feb", L"Mar", L"Apr", L"May", L"Jun",
                                 L"Jul", L"Aug", L"Sep", L"Oct", L"Nov", L"Dec"};

const wchar_t double_targets[DTQ_MACHINES][MAX_TRAP_NAME_LEN] = { L"A (7-8)", L"B (8-9)", L"C (7-9)" };  

const wchar_t stat_dirs_double[DTQ_MACHINES][MAX_TRAP_NAME_LEN] = { L"\\|", L"|/", L"\\/"};  
const wchar_t stat_dirs_single[TQ_MACHINES][MAX_TRAP_NAME_LEN] = 
{ L"/", L"|", L"\\",
  L"/", L"|", L"\\",                 
  L"/", L"|", L"\\",                 
  L"/", L"|", L"\\",                 
  L"/", L"|", L"\\" };

const UCHAR stat_directions[7] = { '-', '\\', '|', '/', 'A', 'B', 'C' };  

const UINT dtq_options[DTQ_OPTIONS][DTQ_SHOTS_PER_SHOOTER/DTQ_STATIONS] = 
   {{1,2,3},{1,3,2},{2,1,3},{2,3,1},{3,1,2},{3,2,1}}; 

const UINT dtf_options[DTM_OPTIONS][DTM_SHOTS_PER_SHOOTER/DTM_STATIONS] = 
   {{1,1,2,2,3},{1,1,2,3,2},{1,1,3,2,2},{1,2,1,2,3},{1,2,1,3,2},{1,2,2,1,3},
    {1,2,2,3,1},{1,2,3,1,2},{1,2,3,2,1},{1,3,1,2,2},{1,3,2,1,2},{1,3,2,2,1},
    {2,1,1,2,3},{2,1,1,3,2},{2,1,2,1,3},{2,1,2,3,1},{2,1,3,1,2},{2,1,3,2,1},
    {2,2,1,1,3},{2,2,1,3,1},{2,2,3,1,1},{2,3,1,1,2},{2,3,1,2,1},{2,3,2,1,1},
    {3,1,1,2,2},{3,1,2,1,2},{3,1,2,2,1},{3,2,1,1,2},{3,2,1,2,1},{3,2,2,1,1}}; 

const wchar_t trap_targets[TQ_MACHINES][MAX_TRAP_NAME_LEN] = { MLSTR_TR01, MLSTR_TR02, MLSTR_TR03, 
                                                               MLSTR_TR04, MLSTR_TR05, MLSTR_TR06,
                                                               MLSTR_TR07, MLSTR_TR08, MLSTR_TR09,
                                                               MLSTR_TR10, MLSTR_TR11, MLSTR_TR12,
                                                               MLSTR_TR13, MLSTR_TR14, MLSTR_TR15 };  

const UINT tq_options[TQ_OPTIONS][TQ_SHOTS_PER_SHOOTER/TQ_STATIONS] = 
   {{1,1,2,3,3},{1,1,3,2,3},{1,1,3,3,2},{1,2,1,3,3},{1,2,3,1,3},{1,2,3,3,1},
    {1,3,1,2,3},{1,3,1,3,2},{1,3,2,1,3},{1,3,2,3,1},{1,3,3,1,2},{1,3,3,2,1},
    {2,1,1,3,3},{2,1,3,1,3},{2,1,3,3,1},{2,3,1,1,3},{2,3,1,3,1},{2,3,3,1,1},
    {3,1,1,2,3},{3,1,1,3,2},{3,1,2,1,3},{3,1,2,3,1},{3,1,3,1,2},{3,1,3,2,1},
    {3,2,1,1,3},{3,2,1,3,1},{3,2,3,1,1},{3,3,1,1,2},{3,3,1,2,1},{3,3,2,1,1}}; 

const UINT ts_options[TS_OPTIONS][TM_SHOTS_PER_SHOOTER/TQ_STATIONS] = 
   {{1,2,3},{1,3,2},{2,1,3},{2,3,1},{3,1,2},{3,2,1}};

const UINT tf_options[TM_OPTIONS][TM_SHOTS_PER_SHOOTER/TM_STATIONS] = 
   {{1,1,2,3,3},{1,1,3,2,3},{1,1,3,3,2},{1,2,1,3,3},{1,2,3,1,3},{1,2,3,3,1},
    {1,3,1,2,3},{1,3,1,3,2},{1,3,2,1,3},{1,3,2,3,1},{1,3,3,1,2},{1,3,3,2,1},
    {2,1,1,3,3},{2,1,3,1,3},{2,1,3,3,1},{2,3,1,1,3},{2,3,1,3,1},{2,3,3,1,1},
    {3,1,1,2,3},{3,1,1,3,2},{3,1,2,1,3},{3,1,2,3,1},{3,1,3,1,2},{3,1,3,2,1},
    {3,2,1,1,3},{3,2,1,3,1},{3,2,3,1,1},{3,3,1,1,2},{3,3,1,2,1},{3,3,2,1,1}}; 

const wchar_t window_title[ROUND_TYPE_MAX][80] = {MLSTR_FNAME_UNKNOWN,
                                                  MLSTR_FNAME_DT_QUALIFICATION,
                                                  MLSTR_FNAME_DT_SEMIFINAL,
                                                  MLSTR_FNAME_DT_MATCH,
                                                  MLSTR_FNAME_T_QUALIFICATION,
                                                  MLSTR_FNAME_T_SEMIFINAL,
                                                  MLSTR_FNAME_T_MATCH, 
                                                  MLSTR_FNAME_FULL_MANUAL,
                                                  MLSTR_FNAME_T_ONE_STATION,
                                                  MLSTR_FNAME_DT_ONE_STATION,
												  MLSTR_FNAME_T_SHOW,
												  MLSTR_FNAME_D_SHOW,
                                                  MLSTR_FNAME_T_SHOOTOFF,
												  MLSTR_FNAME_D_SHOOTOFF,
                                                  MLSTR_FNAME_COMPAC_PRESET,
												  MLSTR_FNAME_COMPAC_CONTROL,
												  MLSTR_FNAME_COMPAK_MANUAL };

const wchar_t file_name[ROUND_TYPE_MAX][80] = {L"Unknown",
                                               L"Double Trap Qualification",
                                               L"Double Trap SemiFinal",
                                               L"Double Trap Medal Match",
                                               L"Trap Qualification",
                                               L"Trap SemiFinal",
                                               L"Trap Medal Match",
                                               L"Bunker Manual Control",
                                               L"Trap One Station Training",
                                               L"Double One Station Training",
                                               L"Trap Show Targets",
                                               L"Double Trap Show Targets",
                                               L"Trap Shoot-off",
                                               L"Double Trap Shoot-off",
	                                           L"Compak FITASC Table",
											   L"Compak Special Setup",
											   L"Compak Manual Control"};

const UINT max_targets[ROUND_TYPE_MAX] = 
   {0, DTQ_TARGETS, 6*15, DTM_TARGETS, TQ_TARGETS,  6*15,  TM_TARGETS,   TQ_TARGETS, 25*5, 15*5,15, 3, 25*6, 15*6};

const UINT max_shooters[ROUND_TYPE_MAX] = 
   {0, DTQ_SHOOTERS, 6, DTM_SHOOTERS, TQ_SHOOTERS,  TQ_SHOOTERS, TM_SHOOTERS,  24, 5, 5, 1, 1};
const UINT min_shooters[ROUND_TYPE_MAX] = {0, 1, 3, 2, 1, 3, 2, 0, 1};

const UINT max_stations[ROUND_TYPE_MAX] = 
   {0, DTQ_STATIONS, DTM_STATIONS, DTM_STATIONS, TQ_STATIONS,  TQ_STATIONS,  TM_STATIONS, 5, 5, 5};

const UINT max_shots_per_shooter[ROUND_TYPE_MAX] = 
   {0, DTQ_SHOTS_PER_SHOOTER, DTM_SHOTS_PER_SHOOTER, DTM_SHOTS_PER_SHOOTER, 
       TQ_SHOTS_PER_SHOOTER,  TM_SHOTS_PER_SHOOTER,  TM_SHOTS_PER_SHOOTER,  25, TQ_SHOTS_PER_SHOOTER, DTQ_SHOTS_PER_SHOOTER};

const COLORREF trap_colors[ROUND_TYPE_MAX][MAX_MACHINES] = {
 { GREY, GREY, GREY, GREY, GREY, GREY, GREY, GREY, GREY, GREY, GREY, GREY, GREY, GREY, GREY },
 { GREY, GREY, GREY, GREY, GREY, GREY, WHITE, WHITE, WHITE, GREY, GREY, GREY, GREY, GREY, GREY },
 { GREY, GREY, GREY, GREY, GREY, GREY, WHITE, WHITE, WHITE, GREY, GREY, GREY, GREY, GREY, GREY },
 { GREY, GREY, GREY, GREY, GREY, GREY, WHITE, WHITE, WHITE, GREY, GREY, GREY, GREY, GREY, GREY },
 { WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE },
 { WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE },
 { GREY, GREY, GREY, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, GREY, GREY, GREY },
 { WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE },
 { WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE },
 { GREY, GREY, GREY, GREY, GREY, GREY, WHITE, WHITE, WHITE, GREY, GREY, GREY, GREY, GREY, GREY },
 { WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE },
 { GREY, GREY, GREY, GREY, GREY, GREY, WHITE, WHITE, WHITE, GREY, GREY, GREY, GREY, GREY, GREY },
 { WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE },
 { GREY, GREY, GREY, GREY, GREY, GREY, WHITE, WHITE, WHITE, GREY, GREY, GREY, GREY, GREY, GREY }
}; 

const COLORREF mike_colors[ROUND_TYPE_MAX][MAX_STATIONS] = {
 { GREY, GREY, GREY, GREY, GREY },
 { WHITE, WHITE, WHITE, WHITE, WHITE },
 { GREY, WHITE, WHITE, WHITE, GREY },
 { GREY, WHITE, WHITE, WHITE, GREY },
 { WHITE, WHITE, WHITE, WHITE, WHITE },
 { WHITE, WHITE, WHITE, WHITE, WHITE },
 { GREY, WHITE, WHITE, WHITE, GREY },
 { WHITE, WHITE, WHITE, WHITE, WHITE },
 { WHITE, WHITE, WHITE, WHITE, WHITE },
 { WHITE, WHITE, WHITE, WHITE, WHITE },
 { WHITE, WHITE, WHITE, WHITE, WHITE },
 { WHITE, WHITE, WHITE, WHITE, WHITE },
 { WHITE, WHITE, WHITE, WHITE, WHITE },
 { WHITE, WHITE, WHITE, WHITE, WHITE },
 { WHITE, WHITE, WHITE, WHITE, WHITE },
 { WHITE, WHITE, WHITE, WHITE, WHITE },
 { WHITE, WHITE, WHITE, WHITE, WHITE }
};

const bool shooters_available[ROUND_TYPE_MAX][6] = { 
 { false, false, false, false, false, false },
 { true,  true,  true,  true,  true,  true  },
 { true,  true,  true,  true,  true,  true  },
 { false, false, false, false, false, false },
 { true,  true,  true,  true,  true,  true  },
 { true,  true,  true,  true,  true,  true  },
 { false, false, false, false, false, false },
 { false, false, false, false, false, false },
 { true,  true,  true,  true,  true,  true  },
 { true,  true,  true,  true,  true,  true  }
};

const BOOL shooters_default[ROUND_TYPE_MAX][6] = { 
 { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
 { TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE  },
 { TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE  },
 { TRUE,  TRUE,  FALSE, FALSE, FALSE, FALSE },
 { TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE  },
 { TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE  },
 { TRUE,  TRUE,  FALSE, FALSE, FALSE, FALSE },
 { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
 { TRUE,  FALSE, FALSE, FALSE, FALSE, FALSE },
 { TRUE,  FALSE, FALSE, FALSE, FALSE, FALSE },
 { TRUE,  FALSE, FALSE, FALSE, FALSE, FALSE },
 { TRUE,  FALSE, FALSE, FALSE, FALSE, FALSE }
};

const UINT target_sequence_number_match[2/*shooters*/][3/*stations*/][5/*shots*/] =
{ { {0,6,12,18,24}, {2,8,14,20,26}, {4,10,16,22,28} },
  { {5,11,17,23,29}, {1,7,13,19,25}, {3,9,15,21,27} } };

const UINT target_sequence_number_semifinal[6/*shooters*/][3/*stations*/][5/*shots*/] =
{ { { 0,18,36,54,72}, { 6,24,42,60,78}, {12,30,48,66,84} },
  { {13,31,49,67,85}, { 1,19,37,55,73}, { 7,25,43,61,79} },
  { { 8,26,44,62,80}, {14,32,50,68,86}, { 2,20,38,56,74} },
  { { 3,21,39,57,75}, { 9,27,45,63,81}, {15,33,51,69,87} },
  { {16,34,52,70,88}, { 4,22,40,58,76}, {10,28,46,64,82} },
  { {11,29,47,65,83}, {17,35,53,71,89}, { 5,23,41,59,77} } };

const wchar_t compak_trap_name[MAX_COMPAK_TRAPS][MAX_TRAP_NAME_LEN] = {L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H" };

#define SAMPLES 512
static unsigned char active_samples[SAMPLES];
static unsigned char noise_samples[SAMPLES];

static UINT wait_for_shot1 = 0;

static UINT SelectPause = DEF_SELECT_PAUSE;
static UINT GlobalThreshold = DEF_GLOBAL_THRESHOLD; //MAX_VOLUME;
static UINT NoiseThreshold = DEF_NOISE_THRESHOLD;  //MAX_VOLUME;
static BOOL Connected = FALSE;
static UINT BrokenTrap = 0;
static UINT ExchanTrap = 0;
static UINT SelectRelay = DEF_SELECT_RELAY;   /* 300 msec */
static UINT MinDelay = 0;
static UINT MaxDelay = 0;
static UINT SerialNumber = 0; 
static BOOL CompakPicture = FALSE;
static UINT FITASCTable = 0;
static UINT FITASCType = 0;
static UINT FITASCShooters = 6;
static UINT ShootoffShooters = 2;
static UINT ShootoffStation = 0;
static UINT BackgroundNoise = 0;
static UINT UserThreshold = DEF_GLOBAL_THRESHOLD; //MAX_VOLUME;
static UINT AirThreshold = DEF_NOISE_THRESHOLD; //MAX_VOLUME;
const UINT extension = 0x1227edcc;
static BOOL FirstSignalReleased = FALSE;
static COMPAK_TARGET_ST compak_current_table[5/* stations*/][5/* targets*/];
static UINT Un0 = 0;
static UINT Un1 = 0;
static UINT Un2 = 0;
static UINT Un3 = 0;
static UINT Un4 = 0;
static UINT Un5 = 0;
static UINT Un6 = 0;
static UINT Un7 = 0;

UINT tx_counter = 0;
UINT tx_err_local = 0;
UINT rx_counter = 0;
UINT rx_err_local = 0;

static INT randoms[MAX_SHOOTERS][MAX_STATIONS];
static time_t LastReleaseTime = 0;

unsigned char  led1_cntr /*noise_counter*/ = 0;
unsigned char  led2_cntr /*echo_counter*/  = 0;
unsigned char  led3_cntr /*pick_counter*/  = 0;
unsigned char  led4_cntr /*loud_counter*/  = 0;
unsigned char  led6_cntr /*repeated_value_counter*/ = 0;
unsigned char  release_delay_msec = 0;
unsigned char  sampling_time_msec = 0;
unsigned char  led5_cntr /*curve_counter*/ = 0;

UINT sn_1 = 0;
UINT sn_2 = 0;
UINT sn_3 = 0;
UINT sn_4 = 0;

typedef struct hid_struct hid_t;
static hid_t *first_hid = NULL;
static hid_t *last_hid = NULL;
struct hid_struct {
	HANDLE handle;
	int open;
	struct hid_struct *prev;
	struct hid_struct *next;
};
static HANDLE rx_event=NULL;
static HANDLE tx_event=NULL;
static CRITICAL_SECTION rx_mutex;
static CRITICAL_SECTION tx_mutex;


// private functions, not intended to be used from outside this file
static void add_hid(hid_t *h);
static hid_t * get_hid(int num);
static void free_all_hid(void);
static void hid_close(hid_t *hid);

static int rawhid_open(int max, int vid, int pid, int usage_page, int usage);
static int rawhid_recv(int num, void *buf, int len, int timeout);
static int rawhid_send(int num, void *buf, int len, int timeout);
static void rawhid_close(int num);


static wchar_t setting_file_buffer[MAX_SETTING_FILE_SIZE];
//static char setting_file_buffer[MAX_SETTING_FILE_SIZE];

//void print_win32_err(void);
#define MAX_MEMORY_SIZE 32256

static unsigned char firmware_image[MAX_MEMORY_SIZE];
static unsigned char firmware_mask[MAX_MEMORY_SIZE];
static int end_record_seen=0;
static int byte_count;
static unsigned int extended_addr = 0;

void ihex_get_data(int addr, int len, unsigned char *bytes);
int parse_hex_line(char *line);

extern unsigned char image_data[];
extern UINT image_size;
extern UINT major_image_version;
extern UINT minor_image_version;
extern UINT image_serial_number;
static UINT major_embedded = 0;
static UINT minor_embedded = 0;

static HANDLE win32_teensy_handle = NULL;
int teensy_open(void);
int teensy_write(void *buf, int len, double timeout);
void teensy_close(void);
const UINT code_size = 32256;
const UINT block_size = 128;

static BOOL Paused = FALSE;
// VZ - end



// CBunkerControlApp initialization

BOOL CBunkerControlApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	//LoadStdProfileSettings(5);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	//CFrameWnd * m_pMainWnd;
    wnd = new CMainFrame();
    m_pMainWnd = wnd;

	CStringW string;
	// The one and only window has been initialized, so show and update it
	//m_pMainWnd->SetWindowPos(NULL,300,300,300,120,SWP_DRAWFRAME | SWP_NOMOVE); 
	m_pMainWnd->SetWindowPos(NULL,400,200,350,300,SWP_DRAWFRAME); 
	m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->SetWindowText(MLSTR_BUNKERCONTROL_TITLE);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
    CString m_vzBoxRevision;
// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog( );
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	m_vzBoxRevision.Format(L"(%d.%d)",major_image_version,minor_image_version);
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog( )
{
	CDialog::OnInitDialog();
	UpdateData(FALSE);
    
	return (1);
}

// CBootloaderDlg dialog used for run vzBox in bootloader mode

class CBootloaderDlg : public CDialog
{
public:
	CBootloaderDlg();
    CString m_password;
	CString m_mypassword;
    bool m_password_ok;

// Dialog Data
	enum { IDD = IDD_VZBOX_BOOT_MODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnClickConfirmBox();
	afx_msg void OnEnChangePassword();
};

CBootloaderDlg::CBootloaderDlg() : CDialog(CBootloaderDlg::IDD)
{
 	m_password_ok = false;
	m_mypassword.Format(L"%d",extension);
	EnableActiveAccessibility();
}

void CBootloaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_LBString(pDX, IDC_PASSWORD, m_password);
}

BEGIN_MESSAGE_MAP(CBootloaderDlg, CDialog)
	ON_BN_CLICKED(IDC_BOOTLOADER, &CBootloaderDlg::OnClickConfirmBox)
	ON_EN_CHANGE(IDC_PASSWORD, &CBootloaderDlg::OnEnChangePassword)
    //ON_EN_KILLFOCUS(IDC_PASSWORD, &CRoundDlg::OnKillFocusPassword)
END_MESSAGE_MAP()

void CBootloaderDlg::OnEnChangePassword()
{
	CEdit *edtPassword;
	edtPassword = reinterpret_cast<CEdit *>(GetDlgItem(IDC_PASSWORD));
    edtPassword->GetWindowText(m_password);

    if(m_password == m_mypassword)
	{
        m_password_ok = true;    
	}
	else
	{
        m_password_ok = false;    
	}
}

void CBootloaderDlg::OnClickConfirmBox(void)
{
  	int r;
    CString s;
    
    m_password_ok = true;

	if( !m_password_ok )
	{
		s.Format(MLSTR_INVALID_PASSWORD);
        AfxMessageBox(s);
        return;
	}

	// C-based example is 16C0:0480:FFAB:0200
	// r = rawhid_open(1, 0x16C0, 0x0480, 0xFFAB, 0x0200);
    // vzBox
	r = rawhid_open(1, 0xBA91, 0xC301, 0xFFAB, 0x0200);
	if (r <= 0)
	{
		s.Format(MLSTR_CANNOTCONNECT_VZBOX);
        AfxMessageBox(s);
        return;
	}
    else if( r > 1 )
	{
		s.Format(MLSTR_MULTIPLE_DEVICES);
        AfxMessageBox(s);
        return;
	}

    Sleep(200);

    //if( Connected )
	{
		unsigned char msg[VZBOX_TX_MSG_SIZE];
 
        msg[0]  = EMB_TX_SIGNATURE;
        msg[1]  = (unsigned char)tx_counter; //tx sequence number
        msg[2]  = EMB_JUMP_BOOTLOADER;
        for( int i = 3; i < VZBOX_TX_MSG_SIZE; ++i )
        { 
            msg[i]  = 0;
        }
        rawhid_send(0,msg,VZBOX_TX_MSG_SIZE,200);
		rawhid_close(0);
	}
    EndDialog(0);
}

// CResetBoxDlg dialog used for reset vzBox
class CResetBoxDlg : public CDialog
{
public:
	CResetBoxDlg();

// Dialog Data
	enum { IDD = IDD_RESET_BOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnClickConfirmBox();
};

CResetBoxDlg::CResetBoxDlg() : CDialog(CResetBoxDlg::IDD)
{
	EnableActiveAccessibility();
}

void CResetBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CResetBoxDlg, CDialog)
	ON_BN_CLICKED(IDC_BOOTLOADER, &CResetBoxDlg::OnClickConfirmBox)
END_MESSAGE_MAP()


void CResetBoxDlg::OnClickConfirmBox(void)
{
  	int r;
    CString s;

	// C-based example is 16C0:0480:FFAB:0200
	// r = rawhid_open(1, 0x16C0, 0x0480, 0xFFAB, 0x0200);
    // vzBox
	r = rawhid_open(1, 0xBA91, 0xC301, 0xFFAB, 0x0200);
	if (r <= 0)
	{
		s.Format(MLSTR_CANNOTCONNECT_VZBOX);
        AfxMessageBox(s);
        return;
	}
    else if( r > 1 )
	{
		s.Format(MLSTR_MULTIPLE_DEVICES);
        AfxMessageBox(s);
        return;
	}

    Sleep(200);

    //if( Connected )
	{
		unsigned char msg[VZBOX_TX_MSG_SIZE];
 
        msg[0]  = EMB_TX_SIGNATURE;
        msg[1]  = (unsigned char)tx_counter; //tx sequence number
        msg[2]  = EMB_JUMP_BOOTLOADER;
        for( int i = 3; i < VZBOX_TX_MSG_SIZE; ++i )
        { 
            msg[i]  = 0;
        }
        rawhid_send(0,msg,VZBOX_TX_MSG_SIZE,200);
		rawhid_close(0);
	}

	Sleep(4000);

	if (!teensy_open())
	{
		s.Format(L"Cannot Find vzBox!");
        AfxMessageBox(s);
        return;	
	}
	int first_block=1;
	unsigned char buf[260];
    // reboot to the old code
    buf[0] = 0xFF;
    buf[1] = 0xFF;
	for( UINT i = 2; i < 260; ++i )
	{
		buf[i] = 0;
	}
    teensy_write(buf, block_size + 2, 0.25);
    teensy_close();

    EndDialog(0);
}

// CConvertFirmwareDlg dialog used to convert hex into h

class CConvertFirmwareDlg : public CDialog
{
public:
	CConvertFirmwareDlg();

	UINT Major;
	UINT Minor;
	UINT SerialNum;

// Dialog Data
	enum { IDD = IDD_CONVERT_FIRMWARE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnClickConfirmBox();
};

CConvertFirmwareDlg::CConvertFirmwareDlg() : CDialog(CConvertFirmwareDlg::IDD)
{
	Major = 0;
	Minor = 0;
	SerialNum = 0;
	EnableActiveAccessibility();
}

void CConvertFirmwareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CConvertFirmwareDlg, CDialog)
	ON_BN_CLICKED(IDC_BOOTLOADER, &CConvertFirmwareDlg::OnClickConfirmBox)
END_MESSAGE_MAP()

void CConvertFirmwareDlg::OnClickConfirmBox(void)
{
    CString s;
	unsigned char r_head[4] = {0x4E,0xB1,0x31,0x08};
    unsigned char r_tail[4] = {0xE8,0xD0,0xF4,0xEB};
	unsigned char s_tail[4] = {0x5B,0x07,0x04,0x18};
    BOOL revision_found = FALSE;
	UINT sn_1,sn_2,sn_3,sn_4;

	Major = 0;
	Minor = 0;
	SerialNum = 0;

	// prompt to find a file
	CString Pathname;
	CString Pathname2 = L"vzBox_converted.h";
    CFileStatus status;
    CString Filters(MLSTR_HEX_FILES);
 
    CFileDialog Dlg(TRUE,                 //open/save file dialog
                    L".hex",              //default extension
                    L"vzBox.hex", //default file name
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    Filters);

    Dlg.m_ofn.lpstrTitle = MLSTR_LOADING_FIRMWARE;

    if( Dlg.DoModal() == IDOK )
    {

        Pathname=Dlg.GetPathName();

		FILE *fp;
        int i, lineno=0;
        char buf[1024];
		errno_t err;

        byte_count = 0;
        end_record_seen = 0;
        for (i=0; i<MAX_MEMORY_SIZE; i++) {
                firmware_image[i] = 0xFF;
                firmware_mask[i] = 0;
        }
        extended_addr = 0;

        err = _tfopen_s(&fp,Pathname, L"r");
        if (err != 0)
		{
            s.Format(MLSTR_CANNOT_OPEN_FILE,Pathname,err);
            AfxMessageBox(s);
            return;
        }
        while (!feof(fp)) {
                *buf = '\0';
                if (!fgets(buf, sizeof(buf), fp)) break;
                lineno++;
                if (*buf)
				{
                        if (parse_hex_line(buf) == 0)
						{
                            s.Format(MLSTR_FILE_TOO_BIG,Pathname,status.m_size);
                            AfxMessageBox(s);
							fclose(fp);
                            return;
                        }
                }
                if (end_record_seen) break;
                if (feof(stdin)) break;
        }
        fclose(fp);

    	if( (byte_count <= 0) || (byte_count >= MAX_MEMORY_SIZE) )
	    {
            s.Format(MLSTR_FILE_TOO_BIG,Pathname,status.m_size);
            AfxMessageBox(s);
		    return;
	    }

		for( INT pos = 0; pos < byte_count; ++pos )
		{
			if(r_head[0] == firmware_image[pos])
			{
             if(r_head[1] == firmware_image[pos+1])
			 {
			  if(r_head[2] == firmware_image[pos+2])
			  {
			   if(r_head[3] == firmware_image[pos+3])
			   {
                if(r_tail[0] == firmware_image[pos+6])
				{
				 if(r_tail[1] == firmware_image[pos+7])
				 {
				  if(r_tail[2] == firmware_image[pos+8])
				  {
				   if(r_tail[3] == firmware_image[pos+9])
				   {
				    if(s_tail[0] == firmware_image[pos+14])
					{
                     if(s_tail[1] == firmware_image[pos+15])
					 {
					  if(s_tail[2] == firmware_image[pos+16])
					  {
					   if(s_tail[3] == firmware_image[pos+17])
					   {
                         // found exact place
					     revision_found = TRUE;
                         Major = firmware_image[pos+4];
                         Minor = firmware_image[pos+5];
						 sn_1 = firmware_image[pos+10];
                         sn_2 = firmware_image[pos+11];
                         sn_3 = firmware_image[pos+12];
                         sn_4 = firmware_image[pos+13];
						 SerialNum = (sn_1 << 24) | (sn_2 << 16) | (sn_3 << 8) | (sn_4 << 0);
					     break;
					   }
					  }
					 }
					}
				   }
				  }
				 }
				}
			   }
			  }
			 }
			}
		}

		if(!revision_found)
		{
           s.Format(L"Firmware revision not found");
           AfxMessageBox(s);
           return;
		}
		// now create h file
		Filters = MLSTR_H_FILES;

		CFileDialog Dlg(FALSE,                 //open/save file dialog
                    L".h",              //default extension
                    L"vzBox_converted.h", //default file name
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    Filters);

		Dlg.m_ofn.lpstrTitle = MLSTR_CONVERTING_FIRMWARE;

        if( Dlg.DoModal() != IDOK )
        {
			return;
		}

        Pathname2=Dlg.GetPathName();

        err = _tfopen_s(&fp,Pathname2, L"w");
		        if (err != 0)
		{
            s.Format(MLSTR_CANNOT_OPEN_FILE,Pathname2,err);
            AfxMessageBox(s);
            return;
        }

		fwprintf(fp,L"\n UINT major_image_version = %d; \n", Major );
        fwprintf(fp,L" UINT minor_image_version = %d; \n", Minor );
        fwprintf(fp,L" UINT image_serial_number = %d; \n", SerialNum );
		fwprintf(fp,L" UINT image_size = %d; \n", byte_count );
		fwprintf(fp,L"\n unsigned char image_data[%d] = { \n", byte_count );
		for(UINT idx = 0; idx < (UINT)byte_count; idx++ )
		{
			if( (idx % 16) == 0) 
			{
                fwprintf(fp,L" \n" );    
			}
			if( idx == byte_count-1 )
			{
			    fwprintf(fp,L" %3d ", firmware_image[idx] );
			}
			else
			{
                fwprintf(fp,L" %3d,", firmware_image[idx] );
			}
			
		}
		fwprintf(fp,L" \n }; \n" );
		fclose(fp);

	}

    EndDialog(0);
}

// CProgramFirmwareDlg dialog used to program vzBox

class CProgramFirmwareDlg : public CDialog
{
public:
	CProgramFirmwareDlg();
    CString m_password;
	CString m_mypassword;
    bool m_password_ok;

// Dialog Data
	enum { IDD = IDD_PROGRAM_FIRMWARE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnClickConfirmBox();
	afx_msg void OnEnChangePassword();
};

CProgramFirmwareDlg::CProgramFirmwareDlg() : CDialog(CProgramFirmwareDlg::IDD)
{
 	m_password_ok = false;
	m_mypassword.Format(L"%d",extension);
	EnableActiveAccessibility();
}

void CProgramFirmwareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_LBString(pDX, IDC_PASSWORD, m_password);
}

BEGIN_MESSAGE_MAP(CProgramFirmwareDlg, CDialog)
	ON_BN_CLICKED(IDC_BOOTLOADER, &CProgramFirmwareDlg::OnClickConfirmBox)
	ON_EN_CHANGE(IDC_PASSWORD, &CProgramFirmwareDlg::OnEnChangePassword)
    //ON_EN_KILLFOCUS(IDC_PASSWORD, &CRoundDlg::OnKillFocusPassword)
END_MESSAGE_MAP()

void CProgramFirmwareDlg::OnEnChangePassword()
{
	CEdit *edtPassword;
	edtPassword = reinterpret_cast<CEdit *>(GetDlgItem(IDC_PASSWORD));
    edtPassword->GetWindowText(m_password);

    if(m_password == m_mypassword)
	{
        m_password_ok = true;    
	}
	else
	{
        m_password_ok = false;    
	}
}

void CProgramFirmwareDlg::OnClickConfirmBox(void)
{
    CString s;
    
    m_password_ok = true;

	if( !m_password_ok )
	{
		s.Format(MLSTR_INVALID_PASSWORD);
        AfxMessageBox(s);
        return;
	}

	if (!teensy_open())
	{
		s.Format(L"Cannot Find vzBox!");
        AfxMessageBox(s);
        return;	
	}
	int r, first_block=1;
	unsigned char buf[260];
	for (UINT addr = 0; addr < code_size; addr += block_size)
	{
      if (code_size < 0x10000)
	  {
          buf[0] = addr & 255;
          buf[1] = (addr >> 8) & 255;
      }
	  else
	  {
          buf[0] = (addr >> 8) & 255;
          buf[1] = (addr >> 16) & 255;
      }
      ihex_get_data(addr, block_size, buf + 2);
      r = teensy_write(buf, block_size + 2, first_block ? 3.0 : 0.25);
      if (!r)
	  {
		  s.Format(L"Error programming vzBox!");
          AfxMessageBox(s);
          return;	
	  }
      first_block = 0;
    }
 
    // reboot to the user's new code
    buf[0] = 0xFF;
    buf[1] = 0xFF;
	for( UINT i = 2; i < 260; ++i )
	{
		buf[i] = 0;
	}
    teensy_write(buf, block_size + 2, 0.25);
    teensy_close();

    EndDialog(0);
}


// CLoadFirmwareDlg dialog used to load vzBox

class CLoadFirmwareDlg : public CDialog
{
public:
	CLoadFirmwareDlg();
 
	CString m_CurrentRevision;
    CString m_NewRevision;

// Dialog Data
	enum { IDD = IDD_LOAD_FIRMWARE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnClickConfirmBox();
};

CLoadFirmwareDlg::CLoadFirmwareDlg() : CDialog(CLoadFirmwareDlg::IDD)
{
	m_CurrentRevision.Format(L"%d.%d", major_embedded, minor_embedded);
    m_NewRevision.Format(L"%d.%d", major_image_version, minor_image_version);
 	EnableActiveAccessibility();
}

void CLoadFirmwareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CURRENT_REVISION, m_CurrentRevision);
	DDX_Text(pDX, IDC_NEW_REVISION, m_NewRevision);
}

BEGIN_MESSAGE_MAP(CLoadFirmwareDlg, CDialog)
	ON_BN_CLICKED(IDC_BOOTLOADER, &CLoadFirmwareDlg::OnClickConfirmBox)
END_MESSAGE_MAP()


void CLoadFirmwareDlg::OnClickConfirmBox(void)
{
	int r;
    CString s;
    
	// C-based example is 16C0:0480:FFAB:0200
	// r = rawhid_open(1, 0x16C0, 0x0480, 0xFFAB, 0x0200);
    // vzBox
	r = rawhid_open(1, 0xBA91, 0xC301, 0xFFAB, 0x0200);
	if (r <= 0)
	{
		s.Format(MLSTR_CANNOTCONNECT_VZBOX);
        AfxMessageBox(s);
        return;
	}
    else if( r > 1 )
	{
		s.Format(MLSTR_MULTIPLE_DEVICES);
        AfxMessageBox(s);
        return;
	}

    Sleep(200);

    //if( Connected )
	{
		unsigned char msg[VZBOX_TX_MSG_SIZE];
 
        msg[0]  = EMB_TX_SIGNATURE;
        msg[1]  = (unsigned char)tx_counter; //tx sequence number
        msg[2]  = EMB_JUMP_BOOTLOADER;
        for( int i = 3; i < VZBOX_TX_MSG_SIZE; ++i )
        { 
            msg[i]  = 0;
        }
        rawhid_send(0,msg,VZBOX_TX_MSG_SIZE,200);
		rawhid_close(0);
	}

	Sleep(4000);

	if (!teensy_open())
	{
		s.Format(L"Cannot Find vzBox!");
        AfxMessageBox(s);
        return;	
	}
	int first_block=1;
	unsigned char buf[260];
	for (UINT addr = 0; addr < code_size; addr += block_size)
	{
      if (code_size < 0x10000)
	  {
          buf[0] = addr & 255;
          buf[1] = (addr >> 8) & 255;
      }
	  else
	  {
          buf[0] = (addr >> 8) & 255;
          buf[1] = (addr >> 16) & 255;
      }
      ihex_get_data(addr, block_size, buf + 2);
      r = teensy_write(buf, block_size + 2, first_block ? 3.0 : 0.25);
      if (!r)
	  {
		  s.Format(L"Error programming vzBox!");
          AfxMessageBox(s);
          return;	
	  }
      first_block = 0;
    }
 
    // reboot to the user's new code
    buf[0] = 0xFF;
    buf[1] = 0xFF;
	for( UINT i = 2; i < 260; ++i )
	{
		buf[i] = 0;
	}
    teensy_write(buf, block_size + 2, 0.25);
    teensy_close();

    EndDialog(0);
}

// CEnterShooterDlg dialog used for entering number of shooters for shoot-off

class CEnterShooterDlg : public CDialog
{
public:
	CEnterShooterDlg();
    CComboBox EnterShooters;
    CComboBox EnterStation;
	
// Dialog Data
	enum { IDD = IDD_ENTER_SHOOTERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnEnterShooters();
	afx_msg void OnEnterStation();
	afx_msg BOOL OnInitDialog( );
};

CEnterShooterDlg::CEnterShooterDlg() : CDialog(CEnterShooterDlg::IDD)
{
	EnableActiveAccessibility();
}

void CEnterShooterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_SHOOTOFF_SHOOTERS, EnterShooters);
	DDX_Control(pDX,IDC_SHOOTOFF_STATION, EnterStation);
}

BEGIN_MESSAGE_MAP(CEnterShooterDlg, CDialog)
    ON_CBN_CLOSEUP(IDC_SHOOTOFF_SHOOTERS, &CEnterShooterDlg::OnEnterShooters)
    ON_CBN_CLOSEUP(IDC_SHOOTOFF_STATION, &CEnterShooterDlg::OnEnterStation)
END_MESSAGE_MAP()

void CEnterShooterDlg::OnEnterShooters(void)
{
	UINT current_select = EnterShooters.GetCurSel();
    ShootoffShooters = current_select+2;
	EnterShooters.SetCurSel(current_select);
}

void CEnterShooterDlg::OnEnterStation(void)
{
	UINT current_select = EnterStation.GetCurSel();
    ShootoffStation = current_select;
	EnterStation.SetCurSel(current_select);
}

BOOL CEnterShooterDlg::OnInitDialog( )
{
	CDialog::OnInitDialog();

	EnterShooters.AddString(L" 2");
	EnterShooters.AddString(L" 3");
	EnterShooters.AddString(L" 4");
	EnterShooters.AddString(L" 5");
	EnterShooters.AddString(L" 6");
	EnterShooters.AddString(L" 7");
	EnterShooters.AddString(L" 8");
	EnterShooters.AddString(L" 9");
	EnterShooters.AddString(L"10");
	EnterShooters.AddString(L"11");
	EnterShooters.AddString(L"12");
    EnterShooters.SetCurSel(ShootoffShooters-2);

	EnterStation.AddString(L"1(Qual)");
    EnterStation.AddString(L"2(Final)");
    EnterStation.SetCurSel(ShootoffStation);

 	GetDlgItem(IDC_SHOOTOFF_SHOOTERS)->EnableWindow(true);
	GetDlgItem(IDC_SHOOTOFF_STATION)->EnableWindow(true);
	
	UpdateData(FALSE);
    
	return (1);
}

// CEnterFITASCDlg dialog used for entering FITASC table number for Compak Preset round

class CEnterFITASCDlg : public CDialog
{
public:
	CEnterFITASCDlg();
    CComboBox EnterType;
	CComboBox EnterTable;
	CComboBox EnterShooters;
	void eraseTable(CDC* dc);
	void displayTable(CDC* dc);
	UINT table_x;
	UINT table_y;
	UINT row_h;
	UINT col_w;
	
// Dialog Data
	enum { IDD = IDD_ENTER_TABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnEnterTable();
	afx_msg void OnEnterType();
	afx_msg void OnEnterShooters();
	afx_msg BOOL OnInitDialog( );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

CEnterFITASCDlg::CEnterFITASCDlg() : CDialog(CEnterFITASCDlg::IDD)
{
	EnableActiveAccessibility();
}

void CEnterFITASCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_COMPAK_TABLE, EnterTable);
	DDX_Control(pDX,IDC_COMPAK_TYPE, EnterType);
	DDX_Control(pDX,IDC_COMPAK_SHOOTERS, EnterShooters);
}

BEGIN_MESSAGE_MAP(CEnterFITASCDlg, CDialog)
	ON_WM_ERASEBKGND()
    ON_CBN_CLOSEUP(IDC_COMPAK_TYPE, &CEnterFITASCDlg::OnEnterType)
	ON_CBN_CLOSEUP(IDC_COMPAK_TABLE, &CEnterFITASCDlg::OnEnterTable)
    ON_CBN_CLOSEUP(IDC_COMPAK_SHOOTERS, &CEnterFITASCDlg::OnEnterShooters)

END_MESSAGE_MAP()

void CEnterFITASCDlg::OnEnterTable(void)
{
	UINT current_select = EnterTable.GetCurSel();
    FITASCTable = FITASCType*8 + current_select;
	EnterTable.SetCurSel(current_select);

	eraseTable(NULL);
	displayTable(NULL);

	UpdateData(FALSE);

}

void CEnterFITASCDlg::OnEnterShooters(void)
{
	UINT current_select = EnterShooters.GetCurSel();
    FITASCShooters = current_select+1;
	EnterShooters.SetCurSel(current_select);
}

void CEnterFITASCDlg::OnEnterType(void)
{
	UINT from = 0;
	UINT to = 40;
    UINT table = 0;
	CString str1;

	UINT current_select = EnterType.GetCurSel();
    FITASCType = current_select;
	EnterType.SetCurSel(current_select);

	from = FITASCType*8;
	to = from+8;

	EnterTable.ResetContent();


	for( table = from; table < to; ++table )
	{
		str1.Format(L"%2d",table+1);
        EnterTable.AddString(str1);
	}

	FITASCTable = from;
    EnterTable.SetCurSel(0);
    
	GetDlgItem(IDC_COMPAK_TABLE)->EnableWindow(true);

	eraseTable(NULL);
	displayTable(NULL);

	UpdateData(FALSE);
}

BOOL CEnterFITASCDlg::OnInitDialog( )
{
	CDialog::OnInitDialog();
    CString str1;
    UINT table;

	table_x = 220;
	table_y = 80;
	row_h   = 30;
	col_w   = 100;

	FITASCType = 0;
	FITASCTable = 0;

	EnterShooters.AddString(L" 1");
	EnterShooters.AddString(L" 2");
    EnterShooters.AddString(L" 3");
    EnterShooters.AddString(L" 4");
    EnterShooters.AddString(L" 5");
    EnterShooters.AddString(L" 6");
    EnterShooters.SetCurSel(FITASCShooters-1);

    EnterType.AddString(MLSTR_5_SINGLES);
    EnterType.AddString(MLSTR_3_SINGLES_1_ON_REPORT);
    EnterType.AddString(MLSTR_3_SINGLES_1_SIMULTANEOUS);
    EnterType.AddString(MLSTR_1_SINGLES_2_ON_REPORT);
    EnterType.AddString(MLSTR_1_SINGLES_2_SIMULTANEOUS);
	EnterType.SetCurSel(FITASCType);
	
	for( table = 0; table < 8; ++table )
	{
		str1.Format(L"%2d",table+1);
        EnterTable.AddString(str1);
	}
    EnterTable.SetCurSel(FITASCTable);

 	GetDlgItem(IDC_COMPAK_TYPE)->EnableWindow(true);
 	GetDlgItem(IDC_COMPAK_TABLE)->EnableWindow(true);
 	GetDlgItem(IDC_COMPAK_SHOOTERS)->EnableWindow(true);

	eraseTable(NULL);
	displayTable(NULL);

	UpdateData(FALSE);
    
	return (1);
}

BOOL CEnterFITASCDlg::OnEraseBkgnd(CDC* pDC)
{
	BOOL release_dc = FALSE;
    CRect rect;
    GetClientRect(&rect);
	if( pDC == NULL )
	{
        pDC = GetDC( );
		release_dc = TRUE;
	}
    //CBrush myBrush(RGB(255, 255, 255));    // dialog background color
    CBrush myBrush(BKG_GREY);    // dialog background color
    CBrush *pOld = pDC->SelectObject(&myBrush);
    BOOL bRes  = pDC->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
    pDC->SelectObject(pOld);    // restore old brush

	eraseTable(NULL);
	displayTable(NULL);

	return TRUE;
}

void CEnterFITASCDlg::eraseTable(CDC* dc)
{
	BOOL release_dc = FALSE;

    if( dc == NULL )
    {
        dc = GetDC( );
        release_dc = TRUE;
	}

	CBrush brush1;   // Must initialize!
    brush1.CreateSolidBrush(WHITE);  
	
    CBrush* pTempBrush = NULL;
    CBrush OrigBrush;

    pTempBrush = (CBrush*)dc->SelectObject(&brush1);
    // Save original brush.
    OrigBrush.FromHandle((HBRUSH)pTempBrush);
    

    dc->Rectangle(table_x - 10, table_y - 10, table_x + col_w*5 + 10, table_y + row_h*5 + 10);    

	//deselect your objects to avoid memory leaks
	dc->SelectObject(&OrigBrush);
    //dc->SelectObject(pOldBrush);

	if(release_dc )
	{
		ReleaseDC(dc);
	}

	return;
}

void CEnterFITASCDlg::displayTable(CDC* dc)
{
    UINT x = 0;
	UINT y = 0;
    BOOL release_dc = FALSE;
	COMPAK_TARGET_ST my_target;
	CPen newPen;
	CString compakStr;
 
	if( dc == NULL )
	{
        dc = GetDC( );
		release_dc = TRUE;
	}
    if( newPen.CreatePen( PS_SOLID, 2, BLACK) )
    {
        // Select it into the device context
        // Save the old pen at the same time
        CPen* pOldPen = dc->SelectObject( &newPen );

        // Draw with the pen
		for( UINT row = 0; row < 6; ++row )
		{
			if( row == 4 )
			{
				if( FITASCTable >= 8 )
				{
					continue;
				}
			}
			if( row == 2 )
			{
                if( FITASCTable >= 24 )
				{
					continue;
				}

			}
		    dc->MoveTo(table_x,        table_y+row*row_h);
            dc->LineTo(table_x+col_w*5,table_y+row*row_h);
		}

		for( UINT col = 0; col < 6; ++col )
		{
		    dc->MoveTo(table_x + col*col_w, table_y        );
            dc->LineTo(table_x + col*col_w, table_y+5*row_h);
		}

		for( UINT t = 0; t < 5; ++t )
		{
			for( UINT s = 0; s < 5; ++s )
			{
    			memcpy(&my_target,&compak_preset_tables[FITASCTable][t][s],sizeof(COMPAK_TARGET_ST));
                switch(my_target.t_type)
				{
				    case COMPAK_SINGLE:
					    compakStr.Format(L"%s",compak_trap_name[my_target.t_one-1]);
            			dc->TextOutW(table_x + col_w*t + col_w/3 + col_w/8, table_y + row_h *s + row_h/4, compakStr );
						break;
					case COMPAK_D_SIGN:
					    compakStr.Format(L"On Report");
						if( (FITASCTable < 24) || (s == 1) )
						{
            			    dc->TextOutW(table_x + col_w*t + col_w/4, table_y + row_h *s + row_h/4, compakStr );
						}
						else 
						{
               			    dc->TextOutW(table_x + col_w*t + col_w/4, table_y + row_h *(s+1) + row_h/4, compakStr );
						}
						compakStr.Format(L"%s - %s",compak_trap_name[my_target.t_one-1],compak_trap_name[my_target.t_two-1]);
                        if( (FITASCTable < 24) || (s == 1) )
						{ 
						    dc->TextOutW(table_x + col_w*t + col_w/3, table_y + row_h*(s+1) + row_h/4, compakStr );
						}
						else 
						{
               			    dc->TextOutW(table_x + col_w*t + col_w/3, table_y + row_h *(s+2) + row_h/4, compakStr );
						}
						break;
					case COMPAK_D_SIMU:
					    compakStr.Format(L"Simultaneous");
						if( (FITASCTable < 24) || (s == 1) )
						{
            			    dc->TextOutW(table_x + col_w*t + col_w/10, table_y + row_h *s + row_h/4, compakStr );
						}
						else 
						{
               			    dc->TextOutW(table_x + col_w*t + col_w/10, table_y + row_h *(s+1) + row_h/4, compakStr );
						}
						compakStr.Format(L"%s - %s",compak_trap_name[my_target.t_one-1],compak_trap_name[my_target.t_two-1]);
                        if( (FITASCTable < 24) || (s == 1) )
						{ 
						    dc->TextOutW(table_x + col_w*t + col_w/3, table_y + row_h*(s+1) + row_h/4, compakStr );
						}
						else 
						{
               			    dc->TextOutW(table_x + col_w*t + col_w/3, table_y + row_h *(s+2) + row_h/4, compakStr );
						}
						break;
					case COMPAK_T_NONE:
					default:
					    continue;
					    break;
				}
 			}
		}

		// Restore the old pen to the device context
        dc->SelectObject( pOldPen );
    

    } 
    if( release_dc )
	{
		ReleaseDC(dc);
	}

	return;
}

// CEnterCompakDlg dialog used for entering new Compak table Compak Control round

class CEnterCompakDlg : public CDialog
{
public:
	CEnterCompakDlg();
    CComboBox EnterType;
	CComboBox EnterShooters;
	CComboBox Single[5][5];
	void eraseTable(CDC* dc);
	void displayTable(CDC* dc);
	void changedValue(UINT a, UINT b, UINT value);
	BOOL CheckValidation();
	void SaveCompak();
	void RestoreCompak();
	UINT table_x;
	UINT table_y;
	UINT row_h;
	UINT col_w;
	
// Dialog Data
	enum { IDD = IDD_NEW_COMPAK_TABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnEnterType();
	afx_msg void OnEnterShooters();
	afx_msg void OnEnterSingle11();
	afx_msg void OnEnterSingle12();
	afx_msg void OnEnterSingle13();
	afx_msg void OnEnterSingle14();
	afx_msg void OnEnterSingle15();
	afx_msg void OnEnterSingle21();
	afx_msg void OnEnterSingle22();
	afx_msg void OnEnterSingle23();
	afx_msg void OnEnterSingle24();
	afx_msg void OnEnterSingle25();
	afx_msg void OnEnterSingle31();
	afx_msg void OnEnterSingle32();
	afx_msg void OnEnterSingle33();
	afx_msg void OnEnterSingle34();
	afx_msg void OnEnterSingle35();
	afx_msg void OnEnterSingle41();
	afx_msg void OnEnterSingle42();
	afx_msg void OnEnterSingle43();
	afx_msg void OnEnterSingle44();
	afx_msg void OnEnterSingle45();
	afx_msg void OnEnterSingle51();
	afx_msg void OnEnterSingle52();
	afx_msg void OnEnterSingle53();
	afx_msg void OnEnterSingle54();
	afx_msg void OnEnterSingle55();
    afx_msg void OnClickValidate();
	afx_msg BOOL OnInitDialog( );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

CEnterCompakDlg::CEnterCompakDlg() : CDialog(CEnterCompakDlg::IDD)
{
	EnableActiveAccessibility();
}

void CEnterCompakDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_COMPAK_TYPE, EnterType);
	DDX_Control(pDX,IDC_COMPAK_SHOOTERS, EnterShooters);
	DDX_Control(pDX,IDC_SINGLE_1_1, Single[0][0]);
	DDX_Control(pDX,IDC_SINGLE_1_2, Single[0][1]);
	DDX_Control(pDX,IDC_SINGLE_1_3, Single[0][2]);
	DDX_Control(pDX,IDC_SINGLE_1_4, Single[0][3]);
	DDX_Control(pDX,IDC_SINGLE_1_5, Single[0][4]);
	DDX_Control(pDX,IDC_SINGLE_2_1, Single[1][0]);
	DDX_Control(pDX,IDC_SINGLE_2_2, Single[1][1]);
	DDX_Control(pDX,IDC_SINGLE_2_3, Single[1][2]);
	DDX_Control(pDX,IDC_SINGLE_2_4, Single[1][3]);
	DDX_Control(pDX,IDC_SINGLE_2_5, Single[1][4]);
	DDX_Control(pDX,IDC_SINGLE_3_1, Single[2][0]);
	DDX_Control(pDX,IDC_SINGLE_3_2, Single[2][1]);
	DDX_Control(pDX,IDC_SINGLE_3_3, Single[2][2]);
	DDX_Control(pDX,IDC_SINGLE_3_4, Single[2][3]);
	DDX_Control(pDX,IDC_SINGLE_3_5, Single[2][4]);
	DDX_Control(pDX,IDC_SINGLE_4_1, Single[3][0]);
	DDX_Control(pDX,IDC_SINGLE_4_2, Single[3][1]);
	DDX_Control(pDX,IDC_SINGLE_4_3, Single[3][2]);
	DDX_Control(pDX,IDC_SINGLE_4_4, Single[3][3]);
	DDX_Control(pDX,IDC_SINGLE_4_5, Single[3][4]);
	DDX_Control(pDX,IDC_SINGLE_5_1, Single[4][0]);
	DDX_Control(pDX,IDC_SINGLE_5_2, Single[4][1]);
	DDX_Control(pDX,IDC_SINGLE_5_3, Single[4][2]);
	DDX_Control(pDX,IDC_SINGLE_5_4, Single[4][3]);
	DDX_Control(pDX,IDC_SINGLE_5_5, Single[4][4]);

}

BEGIN_MESSAGE_MAP(CEnterCompakDlg, CDialog)
	ON_WM_ERASEBKGND()
    ON_CBN_CLOSEUP(IDC_COMPAK_TYPE, &CEnterCompakDlg::OnEnterType)
    ON_CBN_CLOSEUP(IDC_COMPAK_SHOOTERS, &CEnterCompakDlg::OnEnterShooters)
    ON_CBN_CLOSEUP(IDC_SINGLE_1_1, &CEnterCompakDlg::OnEnterSingle11)
    ON_CBN_CLOSEUP(IDC_SINGLE_1_2, &CEnterCompakDlg::OnEnterSingle12)
    ON_CBN_CLOSEUP(IDC_SINGLE_1_3, &CEnterCompakDlg::OnEnterSingle13)
    ON_CBN_CLOSEUP(IDC_SINGLE_1_4, &CEnterCompakDlg::OnEnterSingle14)
    ON_CBN_CLOSEUP(IDC_SINGLE_1_5, &CEnterCompakDlg::OnEnterSingle15)
    ON_CBN_CLOSEUP(IDC_SINGLE_2_1, &CEnterCompakDlg::OnEnterSingle21)
    ON_CBN_CLOSEUP(IDC_SINGLE_2_2, &CEnterCompakDlg::OnEnterSingle22)
    ON_CBN_CLOSEUP(IDC_SINGLE_2_3, &CEnterCompakDlg::OnEnterSingle23)
    ON_CBN_CLOSEUP(IDC_SINGLE_2_4, &CEnterCompakDlg::OnEnterSingle24)
    ON_CBN_CLOSEUP(IDC_SINGLE_2_5, &CEnterCompakDlg::OnEnterSingle25)
    ON_CBN_CLOSEUP(IDC_SINGLE_3_1, &CEnterCompakDlg::OnEnterSingle31)
    ON_CBN_CLOSEUP(IDC_SINGLE_3_2, &CEnterCompakDlg::OnEnterSingle32)
    ON_CBN_CLOSEUP(IDC_SINGLE_3_3, &CEnterCompakDlg::OnEnterSingle33)
    ON_CBN_CLOSEUP(IDC_SINGLE_3_4, &CEnterCompakDlg::OnEnterSingle34)
    ON_CBN_CLOSEUP(IDC_SINGLE_3_5, &CEnterCompakDlg::OnEnterSingle35)
    ON_CBN_CLOSEUP(IDC_SINGLE_4_1, &CEnterCompakDlg::OnEnterSingle41)
    ON_CBN_CLOSEUP(IDC_SINGLE_4_2, &CEnterCompakDlg::OnEnterSingle42)
    ON_CBN_CLOSEUP(IDC_SINGLE_4_3, &CEnterCompakDlg::OnEnterSingle43)
    ON_CBN_CLOSEUP(IDC_SINGLE_4_4, &CEnterCompakDlg::OnEnterSingle44)
    ON_CBN_CLOSEUP(IDC_SINGLE_4_5, &CEnterCompakDlg::OnEnterSingle45)
    ON_CBN_CLOSEUP(IDC_SINGLE_5_1, &CEnterCompakDlg::OnEnterSingle51)
    ON_CBN_CLOSEUP(IDC_SINGLE_5_2, &CEnterCompakDlg::OnEnterSingle52)
    ON_CBN_CLOSEUP(IDC_SINGLE_5_3, &CEnterCompakDlg::OnEnterSingle53)
    ON_CBN_CLOSEUP(IDC_SINGLE_5_4, &CEnterCompakDlg::OnEnterSingle54)
    ON_CBN_CLOSEUP(IDC_SINGLE_5_5, &CEnterCompakDlg::OnEnterSingle55)
	ON_BN_CLICKED(IDC_VALIDATE_COMPAK, &CEnterCompakDlg::OnClickValidate)
	ON_BN_CLICKED(IDC_SAVE_COMPAK, &CEnterCompakDlg::SaveCompak)
	ON_BN_CLICKED(IDC_RESTORE_COMPAK, &CEnterCompakDlg::RestoreCompak)

END_MESSAGE_MAP()

void CEnterCompakDlg::SaveCompak()
{
	// prompt to create a file
	// save:
	CString s,Pathname;
    errno_t err;
    FILE *pFile;


    CString Filters(MLSTR_CPK_FILES);

    CFileDialog Dlg(FALSE,                //open/save file dialog
                    L".cpk",              //default extension
                    L"CompakTable_.cpk", //default file name
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    Filters);

	if( CheckValidation() != TRUE )
	{
		return;
	}

    Dlg.m_ofn.lpstrTitle = L"Saving Compak Table";

    if( Dlg.DoModal() == IDOK )
    {
        Pathname=Dlg.GetPathName();

		if( (err=_tfopen_s(&pFile,Pathname.GetString(),L"wt,ccs=UNICODE"))!=0 )
        {
            s.Format(MLSTR_CANNOT_OPEN_FILE,Pathname,err);
            AfxMessageBox(s);
            return;
        }

        fwprintf(pFile,MLSTR_FILE_VERSION);
        fwprintf(pFile,L"==========================\n");
        fwprintf(pFile,L" FT=%4d \n", FITASCType );
        for( UINT i = 0; i < 5; ++i )
		{
			for( UINT j = 0; j < 5; ++j )
			{
				fwprintf(pFile,L"@%d%d=%d:%d:%d" , i,j,
					compak_current_table[i][j].t_type,compak_current_table[i][j].t_one, compak_current_table[i][j].t_two );
			}
		    fwprintf(pFile,L" \n");
		}
        fwprintf(pFile,L"==========================\n");
		fclose(pFile);
	}
	return;
}

void CEnterCompakDlg::RestoreCompak()
{
	// prompt to find a file
	// save:
	CString s,Pathname;
    errno_t err;
    FILE *pFile;
    CFileStatus status;
    CString Filters(MLSTR_CPK_FILES);
	CString searchStr;
 
    CFileDialog Dlg(TRUE,                 //open/save file dialog
                    L".cpk",              //default extension
                    L"CompakTable_.cpk", //default file name
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    Filters);

    Dlg.m_ofn.lpstrTitle = L"Loading Compak Table";

    if( Dlg.DoModal() == IDOK )
    {
        Pathname=Dlg.GetPathName();

		if( (err=_tfopen_s(&pFile,Pathname.GetString(),L"rt,ccs=UNICODE"))!=0 )
        {
            s.Format(MLSTR_CANNOT_OPEN_FILE,Pathname,err);
            AfxMessageBox(s);
            return;
        }

        if( CFile::GetStatus( Pathname, status ) )   // static version
        {
			if( status.m_size >= MAX_SETTING_FILE_SIZE )
			{
                s.Format(MLSTR_FILE_TOO_BIG,Pathname,status.m_size);
                AfxMessageBox(s);
                fclose(pFile);
				return;
			}
		}
		
		for(UINT c = 0; c < MAX_SETTING_FILE_SIZE; ++c )
		{
			setting_file_buffer[c] = fgetwc(pFile);
			//setting_file_buffer[c] = fgetc(pFile);
			if( setting_file_buffer[c] == WEOF )
			//	if( setting_file_buffer[c] == EOF )
			{
				break;
			}
		}
		fclose(pFile);

		CString buf(setting_file_buffer);
		UINT len = buf.GetLength();

        INT parameter = buf.Find(L"FT=");       
		if( parameter > 0 )
		{
			len -= parameter;
			CString valueBuf = buf.Mid(parameter,len);
			parameter = valueBuf.Find(L"=");
            len -= parameter;
            valueBuf = valueBuf.Mid(parameter,len);
			valueBuf = valueBuf.Mid(1,4);
			FITASCType = _wtoi((LPCTSTR)valueBuf);
			EnterType.SetCurSel(FITASCType);
		}

		for( UINT i =0; i<5;  ++i )
		{
			for( UINT j=0; j<5; ++j )
			{
				searchStr.Format(L"@%d%d=",i,j);

        		len = buf.GetLength();
		        parameter = buf.Find(searchStr);       
        		if( parameter > 0 )
		        {
			        len -= parameter;
			        CString valueBuf = buf.Mid(parameter,len);
			        parameter = valueBuf.Find(L"=");
                    len -= parameter;
                    valueBuf = valueBuf.Mid(parameter,len);
			        valueBuf = valueBuf.Mid(1,5);
					compak_current_table[i][j].t_type = (COMPAK_TARGET_TYPE_EN)(valueBuf[0]-0x30);
					compak_current_table[i][j].t_one  = valueBuf[2]-0x30;
					compak_current_table[i][j].t_two  = valueBuf[4]-0x30;
					if( compak_current_table[i][j].t_type == COMPAK_SINGLE )
					{
						Single[j][i].SetCurSel(compak_current_table[i][j].t_one);
					}
					else 
					{
						Single[j][i].SetCurSel(compak_current_table[i][j].t_two);
					}
				}
			}
		}
	}

	eraseTable(NULL);
	displayTable(NULL);
	UpdateData(FALSE);

	return;
}

void CEnterCompakDlg::OnClickValidate()
{
    if( CheckValidation() == TRUE )
	{
        EndDialog(0);
	}
}

BOOL CEnterCompakDlg::CheckValidation()
{
	BOOL valid = TRUE;
    CString errorMessage;
	COMPAK_TARGET_TYPE_EN target_type;

	errorMessage.Format(L"Setting Table Incomplete");

	switch( FITASCType )
	{
	    case 0:
            for( UINT i = 0; (valid == TRUE )&&(i < 5); ++i )
			{
				for( UINT j = 0; (valid == TRUE )&& (j < 5); ++j )
				{
					if((compak_current_table[i][j].t_type != COMPAK_SINGLE) ||
					   (compak_current_table[i][j].t_one  == 0            ) ||
					   (compak_current_table[i][j].t_two  != 0            ))
					{
						valid = FALSE;
					}
				}
			}
			break;
		case 1:
		case 2:
			if( FITASCType == 1 )
			{
			    target_type = COMPAK_D_SIGN;
			}
			else
			{
			    target_type = COMPAK_D_SIMU;
			}

            for( UINT i = 0; (valid == TRUE )&&(i < 5); ++i )
			{
                for( UINT j = 0; (valid == TRUE )&& (j < 3); ++j )
				{
                    if( (compak_current_table[i][j].t_type != COMPAK_SINGLE) ||
					    (compak_current_table[i][j].t_one  == 0            ) ||
					    (compak_current_table[i][j].t_two  != 0            ))
				    {
					    valid = FALSE;
				    }
			    }
			}

            for( UINT i = 0; (valid == TRUE )&&(i < 5); ++i )
			{
                for( UINT j = 3; (valid == TRUE )&& (j < 4); ++j )
				{
                    if( (compak_current_table[i][j].t_type != target_type  ) ||
					    (compak_current_table[i][j].t_one  == 0            ) ||
					    (compak_current_table[i][j].t_two  == 0            ))
				    {
					    valid = FALSE;
				    }
					else if( compak_current_table[i][j].t_one  == compak_current_table[i][j].t_two )
					{
						valid = FALSE;
						errorMessage.Format(L"Same Trap Doubles not supported");
					}
			    }
			}

			break;
		case 3:
		case 4:
			if( FITASCType == 3 )
			{
			    target_type = COMPAK_D_SIGN;
			}
			else
			{
			    target_type = COMPAK_D_SIMU;
			}

			for( UINT i = 0; (valid == TRUE )&&(i < 5); ++i )
			{
                for( UINT j = 0; (valid == TRUE )&& (j < 1); ++j )
				{
                    if( (compak_current_table[i][j].t_type != COMPAK_SINGLE) ||
					    (compak_current_table[i][j].t_one  == 0            ) ||
					    (compak_current_table[i][j].t_two  != 0            ))
				    {
					    valid = FALSE;
				    }
			    }
			}

			for( UINT i = 0; (valid == TRUE )&&(i < 5); ++i )
			{
                for( UINT j = 1; (valid == TRUE )&& (j < 3); ++j )
				{
                    if( (compak_current_table[i][j].t_type != target_type  ) ||
					    (compak_current_table[i][j].t_one  == 0            ) ||
					    (compak_current_table[i][j].t_two  == 0            ))
				    {
					    valid = FALSE;
				    }
					else if( compak_current_table[i][j].t_one  == compak_current_table[i][j].t_two )
					{
						valid = FALSE;
						errorMessage.Format(L"Same Trap Doubles not supported");
					}
			    }
			}

			break;
		default:
			break;
	}


	if( !valid )
	{
        AfxMessageBox(errorMessage);
	}

	return valid;
}

void CEnterCompakDlg::changedValue(UINT a, UINT b, UINT value)
{
	/* a - row */
	/* b - col */
    switch( FITASCType )
	{
	    case 0:
			if( value > 0 )
			{
			compak_current_table[b][a].t_type = COMPAK_SINGLE;
			compak_current_table[b][a].t_one  = value;
			compak_current_table[b][a].t_two  = 0;
			}
			else
			{
			compak_current_table[b][a].t_type = COMPAK_T_NONE;
			compak_current_table[b][a].t_one  = value;
			compak_current_table[b][a].t_two  = 0;
			}
            break;
		case 1:
		case 2:
			if( a < 3)
			{
    			compak_current_table[b][a].t_type = COMPAK_SINGLE;
	    		compak_current_table[b][a].t_one  = value;
		    	compak_current_table[b][a].t_two  = 0;
			}
			if( (a == 2) && (b == 4) )
			{
	    		compak_current_table[0][3].t_one  = value;
			}
            if( (a == 3) && (b < 4) )
			{
	    		compak_current_table[b+1][3].t_one  = value;
			}
			if( a == 3 )
			{
	    		compak_current_table[b][3].t_two  = value;
			}
			break;
		case 3:
		case 4:
			if( a < 1)
			{
    			compak_current_table[b][a].t_type = COMPAK_SINGLE;
	    		compak_current_table[b][a].t_one  = value;
		    	compak_current_table[b][a].t_two  = 0;
			}
			if( (a == 0) && (b == 4) )
			{
	    		compak_current_table[0][1].t_one  = value;
			}
            if( (a == 1) && (b < 4) )
			{
	    		compak_current_table[b+1][1].t_one  = value;
			}
			if( (a == 1) && (b == 4) )
			{
	    		compak_current_table[0][2].t_one  = value;
			}
            if( (a == 2) && (b < 4) )
			{
	    		compak_current_table[b+1][2].t_one  = value;
			}
  
			if( a > 0 )
			{
	    		compak_current_table[b][a].t_two  = value;
			}
			
			break;
		default:
			break;
	}
    eraseTable(NULL);
	displayTable(NULL);
    return;
}

void CEnterCompakDlg::OnEnterSingle11()
{
	UINT current_select = Single[0][0].GetCurSel();
	changedValue(0,0,current_select);
	Single[0][0].SetCurSel(current_select);
}

void CEnterCompakDlg::OnEnterSingle12()
{
	UINT current_select = Single[0][1].GetCurSel();
	changedValue(0,1,current_select);
	Single[0][1].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle13()
{
	UINT current_select = Single[0][2].GetCurSel();
	changedValue(0,2,current_select);
	Single[0][2].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle14()
{
	UINT current_select = Single[0][3].GetCurSel();
	changedValue(0,3,current_select);
	Single[0][3].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle15()
{
	UINT current_select = Single[0][4].GetCurSel();
	changedValue(0,4,current_select);
	Single[0][4].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle21()
{
	UINT current_select = Single[1][0].GetCurSel();
	changedValue(1,0,current_select);
	Single[1][0].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle22()
{
	UINT current_select = Single[1][1].GetCurSel();
	changedValue(1,1,current_select);
	Single[1][1].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle23()
{
	UINT current_select = Single[1][2].GetCurSel();
	changedValue(1,2,current_select);
	Single[1][2].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle24()
{
	UINT current_select = Single[1][3].GetCurSel();
	changedValue(1,3,current_select);
	Single[1][3].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle25()
{
	UINT current_select = Single[1][4].GetCurSel();
	changedValue(1,4,current_select);
	Single[1][4].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle31()
{
	UINT current_select = Single[2][0].GetCurSel();
	changedValue(2,0,current_select);
	Single[2][0].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle32()
{
	UINT current_select = Single[2][1].GetCurSel();
	changedValue(2,1,current_select);
	Single[2][1].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle33()
{
	UINT current_select = Single[2][2].GetCurSel();
	changedValue(2,2,current_select);
	Single[2][2].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle34()
{
	UINT current_select = Single[2][3].GetCurSel();
	changedValue(2,3,current_select);
	Single[2][3].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle35()
{
	UINT current_select = Single[2][4].GetCurSel();
	changedValue(2,4,current_select);
	Single[2][4].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle41()
{
	UINT current_select = Single[3][0].GetCurSel();
	changedValue(3,0,current_select);
	Single[3][0].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle42()
{
	UINT current_select = Single[3][1].GetCurSel();
	changedValue(3,1,current_select);
	Single[3][1].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle43()
{
	UINT current_select = Single[3][2].GetCurSel();
	changedValue(3,2,current_select);
	Single[3][2].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle44()
{
	UINT current_select = Single[3][3].GetCurSel();
	changedValue(3,3,current_select);
	Single[3][3].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle45()
{
	UINT current_select = Single[3][4].GetCurSel();
	changedValue(3,4,current_select);
	Single[3][4].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle51()
{
	UINT current_select = Single[4][0].GetCurSel();
	changedValue(4,0,current_select);
	Single[4][0].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle52()
{
	UINT current_select = Single[4][1].GetCurSel();
	changedValue(4,1,current_select);
	Single[4][1].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle53()
{
	UINT current_select = Single[4][2].GetCurSel();
	changedValue(4,2,current_select);
	Single[4][2].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle54()
{
	UINT current_select = Single[4][3].GetCurSel();
	changedValue(4,3,current_select);
	Single[4][3].SetCurSel(current_select);
}
void CEnterCompakDlg::OnEnterSingle55()
{
	UINT current_select = Single[4][4].GetCurSel();
	changedValue(4,4,current_select);
	Single[4][4].SetCurSel(current_select);
}

void CEnterCompakDlg::OnEnterShooters(void)
{
	UINT current_select = EnterShooters.GetCurSel();
    FITASCShooters = current_select+1;
	EnterShooters.SetCurSel(current_select);
}

void CEnterCompakDlg::OnEnterType(void)
{
	UINT from = 0;
	UINT to = 40;
    UINT table = 0;
	CString str1;

	UINT current_select = EnterType.GetCurSel();
    FITASCType = current_select;
	EnterType.SetCurSel(current_select);

    switch( FITASCType )
	{
        case 0:
	    	for( UINT st = 0; st < 5; ++st )
	        {
		        for( UINT t = 0; t < 5; ++t )
		        {
        		    compak_current_table[st][t].t_type = COMPAK_SINGLE;
                    compak_current_table[st][t].t_one = Single[t][st].GetCurSel();
		            compak_current_table[st][t].t_two = 0;
				}
			}
			break;
	    case 1:
		case 2:
	    	for( UINT st = 0; st < 5; ++st )
	        {
		        for( UINT t = 0; t < 3; ++t )
		        {
        		    compak_current_table[st][t].t_type = COMPAK_SINGLE;
                    compak_current_table[st][t].t_one = Single[t][st].GetCurSel();
		            compak_current_table[st][t].t_two = 0;
				}
        		compak_current_table[st][4].t_type = COMPAK_T_NONE;
                compak_current_table[st][4].t_one = 0;
		        compak_current_table[st][4].t_two = 0;
				Single[4][st].SetCurSel(0);
                if( FITASCType == 1 )
				{
            		compak_current_table[st][3].t_type = COMPAK_D_SIGN;
				}
				else
				{
            		compak_current_table[st][3].t_type = COMPAK_D_SIMU;
				}

				if( st == 0 )
				{
                    compak_current_table[st][3].t_one = Single[2][4].GetCurSel();
				}
				else
				{
                    compak_current_table[st][3].t_one = Single[3][st-13].GetCurSel();
				}
		        compak_current_table[st][3].t_two = Single[3][st].GetCurSel();;
			}
			break;
		case 3:
		case 4:
	    	for( UINT st = 0; st < 5; ++st )
	        {
       		    compak_current_table[st][0].t_type = COMPAK_SINGLE;
                compak_current_table[st][0].t_one = Single[0][st].GetCurSel();
	            compak_current_table[st][0].t_two = 0;

				compak_current_table[st][3].t_type = COMPAK_T_NONE;
                compak_current_table[st][3].t_one = 0;
		        compak_current_table[st][3].t_two = 0;
                Single[3][st].SetCurSel(0);
				compak_current_table[st][4].t_type = COMPAK_T_NONE;
                compak_current_table[st][4].t_one = 0;
		        compak_current_table[st][4].t_two = 0;
				Single[4][st].SetCurSel(0);
                if( FITASCType == 3 )
				{
            		compak_current_table[st][1].t_type = COMPAK_D_SIGN;
            		compak_current_table[st][2].t_type = COMPAK_D_SIGN;
				}
				else
				{
            		compak_current_table[st][1].t_type = COMPAK_D_SIMU;
            		compak_current_table[st][2].t_type = COMPAK_D_SIMU;
				}
				if( st == 0 )
				{
                    compak_current_table[st][1].t_one = Single[0][4].GetCurSel();
                    compak_current_table[st][2].t_one = Single[1][4].GetCurSel();
				}
				else
				{
                    compak_current_table[st][1].t_one = Single[1][st-1].GetCurSel();
					compak_current_table[st][2].t_one = Single[2][st-1].GetCurSel();
				}
                compak_current_table[st][1].t_two = Single[1][st].GetCurSel();
                compak_current_table[st][2].t_two = Single[2][st].GetCurSel();                
			}
			break;
		default:
			break;
	}

	eraseTable(NULL);
	displayTable(NULL);

	UpdateData(FALSE);
}

BOOL CEnterCompakDlg::OnInitDialog( )
{
	CDialog::OnInitDialog();
    CString str1;
 
	table_x = 220;
	table_y = 80;
	row_h   = 30;
	col_w   = 70;

	FITASCType = 0;
	FITASCTable = 0;

	EnterShooters.AddString(L" 1");
	EnterShooters.AddString(L" 2");
    EnterShooters.AddString(L" 3");
    EnterShooters.AddString(L" 4");
    EnterShooters.AddString(L" 5");
    EnterShooters.AddString(L" 6");
    EnterShooters.SetCurSel(FITASCShooters-1);

    EnterType.AddString(MLSTR_5_SINGLES);
    EnterType.AddString(MLSTR_3_SINGLES_1_ON_REPORT);
    EnterType.AddString(MLSTR_3_SINGLES_1_SIMULTANEOUS);
    EnterType.AddString(MLSTR_1_SINGLES_2_ON_REPORT);
    EnterType.AddString(MLSTR_1_SINGLES_2_SIMULTANEOUS);
	EnterType.SetCurSel(FITASCType);
	
 	GetDlgItem(IDC_COMPAK_TYPE)->EnableWindow(true);
 	GetDlgItem(IDC_COMPAK_SHOOTERS)->EnableWindow(true);

	for( UINT st = 0; st < 5; ++st )
	{
		for( UINT t = 0; t < 5; ++t )
		{
			Single[st][t].AddString(L"");
			Single[st][t].AddString(L"A");
			Single[st][t].AddString(L"B");
			Single[st][t].AddString(L"C");
			Single[st][t].AddString(L"D");
			Single[st][t].AddString(L"E");
			Single[st][t].AddString(L"F");
			Single[st][t].AddString(L"G");
			Single[st][t].AddString(L"H");
            Single[st][t].SetCurSel(0);

		    compak_current_table[st][t].t_type = COMPAK_SINGLE;
            compak_current_table[st][t].t_one = 0;
		    compak_current_table[st][t].t_two = 0;
		}
	}
	eraseTable(NULL);
	displayTable(NULL);

	UpdateData(FALSE);
    
	return (1);
}

BOOL CEnterCompakDlg::OnEraseBkgnd(CDC* pDC)
{
	BOOL release_dc = FALSE;
    CRect rect;
    GetClientRect(&rect);
	if( pDC == NULL )
	{
        pDC = GetDC( );
		release_dc = TRUE;
	}
    //CBrush myBrush(RGB(255, 255, 255));    // dialog background color
    CBrush myBrush(BKG_GREY);    // dialog background color
    CBrush *pOld = pDC->SelectObject(&myBrush);
    BOOL bRes  = pDC->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
    pDC->SelectObject(pOld);    // restore old brush

	eraseTable(NULL);
	displayTable(NULL);

	return TRUE;
}

void CEnterCompakDlg::eraseTable(CDC* dc)
{
	BOOL release_dc = FALSE;

    if( dc == NULL )
    {
        dc = GetDC( );
        release_dc = TRUE;
	}

	CBrush brush1;   // Must initialize!
    brush1.CreateSolidBrush(BKG_GREY);  
	
    CBrush* pTempBrush = NULL;
    CBrush OrigBrush;

    pTempBrush = (CBrush*)dc->SelectObject(&brush1);
    // Save original brush.
    OrigBrush.FromHandle((HBRUSH)pTempBrush);
    
    for( UINT s = 1; s < 5; ++s )
	{
		for( UINT t = 0; t < 5; ++t )
		{
            dc->Rectangle(table_x + col_w*t + 3        , table_y + row_h *s + 3,
                          table_x + col_w*t + col_w -35, table_y + row_h *s + row_h - 3);
		}
	}

    //dc->Rectangle(table_x - 10, table_y - 10, table_x + col_w*5 + 10, table_y + row_h*5 + 10);    

	//deselect your objects to avoid memory leaks
	dc->SelectObject(&OrigBrush);
    //dc->SelectObject(pOldBrush);

	if(release_dc )
	{
		ReleaseDC(dc);
	}

	return;
}

void CEnterCompakDlg::displayTable(CDC* dc)
{
    UINT x = 0;
	UINT y = 0;
    BOOL release_dc = FALSE;
	COMPAK_TARGET_ST my_target;
	CPen newPen;
	CString compakStr;
 
	if( dc == NULL )
	{
        dc = GetDC( );
		release_dc = TRUE;
	}
    if( newPen.CreatePen( PS_SOLID, 2, BLACK) )
    {
        // Select it into the device context
        // Save the old pen at the same time
        CPen* pOldPen = dc->SelectObject( &newPen );

        // Draw with the pen
		for( UINT row = 0; row < 6; ++row )
		{
			if( row == 4 )
			{
				if( FITASCType >= 1 )
				{
					continue;
				}
			}
			if( row == 2 )
			{
                if( FITASCType >= 3 )
				{
					continue;
				}

			}
		    dc->MoveTo(table_x,        table_y+row*row_h);
            dc->LineTo(table_x+col_w*5,table_y+row*row_h);
		}

		for( UINT col = 0; col < 6; ++col )
		{
		    dc->MoveTo(table_x + col*col_w, table_y        );
            dc->LineTo(table_x + col*col_w, table_y+5*row_h);
		}

		for( UINT t = 0; t < 5; ++t )
		{
			for( UINT s = 0; s < 5; ++s )
			{
    			memcpy(&my_target,&compak_current_table[t][s],sizeof(COMPAK_TARGET_ST));
                switch(my_target.t_type)
				{
				    case COMPAK_SINGLE:
					    //compakStr.Format(L"%s",compak_trap_name[my_target.t_one-1]);
            			//dc->TextOutW(table_x + col_w*t + col_w/3 + col_w/8, table_y + row_h *s + row_h/4, compakStr );
						break;
					case COMPAK_D_SIGN:
					case COMPAK_D_SIMU:
						compakStr.Format(L"%s - ",compak_trap_name[my_target.t_one-1]);
           			    dc->TextOutW(table_x + col_w*t + col_w/6, table_y + row_h *s + row_h/4, compakStr );
						break;
					case COMPAK_T_NONE:
					default:
					    continue;
					    break;
				}
 			}
		}

		// Restore the old pen to the device context
        dc->SelectObject( pOldPen );
    

    } 
    if( release_dc )
	{
		ReleaseDC(dc);
	}

	if( FITASCType < 1 )
	{
	GetDlgItem(IDC_SINGLE_5_1)->EnableWindow(true);
	GetDlgItem(IDC_SINGLE_5_2)->EnableWindow(true);
	GetDlgItem(IDC_SINGLE_5_3)->EnableWindow(true);
	GetDlgItem(IDC_SINGLE_5_4)->EnableWindow(true);
	GetDlgItem(IDC_SINGLE_5_5)->EnableWindow(true);
	}
	else
	{
	GetDlgItem(IDC_SINGLE_5_1)->EnableWindow(false);
	GetDlgItem(IDC_SINGLE_5_2)->EnableWindow(false);
	GetDlgItem(IDC_SINGLE_5_3)->EnableWindow(false);
	GetDlgItem(IDC_SINGLE_5_4)->EnableWindow(false);
	GetDlgItem(IDC_SINGLE_5_5)->EnableWindow(false);
	}

	if( FITASCType < 3 )
	{
	GetDlgItem(IDC_SINGLE_4_1)->EnableWindow(true);
	GetDlgItem(IDC_SINGLE_4_2)->EnableWindow(true);
	GetDlgItem(IDC_SINGLE_4_3)->EnableWindow(true);
	GetDlgItem(IDC_SINGLE_4_4)->EnableWindow(true);
	GetDlgItem(IDC_SINGLE_4_5)->EnableWindow(true);
	}
	else
	{
	GetDlgItem(IDC_SINGLE_4_1)->EnableWindow(false);
	GetDlgItem(IDC_SINGLE_4_2)->EnableWindow(false);
	GetDlgItem(IDC_SINGLE_4_3)->EnableWindow(false);
	GetDlgItem(IDC_SINGLE_4_4)->EnableWindow(false);
	GetDlgItem(IDC_SINGLE_4_5)->EnableWindow(false);
	}
	return;
}

// CAudioSamplesDlg dialog used for display audio samples

class CAudioSamplesDlg : public CDialog
{
public:
	CAudioSamplesDlg();
    CAudioSamplesDlg(CString name);
    CString m_Name;
	void displayGraph(CDC* dc);
	
// Dialog Data
	enum { IDD = IDD_AUDIO_SAMPLES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog( );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnWindowPosChanged( WINDOWPOS* lpwndpos );
};

CAudioSamplesDlg::CAudioSamplesDlg() : CDialog(CAudioSamplesDlg::IDD)
{
	EnableActiveAccessibility();
}

CAudioSamplesDlg::CAudioSamplesDlg(CString name) : CDialog(CAudioSamplesDlg::IDD)
{
	m_Name = name;
	EnableActiveAccessibility();
}

void CAudioSamplesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAudioSamplesDlg, CDialog)
	ON_WM_WINDOWPOSCHANGED( )
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CAudioSamplesDlg::OnInitDialog( )
{
 	//GetDlgItem(IDC_SAMPLES_NAME)->EnableWindow(true);

	UpdateData(FALSE);  

	return (1);
}

BOOL CAudioSamplesDlg::OnEraseBkgnd(CDC* pDC)
{
	BOOL release_dc = FALSE;
    CRect rect;
    GetClientRect(&rect);
	if( pDC == NULL )
	{
        pDC = GetDC( );
		release_dc = TRUE;
	}
    //CBrush myBrush(RGB(255, 255, 255));    // dialog background color
    CBrush myBrush(BKG_GREY);    // dialog background color
    CBrush *pOld = pDC->SelectObject(&myBrush);
    BOOL bRes  = pDC->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
    pDC->SelectObject(pOld);    // restore old brush


	displayGraph(NULL);

	return TRUE;
}
#define SHIFT_RIGHT  0

void CAudioSamplesDlg::OnWindowPosChanged( WINDOWPOS* lpwndpos )
{
    displayGraph(NULL);
}

void CAudioSamplesDlg::displayGraph(CDC* dc)
{
    UINT x = 0;
	UINT y = 0;
    BOOL release_dc = FALSE;
	CPen newPenRed;
    CPen newPenYellow;
	CPen newPenBlack;
	CPen newPenGreen;
	CPen newPenWhite;
	CPen* pOldPen = NULL;


	if( dc == NULL )
	{
        dc = GetDC( );
		release_dc = TRUE;
	}
    if( newPenRed.CreatePen( PS_SOLID, 2, RED) )
    {
        // Select it into the device context
        // Save the old pen at the same time
        pOldPen = dc->SelectObject( &newPenRed );
        // Draw with the pen
		dc->MoveTo(20,295-(active_samples[0]>>SHIFT_RIGHT));
		for( UINT s = 1; s < 512; s++ )
	    {
		    x = 20+s;
		    y = active_samples[s]>>SHIFT_RIGHT;
         
	        dc->LineTo(x,295-y);
	    }
	}
	
	if( newPenYellow.CreatePen( PS_SOLID, 2, YELLOW) )
    {
	    dc->SelectObject( &newPenYellow );
		dc->MoveTo(20,295-(noise_samples[0]>>SHIFT_RIGHT));
		for( UINT s = 1; s < 512; s++ )
	    {
		    x = 20+s;
		    y = noise_samples[s]>>SHIFT_RIGHT;
            dc->LineTo(x,295-y);
	    }
	}    

	if( newPenGreen.CreatePen( PS_SOLID, 2, GREEN) )
    {
        dc->SelectObject( &newPenGreen );
        dc->MoveTo(20,295-(GlobalThreshold>>SHIFT_RIGHT));
        dc->LineTo(20+256*2,295-(GlobalThreshold>>SHIFT_RIGHT));
	}

	if( newPenWhite.CreatePen( PS_SOLID, 2, WHITE) )
    {
        dc->SelectObject( &newPenWhite );
        dc->MoveTo(20,295-(NoiseThreshold>>SHIFT_RIGHT));
        dc->LineTo(20+256*2,295-(NoiseThreshold>>SHIFT_RIGHT));
	}
    
	if( newPenBlack.CreatePen( PS_SOLID, 2, BLACK) )
    {
        dc->SelectObject( &newPenBlack );
		dc->MoveTo(20,295-(BackgroundNoise>>SHIFT_RIGHT));
        dc->LineTo(20+256*2,295-(BackgroundNoise>>SHIFT_RIGHT));
		dc->MoveTo(20,295);
        dc->LineTo(20+256*2,295);
		dc->MoveTo(20,295-(255>>SHIFT_RIGHT));
        dc->LineTo(20+256*2,295-(255>>SHIFT_RIGHT));
	}		
	// Restore the old pen to the device context
    if( pOldPen != NULL )
	{
	    dc->SelectObject( pOldPen );
	}
    if( release_dc )
	{
		ReleaseDC(dc);
	}

	return;
}

// CAdvancedDlg dialog used for display advansed settings

class CAdvancedDlg : public CDialog
{
public:
	CAdvancedDlg();
    CAdvancedDlg(CString name);
    CString m_Name;
	void displayGraph(CDC* dc);
	
// Dialog Data
	enum { IDD = IDD_ADVANCED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog( );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

CAdvancedDlg::CAdvancedDlg() : CDialog(CAdvancedDlg::IDD)
{
	EnableActiveAccessibility();
}

CAdvancedDlg::CAdvancedDlg(CString name) : CDialog(CAdvancedDlg::IDD)
{
	m_Name = name;
	EnableActiveAccessibility();
}

void CAdvancedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAdvancedDlg, CDialog)
	ON_WM_WINDOWPOSCHANGED( )
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CAdvancedDlg::OnInitDialog( )
{
 	//GetDlgItem(IDC_SAMPLES_NAME)->EnableWindow(true);

	UpdateData(FALSE);  

	return (1);
}

BOOL CAdvancedDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

// COnRunModeDlg dialog used for "remote control"

COnRunModeDlg::COnRunModeDlg(CRoundDlg* pParent /*=NULL*/)
    : CDialog(COnRunModeDlg::IDD, pParent)
{
    m_parent=pParent;
}	

void COnRunModeDlg::OnDestroy() 
{	
    CDialog::OnDestroy();
	m_parent->onruncreated = FALSE;
	DestroyWindow();
	delete(this);
}

void COnRunModeDlg::PostNcDestroy() 
{	
    CDialog::PostNcDestroy();
	m_parent->onruncreated = FALSE;
	DestroyWindow();
	delete(this);
}

void COnRunModeDlg::OnCancel() 
{
    DestroyWindow();
}

void COnRunModeDlg::OnOK() 
{
    if(UpdateData(true))
    {
        DestroyWindow();	
    }
}


COnRunModeDlg::COnRunModeDlg() : CDialog(COnRunModeDlg::IDD)
{
	m_CurrentStation.Format(L"%d",m_parent->emb_mike);
	m_CurrentShooter.Format(L"S:%2d",m_parent->targets[m_parent->current_target].shooter+1);
	m_CurrentTarget.Format(L"T:%2d",(m_parent->current_target/m_parent->round_shooters)+1);
	
	UpdateData(FALSE);

	EnableActiveAccessibility();
}

void COnRunModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WAITING_MIKE, m_CurrentStation);
	DDX_Text(pDX, IDC_WAITING_SHOOTER, m_CurrentShooter);
	DDX_Text(pDX, IDC_WAITING_TARGET, m_CurrentTarget);
}

BEGIN_MESSAGE_MAP(COnRunModeDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_ROUND_MINUS, &COnRunModeDlg::OnClickMinus)
	ON_BN_CLICKED(IDC_ROUND_PLUS, &COnRunModeDlg::OnClickPlus)
END_MESSAGE_MAP()

BOOL COnRunModeDlg::Create(CWnd * pParent)
{
    return CDialog::Create( COnRunModeDlg::IDD, pParent );
}

void COnRunModeDlg::OnClickMinus(void)
{
	m_parent->Prev();
}

void COnRunModeDlg::OnClickPlus(void)
{
	m_parent->Next();
}

HBRUSH COnRunModeDlg::OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor)
{
	BOOL release_dc = FALSE;

	GetDlgItem(IDC_WAITING_MIKE)->HideCaret();
	GetDlgItem(IDC_WAITING_SHOOTER)->HideCaret();
	GetDlgItem(IDC_WAITING_TARGET)->HideCaret();

	if( pDC == NULL )
	{
        pDC = GetDC();
		release_dc = TRUE;
	}

	switch (nCtlColor)
    {
        case CTLCOLOR_STATIC:
        case CTLCOLOR_EDIT:
        case CTLCOLOR_BTN:
			pDC->SetTextColor(RED);
			if( release_dc )
			{
				ReleaseDC(pDC);
			}
            return (HBRUSH)GetStockObject(WHITE_BRUSH);
		default:
						if( release_dc )
			{
				ReleaseDC(pDC);
			}
            return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    }
}

BOOL COnRunModeDlg::OnInitDialog( )
{
    LOGFONT lf;                        // Used to create the CFont.
    LOGFONT mf;                        // Used to create the CFont.


    CDialog::OnInitDialog();           // Call default ::OnInitDialog

    memset(&lf, 0, sizeof(LOGFONT));   // Clear out structure.
    lf.lfHeight = 180;                 // Request a 100-pixel-high font
    wcscpy_s(lf.lfFaceName, L"Arial");    //    with face name "Arial".
	m_font.CreateFontIndirect(&lf);    // Create the font.

    memset(&mf, 0, sizeof(LOGFONT));   // Clear out structure.
    mf.lfHeight = 100;                 // Request a 100-pixel-high font
    wcscpy_s(mf.lfFaceName, L"Arial");    //    with face name "Arial".
	m_font2.CreateFontIndirect(&mf);    // Create the font.

    GetDlgItem(IDC_WAITING_MIKE)->SetFont(&m_font);
    GetDlgItem(IDC_WAITING_TARGET)->SetFont(&m_font2);
    GetDlgItem(IDC_WAITING_SHOOTER)->SetFont(&m_font2);
	GetDlgItem(IDC_ROUND_MINUS)->SetFont(&m_font);
    GetDlgItem(IDC_ROUND_PLUS)->SetFont(&m_font);
	GetDlgItem(IDC_WAITING_MIKE)->HideCaret();
	GetDlgItem(IDC_WAITING_SHOOTER)->HideCaret();
	GetDlgItem(IDC_WAITING_TARGET)->HideCaret();

	
	UpdateData(FALSE);
    
	return (1);
}

// CDrawShooterDlg dialog used for drawing semifinalists for shoot-off

class CDrawShooterDlg : public CDialog
{
public:
	CDrawShooterDlg();
    CString m_name[6];

// Dialog Data
	enum { IDD = IDD_DRAW_SHOOTERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    
// Implementation
	afx_msg void OnClickDrawBox();
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog( );
	afx_msg void OnKillFocusName( );
};

CDrawShooterDlg::CDrawShooterDlg() : CDialog(CDrawShooterDlg::IDD)
{
	EnableActiveAccessibility();
	srand((unsigned)time(0));
	for( int i = 0; i < 6; ++i )
	{
		m_name[i].Format(L"");
	}
}

void CDrawShooterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME_1, m_name[0]);
	DDX_Text(pDX, IDC_NAME_2, m_name[1]);
	DDX_Text(pDX, IDC_NAME_3, m_name[2]);
	DDX_Text(pDX, IDC_NAME_4, m_name[3]);
	DDX_Text(pDX, IDC_NAME_5, m_name[4]);
	DDX_Text(pDX, IDC_NAME_6, m_name[5]);
}

BEGIN_MESSAGE_MAP(CDrawShooterDlg, CDialog)
		ON_BN_CLICKED(IDC_BOOTLOADER, &CDrawShooterDlg::OnClickDrawBox)
		ON_EN_KILLFOCUS(IDC_NAME_1, &CDrawShooterDlg::OnKillFocusName)
		ON_EN_KILLFOCUS(IDC_NAME_2, &CDrawShooterDlg::OnKillFocusName)
		ON_EN_KILLFOCUS(IDC_NAME_3, &CDrawShooterDlg::OnKillFocusName)
		ON_EN_KILLFOCUS(IDC_NAME_4, &CDrawShooterDlg::OnKillFocusName)
		ON_EN_KILLFOCUS(IDC_NAME_5, &CDrawShooterDlg::OnKillFocusName)
		ON_EN_KILLFOCUS(IDC_NAME_6, &CDrawShooterDlg::OnKillFocusName)
END_MESSAGE_MAP()

BOOL CDrawShooterDlg::OnInitDialog( )
{
	CDialog::OnInitDialog();

	UpdateData(FALSE);
    
	return (1);
}

void CDrawShooterDlg::OnKillFocusName()
{ 
	UpdateData(TRUE); 
}

void CDrawShooterDlg::OnClickDrawBox(void)
{
	CString s;
    INT random_value;
    CString tmp_name[6];
	INT tmp_array[6] = {0,1,2,3,4,5};

    for(int i = 0; i < 6; ++i )
	{
		if( m_name[i] == L"" )
		{
			s.Format(MLSTR_NO_NAME_FOR_SHOOTER,i+1);
            AfxMessageBox(s);
			return;
		}
		for( int j = i+1; j < 6; ++j )
		{
			if( m_name[i] == m_name[j] )
			{
			    s.Format(MLSTR_SAME_NAME_FOR_SHOOTERS,i+1,j+1);
                AfxMessageBox(s);
			    return;
			}
		}
	}
    
	for( int i = 6, j = 0; (i > 0) && (j < 6); --i, ++j )
	{
		random_value = rand()%i;
		tmp_name[j] =m_name[tmp_array[random_value]];
	    for( int k = random_value; k < i-1; ++k )
	    {
		    tmp_array[k] = tmp_array[k+1];
	    }
	}
#if 0
	random_value = rand()%6;
    tmp_name[0] = m_name[tmp_array[random_value]];

	for( int j = random_value; j < 5; ++j )
	{
		tmp_array[j] = tmp_array[j+1];
	}

	random_value = rand()%5;
    tmp_name[1] = m_name[tmp_array[random_value]];

	for( int j = random_value; j < 4; ++j )
	{
		tmp_array[j] = tmp_array[j+1];
	}

	random_value = rand()%4;
    tmp_name[2] = m_name[tmp_array[random_value]];

	for( int j = random_value; j < 3; ++j )
	{
		tmp_array[j] = tmp_array[j+1];
	}

	random_value = rand()%3;
    tmp_name[3] = m_name[tmp_array[random_value]];

	for( int j = random_value; j < 2; ++j )
	{
		tmp_array[j] = tmp_array[j+1];
	}

	random_value = rand()%2;
    tmp_name[4] = m_name[tmp_array[random_value]];

	for( int j = random_value; j < 1; ++j )
	{
		tmp_array[j] = tmp_array[j+1];
	}

    tmp_name[5] = m_name[tmp_array[0]];
#endif

	for( int j=0; j < 6; ++j )
	{
		m_name[j] = tmp_name[j];
	}

	UpdateData(FALSE);
}

class CStatisticsDlg : public CDialog
{
public:
	CStatisticsDlg();
    CString m_vzBoxRevision;
// Dialog Data
	enum { IDD = IDD_STATISTICS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog( );
};

CStatisticsDlg::CStatisticsDlg() : CDialog(CStatisticsDlg::IDD)
{
	EnableActiveAccessibility();
}

void CStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStatisticsDlg, CDialog)
END_MESSAGE_MAP()

BOOL CStatisticsDlg::OnInitDialog( )
{
	CDialog::OnInitDialog();
	UpdateData(FALSE);
    
	return (1);
}

// App command to run the dialog
void CBunkerControlApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CBunkerControlApp::OnAppRunBootloader()
{
	CBootloaderDlg bootloaderDlg;
	bootloaderDlg.DoModal();
}

void CBunkerControlApp::OnAppResetBox()
{
	CResetBoxDlg resetBoxDlg;
	resetBoxDlg.DoModal();
}

void CBunkerControlApp::OnAppConvertFirmware()
{
	CConvertFirmwareDlg convertFirmwareDlg;
	convertFirmwareDlg.DoModal();    
}

void CBunkerControlApp::OnAppProgramFirmware()
{
	CProgramFirmwareDlg programFirmwareDlg;
	programFirmwareDlg.DoModal();    
}


void CBunkerControlApp::OnAppLoadFirmware()
{
	CLoadFirmwareDlg loadFirmwareDlg;
	loadFirmwareDlg.DoModal();    
}

void CBunkerControlApp::OnAppDrawShooters()
{
	CDrawShooterDlg drawShootersDlg;
	drawShootersDlg.DoModal();
}

// CBunkerControlApp message handlers
CRoundDlg::CRoundDlg() : CDialog(CRoundDlg::IDD)
{
    round_type = ROUND_TYPE_NONE;

	EnableActiveAccessibility();
}

CRoundDlg::CRoundDlg(UINT compak_round_type) : CDialog(CRoundDlg::IDQ)
{
	CompakPicture = TRUE;
    round_type = (ROUND_TYPE_EN)compak_round_type;
    in_pause = FALSE;
	compak_stand_targets = 0;
    round_targets  = max_targets[round_type];
    round_shooters = max_shooters[round_type];
    round_stations = max_stations[round_type];
	round_shots_per_shooter = max_shots_per_shooter[round_type];
	memset(shooter_style,0,sizeof(shooter_style));
    onruncreated = FALSE;

	m_pEditBkBrush = new CBrush(BKG_GREY);

    for( UINT s = 0; s < 6; ++s )
    {
        shooters_present[s] = shooters_default[round_type][s];
    }    
    noise_check = TRUE;
	voice_check = TRUE;
	nobird_check = TRUE;
	sound_check = FALSE;
	timer_check = FALSE;
	select_mike = 0;
	select_trap1 = 0;
	select_trap2 = 0;
	select_compak_trap = 0;
    select_compak_trap1 = 0;
    select_compak_trap2 = 0;
	current_target = 0;

    emb_trap1 = 0;
    emb_trap2 = 0;
    emb_mike = 0;
    last_target_shot = 0;
	emb_first_of_signal = 0;

	EmbUn0.Format(L"%d",Un0);
	EmbUn1.Format(L"%d",Un1);
	EmbUn2.Format(L"%d",Un2);
	EmbUn3.Format(L"%d",Un3);
	EmbUn4.Format(L"%d",Un4);
	EmbUn5.Format(L"%d",Un5);
	EmbUn6.Format(L"%d",Un6);
	EmbUn7.Format(L"%d",Un7);

    emb_ver_major = 0; 
    emb_ver_minor = 0; 
    emb_reset_mikes = 0;

    EmbVersion.Format(MLSTR_UNKNOWN);
    EmbHardware.Format(MLSTR_UNKNOWN);
    
	SerialNumber = 0;

	InitRound(TRUE);

    DrawStatus();

	if( !Connected )
	{
	    Connected = FindAndOpenRawhid();
	}
	EnableActiveAccessibility();

}

CRoundDlg::CRoundDlg(ROUND_TYPE_EN par_round_type) : CDialog(CRoundDlg::IDD)
{
	CompakPicture = FALSE;
    round_type = par_round_type;
    in_pause = FALSE;
    round_targets  = max_targets[round_type];
    round_shooters = max_shooters[round_type];
    round_stations = max_stations[round_type];
	round_shots_per_shooter = max_shots_per_shooter[round_type];
	compak_stand_targets = 0;
	memset(shooter_style,0,sizeof(shooter_style));
    onruncreated = FALSE;
	
	m_pEditBkBrush = new CBrush(BKG_GREY);

    for( UINT s = 0; s < 6; ++s )
    {
        shooters_present[s] = shooters_default[round_type][s];
    }    
    noise_check = TRUE;
	voice_check = TRUE;
	nobird_check = TRUE;
	sound_check = FALSE;
	timer_check = FALSE;
	select_mike = 0;
	select_trap1 = 0;
	select_trap2 = 0;
	select_compak_trap = 0;
    select_compak_trap1 = 0;
    select_compak_trap2 = 0;

	current_target = 0;

    emb_trap1 = 0;
    emb_trap2 = 0;
    emb_mike = 0;
    last_target_shot = 0;
	emb_first_of_signal = 0;

	EmbUn0.Format(L"%d",Un0);
	EmbUn1.Format(L"%d",Un1);
	EmbUn2.Format(L"%d",Un2);
	EmbUn3.Format(L"%d",Un3);
	EmbUn4.Format(L"%d",Un4);
	EmbUn5.Format(L"%d",Un5);
	EmbUn6.Format(L"%d",Un6);
	EmbUn7.Format(L"%d",Un7);

    emb_ver_major = 0; 
    emb_ver_minor = 0; 
    emb_reset_mikes = 0;

    EmbVersion.Format(MLSTR_UNKNOWN);
    EmbHardware.Format(MLSTR_UNKNOWN);
    
	SerialNumber = 0;

	InitRound(TRUE);

    DrawStatus();

	if( !Connected )
	{
	    Connected = FindAndOpenRawhid();
	}
	EnableActiveAccessibility();

}

void CRoundDlg::PostNcDestroy() 
{	
    CDialog::PostNcDestroy();
	if( onruncreated )
	{
		onRunModeDlg->DestroyWindow(); 
	   //delete(onRunModeDlg);
       onruncreated = FALSE;
	}
}

void CRoundDlg::OnDestroy()
{
   CDialog::OnDestroy();
   // Free the space allocated for the background brush
   if( Connected )
   {
       rawhid_close(0);
       Connected = FALSE;
   }
   delete m_pEditBkBrush;
   if(onruncreated)
   {
	   onRunModeDlg->DestroyWindow();
	   //delete(onRunModeDlg);
	   onruncreated = FALSE;
   }
}

HBRUSH CRoundDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    BOOL release_dc = FALSE;

	if( pDC == NULL )
	{
        pDC = GetDC();
		release_dc = TRUE;
	}

    switch (nCtlColor) {

        case CTLCOLOR_STATIC:
              // Set color to green on black and return the background
              //brush.
			
            pDC->SetBkColor(BKG_GREY);
			//return (hbr);
			if( release_dc )
			{
				ReleaseDC(pDC);
			}
            return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
	}
	if( release_dc )
	{
		ReleaseDC(pDC);
	}
	return hbr;

}
	

BOOL CRoundDlg::OnEraseBkgnd(CDC* pDC)
{
	BOOL release_dc = FALSE;
    CRect rect;
    GetClientRect(&rect);
	if( pDC == NULL )
	{
        pDC = GetDC( );
		release_dc = TRUE;
	}
    //CBrush myBrush(RGB(255, 255, 255));    // dialog background color
    CBrush myBrush(BKG_GREY);    // dialog background color
    CBrush *pOld = pDC->SelectObject(&myBrush);
    BOOL bRes  = pDC->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
    pDC->SelectObject(pOld);    // restore old brush

	SetWindowText(window_title[round_type]);

	DrawGraphics(pDC);

	if( !in_pause )
	{
        m_PauseTimer = SetTimer(1, VZBOX_COMM_TIMER, 0);
	}

	if( release_dc )
	{
		ReleaseDC(pDC);
	}

    return TRUE;
}

void CRoundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_SHOOTER1, shooters_present[0]);
	DDX_Check(pDX, IDC_SHOOTER2, shooters_present[1]);
	DDX_Check(pDX, IDC_SHOOTER3, shooters_present[2]);
	DDX_Check(pDX, IDC_SHOOTER4, shooters_present[3]);
	DDX_Check(pDX, IDC_SHOOTER5, shooters_present[4]);
	DDX_Check(pDX, IDC_SHOOTER6, shooters_present[5]);
    DDX_Control(pDX, IDC_SHOOTER1, m_Shooters[0]);
    DDX_Control(pDX, IDC_SHOOTER2, m_Shooters[1]);
    DDX_Control(pDX, IDC_SHOOTER3, m_Shooters[2]);
    DDX_Control(pDX, IDC_SHOOTER4, m_Shooters[3]);
    DDX_Control(pDX, IDC_SHOOTER5, m_Shooters[4]);
    DDX_Control(pDX, IDC_SHOOTER6, m_Shooters[5]);

	DDX_Text(pDX, IDC_CURRENT_STATUS, CurrentStatus );
    DDX_Text(pDX, IDC_CURRENT_HARDWARE, CurrentHardware );

	DDX_Text(pDX, IDC_RELEASE_LEVEL, ReleaseLevel );
#if NOISE_THRESHOLD
	DDX_Text(pDX, IDC_NOISE_LEVEL, NoiseLevel );
#endif

	DDX_Control(pDX,IDC_SELECT_MIKE, m_SelectMike );
	if( CompakPicture )
	{
	DDX_Control(pDX,IDC_SELECT_COMPAK_TRAP,  m_SelectCompakTrap  );
	DDX_Control(pDX,IDC_SELECT_COMPAK_TRAP1, m_SelectCompakTrap1 );
	DDX_Control(pDX,IDC_SELECT_COMPAK_TRAP2, m_SelectCompakTrap2 );
	}
	else
	{
	DDX_Control(pDX,IDC_SELECT_TRAP1, m_SelectTrap1 );
	DDX_Control(pDX,IDC_SELECT_TRAP2, m_SelectTrap2 );
	}
	DDX_Control(pDX,IDC_SELECT_PAUSE, m_SelectPause );
	DDX_Control(pDX,IDC_SELECT_RELAY, m_SelectRelay );
	DDX_Check(pDX, IDC_CHECK_NOISE, noise_check );
	DDX_Check(pDX, IDC_CHECK_NOBIRD, nobird_check);
	DDX_Check(pDX, IDC_CHECK_TIMER, timer_check);
	DDX_Check(pDX, IDC_CHECK_SOUND, sound_check);
	DDX_Control(pDX, IDC_CHECK_NOISE, m_NoiseCheck);
    DDX_Control(pDX, IDC_CHECK_NOBIRD, m_NobirdCheck);
    DDX_Control(pDX, IDC_CHECK_SOUND, m_SoundCheck);
    DDX_Control(pDX, IDC_CHECK_TIMER, m_TimerCheck);

	DDX_Check(pDX,IDC_CHECK_VOICE, voice_check );
	DDX_Control(pDX, IDC_CHECK_VOICE, m_VoiceCheck);
	DDX_Control(pDX,IDC_RELEASE_SCROLL, m_ScrollReleaseLevel );
#if NOISE_THRESHOLD
	DDX_Control(pDX,IDC_NOISE_SCROLL, m_ScrollNoiseLevel );
#endif

	if( !CompakPicture )
	{
	DDX_Control(pDX,IDC_BROKEN_TRAP, m_BrokenTrap );
	DDX_Control(pDX,IDC_EXCHAN_TRAP, m_ExchanTrap );
	}

	DDX_Control(pDX,IDC_SH1_STYLE, m_ShooterStyle[0]);
	DDX_Control(pDX,IDC_SH2_STYLE, m_ShooterStyle[1]);
	DDX_Control(pDX,IDC_SH3_STYLE, m_ShooterStyle[2]);
	DDX_Control(pDX,IDC_SH4_STYLE, m_ShooterStyle[3]);
	DDX_Control(pDX,IDC_SH5_STYLE, m_ShooterStyle[4]);
	DDX_Control(pDX,IDC_SH6_STYLE, m_ShooterStyle[5]);

#if TUNING_ENABLED
	DDX_Text(pDX, IDC_UN0_VALUE,    EmbUn0    );
	DDX_Text(pDX, IDC_UN1_VALUE,    EmbUn1    );
	DDX_Text(pDX, IDC_UN2_VALUE,    EmbUn2    );
	DDX_Text(pDX, IDC_UN3_VALUE,    EmbUn3    );
	DDX_Text(pDX, IDC_UN4_VALUE,    EmbUn4    );
	DDX_Text(pDX, IDC_UN5_VALUE,    EmbUn5    );
	DDX_Text(pDX, IDC_UN6_VALUE,    EmbUn6    );
	DDX_Text(pDX, IDC_UN7_VALUE,    EmbUn7    );

	DDX_Text(pDX, IDC_DBG0, EmbDbg0 );
	DDX_Text(pDX, IDC_DBG1, EmbDbg1 );
	DDX_Text(pDX, IDC_DBG2, EmbDbg2 );
	DDX_Text(pDX, IDC_DBG3, EmbDbg3 );
	DDX_Text(pDX, IDC_DBG4, EmbDbg4 );
	DDX_Text(pDX, IDC_DBG5, EmbDbg5 );
	DDX_Text(pDX, IDC_DBG6, EmbDbg6 );
	DDX_Text(pDX, IDC_DBG7, EmbDbg7 );

	DDX_Text(pDX, IDC_MIKE1_VALUE, Mike1 );
	DDX_Text(pDX, IDC_MIKE2_VALUE, Mike2 );
	DDX_Text(pDX, IDC_MIKE3_VALUE, Mike3 );
	DDX_Text(pDX, IDC_MIKE4_VALUE, Mike4 );
	DDX_Text(pDX, IDC_MIKE5_VALUE, Mike5 );

#endif
}

BEGIN_MESSAGE_MAP(CRoundDlg, CDialog)

	//ON_WM_INITDIALOG()
    ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
    ON_WM_VSCROLL() 
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_DEVICECHANGE, &CRoundDlg::OnUsbDeviceChange)
	ON_BN_CLICKED(IDC_SHOOTER1, &CRoundDlg::OnEnChangeShooter1)
    ON_BN_CLICKED(IDC_SHOOTER2, &CRoundDlg::OnEnChangeShooter2)
    ON_BN_CLICKED(IDC_SHOOTER3, &CRoundDlg::OnEnChangeShooter3)
    ON_BN_CLICKED(IDC_SHOOTER4, &CRoundDlg::OnEnChangeShooter4)
    ON_BN_CLICKED(IDC_SHOOTER5, &CRoundDlg::OnEnChangeShooter5)
    ON_BN_CLICKED(IDC_SHOOTER6, &CRoundDlg::OnEnChangeShooter6)

    ON_BN_CLICKED(IDC_ROUND_SAVE, &CRoundDlg::Save)
    ON_BN_CLICKED(IDC_ROUND_LOAD, &CRoundDlg::Load2)

    ON_BN_CLICKED(IDC_ONRUN, &CRoundDlg::OnClickOnRun)
	ON_BN_CLICKED(IDC_AUDIO_SAMPLES, &CRoundDlg::OnClickAudioSamples)
	ON_BN_CLICKED(IDC_ROUND_START, &CRoundDlg::OnClickStart)
    ON_BN_CLICKED(IDC_ROUND_PULL, &CRoundDlg::TargetRelease)
    ON_BN_CLICKED(IDC_ROUND_PLUS, &CRoundDlg::Next)
    ON_BN_CLICKED(IDC_ROUND_MINUS, &CRoundDlg::Prev)
    ON_BN_CLICKED(IDC_RESET_MIKES, &CRoundDlg::ResetMikes)
	ON_BN_CLICKED(IDC_PAUSE, &CRoundDlg::OnClickPause)
    ON_BN_CLICKED(IDC_RESUME, &CRoundDlg::OnClickResume)
	ON_BN_CLICKED(IDC_CHECK_NOISE, &CRoundDlg::OnClickCheckNoise)
	ON_BN_CLICKED(IDC_CHECK_VOICE, &CRoundDlg::OnClickCheckVoice)
    ON_BN_CLICKED(IDC_CHECK_NOBIRD, &CRoundDlg::OnClickCheckNoBird)
	ON_BN_CLICKED(IDC_CHECK_SOUND, &CRoundDlg::OnClickCheckSound)
	ON_BN_CLICKED(IDC_CHECK_TIMER, &CRoundDlg::OnClickCheckTimer)

	ON_CBN_CLOSEUP(IDC_SELECT_MIKE, &CRoundDlg::OnClickSelectMike)
	ON_CBN_CLOSEUP(IDC_BROKEN_TRAP, &CRoundDlg::OnClickBrokenTrap)
	ON_CBN_CLOSEUP(IDC_EXCHAN_TRAP, &CRoundDlg::OnClickExchanTrap)
	ON_CBN_CLOSEUP(IDC_SELECT_TRAP1, &CRoundDlg::OnClickSelectTrap1)
    ON_CBN_CLOSEUP(IDC_SELECT_TRAP2, &CRoundDlg::OnClickSelectTrap2)
	ON_CBN_CLOSEUP(IDC_SELECT_COMPAK_TRAP, &CRoundDlg::OnClickSelectCompakTrap)
	ON_CBN_CLOSEUP(IDC_SELECT_COMPAK_TRAP1, &CRoundDlg::OnClickSelectCompakTrap1)
	ON_CBN_CLOSEUP(IDC_SELECT_COMPAK_TRAP2, &CRoundDlg::OnClickSelectCompakTrap2)

	//ON_CBN_CLOSEUP(IDC_CHECK_NOISE, &CRoundDlg::OnClickCheckNoise)
    //ON_CBN_CLOSEUP(IDC_CHECK_NOBIRD, &CRoundDlg::OnClickCheckNoBird)
    ON_CBN_CLOSEUP(IDC_SELECT_PAUSE, &CRoundDlg::OnClickSelectPause)
    ON_CBN_CLOSEUP(IDC_SELECT_RELAY, &CRoundDlg::OnClickSelectRelay)
    ON_CBN_CLOSEUP(IDC_SH1_STYLE, &CRoundDlg::OnClickSh1Style)
    ON_CBN_CLOSEUP(IDC_SH2_STYLE, &CRoundDlg::OnClickSh2Style)
    ON_CBN_CLOSEUP(IDC_SH3_STYLE, &CRoundDlg::OnClickSh3Style)
    ON_CBN_CLOSEUP(IDC_SH4_STYLE, &CRoundDlg::OnClickSh4Style)
    ON_CBN_CLOSEUP(IDC_SH5_STYLE, &CRoundDlg::OnClickSh5Style)
    ON_CBN_CLOSEUP(IDC_SH6_STYLE, &CRoundDlg::OnClickSh6Style)

	ON_EN_CHANGE(IDC_UN0_VALUE, &CRoundDlg::OnEnChangeUn)
	ON_EN_CHANGE(IDC_UN1_VALUE, &CRoundDlg::OnEnChangeUn)
	ON_EN_CHANGE(IDC_UN2_VALUE, &CRoundDlg::OnEnChangeUn)
	ON_EN_CHANGE(IDC_UN3_VALUE, &CRoundDlg::OnEnChangeUn)
	ON_EN_CHANGE(IDC_UN4_VALUE, &CRoundDlg::OnEnChangeUn)
	ON_EN_CHANGE(IDC_UN5_VALUE, &CRoundDlg::OnEnChangeUn)
	ON_EN_CHANGE(IDC_UN6_VALUE, &CRoundDlg::OnEnChangeUn)
	ON_EN_CHANGE(IDC_UN7_VALUE, &CRoundDlg::OnEnChangeUn)
    

	ON_EN_KILLFOCUS(IDC_UN0_VALUE, &CRoundDlg::OnKillFocusUn0)
	ON_EN_KILLFOCUS(IDC_UN1_VALUE, &CRoundDlg::OnKillFocusUn1)
	ON_EN_KILLFOCUS(IDC_UN2_VALUE, &CRoundDlg::OnKillFocusUn2)
	ON_EN_KILLFOCUS(IDC_UN3_VALUE, &CRoundDlg::OnKillFocusUn3)
	ON_EN_KILLFOCUS(IDC_UN4_VALUE, &CRoundDlg::OnKillFocusUn4)
	ON_EN_KILLFOCUS(IDC_UN5_VALUE, &CRoundDlg::OnKillFocusUn5)
	ON_EN_KILLFOCUS(IDC_UN6_VALUE, &CRoundDlg::OnKillFocusUn6)
	ON_EN_KILLFOCUS(IDC_UN7_VALUE, &CRoundDlg::OnKillFocusUn7)

END_MESSAGE_MAP()

void CRoundDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if( * pScrollBar == m_ScrollReleaseLevel )
    { 
        switch (nSBCode)
        {
            // Include code that checks for other values of nScrollCode.
            // ...
		    case SB_THUMBTRACK:
              OnMoveScroll(5);
              //nNewPos = nPos + si.nMin; // Adding si.nMin is the workaround.
              break;
            case SB_THUMBPOSITION:
			  OnReleaseScroll(5);
              //nNewPos = nPos + si.nMin; // Adding si.nMin is the workaround.
              break;
        }
    }
    else if( * pScrollBar == m_ScrollNoiseLevel )
    { 
        switch (nSBCode)
        {
            // Include code that checks for other values of nScrollCode.
            // ...
		    case SB_THUMBTRACK:
              OnMoveScroll(6);
              //nNewPos = nPos + si.nMin; // Adding si.nMin is the workaround.
              break;
            case SB_THUMBPOSITION:
			  OnReleaseScroll(6);
              //nNewPos = nPos + si.nMin; // Adding si.nMin is the workaround.
              break;
        }
    }
}

void CRoundDlg::OnEnChangeShooter( UINT sh )
{
    INT status = m_Shooters[sh].GetCheck();
    if( status == BST_UNCHECKED )
    {
        shooters_present[sh] = FALSE;
    }
    else if( status == BST_CHECKED )
    {
        shooters_present[sh] = TRUE;
    }
}

void CRoundDlg::OnEnChangeShooter1() { OnEnChangeShooter(0); } 
void CRoundDlg::OnEnChangeShooter2() { OnEnChangeShooter(1); } 
void CRoundDlg::OnEnChangeShooter3() { OnEnChangeShooter(2); } 
void CRoundDlg::OnEnChangeShooter4() { OnEnChangeShooter(3); } 
void CRoundDlg::OnEnChangeShooter5() { OnEnChangeShooter(4); } 
void CRoundDlg::OnEnChangeShooter6() { OnEnChangeShooter(5); } 

void CRoundDlg::OnClickShStyle(UINT sh)
{
	UINT current_select = m_ShooterStyle[sh].GetCurSel();
    shooter_style[sh] = current_select;
	m_ShooterStyle[sh].SetCurSel(shooter_style[sh]);
}

void CRoundDlg::OnClickSh1Style() { OnClickShStyle(0); }
void CRoundDlg::OnClickSh2Style() { OnClickShStyle(1); }
void CRoundDlg::OnClickSh3Style() { OnClickShStyle(2); }
void CRoundDlg::OnClickSh4Style() { OnClickShStyle(3); }
void CRoundDlg::OnClickSh5Style() { OnClickShStyle(4); }
void CRoundDlg::OnClickSh6Style() { OnClickShStyle(5); }


void CRoundDlg::OnClickStart()
{   
	if( in_pause )
	{
		return;
	}
    InitRound(TRUE); 
    DrawGraphics(NULL);

    EmbSetConfig(FALSE); 
}

void CRoundDlg::OnClickAudioSamples()
{
    CAudioSamplesDlg audioSamplesDialog;
    audioSamplesDialog.DoModal();    
}

void CRoundDlg::OnClickOnRun()
{   
#if 0
    CStatisticsDlg statisticsDialog;
    statisticsDialog.DoModal();    
    return;
#endif

	if( !onruncreated )
	{
	    if( (round_type > ROUND_TYPE_NONE) && 
		    (round_type < ROUND_TYPE_MAX)  && 
		    (round_type != ROUND_TYPE_TEST) &&
			(round_type != ROUND_TYPE_COMTEST))
		{
	        //onRunModeDlg.DoModal();
			onRunModeDlg = new COnRunModeDlg(this);
	        onRunModeDlg->Create();
			onruncreated = TRUE;
			GetDlgItem(IDC_ONRUN)->EnableWindow(false);
			UpdateData(FALSE);
	    }
	}
}

void CRoundDlg::OnClickPause()
{
	Paused = TRUE;
	
	GetDlgItem(IDC_PAUSE)->EnableWindow(false);
	GetDlgItem(IDC_RESUME)->EnableWindow(true);

	LastReleaseTime = 0;
    DrawTimerLed(NULL, GREY);

	UpdateData(FALSE);
}

void CRoundDlg::OnClickResume()
{ 
	Paused = FALSE;
	
	GetDlgItem(IDC_PAUSE)->EnableWindow(true);
	GetDlgItem(IDC_RESUME)->EnableWindow(false);

	LastReleaseTime = time(0);
    DrawTimerLed(NULL, GREY);
	
	UpdateData(FALSE);
}

void CRoundDlg::DrawGraphics(CDC* pDC)
{
	BOOL release_dc = FALSE;
    //CWnd::GetDC
    if( pDC == NULL )
    {
		release_dc = TRUE;
        pDC = GetDC( );
    }

    for( UINT trap = 0; trap < MAX_MACHINES; ++trap )
    {
		if( CompakPicture )
		{
			if( trap >= MAX_COMPAK_TRAPS  )
			{
			    continue;
			}

			if( (round_type == ROUND_TYPE_COMPRESET) && ( trap >= 6 ) )
			{
                DrawTrap(pDC,trap,GREY);
			}
			else
			{
                DrawTrap(pDC,trap,WHITE);
			}
			continue;
		}
		if( ((round_type == ROUND_TYPE_TSHOOTOFF) || (round_type == ROUND_TYPE_DSHOOTOFF)) && (ShootoffStation != 0)  )
		{
		    DrawTrap(pDC,trap,trap_colors[ROUND_TYPE_TF][trap]);
		}
		else
		{
		    DrawTrap(pDC,trap,trap_colors[round_type][trap]);
		}
		if( round_type == ROUND_TYPE_TQ )
		{
			if( trap+1 == BrokenTrap )
			{
                DrawTrap(pDC,trap,GREY);
			}
			if( trap+1 == ExchanTrap )
			{
                //DrawTrap(pDC,trap,GREY);
			}
		}
    }
    DrawActiveTrap(pDC,GREEN);

    DrawStatus();

    for( UINT mike = 0; mike < MAX_STATIONS; ++mike )
    {
        DrawMike(pDC,mike,mike_colors[round_type][mike]);
    }

    DrawActiveMike(pDC,GREEN);

    GetDlgItem(IDC_ROUND_PULL)->EnableWindow(true);
  	GetDlgItem(IDC_RELEASE_SCROLL)->EnableWindow(true);
  	GetDlgItem(IDC_CHECK_NOISE)->EnableWindow(true);
	GetDlgItem(IDC_CHECK_VOICE)->EnableWindow(true);
#if TUNING_ENABLED
	GetDlgItem(IDC_UN0_VALUE)->EnableWindow(true);
	GetDlgItem(IDC_UN1_VALUE)->EnableWindow(true);
	GetDlgItem(IDC_UN2_VALUE)->EnableWindow(true);
	GetDlgItem(IDC_UN3_VALUE)->EnableWindow(true);
	GetDlgItem(IDC_UN4_VALUE)->EnableWindow(true);
	GetDlgItem(IDC_UN5_VALUE)->EnableWindow(true);
    GetDlgItem(IDC_UN6_VALUE)->EnableWindow(true);
    GetDlgItem(IDC_UN7_VALUE)->EnableWindow(true);
#endif
	GetDlgItem(IDC_CHECK_NOISE)->EnableWindow(true);
	GetDlgItem(IDC_CHECK_VOICE)->EnableWindow(true);
    GetDlgItem(IDC_CHECK_NOBIRD)->EnableWindow(true);
    GetDlgItem(IDC_CHECK_SOUND)->EnableWindow(true);
	GetDlgItem(IDC_CHECK_TIMER)->EnableWindow(true);

	if( (round_type == ROUND_TYPE_DTQ) || (round_type == ROUND_TYPE_TQ) )
    {
		GetDlgItem(IDC_SH1_STYLE)->EnableWindow(true); 
		GetDlgItem(IDC_SH2_STYLE)->EnableWindow(true); 
		GetDlgItem(IDC_SH3_STYLE)->EnableWindow(true); 
		GetDlgItem(IDC_SH4_STYLE)->EnableWindow(true); 
		GetDlgItem(IDC_SH5_STYLE)->EnableWindow(true); 
		GetDlgItem(IDC_SH6_STYLE)->EnableWindow(true); 

        GetDlgItem(IDC_BROKEN_TRAP)->EnableWindow(true);
		if( BrokenTrap )
		{
            GetDlgItem(IDC_EXCHAN_TRAP)->EnableWindow(true);
		}
		else
		{
            GetDlgItem(IDC_EXCHAN_TRAP)->EnableWindow(false);
		}
    }
    else
	{
		GetDlgItem(IDC_SH1_STYLE)->EnableWindow(false); 
		GetDlgItem(IDC_SH2_STYLE)->EnableWindow(false); 
		GetDlgItem(IDC_SH3_STYLE)->EnableWindow(false); 
		GetDlgItem(IDC_SH4_STYLE)->EnableWindow(false); 
		GetDlgItem(IDC_SH5_STYLE)->EnableWindow(false); 
		GetDlgItem(IDC_SH6_STYLE)->EnableWindow(false); 
        if( !CompakPicture )
		{
        GetDlgItem(IDC_BROKEN_TRAP)->EnableWindow(false);
        GetDlgItem(IDC_EXCHAN_TRAP)->EnableWindow(false);
		}
    }

    if( round_type == ROUND_TYPE_TEST )
    {
        GetDlgItem(IDC_SELECT_MIKE)->EnableWindow(true);
        GetDlgItem(IDC_SELECT_TRAP1)->EnableWindow(true);
        GetDlgItem(IDC_SELECT_TRAP2)->EnableWindow(true);
        GetDlgItem(IDC_ROUND_START)->EnableWindow(false);
		GetDlgItem(IDC_ROUND_PLUS)->EnableWindow(false);
        GetDlgItem(IDC_ROUND_MINUS)->EnableWindow(false);
        GetDlgItem(IDC_ROUND_SAVE)->EnableWindow(false);
        GetDlgItem(IDC_ROUND_LOAD)->EnableWindow(false);
		GetDlgItem(IDC_ONRUN)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_TIMER)->EnableWindow(false);
 	}
	else if( round_type == ROUND_TYPE_COMTEST )
	{
        GetDlgItem(IDC_SELECT_MIKE)->EnableWindow(true);
        GetDlgItem(IDC_SELECT_COMPAK_TRAP1)->EnableWindow(true);
        GetDlgItem(IDC_SELECT_COMPAK_TRAP2)->EnableWindow(true);

        if( (select_compak_trap1 > 0) &&
			(select_compak_trap2 > 0) &&
			(select_compak_trap1 != select_compak_trap2) )
		{
		    GetDlgItem(IDC_SELECT_COMPAK_TRAP)->EnableWindow(true);
		}
		else
		{
		    GetDlgItem(IDC_SELECT_COMPAK_TRAP)->EnableWindow(false);
		}
        GetDlgItem(IDC_ROUND_START)->EnableWindow(false);
		GetDlgItem(IDC_ROUND_PLUS)->EnableWindow(false);
        GetDlgItem(IDC_ROUND_MINUS)->EnableWindow(false);
        GetDlgItem(IDC_ROUND_SAVE)->EnableWindow(false);
        GetDlgItem(IDC_ROUND_LOAD)->EnableWindow(false);
		GetDlgItem(IDC_ONRUN)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_TIMER)->EnableWindow(false);
	}
	else if( (round_type == ROUND_TYPE_TSHOW) || 
		     (round_type == ROUND_TYPE_DSHOW) )
    {
        GetDlgItem(IDC_SELECT_MIKE)->EnableWindow(false);
        GetDlgItem(IDC_SELECT_TRAP1)->EnableWindow(false);
        GetDlgItem(IDC_SELECT_TRAP2)->EnableWindow(false);
        GetDlgItem(IDC_ROUND_START)->EnableWindow(true);
		GetDlgItem(IDC_ROUND_PLUS)->EnableWindow(true);
        GetDlgItem(IDC_ROUND_MINUS)->EnableWindow(true);
        GetDlgItem(IDC_ROUND_LOAD)->EnableWindow(false);
		GetDlgItem(IDC_ROUND_SAVE)->EnableWindow(false);
		if( onruncreated )
		{
		    GetDlgItem(IDC_ONRUN)->EnableWindow(false);
		}
		else
		{
		    GetDlgItem(IDC_ONRUN)->EnableWindow(true);
		}
		GetDlgItem(IDC_CHECK_TIMER)->EnableWindow(false);
	}
	else
    {
        GetDlgItem(IDC_SELECT_MIKE)->EnableWindow(false);
		if( CompakPicture )
		{
		GetDlgItem(IDC_SELECT_COMPAK_TRAP)->EnableWindow(false);
        GetDlgItem(IDC_SELECT_COMPAK_TRAP1)->EnableWindow(false);
        GetDlgItem(IDC_SELECT_COMPAK_TRAP2)->EnableWindow(false);
        GetDlgItem(IDC_ROUND_LOAD)->EnableWindow(false);
		GetDlgItem(IDC_ROUND_SAVE)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_TIMER)->EnableWindow(false);
		}
		else
		{
        GetDlgItem(IDC_SELECT_TRAP1)->EnableWindow(false);
        GetDlgItem(IDC_SELECT_TRAP2)->EnableWindow(false);
        GetDlgItem(IDC_ROUND_LOAD)->EnableWindow(true);
		GetDlgItem(IDC_ROUND_SAVE)->EnableWindow(true);
		}
		GetDlgItem(IDC_ROUND_START)->EnableWindow(true);
        GetDlgItem(IDC_ROUND_PLUS)->EnableWindow(true);
        GetDlgItem(IDC_ROUND_MINUS)->EnableWindow(true);
		if( onruncreated )
		{
		    GetDlgItem(IDC_ONRUN)->EnableWindow(false);
		}
		else
		{
		    GetDlgItem(IDC_ONRUN)->EnableWindow(true);
		}
    }

    GetDlgItem(IDC_SHOOTER1)->EnableWindow(shooters_available[round_type][0]);
    GetDlgItem(IDC_SHOOTER2)->EnableWindow(shooters_available[round_type][1]);
    GetDlgItem(IDC_SHOOTER3)->EnableWindow(shooters_available[round_type][2]);
    GetDlgItem(IDC_SHOOTER4)->EnableWindow(shooters_available[round_type][3]);
    GetDlgItem(IDC_SHOOTER5)->EnableWindow(shooters_available[round_type][4]);
    GetDlgItem(IDC_SHOOTER6)->EnableWindow(shooters_available[round_type][5]);

	if( (round_type == ROUND_TYPE_TEST) || (round_type == ROUND_TYPE_COMTEST) )
	{
        GetDlgItem(IDC_PAUSE)->EnableWindow(false);
	    GetDlgItem(IDC_RESUME)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_TIMER)->EnableWindow(false);
	}
	else
	{
		if( Paused )
	    {
            GetDlgItem(IDC_PAUSE)->EnableWindow(false);
	        GetDlgItem(IDC_RESUME)->EnableWindow(true);
		}
		else
		{
            GetDlgItem(IDC_PAUSE)->EnableWindow(true);
	        GetDlgItem(IDC_RESUME)->EnableWindow(false);
		}
	}

	//LastReleaseTime = time(0);
    DrawTimerLed(NULL, GREY);

	UpdateData(FALSE);

	if( release_dc )
	{
		ReleaseDC(pDC);
	}
}

BOOL CRoundDlg::OnInitDialog( )
{
	CDialog::OnInitDialog();
	CString str;

	str.Format(MLSTR_NONE);

	m_SelectMike.AddString( str );
	if( CompakPicture )
	{
	//m_SelectCompakTrap.AddString( str );
    m_SelectCompakTrap1.AddString( str );
    m_SelectCompakTrap2.AddString( str );
	}
	else
	{
    m_SelectTrap1.AddString( str );
    m_SelectTrap2.AddString( str );
	m_BrokenTrap.AddString( str );
	m_ExchanTrap.AddString( str );
	}
    for (int i=1 ;i < 16 ;i++)
    {
       str.Format(L"%2d", i);
	   if( i < 6 )
	   {
           m_SelectMike.AddString( str );
	   }
	   if( CompakPicture )
	   {
	   }
	   else
	   {
       m_SelectTrap1.AddString( str );
       m_SelectTrap2.AddString( str );
	   m_BrokenTrap.AddString( str );
	   }
	}

	if( CompakPicture )
	{
	   m_SelectCompakTrap.AddString( MLSTR_SIMULTANEOUS );
       m_SelectCompakTrap.AddString( MLSTR_ON_REPORT );
       m_SelectCompakTrap1.AddString( L"A" );
       m_SelectCompakTrap1.AddString( L"B" );
       m_SelectCompakTrap1.AddString( L"C" );
       m_SelectCompakTrap1.AddString( L"D" );
       m_SelectCompakTrap1.AddString( L"E" );
       m_SelectCompakTrap1.AddString( L"F" );
       m_SelectCompakTrap1.AddString( L"G" );
       m_SelectCompakTrap1.AddString( L"H" );
       m_SelectCompakTrap2.AddString( L"A" );
       m_SelectCompakTrap2.AddString( L"B" );
       m_SelectCompakTrap2.AddString( L"C" );
       m_SelectCompakTrap2.AddString( L"D" );
       m_SelectCompakTrap2.AddString( L"E" );
       m_SelectCompakTrap2.AddString( L"F" );
       m_SelectCompakTrap2.AddString( L"G" );
       m_SelectCompakTrap2.AddString( L"H" );
	}

    m_SelectMike.SetCurSel(0);
if( CompakPicture )
{	
	m_SelectCompakTrap1.SetCurSel(0);
	m_SelectCompakTrap2.SetCurSel(0);
	m_SelectCompakTrap.SetCurSel(0);
}
else
{
	m_SelectTrap1.SetCurSel(0);
	m_SelectTrap2.SetCurSel(0);

	m_BrokenTrap.SetCurSel(0);
	m_ExchanTrap.SetCurSel(0);
}

	//m_SelectPause.AddString(MLSTR_1_0_SEC);
	//m_SelectPause.AddString(MLSTR_1_5_SEC);
	m_SelectPause.AddString(MLSTR_2_0_SEC);
	//m_SelectPause.AddString(MLSTR_2_5_SEC);
	m_SelectPause.AddString(MLSTR_3_0_SEC);
	//m_SelectPause.AddString(MLSTR_3_5_SEC);
	m_SelectPause.AddString(MLSTR_4_0_SEC);
	//m_SelectPause.AddString(MLSTR_4_5_SEC);
	m_SelectPause.AddString(MLSTR_5_0_SEC);
	//m_SelectPause.AddString(MLSTR_5_5_SEC);
	m_SelectPause.AddString(MLSTR_6_0_SEC);
	//m_SelectPause.AddString(MLSTR_6_5_SEC);
	m_SelectPause.AddString(MLSTR_7_0_SEC);
	//m_SelectPause.AddString(MLSTR_7_5_SEC);
	m_SelectPause.AddString(MLSTR_8_0_SEC);
	m_SelectPause.AddString(MLSTR_9_0_SEC);
	m_SelectPause.AddString(MLSTR_10_0_SEC);
	m_SelectPause.AddString(MLSTR_11_0_SEC);
	m_SelectPause.AddString(MLSTR_12_0_SEC);

	m_SelectPause.SetCurSel(SelectPause);

	m_SelectRelay.AddString(MLSTR_OFF);
	m_SelectRelay.AddString(MLSTR_100_MSEC);
	m_SelectRelay.AddString(MLSTR_200_MSEC);
	m_SelectRelay.AddString(MLSTR_300_MSEC);
	m_SelectRelay.AddString(MLSTR_400_MSEC);
	//m_SelectRelay.AddString(MLSTR_500_MSEC);
	//m_SelectRelay.AddString(MLSTR_600_MSEC);

	m_SelectRelay.SetCurSel(SelectRelay);

	m_ScrollReleaseLevel.SetScrollRange(0,MAX_VOLUME,TRUE);
#if NOISE_THRESHOLD
	m_ScrollNoiseLevel.SetScrollRange(0,MAX_VOLUME,TRUE);
#endif
	UserThreshold = ((GlobalThreshold - BackgroundNoise)*MAX_VOLUME)/(MAX_VOLUME-BackgroundNoise);
	m_ScrollReleaseLevel.SetScrollPos(MAX_VOLUME-UserThreshold,TRUE);
#if NOISE_THRESHOLD
	AirThreshold = ((NoiseThreshold - BackgroundNoise)*MAX_VOLUME)/(MAX_VOLUME-BackgroundNoise);
	m_ScrollNoiseLevel.SetScrollPos(MAX_VOLUME-AirThreshold,TRUE);
#endif


	for( int s = 0; s < 6; ++s )
	{
		m_ShooterStyle[s].AddString(MLSTR_AUTO);
    	if( round_type == ROUND_TYPE_TQ )
	    {
			m_ShooterStyle[s].AddString(MLSTR_RIGHT);
			m_ShooterStyle[s].AddString(MLSTR_STRAIGHT);
			m_ShooterStyle[s].AddString(MLSTR_LEFT);
	    }
	    else if( round_type == ROUND_TYPE_DTQ )
	    {
			m_ShooterStyle[s].AddString(L"A(7-8)");
			m_ShooterStyle[s].AddString(L"B(8-9)");
			m_ShooterStyle[s].AddString(L"C(7-9)");
	    }
		m_ShooterStyle[s].SetCurSel(0);
	}
	if( round_type != ROUND_TYPE_TQ )
	{
		BrokenTrap = 0;
        ExchanTrap = 0;
	}
	else
	{
	    m_BrokenTrap.SetCurSel(BrokenTrap);
        m_ExchanTrap.SetCurSel(ExchanTrap);
	}

	UpdateData(FALSE);

    EmbSetConfig(FALSE);    

	return (1);
}


void CRoundDlg::DrawActiveMike(CDC* pDC,COLORREF color)
{
	BOOL release_dc = FALSE;

	if( last_target_shot )
	{
		emb_mike = 0;
		return;
	}
	if( pDC == NULL )
    {
        pDC = GetDC( );
        release_dc = TRUE;
	}

	if( (round_type == ROUND_TYPE_TEST) || (round_type == ROUND_TYPE_COMTEST) )
	{
        if( select_mike > 0 )
		{
            DrawMike(pDC,select_mike-1,color);
		}
	}
	else
	{
        DrawMike(pDC,targets[current_target].station,color);
	}

	if( release_dc )
	{
		ReleaseDC(pDC);
	}
}

void CRoundDlg::DrawActiveTrap(CDC* pDC,COLORREF color)
{
	BOOL release_dc = FALSE;
    UINT active_trap = targets[current_target].machine;
	COMPAK_TARGET_ST active_compak_target = targets[current_target].compak_trap;
    emb_trap1 = 0;
    emb_trap2 = 0;
	emb_first_of_signal = 0;
	if( last_target_shot )
	{
		return;
	}

    if( pDC == NULL )
    {
        pDC = GetDC( );
		release_dc = TRUE;
    }

	if( round_type == ROUND_TYPE_TQ )
	{
		if( active_trap > 0)
		{
		    UINT style = shooter_style[targets[current_target].shooter];
		    switch( style )
		    {
			    case 1: // right
				    active_trap = ((active_trap-1)/3)*3 + 1; 
			        break;
			    case 2: //straight
				    active_trap = ((active_trap-1)/3)*3 + 2; 
					break;
			    case 3: //left
				    active_trap = ((active_trap-1)/3)*3 + 3; 
					break;
			    default:
				    break;
			}
		}
        if( (active_trap > 0) && (active_trap == BrokenTrap) )
		{
			if( ExchanTrap )
			{
			    active_trap = ExchanTrap;
			}
            else if( (BrokenTrap % 3) == 1 ) // right
			{
				active_trap += 2;
			}
            else if( (BrokenTrap % 3) == 2 ) //straight
			{
				active_trap += 1;
			}
            else                              //left
			{
				active_trap -= 2;
			}
		}
	}
	else if( round_type == ROUND_TYPE_DTQ )
	{
		if( active_trap > 0)
		{
			UINT style = shooter_style[targets[current_target].shooter];
		    switch( style )
		    {
			    case 1: // 7-8
				    active_trap = 1; 
			        break;
			    case 2: //8-9
				    active_trap = 2; 
					break;
			    case 3: //7-9
				    active_trap = 3; 
					break;
			    default:
				    break;
			}
		}
	}

	switch (round_type)
    {
        case ROUND_TYPE_NONE:
            break;
        case ROUND_TYPE_DTQ:
        case ROUND_TYPE_DTF:
        case ROUND_TYPE_DTM:
		case ROUND_TYPE_DOP:
		case ROUND_TYPE_DSHOW:
		case ROUND_TYPE_DSHOOTOFF:

            if( active_trap == 0 )
            {
            }
            else if( active_trap == 1 )
            {
				if( color == GREEN )
	            {
					emb_trap1 = 7;
					emb_trap2 = 8;
	            }
                DrawTrap(pDC,6,color);
                DrawTrap(pDC,7,color);
            }
            else if( active_trap == 2 )
            {
				if( color == GREEN )
	            {
					emb_trap1 = 9;
					emb_trap2 = 8;
	            }

				DrawTrap(pDC,7,color);
                DrawTrap(pDC,8,color);
            }
            else if( active_trap == 3 )
            {
				if( color == GREEN )
	            {
					emb_trap1 = 7;
					emb_trap2 = 9;
	            }

                DrawTrap(pDC,6,color);
                DrawTrap(pDC,8,color);
            }
            break;
        case ROUND_TYPE_TQ:
        case ROUND_TYPE_TF:
        case ROUND_TYPE_TM:
		case ROUND_TYPE_TOP:
        case ROUND_TYPE_TSHOW:
		case ROUND_TYPE_TSHOOTOFF:
		case ROUND_TYPE_MAX:
			if( color == GREEN )
            {
                emb_trap1 = active_trap;
            }
            if( active_trap == 0 )
            {

            }
			else
			{
                DrawTrap(pDC,active_trap-1,color);
			}
            break;
		case ROUND_TYPE_TEST:
			if( select_trap1 > 0 )
			{
    			if( color == GREEN )
                {
		    		emb_trap1 = select_trap1;
                }

				DrawTrap(pDC,select_trap1-1,color);
			}
			if( select_trap2 > 0 )
			{
    			if( color == GREEN )
                {
		    		emb_trap2 = select_trap2;
                }

				DrawTrap(pDC,select_trap2-1,color);
			}
		    break;
		
		case ROUND_TYPE_COMTEST:
			if( select_compak_trap1 > 0 )
			{
    			if( color == GREEN )
                {
		    		emb_trap1 = select_compak_trap1;
					if( select_compak_trap > 0 )
					{
						if( !FirstSignalReleased )
						{
					        emb_first_of_signal = 1;
						}
					}
                }
                if( FirstSignalReleased )
    			{
		    		emb_trap1 = select_compak_trap2;
		    		emb_trap2 = 0;
					emb_first_of_signal = 0;
				}
				DrawTrap(pDC,select_compak_trap1-1,color);
			}
			if( select_compak_trap2 > 0 )
			{
    			if( color == GREEN )
                {
                    if( select_compak_trap > 0 )
					{
						color = YELLOW;
						if( !FirstSignalReleased )
						{
					        emb_first_of_signal = 1;
						}
					}
					else
					{
    			        emb_trap2 = select_compak_trap2-1;
				    }
				}
				else if( color == RED )
				{
                   if( FirstSignalReleased )
				   {
                       color = GREEN; 
				   }
				}
                if( FirstSignalReleased )
    			{
				    emb_first_of_signal = 0;
				}
				DrawTrap(pDC,select_compak_trap2-1,color);
			}
			break;
		case ROUND_TYPE_COMPRESET:
		case ROUND_TYPE_COMCONTROL:
			if( active_compak_target.t_one > 0 )
			{
    			if( color == GREEN )
                {
		    		emb_trap1 = active_compak_target.t_one;
					if( active_compak_target.t_type == COMPAK_D_SIGN )
					{
						if( !FirstSignalReleased )
						{
					        emb_first_of_signal = 1;
						}
					}
					if( active_compak_target.t_type == COMPAK_E_SHOW )
					{
						color = BLACK;
					}
                }
                if( FirstSignalReleased )
    			{
		    		emb_trap1 = active_compak_target.t_two;
		    		emb_trap2 = 0;
					emb_first_of_signal = 0;
				}
				DrawTrap(pDC,active_compak_target.t_one-1,color);
			}
			if( active_compak_target.t_two > 0 )
			{
    			if( color == GREEN )
                {
                    if( active_compak_target.t_type == COMPAK_D_SIGN )
					{
						color = YELLOW;
						if( !FirstSignalReleased )
						{
					        emb_first_of_signal = 1;
						}
					}
					else
					{
    			        emb_trap2 = active_compak_target.t_two-1;
				    }
				}
				else if( color == RED )
				{
                   if( FirstSignalReleased )
				   {
                       color = GREEN; 
				   }
				}
                if( FirstSignalReleased )
    			{
				    emb_first_of_signal = 0;
				}
				DrawTrap(pDC,active_compak_target.t_two-1,color);
			}
			break;
		default:
            break;
    }

	if( release_dc )
	{
		ReleaseDC(pDC);
	}
}

void CRoundDlg::DrawTrap(CDC* dc, UINT trap, COLORREF color)
{
	BOOL release_dc = FALSE;
    UINT x;

	if( dc == NULL )
    {
        dc = GetDC( );
		release_dc = TRUE;
    }

	CBrush brush1;   // Must initialize!
    brush1.CreateSolidBrush(color);  
	
    CBrush* pTempBrush = NULL;
    CBrush OrigBrush;

    pTempBrush = (CBrush*)dc->SelectObject(&brush1);
    // Save original brush.
    OrigBrush.FromHandle((HBRUSH)pTempBrush);
	//CBrush fill(color);
    //CBrush * pOldBrush;

    //pOldBrush = dc->SelectObject(&fill);

	if( CompakPicture )
	{
        x = 23 + 50*trap;

        dc->Ellipse(x,136,x+20,156);    
	}
	else
	{
        x = (trap*20+30) + (trap/3)*21;

        dc->Ellipse(x,140,x+14,154);    
	}
    //deselect your objects to avoid memory leaks
	dc->SelectObject(&OrigBrush);
    //dc->SelectObject(pOldBrush);

	if( release_dc )
	{
		ReleaseDC(dc);
	}

}

void CRoundDlg::DrawStatusLed(CDC* dc, COLORREF color, UINT number)
{
	BOOL release_dc = FALSE;
    if( dc == NULL )
    {
		if( IsWindow(m_hWnd) )
		{
	        dc = GetDC( );
			release_dc = TRUE;
			if( dc == NULL )
	        {
				ReleaseDC(dc);
		        return;
	        }
		}
		else
		{
			return;
		}
	}

	CBrush brush1;   // Must initialize!
    brush1.CreateSolidBrush(color);  
	
    CBrush* pTempBrush = NULL;
    CBrush OrigBrush;

	if( dc == NULL )
	{
		return;
	}
	pTempBrush = (CBrush*)dc->SelectObject(&brush1);
    // Save original brush.
    OrigBrush.FromHandle((HBRUSH)pTempBrush);
    //CBrush fill(color);
    //CBrush * pOldBrush;

    //pOldBrush = dc->SelectObject(&fill);
    UINT x = 320 + (number*2*12);
    //dc->Ellipse(105,450,119,464);
	dc->Ellipse(x,483,x+14,497);
    //dc->Ellipse(320,483,334,497);
    //deselect your objects to avoid memory leaks
	dc->SelectObject(&OrigBrush);
    //dc->SelectObject(pOldBrush);
	if( release_dc )
	{
		ReleaseDC(dc);
	}

}

void CRoundDlg::DrawTimerLed(CDC* dc, COLORREF color)
{
	BOOL release_dc = FALSE;
    if( dc == NULL )
    {
		if( IsWindow(m_hWnd) )
		{
	        dc = GetDC( );
			release_dc = TRUE;
			if( dc == NULL )
	        {
				ReleaseDC(dc);
		        return;
	        }
		}
		else
		{
			return;
		}
	}

	CBrush brush1;   // Must initialize!
    brush1.CreateSolidBrush(color);  
	
    CBrush* pTempBrush = NULL;
    CBrush OrigBrush;

	if( dc == NULL )
	{
		return;
	}
	pTempBrush = (CBrush*)dc->SelectObject(&brush1);
    // Save original brush.
    OrigBrush.FromHandle((HBRUSH)pTempBrush);
    //CBrush fill(color);
    //CBrush * pOldBrush;

    //pOldBrush = dc->SelectObject(&fill);
    UINT x = 400;
	UINT y = 452;
	//dc->Ellipse(105,450,119,464);
	dc->Ellipse(x,y,x+14,y+14);
    //dc->Ellipse(320,483,334,497);
    //deselect your objects to avoid memory leaks
	dc->SelectObject(&OrigBrush);
    //dc->SelectObject(pOldBrush);
	if( release_dc )
	{
		ReleaseDC(dc);
	}

}

void CRoundDlg::DrawMike(CDC* dc, UINT mike, COLORREF color)
{
	BOOL release_dc = FALSE;

    if( dc == NULL )
    {
        dc = GetDC( );
        release_dc = TRUE;
	}

	CBrush brush1;   // Must initialize!
    brush1.CreateSolidBrush(color);  
	
    CBrush* pTempBrush = NULL;
    CBrush OrigBrush;

    pTempBrush = (CBrush*)dc->SelectObject(&brush1);
    // Save original brush.
    OrigBrush.FromHandle((HBRUSH)pTempBrush);
    
	//CBrush fill(color);
    //CBrush * pOldBrush;
	//CBrush OrigBrush;

    //pOldBrush = dc->SelectObject(&fill);

    UINT x = mike*81+42;
	UINT y = 200;
    dc->Rectangle(x,y,x+25,y+25);    
    //deselect your objects to avoid memory leaks
	dc->SelectObject(&OrigBrush);
    //dc->SelectObject(pOldBrush);

	if( color == GREEN )
	{
        emb_mike = mike+1;
		if(onruncreated)
		{
		    onRunModeDlg->m_CurrentStation.Format(L"%d",emb_mike);
			onRunModeDlg->m_CurrentShooter.Format(L"S:%2d",targets[current_target].shooter+1);
			onRunModeDlg->m_CurrentTarget.Format(L"T:%2d",(current_target/round_shooters)+1);
			onRunModeDlg->UpdateData(FALSE);
		}
	}
	else
	{
		if( (wait_for_shot1 == 0) && (!FirstSignalReleased) )
		{
            emb_mike = 0;
		}
	}
	if(release_dc )
	{
		ReleaseDC(dc);
	}

}

void CRoundDlg::DrawStatus()
{
  	if( round_type == ROUND_TYPE_TEST )
    {
        CurrentStatus.Format(MLSTR_FULL_MANUAL_CONTROL_MODE);
    }
	else if( round_type == ROUND_TYPE_COMTEST )
    {
        CurrentStatus.Format(MLSTR_COMPAK_MANUAL_CONTROL_MODE);
    }
	else if( (round_type == ROUND_TYPE_TSHOW) ||
             (round_type == ROUND_TYPE_DSHOW) )
	{
        CurrentStatus.Format(MLSTR_SHOW_TARGETS);
	}
    else
    {
        CurrentStatus.Format(MLSTR_CURRENT_TARGET,
                           current_target+1, 
                           targets[current_target].shooter+1,
						   (current_target/round_shooters)+1,
                           targets[current_target].station+1,
                           targets[current_target].trap
                       );
    }

    if( Connected )
    {
		DrawStatusLed(NULL, GREEN, 0);
		CurrentHardware.Format(MLSTR_CURRENT_HARDWARE,EmbHardware,EmbVersion,SerialNumber, 
                               tx_counter,rx_counter);
	}
    else
    {
		DrawStatusLed(NULL, RED, 0);
		for( UINT led = 1; led < 8; ++led )
		{
    		DrawStatusLed(NULL, GREY, led);
		}
   		CurrentHardware.Format(MLSTR_HW_DISCONNECTED);
	}
	ReleaseLevel.Format(L" %4d",GlobalThreshold);
	NoiseLevel.Format(L" %4d",NoiseThreshold);
}

void CRoundDlg::InitRound(BOOL random)
{
    srand((unsigned)time(0));
    CString s;
    CEnterShooterDlg enterShootersDlg;
	CEnterFITASCDlg enterTableDlg;
	CEnterCompakDlg enterCompakDlg;
    UINT num_of_shooters = 0;

	if( random )
	{
	    memset(randoms,0xff,sizeof(randoms));
	}

    if( sound_check )
	{ 
        Beep(500,200);
	}

	for( UINT athlet = 0; athlet < MAX_SHOOTERS; ++athlet )
    {
        if(shooters_present[athlet])
        {
            num_of_shooters++;
        }
   }
   
   if( num_of_shooters < min_shooters[round_type] )
   {
      s.Format(MLSTR_TOO_FEW_SHOOTERES);
	  AfxMessageBox(s);
      return;
    }
    memset(targets,0,sizeof(targets));
	statistics.stat_round_type = round_type;
	statistics.stat_start_time = time(0);

	for( UINT a = 0; a < MAX_SHOOTERS; ++a )
	{
		for( UINT t = 0; t < MAX_TARGETS; ++t )
		{
			statistics.stat_target[a][t].stat_shooter_num = a+1;
			statistics.stat_target[a][t].stat_target_num = t+1;
			statistics.stat_target[a][t].stat_direction  = stat_directions[0];
			statistics.stat_target[a][t].stat_result = STAT_RESULT_NO;
			statistics.stat_target[a][t].stat_first_shot_usec = 0;
			statistics.stat_target[a][t].stat_second_shot_usec = 0;
			statistics.stat_target[a][t].stat_preparation_sec = 0;
		}
	}

    switch (round_type)
    {
        case ROUND_TYPE_NONE:
            break;
        case ROUND_TYPE_DTQ:
            for( UINT shooter = 0; shooter < round_shooters; ++shooter )
            {
                for( UINT station = 0; station < round_stations; ++station )
                {
					if( random )
					{
                        randoms[shooter][station] = rand()%DTQ_OPTIONS;
					}
                    for( UINT shot = 0; shot < round_shots_per_shooter/round_stations; ++shot )
                    { 
                        UINT target = (station*round_shooters + 
                                       shot*round_shooters*round_stations + 
                                      ((round_shooters-shooter)*round_stations)%(round_shooters*round_stations))%round_targets;
                        targets[target].target = target;
                        targets[target].shooter = shooter; 
                        targets[target].station = station;
                        targets[target].machine = dtq_options[randoms[shooter][station]][shot];
                        wcscpy_s(targets[target].trap,MAX_TRAP_NAME_LEN,double_targets[targets[target].machine-1]);
                    } 
                }
            }
            break;
        case ROUND_TYPE_DTF:
		case ROUND_TYPE_DTM:
            for( UINT shooter = 0; shooter < round_shooters; ++shooter )
            {
                for( UINT station = 0; station < round_stations; ++station )
                {
					if( random )
					{
                        randoms[shooter][station] = rand()%DTM_OPTIONS;
					}
                    for( UINT shot = 0; shot < round_shots_per_shooter/round_stations; ++shot )
                    { 
						UINT target;
						if( round_type == ROUND_TYPE_DTM )
						{
							target = target_sequence_number_match[shooter][station][shot];
						}
						else
						{
							target = target_sequence_number_semifinal[shooter][station][shot];
						}
                        targets[target].target = target;
                        targets[target].shooter = shooter; 
                        targets[target].station = station+1; //because 0 is skipped
                        targets[target].machine = dtf_options[randoms[shooter][station]][shot];
                        wcscpy_s(targets[target].trap,MAX_TRAP_NAME_LEN,double_targets[targets[target].machine-1]);
                    } 
                }
            }
            break;

        case ROUND_TYPE_TQ:
            for( UINT shooter = 0; shooter < round_shooters; ++shooter )
            {
                for( UINT station = 0; station < round_stations; ++station )
                {
					if( random )
					{
                        randoms[shooter][station] = rand()%TQ_OPTIONS;
					}
                    for( UINT shot = 0; shot < round_shots_per_shooter/round_stations; ++shot )
                    { 
                        UINT target = (station*round_shooters + 
                                       shot*round_shooters*round_stations + 
                                      ((round_shooters-shooter)*round_stations)%(round_shooters*round_stations))%round_targets;
                        targets[target].target = target;
                        targets[target].shooter = shooter; 
                        targets[target].station = station;
                        targets[target].machine = (tq_options[randoms[shooter][station]][shot]+3*station);
                        wcscpy_s(targets[target].trap,MAX_TRAP_NAME_LEN,trap_targets[targets[target].machine-1]);
                    } 
                }
            }
            break;

		case ROUND_TYPE_TF:
			for( UINT shooter = 0; shooter < round_shooters; ++shooter )
            {
                for( UINT station = 0; station < round_stations; ++station )
                {
					if( random )
					{
                        randoms[shooter][station] = rand()%TS_OPTIONS;
					}
					for( UINT shot = 0; shot < round_shots_per_shooter/round_stations; ++shot )
                    { 
                        UINT target = (station*round_shooters + 
                                       shot*round_shooters*round_stations + 
                                      ((round_shooters-shooter)*round_stations)%(round_shooters*round_stations))%round_targets;
                        targets[target].target = target;
                        targets[target].shooter = shooter; 
                        targets[target].station = station;
                        targets[target].machine = (ts_options[randoms[shooter][station]][shot]+3*station);
                        wcscpy_s(targets[target].trap,MAX_TRAP_NAME_LEN,trap_targets[targets[target].machine-1]);
                    } 
                }
            }
            break;

        case ROUND_TYPE_TM:
			for( UINT shooter = 0; shooter < round_shooters; ++shooter )
            {
                for( UINT station = 0; station < round_stations; ++station )
                {
					if( random )
					{
                        randoms[shooter][station] = rand()%TM_OPTIONS;
					}
					for( UINT shot = 0; shot < round_shots_per_shooter/round_stations; ++shot )
                    { 
                        UINT target;
						target = target_sequence_number_match[shooter][station][shot];
                        targets[target].target = target;
						targets[target].shooter = shooter;
                        targets[target].station = station+1;
                        targets[target].machine = (tf_options[randoms[shooter][station]][shot]+3*station) + 3;
                        wcscpy_s(targets[target].trap,MAX_TRAP_NAME_LEN,trap_targets[targets[target].machine-1]);
                    } 
                }
            }
            break;
		case ROUND_TYPE_TSHOOTOFF:

	        enterShootersDlg.DoModal();
            round_shooters = ShootoffShooters;
            round_targets = ShootoffShooters*25;

			for( UINT target = 0; target < round_targets; ++target )
			{
				if( target < round_shooters )
				{
				    shooters_present[target] = TRUE;
				}
                targets[target].target = target;
                targets[target].shooter = (target%round_shooters);
			    targets[target].station = target/round_shooters;
				if( ShootoffStation == 0 )
				{
                    targets[target].station = targets[target].station%5;
				}
				else
				{
                    targets[target].station = targets[target].station%3+1;
				}
			    if( ((target / round_shooters) %2) == 0 )
				{
				    targets[target].machine = targets[target].station*3+3; // left target
				}
				else
				{
				    targets[target].machine = targets[target].station*3+1; // right target
				}
                wcscpy_s(targets[target].trap,MAX_TRAP_NAME_LEN,trap_targets[targets[target].machine-1]);
			}

			break;
		case ROUND_TYPE_DSHOOTOFF:
	        enterShootersDlg.DoModal();
            round_targets = ShootoffShooters*25;
            round_shooters = ShootoffShooters;

			for( UINT target = 0; target < round_targets; ++target )
			{
				if( target < round_shooters )
				{
				    shooters_present[target] = TRUE;
				}
				targets[target].target = target;
                targets[target].shooter = (target%round_shooters);
			    targets[target].station = target/round_shooters;
				if( ShootoffStation == 0 )
				{
                    targets[target].station = targets[target].station%5;
				}
				else
				{
                    targets[target].station = targets[target].station%3+1;
				}
				targets[target].machine = 3; 
                wcscpy_s(targets[target].trap,MAX_TRAP_NAME_LEN,double_targets[2]);
			}

			break;

		case ROUND_TYPE_TEST:
		case ROUND_TYPE_COMTEST:
            break;
	    case ROUND_TYPE_TOP:
            for( UINT shooter = 0; shooter < round_shooters; ++shooter )
            {
                for( UINT station = 0; station < round_stations; ++station )
                {
					if( random )
					{
                        randoms[shooter][station] = rand()%TQ_OPTIONS;
					}
                    for( UINT shot = 0; shot < round_shots_per_shooter/round_stations; ++shot )
                    { 
                        UINT target = shooter + station*25 + shot*5;
                        targets[target].target = target;
                        targets[target].shooter = shooter; 
                        targets[target].station = shooter;
                        targets[target].machine = (tq_options[randoms[shooter][station]][shot]+3*shooter);
                        wcscpy_s(targets[target].trap,MAX_TRAP_NAME_LEN,trap_targets[targets[target].machine-1]);
 					} 
                }
            }
            break;
		case ROUND_TYPE_DOP:
            for( UINT shooter = 0; shooter < round_shooters; ++shooter )
            {
                for( UINT station = 0; station < round_stations; ++station )
                {
					if( random )
					{
                        randoms[shooter][station] = rand()%DTQ_OPTIONS;
					}
                    for( UINT shot = 0; shot < round_shots_per_shooter/round_stations; ++shot )
                    { 
                        UINT target = shooter + station*15 + shot*5;
                        targets[target].target = target;
                        targets[target].shooter = shooter; 
                        targets[target].station = shooter;
                        targets[target].machine = dtq_options[randoms[shooter][station]][shot];
                        wcscpy_s(targets[target].trap,MAX_TRAP_NAME_LEN,double_targets[targets[target].machine-1]);
                    } 
                }
            }
            break;
    	case ROUND_TYPE_MAX:
            break;
		case ROUND_TYPE_TSHOW:
            noise_check = FALSE;
	        voice_check = FALSE;
	        nobird_check = FALSE;
			for( UINT target = 0; target < 15; ++target )
			{
                targets[target].target = target;
                targets[target].shooter = 0; 
                targets[target].station = target/3;
                targets[target].machine = target+1;
			}
			break;
		case ROUND_TYPE_DSHOW:
            noise_check = FALSE;
	        voice_check = FALSE;
			for( UINT target = 0; target < 3; ++target )
			{
                targets[target].target  = target;
                targets[target].shooter = 0; 
                targets[target].station = 0;
                targets[target].machine = dtq_options[0][target];
			}
			break;

		case ROUND_TYPE_COMPRESET:
			enterTableDlg.DoModal();
			memcpy(&compak_current_table,
				   &compak_preset_tables[FITASCTable],
				   sizeof(compak_current_table));
			SetCompakDistribution();
			break;

		case ROUND_TYPE_COMCONTROL:
			enterCompakDlg.DoModal();
			SetCompakDistribution();
			break;

		default:
            break;
    }
    current_target = 0;
	last_target_shot = 0;
    for( UINT t = 0; t < round_shooters; ++t )
    {
        if( shooters_present[targets[t].shooter] )
        { 
            current_target = t;
            break;
        }
    }
    for( UINT t = 0; t < round_targets; ++t )
	{
		statistics.stat_target[targets[t].shooter][targets[t].target/round_shooters].stat_direction = 
		   targets[t].machine;
	}
	if( (round_type == ROUND_TYPE_DTQ) ||     // DT Qual
        (round_type == ROUND_TYPE_DTM) ||     // DT Br/Gold Match
        (round_type == ROUND_TYPE_TQ ) ||     // T Qual
        (round_type == ROUND_TYPE_TF ) ||     // T Quat/Semi
        (round_type == ROUND_TYPE_TM ) ||     // T Br/Gold Match
        (round_type == ROUND_TYPE_TOP) ||
        (round_type == ROUND_TYPE_DOP) )      
	{
		//save distribution
		Save2();
	}
 
}

void CRoundDlg::SetCompakDistribution(void)
{
    UINT compak_target=0;
	CString compakStr;
    COMPAK_TARGET_ST my_target;
    COMPAK_SEQUENCE_ST my_sequence;
	UINT previous_station = 0;
	BOOL add_empty = FALSE;

			round_shooters = FITASCShooters;
			switch(FITASCType)
			{
			    case 0:
                    round_targets  = round_shooters*25;
					compak_stand_targets = 5;
					break;
				case 1:
				case 2:
                    round_targets  = round_shooters*20;
					compak_stand_targets = 4;
					break;
				case 3:
				case 4:
				default:
                    round_targets  = round_shooters*15;
					compak_stand_targets = 3;
					break;
			}

			for( UINT shooter = 0; shooter < round_shooters; ++shooter )
			{
			    shooters_present[shooter] = TRUE;
			}

			compak_target = 0;
			for( UINT round = 0; round < 6; ++round )
			{
				for( UINT target = 0; target < compak_stand_targets; ++target )
				{
					for( UINT pos = 0; pos < 5; ++pos )
					{
				        if( compak_sequence_tables2[round_shooters-1][round][pos].shooter == 0 )
						{
							pos = 5;
							continue;
						}
        				memcpy(&my_sequence,&compak_sequence_tables2[round_shooters-1][round][pos],sizeof(COMPAK_SEQUENCE_ST));
					    memcpy(&my_target,&compak_current_table[my_sequence.station-1][target],sizeof(COMPAK_TARGET_ST));
						add_empty = FALSE;
						if( compak_target > 0 )
						{
                            /* insert show from empty position */
							previous_station = targets[compak_target-1].station;
							if( (previous_station + 1) < 4 )
							{
							    if(my_sequence.station-1 == previous_station+1)
								{
									/* ok */
								}
								else
								{
									/* add empty */
									add_empty = TRUE;
								}
							}
							else
							{
								if(my_sequence.station-1 == 0)
								{
									/* ok */
								}
								else
								{
									/* add empty */
									add_empty = TRUE;
								}
							}
							if( add_empty )
							{
								if( (my_target.t_type == COMPAK_D_SIGN) ||
                                    (my_target.t_type == COMPAK_D_SIGN) )
								{
									targets[compak_target].target = compak_target;
                                    targets[compak_target].shooter = my_sequence.shooter-1;
	    		                    targets[compak_target].station = my_sequence.station-1;
                                     
                                    targets[compak_target].compak_trap.t_type = COMPAK_E_SHOW;
									targets[compak_target].compak_trap.t_one  = my_target.t_one;
                                    targets[compak_target].compak_trap.t_two  = 0;
	    	     				    compakStr.Format(MLSTR_SHOW_SINGLE,compak_trap_name[my_target.t_one-1]);
             					    wcscpy_s(targets[compak_target].trap,MAX_TRAP_NAME_LEN,compakStr);
									compak_target++;
								}
							} 
						}
						targets[compak_target].target = compak_target;
    			    	targets[compak_target].shooter = my_sequence.shooter-1;
	    		        targets[compak_target].station = my_sequence.station-1;

					    memcpy(&targets[compak_target].compak_trap,&compak_current_table[my_sequence.station-1][target],sizeof(COMPAK_TARGET_ST));

    					switch(my_target.t_type)
	    				{
		    			    case COMPAK_SINGLE:
			    			    compakStr.Format(MLSTR_SINGLE,compak_trap_name[my_target.t_one-1]);
				    		    break;
					    	case COMPAK_D_SIGN:
						        compakStr.Format(MLSTR_D_ON_REPORT,compak_trap_name[my_target.t_one-1],compak_trap_name[my_target.t_two-1]);
						        break;
    						case COMPAK_D_SIMU:
	    					    compakStr.Format(MLSTR_D_SIMULTANEOUS,compak_trap_name[my_target.t_one-1],compak_trap_name[my_target.t_two-1]);
								break;
    						case COMPAK_E_SHOW:
	    					    compakStr.Format(MLSTR_SHOW_SINGLE,compak_trap_name[my_target.t_one-1]);
								break;
			    			case COMPAK_T_NONE:
				    		default:
					    	    compakStr.Format(L"Unknown!!!");
						        break;
					    }
					    wcscpy_s(targets[compak_target].trap,MAX_TRAP_NAME_LEN,compakStr);
						compak_target++;
					}
				}
			}
}

void CRoundDlg::TargetRelease(void)
{
	CString s;


	if( in_pause )
	{
		return;
	}
    if( (round_type != ROUND_TYPE_TEST)     && 
		( round_type != ROUND_TYPE_COMTEST) &&
        (current_target >= round_targets) )
        return;

	if( last_target_shot ) //last taget was shot
		return;

	if( round_type == ROUND_TYPE_TEST )
	{
		if( (select_trap1 == 0) && (select_trap2 == 0) )
		{
    		s.Format(MLSTR_TRAPS_NOT_SELECTED);
            AfxMessageBox(s);
            return;
		}
	}
	else if( round_type == ROUND_TYPE_COMTEST )
	{
		if( (select_compak_trap1 == 0) && (select_compak_trap2 == 0) )
		{
    		s.Format(MLSTR_TRAPS_NOT_SELECTED);
            AfxMessageBox(s);
            return;
		}
	}

	if( emb_first_of_signal )
	{
        FirstSignalReleased = TRUE;
	}
	else
	{
        FirstSignalReleased = FALSE;
	}
	
	LastReleaseTime = time(0);
    DrawTimerLed(NULL, GREY);
    KillTimer(m_PauseTimer);
	EmbSetConfig(TRUE);
    if( sound_check )
	{ 
    	Beep(1000,300);
	}
	emb_first_of_signal = 0;
	in_pause = TRUE;
    
	if( nobird_check )
	{
        m_PauseTimer = SetTimer(1, VZBOX_COMM_TIMER, 0);
		wait_for_shot1 = 2500/VZBOX_COMM_TIMER;  //1.5 sec
	}
	else if( FirstSignalReleased )
	{
	    m_PauseTimer = SetTimer(1, FIRST_SIGNAL_TIMER, 0); // wait 5 sec
	}
	else
	{
	    m_PauseTimer = SetTimer(1, 1000*(SelectPause+2), 0);
	}

	GetDlgItem(IDC_ROUND_START)->EnableWindow(false);
	GetDlgItem(IDC_ROUND_PULL)->EnableWindow(false);
	GetDlgItem(IDC_ROUND_PLUS)->EnableWindow(false);
	GetDlgItem(IDC_ROUND_MINUS)->EnableWindow(false);

    DrawActiveTrap(NULL, RED);
    DrawActiveMike(NULL, RED);
}

void CRoundDlg::EmbTargetRelease(void)
{
	if( FirstSignalReleased )
	{

	}
	else if( in_pause )
	{
		return;
	}
    if( current_target > (round_targets-1) )
        return;

	if( emb_first_of_signal )
	{
        FirstSignalReleased = TRUE;
	}
	else
	{
        FirstSignalReleased = FALSE;
	}
    
	in_pause = TRUE;
	LastReleaseTime = time(0);
	DrawTimerLed(NULL, GREY);
    KillTimer(m_PauseTimer);
    emb_first_of_signal = 0;

	if( nobird_check )
	{
        m_PauseTimer = SetTimer(1, VZBOX_COMM_TIMER, 0);
		wait_for_shot1 = 2500/VZBOX_COMM_TIMER;  //1.5 sec
	}
	else if( FirstSignalReleased )
	{
		in_pause = FALSE;
	    m_PauseTimer = SetTimer(1, VZBOX_COMM_TIMER, 0); // wait 5 sec
	}
	else
	{
	    m_PauseTimer = SetTimer(1, 1000*(SelectPause+2), 0);
	}

	EmbSetConfig(FALSE);
    if( sound_check )
	{
	    Beep(1000,300);
	}
	GetDlgItem(IDC_ROUND_START)->EnableWindow(false);
	GetDlgItem(IDC_ROUND_PULL)->EnableWindow(false);
	GetDlgItem(IDC_ROUND_PLUS)->EnableWindow(false);
	GetDlgItem(IDC_ROUND_MINUS)->EnableWindow(false);

    DrawActiveTrap(NULL, RED);
    DrawActiveMike(NULL, RED);
}

void CRoundDlg::EmbTargetNoBird(void)
{
	if( !in_pause )
	{
		// something went wrong - reseived no bird when not in pause
		return;
	}
	//KillTimer(m_PauseTimer);
    if( sound_check )
	{
		MessageBeep(BEEP_NOTARGET);
	}
    GetDlgItem(IDC_ROUND_PULL)->EnableWindow(true);
	if( (round_type != ROUND_TYPE_TEST) && (round_type != ROUND_TYPE_COMTEST) )
	{
	    GetDlgItem(IDC_ROUND_START)->EnableWindow(true);
	    GetDlgItem(IDC_ROUND_PLUS)->EnableWindow(true);
	    GetDlgItem(IDC_ROUND_MINUS)->EnableWindow(true);
	}

	m_PauseTimer = SetTimer(1, VZBOX_COMM_TIMER, 0);
	
    DrawActiveTrap(NULL, GREEN);
    DrawActiveMike(NULL, GREEN);
 	EmbSetConfig(FALSE);
	in_pause = FALSE;
}

void CRoundDlg::Next(void)
{
	UINT t = 0;
    
	if( in_pause )
	{
		return;
	}
    if( current_target > round_targets )
	{
        return;
	}
    if( sound_check )
	{
		Beep(500,200);
	}
    DrawActiveTrap(NULL, WHITE);
    DrawActiveMike(NULL, WHITE);

	LastReleaseTime = time(0);
    DrawTimerLed(NULL, GREY);

	if( FirstSignalReleased )
	{
        FirstSignalReleased = FALSE;
	}

    for( t = (current_target+1); t < round_targets; ++t )
    {
        if( shooters_present[targets[t].shooter] )
        { 
            current_target = t;
            break;
        }
 	}

    if( (round_type == ROUND_TYPE_TEST) || (round_type == ROUND_TYPE_COMTEST) )
	{

	}
	else if( t >= round_targets )
	{
  		last_target_shot = 1;
	}

    DrawActiveTrap(NULL, GREEN);
    DrawActiveMike(NULL, GREEN);
	
    DrawStatus();
    UpdateData(FALSE);
    emb_reset_mikes = 1;
	EmbSetConfig(FALSE);

}


void CRoundDlg::Prev(void)
{
	if( in_pause )
	{
		return;
	}
    if( current_target == 0 )
        return;

	last_target_shot = 0;

    DrawActiveTrap(NULL, WHITE);
    DrawMike(NULL,targets[current_target].station,WHITE);

	LastReleaseTime = time(0);
    DrawTimerLed(NULL, GREY);

 	if( FirstSignalReleased )
	{
        FirstSignalReleased = FALSE;
	}
	for( UINT t = current_target-1; ; --t )
    {
        if( shooters_present[targets[t].shooter] )
        { 
            current_target = t;
            break;
        }
		if( t == 0 )
		{
			break;
		}
    }

    DrawActiveTrap(NULL, GREEN);
    DrawActiveMike(NULL, GREEN);
	
    DrawStatus();
    UpdateData(FALSE);

	EmbSetConfig(FALSE);

}

void CRoundDlg::Save(void)
{
    CString s,Pathname;
    errno_t err;
    FILE *pFile;
	struct tm * start = localtime( & statistics.stat_start_time );

    CString Filters(MLSTR_TEXT_FILES);

    CFileDialog Dlg(FALSE,                 //open/save file dialog
                    L".txt",                //default extension
                    file_name[round_type], //default file name
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    Filters);

    Dlg.m_ofn.lpstrTitle = MLSTR_SAVING_TARGET_DISTRIBUTION;

    if( Dlg.DoModal() == IDOK )
    {
        Pathname=Dlg.GetPathName();

		if( (err=_tfopen_s(&pFile,Pathname.GetString(),L"wt,ccs=UNICODE"))!=0 )
        {
            s.Format(MLSTR_CANNOT_OPEN_FILE,Pathname,err);
            AfxMessageBox(s);
            return;
        }

        fwprintf(pFile,MLSTR_FILE_VERSION);
        fwprintf(pFile,MLSTR_FILE_COPYRIGHT) ;
        fwprintf(pFile,L"==================================\n");
		fwprintf(pFile,L"%d-%s-%2d %2d:%2d\n", 
                          start->tm_year + 1900,
						  monthes[start->tm_mon],
						  start->tm_mday,
						  start->tm_hour,
						  start->tm_min );
		fwprintf(pFile,L"==================================\n");
		fwprintf(pFile,MLSTR_FILE_TARGET_DISTRIBUTION,file_name[round_type]);
        fwprintf(pFile,L"==================================\n");
		if( (round_type == ROUND_TYPE_COMPRESET) || 
			(round_type == ROUND_TYPE_COMCONTROL) )
		{
            fwprintf(pFile,L" Compak Trajectory Setting Table \n");
            fwprintf(pFile,L" --------------------------------------------------- \n");
            fwprintf(pFile,L" |    %s    |    %s    |    %s    |    %s    |    %s    |  \n",
                compak_trap_name[compak_current_table[0][0].t_one-1],
                compak_trap_name[compak_current_table[1][0].t_one-1],
                compak_trap_name[compak_current_table[2][0].t_one-1],
                compak_trap_name[compak_current_table[3][0].t_one-1],
                compak_trap_name[compak_current_table[4][0].t_one-1]);
            fwprintf(pFile,L" --------------------------------------------------- \n");
			if( FITASCType == 0 )
			{
            fwprintf(pFile,L" |    %s    |    %s    |    %s    |    %s    |    %s    |  \n",
                compak_trap_name[compak_current_table[0][1].t_one-1],
                compak_trap_name[compak_current_table[1][1].t_one-1],
                compak_trap_name[compak_current_table[2][1].t_one-1],
                compak_trap_name[compak_current_table[3][1].t_one-1],
                compak_trap_name[compak_current_table[4][1].t_one-1]);
            fwprintf(pFile,L" --------------------------------------------------- \n");
            fwprintf(pFile,L" |    %s    |    %s    |    %s    |    %s    |    %s    |  \n",
                compak_trap_name[compak_current_table[0][2].t_one-1],
                compak_trap_name[compak_current_table[1][2].t_one-1],
                compak_trap_name[compak_current_table[2][2].t_one-1],
                compak_trap_name[compak_current_table[3][2].t_one-1],
                compak_trap_name[compak_current_table[4][2].t_one-1]);
            fwprintf(pFile,L" --------------------------------------------------- \n");
            fwprintf(pFile,L" |    %s    |    %s    |    %s    |    %s    |    %s    |  \n",
                compak_trap_name[compak_current_table[0][3].t_one-1],
                compak_trap_name[compak_current_table[1][3].t_one-1],
                compak_trap_name[compak_current_table[2][3].t_one-1],
                compak_trap_name[compak_current_table[3][3].t_one-1],
                compak_trap_name[compak_current_table[4][3].t_one-1]);
            fwprintf(pFile,L" --------------------------------------------------- \n");
            fwprintf(pFile,L" |    %s    |    %s    |    %s    |    %s    |    %s    |  \n",
                compak_trap_name[compak_current_table[0][4].t_one-1],
                compak_trap_name[compak_current_table[1][4].t_one-1],
                compak_trap_name[compak_current_table[2][4].t_one-1],
                compak_trap_name[compak_current_table[3][4].t_one-1],
                compak_trap_name[compak_current_table[4][4].t_one-1]);
            fwprintf(pFile,L" --------------------------------------------------- \n");
			}
			else if( FITASCType < 3 )
			{
            fwprintf(pFile,L" |    %s    |    %s    |    %s    |    %s    |    %s    |  \n",
                compak_trap_name[compak_current_table[0][1].t_one-1],
                compak_trap_name[compak_current_table[1][1].t_one-1],
                compak_trap_name[compak_current_table[2][1].t_one-1],
                compak_trap_name[compak_current_table[3][1].t_one-1],
                compak_trap_name[compak_current_table[4][1].t_one-1]);
            fwprintf(pFile,L" --------------------------------------------------- \n");
            fwprintf(pFile,L" |    %s    |    %s    |    %s    |    %s    |    %s    |  \n",
                compak_trap_name[compak_current_table[0][2].t_one-1],
                compak_trap_name[compak_current_table[1][2].t_one-1],
                compak_trap_name[compak_current_table[2][2].t_one-1],
                compak_trap_name[compak_current_table[3][2].t_one-1],
                compak_trap_name[compak_current_table[4][2].t_one-1]);
            fwprintf(pFile,L" --------------------------------------------------- \n");
			if( FITASCType == 1 )
			{
			fwprintf(pFile,L" |On Report|On Report|On Report|On Report|On Report|  \n");
			}
			else
			{
			fwprintf(pFile,L" | Simult. | Simult. | Simult. | Simult. | Simult. |  \n");
			}
			fwprintf(pFile,L" |  %s - %s  |  %s - %s  |  %s - %s  |  %s - %s  |  %s - %s  |  \n",
                compak_trap_name[compak_current_table[0][3].t_one-1],
                compak_trap_name[compak_current_table[0][3].t_two-1],
                compak_trap_name[compak_current_table[1][3].t_one-1],
                compak_trap_name[compak_current_table[1][3].t_two-1],
				compak_trap_name[compak_current_table[2][3].t_one-1],
                compak_trap_name[compak_current_table[2][3].t_two-1],
				compak_trap_name[compak_current_table[3][3].t_one-1],
                compak_trap_name[compak_current_table[3][3].t_two-1],
				compak_trap_name[compak_current_table[4][3].t_one-1],
                compak_trap_name[compak_current_table[4][3].t_two-1]);
            fwprintf(pFile,L" --------------------------------------------------- \n");
			}
			else 
			{
            if( FITASCType == 3 )
			{
			fwprintf(pFile,L" |On Report|On Report|On Report|On Report|On Report|  \n");
			}
			else
			{
			fwprintf(pFile,L" | Simult. | Simult. | Simult. | Simult. | Simult. |  \n");
			}
			fwprintf(pFile,L" |  %s - %s  |  %s - %s  |  %s - %s  |  %s - %s  |  %s - %s  |  \n",
                compak_trap_name[compak_current_table[0][1].t_one-1],
                compak_trap_name[compak_current_table[0][1].t_two-1],
                compak_trap_name[compak_current_table[1][1].t_one-1],
                compak_trap_name[compak_current_table[1][1].t_two-1],
				compak_trap_name[compak_current_table[2][1].t_one-1],
                compak_trap_name[compak_current_table[2][1].t_two-1],
				compak_trap_name[compak_current_table[3][1].t_one-1],
                compak_trap_name[compak_current_table[3][1].t_two-1],
				compak_trap_name[compak_current_table[4][1].t_one-1],
                compak_trap_name[compak_current_table[4][1].t_two-1]);
            fwprintf(pFile,L" --------------------------------------------------- \n");
            if( FITASCType == 3 )
			{
			fwprintf(pFile,L" |On Report|On Report|On Report|On Report|On Report|  \n");
			}
			else
			{
			fwprintf(pFile,L" | Simult. | Simult. | Simult. | Simult. | Simult. |  \n");
			}
			fwprintf(pFile,L" |  %s - %s  |  %s - %s  |  %s - %s  |  %s - %s  |  %s - %s  |  \n",
                compak_trap_name[compak_current_table[0][2].t_one-1],
                compak_trap_name[compak_current_table[0][2].t_two-1],
                compak_trap_name[compak_current_table[1][2].t_one-1],
                compak_trap_name[compak_current_table[1][2].t_two-1],
				compak_trap_name[compak_current_table[2][2].t_one-1],
                compak_trap_name[compak_current_table[2][2].t_two-1],
				compak_trap_name[compak_current_table[3][2].t_one-1],
                compak_trap_name[compak_current_table[3][2].t_two-1],
				compak_trap_name[compak_current_table[4][2].t_one-1],
                compak_trap_name[compak_current_table[4][1].t_two-1]);
            fwprintf(pFile,L" --------------------------------------------------- \n");
			}
 		}
#if 0
        for( UINT shooter = 0; shooter < MAX_SHOOTERS; ++shooter )
        {
			fwprintf(pFile,MLSTR_FILE_SHOOTER_PRESENT, shooter+1 , shooters_present[shooter] ? MLSTR_FILE_YES : MLSTR_FILE_NO );
        }
        fwprintf(pFile,L"==================================\n");
        fwprintf(pFile,MLSTR_FILE_TBL_TITLE);
        for( UINT target = 0, sequence = 0; target < round_targets; ++target )
        {
            if( shooters_present[targets[target].shooter] )
            {  
                sequence++;
                fwprintf(pFile,L"  %03d  |   %d     |    %d    |  %s \n",
                             sequence, 
                             targets[target].shooter+1,
                             targets[target].station+1,
                             targets[target].trap);
            }
        } 
#endif

		if( (round_type != ROUND_TYPE_COMPRESET) && 
			(round_type != ROUND_TYPE_COMCONTROL) )
		{
			fwprintf(pFile,L"          ");
            for( UINT t = 0; t < max_shots_per_shooter[round_type]; ++t )
			{
                fwprintf(pFile,L"=====");
			}
		    fwprintf(pFile,L"\n          ");

			for( UINT t = 0; t < max_shots_per_shooter[round_type]; ++t )
			{
                fwprintf(pFile,L"| %2d ",t+1);
			}
			fwprintf(pFile,L"|\n          ");
			for( UINT t = 0; t < max_shots_per_shooter[round_type]; ++t )
			{
                fwprintf(pFile,L"=====");
			}
                        
			for( UINT a = 0; a < MAX_SHOOTERS; ++a )
			{
				if( !shooters_present[a] )
				{
					continue;
				}
				fwprintf(pFile,L"\nShooter #%d\n",statistics.stat_target[a][0].stat_shooter_num);
				//fwprintf(pFile,L"          ");
				//for( UINT t = 0; t < max_shots_per_shooter[round_type]; ++t )
				//{
				//	fwprintf(pFile,L"| %2d ",statistics.stat_target[a][t].stat_result);
				//}
				//fwprintf(pFile,L"|\n");
				fwprintf(pFile,L"Target:   ");
				for( UINT t = 0; t < max_shots_per_shooter[round_type]; ++t )
				{
					switch( round_type )
					{
					    case ROUND_TYPE_DTQ:
						case ROUND_TYPE_DTF:
						case ROUND_TYPE_DTM:
						case ROUND_TYPE_DOP:
						case ROUND_TYPE_DSHOW:
						case ROUND_TYPE_DSHOOTOFF:
                            fwprintf(pFile,L"|  %s ",
							stat_dirs_double[statistics.stat_target[a][t].stat_direction-1]);
							break;
						case ROUND_TYPE_TQ:
						case ROUND_TYPE_TF:
						case ROUND_TYPE_TM:
						case ROUND_TYPE_TEST:
						case ROUND_TYPE_TOP:
						case ROUND_TYPE_TSHOW:
						case ROUND_TYPE_TSHOOTOFF:
						default:
							fwprintf(pFile,L"|  %s ",
							stat_dirs_single[statistics.stat_target[a][t].stat_direction-1]);
							break;
					}
				}
				fwprintf(pFile,L"|\n");
				fwprintf(pFile,L"1st (ms): ");
				for( UINT t = 0; t < max_shots_per_shooter[round_type]; ++t )
				{
					if( statistics.stat_target[a][t].stat_first_shot_usec == 0 )
					{
                    fwprintf(pFile,L"| -- ");
					}
					else
					{
                    fwprintf(pFile,L"|%d.%02d",statistics.stat_target[a][t].stat_first_shot_usec/100,
						                      statistics.stat_target[a][t].stat_first_shot_usec%100);
					}
				}
				fwprintf(pFile,L"|\n");
				fwprintf(pFile,L"2nd (ms): ");
				for( UINT t = 0; t < max_shots_per_shooter[round_type]; ++t )
				{
					if( statistics.stat_target[a][t].stat_second_shot_usec == 0 )
					{
                    fwprintf(pFile,L"| -- ");
					}
					else
					{
                    fwprintf(pFile,L"|%d.%02d",statistics.stat_target[a][t].stat_second_shot_usec/100,
						                      statistics.stat_target[a][t].stat_second_shot_usec%100);
					}
				}
				fwprintf(pFile,L"|\n");
				fwprintf(pFile,L"Mount:    ");
				for( UINT t = 0; t < max_shots_per_shooter[round_type]; ++t )
				{
					if( statistics.stat_target[a][t].stat_preparation_sec == 0 )
					{
                    fwprintf(pFile,L"| -- ");
					}
					else
					{
                    fwprintf(pFile,L"|%2d.%d",statistics.stat_target[a][t].stat_preparation_sec/10,
						                      statistics.stat_target[a][t].stat_preparation_sec%10);
					}
				}
				fwprintf(pFile,L"|\n          ");
                for( UINT t = 0; t < max_shots_per_shooter[round_type]; ++t )
			    {
                    fwprintf(pFile,L"=====");
			    }
				fwprintf(pFile,L"\n");
			}
		}
        fwprintf(pFile,L"==================================\n");
        fwprintf(pFile,MLSTR_FILE_SEND_COMMENTS);
        fwprintf(pFile,L"==================================\n");

		fclose(pFile);

	}
}
void CRoundDlg::Save2(void)
{
    CString s,Pathname;
	CString Datetime;
    errno_t err;
    FILE *pFile;
	struct tm * start = localtime( & statistics.stat_start_time );

	Datetime.Format(L"%d-%s-%02d %02d-%02d-%02d", 
                      start->tm_year + 1900,
	    			  monthes[start->tm_mon],
					  start->tm_mday,
					  start->tm_hour,
					  start->tm_min,
					  start->tm_sec);
	Pathname.Format(L"./%s %s.txt",file_name[round_type],Datetime.GetString());

	if( (err=_tfopen_s(&pFile,Pathname.GetString(),L"wt,ccs=UNICODE"))!=0 )
    {
        s.Format(MLSTR_CANNOT_OPEN_FILE,Pathname,err);
        AfxMessageBox(s);
        return;
    }

	for( UINT shooter = 0; shooter < round_shooters; ++shooter )
    {
        for( UINT station = 0; station < round_stations; ++station )
        {
			fwprintf(pFile,L" %2d", randoms[shooter][station]);
		}
        fwprintf(pFile,L"\n");
	}
    fwprintf(pFile,L"========================\n");

	fclose(pFile);

	return;
}

void CRoundDlg::Load2(void)
{
    CString s,Pathname;
    errno_t err;
    FILE *pFile;
	UINT8 buf[32];

    if( in_pause )
	{
		return;
	}

	CString Filters(MLSTR_TEXT_FILES);

    CFileDialog Dlg(TRUE,                  //open/save file dialog
                    L".txt",               //default extension
                    file_name[round_type], //default file name
                    OFN_HIDEREADONLY,
                    Filters);

    if( Dlg.DoModal() == IDOK )
    {
        Pathname=Dlg.GetPathName();

		if( (err=_tfopen_s(&pFile,Pathname.GetString(),L"r,ccs=UNICODE"))!=0 )
        {
            s.Format(MLSTR_CANNOT_OPEN_FILE,Pathname,err);
            AfxMessageBox(s);
            return;
        }
	}
	else
	{
		return;
	}

	fseek(pFile,0,SEEK_SET);

	for( UINT shooter = 0; shooter < round_shooters; ++shooter )
    {
    	buf[0] = 0xff;

	    for( ; buf[0]!= 0; )
	    {
		    fread(buf,1,1,pFile);
	    }

		for( UINT station = 0; station < round_stations; ++station )
        {
			fread(buf,1,6,pFile);
			if(buf[0] < 0x30)
			{
				buf[0] = 0x30;
			}
			randoms[shooter][station] = (buf[0]-0x30)*10+(buf[2]-0x30);
		}
	}

	fclose(pFile);

    InitRound(FALSE); 
    DrawGraphics(NULL);
    EmbSetConfig(FALSE); 

	return;
}

void CRoundDlg::ResetMikes(void)
{
	emb_reset_mikes = 1;
}

// App command to run the dialog
void CBunkerControlApp::OnDtq()
{
	CRoundDlg roundDlg(ROUND_TYPE_DTQ);
	roundDlg.DoModal();
}

void CBunkerControlApp::OnDtm()
{
	CRoundDlg roundDlg(ROUND_TYPE_DTM);
	roundDlg.DoModal();
}

void CBunkerControlApp::OnDshow()
{
	CRoundDlg roundDlg(ROUND_TYPE_DSHOW);
	roundDlg.DoModal();
}

void CBunkerControlApp::OnTshow()
{
	CRoundDlg roundDlg(ROUND_TYPE_TSHOW);
	roundDlg.DoModal();
}

void CBunkerControlApp::OnTshootoff()
{
	CRoundDlg roundDlg(ROUND_TYPE_TSHOOTOFF);
	roundDlg.DoModal();
}

void CBunkerControlApp::OnDshootoff()
{
	CRoundDlg roundDlg(ROUND_TYPE_DSHOOTOFF);
	roundDlg.DoModal();
}

void CBunkerControlApp::OnDtf()
{
	CRoundDlg roundDlg(ROUND_TYPE_DTF);
	roundDlg.DoModal();
}
void CBunkerControlApp::OnTq()
{
	CRoundDlg roundDlg(ROUND_TYPE_TQ);
	roundDlg.DoModal();
}
void CBunkerControlApp::OnTf()
{
	CRoundDlg roundDlg(ROUND_TYPE_TF);
	roundDlg.DoModal();
}
void CBunkerControlApp::OnTm()
{
	CRoundDlg roundDlg(ROUND_TYPE_TM);
	roundDlg.DoModal();
}

void CBunkerControlApp::OnTest()
{
	CRoundDlg roundDlg(ROUND_TYPE_TEST);
	roundDlg.DoModal();
}

void CBunkerControlApp::OnTop()
{
	CRoundDlg roundDlg(ROUND_TYPE_TOP);
	roundDlg.DoModal();
}

void CBunkerControlApp::OnDop()
{
	CRoundDlg roundDlg(ROUND_TYPE_DOP);
	roundDlg.DoModal();
}

void CBunkerControlApp::OnComPreset()
{
	CRoundDlg roundDlg((UINT)ROUND_TYPE_COMPRESET);
	roundDlg.DoModal();
}

void CBunkerControlApp::OnComControl()
{
	CRoundDlg roundDlg((UINT)ROUND_TYPE_COMCONTROL);
	roundDlg.DoModal();
}

void CBunkerControlApp::OnComTest()
{
	CRoundDlg roundDlg((UINT)ROUND_TYPE_COMTEST);
	roundDlg.DoModal();
}

void CRoundDlg::OnClickBrokenTrap()
{
	UINT current_select = m_BrokenTrap.GetCurSel();
    if( current_select != BrokenTrap )
	{
    	for (int i = m_ExchanTrap.GetCount()-1; i >= 0; i--)
        {
            m_ExchanTrap.DeleteString( i );
        }
		ExchanTrap = 0;

		if( BrokenTrap > 0 )
		{
            DrawTrap(NULL,BrokenTrap-1,WHITE);
		}

		if( current_select > 0 )
		{
            DrawTrap(NULL,current_select - 1, GREY);
			GetDlgItem(IDC_EXCHAN_TRAP)->EnableWindow(true);
			CString str1;
			CString str2;
			if( (current_select % 3) == 0 )
			{
				str1.Format(L"%2d",current_select-2);
				str2.Format(L"%2d",current_select-1);
			}
			else if( (current_select % 3) == 1 )
			{
				str1.Format(L"%2d",current_select+1);
				str2.Format(L"%2d",current_select+2);
			}
			else if( (current_select % 3) == 2 )
			{
				str1.Format(L"%2d",current_select-1);
				str2.Format(L"%2d",current_select+1);
			}

			m_ExchanTrap.AddString( str1 );
			m_ExchanTrap.AddString( str2 );
		}
		else
		{
			GetDlgItem(IDC_EXCHAN_TRAP)->EnableWindow(false);
			m_ExchanTrap.AddString(MLSTR_NONE);
		} 
        m_ExchanTrap.SetCurSel(0);
	}
    BrokenTrap = current_select;
	m_BrokenTrap.SetCurSel(current_select);
}	

void CRoundDlg::OnClickExchanTrap()
{
	UINT current_select = m_ExchanTrap.GetCurSel();
    ExchanTrap = current_select;
	m_ExchanTrap.SetCurSel(current_select);
}	

void CRoundDlg::OnClickSelectCompakTrap()
{
	UINT current_select = m_SelectCompakTrap.GetCurSel();
    select_compak_trap = current_select;
	m_SelectCompakTrap.SetCurSel(current_select);
    DrawActiveTrap(NULL,GREEN); 
}

void CRoundDlg::OnClickSelectCompakTrap1()
{
	UINT current_select = m_SelectCompakTrap1.GetCurSel();
    select_compak_trap1 = current_select;
	m_SelectCompakTrap1.SetCurSel(current_select);
	if( (select_compak_trap1 > 0) &&
		(select_compak_trap2 > 0) &&
		(select_compak_trap1 != select_compak_trap2) )
	{
	    GetDlgItem(IDC_SELECT_COMPAK_TRAP)->EnableWindow(true);
	}
	else
	{
	    GetDlgItem(IDC_SELECT_COMPAK_TRAP)->EnableWindow(false);
	}
    DrawActiveTrap(NULL,GREEN); 

}	

void CRoundDlg::OnClickSelectCompakTrap2()
{
	UINT current_select = m_SelectCompakTrap2.GetCurSel();
	select_compak_trap2 = current_select;
	m_SelectCompakTrap2.SetCurSel(current_select);
	if( (select_compak_trap1 > 0) &&
		(select_compak_trap2 > 0) &&
		(select_compak_trap1 != select_compak_trap2) )
	{
	    GetDlgItem(IDC_SELECT_COMPAK_TRAP)->EnableWindow(true);
	}
	else
	{
	    GetDlgItem(IDC_SELECT_COMPAK_TRAP)->EnableWindow(false);
	}
    DrawActiveTrap(NULL,GREEN); 

}	

void CRoundDlg::OnClickSelectTrap1()
{
	UINT current_select = m_SelectTrap1.GetCurSel();
    if( current_select != select_trap1 )
	{
        if( select_trap1 > 0 )
		{
			if( select_trap1 != select_trap2 )
			{
                DrawTrap(NULL,select_trap1-1,WHITE);
			}
		}
		if( current_select > 0 )
		{
            DrawTrap(NULL,current_select - 1, GREEN);
		}
	}
    select_trap1 = current_select;
	m_SelectTrap1.SetCurSel(current_select);
}	

void CRoundDlg::OnClickSelectTrap2()
{
	UINT current_select = m_SelectTrap2.GetCurSel();
    if( current_select != select_trap2 )
	{
        if( select_trap2 > 0 )
		{
			if( select_trap1 != select_trap2 )
			{
                DrawTrap(NULL,select_trap2-1,WHITE);
			}
		}
		if( current_select > 0 )
		{
            DrawTrap(NULL,current_select - 1, GREEN);
		}
	}
    select_trap2 = current_select;
	m_SelectTrap2.SetCurSel(current_select);

}

void CRoundDlg::OnClickSelectMike()
{
	UINT current_select = m_SelectMike.GetCurSel();
    
    if( current_select != select_mike )
	{
        if( select_mike > 0 )
		{
            DrawMike(NULL,select_mike - 1, WHITE);
		}
		if( current_select > 0 )
		{
            DrawMike(NULL,current_select - 1, GREEN);
		}

		UpdateData(FALSE);
	}
    select_mike = current_select;
	m_SelectMike.SetCurSel(current_select);

}
void CRoundDlg::OnClickSelectPause()
{
	SelectPause = m_SelectPause.GetCurSel();
	m_SelectPause.SetCurSel(SelectPause);
}	

void CRoundDlg::OnReleaseScroll(UINT mike)
{
	SCROLLINFO info;

	if( mike == 5 )
	{
	    m_ScrollReleaseLevel.GetScrollInfo(&info,SIF_TRACKPOS);
	    UserThreshold = MAX_VOLUME-info.nTrackPos;
	
		GlobalThreshold = 3+((UserThreshold*(MAX_VOLUME-BackgroundNoise))/MAX_VOLUME)+BackgroundNoise;
		//if( GlobalThreshold < BackgroundNoise+3 )
		//{
        //    GlobalThreshold = BackgroundNoise+3;
		//}
 
        m_ScrollReleaseLevel.SetScrollPos(MAX_VOLUME-UserThreshold);
	    ReleaseLevel.Format(L" %4d",GlobalThreshold);
	}
	else if( mike == 6 )
	{
	    m_ScrollNoiseLevel.GetScrollInfo(&info,SIF_TRACKPOS);
	    AirThreshold = MAX_VOLUME-info.nTrackPos;
	
		NoiseThreshold = 3+((AirThreshold*(MAX_VOLUME-BackgroundNoise))/MAX_VOLUME)+BackgroundNoise;
		//if( NoiseThreshold < BackgroundNoise+5 )
		//{
        //    NoiseThreshold = BackgroundNoise+5;
		//}
 
        m_ScrollNoiseLevel.SetScrollPos(MAX_VOLUME-AirThreshold);
	    NoiseLevel.Format(L" %4d",NoiseThreshold);
	}
	UpdateData(FALSE);
}	

void CRoundDlg::OnClickSelectRelay()
{
	SelectRelay = m_SelectRelay.GetCurSel();
	m_SelectRelay.SetCurSel(SelectRelay);
}	

void CRoundDlg::OnClickCheckNoise()
{
	INT status = m_NoiseCheck.GetCheck();
    if( status == BST_UNCHECKED )
    {
        noise_check = FALSE;
    }
    else if( status == BST_CHECKED )
    {
        noise_check = TRUE;
    }
}

void CRoundDlg::OnClickCheckVoice()
{
	INT status = m_VoiceCheck.GetCheck();
    if( status == BST_UNCHECKED )
    {
        voice_check = FALSE;
    }
    else if( status == BST_CHECKED )
    {
        voice_check = TRUE;
    }
}

void CRoundDlg::OnClickCheckNoBird()
{
	INT status = m_NobirdCheck.GetCheck();
    if( status == BST_UNCHECKED )
    {
        nobird_check = FALSE;
    }
    else if( status == BST_CHECKED )
    {
        nobird_check = TRUE;
    }
}	

void CRoundDlg::OnClickCheckSound()
{
	INT status = m_SoundCheck.GetCheck();
    if( status == BST_UNCHECKED )
    {
        sound_check = FALSE;
    }
    else if( status == BST_CHECKED )
    {
        sound_check = TRUE;
    }
}	

void CRoundDlg::OnClickCheckTimer()
{
	INT status = m_TimerCheck.GetCheck();
    if( status == BST_UNCHECKED )
    {
        timer_check = FALSE;
		LastReleaseTime = 0;
		DrawTimerLed(NULL,GREY);
    }
    else if( status == BST_CHECKED )
    {
        timer_check = TRUE;
		LastReleaseTime = time(0);
		DrawTimerLed(NULL,GREY);
    }
}	

void CRoundDlg::OnMoveScroll(UINT mike)
{
	SCROLLINFO info;

	if( mike == 5 )
	{
	    m_ScrollReleaseLevel.GetScrollInfo(&info,SIF_TRACKPOS);
 
	    UINT current_select = MAX_VOLUME-info.nTrackPos;
		current_select = ((current_select*(MAX_VOLUME-BackgroundNoise))/MAX_VOLUME)+BackgroundNoise;
	    ReleaseLevel.Format(L" %4d",current_select);
	}
	else if( mike == 6 )
	{
		m_ScrollNoiseLevel.GetScrollInfo(&info,SIF_TRACKPOS);

		UINT current_select = MAX_VOLUME-info.nTrackPos;
	    NoiseLevel.Format(L" %4d",current_select);
	}
	UpdateData(FALSE);
}

void CRoundDlg::OnTimer(UINT Event)
{
    time_t time_now = 0;
    KillTimer(m_PauseTimer);

	if( in_pause )
    {
		if( nobird_check )
		{
			if( wait_for_shot1 > 0 )
			{
                RELEASE_EN emb_return = EmbSetConfig(FALSE);
                if( emb_return == RELEASE_NO_BIRD )
                {
			        EmbTargetNoBird();
					return;
				}
                
                wait_for_shot1--;
				if( wait_for_shot1 == 0 )
				{
					if( 1000*(SelectPause+2) > 2500 )
					{
					    m_PauseTimer = SetTimer(1, 1000*(SelectPause+2)-2500, 0);
					}
					else
					{
						m_PauseTimer = SetTimer(1, VZBOX_COMM_TIMER, 0);
					}

				}
				else
				{
					m_PauseTimer = SetTimer(1, VZBOX_COMM_TIMER, 0);
				}
				return;
			}
		}

		in_pause = FALSE;
		if( !FirstSignalReleased )
		{
		    Next();
		}
        GetDlgItem(IDC_ROUND_PULL)->EnableWindow(true);
		if( (round_type != ROUND_TYPE_TEST) && (round_type != ROUND_TYPE_COMTEST) )
		{
		    GetDlgItem(IDC_ROUND_START)->EnableWindow(true);
		    GetDlgItem(IDC_ROUND_PLUS)->EnableWindow(true);
		    GetDlgItem(IDC_ROUND_MINUS)->EnableWindow(true);
		}
		m_PauseTimer = SetTimer(1, VZBOX_COMM_TIMER, 0);
    }
    else
    {
        RELEASE_EN emb_return = EmbSetConfig(FALSE);
        if( emb_return == RELEASE_TARGET )
        {
            // target released !!!
            EmbTargetRelease();
            return;
        }
		else if( emb_return == RELEASE_NO_BIRD )
        {
			LastReleaseTime = time(0);
			if( !nobird_check )
			{
                // something went wrong
				return;
			}
            // target released, but no shot registered !!!
            EmbTargetNoBird();
            return;
        }
		else if( timer_check )
		{
			if( LastReleaseTime != 0 )
			{
				time_now = time(0);

				if( (time_now - LastReleaseTime) > 14 )
				{
                    DrawTimerLed(NULL, RED);
					if( sound_check )
	                {
		                Beep(500,200);
	                }
				}
			}
		}
        
        UpdateData(FALSE);

        m_PauseTimer = SetTimer(1, VZBOX_COMM_TIMER, 0);
	}
    
}


RELEASE_EN CRoundDlg::EmbSetConfig(BOOL release)
{
	static PCVZ_MSG_UN prev_msg_u = {0}; 
    static UINT connection_lost = 0;

    if( Connected )
	{
		int res = 0;
		PCVZ_MSG_UN   msg_u;

		msg_u.msg_st.signature = EMB_TX_SIGNATURE;
		msg_u.msg_st.sequence_num = (unsigned char)tx_counter; //tx sequence number
		if( release )
		{
			msg_u.msg_st.operation = EMB_RELEASE;
		}
		else
		{
			msg_u.msg_st.operation =  EMB_CONFIG; // config or 0xAA release
		}

		msg_u.msg_st.trap_one = emb_trap1;
		msg_u.msg_st.trap_two = emb_trap2;
		if( (Paused) || (in_pause) )
		{
            msg_u.msg_st.mike = 0;
		}
		else
		{
		    msg_u.msg_st.mike = emb_mike;
		}
		msg_u.msg_st.reset_info = emb_reset_mikes;
		if( emb_reset_mikes > 0 )
		{
			emb_reset_mikes = 0;
		}
		msg_u.msg_st.global_threshold = GlobalThreshold;
		msg_u.msg_st.to_be_used0 = Un0;    
		msg_u.msg_st.relay_duration = (SelectRelay);
		if( noise_check == TRUE )
		{
			msg_u.msg_st.num_of_mikes = 0; // all mikes
		}
		else
		{
             msg_u.msg_st.num_of_mikes = 1;
		} 
		msg_u.msg_st.min_delay = MinDelay; // min delay
		msg_u.msg_st.max_delay = MaxDelay; // min delay
		msg_u.msg_st.to_be_used1 = Un1;
		msg_u.msg_st.to_be_used2 = Un2;
		msg_u.msg_st.to_be_used3 = Un3;
		msg_u.msg_st.to_be_used4 = Un4;
		msg_u.msg_st.to_be_used5 = Un5;
		msg_u.msg_st.to_be_used6 = Un6;
		msg_u.msg_st.to_be_used7 = Un7;
#if NOISE_THRESHOLD
		msg_u.msg_st.noise_threshold = NoiseThreshold;
#else
		msg_u.msg_st.noise_threshold = GlobalThreshold;
#endif
		msg_u.msg_st.check_nobird = nobird_check;
		msg_u.msg_st.voice_recognition = voice_check;
		msg_u.msg_st.on_report = FirstSignalReleased;

		tx_counter++;
		msg_u.msg_st.sequence_num = (unsigned char)tx_counter; //tx sequence number
        memcpy(&prev_msg_u.bytes[0], &msg_u.bytes[0], VZBOX_TX_MSG_SIZE);
		res = rawhid_send(0,msg_u.bytes,VZBOX_TX_MSG_SIZE,200);
		if( res != VZBOX_TX_MSG_SIZE )
		{
			++tx_err_local;
			DrawStatusLed(NULL, YELLOW, 0);
			connection_lost++;
			if( connection_lost > 10 )
			{
				Connected = FALSE;
				DrawStatusLed(NULL,RED, 0);
                connection_lost = 0;
		    	for( UINT led = 1; led < 8; ++led )
		        {
    		        DrawStatusLed(NULL, GREY, led);
		        }
			}
		}
		else
		{
            connection_lost = 0;
			DrawStatusLed(NULL,GREEN, 0);
		}

        return EmbGetStatus();
	}
    return RELEASE_IDLE;
}


RELEASE_EN CRoundDlg::EmbGetStatus()
{
	static unsigned char seq = 0;
	STAT_ONE_TARGET_ST* p_one_target_stat;

    if( Connected )
	{
		int res = 0;
		VZPC_MSG_UN msg_un;

        memset(&msg_un,0,sizeof(msg_un));

        seq++;
    	res = rawhid_recv(0,msg_un.bytes,VZBOX_RX_MSG_SIZE,200);
		if( res != VZBOX_RX_MSG_SIZE )
		{
			DrawStatusLed(NULL, YELLOW,0);
			++rx_err_local;
		}
		else if( msg_un.stat_msg_st.signature == EMB_RELEASE )
        { 
            return RELEASE_TARGET;
        }
		else if ( msg_un.stat_msg_st.signature == EMB_NO_BIRD )
		{
            return RELEASE_NO_BIRD;
		}
		else if( msg_un.stat_msg_st.signature == EMB_DATA )
		{
			EmbGetData(&msg_un);
			return RELEASE_IDLE;
		}
        else if( msg_un.stat_msg_st.signature != EMB_RX_SIGNATURE ) 
		{
			++rx_err_local;
			DrawStatusLed(NULL, YELLOW, 0);
		}
        else 
        {
            rx_counter++;
			if( msg_un.stat_msg_st.sequence_num != seq )
            {
				DrawStatusLed(NULL, YELLOW,0);
                ++rx_err_local;
                seq = msg_un.stat_msg_st.sequence_num;
            }
			emb_ver_major =  msg_un.stat_msg_st.ver_major;
			emb_ver_minor =  msg_un.stat_msg_st.ver_minor;
			major_embedded = emb_ver_major;
            minor_embedded = emb_ver_minor;

			EmbVersion.Format(L"%d.%02d",emb_ver_major,emb_ver_minor);
            EmbHardware.Format(L"%02d",emb_ver_major);

			release_delay_msec = msg_un.stat_msg_st.release_delay;
			sampling_time_msec = msg_un.stat_msg_st.sampling_time;

			if( msg_un.stat_msg_st.background > 0 )
			{
                if( msg_un.stat_msg_st.background > BackgroundNoise )
				{
					if( msg_un.stat_msg_st.background - BackgroundNoise > 3 )
					{
                        // why it is jumping? 
						DrawStatusLed(NULL, YELLOW,0);
					}
				    BackgroundNoise = msg_un.stat_msg_st.background;	
				}
				else
				{
					if( BackgroundNoise - msg_un.stat_msg_st.background > 3 )
					{
                        // why it is jumping?
						DrawStatusLed(NULL, YELLOW,0);
						BackgroundNoise--;
					}
				}
			}

			sn_1 = msg_un.stat_msg_st.sn_1;
			sn_2 = msg_un.stat_msg_st.sn_2;
			sn_3 = msg_un.stat_msg_st.sn_3;
			sn_4 = msg_un.stat_msg_st.sn_4;

			SerialNumber = (sn_1 << 24) | (sn_2 << 16) | (sn_3 << 8) | (sn_4 << 0);

			CurrentHardware.Format(MLSTR_CURRENT_HARDWARE,EmbHardware,EmbVersion,SerialNumber, 
                               tx_counter,
							   rx_counter);
#if TUNING_ENABLED
            EmbDbg0.Format(L"Bkground=%d",msg_un.stat_msg_st.background);
			EmbDbg1.Format(L"Sampling=%d",msg_un.stat_msg_st.sampling_time);
			EmbDbg2.Format(L"Rel.delay=%d",msg_un.stat_msg_st.release_delay);
			EmbDbg3.Format(L"1st shot=%d.%d",
				       (msg_un.stat_msg_st.first_shot*10)/1000,
					   (msg_un.stat_msg_st.first_shot*10)%1000);
			EmbDbg4.Format(L"2nd shot=%d.%d",
				       (msg_un.stat_msg_st.second_shot*10)/1000,
					   (msg_un.stat_msg_st.second_shot*10)%1000);
			EmbDbg5.Format(L"X-counter=%d",msg_un.stat_msg_st.dbg5);
			EmbDbg6.Format(L"Gap =%d.%d",
				msg_un.stat_msg_st.between_releases/10,
				msg_un.stat_msg_st.between_releases%10);
			EmbDbg7.Format(L"Cl. gun=%d",msg_un.stat_msg_st.dbg4*10);

			Mike1.Format(L"%3d",msg_un.stat_msg_st.mike_1);
            Mike2.Format(L"%3d",msg_un.stat_msg_st.mike_2);
            Mike3.Format(L"%3d",msg_un.stat_msg_st.mike_3);
            Mike4.Format(L"%3d",msg_un.stat_msg_st.mike_4);
            Mike5.Format(L"%3d",msg_un.stat_msg_st.mike_5);

#endif
            p_one_target_stat = &statistics.stat_target[targets[current_target].shooter]
			                     [targets[current_target].target/round_shooters];
            
			p_one_target_stat->stat_first_shot_usec  = msg_un.stat_msg_st.first_shot;
            p_one_target_stat->stat_second_shot_usec = msg_un.stat_msg_st.second_shot;
			p_one_target_stat->stat_preparation_sec  = msg_un.stat_msg_st.between_releases;
                        
			if( led1_cntr /*noise_counter*/ < msg_un.stat_msg_st.led1_cntr /*noise_counter*/ )
			{
                DrawStatusLed(NULL,YELLOW,1);      
			}
			else
			{
                DrawStatusLed(NULL,GREEN,1);
			}
            led1_cntr /*noise_counter*/ = msg_un.stat_msg_st.led1_cntr /*noise_counter*/;
			if( led1_cntr /*noise_counter*/ == 0 )
			{
                DrawStatusLed(NULL,GREY,1);
			}

			if( led2_cntr /*echo_counter*/ < msg_un.stat_msg_st.led2_cntr /*echo_counter*/ )
			{
                DrawStatusLed(NULL,YELLOW,2);      
			}
			else
			{
                DrawStatusLed(NULL,GREEN,2);
			}
            led2_cntr /*echo_counter*/ = msg_un.stat_msg_st.led2_cntr /*echo_counter*/;
			if( led2_cntr /*echo_counter*/ == 0 )
			{
                DrawStatusLed(NULL,GREY,2);
			}

			if( led3_cntr /*pick_counter*/ < msg_un.stat_msg_st.led3_cntr /*pick_counter*/ )
			{
                DrawStatusLed(NULL,YELLOW,3);      
			}
			else
			{
                DrawStatusLed(NULL,GREEN,3);
			}
            led3_cntr /*pick_counter*/ = msg_un.stat_msg_st.led3_cntr /*pick_counter*/;
			if( led3_cntr /*pick_counter*/ == 0 )
			{
                DrawStatusLed(NULL,GREY,3);
			}

			if( led4_cntr /*loud_counter*/ < msg_un.stat_msg_st.led4_cntr /*loud_counter*/ )
			{
                DrawStatusLed(NULL,YELLOW,4);      
			}
			else
			{
                DrawStatusLed(NULL,GREEN,4);
			}
            led4_cntr /*loud_counter*/ = msg_un.stat_msg_st.led4_cntr /*loud_counter*/;
			if( led4_cntr /*loud_counter*/ == 0 )
			{
                DrawStatusLed(NULL,GREY,4);
			}

			if( led5_cntr /*curve_counter*/ < msg_un.stat_msg_st.led5_cntr /*curve_counter*/ )
			{
                DrawStatusLed(NULL,YELLOW,5);      
			}
			else
			{
                DrawStatusLed(NULL,GREEN,5);
			}
            led5_cntr /*curve_counter*/ = msg_un.stat_msg_st.led5_cntr /*curve_counter*/;
			if( led5_cntr /*curve_counter*/ == 0 )
			{
                DrawStatusLed(NULL,GREY,5);
			}
#if 0
			if( led6_cntr /*repeated_value_counter*/ < msg_un.stat_msg_st.led6_cntr /*repeated_value_counter*/ )
			{
                DrawStatusLed(NULL,YELLOW,6);      
			}
			else
			{
                DrawStatusLed(NULL,GREEN,6);
			}
            led6_cntr /*repeated_value_counter*/ = msg_un.stat_msg_st.led6_cntr /*repeated_value_counter*/;
			if( led6_cntr /*repeated_value_counter*/ == 0 )
			{
                DrawStatusLed(NULL,GREY,6);
			}
#endif
			if( msg_un.stat_msg_st.data_msgs > 0 )
		    {
                DrawStatusLed(NULL,YELLOW,6);      
		    }
			else
			{
                DrawStatusLed(NULL,GREY,6);
			}

            if( Connected )
			{
				if( msg_un.stat_msg_st.background > 0 )
				{
                    DrawStatusLed(NULL,GREEN,7);
				}
				else
				{
                    DrawStatusLed(NULL,YELLOW,7);
				}
			}
			else
			{
			    DrawStatusLed(NULL,GREY,7);
			}

			UpdateData(FALSE);

			if( GlobalThreshold < (BackgroundNoise + 3) )
			{
                GlobalThreshold = BackgroundNoise + 3;
			}
			if( NoiseThreshold < (BackgroundNoise + 3) )
			{
                NoiseThreshold = BackgroundNoise + 3;
			}
 
			UINT new_position = ((GlobalThreshold - BackgroundNoise)*MAX_VOLUME)/(MAX_VOLUME-BackgroundNoise);
            if( new_position != UserThreshold )
			{
			    UserThreshold = new_position;
                m_ScrollReleaseLevel.SetScrollPos(MAX_VOLUME-UserThreshold);
			}
#if NOISE_THRESHOLD
			new_position = ((NoiseThreshold - BackgroundNoise)*MAX_VOLUME)/(MAX_VOLUME-BackgroundNoise);
            if( new_position != AirThreshold )
			{
			    AirThreshold = new_position;
                m_ScrollNoiseLevel.SetScrollPos(MAX_VOLUME-AirThreshold);
			}
#endif

        }

	}
    return RELEASE_IDLE;
}

void CRoundDlg::EmbGetData(VZPC_MSG_UN* p_msg_un)
{
	UINT data_seq = 0;

    if( !Connected )
	{
	    return;
	}
	
    if( p_msg_un->data_msg_st.signature != EMB_DATA )
    { 
	    ++rx_err_local;
		DrawStatusLed(NULL, RED, 0);
		return;
	}
    else 
    {
        rx_counter++;
		if( p_msg_un->data_msg_st.data_st.data_type == DATA_TYPE_UNDEFINED )
		{
			return;
		}
		else if( p_msg_un->data_msg_st.data_st.data_type == DATA_TYPE_ACTIVE_SAMPLES )
		{
            data_seq = p_msg_un->data_msg_st.data_st.data_seq;
			for( UINT i = 0; i < 58; ++i )
			{
				if( (58*data_seq + i) < 512 )
				{
                    active_samples[58*data_seq + i] = 
                                  p_msg_un->data_msg_st.data_st.data_bytes[i];
				}
			}
		}
		else if( p_msg_un->data_msg_st.data_st.data_type == DATA_TYPE_NOISE_SAMPLES )
		{
			data_seq = p_msg_un->data_msg_st.data_st.data_seq;
			for( UINT i = 0; i < 58; ++i )
			{
                if( (58*data_seq + i) < 512 )
				{
				    noise_samples[58*data_seq + i] = 
                                  p_msg_un->data_msg_st.data_st.data_bytes[i];
				}
			}
		}
		else
		{
            ++rx_err_local;
		    DrawStatusLed(NULL, RED, 0);
		}
	}
	return;
}

LRESULT CRoundDlg::OnUsbDeviceChange(WPARAM wParam, LPARAM lParam)
{
	static UINT count = 0;
	++count;
    if( !Connected )
    {
        // try to connect
       	Connected = FindAndOpenRawhid();
		if( Connected )
		{
			DrawStatusLed(NULL,GREEN,0);
		}
        EmbSetConfig(FALSE);
    }
#if 0
    // for more information, see MSDN help of WM_DEVICECHANGE
    // this part should not be very difficult to understand
    if ( DBT_DEVICEARRIVAL == wParam || DBT_DEVICEREMOVECOMPLETE == wParam ) {
        PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
        switch( pHdr->dbch_devicetype ) {
            case DBT_DEVTYP_DEVICEINTERFACE:
                PDEV_BROADCAST_DEVICEINTERFACE pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)pHdr;
                // do something...
                break;

            case DBT_DEVTYP_HANDLE:
                PDEV_BROADCAST_HANDLE pDevHnd = (PDEV_BROADCAST_HANDLE)pHdr;
                // do something...
                break;

            case DBT_DEVTYP_OEM:
                PDEV_BROADCAST_OEM pDevOem = (PDEV_BROADCAST_OEM)pHdr;
                // do something...
                break;

            case DBT_DEVTYP_PORT:
                PDEV_BROADCAST_PORT pDevPort = (PDEV_BROADCAST_PORT)pHdr;
                // do something...
                break;

            case DBT_DEVTYP_VOLUME:
                PDEV_BROADCAST_VOLUME pDevVolume = (PDEV_BROADCAST_VOLUME)pHdr;
                // do something...
                break;
        }
    }
#endif
    return 0;
}

static int rawhid_recv(int num, void *buf, int len, int timeout)
{
	hid_t *hid;
    unsigned char rcv_buf[512];
	OVERLAPPED ov;
	DWORD n, r;

	if (sizeof(rcv_buf) < len + 1) return -1;
	hid = get_hid(num);
	if (!hid || !hid->open) return -1;
	EnterCriticalSection(&rx_mutex);
	ResetEvent(&rx_event);
	memset(&ov, 0, sizeof(ov));
	ov.hEvent = rx_event;
	if (!ReadFile(hid->handle, rcv_buf, len + 1, NULL, &ov)) {
		if (GetLastError() != ERROR_IO_PENDING) goto return_error;
		r = WaitForSingleObject(rx_event, timeout);
		if (r == WAIT_TIMEOUT) goto return_timeout;
		if (r != WAIT_OBJECT_0) goto return_error;
	}
	if (!GetOverlappedResult(hid->handle, &ov, &n, FALSE)) goto return_error;
	LeaveCriticalSection(&rx_mutex);
	if (n <= 0) 
	{
		rx_err_local++;
		return -1;
	}
	n--;
	if (n > len) n = len;
	memcpy(buf, rcv_buf + 1, n);
	return n;
return_timeout:
    rx_err_local++;
	CancelIo(hid->handle);
	LeaveCriticalSection(&rx_mutex);
	return 0;
return_error:
    rx_err_local++;
	//print_win32_err();
	LeaveCriticalSection(&rx_mutex);
	return -1;
}

//  rawhid_send - send a packet
//    Inputs:
//	num = device to transmit to (zero based)
//	buf = buffer containing packet to send
//	len = number of bytes to transmit
//	timeout = time to wait, in milliseconds
//    Output:
//	number of bytes sent, or -1 on error
//

static int rawhid_send(int num, void *buf, int len, int timeout)
{
	hid_t *hid;
    unsigned char tmpbuf[32];
	OVERLAPPED ov;
	DWORD n, r;
	DWORD failure;

	if (sizeof(tmpbuf) < len + 1) return -1;
    memset(tmpbuf,0,sizeof(tmpbuf));
	hid = get_hid(num);
	if (!hid || !hid->open) return -1;
	EnterCriticalSection(&tx_mutex);
	ResetEvent(&tx_event);
	memset(&ov, 0, sizeof(ov));
	ov.hEvent = tx_event;
	tmpbuf[0] = 0;
	memcpy(tmpbuf + 1, buf, len);
    
	if (!WriteFile(hid->handle, tmpbuf, len + 1, NULL, &ov))
	{
		failure = GetLastError();
		if (failure != ERROR_IO_PENDING) goto return_error;
		r = WaitForSingleObject(tx_event, timeout);
		if (r == WAIT_TIMEOUT) goto return_timeout;
		if (r != WAIT_OBJECT_0) goto return_error;
	}
	if (!GetOverlappedResult(hid->handle, &ov, &n, FALSE)) goto return_error;
	LeaveCriticalSection(&tx_mutex);
	if (n <= 0)
	{
        tx_err_local++;
		return -1;
	}
	return n - 1;
return_timeout:
    tx_err_local++;
	CancelIo(hid->handle);
	LeaveCriticalSection(&tx_mutex);
	return 0;
return_error:
    tx_err_local++;
	//print_win32_err();
	LeaveCriticalSection(&tx_mutex);
	return -1;
}

//  rawhid_open - open 1 or more devices
//
//    Inputs:
//	max = maximum number of devices to open
//	vid = Vendor ID, or -1 if any
//	pid = Product ID, or -1 if any
//	usage_page = top level usage page, or -1 if any
//	usage = top level usage number, or -1 if any
//    Output:
//	actual number of devices opened
//
static int rawhid_open(int max, int vid, int pid, int usage_page, int usage)
{
    GUID guid;
    HDEVINFO info;
    DWORD index=0, reqd_size;
    SP_DEVICE_INTERFACE_DATA iface;
    SP_DEVICE_INTERFACE_DETAIL_DATA *details;
    HIDD_ATTRIBUTES attrib;
    PHIDP_PREPARSED_DATA hid_data;
    HIDP_CAPS capabilities;
    HANDLE h;
    BOOL ret;
	hid_t *hid;
	int count=0;

	if (first_hid) free_all_hid();
	if (max < 1) return 0;
	if (!rx_event) {
		rx_event = CreateEvent(NULL, TRUE, TRUE, NULL);
		tx_event = CreateEvent(NULL, TRUE, TRUE, NULL);
		InitializeCriticalSection(&rx_mutex);
		InitializeCriticalSection(&tx_mutex);
	}
	HidD_GetHidGuid(&guid);
	info = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	if (info == INVALID_HANDLE_VALUE) return 0;
	for (index=0; 1 ;index++) {
		iface.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		ret = SetupDiEnumDeviceInterfaces(info, NULL, &guid, index, &iface);
		if (!ret) return count;
		SetupDiGetInterfaceDeviceDetail(info, &iface, NULL, 0, &reqd_size, NULL);
		details = (SP_DEVICE_INTERFACE_DETAIL_DATA *)malloc(reqd_size);
		if (details == NULL) continue;

		memset(details, 0, reqd_size);
		details->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		ret = SetupDiGetDeviceInterfaceDetail(info, &iface, details,
			reqd_size, NULL, NULL);
		if (!ret) {
			free(details);
			continue;
		}
		h = CreateFile(details->DevicePath, GENERIC_READ|GENERIC_WRITE,
			FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
		free(details);
		if (h == INVALID_HANDLE_VALUE) continue;
		attrib.Size = sizeof(HIDD_ATTRIBUTES);
		ret = HidD_GetAttributes(h, &attrib);
		//printf("vid: %4x\n", attrib.VendorID);
		if (!ret || (vid > 0 && attrib.VendorID != vid) ||
		  (pid > 0 && attrib.ProductID != pid) ||
		  !HidD_GetPreparsedData(h, &hid_data)) {
			CloseHandle(h);
			continue;
		}
		if (!HidP_GetCaps(hid_data, &capabilities) ||
		  (usage_page > 0 && capabilities.UsagePage != usage_page) ||
		  (usage > 0 && capabilities.Usage != usage)) {
			HidD_FreePreparsedData(hid_data);
			CloseHandle(h);
			continue;
		}
		HidD_FreePreparsedData(hid_data);
		hid = (struct hid_struct *)malloc(sizeof(struct hid_struct));
		if (!hid) {
			CloseHandle(h);
			continue;
		}
		hid->handle = h;
		hid->open = 1;
		add_hid(hid);
		count++;
		if (count >= max) return count;
	}
	return count;
}


//  rawhid_close - close a device
//
//    Inputs:
//	num = device to close (zero based)
//    Output
//	(nothing)
//
static void rawhid_close(int num)
{
	//hid_t *hid;
    free_all_hid();
	//hid = get_hid(num);
	//if (!hid || !hid->open) return;
	//hid_close(hid);
}



static void add_hid(hid_t *h)
{
	if (!first_hid || !last_hid) {
		first_hid = last_hid = h;
		h->next = h->prev = NULL;
		return;
	}
	last_hid->next = h;
	h->prev = last_hid;
	h->next = NULL;
	last_hid = h;
}


static hid_t * get_hid(int num)
{
	hid_t *p;
	for (p = first_hid; p && num > 0; p = p->next, num--) ;
	return p;
}


static void free_all_hid(void)
{
	hid_t *p, *q;

	for (p = first_hid; p; p = p->next) {
		hid_close(p);
	}
	p = first_hid;
	while (p) {
		q = p;
		p = p->next;
		free(q);
	}
	first_hid = last_hid = NULL;
}


static void hid_close(hid_t *hid)
{
	CloseHandle(hid->handle);
	hid->handle = NULL;
	//free_all_hid();
	//free(hid);
}

#if 0
void print_win32_err(void)
{
	char buf[256];
	DWORD err;

	err = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err,
		0, buf, sizeof(buf), NULL);
	printf("err %ld: %s\n", err, buf);
}
#endif

BOOL CRoundDlg::FindAndOpenRawhid()
{
	int r;

	// C-based example is 16C0:0480:FFAB:0200
	// r = rawhid_open(1, 0x16C0, 0x0480, 0xFFAB, 0x0200);
    // vzBox
	r = rawhid_open(1, 0xBA91, 0xC301, 0xFFAB, 0x0200);
	if (r <= 0)
	{
		// Arduino-based example is 16C0:0486:FFAB:0200
		//r = rawhid_open(1, 0x16C0, 0x0486, 0xFFAB, 0x0200);
		//if (r <= 0)
		//{
			//printf("no rawhid device found\n");
			return FALSE;
		//}
	}
    else if( r > 1 )
	{
        // too many devices   
	}

    Sleep(100);

	//printf("found rawhid device\n");
	return TRUE;
}




void CRoundDlg::OnEnChangeUn() { UpdateData(TRUE); }

void CRoundDlg::OnKillFocusUn0() { UpdateData(TRUE); Un0 = _wtoi(EmbUn0); }
void CRoundDlg::OnKillFocusUn1() { UpdateData(TRUE); Un1 = _wtoi(EmbUn1); }
void CRoundDlg::OnKillFocusUn2() { UpdateData(TRUE); Un2 = _wtoi(EmbUn2); }
void CRoundDlg::OnKillFocusUn3() { UpdateData(TRUE); Un3 = _wtoi(EmbUn3); }
void CRoundDlg::OnKillFocusUn4() { UpdateData(TRUE); Un4 = _wtoi(EmbUn4); }
void CRoundDlg::OnKillFocusUn5() { UpdateData(TRUE); Un5 = _wtoi(EmbUn5); }
void CRoundDlg::OnKillFocusUn6() { UpdateData(TRUE); Un6 = _wtoi(EmbUn6); }
void CRoundDlg::OnKillFocusUn7() { UpdateData(TRUE); Un7 = _wtoi(EmbUn7); }

/* from ihex.c, at http://www.pjrc.com/tech/8051/pm2_docs/intel-hex.html */

/* parses a line of intel hex code, stores the data in bytes[] */
/* and the beginning address in addr, and returns a 1 if the */
/* line was valid, or a 0 if an error occured.  The variable */
/* num gets the number of bytes that were stored into bytes[] */


int
parse_hex_line(char *line)
{
        int addr, code, num;
        int sum, len, cksum, i;
        char *ptr;
        
        num = 0;
        if (line[0] != ':') return 0;
        if (strlen(line) < 11) return 0;
        ptr = line+1;
        if (!sscanf_s(ptr, "%02x", &len)) return 0;
        ptr += 2;
        if ((int)strlen(line) < (11 + (len * 2)) ) return 0;
        if (!sscanf_s(ptr, "%04x", &addr)) return 0;
        ptr += 4;
          /* printf("Line: length=%d Addr=%d\n", len, addr); */
        if (!sscanf_s(ptr, "%02x", &code)) return 0;
        if (addr + extended_addr + len >= MAX_MEMORY_SIZE) return 0;
        ptr += 2;
        sum = (len & 255) + ((addr >> 8) & 255) + (addr & 255) + (code & 255);
        if (code != 0) {
                if (code == 1) {
                        end_record_seen = 1;
                        return 1;
                }
                if (code == 2 && len == 2) {
                        if (!sscanf_s(ptr, "%04x", &i)) return 1;
                        ptr += 4;
                        sum += ((i >> 8) & 255) + (i & 255);
                        if (!sscanf_s(ptr, "%02x", &cksum)) return 1;
                        if (((sum & 255) + (cksum & 255)) & 255) return 1;
                        extended_addr = i << 4;
                        //printf("ext addr = %05X\n", extended_addr);
                }
                if (code == 4 && len == 2) {
                        if (!sscanf_s(ptr, "%04x", &i)) return 1;
                        ptr += 4;
                        sum += ((i >> 8) & 255) + (i & 255);
                        if (!sscanf_s(ptr, "%02x", &cksum)) return 1;
                        if (((sum & 255) + (cksum & 255)) & 255) return 1;
                        extended_addr = i << 16;
                        //printf("ext addr = %08X\n", extended_addr);
                }
                return 1;       // non-data line
        }
        byte_count += len;
        while (num != len) {
                if (sscanf_s(ptr, "%02x", &i) != 1) return 0;
                i &= 255;
                firmware_image[addr + extended_addr + num] = i;
                firmware_mask[addr + extended_addr + num] = 1;
                ptr += 2;
                sum += i;
                (num)++;
                if (num >= 256) return 0;
        }
        if (!sscanf_s(ptr, "%02x", &cksum)) return 0;
        if (((sum & 255) + (cksum & 255)) & 255) return 0; /* checksum error */
        return 1;
}

void ihex_get_data(int addr, int len, unsigned char *bytes)
{
        int i;

        if (addr < 0 || len < 0 || addr + len >= MAX_MEMORY_SIZE) {
                for (i=0; i<len; i++) {
                        bytes[i] = 255;
                }
                return;
        }
        for (i=0; i<len; i++)
		{
			if( addr < (INT)image_size )
			{
                bytes[i] = image_data[addr];
            }
        	else
			{
                bytes[i] = 255;
            }
            addr++;
        }
}

HANDLE open_usb_device(int vid, int pid)
{
        GUID guid;
        HDEVINFO info;
        DWORD index, required_size;
        SP_DEVICE_INTERFACE_DATA iface;
        SP_DEVICE_INTERFACE_DETAIL_DATA *details;
        HIDD_ATTRIBUTES attrib;
        HANDLE h;
        BOOL ret;

        HidD_GetHidGuid(&guid);
        info = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
        if (info == INVALID_HANDLE_VALUE) return NULL;
        for (index=0; 1 ;index++) {
                iface.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
                ret = SetupDiEnumDeviceInterfaces(info, NULL, &guid, index, &iface);
                if (!ret) {
                        SetupDiDestroyDeviceInfoList(info);
                        break;
                }
                SetupDiGetInterfaceDeviceDetail(info, &iface, NULL, 0, &required_size, NULL);
                details = (SP_DEVICE_INTERFACE_DETAIL_DATA *)malloc(required_size);
                if (details == NULL) continue;
                memset(details, 0, required_size);
                details->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
                ret = SetupDiGetDeviceInterfaceDetail(info, &iface, details,
                        required_size, NULL, NULL);
                if (!ret) {
                        free(details);
                        continue;
                }
                h = CreateFile(details->DevicePath, GENERIC_READ|GENERIC_WRITE,
                        FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
                        FILE_FLAG_OVERLAPPED, NULL);
                free(details);
                if (h == INVALID_HANDLE_VALUE) continue;
                attrib.Size = sizeof(HIDD_ATTRIBUTES);
                ret = HidD_GetAttributes(h, &attrib);
                if (!ret) {
                        CloseHandle(h);
                        continue;
                }
                if (attrib.VendorID != vid || attrib.ProductID != pid) {
                        CloseHandle(h);
                        continue;
                }
                SetupDiDestroyDeviceInfoList(info);
                return h;
        }
        return NULL;
}

int write_usb_device(HANDLE h, void *buf, int len, int timeout)
{
        static HANDLE event = NULL;
        unsigned char tmpbuf[1040];
        OVERLAPPED ov;
        DWORD n, r;

        if (len > sizeof(tmpbuf) - 1) return 0;
        if (event == NULL) {
                event = CreateEvent(NULL, TRUE, TRUE, NULL);
                if (!event) return 0;
        }
        ResetEvent(&event);
        memset(&ov, 0, sizeof(ov));
        ov.hEvent = event;
        tmpbuf[0] = 0;
        memcpy(tmpbuf + 1, buf, len);
        if (!WriteFile(h, tmpbuf, len + 1, NULL, &ov)) {
                if (GetLastError() != ERROR_IO_PENDING) return 0;
                r = WaitForSingleObject(event, timeout);
                if (r == WAIT_TIMEOUT) {
                        CancelIo(h);
                        return 0;
                }
                if (r != WAIT_OBJECT_0) return 0;
        }
        if (!GetOverlappedResult(h, &ov, &n, FALSE)) return 0;
        if (n <= 0) return 0;
        return 1;
}




int teensy_open(void)
{
        teensy_close();
        win32_teensy_handle = open_usb_device(0x16C0, 0x0478);
        if (win32_teensy_handle) return 1;
        return 0;
}

int teensy_write(void *buf, int len, double timeout)
{
        int r;
        if (!win32_teensy_handle) return 0;
        r = write_usb_device(win32_teensy_handle, buf, len, (int)(timeout * 1000.0));
        //if (!r) print_win32_err();
        return r;
}

void teensy_close(void)
{
        if (!win32_teensy_handle) return;
        CloseHandle(win32_teensy_handle);
        win32_teensy_handle = NULL;
}


