#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <array>
#include <switch.h>

#define SAVE_FILE_PATH "sdmc:/buttonbreak_highscore.txt"
#define MAX_LOCAL_PLAYERS 4

static const char* getButtonName(u64 kDown) {
    if (kDown & HidNpadButton_A) return "A Button";
    if (kDown & HidNpadButton_X) return "X Button";
    if (kDown & HidNpadButton_Y) return "Y Button";
    if (kDown & HidNpadButton_B) return "B Button";
    if (kDown & HidNpadButton_R) return "R Button";
    if (kDown & HidNpadButton_L) return "L Button";
    if (kDown & HidNpadButton_ZR) return "ZR Button";
    if (kDown & HidNpadButton_ZL) return "ZL Button";
    if (kDown & HidNpadButton_Up) return "D-Pad Up Button";
    if (kDown & HidNpadButton_Down) return "D-Pad Down Button";
    if (kDown & HidNpadButton_Left) return "D-Pad Left Button";
    if (kDown & HidNpadButton_Right) return "D-Pad Right Button";
    if (kDown & HidNpadButton_StickL) return "Left Stick Button";
    if (kDown & HidNpadButton_StickR) return "Right Stick Button";
    if (kDown & HidNpadButton_Minus) return "- Button";
    if (kDown & HidNpadButton_Plus) return "+ Button";
    if (kDown & HidNpadButton_LeftSL) return "Left SL Button";
    if (kDown & HidNpadButton_LeftSR) return "Left SR Button";
    if (kDown & HidNpadButton_RightSL) return "Right SL Button";
    if (kDown & HidNpadButton_RightSR) return "Right SR Button";
    if (kDown & HidNpadButton_HandheldLeftB) return "Handheld Left B Button";
    if (kDown & HidNpadButton_Palma) return "Poke Ball Plus Button";
    if (kDown & HidNpadButton_LagonCLeft) return "N64 C-Left Button";
    if (kDown & HidNpadButton_LagonCUp) return "N64 C-Up Button";
    if (kDown & HidNpadButton_LagonCRight) return "N64 C-Right Button";
    if (kDown & HidNpadButton_LagonCDown) return "N64 C-Down Button";
    return NULL;
}

static void printScoreLine(
    const std::array<int, MAX_LOCAL_PLAYERS>& localPresses,
    const std::array<bool, MAX_LOCAL_PLAYERS>& localConnected,
    int totalPresses)
{
    bool multiPlayer = localConnected.at(1) || localConnected.at(2) || localConnected.at(3);
    if (!multiPlayer) {
        printf("You mashed %d Buttons!\n", localPresses.at(0));
        return;
    }

    printf("P1: %d", localPresses.at(0));
    for (int i = 1; i < MAX_LOCAL_PLAYERS; i++) {
        if (localConnected.at(i)) {
            printf(" | P%d: %d", i + 1, localPresses.at(i));
        }
    }
    printf(" | Total: %d\n", totalPresses);
}

static void sendRumbleToPlayer(
    const PadState& pad,
    const std::array<HidVibrationDeviceHandle, 2>& handlesStandard,
    const std::array<HidVibrationDeviceHandle, 1>& handleJoyLeft,
    const std::array<HidVibrationDeviceHandle, 1>& handleJoyRight,
    const std::array<HidVibrationDeviceHandle, 2>& handlesHandheld,
    const std::array<HidVibrationValue, 2>& values,
    bool isPlayerOne)
{
    u32 style = padGetStyleSet(&pad);
    if (isPlayerOne && (style & HidNpadStyleTag_NpadHandheld)) {
        hidSendVibrationValues(handlesHandheld.data(), values.data(), 2);
    }
    else if (style & HidNpadStyleTag_NpadJoyLeft) {
        hidSendVibrationValues(handleJoyLeft.data(), values.data(), 1);
    }
    else if (style & HidNpadStyleTag_NpadJoyRight) {
        hidSendVibrationValues(handleJoyRight.data(), values.data(), 1);
    }
    else {
        hidSendVibrationValues(handlesStandard.data(), values.data(), 2);
    }
}

int main(int argc, char* argv[])
{
    consoleInit(NULL);

    padConfigureInput(MAX_LOCAL_PLAYERS,
        HidNpadStyleSet_NpadStandard |
        HidNpadStyleTag_NpadHandheld |
        HidNpadStyleTag_NpadJoyLeft |
        HidNpadStyleTag_NpadJoyRight);

    std::array<PadState, MAX_LOCAL_PLAYERS> pads;
    padInitialize(&pads.at(0), HidNpadIdType_No1, HidNpadIdType_Handheld);
    padInitialize(&pads.at(1), HidNpadIdType_No2);
    padInitialize(&pads.at(2), HidNpadIdType_No3);
    padInitialize(&pads.at(3), HidNpadIdType_No4);

    std::array<int, MAX_LOCAL_PLAYERS> localPresses = { 0, 0, 0, 0 };
    std::array<bool, MAX_LOCAL_PLAYERS> wasLocalConnected = { true, false, false, false };
    int highScore = 0;

    FILE* readFile = fopen(SAVE_FILE_PATH, "r");
    if (readFile != NULL) {
        fscanf(readFile, "%d", &highScore);
        fclose(readFile);
    }

    std::array<const char*, 7> bgColors = {
        "\x1b[41m\x1b[37m",
        "\x1b[42m\x1b[37m",
        "\x1b[44m\x1b[37m",
        "\x1b[46m\x1b[37m",
        "\x1b[45m\x1b[37m",
        "\x1b[48;5;208m\x1b[37m",
        "\x1b[43m\x1b[37m"
    };
    int currentColorIndex = 0;

    std::array<std::array<HidVibrationDeviceHandle, 2>, MAX_LOCAL_PLAYERS> rumbleStandard;
    std::array<std::array<HidVibrationDeviceHandle, 1>, MAX_LOCAL_PLAYERS> rumbleJoyLeft;
    std::array<std::array<HidVibrationDeviceHandle, 1>, MAX_LOCAL_PLAYERS> rumbleJoyRight;
    std::array<HidVibrationDeviceHandle, 2> rumbleHandheld;

    for (int i = 0; i < MAX_LOCAL_PLAYERS; i++) {
        HidNpadIdType npadId = (HidNpadIdType)(HidNpadIdType_No1 + i);
        hidInitializeVibrationDevices(rumbleStandard.at(i).data(), 2, npadId, HidNpadStyleSet_NpadStandard);
        hidInitializeVibrationDevices(rumbleJoyLeft.at(i).data(), 1, npadId, HidNpadStyleTag_NpadJoyLeft);
        hidInitializeVibrationDevices(rumbleJoyRight.at(i).data(), 1, npadId, HidNpadStyleTag_NpadJoyRight);
    }
    hidInitializeVibrationDevices(rumbleHandheld.data(), 2, HidNpadIdType_Handheld, HidNpadStyleTag_NpadHandheld);

    HidVibrationValue rumbleValueOn;
    rumbleValueOn.freq_low = 160.0f;
    rumbleValueOn.amp_low = 0.35f;
    rumbleValueOn.freq_high = 320.0f;
    rumbleValueOn.amp_high = 0.35f;

    std::array<HidVibrationValue, 2> rumbleValuesOn = { rumbleValueOn, rumbleValueOn };
    std::array<HidVibrationValue, 2> rumbleValuesOff = {};
    memset(rumbleValuesOff.data(), 0, sizeof(rumbleValuesOff));

    printf("ButtonBreakSwitch (v0.8.0)\n");
    printf("Created by StrongEnough\n");
    printf("----------------------------\n");
    printf("High Score: %d\n", highScore);
    printf("----------------------------\n");
    printf("Hold + and - together to exit.\n");
    printf("Hold ZL and R together to pair/split controllers.\n");

    while (appletMainLoop())
    {
        bool anyPressed = false;
        bool shouldExit = false;
        std::array<bool, MAX_LOCAL_PLAYERS> localConnected = { true, false, false, false };

        for (int i = 0; i < MAX_LOCAL_PLAYERS; i++) {
            padUpdate(&pads.at(i));
            u64 kDown = padGetButtonsDown(&pads.at(i));
            u64 kHeld = padGetButtons(&pads.at(i));

            if (i > 0) {
                bool connected = padIsConnected(&pads.at(i));
                localConnected.at(i) = connected;
                if (connected && !wasLocalConnected.at(i)) {
                    wasLocalConnected.at(i) = true;
                    printf("--------------------------------\nPlayer %d connected!\n", i + 1);
                }
                else if (!connected && wasLocalConnected.at(i)) {
                    wasLocalConnected.at(i) = false;
                    localPresses.at(i) = 0;
                    printf("--------------------------------\nPlayer %d disconnected.\n", i + 1);
                }
            }

            if ((kHeld & (HidNpadButton_Plus | HidNpadButton_Minus)) == (HidNpadButton_Plus | HidNpadButton_Minus)) {
                shouldExit = true;
                break;
            }

            if ((kHeld & (HidNpadButton_ZL | HidNpadButton_R)) == (HidNpadButton_ZL | HidNpadButton_R)) {
                HidLaControllerSupportArg arg;
                hidLaCreateControllerSupportArg(&arg);
                arg.hdr.player_count_min = 1;
                arg.hdr.player_count_max = MAX_LOCAL_PLAYERS;
                arg.hdr.enable_single_mode = false;
                arg.hdr.enable_permit_joy_dual = true;
                hidLaShowControllerSupportForSystem(NULL, &arg, false);
            }

            const char* btnName = getButtonName(kDown);
            if (btnName != NULL) {
                localConnected.at(i) = true;
                localPresses.at(i)++;
                anyPressed = true;
                printf("--------------------------------\n[P%d] You pressed the %s!\n", i + 1, btnName);
            }
        }

        if (shouldExit)
            break;

        if (anyPressed) {
            int totalPresses = 0;
            for (int i = 0; i < MAX_LOCAL_PLAYERS; i++) {
                totalPresses += localPresses.at(i);
            }

            printScoreLine(localPresses, localConnected, totalPresses);

            if (totalPresses > highScore) {
                highScore = totalPresses;
                FILE* writeFile = fopen(SAVE_FILE_PATH, "w");
                if (writeFile != NULL) {
                    fprintf(writeFile, "%d", highScore);
                    fclose(writeFile);
                }
            }

            if (totalPresses % 100 == 0) {
                printf("%s\x1b[2J\x1b[H", bgColors.at(currentColorIndex));
                currentColorIndex = (currentColorIndex + 1) % (int)bgColors.size();

                printf("Hold + and - together to exit. | Hold ZL + R for Controllers.\n");
                printf("--------------------------------\n");
                printf("High Score: %d\n", highScore);
                printf("--------------------------------\n");
                printScoreLine(localPresses, localConnected, totalPresses);
                consoleUpdate(NULL);

                for (int i = 0; i < MAX_LOCAL_PLAYERS; i++) {
                    if (localConnected.at(i)) {
                        sendRumbleToPlayer(
                            pads.at(i),
                            rumbleStandard.at(i),
                            rumbleJoyLeft.at(i),
                            rumbleJoyRight.at(i),
                            rumbleHandheld,
                            rumbleValuesOn,
                            i == 0);
                    }
                }
                svcSleepThread(120000000ULL);
                for (int i = 0; i < MAX_LOCAL_PLAYERS; i++) {
                    if (localConnected.at(i)) {
                        sendRumbleToPlayer(
                            pads.at(i),
                            rumbleStandard.at(i),
                            rumbleJoyLeft.at(i),
                            rumbleJoyRight.at(i),
                            rumbleHandheld,
                            rumbleValuesOff,
                            i == 0);
                    }
                }
            }
        }
        consoleUpdate(NULL);
    }
    for (int i = 0; i < MAX_LOCAL_PLAYERS; i++) {
        hidSendVibrationValues(rumbleStandard.at(i).data(), rumbleValuesOff.data(), 2);
        hidSendVibrationValues(rumbleJoyLeft.at(i).data(), rumbleValuesOff.data(), 1);
        hidSendVibrationValues(rumbleJoyRight.at(i).data(), rumbleValuesOff.data(), 1);
    }
    hidSendVibrationValues(rumbleHandheld.data(), rumbleValuesOff.data(), 2);
    printf("\x1b[0m\x1b[2J\x1b[H");
    consoleUpdate(NULL);
    consoleExit(NULL);
    return 0;
}