
// ChildView.h : interface of the CChildView class
//


#pragma once

class SpriteDrawer {
public:
  CImage sprites;
  bool initOK;
  SpriteDrawer();

  void drawSprite(int myChar, CRect& dest, CDC& dc);
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

