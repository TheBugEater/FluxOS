#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include <stdarg.h>

#define GRAPHIC_DEVICE 0xB8000

#define TERMINAL_WIDTH 80
#define TERMINAL_HEIGHT 25

// Type Declarations
enum TERMINAL_COLOR
{
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};

typedef struct _TerminalVector
{
    unsigned int X;
    unsigned int Y;
} TerminalVector;

// Variable Declarations
static TerminalVector currentCursor = {.X = 0,.Y = 0};
static unsigned char terminalColor;

// Function Declarations
unsigned int strlen(const char* message);
int putchar(int c);

void printchar(char **str, int c);

int prints(char **out, const char *string, int width, int pad);

int printi(char **out, int i, int b, int sg, int width, int pad, int letbase);

int print(char **out, const char *format, va_list args );

int printf(const char *format, ...);

int sprintf(char *out, const char *format, ...);

void clrscr();

void setcolor(enum TERMINAL_COLOR fg, enum TERMINAL_COLOR bg);

void terminal_initialize();
#endif
