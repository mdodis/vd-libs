/**
 * ryu.c - Standalone implementation of https://dl.acm.org/doi/10.1145/3192366.3192369, by Michael Dodis <michaeldodisgr@gmail.com>
 * ---------------------------------------------------------------------------------------------------------------------
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <https://unlicense.org/>
 * ---------------------------------------------------------------------------------------------------------------------
 */
#include <stdio.h>

int cv_float_to_str(float x, char *buf)
{
    int count = 0;

    unsigned int valu = *((unsigned int*)&x);
    unsigned int sign        = valu >> 31;
    unsigned int exponent    = (valu & 0x7F800000) >> 23;
    unsigned int mantissa    = (valu & 0x007FFFFF);
    unsigned int unbiasedexp = exponent - 127;
    int subnormal = exponent == 0;

    if (valu == 0x7F800000) {
        const char *str = "inf";
        while (*str) { 
            buf[count++] = *str++;
        }
    } else if (valu == 0xFF800000) {
        const char *str = "-inf";
        while (*str) { 
            buf[count++] = *str++;
        }
    } else if (exponent == 0xFF && mantissa != 0) {
        const char *str = "nan";
        while (*str) { 
            buf[count++] = *str++;
        }
    } else if (sign && exponent == 0 && mantissa == 0) {
        const char *str = "-0";
        while (*str) { 
            buf[count++] = *str++;
        }
    } else if (!sign && exponent == 0 && mantissa == 0) {
        const char *str = "0";
        while (*str) { 
            buf[count++] = *str++;
        }
    }

    int e = exponent;
    unsigned int m = mantissa;

    // Step 1. Decode floating point number & unify normalized and subnormal cases.
    //      { 2^len(m) + m, if e != 0
    // mf = {
    //      { m,            if e == 0
    unsigned int mf = (e != 0)
        ? (1u << 23) | m
        : m;
    //      { e - bias - len(m), if e != 0
    // ef = {
    //      { 1 - bias - len(m), if e == 0
    int ef = (e != 0)
        ? e - 127 - 23
        : 1 - 127 - 23;

    // Step 2. Determine the interval of information-preserving outputs.
    int e2 = ef - 2;

    //           { 1, if m == (0..0)binary and e > (0..01)binary
    // u = 4mf - {
    //           { 2, otherwise
    unsigned int fu = 4 * mf - ((m == 0) && (e > 1)) ? 1 : 2;

    // v = 4mf
    unsigned int fv = 4 * mf;

    // w = 4mf + 2
    unsigned int fw = 4 * mf + 2;

    // Step 3. Convert (fu,fv,fw) * 2^e2 to a decimal power base.
    int e10;
    if (e2 >= 0) {
        // We still need to know by how far the decimal point will shift. We do this by computing
        // q = floor(log10(pow(2,e2))) == floor(e2log10(2))
        // The following is a good approximation until 2^1651
        unsigned int q = (((unsigned int)e2) * 78913) >> 18;
        e10 = (int)q;

    } else {
        e10 = e2;

    }

    buf[count++] = 0;

    return count;
}

void test_float(float x) {
    char buf[46];
    cv_float_to_str(x, buf);

    printf("|%-32f|%-32s|\n", x, buf);
}

int main(int argc, char const *argv[])
{

    printf("|%-32s|%-32s|\n", "printf", "ryu");
    puts("|--------------------------------|--------------------------------|");
    unsigned int inf = 0x7F800000;
    unsigned int minf = 0xFF800000;
    unsigned int nan = 0x7FFBDFFF;
    test_float(*((float*)(&inf)));
    test_float(*((float*)(&minf)));
    test_float(*((float*)(&nan)));
    test_float(0.f);
    test_float(-0.f);

    return 0;
}