/*
 * unions_simple.c
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

// This is a union example in C
#include <stdio.h>

// a union can hold different data types in the same memory location
// but only one member can be used at a time
union Number {
    int   i;    // integer member
    float f;    // float member
};

// let's define the same structure for comparison
struct NumberStruct {
    int   i;    // integer member
    float f;    // float member
};

int unions_main(void) {
    union Number n;     // variable of union type
    
    n.i = 42;           // set integer member

    printf("As integer: %d\n", n.i);

    n.f = 3.14f;        // now the float member is the “active” one
    printf("As float: %f\n", n.f);

    // please observe what happens with the intger member now
    printf("As integer (after setting float): %d\n", n.i);

    // Now let's see how the struct behaves
    struct NumberStruct ns; // variable of struct type
    ns.i = 42;             // set integer member
    printf("Struct as integer: %d\n", ns.i);

    ns.f = 3.14f;         // set float member
    printf("Struct as float: %f\n", ns.f);

    // both members retain their values in the struct
    printf("Struct as integer (after setting float): %d\n", ns.i);
}