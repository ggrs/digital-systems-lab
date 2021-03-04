/*
	EA871 - Lab 05
	Gustavo Graziosi Silva	RA: 169320

*/

void init_GPIO_LCD(void);

void init_LCD(void);

void clear_LCD(void);

void putchar_LCD(unsigned char caracter);

void putcmd_LCD(unsigned char car_cmd, unsigned int tempo);

void puts_LCD(unsigned char *sequencia);

void setpos_LCD(unsigned int linha, unsigned int posicao);
