#include <project.h>

int main()
{
    CyGlobalIntEnable;
    
    // turn off user LED
    UserLED_Write(0);
    
    /*
    //TODO if button pressed, do bootloader flow
    //TODO make it wait forever (or long time)
    if(UserButton_Read() == 0) {
        //start the bootloader flow
        UserLED_Write(1);
        Bootloader_1_Start();
    }
    else {
        UserLED_Write(0);
        //jump directly into application
        Bootloader_1_Exit(Bootloader_1_EXIT_TO_BTLDB);
    }
    */
    
    //start the bootloader flow
    Bootloader_1_Start();
    
    for(;;)
    {
    }
}

