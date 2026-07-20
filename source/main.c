
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <switch.h>


int main(int argc, char* argv[])
{
    
    consoleInit(NULL);

    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    PadState pad;
    padInitializeDefault(&pad);

    printf("ButtonBreakSwitch v0.1\n");
    printf("----------------------------\n");
    printf("Press the Plus button to exit.\n");


    while (appletMainLoop())
    {
        padUpdate(&pad);

        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus)
            break;
        
        padUpdate(&pad);

        if (kDown & HidNpadButton_A)
            printf("--------------------------\n");
            
        padUpdate(&pad);

        if (kDown & HidNpadButton_A)
        printf("A Button Pressed.\n");
        
        padUpdate(&pad);

        if (kDown & HidNpadButton_X)
            printf("--------------------------\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_X)
            printf("X Button Pressed.\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_Y)
            printf("--------------------------\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_Y)
            printf("Y Button Pressed.\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_B)
            printf("--------------------------\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_B)
            printf("B Button Pressed.\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_R)
            printf("--------------------------\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_R)
            printf("R Button Pressed.\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_L)
            printf("--------------------------\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_L)
            printf("L Button Pressed.\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_ZR)
            printf("--------------------------\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_ZR)
            printf("ZR Trigger Pressed.\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_ZL)
            printf("--------------------------\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_ZL)
            printf("ZL Trigger Pressed.\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_Up)
            printf("--------------------------\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_Up)
            printf("D-Pad Up Button Pressed.\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_Down)
            printf("--------------------------\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_Down)
            printf("D-Pad Down Button Pressed.\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_Left)
            printf("--------------------------\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_Left)
            printf("D-Pad Left Button Pressed.\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_Right)
            printf("--------------------------\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_Right)
            printf("D-Pad Right Button Pressed.\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_StickL)
            printf("--------------------------\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_StickL)
            printf("Left Stick Button Pressed.\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_StickR)
            printf("--------------------------\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_StickR)
            printf("Right Stick Button Pressed.\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_Minus)
            printf("--------------------------\n");

        padUpdate(&pad);

        if (kDown & HidNpadButton_Minus)
            printf("Minus Button Pressed.\n");

        consoleUpdate(NULL);
    }
    consoleExit(NULL);
    return 0;
}
