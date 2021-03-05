#include <xc.h>
#include <pic16f887.h>
#include "USART.h"

void _baudios(void){
    SPBRG = 12; //9600 baudios para 8MHZ
}
//Configuracion dada en el datasheet
void config_txsta(void){
    TXSTAbits.CSRC = 0;//Clock terminal
    TXSTAbits.TX9 = 0;//8 bits de transmicion 
    TXSTAbits.TXEN = 1;//Transmicion habilitada
    TXSTAbits.SYNC = 0;//modo asincrono
    TXSTAbits.BRGH = 0;//low speed
    TXSTAbits.TRMT = 0;//Tsr full
    TXSTAbits.TX9D = 0;
}
//Configuracion dada en el datasheet
void config_rcsta(void){
    RCSTAbits.SPEN = 1;//Se habilita el puerto serial 
    RCSTAbits.RX9 = 0;
    RCSTAbits.SREN = 0;
    RCSTAbits.CREN = 1;//Recibir habilitadp 
    RCREG = 0;  
}
void Write_USART(uint8_t a){
    while(!TRMT);
    TXREG=a;
}
void Write_USART_String(char *a){
    uint8_t i;
    for(i=0;a[i]!='\0';i++){
        Write_USART(a[i]);
    }
}
uint8_t Read_USART(){
  while(!RCIF);
  return RCREG;
}

