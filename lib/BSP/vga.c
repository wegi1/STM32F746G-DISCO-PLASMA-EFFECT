#include "vga.h"
#include "stdint.h" 
#include "math.h"




  void init_sin()
  {
    uint16_t i;
    float v;
    for (i = 0; i < 256; ++i)
    {
      v = (sin(2.0 * M_PI * i / 255.0) + 1.0) / 2.0;
      SIN256[i] = (uint8_t)(255 * v);
      tab[i] = (uint8_t)(255 * v) << 16;
      v = (cos(2.0 * M_PI * i / 255.0) + 1.0) / 2.0;
      COS256[i] = (uint8_t)(255 * v);
    }
  }

  void genTAB(uint32_t phase, uint32_t tick)
  {
    uint8_t index = phase % 256;
    rColor[index] = (uint8_t)(128 + 127 * cos((double)phase * M_PI / 256 + (double)tick / 76));
    gColor[index] = (uint8_t)(128 + 127 * sin((double)phase * M_PI / 256 + (double)tick / 74));
    bColor[index] = (uint8_t)(128 + 127 * cos((double)phase * M_PI / 256 + (double)tick / 62));
    paleta[index] = rColor[index] << 16 | gColor[index] << 8 | bColor[index];

    HAL_LTDC_ConfigCLUT(&hLtdcHandler, (uint32_t *)paleta, 256, 0);
  }
  void waitVBL()
  {
    while (!(LTDC->CDSR & LTDC_CDSR_VSYNCS));   
  }

  void resetScreen(uint8_t color)
  {
    extern const  uint32_t DefaultPalette[];
      for (uint16_t x=0;x<480;x++)
  {
    for (uint16_t y=0;y<272;y++)
    {
      BSP_LCD_DrawPixel(x,y,color);
    }
  }
  HAL_LTDC_ConfigCLUT(&hLtdcHandler, (uint32_t *)DefaultPalette, 256, 0); 
  }

  void fadePalette( )
  {
    
      for (int y=0;y<256;y++){
      if (rColor[y] > 0) rColor[y]--;
      if (gColor[y] > 0) gColor[y]--;
      if (bColor[y] > 0) bColor[y]--;

      paleta[y] = rColor[y] << 16 | gColor[y] << 8 | bColor[y];
      }
      HAL_LTDC_ConfigCLUT(&hLtdcHandler, (uint32_t *)paleta, 256, 0);
      BSP_LED_Toggle(LED_GREEN);
    
  }