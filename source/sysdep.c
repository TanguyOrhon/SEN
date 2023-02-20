/****************************************************************
 * calc
 *  (C) 1993-2021 R. Grothmann
 *  (C) 2021-2022 E. Bouchare
 *
 * sysdepmin.c
 *
 ****************************************************************/
/* Primitive user interface for calc.
Missing are:
	- graphics
	- single keystrokes (e.g. for tracing)
	- extended command line editing and command history
	- wait does not work, but waits for a keystroke
*/

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

#include "fsl_power.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#include "sysdep.h"
#include "calc.h"
#include "stack.h"

Calc _calc;
Calc *calc=&_calc;

static int editing=0;

/*********************** text IO routines **********************/

/* Allow color and prompt customisation
 *   valid 'mode' parameter values include
 *   - CC_OUTPUT: prepare for output display
 *   - CC_WARN: prepare for warning display
 *   - CC_ERROR: prepare for error display
 *   - CC_EDIT: prepare for standard input (prompt "> ")
 *   - CC_FEDIT: prepare for function body input (prompt "$ ")
 */
void sys_out_mode (int mode)
{
	switch (mode) {
		case CC_EDIT:
			printf("> ");
			break;
		case CC_FEDIT:
			printf("$ ");
			break;
		default:
			break;
	}
	fflush(stdout);
}

void sys_print (char *s)
/*****
Print a line onto the text screen, parse tabs and '\n'.
Printing should be done at the cursor position. There is no need
to clear the line at a '\n'.
The cursor should move forward with the print.
Think of the function as a simple emulator.
If you have a line buffered input with echo then do not print,
when the command line is on.
*****/
{
	if (!editing) {
		printf("%s",s);
		fflush(stdout);
	}
}

int sys_wait_key (scan_t *scan)
/***** 
	wait for a keystroke. return the scancode and the ascii code.
	scancode should be a code from scan_t. Do at least generate
	'enter'.
*****/
{	int c;
	c=getchar();
	switch (c) {
	case EOF:
	case '\n' : *scan=enter; break;
	default : *scan=key_normal;
	}
	return c;
}


int sys_test_key (void)
/***** test_key
	see, if user pressed the keyboard.
	return the scancode, if he did.
*****/
{
	return 0;
}

void text_mode (void)
{
}

/********************** Text output ***************************/

/****
The following text screen commands should be emulated on a graphic
work station. This can be done by a standard emulator (e.g. VT100)
or within a window displaying the text. Additional features may be
added, such as viewing old text. But the input line should be
visible as soon as a key is pressed by the user.
****/

void sys_clear (void)
/***** Clear the text screen 
******/
{
}

void move_cl_cb (void)
/* move the text cursor left */
{
}

void move_cr_cb (void)
/* move the text cursor right */
{
}

void cursor_on_cb (void)
/* switch cursor on */
{
}

void cursor_off_cb (void)
/* switch cursor off */
{
}

void clear_eol (void)
/* clear the text line from cursor position */
{
}

void edit_off_cb (void)
/* the command line is no longer in use (graphics or computing) */
{
	editing=0;
}

void edit_on_cb (void)
/* the command line is active */
{
	editing=1;
}

void page_up_cb(void)
{
}

void page_down_cb(void)
{
}

/***************** clock and wait ********************/

real sys_clock (void)
/***** define a timer in seconds. 
******/
{
	return 0.0;
}

void sys_wait (real time, scan_t *scan)
/***** Wait for time seconds or until a key press.
Return the scan code or 0 (time exceeded).
******/
{	int c=getchar();
	printf(" *** Press Return\n");
	switch(c) {
		case 27 : *scan=escape; break;
		default : *scan=enter;
	}
}

/**************** directory *******************/

/* search path list:
 *   path[0]       --> current directory
 */
char *path[MAX_PATH];
int npath=0;

char *fs_cd (char *dir)
/* sets the path if dir!=0 and returns the path*/
{
	return path[0];
}


/*
 *	scan a directory and get :
 *		files : an array of entries matching the pattern
 *		files_count : number of files entries
 *	the function returns the max length of a file entry
 */
int fs_dir(char *dir_name, char *pat, char ** files[], int *files_count)
{
	int len=0;
	return len;
}

int fs_rm(char* filename)
{
	return -1;
}

int fs_mkdir(char* dirname)
{
	return -1;
}

int fs_exec (char *name, char *args)
/**** execute
	Call an external program, return 0, if there was no error.
	No need to support this on multitasking systems.
****/
{
	return -1;
}

/******************* Graphics IO ***************************/

void getpixel (double *x, double *y)
/***** Compute the size of pixel in screen coordinates.
******/
{	*x=1;
	*y=1;
}

void gflush (void)
/***** Flush out remaining graphic commands (for multitasking).
This serves to synchronize the graphics on multitasking systems.
******/
{
}

void graphic_mode (void)
{
}

void gclear (void)
/***** clear the graphics screen 
*****/
{

}

void mouse (int* x, int* y)
/****** mouse
	wait, until the user marked a screen point with the mouse.
	Return screen coordinates.
******/
{	*x=0; *y=0;
}

/*************** main ********************************/

int main (void)
/******
Initialize memory and call main_loop
******/
{
    /* Init board hardware. */
    /* set BOD VBAT level to 1.65V */
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);
    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
	#if !defined(DONT_ENABLE_FLASH_PREFETCH)
    	/* enable flash prefetch for better performance */
    	SYSCON->FMCCR |= SYSCON_FMCCR_PREFEN_MASK;
	#endif


	calc->ramstart=(char*)0x20010000;
	calc->ramend=(char*)0x20040000;

	
	/* set up default pathes and directory */
	
	path[0]=".";
	npath=1;
	
	/* get width of the terminal */
	calc->termwidth = TERMWIDTH;
	
	main_loop(calc,0,NULL);

	return EXIT_SUCCESS;
}
