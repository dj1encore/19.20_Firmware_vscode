#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include"math.h"
//char CRC_Lower;
//char CRC_Upper;
// Definitions private to the module
#define CRC16		0xA001					// Used in CRC calculation
static unsigned char crc_table_1[256];		// Table for CRC1 calculation
static unsigned char crc_table_2[256];		// Table for CRC2 calculation
char modbus_querry[8]={};
char abc[2]={0x02,0x02};
//char modbus[8]={0x01, 0x03, 0x10,0x00,0x00,0x3C,0x41,0x1B}; 
char abcmodbus[6]={0x01, 0x03, 0x10,0x00,0x00,0x3C}; 
void read_register_f(char address, char register_address)
{
//schar address= 0x01;
char function=0x03;
char offset=0x00;
//ModRTU_CRC(abcmodbus,6);
modbus_querry[0]=address;
modbus_querry[1]=function;
modbus_querry[2]=register_address;
modbus_querry[3]=offset;
modbus_querry[4]=offset;
modbus_querry[5]=0x3C;
modbus_querry[6]=0x41;
modbus_querry[7]=0x1B;
for(int l=0;l<8;l++)
{uart_putc(uart0, modbus_querry[l]);}

}



/**********************************************************************/

void ModRTU_CRC(unsigned char * buf, int len)
{
  int crc = 0xFFFF;
  
  for (int pos = 0; pos < len; pos++) {
    crc ^= (int)buf[pos];          // XOR byte into least sig. byte of crc
  
    for (int i = 8; i != 0; i--) {    // Loop over each bit
      if ((crc & 0x0001) != 0) {      // If the LSB is set
        crc >>= 1;                    // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
 uart_putc(uart0, crc);
  //uart_putc(uart0, crc<<8);
  uart_putc(uart0, crc>>8);
  return crc;  
}