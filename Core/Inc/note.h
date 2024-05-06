#ifndef __NOTE_H
#define __NOTE_H

#include "note.h"
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define PROPORT 72000 // Tclk/(arr+1)=72000000/(1000)

// 根据Tout=
// ((arr+1)*(psc+1))/Tclk推出psc值就是define定义的值，Tout为音调频率131Hz的倒数，Tclk=72MHz
#define L1 ((PROPORT / 262) - 1)  // 低调　do 的频率
#define L1h ((PROPORT / 277) - 1) // 低调　do# 的频率
#define L2 ((PROPORT / 296) - 1)  // 低调　re 的频率
#define L2h ((PROPORT / 311) - 1) // 低调　re# 的频率
#define L3 ((PROPORT / 330) - 1)  // 低调　mi 的频率
#define L4 ((PROPORT / 349) - 1)  // 低调　fa 的频率
#define L4h ((PROPORT / 370) - 1) // 低调　fa# 的频率
#define L5 ((PROPORT / 392) - 1)  // 低调　sol 的频率
#define L5h ((PROPORT / 415) - 1) // 低调　sol# 的频率
#define L6 ((PROPORT / 440) - 1)  // 低调　la 的频率
#define L6h ((PROPORT / 466) - 1) // 低调　la# 的频率
#define L7 ((PROPORT / 494) - 1)  // 低调　si 的频率

#define M1 ((PROPORT / 523) - 1)  // 中调　do 的频率
#define M1h ((PROPORT / 554) - 1) // 中调　do# 的频率
#define M2 ((PROPORT / 587) - 1)  // 中调　re 的频率
#define M2h ((PROPORT / 622) - 1) // 中调　re# 的频率
#define M3 ((PROPORT / 659) - 1)  // 中调　mi 的频率
#define M4 ((PROPORT / 699) - 1)  // 中调　fa 的频率
#define M4h ((PROPORT / 740) - 1) // 中调　fa# 的频率
#define M5 ((PROPORT / 784) - 1)  // 中调　sol 的频率
#define M5h ((PROPORT / 831) - 1) // 中调　sol# 的频率
#define M6 ((PROPORT / 880) - 1)  // 中调　la 的频率
#define M6h ((PROPORT / 932) - 1) // 中调　la# 的频率
#define M7 ((PROPORT / 988) - 1)  // 中调　si 的频率

#define H1 ((PROPORT / 1048) - 1)  // 高调　do 的频率
#define H1h ((PROPORT / 1109) - 1) // 高调　do# 的频率
#define H2 ((PROPORT / 1176) - 1)  // 高调　re 的频率
#define H2h ((PROPORT / 1245) - 1) // 高调　re# 的频率
#define H3 ((PROPORT / 1320) - 1)  // 高调　mi 的频率
#define H4 ((PROPORT / 1397) - 1)  // 高调　fa 的频率
#define H4h ((PROPORT / 1480) - 1) // 高调　fa# 的频率
#define H5 ((PROPORT / 1568) - 1)  // 高调　sol 的频率
#define H5h ((PROPORT / 1640) - 1) // 高调　sol# 的频率
#define H6 ((PROPORT / 1760) - 1)  // 高调　la 的频率
#define H6h ((PROPORT / 1865) - 1) // 高调　la# 的频率
#define H7 ((PROPORT / 1976) - 1)  // 高调　si 的频率
#define H8 ((PROPORT / 2093) - 1)  // 高调　do## 的频率

#define Z0 0

unsigned int FreTab[] = {L1, L2, L3, L4, L5, L6, L7, 
                         M1, M2, M3, M4,M5, M6, M7, 
                         H1, H2, H3, H4, H5, H6, H7, H8};
char FreTabStr[][3] = {"L1", "L2", "L3", "L4", "L5", "L6", "L7", 
                       "M1", "M2", "M3", "M4", "M5", "M6", "M7", 
                       "H1", "H2","H3", "H4", "H5", "H6", "H7", "H8"};

enum note_offset_t { kOffsetLow = -1, kOffsetMiddle = 6, kOffsetHigh = 13 };

struct music_sheet_t
{
    const uint16_t* note_sheet;
    const uint16_t length;
};

#define NOTE_INCLUDE_SOLITARY_BRAVE
#define NOTE_INCLUDE_XIAOXINGXING

#ifdef NOTE_INCLUDE_SOLITARY_BRAVE
const uint16_t note_solitary_brave[] = {
    M6, 50, 
    M7, 50, 
    H1, 50, 
    H2, 50, 
    M7, 50, 
    H1, 50, 
    H1, 100, 
    Z0, 10, // 爱你孤身走暗巷
    H1, 50, 
    M7, 50, 
    H1, 50, 
    H2, 50, 
    M7, 50, 
    H1, 50, 
    H1, 100, 
    Z0, 10, // 爱你不跪的模样
    H1, 50, 
    H2, 50, 
    H3, 50, 
    H2, 50, 
    H3, 50, 
    H2, 50, 
    H3, 100, 
    H3, 50, 
    H3, 50, 
    H2, 50, 
    H3, 100, 
    H5, 100, 
    H3, 100, 
    Z0, 10, // 爱你对峙过绝望不肯哭一场
};
#endif

#ifdef NOTE_INCLUDE_XIAOXINGXING
const uint16_t note_xiaoxingxing[] = {
    H1, 50, 
    Z0, 10, 
    H1, 50, 
    Z0, 10, 
    H5, 50, 
    Z0, 10, 
    H5, 50, 
    Z0, 10, 
    H6, 50, 
    Z0, 10, 
    H6, 50, 
    Z0, 10, 
    H5, 100, 
    Z0, 10, 
    H4, 50, 
    Z0, 10, 
    H4, 50, 
    Z0, 10, 
    H3, 50, 
    Z0, 10, 
    H3, 50, 
    Z0, 10, 
    H2, 50, 
    Z0, 10, 
    H2, 50, 
    Z0, 10, 
    H1, 100, 
    Z0, 10, 

    H5, 50, 
    Z0, 10, 
    H5, 50, 
    Z0, 10, 
    H4, 50, 
    Z0, 10, 
    H4, 50, 
    Z0, 10, 
    H3, 50, 
    Z0, 10, 
    H3, 50, 
    Z0, 10, 
    H2, 100, 
    Z0, 10, 

    H5, 50, 
    Z0, 10, 
    H5, 50, 
    Z0, 10, 
    H4, 50, 
    Z0, 10, 
    H4, 50, 
    Z0, 10, 
    H3, 50, 
    Z0, 10, 
    H3, 50, 
    Z0, 10, 
    H2, 100, 
    Z0, 10, 

    H1, 50, 
    Z0, 10, 
    H1, 50, 
    Z0, 10, 
    H5, 50, 
    Z0, 10, 
    H5, 50, 
    Z0, 10, 
    H6, 50, 
    Z0, 10, 
    H6, 50, 
    Z0, 10, 
    H5, 100, 
    Z0, 10, 
    H4, 50, 
    Z0, 10, 
    H4, 50, 
    Z0, 10, 
    H3, 50, 
    Z0, 10, 
    H3, 50, 
    Z0, 10, 
    H2, 50, 
    Z0, 10, 
    H2, 50, 
    Z0, 10, 
    H1, 100, 
    Z0, 100, 

};
#endif

#ifdef NOTE_INCLUDE_SOLITARY_BRAVE
struct music_sheet_t music_solitary_brave = {note_solitary_brave, (sizeof(note_solitary_brave) / sizeof(note_solitary_brave[0])) / 2};
#endif

#ifdef NOTE_INCLUDE_XIAOXINGXING
struct music_sheet_t music_xiaoxingxing = {note_xiaoxingxing, (sizeof(note_xiaoxingxing) / sizeof(note_xiaoxingxing[0])) / 2};
#endif

#ifdef __cplusplus
}
#endif

#endif /* __NOTE_H */
