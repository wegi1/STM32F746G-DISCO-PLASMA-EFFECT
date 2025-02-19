#ifndef __VGA_H_
#define __VGA_H_
#include "main.h"
//------------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif


  extern LTDC_HandleTypeDef hLtdcHandler;

  uint8_t SIN256[256];
  uint8_t COS256[256];
  uint32_t paleta[256];
  uint8_t rColor[256];
  uint8_t gColor[256];
  uint8_t bColor[256];

  uint32_t tab[256];

  void init_sin();
  void genTAB(uint32_t phase, uint32_t tick);
  void waitVBL();
  void resetScreen(uint8_t color);
  void fadePalette();

//------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------------
#endif /* __VGA_H_ */