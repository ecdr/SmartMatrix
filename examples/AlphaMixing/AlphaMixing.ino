/*
 * Alpha Mixing Experiment for the 32x32 RGB LED Matrix
 *
 * Written by: Craig A. Lindley
 * Version: 1.0
 * Last Update: 07/22/2014
 *
 * Copyright (c) 2014 Craig A. Lindley
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/* 
// Begin changes to SmartMatrix.h 
#define USE_ALPHA_MIXING 1

// color
typedef struct rgb24 {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
#if (USE_ALPHA_MIXING == 1)
    uint8_t alpha;	
#endif
} rgb24;
// End changes to SmartMatrix.h 

// Begin changes to MatrixGraphics.cpp 
#if (USE_ALPHA_MIXING == 1)
// Alpha mix pixel being drawn to the screen (srcPixelColor) with pixel on screen (destPixelColor)
rgb24 alphaMixColors(rgb24 destPixelColor, rgb24 srcPixelColor) {

    rgb24 newColor;
    newColor.alpha = 255;

    float alpha = srcPixelColor.alpha / 255.0;
    float oneMinusAlpha = 1.0 - alpha;

    newColor.red   = (srcPixelColor.red   * alpha) + (destPixelColor.red   * oneMinusAlpha);
    newColor.green = (srcPixelColor.green * alpha) + (destPixelColor.green * oneMinusAlpha);
    newColor.blue  = (srcPixelColor.blue  * alpha) + (destPixelColor.blue  * oneMinusAlpha);

    return newColor;
}
#endif
 
void SmartMatrix::drawPixel(int16_t x, int16_t y, rgb24 color) {
    int hwx, hwy;

    // check for out of bounds coordinates
    if (x < 0 || y < 0 || x >= screenConfig.localWidth || y >= screenConfig.localHeight)
        return;

    // map pixel into hardware buffer before writing
    if (screenConfig.rotation == rotation0) {
        hwx = x;
        hwy = y;
    } else if (screenConfig.rotation == rotation180) {
        hwx = (MATRIX_WIDTH - 1) - x;
        hwy = (MATRIX_HEIGHT - 1) - y;
    } else if (screenConfig.rotation == rotation90) {
        hwx = (MATRIX_WIDTH - 1) - y;
        hwy = x;
    } else { // if (screenConfig.rotation == rotation270)
        hwx = y;
        hwy = (MATRIX_HEIGHT - 1) - x;
    }

#if (USE_ALPHA_MIXING == 1)
    rgb24 destPixelColor = readPixel(x, y);
    rgb24 newColor = alphaMixColors(destPixelColor, color);
    copyRgb24(&backgroundBuffer[currentDrawBuffer][hwy][hwx], newColor);
#else
    copyRgb24(&backgroundBuffer[currentDrawBuffer][hwy][hwx], color);
#endif
}

// x0, x1, and y must be in bounds (0-localWidth/Height), x1 > x0
void SmartMatrix::drawHardwareHLine(uint8_t x0, uint8_t x1, uint8_t y, rgb24 color) {
    int i;

    for (i = x0; i <= x1; i++) {

#if (USE_ALPHA_MIXING == 1)
    rgb24 destPixelColor = backgroundBuffer[currentDrawBuffer][y][i];
    rgb24 newColor = alphaMixColors(destPixelColor, color);
    copyRgb24(&backgroundBuffer[currentDrawBuffer][y][i], newColor);
#else
    copyRgb24(&backgroundBuffer[currentDrawBuffer][y][i], color);
#endif
    }
}

// x, y0, and y1 must be in bounds (0-localWidth/Height), y1 > y0
void SmartMatrix::drawHardwareVLine(uint8_t x, uint8_t y0, uint8_t y1, rgb24 color) {
    int i;

    for (i = y0; i <= y1; i++) {

#if (USE_ALPHA_MIXING == 1)
    rgb24 destPixelColor = backgroundBuffer[currentDrawBuffer][i][x];
    rgb24 newColor = alphaMixColors(destPixelColor, color);
    copyRgb24(&backgroundBuffer[currentDrawBuffer][i][x], newColor);
#else
    copyRgb24(&backgroundBuffer[currentDrawBuffer][i][x], color);
#endif
    }
}
// End changes to MatrixGraphics.cpp 
 
// Begin changes to MatrixColor.cpp 
void SmartMatrix::copyRgb24(rgb24 *dst, rgb24 *src) {
    dst->green = src->green;
    dst->red = src->red;
    dst->blue = src->blue;
#if (USE_ALPHA_MIXING == 1)
    dst->alpha = src->alpha;
#endif
}

void SmartMatrix::copyRgb24(rgb24 *dst, rgb24 src) {
    dst->green = src.green;
    dst->red = src.red;
    dst->blue = src.blue;
#if (USE_ALPHA_MIXING == 1)
    dst->alpha = src.alpha;
#endif
}
// End changes to MatrixColor.cpp 
*/

#include "SmartMatrix_32x32.h"

// Create instance of matrix
SmartMatrix matrix;

const int DEFAULT_BRIGHTNESS = 100;

const int WIDTH = 32;
const int MINX  = 0;
const int MAXX  = WIDTH - 1;
const int MIDX  = WIDTH / 2;

const int HEIGHT = 32;
const int MINY   = 0;
const int MAXY   = HEIGHT - 1;
const int MIDY   = HEIGHT / 2;

/*******************************************************************/
/***                     Main Program Setup                     ***/
/*******************************************************************/

#define COLOR_BLACK {0, 0, 0, 255}

void setup() {

    // Setup serial interface
    Serial.begin(115200);

    // Wait for serial interface to settle
    delay(2000);

    // Initialize 32x32 LED Matrix
    matrix.begin();
    matrix.setBrightness(DEFAULT_BRIGHTNESS);
    matrix.setColorCorrection(cc24);

    // Clear screen
    matrix.fillScreen(COLOR_BLACK);
    matrix.swapBuffers();
}

/*******************************************************************/
/***                      Main Program Loop                      ***/
/*******************************************************************/

void loop() {
    
    // Grid of red pixels on black background
    for (int y = 0; y < HEIGHT; y += 4) {
        for (int x = 0; x < WIDTH; x += 4) {
            matrix.drawPixel(x, y, {255, 0, 0, 128});
        }
    }
    // Divide screen into four quadrants each with a different alpha value
    matrix.fillRectangle(   0,    0, MIDX, MIDY, {100, 100, 100,   0});
    matrix.fillRectangle(MIDX,    0, MAXX, MIDY, {100, 100, 100,  64});
    matrix.fillRectangle(   0, MIDY, MIDX, MAXY, {100, 100, 100, 128});
    matrix.fillRectangle(MIDX, MIDY, MAXX, MAXY, {100, 100, 100, 255});
    
    matrix.swapBuffers();
    
    while (true) {
        
        ;
    }
}


