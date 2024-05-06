#ifndef __INTERFACE_H
#define __INTERFACE_H

#include "main.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal_gpio.h"
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

// UI状态flag标记位
enum ui_state_t{
    kStatePiano, // 电子琴模式
    kStateRecording, // 录制模式
    kStatePlay, // 播放模式
    kStateNone
};

enum button_state_t{
    kNone,
    kBT1 = 1,
    kBT2 = 2,
    kBT3 = 3,
    kBT4 = 4,
    kBT5 = 5,
    kBT6 = 6,
    kBT7 = 7,
    kBT8 = 8,
    kMenu,
};

struct user_interface_t
{
    volatile enum ui_state_t f_state_ui;
} GUI;

uint8_t readPianoPin(void)
{
    if (!HAL_GPIO_ReadPin(BT_MENU_GPIO_Port, BT_MENU_Pin)) {
    return kMenu;
    }
    if (!HAL_GPIO_ReadPin(BT1_GPIO_Port, BT1_Pin)) {
    return kBT1;
    }
    if (!HAL_GPIO_ReadPin(BT2_GPIO_Port, BT2_Pin)) {
    return kBT2;
    }
    if (!HAL_GPIO_ReadPin(BT3_GPIO_Port, BT3_Pin)) {
    return kBT3;
    }
    if (!HAL_GPIO_ReadPin(BT4_GPIO_Port, BT4_Pin)) {
    return kBT4;
    }
    if (!HAL_GPIO_ReadPin(BT5_GPIO_Port, BT5_Pin)) {
    return kBT5;
    }
    if (!HAL_GPIO_ReadPin(BT6_GPIO_Port, BT6_Pin)) {
    return kBT6;
    }
    if (!HAL_GPIO_ReadPin(BT7_GPIO_Port, BT7_Pin)) {
    return kBT7;
    }
    if (!HAL_GPIO_ReadPin(BT8_GPIO_Port, BT8_Pin)) {
    return kBT8;
    }
    return kNone;
}

#ifdef __cplusplus
}
#endif

#endif /* __INTERFACE_H */