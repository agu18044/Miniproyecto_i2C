/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef USART_H
#define	USART_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

void _baudios(void);
void config_txsta(void);
void config_rcsta(void);

void Write_USART(uint8_t a);
void Write_USART_String(char *a);
uint8_t Read_USART(void);

      

#endif	/* XC_HEADER_TEMPLATE_H */