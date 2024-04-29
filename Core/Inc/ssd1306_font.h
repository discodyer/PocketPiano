#ifndef __SSD1306_FONT_H__
#define __SSD1306_FONT_H__


#include <stdint.h>


/* SSD1306 Include Font */
#define SSD1306_INCLUDE_FONT_6X8
#define SSD1306_INCLUDE_FONT_7X10
#define SSD1306_INCLUDE_FONT_11X18
#define SSD1306_INCLUDE_FONT_16X26
#define SSD1306_INCLUDE_FONT_CN_16X16


/* SSD1306 Font Structure */
typedef struct 
{
    uint8_t width;
    uint8_t height;
    uint16_t *data;

} SSD1306_FONT;

/* SSD1306 CN Font Structure */
typedef struct 
{
    uint8_t width;
    uint8_t height;
    uint8_t *data;

} SSD1306_CN_FONT;


/* SSD1306 Font Variable */
#ifdef SSD1306_INCLUDE_FONT_6X8
extern SSD1306_FONT font6x8;
#endif

#ifdef SSD1306_INCLUDE_FONT_7X10
extern SSD1306_FONT font7x10;
#endif

#ifdef SSD1306_INCLUDE_FONT_11X18
extern SSD1306_FONT font11x18;
#endif

#ifdef SSD1306_INCLUDE_FONT_16X26
extern SSD1306_FONT font16x26;
#endif

#ifdef SSD1306_INCLUDE_FONT_CN_16X16
extern SSD1306_CN_FONT fontCN16x16;
#endif


#endif /* __SSD1306_FONT_H__ */
