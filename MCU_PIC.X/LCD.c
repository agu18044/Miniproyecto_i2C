#include <xc.h>
#include <stdint.h>
#include "LCD.h"
#define _XTAL_FREQ 8000000


void port(uint8_t a){
    if(a & 1){
        D0=1;}
    else{
        D0=0;}
    if(a & 2){
        D1=1;}
    else{
        D1=0;}
    if(a & 4){
        D2=1;}
    else{
        D2=0;}
    if(a & 8){
        D3=1;}
    else{
        D3=0;}
    if(a & 16){
        D4=1;}
    else{
        D4=0;}
    if(a & 32){
        D5=1;}
    else{
        D5=0;}
    if(a & 64){
        D6=1;}
    else{
        D6=0;}
    if(a & 128){
        D7=1;}
    else{
        D7=0;}
}

void CMDs(uint8_t a){
    RS = 0;
    port(a);
    EN = 1;
    __delay_us(5);
    EN = 0;
    __delay_us(5);
    __delay_ms(2);
}

void LCD_clear(void){
    CMDs(0);
    CMDs(1);
}

void set_cursor (uint8_t a, uint8_t b){
    uint8_t x;
    if (a == 1){
       x = 0x80 + b;
       CMDs(x); 
    }
    else if (a == 2){
        x = 0xC0 + b;
        CMDs(x);
    }
}

void LCD_init(void){
    __delay_ms(20);
    CMDs (0x30);
    __delay_ms(5);
    CMDs (0x30);
    __delay_us(100);
    CMDs (0x30);
    __delay_us(100);
    CMDs (0x38);
    __delay_us(60);
    CMDs (0x08);
    __delay_us(60);
    CMDs (0x01);
    __delay_ms(5);
    CMDs (0x06);
    __delay_us(60);
    CMDs (0x0C);     
    __delay_us(60);
}

void write_char(char a){
    RS = 1;
    port(a);
    EN = 1;
    __delay_us(5);
    EN = 0;
    __delay_us(5);
    __delay_us(50);
}

void write_string(char *a){
    int i;
	for(i=0;a[i]!='\0';i++)
	write_char(a[i]);
}
