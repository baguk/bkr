// BunkerControl.h : main header file for the BunkerControl application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "language.h"       // language slection support
#include "MainFrm.h"

#define MAX_SETTING_FILE_SIZE (4096)
#define MAX_TRAP_NAME_LEN     (80)

#define MAX_SHOOTERS          (6)
#define MAX_STATIONS          (5)
#define MAX_SHOTS_PER_SHOOTER (25)
#define MAX_TARGETS           ((MAX_SHOOTERS*MAX_SHOTS_PER_SHOOTER) * 2)
#define MAX_MACHINES          (15)
#define MAX_COMPAK_TRAPS      (8)  

#define DTQ_SHOOTERS          (6)
#define DTQ_STATIONS          (5)
#define DTQ_OPTIONS           (6)
#define DTQ_SHOTS_PER_SHOOTER (15)
#define DTQ_MACHINES          (3)
#define DTQ_TARGETS           (DTQ_SHOOTERS*DTQ_SHOTS_PER_SHOOTER)

#define DTM_SHOOTERS          (2)
#define DTM_STATIONS          (3)
#define DTM_OPTIONS           (30)
#define DTM_SHOTS_PER_SHOOTER (15)
#define DTM_MACHINES          (3)
#define DTM_TARGETS           (DTM_SHOOTERS*DTM_SHOTS_PER_SHOOTER)

#define TQ_SHOOTERS          (6)
#define TQ_STATIONS          (5)
#define TQ_OPTIONS           (30)
#define TQ_SHOTS_PER_SHOOTER (25)
#define TQ_MACHINES          (15)
#define TQ_TARGETS           (TQ_SHOOTERS*TQ_SHOTS_PER_SHOOTER)

#define TS_OPTIONS           (6)

#define TM_SHOOTERS          (2)
#define TM_STATIONS          (3)
#define TM_OPTIONS           (30)
#define TM_SHOTS_PER_SHOOTER (15)
#define TM_MACHINES          (9)
#define TM_TARGETS           (TM_SHOOTERS*TM_SHOTS_PER_SHOOTER)

#define MAX_VOLUME           (212)
#define DEF_LOOPS             4
#define DEF_SELECT_RELAY      2
#define DEF_AUDIO_SAMPLES    32
#define DEF_CHECK_NOISE       0
#define DEF_CHECK_NOBIRD      0
#define DEF_SKIP_SAMPLES      3
#define DEF_SELECT_PAUSE      2
#define DEF_GLOBAL_THRESHOLD 135
#define DEF_NOISE_THRESHOLD  130

#define VZBOX_TX_MSG_SIZE    (24)
#define VZBOX_RX_MSG_SIZE    (64)
#define VZBOX_COMM_TIMER      (100)

#define FIRST_SIGNAL_TIMER    (50)
// CBunkerControlApp:
// See BunkerControl.cpp for the implementation of this class
//
#define BKG_GREY (0xbebebe)
 	
#define GREY     (BKG_GREY)
#define WHITE    (0xffffff)
#define MY_BLACK (0)
#define RED      (0x0000ff)
#define GREEN    (0x00ff00)
#define TEAL     (0xffff00)
#define YELLOW   (0x00ffff)
#define BLACK    (0x000001)


typedef enum
{
    ROUND_TYPE_NONE,
    ROUND_TYPE_DTQ,    // DT Qual
    ROUND_TYPE_DTF,    // DT Quat/Semi
    ROUND_TYPE_DTM,    // DT Br/Gold Match
    ROUND_TYPE_TQ,     // T Qual
    ROUND_TYPE_TF,     // T Quat/Semi
    ROUND_TYPE_TM,     // T Br/Gold Match
	ROUND_TYPE_TEST,   // Practice
	ROUND_TYPE_TOP,    // T one station
	ROUND_TYPE_DOP,    // D one station
	ROUND_TYPE_TSHOW,
	ROUND_TYPE_DSHOW,
	ROUND_TYPE_TSHOOTOFF,
	ROUND_TYPE_DSHOOTOFF,
	ROUND_TYPE_COMPRESET,
	ROUND_TYPE_COMCONTROL,
	ROUND_TYPE_COMTEST,
    ROUND_TYPE_MAX     
} ROUND_TYPE_EN;

typedef enum
{
	COMPAK_T_NONE,
    COMPAK_SINGLE,
    COMPAK_D_SIGN,   
    COMPAK_D_SIMU,
	COMPAK_E_SHOW
} COMPAK_TARGET_TYPE_EN;

typedef struct
{
    COMPAK_TARGET_TYPE_EN    t_type;
	UINT				     t_one;
	UINT                     t_two;
} COMPAK_TARGET_ST;

typedef struct
{
    UINT target;
    UINT shooter;
    UINT station;
    UINT machine;
    wchar_t trap[MAX_TRAP_NAME_LEN];
	COMPAK_TARGET_ST compak_trap;
} TARGET_ST;

#define EMB_TX_SIGNATURE     0xBA
#define EMB_RX_SIGNATURE     0xD1
#define EMB_CONFIG           0x55
#define EMB_RELEASE          0xAA
#define EMB_NO_BIRD          0xE7
#define EMB_JUMP_BOOTLOADER  0x22
#define EMB_DATA             0x33

typedef enum
{
	RELEASE_IDLE,
	RELEASE_TARGET,
	RELEASE_NO_BIRD
}RELEASE_EN;


typedef struct
{
  /* 0 */ unsigned char signature;
  /* 1 */ unsigned char sequence_num;
  /* 2 */ unsigned char operation;
  /* 3 */ unsigned char trap_one;
  /* 4 */ unsigned char trap_two;
  /* 5 */ unsigned char mike;
  /* 6 */ unsigned char reset_info;
  /* 7 */ unsigned char global_threshold;
  /* 8 */ unsigned char to_be_used0;
  /* 9 */ unsigned char relay_duration;
  /*10 */ unsigned char num_of_mikes;
  /*11 */ unsigned char min_delay;
  /*12 */ unsigned char max_delay;
  /*13 */ unsigned char to_be_used1;
  /*14 */ unsigned char to_be_used2;
  /*15 */ unsigned char to_be_used3;
  /*16 */ unsigned char to_be_used4;
  /*17 */ unsigned char to_be_used5;
  /*18 */ unsigned char to_be_used6;
  /*19 */ unsigned char to_be_used7;
  /*20 */ unsigned char noise_threshold;
  /*21 */ unsigned char check_nobird;
  /*22 */ unsigned char voice_recognition;
  /*23 */ unsigned char on_report;
} PCVZ_MSG_ST;

typedef union
{
    PCVZ_MSG_ST  msg_st;
	unsigned char bytes[VZBOX_TX_MSG_SIZE];
} PCVZ_MSG_UN;

typedef struct
{
  /* 0 */ unsigned char signature;
  /* 1 */ unsigned char sequence_num;
  /* 2 */ unsigned char ver_major;
  /* 3 */ unsigned char ver_minor;
  /* 4 */ unsigned char dbg1;
  /* 5 */ unsigned char dbg2;
  /* 6 */ unsigned char second_shot;
  /* 7 */ unsigned char dbg4;
  /* 8 */ unsigned char dbg5;
  /* 9 */ unsigned char dbg6;
  /*10 */ unsigned char data_msgs;
  /*11 */ unsigned char led5_cntr /*curve_counter*/;
  /*12 */ unsigned char led4_cntr /*loud_counter*/;
  /*13 */ unsigned char led3_cntr /*pick_counter*/;
  /*14 */ unsigned char release_delay;
  /*15 */ unsigned char sampling_time;
  /*16 */ unsigned char first_shot;
  /*17 */ unsigned char background;
  /*18 */ unsigned char led1_cntr /*noise_counter*/;
  /*19 */ unsigned char led2_cntr /*echo_counter*/;
  /*20 */ unsigned char sn_1;
  /*21 */ unsigned char sn_2;
  /*22 */ unsigned char sn_3;
  /*23 */ unsigned char sn_4;
  /*24 */ unsigned char mike_1;
  /*25 */ unsigned char mike_2;
  /*26 */ unsigned char mike_3;
  /*27 */ unsigned char mike_4;
  /*28 */ unsigned char mike_5;
  /*29 */ unsigned char between_releases;

} VZPC_STAT_MSG_ST;

#define DATA_TYPE_UNDEFINED          0
#define DATA_TYPE_ACTIVE_SAMPLES     1
#define DATA_TYPE_NOISE_SAMPLES      2
#define DATA_TYPE_EVENTS             3
#define DATA_TYPE_CONTINUE         255   

typedef struct 
{
	unsigned char data_type;        /* 1 or 2 */
	unsigned char data_length_high; 
	unsigned char data_length_low;
	unsigned char data_seq;         /* msg# */
	unsigned char data_bytes[58];
} VZPC_DATA_ST;

typedef struct
{
  /* 0 */ unsigned char signature;
  /* 1 */ unsigned char sequence_num;
  /* 2 */ VZPC_DATA_ST  data_st;
} VZPC_DATA_MSG_ST;

typedef union
{
    VZPC_STAT_MSG_ST     stat_msg_st;
    VZPC_DATA_MSG_ST     data_msg_st;
	unsigned char bytes[VZBOX_RX_MSG_SIZE];
} VZPC_MSG_UN;

typedef enum
{
	STAT_RESULT_NO = 0,
	STAT_RESULT_HIT_1ST,
	STAT_RESULT_HIT_2ND,
	STAT_RESULT_MISS_1,
	STAT_RESULT_DBL_HIT,
	STAT_RESULT_DBL_1ST,
	STAT_RESULT_DBL_2ND
} STAT_RESULT_EN;

typedef struct
{
	UINT           stat_shooter_num;
	UINT           stat_target_num;
	UCHAR          stat_direction;
	wchar_t        stat_dir[MAX_TRAP_NAME_LEN];
	STAT_RESULT_EN stat_result;
	UINT           stat_first_shot_usec;
	UINT           stat_second_shot_usec;
	UINT           stat_preparation_sec;
} STAT_ONE_TARGET_ST;

typedef struct
{
	time_t             stat_start_time;
	ROUND_TYPE_EN      stat_round_type;
	STAT_ONE_TARGET_ST stat_target[MAX_SHOOTERS][MAX_TARGETS];
} STATISTICS_ST;

class CBunkerControlApp : public CWinApp
{
public:
	CBunkerControlApp();    

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	CMainFrame *wnd;
	// Server object for document creation
	afx_msg void OnAppAbout();
	afx_msg void OnAppRunBootloader();
	afx_msg void OnAppConvertFirmware();
	afx_msg void OnAppProgramFirmware();
	afx_msg void OnAppLoadFirmware();
	afx_msg void OnAppResetBox();
	afx_msg void OnAppDrawShooters();
	
	// Bunker Control
    afx_msg void OnDtq();
    afx_msg void OnDtf();
    afx_msg void OnDtm();
    afx_msg void OnDshow();
    afx_msg void OnTshow();
	afx_msg void OnDshootoff();
    afx_msg void OnTshootoff();
	afx_msg void OnTq();
    afx_msg void OnTf();
    afx_msg void OnTm();
    afx_msg void OnTest();
    afx_msg void OnTop();
    afx_msg void OnDop();
	afx_msg void OnComPreset();
	afx_msg void OnComControl();
	afx_msg void OnComTest();

	DECLARE_MESSAGE_MAP()
};

extern CBunkerControlApp theApp;

class CRoundDlg;

class COnRunModeDlg : public CDialog
{
public:
	COnRunModeDlg();
	COnRunModeDlg(CRoundDlg * pParent);
    CString m_CurrentStation;
	CString m_CurrentShooter;
	CString m_CurrentTarget;
    CRoundDlg* m_parent;
    CFont m_font;
    CFont m_font2;

	BOOL Create(CWnd * pParent = NULL);
	void PostNcDestroy();
	void OnDestroy();
	void OnCancel();
	void OnOK();
// Dialog Data
	enum { IDD = IDD_ON_RUN_MODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnClickMinus();
	afx_msg void OnClickPlus();
	afx_msg BOOL OnInitDialog( );
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor);
};


class CRoundDlg : public CDialog
{

public:
	CRoundDlg();
	CRoundDlg(ROUND_TYPE_EN round_type);
	CRoundDlg(UINT compak_round_type);

// Dialog Data
    CBrush* m_pEditBkBrush;

	enum { IDD = IDD_ROUND_BOX, IDQ = IDD_COMPAK_BOX };
    CButton m_Shooters[MAX_SHOOTERS];
    BOOL shooters_present[MAX_SHOOTERS*2];
    TARGET_ST targets[MAX_TARGETS];
	STATISTICS_ST statistics;
	CButton m_NoiseCheck;
	CButton m_VoiceCheck;
	CButton m_NobirdCheck;
	CButton m_SoundCheck;
	CButton m_TimerCheck;

	BOOL nobird_check;
	BOOL noise_check;
	BOOL voice_check;
	BOOL sound_check;
	BOOL timer_check;
    UINT current_target;
    ROUND_TYPE_EN round_type;
    UINT round_targets;
    UINT round_shooters;
    UINT round_stations;
	UINT round_shots_per_shooter;
	UINT compak_stand_targets;
	CStringW CurrentStatus;
	CStringW CurrentHardware;
	CStringW ReleaseLevel;
	CStringW NoiseLevel;

	CString EmbUn0;
	CString EmbUn1;
    CString EmbUn2;
    CString EmbUn3;
    CString EmbUn4;
    CString EmbUn5;
	CString EmbUn6;
	CString EmbUn7;

	CString EmbDbg0;
	CString EmbDbg1;
    CString EmbDbg2;
    CString EmbDbg3;
    CString EmbDbg4;
    CString EmbDbg5;
	CString EmbDbg6;
	CString EmbDbg7;

	CString Mike1;
	CString Mike2;
	CString Mike3;
	CString Mike4;
	CString Mike5;

	CComboBox m_SelectMike;
	CComboBox m_SelectTrap1;
	CComboBox m_SelectTrap2;
	CComboBox m_SelectCompakTrap;
	CComboBox m_SelectCompakTrap1;
	CComboBox m_SelectCompakTrap2;
	CComboBox m_SelectPause;
	CComboBox m_BrokenTrap;
	CComboBox m_ExchanTrap;
	CComboBox m_SelectRelay;

	CComboBox m_ShooterStyle[6];

	CScrollBar m_ScrollReleaseLevel;
	CScrollBar m_ScrollNoiseLevel;
    CScrollBar m_ScrollThreshold[5];
    CScrollBar m_MikeVolume[5];

	COnRunModeDlg* onRunModeDlg;
    BOOL onruncreated;

	UINT select_mike;
    UINT select_trap1;
    UINT select_trap2;
    UINT select_compak_trap;
    UINT select_compak_trap1;
    UINT select_compak_trap2;


	BOOL in_pause;
    UINT_PTR m_PauseTimer;
	
	UINT shooter_style[6];

    UINT rx[4];

    void Next(void);
    void Prev(void);
	void PostNcDestroy();
    unsigned char emb_mike;
	unsigned char last_target_shot;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:

    void InitRound(BOOL random = TRUE);
    void Save(void);
	void Save2(void);
	void Load2(void);
    void TargetRelease(void);
	void SetCompakDistribution(void);
    void EmbTargetRelease(void);
	void EmbTargetNoBird(void);
	void ResetMikes(void);
	void OnClickShStyle(UINT sh);
    void OnEnChangeShooter(UINT sh);

	RELEASE_EN EmbSetConfig(BOOL release);
	RELEASE_EN EmbGetStatus();
	void EmbGetData(VZPC_MSG_UN* p_msg_un);

    BOOL CRoundDlg::FindAndOpenRawhid();

    unsigned char emb_trap1;
    unsigned char emb_trap2;
    unsigned char emb_ver_major;
    unsigned char emb_ver_minor;

	unsigned char emb_first_of_signal;
	unsigned char emb_reset_mikes;
    
	CString EmbVersion;
	CString EmbHardware;

	DECLARE_MESSAGE_MAP()
    afx_msg void OnEnChangeShooter1();
    afx_msg void OnEnChangeShooter2();
    afx_msg void OnEnChangeShooter3();
    afx_msg void OnEnChangeShooter4();
    afx_msg void OnEnChangeShooter5();
    afx_msg void OnEnChangeShooter6();
    afx_msg void OnClickStart();
    afx_msg void OnClickOnRun();
	afx_msg void OnClickPause();
	afx_msg void OnClickResume();
	afx_msg void OnClickAudioSamples();
	afx_msg void DrawGraphics(CDC* dc);
    afx_msg void DrawTrap(CDC* dc, UINT trap, COLORREF color);
    afx_msg void DrawStatusLed(CDC* dc, COLORREF color, UINT number);
	afx_msg void CRoundDlg::DrawTimerLed(CDC* dc, COLORREF color);
    afx_msg void DrawActiveTrap(CDC* dc, COLORREF color);
    afx_msg void DrawMike(CDC* dc, UINT mike, COLORREF color);
	afx_msg void DrawActiveMike(CDC* dc, COLORREF color);
	afx_msg void DrawStatus();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnClickSelectMike();
	afx_msg void OnClickSelectTrap1();
	afx_msg void OnClickSelectTrap2();
	afx_msg void OnClickSelectCompakTrap();
	afx_msg void OnClickSelectCompakTrap1();
	afx_msg void OnClickSelectCompakTrap2();
	afx_msg void OnClickBrokenTrap();
	afx_msg void OnClickExchanTrap();
	afx_msg void OnClickSelectPause();
	afx_msg void OnReleaseScroll(UINT mike);
	afx_msg void OnClickSelectRelay();
	afx_msg void OnClickCheckNoise();
	afx_msg void OnClickCheckVoice();
	afx_msg void OnClickCheckNoBird();
	afx_msg void OnClickCheckSound();
	afx_msg void OnClickCheckTimer();
	afx_msg void OnMoveScroll(UINT mike);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT Event);
    afx_msg UINT GetLocalVolume(UINT devId);
    afx_msg void OnClickSelectLocalSource();
	afx_msg void OnClickSelectFactor();
	afx_msg void OnClickSh1Style();
	afx_msg void OnClickSh2Style();
	afx_msg void OnClickSh3Style();
	afx_msg void OnClickSh4Style();
	afx_msg void OnClickSh5Style();
	afx_msg void OnClickSh6Style();
	afx_msg BOOL OnInitDialog( );
    afx_msg	LRESULT OnUsbDeviceChange(WPARAM wParam, LPARAM lParam);

    afx_msg void OnEnChangeUn();
 
    afx_msg void OnKillFocusUn0();
	afx_msg void OnKillFocusUn1();
    afx_msg void OnKillFocusUn2();
    afx_msg void OnKillFocusUn3();
    afx_msg void OnKillFocusUn4();
    afx_msg void OnKillFocusUn5();
    afx_msg void OnKillFocusUn6();
    afx_msg void OnKillFocusUn7();

};

const COMPAK_TARGET_ST compak_preset_tables[40][5/* stations*/][5/* targets*/] = 
{
	       /* table 1 */
	{
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 4, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 5, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 6, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 1, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0} } /* stand 5 */
	},

	        /* table 2 */
	{
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 5, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 6, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 1, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0} } /* stand 5 */
	},

			/* table 3 */
	{
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 6, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 1, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 4, 0} } /* stand 5 */
	},

			/* table 4 */
	{
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 1, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 4, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 5, 0} } /* stand 5 */
	},

			/* table 5 */
	{
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 4, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 5, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 6, 0} } /* stand 5 */
	},

			/* table 6 */
	{
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 4, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 5, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 6, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 1, 0} } /* stand 5 */
	},

			/* table 7 */
	{
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 5, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 3, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 6, 0} } /* stand 5 */
	},

			/* table 8 */
	{
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 2, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 5, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 3, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 6, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 1, 0} } /* stand 5 */
	},

				/* table 9 */
	{
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIGN, 3, 4}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIGN, 4, 5}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIGN, 5, 6}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIGN, 6, 1}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIGN, 1, 2}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

				/* table 10 */
	{
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIGN, 1, 6}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIGN, 6, 3}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIGN, 3, 5}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIGN, 5, 2}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIGN, 2, 4}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

						/* table 11 */
	{
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIGN, 4, 1}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIGN, 1, 6}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIGN, 6, 2}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIGN, 2, 3}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIGN, 3, 5}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

				/* table 12 */
	{
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIGN, 1, 3}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIGN, 3, 5}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIGN, 5, 1}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIGN, 1, 2}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIGN, 2, 6}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

						/* table 13 */
	{
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIGN, 6, 3}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIGN, 3, 1}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIGN, 1, 4}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIGN, 4, 5}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIGN, 5, 2}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

						/* table 14 */
	{
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIGN, 5, 4}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIGN, 4, 1}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIGN, 1, 3}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIGN, 3, 6}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIGN, 6, 2}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

						/* table 15 */
	{
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIGN, 5, 4}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIGN, 4, 1}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIGN, 1, 4}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIGN, 4, 3}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIGN, 3, 6}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

						/* table 16 */
	{
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIGN, 5, 3}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIGN, 3, 6}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIGN, 6, 4}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIGN, 4, 2}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIGN, 2, 1}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 17 */
	{
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIMU, 2, 3}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIMU, 3, 1}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIMU, 1, 4}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIMU, 4, 6}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIMU, 6, 5}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 18 */
	{
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIMU, 1, 2}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIMU, 2, 1}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIMU, 1, 6}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIMU, 6, 3}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIMU, 3, 5}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 19 */
	{
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIMU, 4, 2}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIMU, 2, 3}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIMU, 3, 6}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIMU, 6, 1}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIMU, 1, 5}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 20 */
	{
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIMU, 6, 3}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIMU, 3, 4}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIMU, 4, 5}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIMU, 5, 1}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIMU, 1, 2}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 21 */
	{
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIMU, 6, 5}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIMU, 5, 3}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIMU, 3, 4}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIMU, 4, 1}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIMU, 1, 3}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 22 */
	{
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIMU, 5, 1}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIMU, 1, 3}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIMU, 3, 5}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIMU, 5, 4}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIMU, 4, 6}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 23 */
	{
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIMU, 1, 4}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIMU, 4, 2}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIMU, 2, 5}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIMU, 5, 3}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIMU, 3, 6}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 24 */
	{
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIMU, 1, 3}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIMU, 3, 5}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_SINGLE, 4, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIMU, 5, 2}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 6, 0}, { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIMU, 2, 4}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_SINGLE, 3, 0}, { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIMU, 4, 6}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 25 */
	{
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIGN, 4, 2}, { COMPAK_D_SIGN, 6, 1}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIGN, 2, 6}, { COMPAK_D_SIGN, 1, 4}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIGN, 6, 3}, { COMPAK_D_SIGN, 4, 5}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIGN, 3, 1}, { COMPAK_D_SIGN, 5, 2}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIGN, 1, 6}, { COMPAK_D_SIGN, 2, 3}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 26 */
	{
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIGN, 5, 1}, { COMPAK_D_SIGN, 3, 2}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIGN, 1, 4}, { COMPAK_D_SIGN, 2, 5}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIGN, 4, 6}, { COMPAK_D_SIGN, 5, 1}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIGN, 6, 2}, { COMPAK_D_SIGN, 1, 4}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIGN, 2, 3}, { COMPAK_D_SIGN, 4, 6}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 27 */
	{
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIGN, 5, 6}, { COMPAK_D_SIGN, 2, 3}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIGN, 6, 4}, { COMPAK_D_SIGN, 3, 1}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIGN, 4, 5}, { COMPAK_D_SIGN, 1, 6}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIGN, 5, 1}, { COMPAK_D_SIGN, 6, 3}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIGN, 1, 2}, { COMPAK_D_SIGN, 3, 4}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 28 */
	{
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIGN, 3, 5}, { COMPAK_D_SIGN, 1, 6}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIGN, 5, 1}, { COMPAK_D_SIGN, 6, 2}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIGN, 1, 3}, { COMPAK_D_SIGN, 2, 5}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIGN, 3, 4}, { COMPAK_D_SIGN, 5, 6}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIGN, 4, 1}, { COMPAK_D_SIGN, 6, 2}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 29 */
	{
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIGN, 3, 4}, { COMPAK_D_SIGN, 1, 2}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIGN, 4, 6}, { COMPAK_D_SIGN, 2, 3}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIGN, 6, 1}, { COMPAK_D_SIGN, 3, 5}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIGN, 1, 4}, { COMPAK_D_SIGN, 5, 6}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIGN, 4, 1}, { COMPAK_D_SIGN, 6, 2}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 30 */
	{
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIGN, 4, 1}, { COMPAK_D_SIGN, 2, 5}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIGN, 1, 6}, { COMPAK_D_SIGN, 5, 2}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIGN, 6, 4}, { COMPAK_D_SIGN, 2, 1}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIGN, 4, 3}, { COMPAK_D_SIGN, 1, 5}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIGN, 3, 2}, { COMPAK_D_SIGN, 5, 6}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 31 */
	{
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIGN, 3, 4}, { COMPAK_D_SIGN, 5, 2}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIGN, 4, 5}, { COMPAK_D_SIGN, 2, 1}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIGN, 5, 3}, { COMPAK_D_SIGN, 1, 6}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIGN, 3, 1}, { COMPAK_D_SIGN, 6, 4}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIGN, 1, 5}, { COMPAK_D_SIGN, 4, 2}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 32 */
	{
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIGN, 2, 4}, { COMPAK_D_SIGN, 5, 3}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIGN, 4, 6}, { COMPAK_D_SIGN, 3, 2}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIGN, 6, 1}, { COMPAK_D_SIGN, 2, 5}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIGN, 1, 3}, { COMPAK_D_SIGN, 5, 6}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIGN, 3, 5}, { COMPAK_D_SIGN, 6, 4}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 33 */
	{
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIMU, 2, 6}, { COMPAK_D_SIMU, 3, 5}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIMU, 6, 1}, { COMPAK_D_SIMU, 5, 4}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIMU, 1, 2}, { COMPAK_D_SIMU, 4, 3}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIMU, 2, 5}, { COMPAK_D_SIMU, 3, 6}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIMU, 5, 3}, { COMPAK_D_SIMU, 6, 4}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 34 */
	{
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIMU, 1, 3}, { COMPAK_D_SIMU, 6, 2}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIMU, 3, 6}, { COMPAK_D_SIMU, 2, 5}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIMU, 6, 1}, { COMPAK_D_SIMU, 5, 4}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIMU, 1, 2}, { COMPAK_D_SIMU, 4, 3}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIMU, 2, 6}, { COMPAK_D_SIMU, 3, 5}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 35 */
	{
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIMU, 4, 3}, { COMPAK_D_SIMU, 6, 1}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIMU, 3, 5}, { COMPAK_D_SIMU, 1, 2}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIMU, 5, 4}, { COMPAK_D_SIMU, 2, 6}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIMU, 4, 1}, { COMPAK_D_SIMU, 6, 3}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIMU, 1, 6}, { COMPAK_D_SIMU, 3, 5}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 36 */
	{
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIMU, 6, 5}, { COMPAK_D_SIMU, 2, 4}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIMU, 5, 2}, { COMPAK_D_SIMU, 4, 3}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIMU, 2, 6}, { COMPAK_D_SIMU, 3, 1}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIMU, 6, 4}, { COMPAK_D_SIMU, 1, 5}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIMU, 4, 2}, { COMPAK_D_SIMU, 5, 3}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 37 */
	{
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIMU, 5, 2}, { COMPAK_D_SIMU, 4, 6}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIMU, 2, 3}, { COMPAK_D_SIMU, 6, 1}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIMU, 3, 5}, { COMPAK_D_SIMU, 1, 4}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIMU, 5, 6}, { COMPAK_D_SIMU, 4, 2}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIMU, 6, 4}, { COMPAK_D_SIMU, 2, 1}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 38 */
	{
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIMU, 3, 4}, { COMPAK_D_SIMU, 6, 1}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIMU, 4, 6}, { COMPAK_D_SIMU, 1, 2}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIMU, 6, 3}, { COMPAK_D_SIMU, 2, 5}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIMU, 3, 1}, { COMPAK_D_SIMU, 5, 4}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIMU, 1, 6}, { COMPAK_D_SIMU, 4, 2}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 39 */
	{
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIMU, 4, 5}, { COMPAK_D_SIMU, 2, 6}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIMU, 5, 1}, { COMPAK_D_SIMU, 6, 3}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIMU, 1, 4}, { COMPAK_D_SIMU, 3, 2}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 1, 0}, { COMPAK_D_SIMU, 4, 6}, { COMPAK_D_SIMU, 2, 5}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIMU, 6, 2}, { COMPAK_D_SIMU, 5, 3}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	},

			/* table 40 */
	{
		{ { COMPAK_SINGLE, 6, 0}, { COMPAK_D_SIMU, 3, 1}, { COMPAK_D_SIMU, 4, 2}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 1*/
		{ { COMPAK_SINGLE, 4, 0}, { COMPAK_D_SIMU, 1, 5}, { COMPAK_D_SIMU, 2, 6}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 2*/
		{ { COMPAK_SINGLE, 2, 0}, { COMPAK_D_SIMU, 5, 3}, { COMPAK_D_SIMU, 6, 4}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 3*/
		{ { COMPAK_SINGLE, 5, 0}, { COMPAK_D_SIMU, 3, 6}, { COMPAK_D_SIMU, 4, 1}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} }, /* stand 4 */
		{ { COMPAK_SINGLE, 3, 0}, { COMPAK_D_SIMU, 6, 4}, { COMPAK_D_SIMU, 1, 2}, { COMPAK_T_NONE, 0, 0}, { COMPAK_T_NONE, 0, 0} } /* stand 5 */
	}

};

typedef struct 
{
	UINT shooter;
	UINT station;
}COMPAK_SEQUENCE_ST;

const COMPAK_SEQUENCE_ST compak_sequence_tables[6/* shooters*/][5*6] =
{
	{ /* 1 shooter */
	 { 1, 1}, {1,2},{1,3},{1,4},{1,5}, 
	 { 0, 0}, {0,0},{0,0},{0,0},{0,0}, 
	 { 0, 0}, {0,0},{0,0},{0,0},{0,0},
	 { 0, 0}, {0,0},{0,0},{0,0},{0,0},
	 { 0, 0}, {0,0},{0,0},{0,0},{0,0},
	 { 0, 0}, {0,0},{0,0},{0,0},{0,0} 
	},

	{ /* 2 shooters */
	 { 1, 1}, {2,2},{1,2},{2,3},{1,3}, 
	 { 2, 4}, {1,4},{2,5},{1,5},{2,1}, 
	 { 0, 0}, {0,0},{0,0},{0,0},{0,0},
	 { 0, 0}, {0,0},{0,0},{0,0},{0,0},
	 { 0, 0}, {0,0},{0,0},{0,0},{0,0},
	 { 0, 0}, {0,0},{0,0},{0,0},{0,0} 
	},

	{ /* 3 shooters */
	 { 1, 1}, {2,2},{3,3},{1,2},{2,3}, 
	 { 3, 4}, {1,3},{2,4},{3,5},{1,4}, 
	 { 2, 5}, {1,5},{3,1},{2,1},{3,2},
	 { 0, 0}, {0,0},{0,0},{0,0},{0,0},
	 { 0, 0}, {0,0},{0,0},{0,0},{0,0},
	 { 0, 0}, {0,0},{0,0},{0,0},{0,0} 
	},

	{ /* 4 shooters */
	 { 1, 1}, {2,2},{3,3},{4,4},{1,2}, 
	 { 2, 3}, {3,4},{4,5},{1,3},{2,4}, 
	 { 3, 5}, {1,4},{2,5},{4,1},{1,5},
	 { 3, 1}, {4,2},{2,1},{3,2},{4,3},
	 { 0, 0}, {0,0},{0,0},{0,0},{0,0},
	 { 0, 0}, {0,0},{0,0},{0,0},{0,0} 
	},

	{ /* 5 shooters */
	 { 1, 1}, {2,2},{3,3},{4,4},{5,5}, 
	 { 1, 2}, {2,3},{3,4},{4,5},{1,3}, 
	 { 2, 4}, {3,5},{5,1},{1,4},{2,5},
	 { 4, 1}, {5,2},{1,5},{3,1},{4,2},
	 { 5, 3}, {2,1},{3,2},{4,3},{5,4},
	 { 0, 0}, {0,0},{0,0},{0,0},{0,0} 
	},

	{ /* 6 shooters */
	 { 1, 1}, {2,2},{3,3},{4,4},{5,5}, 
	 { 1, 2}, {2,3},{3,4},{4,5},{6,1}, 
	 { 1, 3}, {2,4},{3,5},{5,1},{6,2},
	 { 1, 4}, {2,5},{4,1},{5,2},{6,3},
	 { 1, 5}, {3,1},{4,2},{5,3},{6,4},
	 { 2, 1}, {3,2},{4,3},{5,4},{6,5} 
	}

};

const COMPAK_SEQUENCE_ST compak_sequence_tables2[6 /* shooters*/  ]
                                                [6 /* rounds */   ]
												[5 /* positions */] = 
{
	{ /* 1 shooter */
		{{ 1, 1}, {0,0},{0,0},{0,0},{0,0}}, 
		{{ 1, 2}, {0,0},{0,0},{0,0},{0,0}}, 
		{{ 1, 3}, {0,0},{0,0},{0,0},{0,0}},
		{{ 1, 4}, {0,0},{0,0},{0,0},{0,0}},
		{{ 1, 5}, {0,0},{0,0},{0,0},{0,0}},
		{{ 0, 0}, {0,0},{0,0},{0,0},{0,0}} 
	},

	{ /* 2 shooters */
		{{ 1, 1}, {2,2},{0,0},{0,0},{0,0}},
		{{ 1, 2}, {2,3},{0,0},{0,0},{0,0}},
		{{ 1, 3}, {2,4},{0,0},{0,0},{0,0}},
		{{ 1, 4}, {2,5},{0,0},{0,0},{0,0}},
		{{ 1, 5}, {0,0},{0,0},{0,0},{0,0}},
		{{ 2, 1}, {0,0},{0,0},{0,0},{0,0}} 
	},

	{ /* 3 shooters */
		{{ 1, 1}, {2,2},{3,3},{0,0},{0,0}}, 
		{{ 1, 2}, {2,3},{3,4},{0,0},{0,0}}, 
		{{ 1, 3}, {2,4},{3,5},{0,0},{0,0}},
		{{ 1, 4}, {2,5},{0,0},{0,0},{0,0}},
		{{ 1, 5}, {3,1},{0,0},{0,0},{0,0}},
		{{ 2, 1}, {3,2},{0,0},{0,0},{0,0}} 
	},

	{ /* 4 shooters */
		{{ 1, 1}, {2,2},{3,3},{4,4},{0,0}}, 
		{{ 1, 2}, {2,3},{3,4},{4,5},{0,0}}, 
		{{ 1, 3}, {2,4},{3,5},{0,0},{0,0}},
		{{ 1, 4}, {2,5},{4,1},{0,0},{0,0}},
		{{ 1, 5}, {3,1},{4,2},{0,0},{0,0}},
		{{ 2, 1}, {3,2},{4,3},{0,0},{0,0}} 
	},

	{ /* 5 shooters */
		{{ 1, 1}, {2,2},{3,3},{4,4},{5,5}}, 
		{{ 1, 2}, {2,3},{3,4},{4,5},{0,0}}, 
		{{ 1, 3}, {2,4},{3,5},{5,1},{0,0}},
		{{ 1, 4}, {2,5},{4,1},{5,2},{0,0}},
		{{ 1, 5}, {3,1},{4,2},{5,3},{0,0}},
		{{ 2, 1}, {3,2},{4,3},{5,4},{0,0}} 
	},

	{ /* 6 shooters */
		{{ 1, 1}, {2,2},{3,3},{4,4},{5,5}}, 
		{{ 1, 2}, {2,3},{3,4},{4,5},{6,1}}, 
		{{ 1, 3}, {2,4},{3,5},{5,1},{6,2}},
		{{ 1, 4}, {2,5},{4,1},{5,2},{6,3}},
		{{ 1, 5}, {3,1},{4,2},{5,3},{6,4}},
		{{ 2, 1}, {3,2},{4,3},{5,4},{6,5}} 
	}

};