/* 
  Demos on STM32F746G-Disco by ProteusPL
*/
#include "main.h"
#include "plasma.h"
#include "board_init.h"
#include "vga.h"

int main(void)
{
  boardInit();
  plasma();
  resetScreen(1);
  while(1);
}