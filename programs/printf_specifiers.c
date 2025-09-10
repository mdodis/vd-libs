#include <stdio.h>

#define _STRINGIZE(x) #x
#define STRINGIZE(x) _STRINGIZE(x)
#define PRINTF(fmt, ...) printf("- \"%s\"\n", fmt " " STRINGIZE(__VA_ARGS__)); printf(fmt "\n", __VA_ARGS__)

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    PRINTF("%d", 1);

    puts("                                                                                              RIGHT JUSTIFY");
    puts("-----------------------------------------------------------------------------------------------------------");
    PRINTF("%3d hello", 1);
    PRINTF("%3d hello", 123);
    PRINTF("%3d hello", 12345);
    PRINTF("%03d hello", 1);
    puts("-----------------------------------------------------------------------------------------------------------");
    puts("");

    puts("                                                                                               LEFT JUSTIFY");
    puts("-----------------------------------------------------------------------------------------------------------");
    PRINTF("%-3d hello", 1);
    PRINTF("%-3d hello", 123);
    PRINTF("%-3d hello", 12345);
    puts("-----------------------------------------------------------------------------------------------------------");
    return 0;
}