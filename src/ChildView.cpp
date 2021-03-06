
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "vsmoria.h"
#include "ChildView.h"

#include "debstr.h"

#include "cursesJG.h"



#include <math.h>
#include <gdiplus.h>

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#include "config.h"
#include "constant.h"
#include "types.h"
#include "externs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const int SpriteDrawer::cellwNarrow = CONST_CellWidthNarrow; // 20;
int SpriteDrawer::cellw = CONST_CellWidthDefault; // 32; //   20; // 32
int SpriteDrawer::cellh = CONST_CellWidthDefault; // 32;

CString tileFile = L"sprites\\nethack_tiles_32x32px_by_nevanda-d6w352s.png";

SpriteDrawer::SpriteDrawer()
:sprites2(tileFile) 
{
  // char buf[256];
  // GetCurrentDirectory(sizeof buf, buf);
  // int res = sprites.Load( _T("sprites\\sprites.png") );
  int res = sprites.Load(tileFile); // _T("sprites\\nethack_tiles_32x32px_by_nevanda-d6w352s.png") );
  initOK = (res >= 0);

  CString tilemapFile = _T("tileFile.txt");
  tileMap.load(tilemapFile);
  tileMap.buildHash();

  imgWidth = sprites.GetWidth();
  imgHeight = sprites.GetHeight();

  cellWidth = cellw; //  32; //  20; // 32; // imgWidth / colCount;
  cellHeight = cellh; // 32; //  imgHeight / rowCount; // dest.Height();

  colCount = 40; // 16; //  imgWidth / cellWidth;
  rowCount = imgHeight / cellHeight; // 16;
}



void rotator(CDC& dc, Gdiplus::Graphics& graphics, double angle) {
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms533866%28v=vs.85%29.aspx


	Image            image(L"potion.png"); // RotationInput.bmp");
	ImageAttributes  imageAttributes;
	UINT             width = image.GetWidth();
	UINT             height = image.GetHeight();

	REAL             degrees = angle; //60;
	REAL             pi = acos(-1.0);  // the angle whose cosine is -1.
	REAL             r = degrees * pi / 180;  // degrees to radians

	// graphics.DrawImage(&image, 10, 10, width, height);

	// COLORREF tintingColor = RGB(255,0,0);
	Color tintingColor(255,0,0); 
	// tintingColor.SetValue( Color::MakeARGB(255, angle, 255-angle, tintingColor.GetBlue() )); // tintingColor.GetGreen()

	float cr = tintingColor.GetRed()   / 255.0f;
  float cg = tintingColor.GetGreen() / 255.0f;
  float cb = tintingColor.GetBlue()  / 255.0f;

	ColorMatrix colorMatrix = {
		cr,  cg,  cb,  0,  0, 
		cb,  cr,  cg,  0,  0, 
		cg,  cb,  cr,  0,  0, 
		 0,   0,   0,  1,  0, 
		 0,   0,   0,  0,  1
      /*
	   cos(r),  sin(r), 0.0f, 0.0f, 0.0f,
	   -sin(r), cos(r), 0.0f, 0.0f, 0.0f,
	   0.0f,    0.0f,   1.0f, 0.0f, 0.0f,
	   0.0f,    0.0f,   0.0f, 1.0f, 0.0f,
	   0.0f,    0.0f,   0.0f, 0.0f, 1.0f
	  */
	};
   
	imageAttributes.SetColorMatrix(
	   &colorMatrix, 
	   ColorMatrixFlagsDefault,
	   ColorAdjustTypeBitmap);   

	graphics.DrawImage(
	   &image, 
	   Gdiplus::Rect(32, 5, width, height),  // destination rectangle 
	   0, 0,        // upper-left corner of source rectangle 
	   width,       // width of source rectangle
	   height,      // height of source rectangle
	   UnitPixel,
	   &imageAttributes);

	CRect rect(0,555,32,590);
	CBrush brush(tintingColor.ToCOLORREF());                    //RGB(0,0,20));   // Black  background
	dc.FillRect(&rect, &brush);
}





void SpriteDrawer::drawSprite(int myChar, CRect& dest, CDC& dc, Gdiplus::Graphics& graphics, int creatureThingIndex, TileEnum tileType, COLORREF color, int matIndex) { // , int tval) {
  if (!initOK) { return;  } // Fail silently/gracefully.

  int row = 0, col = 0;
  if (false) {
    row = myChar / colCount;
    col = myChar % colCount;
  } else {
    bool bFound = tileMap.charFromHash(myChar, col, row, creatureThingIndex, tileType); // row, col);
    if (!bFound) { return; } // E.g. 'store digits' are not in this set.
  }

  int offset_x = col * cellWidth;
  int offset_y = row * cellHeight;
  CRect srcR(CPoint(0, 0), CSize(cellWidth, cellHeight));
  srcR.OffsetRect(offset_x, offset_y);

  sprites.Draw(dc, dest, srcR);

  if ((matIndex < 0 || matIndex >= MAX_COLORS) || tileType == Ti_Potion) { return; } //  (tval != TV_POTION1 && tval != TV_POTION2)) { return; }  //  color == colorNone)

  COLORREF matColor = colors[matIndex].color;

  CBrush brush(matColor);
  // dc.SelectObject(brush);
  dc.FillRect(&dest, &brush);

  Gdiplus::Rect dest2( dest.left,dest.top,  dest.Width(),dest.Height()); 

  Color tintingColor( GetRValue(matColor), GetGValue(matColor), GetBValue(matColor) ); // 255, 0, 0);
  // int angle = 154;
	// tintingColor.SetValue( Color::MakeARGB(255, angle, 255-angle, tintingColor.GetBlue() )); // tintingColor.GetGreen()

	float cr = tintingColor.GetRed()   / 255.0f;
  float cg = tintingColor.GetGreen() / 255.0f;
  float cb = tintingColor.GetBlue()  / 255.0f;

  ColorMatrix colorMatrix = {
    // 1,0,0,0,0, 
    cr, cg, cb, 0, 0,
    // 0,1,0,0,0, 
    cb, cr, cg, 0, 0,
    // 0,0,1,0,0, 
    cg, cb, cr, 0, 0,
    0, 0, 0, 1, 0,
    0, 0, 0, 0, 1
  }; 
  ImageAttributes  imageAttributes;

	imageAttributes.SetColorMatrix(
	   &colorMatrix, 
	   ColorMatrixFlagsDefault,
	   ColorAdjustTypeBitmap);   


  // UINT             zzwidth = sprites2.GetWidth();
	graphics.DrawImage(
	   &sprites2, 
	   dest2, // Rect(32, 5, width, height),  // destination rectangle 
	   offset_x, offset_y, // 0, 0,        // upper-left corner of source rectangle 
	   cellWidth, // width,       // width of source rectangle
	   cellHeight, // height,      // height of source rectangle
	   UnitPixel,
	   &imageAttributes);


}








void keyEaterLoopJGImpl(int waitCount, int waitMS) {
  debstr() << "keyEaterLoopJG begin\n";
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

void keyEaterLoopJG() {
  // JG: be careful this doesn't slow us too much..
  const int waitCount = 4;
  const int waitMS = 25;
  keyEaterLoopJGImpl(waitCount, waitMS);
}


void processQueuedMsgs() {
  // JG: be careful this doesn't slow us too much..
  const int waitCount = 2;
  const int waitMS = 5;
  keyEaterLoopJGImpl(waitCount, waitMS);
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



void invalidateWndJG(CRect* pRect, bool erase) { // Used by curses-emulator.
  /* FIXME: we should limit/reduce these invalidates,
  to the actual necessary rectangles,
  and possibly stop 'erase=true'.
  */
  if (singletonWnd == NULL) { return;  }
	// assert(singletonWnd != NULL);
  singletonWnd->InvalidateRect(pRect, erase); //  true);
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


//const int cellw = 20, cellh = 20;


void invalidateCell(int row, int col) {
  int cellw2 = SpriteDrawer::cellw;
  if (row == 0) { cellw2 = SpriteDrawer::cellwNarrow;  }

	int x = col*cellw2, y = row*SpriteDrawer::cellh;
	CRect cellR(CPoint(x, y), CSize(cellw2, SpriteDrawer::cellh));
  invalidateWndJG(&cellR, false);
}








void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting


	CPen pen(PS_SOLID, 1, RGB(16, 0, 32));
	dc.SelectObject(pen);
	// dc.SetBkMode(OPAQUE);
	dc.SetBkMode(TRANSPARENT);


	// const int cellw=16, cellh=16;
	// const int cellw=24, cellh=24;


	LOGFONT logFont;
	CFont* pOldFont = dc.GetCurrentFont();
	pOldFont->GetLogFont(&logFont);
	logFont.lfWeight = FW_BOLD;
	logFont.lfWidth = 0;

	// -17 and terminal, 'fits', but is too spaced out.
	// -28 and terminal, has proper widths, but breaks gyjgyj outliers.
	// -24 is a compromise.
	// logFont.lfHeight = (int)-18; //IT WAS 18 for 20tile
	logFont.lfHeight = (int)-29; //So we take 29 for 32tile.
	// const char* fontName = "Terminal"; //"STENCIL"; // "Terminal"
	const TCHAR* fontName = _T("Rockwell Extra Bold"); //"STENCIL"; // "Terminal"

	// _CRT_SECURE_NO_WARNINGS
	_tcsncpy(logFont.lfFaceName, fontName, sizeof logFont.lfFaceName / sizeof(TCHAR));
	// strncpy_s(logFont.lfFaceName, sizeof logFont.lfFaceName, fontName, sizeof logFont.lfFaceName);

	CFont newFont;
	newFont.CreateFontIndirect(&logFont);
	dc.SelectObject(newFont);



  extern CursesJG Csr;

	Graphics graphics(dc);

	for (int row = 0; row<Csr.LINES; ++row) {
		for (int col = 0; col<Csr.COLS; ++col) {
      const ScreenCell& cell = Csr.cell(row, col);

			CPoint p(col, row);
      int cellw2 = SpriteDrawer::cellw;
      if (row == 0) { cellw2 = SpriteDrawer::cellwNarrow;  } // KLUDGE TO READ TEXT, on top msg row.
			int x = col*cellw2, y = row*SpriteDrawer::cellh;
			CRect cellR(CPoint(x, y), CSize(cellw2, SpriteDrawer::cellh));

      COLORREF cellBackground = RGB(0, 0, 20); // "Black"  background
      if (cell.back != colorNone) { cellBackground = cell.back;  }
	    CBrush cellBackgroundBrush(cellBackground);   

			dc.Rectangle(&cellR);  
      CRect shrink = cellR;
      shrink.InflateRect(0, 0, -1, -1);
      dc.FillRect(&shrink, &cellBackgroundBrush);

      COLORREF back = RGB(0, 10, 0);
      COLORREF front = RGB(250, 255, 250); // cell.color; //  RGB(255, 255, 0);
			if (cell.color != colorNone) { // .c == '@') { //  == pos) {
        // At least color the player differently..
        front = cell.color;
      }

			dc.SetBkColor(back);
			dc.SetTextColor(front);

      char a_item[2] = "a"; a_item[0] = cell.c; // Make ascii string with char in it.
      CA2T u_item(a_item, CP_ACP); // Convert ascii string to Unicode. //  CP_UTF8);

      dc.DrawText((LPCTSTR) u_item, 
        &cellR,
				DT_CENTER
				| DT_VCENTER
				| DT_SINGLELINE
				);

      if (cell.color != colorNone || cell.tile == Ti_Thing || cell.tile == Ti_Environ) { // IE if monster, not text..
        sprites.drawSprite(cell.c, cellR, dc, graphics, cell.creatureThingIndex, cell.tile, cell.color, cell.matIndex); //  , cell.tval);
      }
		}
	}

}

