/*
 * File:   main.c
 * Author: Usuario Dell
 *
 * Created on 4 de marzo de 2021, 12:19 AM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "I2C.h"

//******************************************************************************
//  Palabra de cofiguración
//******************************************************************************
// CONFIG1
#pragma config FOSC = EXTRC_NOCLKOUT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//******************************************************************************
//  Variables
//******************************************************************************
#define _XTAL_FREQ 8000000
int seg = 00;
int min = 00;
int hora = 00;
int dia = 00;
int mes = 00;
int year = 00;
uint8_t segundos = 0;

//******************************************************************************
//  Prototipos de funciones
//******************************************************************************
void setup(void);
void set_tiempo (void);
uint8_t bcd_a_dec (uint8_t valor);
uint8_t dec_a_bcd (uint8_t valor);


//******************************************************************************
//  Ciclo principal
//******************************************************************************
void main(void) {
    setup();
    set_tiempo();
    while (1){
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0);
        I2C_Master_Stop();
        
        I2C_Master_Start();
        I2C_Master_Write(0xD1);
        seg = (I2C_Master_Read(1));
        I2C_Master_Read(1);
        I2C_Master_Stop();
        
        I2C_Master_Start();
        I2C_Master_Write(0xD1);
        I2C_Master_Write(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        segundos = bcd_a_dec(seg);
      
    }
}
uint8_t bcd_a_dec (uint8_t valor){
    return((valor >> 4)*10+(valor & 0x0F));
}

uint8_t dec_a_bcd (uint8_t valor){
    return(((valor/10) << 4) + (valor % 10));
}

void set_tiempo (void){
    I2C_Master_Start();
    I2C_Master_Write(0xD0);
    I2C_Master_Write(dec_a_bcd(seg));
    I2C_Master_Write(dec_a_bcd(min));
    I2C_Master_Write(dec_a_bcd(hora));
    I2C_Master_Write (1);
    I2C_Master_Write(dec_a_bcd(dia));
    I2C_Master_Write(dec_a_bcd(mes));
    I2C_Master_Write(dec_a_bcd(year));
    I2C_Master_Stop();
    __delay_ms(200);
}

//******************************************************************************
//  Cofiguración
//******************************************************************************
void setup(void) {
    ANSEL = 0;
    ANSELH = 0;
    
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}