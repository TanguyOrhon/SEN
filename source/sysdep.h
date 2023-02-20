/****************************************************************
 * calc
 *  (C) 1993-2021 R. Grothmann
 *  (C) 2021-2022 E. Bouchare
 *
 * sysdep.h
 *
 ****************************************************************/
#ifndef SYSDEP_H
#define SYSDEP_H


#define TERMWIDTH		80
#define TERMHEIGHT		24

#define HELPFILE "help.txt"

#include <math.h>
#include <float.h>

#define ALIGNMENT	4
#define ALIGN(sz)	((sz)!=0 ? ((((sz)-1)/ALIGNMENT)+1)*ALIGNMENT : 0)

#define LABEL_LEN_MAX		14	/* Maximum length of an identifier */

#define LONG	long
#define ULONG	unsigned long

#ifdef FLOAT32
typedef float	real;

#define fmod	fmodf
#define floor	floorf
#define ceil	ceilf
#define	fabs	fabsf
#define sqrt	sqrtf
#define pow		powf
#define log		logf
#define exp		expf
#define cos		cosf
#define sin		sinf
#define tan		tanf
#define acos	acosf
#define asin	asinf
#define atan	atanf
#define atan2	atan2f

#define EPSILON	FLT_EPSILON

#else

typedef double	real;

#define fmod	fmod
#define floor	floor
#define ceil	ceil
#define fabs	fabs
#define sqrt	sqrt
#define pow		pow
#define log		log
#define exp		exp
#define cos		cos
#define sin		sin
#define tan		tan
#define acos	acos
#define asin	asin
#define atan	atan
#define atan2	atan2

#define EPSILON	DBL_EPSILON

#endif

#ifndef M_PI
#define M_PI            3.14159265358979323846
#endif

/* Filesystem interaction */
#ifdef _WIN32
#define PATH_DELIM_CHAR '\\'
#define PATH_DELIM_STR "\\"
#else
#define PATH_DELIM_CHAR '/'
#define PATH_DELIM_STR "/"
#endif

#define MAX_PATH	5

extern char* path[MAX_PATH];
extern int npath;

char* fs_cd (char *dir);
int   fs_dir(char *dir_name, char *pat, char ** files[], int *files_count);
int   fs_rm(char *filename);
int   fs_mkdir(char* dirname);
int   fs_exec(char *name, char *args);

#define EXTENSION ".e"

typedef enum {
	key_normal, cursor_up, cursor_down, cursor_left, cursor_right,
	escape, delete, backspace, clear_home, switch_screen, enter,
	space, line_end, line_start, fk1, fk2, fk3, fk4, fk5, fk6, fk7,
	fk8, fk9, fk10, fk11, fk12, word_left, word_right, help, sel_insert,
	page_up, page_down, eot
} scan_t;

/* time */
real sys_clock (void);
void sys_wait (real delay, scan_t *scan);

/* text IO */
/*** output modes ***
   CC_OUTPUT:	standard result output mode
   CC_EDIT:		edited character echo mode
   CC_FEDIT:	edited function echo message
   CC_WARN:		warning message mode
   CC_ERROR:	error message mode
 */
#define	CC_OUTPUT		1
#define	CC_WARN			2
#define	CC_ERROR		3
#define CC_EDIT			4
#define CC_FEDIT		5

void sys_out_mode(int mode);
int sys_wait_key (scan_t *scan);
int sys_test_key (void);

/* output */
void sys_clear (void);		/* clear the output peripheral */
void sys_print (char *s);	/* print an output text (no newline) */

void text_mode (void);

void graphic_mode (void);
void gflush (void);		/* flush out graphics */
void gclear (void);		/* clear the graphical screen */

void edit_on_cb (void);
void edit_off_cb (void);
void cursor_off_cb (void); 
void cursor_on_cb (void);
void move_cr_cb (void);
void move_cl_cb (void);
void page_up_cb(void);
void page_down_cb(void);

void clear_eol (void);

#endif
