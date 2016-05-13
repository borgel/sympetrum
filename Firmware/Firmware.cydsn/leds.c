/*
*/
/*
Notes on dotstars:
https://cpldcpu.wordpress.com/2014/08/27/apa102/
people have seen global brightness below (30?) cause flickering, so ignore it
someone pointed out endframe is no different looking then a full bright LED (which
sort of matches behavior I've seen....). So omit that too.

*/
#include "leds.h"
#include <project.h>

#include <stdint.h>

static const uint32_t LED_START_FRAME        = 0x00000000;
static const uint32_t LED_END_FRAME          = 0xFFFFFFFF;
static const uint8_t LED_PACKET_HEADER       = 0x7;     //0b111
struct led_PackedColor {
    uint8_t     b;
    uint8_t     g;
    uint8_t     r;
}__attribute__((packed));

struct led_data {
    uint8_t     header  :3;
    uint8_t     global  :5;
    struct led_PackedColor color;
}__attribute__((packed));


//0x7 is a constant
//hdr, 5 bit global brightness divisor, each channel
//0x1F is max current
static const struct led_data led50 = {0x7, 0x1F/2, {255/10, 255/10, 255/10}};
static const struct led_data led0 = {0x7, 0x0, {0, 0, 0}};


void led_Do(void) {

    
    /*
    //FIXME rm... to let other devices drive
    DotStar_MOSI_SetDriveMode(DotStar_MOSI_DM_DIG_HIZ);
    DotStar_SCK_SetDriveMode(DotStar_SCK_DM_DIG_HIZ);
    while(1){};
    */

    SPI_LED_Start();

    
    
    uint8_t count = 0;
    int len, j, i;
    uint8_t buf[100];
    
    /*
    //start frame
    buf[0] = buf[1] = buf[2] = buf[3] = 0x00;
    
    //an LED
    buf[4] = 0xFF;
    buf[5] = 0x00;
    buf[6] = 0xAA;
    buf[7] = 0x00;
    
    
    buf[ 8] = 0xFF;
    buf[ 9] = 0x00;
    buf[10] = 0xAA;
    buf[11] = 0x00;
    
    //end frame
    buf[12] = buf[13] = buf[14] = buf[15] = 0xFF;
    
    SPI_LED_PutArray(buf, 16);
    */
    
    //LED_PACKET_HEADER = 0x7
    struct led_data wat = {0x7, 31, {0, 5, 0}};
            
    /*
    SPI_LED_WriteByte(0x70);    //3 1's + full global brightness    0xFF for full bright. 0x70 for no bright
    SPI_LED_WriteByte(0x00);    //b
    SPI_LED_WriteByte(0x00);    //g
    SPI_LED_WriteByte(0x00);    //r
    */
    
    for(;;) {
        //wat.color.g = count;
        
        //start frame
        SPI_LED_SpiUartWriteTxData(0x00);
        SPI_LED_SpiUartWriteTxData(0x00);
        SPI_LED_SpiUartWriteTxData(0x00);
        SPI_LED_SpiUartWriteTxData(0x00);
        
        for(i = 0; i < 4; i++) {
            //1 LED
            //SPI_LED_SpiUartPutArray((uint8_t*)&wat, sizeof(wat));
            
            
            SPI_LED_SpiUartWriteTxData(0xFF);    //3 1's + full global brightness    0xFF for full bright. 0x70 for no bright
            SPI_LED_SpiUartWriteTxData(count/2);    //b
            SPI_LED_SpiUartWriteTxData(count/2);    //g
            SPI_LED_SpiUartWriteTxData(count/2);    //r
            
        }
        
        SPI_LED_SpiUartWriteTxData(0xFF);
        SPI_LED_SpiUartWriteTxData(0xFF);
        SPI_LED_SpiUartWriteTxData(0xFF);
        SPI_LED_SpiUartWriteTxData(0xFF);
        
        
        //CyDelay(200);
        CyDelay(10);
        count+=1;
    }

}
