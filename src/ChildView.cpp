
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "vsmoria.h"
#include "ChildView.h"

#include <sstream>

#include "cursesJG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



SpriteDrawer::SpriteDrawer() {
  // char buf[256];
  // GetCurrentDirectory(sizeof buf, buf);
  int res = sprites.Load( _T("sprites\\sprites.png") );
  initOK = (res >= 0);
}


void SpriteDrawer::drawSprite(int myChar, CRect& dest, CDC& dc) {
  if (!initOK) { return;  } // Fail silently/gracefully.

  int cellWidth = dest.Width();
  int cellHeight = dest.Height();
  int imgWidth = sprites.GetWidth();
  int colCount = imgWidth / cellWidth;

  int row = myChar / colCount;
  int col = myChar % colCount;
  int offset_x = col * cellWidth;
  int offset_y = row * cellHeight;
  CRect srcR(CPoint(0, 0), CSize(cellWidth, cellHeight));
  srcR.OffsetRect(offset_x, offset_y);

  sprites.Draw(dc, dest, srcR);
}




class debstr : public std::stringstream {
public:
	~debstr() {
		std::string s = str();
		CA2T uc(s.c_str(), CP_ACP); //  CP_UTF8);
		OutputDebugString(uc);
	}
};




void keyEaterLoopJG() {
  debstr() << "keyEaterLoopJG begin\n";
  const int waitCount = 10;
  const int waitMS = 50;
  for (int i = 0; i < waitCount; ++i) {
    // Check to see if any messages are waiting in the queue
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // Translate the message and dispatch it to WindowProc()
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    // If the message is WM_QUIT, exit the while loop
    if (msg.message == WM_QUIT) {
      break;
    }
    debstr() << "keyEaterLoop, sleep#" << i << "\n";
    Sleep(waitMS);
  }
  debstr() << "keyEaterLoopJG end\n";
}




// WM_KEYDOWN: https://msdn.microsoft.com/en-us/library/windows/desktop/ms646280%28v=vs.85%29.aspx
// key codes: https://msdn.microsoft.com/en-us/library/windows/desktop/ms646280%28v=vs.85%29.aspx


UINT getNextKey() { 
	// GetMessage loop example. // http://www.cplusplus.com/forum/beginner/38860/	
	MSG msg;
  while (GetMessage(&msg, NULL, 0, 0) > 0) {
		// if (msg.message == WM_KEYDOWN) {
		// 	UINT keyCode = msg.wParam;
		// 	return keyCode;
		// }
    if (msg.message == WM_CHAR) {
      UINT charCode = msg.wParam;
      return charCode;
    }

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	} // If we get out of here (return value not == 0), it means we got WM_QUIT. (or < 0 on error.)

	return VK_CANCEL; // bad, the user is closing app, WM_QUIT!
}



int getchJG() {
	UINT key = getNextKey(); // FIXME, look into this.
	return (int) key;
}



// CChildView


static CChildView* singletonWnd = NULL;



void invalidateWndJG() { // Used by curses-emulator.
  /* FIXME: we should limit/reduce these invalidates,
  to the actual necessary rectangles,
  and possibly stop 'erase=true'.
  */
  if (singletonWnd == NULL) { return;  }
	// assert(singletonWnd != NULL);
  singletonWnd->Invalidate(false); //  true);
}

void flashWndJG() {
  if (singletonWnd != NULL) {
    singletonWnd->FlashWindow(true);
  }
}

CChildView::CChildView()
{
	singletonWnd = this;
}

CChildView::~CChildView()
{
	singletonWnd = NULL;
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
  // cs.cx = 1900;
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

  // cs.cx = 1900;

	static CBrush winBackground(RGB(20, 40, 40)); // Dark bluish grey, let's us see how we draw.

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), 
		(HBRUSH)winBackground, // reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1),
		NULL // icon.
	);

	return TRUE;
}



void CChildView::OnTimer(UINT nIDEvent) { // Used to start app loop.
	debstr() << "ontimer:" << nIDEvent << "\n";
	KillTimer(timerID);
	// do stuff..
	UINT key = getNextKey(); // TODO: Add your command handler code here
	debstr() << "got:" << key << "\n";

	extern int moria_main(int argc, char *argv[]);
	static char *argv[] = { "moria.exz", NULL };
	moria_main(1, argv);
}




void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting


	CPen pen(PS_SOLID, 1, RGB(128, 0, 0));
	dc.SelectObject(pen);
	CBrush cellBackgroundBrush(RGB(0, 0, 20));   // Black  background
	// dc.SetBkMode(OPAQUE);
	dc.SetBkMode(TRANSPARENT);


	// const int cellw=16, cellh=16;
	// const int cellw=24, cellh=24;
	const int cellw = 20, cellh = 20;


	LOGFONT logFont;
	CFont* pOldFont = dc.GetCurrentFont();
	pOldFont->GetLogFont(&logFont);
	logFont.lfWeight = FW_BOLD;
	logFont.lfWidth = 0;

	// -17 and terminal, 'fits', but is too spaced out.
	// -28 and terminal, has proper widths, but breaks gyjgyj outliers.
	// -24 is a compromise.
	logFont.lfHeight = (int)-18; //-24; // 14*1.3f; //-50; //22;
	// const char* fontName = "Terminal"; //"STENCIL"; // "Terminal"
	const TCHAR* fontName = _T("Rockwell Extra Bold"); //"STENCIL"; // "Terminal"

	// _CRT_SECURE_NO_WARNINGS
	_tcsncpy(logFont.lfFaceName, fontName, sizeof logFont.lfFaceName / sizeof(TCHAR));
	// strncpy_s(logFont.lfFaceName, sizeof logFont.lfFaceName, fontName, sizeof logFont.lfFaceName);

	CFont newFont;
	newFont.CreateFontIndirect(&logFont);
	dc.SelectObject(newFont);


	// CPoint pos = CPoint(10, 15);


  extern CursesJG Csr;


	for (int row = 0; row<25; ++row) {
		for (int col = 0; col<80; ++col) {
      const ScreenCell& cell = Csr.cell(row, col);

			CPoint p(col, row);
			int x = col*cellw, y = row*cellh;
			CRect cellR(CPoint(x, y), CSize(cellw, cellh));

			// dc.Rectangle(&cellR);  
      CRect shrink = cellR;
      shrink.InflateRect(0, 0, -1, -1);
      dc.FillRect(&shrink, &cellBackgroundBrush);

			COLORREF back = RGB(0, 110, 0), front = RGB(255, 255, 0);
			if (cell.c == '@') { //  == pos) {
        // At least color the player differently..
				back = RGB(230, 120, 50); front = RGB(255, 200, 40);
        back = RGB(230, 120, 50); front = RGB(155, 100, 255);
      }

			// const TCHAR* flavor = _T("  MWmw You feel a stirring presence nearby.");
      // if (row == 1 && col < (int)_tcslen(flavor)) { // 
      // 	static TCHAR myfield[2] = _T("A");
      // 	item = myfield;
      // 	myfield[0] = flavor[col];
      // }
      // const TCHAR* item = _T("."); // "#"; // item = _T("@");

			dc.SetBkColor(back);
			dc.SetTextColor(front);

			// FrameRect Rectangle RoundRect
      char a_item[2] = "a"; a_item[0] = cell.c; // Make ascii string with char in it.

      CA2T u_item(a_item, CP_ACP); // Convert ascii string to Unicode. //  CP_UTF8);
      // CString s = u_item;

      // LPCWSTR
      dc.DrawText((LPCTSTR) u_item, //CString(u_item), 
        &cellR,
				DT_CENTER
				| DT_VCENTER
				| DT_SINGLELINE
				);

      sprites.drawSprite(cell.c, cellR, dc);

		}
	}

}

