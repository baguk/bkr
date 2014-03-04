// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "BunkerControl.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
    ON_WM_PAINT()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	//    BunkerControlWindow()
    {
        Create(NULL,L"BunkerControl");
    }

	EnableActiveAccessibility();
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}


	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	CMenu m_NewMenu;
	m_NewMenu.LoadMenu(IDR_MAINFRAME);
	SetMenu(&m_NewMenu);

	m_bmp.LoadBitmap(FON_MAINFRAME);

	HICON hicon;
    hicon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MYICON));
    SetIcon(hicon, true );

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
    cs.style &= ~WS_THICKFRAME;

	return TRUE;
}

    void CMainFrame::OnPaint()
    {
        CPaintDC dc(this);
        CRect rect;
        GetClientRect(&rect);
        CDC dcBitmap;  
     
        dcBitmap.CreateCompatibleDC(&dc);
        CBitmap* poldBitmap = dcBitmap.SelectObject(&m_bmp);    
//        dc.BitBlt(rect.left, rect.top, 600, 400, &dcBitmap, rectMenu.left, rectMenu.top, SRCCOPY);
        dc.BitBlt(rect.left, rect.top+30, 600, 400, &dcBitmap, 0, 0, SRCCOPY);

		dcBitmap.SelectObject(poldBitmap);
    }


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers



