/* Copyright (c) 1998 Lucent Technologies - All rights reserved. */
#ifndef _LIBG_H
#define _LIBG_H

/*
 *  Like Plan9's libg.h, but suitable for inclusion on non-Plan9 machines
 */

#include <X11/Xft/Xft.h>

#include <commands.h>

enum{ EMAXMSG = 128+8192 }; /* max event size */

/*
 * Cursors
 */

enum{
    DefaultCursor,
    BullseyeCursor,
    SweepCursor,
    DeadCursor,
    LockCursor
};

/*
 * Types
 */

typedef struct  Bitmap      Bitmap;
typedef struct  Point       Point;
typedef struct  Rectangle   Rectangle;
typedef struct  Keystroke   Keystroke;
typedef struct  Mouse       Mouse;
typedef struct  Menu        Menu;
typedef struct  Event       Event;
typedef struct  RGB     RGB;

struct  Point
{
    int x;
    int y;
};

struct Rectangle
{
    Point min;
    Point max;
};

struct  Bitmap
{
    Rectangle r;        /* rectangle in data area, local coords */
    Rectangle clipr;    /* clipping region */
    int ldepth;
    int id;     /* as known by the X server */
    Bitmap  *cache;     /* zero; distinguishes bitmap from layer */
    XftDraw *fd; /* font drawable */
    int flag;       /* flag used by X implementation of libg */
};

struct  Mouse
{
    int     buttons; /* bit array: LMR=124 */
    Point       xy;
    unsigned long   msec;
};

struct  Keystroke
{
    int k;
    int c;
    int t;
    Point p;
};

struct Menu
{
    char    **item;
    char    *(*gen)(int);
    int lasthit;
};

struct  Event
{
    Keystroke   keystroke;
    Mouse       mouse;
    int     n;      /* number of characters in mesage */
    unsigned char   data[EMAXMSG];  /* message from an arbitrary file descriptor */
};

struct RGB
{
    unsigned long   red;
    unsigned long   green;
    unsigned long   blue;
};

/*
 * Codes for bitblt etc.
 *
 *         D
 *       0   1
 *         ---------
 *   0 | 1 | 2 |
 *     S   |---|---|
 *   1 | 4 | 8 |
 *         ---------
 *
 *  Usually used as D|S; DorS is so tracebacks are readable.
 */
typedef
enum    Fcode
{
    Zero        = 0x0,
    DnorS       = 0x1,
    DandnotS    = 0x2,
    notS        = 0x3,
    notDandS    = 0x4,
    notD        = 0x5,
    DxorS       = 0x6,
    DnandS      = 0x7,
    DandS       = 0x8,
    DxnorS      = 0x9,
    D       = 0xA,
    DornotS     = 0xB,
    S       = 0xC,
    notDorS     = 0xD,
    DorS        = 0xE,
    F       = 0xF
} Fcode;

/*
 * Miscellany
 */

typedef void     (*Errfunc)(char *);

extern void addlatin(char, char, short);
extern Point     add(Point, Point);
extern Point     sub(Point, Point);
extern Point     mul(Point, int);
extern Point     divpt(Point, int);
extern Rectangle rsubp(Rectangle, Point);
extern Rectangle raddp(Rectangle, Point);
extern Rectangle inset(Rectangle, int);
extern Rectangle rmul(Rectangle, int);
extern Rectangle rdiv(Rectangle, int);
extern Rectangle rshift(Rectangle, int);
extern Rectangle rcanon(Rectangle);
extern Bitmap*   balloc(Rectangle, int);
extern void  bfree(Bitmap*);
extern int   rectclip(Rectangle*, Rectangle);
extern void  xtbinit(Errfunc, char*, int*, char**, char**);
extern void  bclose(void);
extern void  berror(char*);
extern void  bitblt2(Bitmap*, Point, Bitmap*, Rectangle, Fcode, unsigned long, unsigned long);
extern void  bitblt(Bitmap*, Point, Bitmap*, Rectangle, Fcode);


extern Point     string(Bitmap*, Point, XftFont*, char*, Fcode);
extern long  strwidth(XftFont*, char*);
extern Point     strsize(XftFont*, char*);
extern long  charwidth(XftFont*, Rune);
extern void  texture(Bitmap*, Rectangle, Bitmap*, Fcode);
extern void  wrbitmap(Bitmap*, int, int, unsigned char*);
extern void  rdbitmap(Bitmap*, int, int, unsigned char*);
extern void  wrbitmapfile(int, Bitmap*);
extern Bitmap*   rdbitmapfile(int);
extern int   ptinrect(Point, Rectangle);
extern int   rectXrect(Rectangle, Rectangle);
extern int   eqpt(Point, Point);
extern int   eqrect(Rectangle, Rectangle);
extern void  border(Bitmap*, Rectangle, int, Fcode, unsigned long);
extern void  cursorswitch(unsigned int);
extern void  cursorset(Point);
extern Rectangle bscreenrect(Rectangle*);
extern void  bflush(void);

extern int   clipr(Bitmap*, Rectangle);
extern int   scrpix(int*,int*);
extern unsigned long getbg(void);

extern void  einit(unsigned long);
extern unsigned long estart(unsigned long, int, int);

extern unsigned long event(Event*);
extern unsigned long eread(unsigned long, Event*);
extern Mouse     emouse(void);
extern Keystroke     ekbd(void);
extern void  pushkbd(int c);
extern int   ecanread(unsigned long);
extern int   ecanmouse(void);
extern int   ecankbd(void);
extern void  ereshaped(Rectangle);  /* supplied by user */

extern int   menuhit(int, Mouse*, Menu*);
extern Rectangle getrect(int, Mouse*);

extern void  rdcolmap(Bitmap*, RGB*);
extern void  wrcolmap(Bitmap*, RGB*);
extern void raisewindow(void);

/* Extra functions supplied by libXg */
extern int  snarfswap(char*, int, char**);
extern int  scrollfwdbut(void);

enum{
    Emouse      = 1,
    Ekeyboard   = 2
};

extern Point     Pt(int, int);
extern Rectangle Rect(int, int, int, int);
extern Rectangle Rpt(Point, Point);


#define Dx(r)   ((r).max.x-(r).min.x)
#define Dy(r)   ((r).max.y-(r).min.y)


extern  Bitmap  screen;
extern  XftFont *font;
extern  XftColor fontcolor;
extern  XftColor bgcolor;

#define BGSHORT(p)      (((p)[0]<<0) | ((p)[1]<<8))
#define BGLONG(p)       ((BGSHORT(p)<<0) | (BGSHORT(p+2)<<16))
#define BPSHORT(p, v)       ((p)[0]=(v), (p)[1]=((v)>>8))
#define BPLONG(p, v)        (BPSHORT(p, (v)), BPSHORT(p+2, (v)>>16))

extern int installbinding(int, KeySym, int, int);
extern int installchord(int, int, int, int);
#endif
