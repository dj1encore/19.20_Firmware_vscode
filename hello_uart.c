/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include"math.h"
#include"ModbusRtu.h"
/// \tag::hello_uart[]a

#define UART_ID uart0
#define BAUD_RATE 115200

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

/**************************************************************************/
/* 
Modbus RTU query of total 8 bytes send to slave with 
address    0x01
funcation  0x03
Register Address 2 bytes 0x01, 0x00
Offset address 2 bytes   0x00, 0x3C
CRC 2 bytes  0x41, 0x1B
You can download  excel sheet " for all calculation of CRC" We put first 6 bytes in it and it 
calcualtes CRC https://www.simplymodbus.ca/crc.xls
*/

char modbus[8]={0x01, 0x03, 0x10,0x00,0x00,0x3C,0x41,0x1B}; 


/**************************************************************************/

char buffer[133];//125
int b=0;
int sign;
int c=0;
int d=0;
int masking=0x7F;		// 1111111
int masking2=0x80;	//10000000
int masking3=0x7FFFFF;
int masking4=0x0080; 
int rightshift_0=0,rightshift_8=0,rightshift_16=0,append_all=0;
float f=0;
float g;
float powerpameters[120]; 

int main() {
    // Set up our UART with the required speed.
 stdio_init_all();
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Use some the various UART functions to send out data
    // In a default system, printf will also output via the default UART

while(1)
/**********************************************************************************/

{
  /****************** Only for debugging and Testing purpose**********************/
    // Send out a character without any conversions
  //  uart_putc_raw(UART_ID, 'A');  // Only
//sleep_ms(1000);
    // Send out a character but do CR/LF conversions
  //  uart_putc(UART_ID, 'B');
//sleep_ms(1000);
    // Send out a string, with CR/LF conversions
   // uart_puts(UART_ID, " Hello, UART!\n");
   // uart_write_blocking(UART_ID, &modbus, 8);
  /****************** Only for debugging and Testing purpose**********************/

/************************************************************/
// Pico pi will send query to server @ interval of 5 sec.
sleep_ms(5000);
//for(int l=0;l<8;l++)
//{uart_putc(UART_ID, modbus[l]);}
read_register_f(0x01,0x10);

}
/************************************************************/
/* Function used to receive values from UART */
/************************************************************

{

while(uart_is_readable(UART_ID))
{
			uart_read_blocking (UART_ID,&buffer,133);
      calculations();
        
        //printf("%c\n",ch); // only for debuging purpose
}       // while uart
   
        //   for(int m=0;m<15;m++)    //// only for debuging purpose
        //  {printf("%c\n",ch[m]); }   //// only for debuging purpose
        //printf("No received\n");       //// only for debuging purpose
        //sleep_ms(500);                 //// only for debuging purpose
} // while (1)

/************************************************************/
 
}       //main
        /// \end::hello_uart[]


/**************************************************************
/ In modbus received daata consist of 4 bytes, Received data is in "IEEE 754
32 bit floating point representation, To Extract desired value we have to perform "Hex to float conversion and coding
of this conversion is given below. 
******************************************************************

void calculations(void)
{ //calculations start
//printf("calculation start\n"); 
  for( int ploop=10;ploop<40;ploop++)
{
b  =  (buffer[ploop]  & (masking4)); // either 1 or 0 
b=(b>>7);
if(b==1)
{sign=-1;} else {sign=1;}					 
c  =((buffer[ploop]  & (masking)));//   1111111 =7
d=  ((buffer[ploop+1]& (masking2))); // 10000000		
c=(((c<<1)|(d>>7))-127);
int e= buffer[ploop+1]<<1;
rightshift_0  = buffer[ploop+3] & (0x0000FF);
rightshift_8  = (buffer[ploop+2]<<8) & (0x00FF00);				  
rightshift_16  = (buffer[ploop+1]<<16)& (0x7FFFFF);  				 
 append_all=rightshift_0|rightshift_8|rightshift_16;
f= (append_all)&(masking3);  //16
float g=(f/8388608)+1; 	//1.732	
powerpameters[ploop]= (sign)* g * (pow(2,c));
}
printf("Voltage is::\n");
printf("%f",powerpameters[35]);printf("\n"); // To print only value of voltage
/**********************************************************************/
 // If we want to display multiple values
 //for(int d=35;d<40;d++)
   //  {printf("%f",powerpameters[d]);}
//printf("**********************\n");     
/***********************************************************************
} //calculations end
/********************************************************************/