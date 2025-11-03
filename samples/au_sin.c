#include <stdio.h>
#include "vd_au.h"

int main(int argc, char const *argv[])
{
    vd_au_init();
    return 0;
}

#define VD_AU_IMPL
#include "vd_au.h"
