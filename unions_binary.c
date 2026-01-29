/*
 * unions_binary.c
 * 
 * Copyright (c) 2026 Miroslaw Staron
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// Standard C libraries for input/output and time functions
#include <stdio.h>
#include <time.h>

/*
 * WeekdayBits union - demonstrates how the same memory location can be
 * interpreted in two different ways:
 *  - an unsigned char (raw value) - view all 8 bits as a single byte
 *  - bit-fields (individual bits) - access specific bits by name
 * 
 * This is useful for compact storage of boolean flags or bit patterns
 */
// #pragma pack(1)      
typedef union {
    unsigned char value;  // Access as a single 8-bit value (0-255)

    // Named bit-fields for each day of the week (each takes 1 bit)
    struct {
        unsigned mon : 1;  // Bit 0: Monday flag
        unsigned tue : 1;  // Bit 1: Tuesday flag
        unsigned wed : 1;  // Bit 2: Wednesday flag
        unsigned thu : 1;  // Bit 3: Thursday flag
        unsigned fri : 1;  // Bit 4: Friday flag
        unsigned sat : 1;  // Bit 5: Saturday flag
        unsigned sun : 1;  // Bit 6: Sunday flag
        unsigned     : 1;  // Bit 7: unused padding to fill 8 bits
    } bits;
} WeekdayBits;
//#pragma pack()

int bitunions_main(void) {
    // Get the current system time
    time_t now = time(NULL);
    
    // Convert system time to local time structure (contains day of week info)
    struct tm *t = localtime(&now);

    // Create a union instance and initialize to zero (all bits off)
    WeekdayBits day = {0};

    // Map the current day of week to the appropriate bit in our union
    // tm_wday: 0 = Sunday, 1 = Monday, ..., 6 = Saturday
    switch (t->tm_wday) {
        case 0: day.bits.sun = 1; break;  // Sunday
        case 1: day.bits.mon = 1; break;  // Monday
        case 2: day.bits.tue = 1; break;  // Tuesday
        case 3: day.bits.wed = 1; break;  // Wednesday
        case 4: day.bits.thu = 1; break;  // Thursday
        case 5: day.bits.fri = 1; break;  // Friday
        case 6: day.bits.sat = 1; break;  // Saturday
    }

    // Display the raw byte value in hexadecimal format
    printf("Raw value: 0x%02X\n", day.value);

    // Display which day it is by checking each bit individually
    printf("Today is: ");
    if (day.bits.mon) printf("Monday");
    if (day.bits.tue) printf("Tuesday");
    if (day.bits.wed) printf("Wednesday");
    if (day.bits.thu) printf("Thursday");
    if (day.bits.fri) printf("Friday");
    if (day.bits.sat) printf("Saturday");
    if (day.bits.sun) printf("Sunday");
    printf("\n");

    // now, we do the same, but with a switch statement
    printf("Today is (switch): ");
    switch (t->tm_wday) {
        case 0: printf("Sunday"); break;
        case 1: printf("Monday"); break;
        case 2: printf("Tuesday"); break;
        case 3: printf("Wednesday"); break;
        case 4: printf("Thursday"); break;
        case 5: printf("Friday"); break;
        case 6: printf("Saturday"); break;
    }
    printf("\n");

    // Show that the union occupies only 1 byte of memory
    printf("Union size: %zu byte\n", sizeof(WeekdayBits));

    // Show the alignment requirement of the union
    printf("Alignment: %zu bytes\n", _Alignof(WeekdayBits));

    // now, let's change the pragma pack to 1 and see the size again
    // we need to recompile it, because it is a compile-time directive

    return 0;
}
