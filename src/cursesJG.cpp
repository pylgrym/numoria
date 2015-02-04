#include "stdafx.h"
#include "cursesJG.h"
#include "LocInf.h"

#include <assert.h>

/* FIXME; I want to catch calls to fprintf(stderror, e.g. 
..(void) fprintf (stderr, "Can't open score file \"%s\"\n", MORIA_TOP);

NB! - anything failing should return ERR=0.
*/
extern void invalidateWndJG(CRect* pRect, bool erase);
extern void invalidateCell(int row, int col);


extern WINDOW *stdscr=NULL, *curscr=NULL;

CursesJG::CursesJG() {
	stdscr = NULL;
	curscr = NULL;
	// csr_x=0, csr_y=0;
}

// WINDOW   * CursesJG::stdscr = NULL, *curscr = NULL;


int  CursesJG::move(int y, int x) { curscr->csr_y = y, curscr->csr_x = x; return 1; }

int  CursesJG::addch(const chtype ch, COLORREF backColor) { 
  curscr->cell(curscr->csr_y, curscr->csr_x) = ScreenCell(ch, backColor); 
  invalidateCell(curscr->csr_y, curscr->csr_x);
  help_advCsr(); return 1; 
}
int  CursesJG::addch_L(const struct LocInf& ch) { 
  curscr->cell(curscr->csr_y, curscr->csr_x) = ScreenCell(ch); 
  invalidateCell(curscr->csr_y, curscr->csr_x);
  help_advCsr(); return 1; 
}

void CursesJG::help_advCsr() { // Move cursor right, then new line, then top of screen.
	curscr->csr_x += 1; // Move right.
	if (curscr->csr_x < curscr->ncols) { return; } // COLS

	curscr->csr_x = 0; curscr->csr_y += 1; // If we hit right edge, move to left start of next line.
	if (curscr->csr_y < curscr->nlines) { return; } // LINES

	// If we hit bottom, move to top left of screen:
	curscr->csr_y = 0;
}

int  CursesJG::mvaddch(int y, int x, const chtype ch) { move(y, x); addch(ch, colorNone); return 1; }
int  CursesJG::mvaddch_L(int y, int x, const struct LocInf& ch) { move(y, x); addch_L(ch); return 1; }

int CursesJG::addstr(const char *str, COLORREF backColor) {
	int count = 0;
	for (int i = 0; str[i] != NULL; ++i, ++count) {
		addch(str[i], backColor);
		if (count > (COLS*LINES)) {
			assert(false); // string too long, in addstr().
			count = 1 / (count - count);
		}
	}
	return 1;
}

int CursesJG::mvaddstr(int y, int x, const char *str, COLORREF backColor) { move(y, x); addstr(str, backColor); return 1; }

int CursesJG::wrefresh(WINDOW *) { 
	// FIXME, window-mechanism doesn't exist yet.
  // invalidateWndJG(NULL); // in clear.
	return 1; 
}





int CursesJG::clear() { 
	assert(curscr != NULL);
	for (int y = 0; y < curscr->nlines; ++y) { // LINES
		for (int x = 0; x < curscr->ncols; ++x) { // COLS
			curscr->cell(y, x).clearCell();
		}
	}
  // http://linux.die.net/man/3/clear
  // NB, doc of clear() says it also causes repaint-from-scratch ON NEXT WREFRESH.
  // I'll just trigger a complete redraw here
  // (So far I only use MS-Window's Invalidate, 
  // I don't keep separate track of 'which parts are dirty/need refreshing', on char-by-char basis.)
  invalidateWndJG(NULL, true); // in clear.

	return 1; 
}


int CursesJG::refresh() {
  /*
   JG: Beware that refresh no longer TRIGGERS redraw-dirty, which should be OK,
   because it was never meant to TRIGGER, but instead to allow execution of queued dirty-draws.
   However, it might make sense to hook it up with a small 'peekmessage' loop,
   to allow system to process a bit of painting.. on the other hand, we are already supposed to reach 'inkey waits', waiting for user input.
   However, in animation-situaitons, maybe loops here will allow us to SEE the animation..?
  */

  extern void processQueuedMsgs();
  // Don't, it makes everything slow:
  // processQueuedMsgs();

  // Consider: should call UpdateWindow or something.
  // invalidateWndJG(); // in refresh
  /* refresh is the action that should cause the actual redraw to screen,
  and arguably should even 'force' it / do it immediately (think 'updateWindow/redrawWindow').
    Currently, both 'clear' and 'refresh' do similar things,
  but that is because the actual-actual redrawing will only happen
  whenever we reach a 'get-next-key-press' eventloop.
    However, if the perceived behaviour of running moria appears weird,
  we should look into how badly we (dis)respect these intentions of curses-code.
  */
  // Note that docs/specs have info on how 'touched' windows behave:
  // http://linux.die.net/man/3/refresh
  return 1;
}


int CursesJG::clrtoeol() { 
	assert(curscr != NULL);
	for (int x = curscr->csr_x; x < curscr->ncols; ++x) { // COLS
		curscr->cell(curscr->csr_y, x).clearCell();
  invalidateCell(curscr->csr_y, x);
	}
	return 1;
}

int CursesJG::clrtobot() { 
	assert(curscr != NULL);
	// We clear from cursor, to the right,
	// and then remaining lines on screen:
	int x = curscr->csr_x;
	for (int y = curscr->csr_y; y < curscr->nlines; ++y) { // LINES
		for ( ; x < curscr->ncols; ++x) { // COLS
			curscr->cell(y, x).clearCell();
      invalidateCell(y, x);
		}
		x = 0; // Rest of lines, start at the left side.
	}
	return 1;
}


int CursesJG::overwrite(const WINDOW * src, WINDOW * tgt) { 
  tgt->overwrite(*src);  
  invalidateWndJG(NULL, true); // false); // in touchwin.
  return 1; 
}

void CursesJG::getyx(WINDOW *win, int& y, int& x) { y = win->csr_y; x = win->csr_x; }

int CursesJG::touchwin(WINDOW *win) { 
	// cur_scr = win; // Possibly this?
  invalidateWndJG(NULL, true); // in touchwin.
  return 1;
}


WINDOW* CursesJG::initscr() { 
	// http://pubs.opengroup.org/onlinepubs/7908799/xcurses/initscr.html
	stdscr = newwin(0, 0, 0, 0); // LINES // COLS
	curscr = stdscr;  // required behaviour of initscr.
	// FIXME/todo/Consider: initscr is supposed to refresh/init actual screen too!
	invalidateWndJG(NULL, true); // in initscr.
	return stdscr; 
}

WINDOW* CursesJG::newwin(int nlines, int ncols, int begin_y, int begin_x) { 
	return new WINDOW(nlines,ncols,begin_y, begin_x); 
}

extern int getchJG();

int CursesJG::getch() { return getchJG(); }







const ScreenCell& CursesJG::cell(int y, int x) {
  static const ScreenCell empty('§');
  static const ScreenCell bad('£');
  if (stdscr == NULL) { return empty; }
  if (y < 0 || y >= stdscr->nlines || x < 0 || x >= stdscr->ncols) { return bad;  }
  return stdscr->cell(y, x);
}
