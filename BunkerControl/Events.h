//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by BunkerControl.rc
//
typedef enum
{
   EVENT_NO,        /* 000 */
   EVENT_RELAY_OFF, /* 001 */
   EVENT_INTER_1,   /* 002 */
   EVENT_INTER_2,   /* 003 */
   EVENT_INTER_3,   /* 004 */
   EVENT_INTER_4,   /* 005 */
   EVENT_INTER_5,   /* 006 */
   EVENT_NOTARGET,  /* 007 */
   EVENT_RELAY_CL,  /* 008 */
   EVENT_ACTIVE_0,  /* 009 */
   EVENT_ACTIVE_1,  /* 010 */
   EVENT_ACTIVE_2,  /* 011 */
   EVENT_ACTIVE_3,  /* 012 */
   EVENT_ACTIVE_4,  /* 013 */
   EVENT_ACTIVE_5,  /* 014 */
   EVENT_RELAY_01,  /* 015 */
   EVENT_RELAY_02,  /* 016 */
   EVENT_RELAY_03,  /* 017 */
   EVENT_RELAY_04,  /* 018 */
   EVENT_RELAY_05,  /* 019 */
   EVENT_RELAY_06,  /* 020 */
   EVENT_RELAY_07,  /* 021 */
   EVENT_RELAY_08,  /* 022 */
   EVENT_RELAY_09,  /* 023 */
   EVENT_RELAY_10,  /* 024 */
   EVENT_RELAY_11,  /* 025 */
   EVENT_RELAY_12,  /* 026 */
   EVENT_RELAY_13,  /* 027 */
   EVENT_RELAY_14,  /* 028 */
   EVENT_RELAY_15,  /* 029 */
   EVENT_BKGRND,    /* 030 */
   EVENT_SHOT,      /* 031 */
   EVENT_NOISE,     /* 032 */
   EVENT_MIKE_0,    /* 033 */
   EVENT_MIKE_1,    /* 034 */
   EVENT_MIKE_2,    /* 035 */
   EVENT_MIKE_3,    /* 036 */
   EVENT_MIKE_4,    /* 037 */
   EVENT_MIKE_5,    /* 038 */
   EVENT_RCV_CFG,   /* 039 */
   EVENT_RCV_RLZ,   /* 040 */
   EVENT_TX_STATUS, /* 041 */
   EVENT_TX_RLZ,    /* 042 */
   EVENT_TX_BROKEN, /* 043 */

   EVENT_LAST = 255 /* 255 */
}EVENTS_EN;

typedef struct
{
	UINT      aux;
	CString   str;
}EVENT_ST;
