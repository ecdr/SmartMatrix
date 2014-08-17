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


