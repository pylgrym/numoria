
// ChildView.h : interface of the CChildView class
//


#pragma once

#include <gdiplus.h>

#include "./Tilemap.h"
#include "./LocInf.h"

class SpriteDrawer {
public:
  CImage sprites;
  Gdiplus::Image sprites2; // (L"potion.png"); // RotationInput.bmp");

  int imgWidth, imgHeight;
  int rowCount, colCount;
  int cellWidth, cellHeight;

  Tilemap tileMap;
  bool initOK;
  SpriteDrawer();

  void drawSprite(int myChar, CRect& dest, CDC& dc, 
	  Gdiplus::Graphics& graphics,
    int creatureThingIndex,
    TileEnum tileType,
    COLORREF color,
    int matIndex 
    // int tval
  );

  static const int cellwNarrow; // = 20;
  static int cellw; // = 32; //   20; // 32 // Will vary on runtime!
  static int cellh; // = 32;
  enum { CONST_CellWidthDefault = 32, CONST_CellWidthNarrow = 20 }; // Don't use these directly, use the statics above instead.
  static void setCellWidth(int newWidth) { cellw = newWidth; }
};

// CChildView window

const int timerID = 1001;

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:
  SpriteDrawer sprites;

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
};

