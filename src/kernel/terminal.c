#include <kernel/terminal.h>
#include <utility/utility.h>

void printchar(char **str, int c)
{
	extern int putchar(int c);
	
	if (str) {
		**str = c;
		++(*str);
	}
	else (void)putchar(c);
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

int prints(char **out, const char *string, int width, int pad)
{
	register int pc = 0, padchar = ' ';

	if (width > 0) {
		register int len = 0;
		register const char *ptr;
		for (ptr = string; *ptr; ++ptr) ++len;
		if (len >= width) width = 0;
		else width -= len;
		if (pad & PAD_ZERO) padchar = '0';
	}
	if (!(pad & PAD_RIGHT)) {
		for ( ; width > 0; --width) {
			printchar (out, padchar);
			++pc;
		}
	}
	for ( ; *string ; ++string) {
		printchar (out, *string);
		++pc;
	}
	for ( ; width > 0; --width) {
		printchar (out, padchar);
		++pc;
	}

	return pc;
}

/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12

int printi(char **out, int i, int b, int sg, int width, int pad, int letbase)
{
	char print_buf[PRINT_BUF_LEN];
	register char *s;
	register int t, neg = 0, pc = 0;
	register unsigned int u = i;

	if (i == 0) {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return prints (out, print_buf, width, pad);
	}

	if (sg && b == 10 && i < 0) {
		neg = 1;
		u = -i;
	}

	s = print_buf + PRINT_BUF_LEN-1;
	*s = '\0';

	while (u) {
		t = u % b;
		if( t >= 10 )
			t += letbase - '0' - 10;
		*--s = t + '0';
		u /= b;
	}

	if (neg) {
		if( width && (pad & PAD_ZERO) ) {
			printchar (out, '-');
			++pc;
			--width;
		}
		else {
			*--s = '-';
		}
	}

	return pc + prints (out, s, width, pad);
}

int print(char **out, const char *format, va_list args )
{
	register int width, pad;
	register int pc = 0;
	char scr[2];

	for (; *format != 0; ++format) {
		if (*format == '%') {
			++format;
			width = pad = 0;
			if (*format == '\0') break;
			if (*format == '%') goto out;
			if (*format == '-') {
				++format;
				pad = PAD_RIGHT;
			}
			while (*format == '0') {
				++format;
				pad |= PAD_ZERO;
			}
			for ( ; *format >= '0' && *format <= '9'; ++format) {
				width *= 10;
				width += *format - '0';
			}
			if( *format == 's' ) {
				register char *s = (char *)va_arg( args, int );
				pc += prints (out, s?s:"(null)", width, pad);
				continue;
			}
			if( *format == 'd' ) {
				pc += printi (out, va_arg( args, int ), 10, 1, width, pad, 'a');
				continue;
			}
			if( *format == 'x' ) {
				pc += printi (out, va_arg( args, int ), 16, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'X' ) {
				pc += printi (out, va_arg( args, int ), 16, 0, width, pad, 'A');
				continue;
			}
			if( *format == 'u' ) {
				pc += printi (out, va_arg( args, int ), 10, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'c' ) {
				/* char are converted to int then pushed on the stack */
				scr[0] = (char)va_arg( args, int );
				scr[1] = '\0';
				pc += prints (out, scr, width, pad);
				continue;
			}
		}
		else {
		out:
			printchar (out, *format);
			++pc;
		}
	}
	if (out) **out = '\0';
	va_end( args );
    updatecursor();
	return pc;
}

int printk(const char *format, ...)
{
        va_list args;
        
        va_start( args, format );
        return print( 0, format, args );
}

int sprintk(char *out, const char *format, ...)
{
        va_list args;
        
        va_start( args, format );
        return print( &out, format, args );
}

///////////////////////////////////////////////////
/////////// Terminal Functions ////////////////////
unsigned int strlen(const char* message)
{
	unsigned int ret = 0;
	while(message[ret] != 0)
		ret++;

	return ret;

}

int puts ( const char * str )
{
    printk(str);
}

int putchar(int character)
{
    unsigned short* pGraphicDevice = (unsigned short*)GRAPHIC_DEVICE;

    const unsigned int index = currentCursor.Y * TERMINAL_WIDTH + currentCursor.X;

    if(character == '\n')
    {
        currentCursor.X = 0;
        currentCursor.Y++;
        scroll();
        return character;
    }
    else if(character == '\t')
    {
        currentCursor.X += 10;
        if(currentCursor.X > TERMINAL_WIDTH)
        {
            currentCursor.X = TERMINAL_WIDTH;
        }
        scroll();
        return character;
    }

	pGraphicDevice[index] = character | terminalColor << 8;

	if ( ++currentCursor.X >= TERMINAL_WIDTH )
	{
		currentCursor.X = 0;
		if ( ++currentCursor.Y >= TERMINAL_HEIGHT )
		{
			currentCursor.Y = 0;
		}
        scroll();
        return character;
	}

    return 0;
}

void setcolor(enum TERMINAL_COLOR fg, enum TERMINAL_COLOR bg)
{
	terminalColor = fg | bg << 4;
}

void clrscr()
{
    currentCursor  = (TerminalVector){.X = 0,.Y = 0};

 	unsigned short *pGraphicDevice = (unsigned short*)GRAPHIC_DEVICE;

	for(unsigned int y = 0; y < TERMINAL_HEIGHT; y++)
	{
		for(unsigned int x = 0; x < TERMINAL_WIDTH; x++)
		{
			int index = y * TERMINAL_WIDTH + x;
			pGraphicDevice[index] = ' ' | terminalColor << 8;
		}	
	}
   
}

void scroll()
{
 	unsigned short *pGraphicDevice = (unsigned short*)GRAPHIC_DEVICE;

    // TERMINAL_HEIGHT - 1 = Last Line
    if(currentCursor.Y >= TERMINAL_HEIGHT - 1)
    {
       // Get the array index after skipping the FluxOS ~10 lines:
        
       unsigned int startPos = LOGON_MESSAGE_HEIGHT * TERMINAL_WIDTH;
       unsigned int copyStart = startPos + TERMINAL_WIDTH;
       unsigned int lastLine = TERMINAL_WIDTH * (TERMINAL_HEIGHT - 1);
        
       memcpy(pGraphicDevice + startPos, pGraphicDevice + copyStart,2 * TERMINAL_WIDTH * (TERMINAL_HEIGHT - LOGON_MESSAGE_HEIGHT));
       memset(pGraphicDevice + lastLine, 0, TERMINAL_WIDTH);
        
       currentCursor.Y = TERMINAL_HEIGHT - 2;
    }
}

void updatecursor()
{
    unsigned int pos = currentCursor.Y * TERMINAL_WIDTH + currentCursor.X;

    outb(0x3D4, 14);
    outb(0x3D5, pos >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, pos);
}

void terminal_initialize()
{
   setcolor(COLOR_LIGHT_GREY, COLOR_BLACK);
   clrscr();
/*
    char *logonMessage = 
      {
            "\t******************************************\n"
            "\t# ##### #     #   # #   #    ##### ##### #\n"
            "\t# #     #     #   #  # #     #   # #     #\n"
            "\t# ####  #     #   #   #      #   #  ###  #\n"
            "\t# #     #     #   #  # #     #   #     # #\n" 
            "\t# #     ##### ##### #   #    ##### ##### #\n"
            "\t******************************************"
      };
   
    printk("%s", logonMessage);
*/
    setcolor(COLOR_LIGHT_GREEN, COLOR_BLACK);
    printk("FLUX OS - A Hobby Operating System Kernel\n");
    printk("Copyrights (c) Dilhan Geeth. All Rights Reserved.\n");
    printk("\n");

    setcolor(COLOR_LIGHT_GREY, COLOR_BLACK);
}
