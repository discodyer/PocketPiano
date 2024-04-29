#include "buzzer.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
#include <stdint.h>

// 开启蜂鸣器
void BUZZER_Start(void) { HAL_TIM_PWM_Start(BUZZER_TIM, BUZZER_TIM_CHANNEL); }

// 停止蜂鸣器
void BUZZER_Stop(void) { HAL_TIM_PWM_Stop(BUZZER_TIM, BUZZER_TIM_CHANNEL); }

// 蜂鸣器淡出效果
void BUZZER_Fadeout(void) {
  // 记录当前音量
  uint16_t loudness = __HAL_TIM_GET_COMPARE(BUZZER_TIM, BUZZER_TIM_CHANNEL);
  BUZZER_SetLoudness(100);
  while (__HAL_TIM_GET_COMPARE(BUZZER_TIM, BUZZER_TIM_CHANNEL) > 1) {
    HAL_Delay(5); // 延迟5ms
    // 每次循环减少一点音量
    BUZZER_SetLoudness(__HAL_TIM_GET_COMPARE(BUZZER_TIM, BUZZER_TIM_CHANNEL) -
                       10);
  }
  // 关闭输出
  BUZZER_Stop();
  // 重置音量，不影响下个音符
  BUZZER_SetLoudness(loudness);
}

// 设置蜂鸣器音符
void BUZZER_SetNote(uint32_t note) {
  __HAL_TIM_SET_PRESCALER(BUZZER_TIM, note);
}

// 设置蜂鸣器音量，范围 1~999
void BUZZER_SetLoudness(uint16_t loudness) {
  if (loudness > 999) {
    loudness = 999;
  } else if (loudness < 1) {
    loudness = 1;
  }
  __HAL_TIM_SetCompare(BUZZER_TIM, BUZZER_TIM_CHANNEL, loudness);
}

// 演奏音符
void BUZZER_PlayNote(uint32_t note) {
  //   BUZZER_Stop();
  if (note == 0) { // 乐谱中的 Z0 音符是停止符
    BUZZER_Stop();
    // BUZZER_Fadeout();
    return;
  }
  BUZZER_SetNote(note);
  BUZZER_Start();
}
