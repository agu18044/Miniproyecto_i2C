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
#include "USART.h"
#include "LCD.h"

//******************************************************************************
//  Palabra de cofiguración
//******************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT         // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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
// variables para ingresar valores al reloj ////////////
int seg = 00;
int min = 00;
int hora = 00;
int dia = 00;
int mes = 00;
int year = 00;
///////////////////////////////////////////////////////
uint8_t segundos = 0;
char lecUSART = 0;
char BUFFER[20];

//******************************************************************************
//  Prototipos de funciones
//******************************************************************************
void setup(void);
void set_tiempo (void);
void lec_tiempo (void);
uint8_t bcd_a_dec (uint8_t valor);
uint8_t dec_a_bcd (uint8_t valor);
void sep_char_LCD (void);

//******************************************************************************
// Interupciones 
//******************************************************************************
void __interrupt() ISR(){
   if (RCIF == 1){
       RCIF = 0;
       lecUSART = Read_USART();
       if (lecUSART == '+'){
           PORTBbits.RB0 = 1;
       }
       else if (lecUSART == '-'){
           PORTBbits.RB0 = 0;
       }
       if (lecUSART == '*'){
           PORTBbits.RB1 = 1;
       }
       else if (lecUSART == '/'){
           PORTBbits.RB1 = 0;
       }
   } 
}

//******************************************************************************
//  Ciclo principal
//******************************************************************************
void main(void) {
    setup();
    LCD_init();         //configuración para la pantalla LCD
    LCD_clear();
    _baudios();         // configuración para comunicación UART
    config_txsta();
    config_rcsta();
    set_tiempo();       //ingresar valores iniciales al reloj
    while (1){
        lec_tiempo();
                              
        sep_char_LCD();         
        
        sprintf(BUFFER, "%d", seg); 
        Write_USART_String(BUFFER);  //enviar por UART el dato de segundos como un string
        Write_USART(13);
        Write_USART(10);
        
        __delay_ms(500); 
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
    I2C_Master_Write(0);
    I2C_Master_Write(dec_a_bcd(seg));
    I2C_Master_Write(dec_a_bcd(min));
    I2C_Master_Write(dec_a_bcd(hora));
    I2C_Master_Write (1);
    I2C_Master_Write(dec_a_bcd(dia));
    I2C_Master_Write(dec_a_bcd(mes));
    I2C_Master_Write(dec_a_bcd(year));
    I2C_Master_Stop();
    //__delay_ms(200);
}

void lec_tiempo (void){
      I2C_Master_Start();
      I2C_Master_Write(0xD0);
      I2C_Master_Write(0);
      I2C_Master_Stop();
      
      I2C_Master_Start();
      I2C_Master_Write(0xD1);
      seg = bcd_a_dec(I2C_Master_Read(1));
      min = bcd_a_dec(I2C_Master_Read(1));
      hora = bcd_a_dec(I2C_Master_Read(1));
      I2C_Master_Read(1);
      dia = bcd_a_dec(I2C_Master_Read(1));
      mes = bcd_a_dec(I2C_Master_Read(1));
      year = bcd_a_dec(I2C_Master_Read(1));
      I2C_Master_Stop();
      
      I2C_Master_Start();
      I2C_Master_Write(0xD1);
      I2C_Master_Read(1);
      I2C_Master_Stop();
      __delay_ms(200); 
       
}

void sep_char_LCD (void){
    char seg_0 = seg%10;
    char seg_1 = (seg/10);
    char min_0 = min%10;
    char min_1 = (min/10);
    char hora_0 = hora%10;
    char hora_1 = (hora/10);
    
    set_cursor(1, 1);
    write_string("TIEMPO ");
    write_char(hora_1 + '0');
    write_char(hora_0 + '0');
    write_char(':');
    write_char(min_1 + '0');
    write_char(min_0 + '0');
    write_char(':');
    write_char(seg_1 + '0');
    write_char(seg_0 + '0');
}

//******************************************************************************
//  Cofiguración
//******************************************************************************
void setup(void) {
    ANSEL = 0;
    ANSELH = 0;
    TRISD = 0;
    PORTD = 0;
    TRISB = 0;
    PORTB = 0;
    TRISE = 0;
    PORTE = 0;
    IRCF0 = 1;
    IRCF1 = 1;
    IRCF2 = 1;

    INTCONbits.PEIE = 1;
    PIE1bits.RCIE = 1;
    PIR1bits.RCIF = 0;
    INTCONbits.GIE = 1;

    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}