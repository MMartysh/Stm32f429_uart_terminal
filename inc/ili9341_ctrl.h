#ifndef ILI9341_CTRL_H
#define ILI9341_CTRL_H

typedef struct txtinfo 
{
	const uint8_t *font;
	int f_hight;
	int f_size;
	int f_bgcolor;
	int f_fgcolor;
	int f_line;
	int f_column;
}txtinfo;
void ili9341Init(void);
#endif
