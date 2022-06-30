/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause


   GPIO 16 (pin 21) MISO/spi0_rx-> SDO/SDO on MAx7219 board
   GPIO 17 (pin 22) Chip select -> CSB/!CS on MAx7219 board
   GPIO 18 (pin 24) SCK/spi0_sclk -> SCL/SCK on MAx7219 board
   GPIO 19 (pin 25) MOSI/spi0_tx -> SDA/SDI on MAx7219 board
   VBUS -> VCC on MAx7219 board
   GND (pin 38)  -> GND on MAx7219 board
 */
// gp28 increment didigt in eeprom and display on seven segment.
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

#define PIN_MISO 4
#define PIN_CS   13
#define PIN_SCK  10
#define PIN_MOSI 11

#define SPI_PORT spi1

//#ifdef PICO_DEFAULT_SPI_CSN_PIN
static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PIN_CS, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PIN_CS, 1);
    asm volatile("nop \n nop \n nop");
}


//#if defined(spi_default) && defined(PICO_DEFAULT_SPI_CSN_PIN)
static void write_register(uint8_t reg, uint8_t data) {
    uint8_t buf[2];
    buf[0] = reg & 0x7f;  // remove read bit as this is a write
    buf[1] = data;
    cs_select();
    spi_write_blocking(SPI_PORT, buf, 2);
    cs_deselect();
    sleep_ms(10);
}


int32_t num=0x17;
int main() {
    stdio_init_all();
//#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(PICO_DEFAULT_SPI_CSN_PIN)
//#warning spi/bme280_spi example requires a board with SPI pins
//    puts("Default SPI pins were not defined");//
//#else

   

    // This example will use SPI0 at 0.5MHz.
    spi_init(SPI_PORT, 500 * 1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    // Make the SPI pins available to picotool
   // bi_decl(bi_3pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI));
    bi_decl(bi_3pins_with_func(PIN_MISO, PIN_MOSI, PIN_SCK, GPIO_FUNC_SPI));
    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    // Make the CS pin available to picotool
    //bi_decl(bi_1pin_with_name(PICO_DEFAULT_SPI_CSN_PIN, "SPI CS"));
    bi_decl(bi_1pin_with_name(PIN_CS, "SPI CS"));
/***************************************************************************************/
/*************************************MAX 7219 settings*********************************/
        
        write_register(0x0c,1); // normal mode
         sleep_ms(1000);
        write_register(0x0b ,0x01); // SCan 1 digits , We can use 0x00 to 0x07 depends on the number of numbers we want to use.
        sleep_ms(1000);
        write_register(0x09 ,0xFF); // Deode mode all
        sleep_ms(1000);
        write_register(0x0f,1);  // test display ON
        sleep_ms(1000);
        write_register(0x0f,0);  // test display OFF
        sleep_ms(1000);
        write_register(0x0a ,0x05); // brightness level 1
        sleep_ms(1000);
/***************************************************************************************/
    while (1) {
/*************************************For loop to dipslay from 9 to 0 ******************      
    for (int abc=9;abc>0;abc--)
    {
         write_register(0x01 ,abc); // 
        sleep_ms(1000);
    }
/***************************************************************************************/    

        write_register(0x01 ,0x00); // 
        write_register(0x02 ,0x00); // 
        sleep_ms(1000);
       write_register(0x01 ,0x01); // 
        sleep_ms(1000);
         write_register(0x01 ,0x02); // 
        sleep_ms(1000);
         write_register(0x01 ,0x03); // 
        sleep_ms(1000);
         write_register(0x01 ,0x04); // 
        sleep_ms(1000);
        write_register(0x01 ,0x05); // 
        sleep_ms(1000);
        write_register(0x01 ,0x06); // 
        sleep_ms(1000);
        write_register(0x01 ,0x07); // 
        sleep_ms(1000);
        write_register(0x01 ,0x08); // 
        sleep_ms(1000);
        write_register(0x01 ,0x09); // 
        sleep_ms(1000);
        write_register(0x02 ,0x01); //
        write_register(0x01 ,0x00); //  
        sleep_ms(1000);
        
    }

    return 0;

}
