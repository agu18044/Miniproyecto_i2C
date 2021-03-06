/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LCD_H
#define	LCD_H   

#ifndef RS
#define RS RE0
#endif 

#ifndef EN
#define EN RE1
#endif 

#ifndef D0
#define D0 RD0 
#endif 
#ifndef D1
#define D1 RD1 
#endif 
#ifndef D2
#define D2 RD2 
#endif 
#ifndef D3
#define D3 RD3 
#endif 
#ifndef D4
#define D4 RD4 
#endif 
#ifndef D5
#define D5 RD5 
#endif 
#ifndef D6
#define D6 RD6 
#endif 
#ifndef D7
#define D7 RD7 
#endif

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

void port(uint8_t a);
void CMD(uint8_t a);
void LCD_clear(void);
void set_cursor (uint8_t a, uint8_t b);
void LCD_init(void);
void write_char(char a);
void write_string(char *a);

#endif	/* XC_HEADER_TEMPLATE_H */