// BunkerControl.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "BunkerControl.h"
#include "MainFrm.h"
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
#if 1
const EVENT_ST event_st[EVENT_LAST] = 
{ 
    { 0, L"End of events" },                             //EVENT_NO,        /* 000 */
	{ 0, L"Relay(s) Off"},                               //EVENT_RELAY_OFF, /* 001 */
	{ 1, L"Mike1 Interrupted by noise level %d"},        //EVENT_INTER_1,   /* 002 */
    { 1, L"Mike2 Interrupted by noise level %d"},        //EVENT_INTER_2,   /* 003 */
    { 1, L"Mike3 Interrupted by noise level %d"},        //EVENT_INTER_3,   /* 004 */
    { 1, L"Mike4 Interrupted by noise level %d"},        //EVENT_INTER_4,   /* 005 */
    { 1, L"Mike5 Interrupted by noise level %d"},        //EVENT_INTER_5,   /* 006 */
    { 0, L"No Target timer expired"},                    //EVENT_NOTARGET,  /* 007 */
    { 0, L"Reserved Event"}                             //EVENT_RELAY_CL,  /* 008 */
   //EVENT_ACTIVE_0,  /* 009 */
   //EVENT_ACTIVE_1,  /* 010 */
   //EVENT_ACTIVE_2,  /* 011 */
   //EVENT_ACTIVE_3,  /* 012 */
   //EVENT_ACTIVE_4,  /* 013 */
   //EVENT_ACTIVE_5,  /* 014 */
   //EVENT_RELAY_01,  /* 015 */
   //EVENT_RELAY_02,  /* 016 */
   //EVENT_RELAY_03,  /* 017 */
   //EVENT_RELAY_04,  /* 018 */
   //EVENT_RELAY_05,  /* 019 */
   //EVENT_RELAY_06,  /* 020 */
   //EVENT_RELAY_07,  /* 021 */
   //EVENT_RELAY_08,  /* 022 */
   //EVENT_RELAY_09,  /* 023 */
   //EVENT_RELAY_10,  /* 024 */
   //EVENT_RELAY_11,  /* 025 */
   //EVENT_RELAY_12,  /* 026 */
   //EVENT_RELAY_13,  /* 027 */
   //EVENT_RELAY_14,  /* 028 */
   //EVENT_RELAY_15,  /* 029 */
   //EVENT_BKGRND,    /* 030 */
   //EVENT_SHOT,      /* 031 */
   //EVENT_NOISE,     /* 032 */
   //EVENT_MIKE_0,    /* 033 */
   //EVENT_MIKE_1,    /* 034 */
   //EVENT_MIKE_2,    /* 035 */
   //EVENT_MIKE_3,    /* 036 */
   //EVENT_MIKE_4,    /* 037 */
   //EVENT_MIKE_5,    /* 038 */
   //EVENT_RCV_CFG,   /* 039 */
   //EVENT_RCV_RLZ,   /* 040 */
   //EVENT_TX_STATUS, /* 041 */
   //EVENT_TX_RLZ,    /* 042 */
   //EVENT_TX_BROKEN, /* 043 */

   //EVENT_LAST = 255 /* 255 */
};

#endif


