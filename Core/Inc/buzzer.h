#ifndef __BUZZER_H
#define __BUZZER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tim.h"

#define BUZZER_TIM (&htim1)
#define BUZZER_TIM_CHANNEL TIM_CHANNEL_1

void BUZZER_Start(void);
void BUZZER_Stop(void);
void BUZZER_SetNote(uint32_t note);
void BUZZER_SetLoudness(uint16_t loudness);
void BUZZER_PlayNote(uint32_t note);

#ifdef __cplusplus
}
#endif

#endif /* __BUZZER_H */