#define GRAPHIC_DEVICE 0xB8000

unsigned int strlen(const char* message)
{
	unsigned int ret = 0;
	while(message[ret] != 0)
		ret++;

	return ret;

}


void kmain(void)
{

}
