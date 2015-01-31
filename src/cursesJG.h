#ifndef CURSES_JG_H
#define CURSES_JG_H

# define        ERR     (0)

typedef char chtype;

class ScreenCell {
public:
	enum CellConstants { Empty = ' ' };
	chtype c;
	// colors, graphics, sounds, fantastic other stuff.
	ScreenCell() { c = Empty; }
	ScreenCell(chtype c_) { c = c_; }

	void clearCell() {
		c = Empty;
	}
};

enum CursConstantsEnum {
	CSR_LINES = 25,
	CSR_COLS = 80
};

struct WINDOW {
	void overwrite(const WINDOW& src) { 
    WINDOW* that = this;
    // This was wrong:
    // ScreenCell* tmp = cells;
		// *this = src;  
		// cells = tmp;

    // This is wrong, only overlap should be copied!
		// FIXME, must copy cells too!
    for (int y = src.begin_y; y < src.begin_y+src.nlines; ++y) {
      for (int x = src.begin_x; x <src.begin_x + src.ncols; ++x) {
        cell(y, x) = src.cellConst(y, x);
      }
    }
	}

	int nlines, ncols; // , int begin_y, int begin_x;
	int begin_y, begin_x;
	int csr_x, csr_y;
	ScreenCell* cells;
	// int csr_y, csr_x;
	WINDOW(int nlines_, int ncols_, int begin_y_, int begin_x_)
		:nlines(nlines_), ncols(ncols_), begin_y(begin_y_), begin_x(begin_x_)
		, csr_y(0), csr_x(0)
	{
		if (nlines_ == 0) { nlines = (CSR_LINES - begin_y); }
		if (ncols_  == 0) { ncols  = (CSR_COLS  - begin_x);  }
		cells = new ScreenCell[nlines*ncols];
	}

	~WINDOW() { delete[] cells; }

	ScreenCell& cell(int y, int x) { // Will allow us to do bounds-check.
		assert(x >= 0);
		assert(x < ncols);
		assert(y >= 0);
		assert(y < nlines);
		return cells[y*ncols + x];
	} 

  const ScreenCell& cellConst(int y, int x) const { // Will allow us to do bounds-check.
    assert(x >= 0);
    assert(x < ncols);
    assert(y >= 0);
    assert(y < nlines);
    return cells[y*ncols + x];
  }

};


class CursesJG {
public:
	CursesJG();

	enum ConstantsEnum {
		LINES = CSR_LINES,
		COLS  = CSR_COLS
	};

	// FIXME: cells and csr should be moved to/handled by WINDOW,
	// and all Curses methods should delegate to cur_scr.
	// ScreenCell cells[LINES][COLS];
	// ScreenCell& cell(int y, int x) { return cells[y][x];  } // Will allow us to do bounds-check.
	// int csr_x, csr_y;

	int  mvaddstr(int y, int x, const char *str);  
	int  addstr(const char *str);  
	int  addch(const chtype ch);  
	int  mvaddch(int y, int x, const chtype ch);  
	int  move(int, int);  
	int  wrefresh(WINDOW *);  
	int  clear(); // Curses Spec: clear internal buffer, and MARK for redraw (don't actually trigger the redraw..) 
  int  refresh(); // Curses Spec: DO the actual redraw of stuff marked dirty.
  int  overwrite(const WINDOW *, WINDOW *);
	int  clrtoeol();  
	int  clrtobot();  
	void getyx(WINDOW *win, int& y, int& x); // Was a macro in curses.
	int  touchwin(WINDOW *win);  
	WINDOW *initscr(); //  { return NULL; }
	WINDOW *newwin(int nlines, int ncols, int begin_y, int begin_x); //  { return NULL; }
	int getch();  


	// Helpers: 
	void help_advCsr(); // Move cursor right, then new line, then top of screen.

  // External access from 'watchers':
  const ScreenCell& cell(int y, int x);
};

extern WINDOW *stdscr, *curscr;



#endif // CURSES_JG_H.
