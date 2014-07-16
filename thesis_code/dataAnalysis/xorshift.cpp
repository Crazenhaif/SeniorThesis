//
//  xorshift.c
//  
//
//  Created by Cord LaBarre on 10/10/13.
//
//

#include <stdio.h>
#include <stdint.h>
#include <cmath>
#include <time.h>
#include <stdlib.h>
    // INT_MAX header.
#include <limits.h>

/*
 * Returns a random number between -1 and 1 exclusive using an exclusive-or
 * Bit-shifting algorithm.
 * 
 * Arguments: none.
 */
double xor128(void) {
    srand (time(NULL));
        // Declare vars used for shifting.
    static uint32_t x = rand() % INT_MAX;
    static uint32_t y = rand() % INT_MAX;
    static uint32_t z = rand() % INT_MAX;
    static uint32_t w = rand() % INT_MAX;
    uint32_t t;
        // Perform bit shifts for randomization.
    t = x ^ (x << 11);
    x = y; y = z; z = w;
    w = w ^ (w >> 19) ^ (t ^ (t >> 8));
    
        // Convert the number generated to a 0 < 1.
    double f = (double) w / 1000000;
    double intPart, decPart;
    decPart = modf(f, &intPart);
        // Return our
    return decPart;
}
// http:// en.wikkipedia.org/wiki/Xorshift
// http:// en.wikipedia.org/wiki/Box%E2%80%93Muller_transform

