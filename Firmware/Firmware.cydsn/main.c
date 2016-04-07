/*
*/
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
static const struct led_data led50 = {0x7, 0x2, {255/10, 255/10, 255/10}};

int main()
{
    CyGlobalIntEnable;

    
    SPI_Clock_Start();
    SPI_LED_Start();

    uint8_t count = 0;
    int len, j;
    uint8_t buf[100];
    for(;;) {
        
        //start frame
        SPI_LED_WriteByte(0x00);
        SPI_LED_WriteByte(0x00);
        SPI_LED_WriteByte(0x00);
        SPI_LED_WriteByte(0x00);
        
        //1 LED
        SPI_LED_PutArray((uint8_t*)&led50, sizeof(led50));
        
        //end frame
        SPI_LED_WriteByte(0xFF);
        SPI_LED_WriteByte(0xFF);
        SPI_LED_WriteByte(0xFF);
        SPI_LED_WriteByte(0xFF);
        
        
        /*
        //start frame
        j = 0;
        buf[j++] = 0x00;
        buf[j++] = 0x00;
        buf[j++] = 0x00;
        buf[j++] = 0x00;
        
        //6 LEDs of data (4 bytes)
        
        //stop frame
        buf[j++] = 0xFF;
        buf[j++] = 0xFF;
        buf[j++] = 0xFF;
        buf[j++] = 0xFF;
        
        //data len
        len = j - 1;
        
        //blast it out
        SPI_LED_PutArray(buf, len);
        */
        
        CyDelay(1000);
        count++;
    }
}

/* [] END OF FILE */
