#define GRAPHIC_DEVICE 0xB8000

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

    TerminalVector()
    {
        X = 0;
        Y = 0;
    }
}TerminalVector{ 0,0 };

// Variable Declarations
TerminalVector currentCursor;
unsigned char terminalColor;

// Function Declarations
static unsigned int strlen(const char* message);
static int putchar(int c);

static void printchar(char **str, int c);

static int prints(char **out, const char *string, int width, int pad);

static int printi(char **out, int i, int b, int sg, int width, int pad, int letbase);

static int print(char **out, const char *format, va_list args );

static int printf(const char *format, ...);

static int sprintf(char *out, const char *format, ...);

static void clrscr();

static void setcolor(TERMINAL_COLOR fg, TERMINAL_COLOR bg);

