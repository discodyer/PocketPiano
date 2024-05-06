/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "i2c.h"
#include "ssd1306_font.h"
#include "stm32f1xx_hal.h"
#include "tim.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "buzzer.h"
#include "interface.h"
#include "note.h"
#include "ssd1306.h"
#include <stdint.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t record_note_sheet[256] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Piano_loop(void);
void MusicPlayer_loop(void);
void Record_loop(void);
void RecordPlayer_loop(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick.
   */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  BUZZER_SetLoudness(10); // 设置音量 1~999
  // Solitary_brave();

  ssd1306_init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    Piano_loop();        // 电子琴模式
    MusicPlayer_loop();  // 音乐模式
    Record_loop();       // 录制模式
    RecordPlayer_loop(); // 播放录制模式
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

void Piano_loop(void) {
  enum button_state_t bt_state_last = kNone; // 记录上一次的按钮状态
  uint32_t button_menu_tick = 0; // 记录上一次的menu按钮按下的时间
  enum note_offset_t note_offset = kOffsetMiddle; // 音符偏移，默认中音

  ssd1306_set_cursor(32, 0);
  ssd1306_write_char_cn(fontCN16x16, 0); // 钢
  ssd1306_write_char_cn(fontCN16x16, 1); // 琴
  ssd1306_write_char_cn(fontCN16x16, 2); // 模
  ssd1306_write_char_cn(fontCN16x16, 3); // 式
  ssd1306_update_screen();

  while (1) {
    enum button_state_t bt_state_now = readPianoPin(); // 读取当前的按钮状态
    switch (bt_state_now) {
    case kMenu:
      if (bt_state_last !=
          bt_state_now) { // 如果上一次没有按下menu按钮，说明这是刚按下menu按钮
        button_menu_tick = HAL_GetTick(); // 重置menu按钮计数器为当前时间
      }
      if (bt_state_last == kMenu && (HAL_GetTick() - button_menu_tick) > 300) {
        // 大于300ms说明是长按
        // 切换界面状态
        BUZZER_PlayNote(M1);
        HAL_Delay(100);
        BUZZER_Stop();
        ssd1306_black_screen();
        ssd1306_update_screen();
        return;
      }
      break; // 菜单按钮逻辑

    case kBT1:
    case kBT2:
    case kBT3:
    case kBT4:
    case kBT5:
    case kBT6:
    case kBT7:
    case kBT8:
      BUZZER_PlayNote(FreTab[bt_state_now + note_offset]);
      ssd1306_set_cursor(32 + 16, 16 + 2);
      ssd1306_write_string(font16x26, FreTabStr[bt_state_now + note_offset]);
      ssd1306_update_screen();
      break; // 钢琴按钮逻辑

    case kNone:
    default:
      if (bt_state_last ==
          kMenu) { // 如果上一个按钮状态是menu，说明松开了menu按钮
        // 切换高低音(低->中->高)
        if (note_offset == kOffsetHigh) {
          note_offset = kOffsetLow;
        } else if (note_offset == kOffsetMiddle) {
          note_offset = kOffsetHigh;
        } else if (note_offset == kOffsetLow) {
          note_offset = kOffsetMiddle;
        }
      }
      if (bt_state_last < 9 && bt_state_last > 0) {
        BUZZER_Stop();
        ssd1306_set_cursor(32 + 16, 16 + 2);
        ssd1306_draw_black(16 * 2, 26); // 清除音符显示部分
        ssd1306_update_screen();
      }
      break; // 其他情况逻辑
    }
    bt_state_last = bt_state_now;
  }
}

void MusicPlayer_loop(void) {
  enum button_state_t bt_state_last = kNone; // 记录上一次的按钮状态
  uint32_t button_menu_tick = 0; // 记录上一次的menu按钮按下的时间
  uint8_t is_music_play = 1;
  struct music_sheet_t *music_note_sheet = &music_solitary_brave;
  uint16_t note_counter = 0;
  // uint32_t note_halt_tick = 0; // 记录上一音符开始休眠的时间

  ssd1306_set_cursor(32, 0);
  ssd1306_write_char_cn(fontCN16x16, 4);  // 音
  ssd1306_write_char_cn(fontCN16x16, 10); // 乐
  ssd1306_write_char_cn(fontCN16x16, 2);  // 模
  ssd1306_write_char_cn(fontCN16x16, 3);  // 式

  ssd1306_set_cursor(0, 48);
  ssd1306_write_char_cn(fontCN16x16, 21); // 播放
  ssd1306_write_char_cn(fontCN16x16, 22);

  ssd1306_set_cursor(40, 16);
  ssd1306_write_char_cn(fontCN16x16, 25); // 孤勇者
  ssd1306_write_char_cn(fontCN16x16, 26);
  ssd1306_write_char_cn(fontCN16x16, 27);
  ssd1306_update_screen();

  while (1) {
    enum button_state_t bt_state_now = readPianoPin(); // 读取当前的按钮状态
    switch (bt_state_now) {
    case kMenu:
      if (bt_state_last !=
          bt_state_now) { // 如果上一次没有按下menu按钮，说明这是刚按下menu按钮
        button_menu_tick = HAL_GetTick(); // 重置menu按钮计数器为当前时间
      }
      if (bt_state_last == kMenu && (HAL_GetTick() - button_menu_tick) > 300) {
        // 大于300ms说明是长按
        // 切换界面状态
        BUZZER_PlayNote(M1);
        HAL_Delay(100);
        BUZZER_Stop();
        ssd1306_black_screen();
        ssd1306_update_screen();
        return;
      }
      break; // 菜单按钮逻辑

    case kBT1:                     // 暂停
      if (bt_state_last != kBT1) { // 说明是刚按下状态
        if (is_music_play) {
          is_music_play = 0;
          BUZZER_Stop();
          ssd1306_set_cursor(0, 48);
          ssd1306_write_char_cn(fontCN16x16, 23); // 暂停
          ssd1306_write_char_cn(fontCN16x16, 24); // 暂停
          ssd1306_update_screen();
        } else {
          is_music_play = 1;
          ssd1306_set_cursor(0, 48);
          ssd1306_write_char_cn(fontCN16x16, 21); // 播放
          ssd1306_write_char_cn(fontCN16x16, 22); // 播放
          ssd1306_update_screen();
        }
      }
      break;

    case kBT2:
    case kBT3:
    case kBT4:
    case kBT5:
    case kBT6:
    case kBT7:
    case kBT8:
    case kNone:
    default:
      if (bt_state_last ==
          kMenu) { // 如果上一个按钮状态是menu，说明松开了menu按钮
        // 切歌
        if (music_note_sheet == &music_solitary_brave) {
          note_counter = 0;
          music_note_sheet = &music_xiaoxingxing;
          ssd1306_set_cursor(40, 16);
          ssd1306_write_char_cn(fontCN16x16, 11); // 小星星
          ssd1306_write_char_cn(fontCN16x16, 12);
          ssd1306_write_char_cn(fontCN16x16, 12);
          ssd1306_update_screen();
        } else {
          note_counter = 0;
          music_note_sheet = &music_solitary_brave;
          ssd1306_set_cursor(40, 16);
          ssd1306_write_char_cn(fontCN16x16, 25); // 孤勇者
          ssd1306_write_char_cn(fontCN16x16, 26);
          ssd1306_write_char_cn(fontCN16x16, 27);
          ssd1306_update_screen();
        }
        break;
      }

      if (is_music_play) {
        if (note_counter >
            music_note_sheet->length - 1) { // 如果超出乐谱长度，从头开始演奏
          note_counter = 0;
        }
        BUZZER_PlayNote(music_note_sheet->note_sheet[note_counter * 2]);
        HAL_Delay(5 * music_note_sheet->note_sheet[note_counter * 2 + 1]);
        note_counter++;
      }

      break; // 其他情况逻辑
    }
    bt_state_last = bt_state_now;
  }
}

void Record_loop(void) {
  enum button_state_t bt_state_last = kNone; // 记录上一次的按钮状态
  uint32_t button_menu_tick = 0; // 记录上一次的menu按钮按下的时间
  uint32_t button_buzzer_tick = 0; // 记录上一次的menu按钮按下的时间
  enum note_offset_t note_offset = kOffsetMiddle; // 音符偏移，默认中音
  uint16_t record_note_counter = 0;

  ssd1306_set_cursor(32, 0);
  ssd1306_write_char_cn(fontCN16x16, 13); // 录制模式
  ssd1306_write_char_cn(fontCN16x16, 14);
  ssd1306_write_char_cn(fontCN16x16, 2);
  ssd1306_write_char_cn(fontCN16x16, 3);
  ssd1306_update_screen();

  while (1) {
    enum button_state_t bt_state_now = readPianoPin(); // 读取当前的按钮状态
    switch (bt_state_now) {
    case kMenu:
      if (bt_state_last !=
          bt_state_now) { // 如果上一次没有按下menu按钮，说明这是刚按下menu按钮
        button_menu_tick = HAL_GetTick(); // 重置menu按钮计数器为当前时间
      }
      if (bt_state_last == kMenu && (HAL_GetTick() - button_menu_tick) > 300) {
        // 大于300ms说明是长按
        // 切换界面状态
        BUZZER_PlayNote(M1);
        HAL_Delay(100);
        BUZZER_Stop();
        ssd1306_black_screen();
        ssd1306_update_screen();
        return;
      }
      break; // 菜单按钮逻辑

    case kBT1:
    case kBT2:
    case kBT3:
    case kBT4:
    case kBT5:
    case kBT6:
    case kBT7:
    case kBT8:
      BUZZER_PlayNote(FreTab[bt_state_now + note_offset]);
      button_buzzer_tick = HAL_GetTick();
      record_note_sheet[record_note_counter * 2] =
          FreTab[bt_state_now + note_offset];

      ssd1306_set_cursor(32 + 16, 16 + 2);
      ssd1306_write_string(font16x26, FreTabStr[bt_state_now + note_offset]);
      ssd1306_update_screen();
      break; // 钢琴按钮逻辑

    case kNone:
    default:
      if (bt_state_last ==
          kMenu) { // 如果上一个按钮状态是menu，说明松开了menu按钮
        // 切换高低音(低->中->高)
        if (note_offset == kOffsetHigh) {
          note_offset = kOffsetLow;
        } else if (note_offset == kOffsetMiddle) {
          note_offset = kOffsetHigh;
        } else if (note_offset == kOffsetLow) {
          note_offset = kOffsetMiddle;
        }
      }
      if (bt_state_last < 9 && bt_state_last > 0) {
        BUZZER_Stop();
        record_note_sheet[record_note_counter * 2 + 1] =
            HAL_GetTick() - button_buzzer_tick;
        record_note_counter++;
        ssd1306_set_cursor(32 + 16, 16 + 2);
        ssd1306_draw_black(16 * 2, 26); // 清除音符显示部分
        ssd1306_update_screen();
      }
      break; // 其他情况逻辑
    }
    bt_state_last = bt_state_now;
  }
}

void RecordPlayer_loop(void) {
  if (record_note_sheet[0] == 0) { // 如果没有录制歌曲则退出
    return;
  }
  enum button_state_t bt_state_last = kNone; // 记录上一次的按钮状态
  uint32_t button_menu_tick = 0; // 记录上一次的menu按钮按下的时间
  uint8_t is_music_play = 1;
  uint16_t note_counter = 0;

  ssd1306_set_cursor(16, 0);
  ssd1306_write_char_cn(fontCN16x16, 13); // 录制播放模式
  ssd1306_write_char_cn(fontCN16x16, 14);
  ssd1306_write_char_cn(fontCN16x16, 21);
  ssd1306_write_char_cn(fontCN16x16, 22);
  ssd1306_write_char_cn(fontCN16x16, 2);
  ssd1306_write_char_cn(fontCN16x16, 3);

  ssd1306_set_cursor(0, 48);
  ssd1306_write_char_cn(fontCN16x16, 21); // 播放
  ssd1306_write_char_cn(fontCN16x16, 22);
  ssd1306_update_screen();

  while (1) {
    enum button_state_t bt_state_now = readPianoPin(); // 读取当前的按钮状态
    switch (bt_state_now) {
    case kMenu:
      if (bt_state_last !=
          bt_state_now) { // 如果上一次没有按下menu按钮，说明这是刚按下menu按钮
        button_menu_tick = HAL_GetTick(); // 重置menu按钮计数器为当前时间
      }
      if (bt_state_last == kMenu && (HAL_GetTick() - button_menu_tick) > 300) {
        // 大于300ms说明是长按
        // 切换界面状态
        BUZZER_PlayNote(M1);
        HAL_Delay(100);
        BUZZER_Stop();
        ssd1306_black_screen();
        ssd1306_update_screen();
        return;
      }
      break; // 菜单按钮逻辑

    case kBT1:                     // 暂停
      if (bt_state_last != kBT1) { // 说明是刚按下状态
        if (is_music_play) {
          is_music_play = 0;
          BUZZER_Stop();
          ssd1306_set_cursor(0, 48);
          ssd1306_write_char_cn(fontCN16x16, 23); // 暂停
          ssd1306_write_char_cn(fontCN16x16, 24);
          ssd1306_update_screen();
        } else {
          is_music_play = 1;
          ssd1306_set_cursor(0, 48);
          ssd1306_write_char_cn(fontCN16x16, 21); // 播放
          ssd1306_write_char_cn(fontCN16x16, 22);
          ssd1306_update_screen();
        }
      }
      break;

    case kBT2:
    case kBT3:
    case kBT4:
    case kBT5:
    case kBT6:
    case kBT7:
    case kBT8:
    case kNone:
    default:
      if (bt_state_last ==
          kMenu) { // 如果上一个按钮状态是menu，说明松开了menu按钮
        // 切歌
        break;
      }

      if (is_music_play) {
        if (note_counter > 127) { // 如果超出乐谱长度，从头开始演奏
          note_counter = 0;
        }
        BUZZER_PlayNote(record_note_sheet[note_counter * 2]);
        HAL_Delay(10 * record_note_sheet[note_counter * 2 + 1]);
        note_counter++;
      }

      break; // 其他情况逻辑
    }
    bt_state_last = bt_state_now;
  }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
     line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
