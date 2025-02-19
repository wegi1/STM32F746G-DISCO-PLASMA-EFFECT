#include "plasma.h"
#include "vga.h"
#include "main.h"

uint8_t activeLayer = 0;

extern unsigned long Getrandom(uint32_t min, uint32_t max);

//------------------------------------------------------------------------------------
uint8_t sintable[256] = {
    127, 124, 121, 118, 114, 111, 108, 105, 102, 99, 96, 93, 90, 87, 84, 81,
    78, 75, 72, 69, 67, 64, 61, 59, 56, 53, 51, 48, 46, 43, 41, 39,
    37, 34, 32, 30, 28, 26, 24, 22, 21, 19, 17, 16, 14, 13, 12, 10,
    9, 8, 7, 6, 5, 4, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 4, 4, 5, 6, 7, 8,
    10, 11, 12, 14, 15, 17, 18, 20, 22, 23, 25, 27, 29, 31, 33, 35,
    38, 40, 42, 45, 47, 50, 52, 55, 57, 60, 63, 65, 68, 71, 74, 77,
    79, 82, 85, 88, 91, 94, 97, 100, 104, 107, 110, 113, 116, 119, 122, 125,
    129, 132, 135, 138, 141, 144, 147, 150, 154, 157, 160, 163, 166, 169, 172, 175,
    177, 180, 183, 186, 189, 191, 194, 197, 199, 202, 204, 207, 209, 212, 214, 216,
    219, 221, 223, 225, 227, 229, 231, 232, 234, 236, 237, 239, 240, 242, 243, 244,
    246, 247, 248, 249, 250, 250, 251, 252, 252, 253, 253, 254, 254, 254, 254, 254,
    254, 254, 254, 254, 254, 253, 253, 252, 252, 251, 250, 249, 248, 247, 246, 245,
    244, 242, 241, 240, 238, 237, 235, 233, 232, 230, 228, 226, 224, 222, 220, 217,
    215, 213, 211, 208, 206, 203, 201, 198, 195, 193, 190, 187, 185, 182, 179, 176,
    173, 170, 167, 164, 161, 158, 155, 152, 149, 146, 143, 140, 136, 133, 130, 127};
//------------------------------------------------------------------------------------

void draw_plasma(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t t, uint16_t zoom)
{
    uint8_t c;
    uint16_t c1, c2, c3, v1, v2;
    uint16_t i, j;

    v1 = SIN256[(Getrandom(0, 3) + t * 3) % 256];
    v2 = COS256[(127 + t * 2) % 256];
    for (j = y; j < y + h; j++)
    {
        c3 = SIN256[(((j * zoom) >> 8) + t * 3 - t) % 256];
        for (i = x; i < x + w; i++)
        {
            c1 = SIN256[(((i * zoom) >> 8) + t * 3 - t) % 256];
            c2 = COS256[((i * v1) / w + (j * v2) / h + t) % 256];
            c = (uint8_t)((c1 + c2 + c3) / 3);
            if ((i >= 0) && (i < 480) && (j >= 0) && (y < 272))
            // BSP_LCD_DrawPixel(i, j, c);
            {
                if (activeLayer == 1)
                    *(__IO uint8_t *)(LCD_FB_START_ADDRESS + i + j * 480) = (uint8_t)c & 0xff;
                if (activeLayer == 0)
                    *(__IO uint8_t *)(480 * 272 + LCD_FB_START_ADDRESS + i + j * 480) = (uint8_t)c & 0xff;
            }
        }
    }
}

void plasma()
{
    uint32_t tt = 0;
    uint32_t frame = 0;
    uint8_t mini_frame = 0;
    uint32_t step = 90000;
    uint8_t z = 0;
    int8_t dx = 7;
    init_sin();

    while (!BSP_PB_GetState(BUTTON_WAKEUP))
    {
        tt += 2;
        z++;

        genTAB(frame, step);
        frame++;
        mini_frame++;
        if (mini_frame == 3)
        {
            step += 2;
            mini_frame = 0;
        }

        draw_plasma(0, 0, 480, 272, tt, sintable[z]);
        if (activeLayer == 1)
        {
            waitVBL();
            BSP_LCD_SetLayerAddress(0, LCD_FB_START_ADDRESS);
            activeLayer = 0;
        }
        else
        {
            waitVBL();
            BSP_LCD_SetLayerAddress(0, LCD_FB_START_ADDRESS + 480 * 272);
            activeLayer = 1;
        }
    }
    uint32_t test = 0;
    while (test < 256)
    {
        tt++;
        test++;
        waitVBL();
        z += dx;
        if (z >= 300)
            dx = -dx;
        if (z <= 0)
            dx = -dx;
        fadePalette();
        draw_plasma(0, 0, 480, 272, tt, z);
        if (activeLayer == 1)
        {
            waitVBL();
            BSP_LCD_SetLayerAddress(0, LCD_FB_START_ADDRESS);
            activeLayer = 0;
        }
        else
        {
            waitVBL();
            BSP_LCD_SetLayerAddress(0, LCD_FB_START_ADDRESS + 480 * 272);
            activeLayer = 1;
        }
    }
}