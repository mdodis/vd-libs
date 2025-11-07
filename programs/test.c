#define VD_USE_CRT 1
#define VD_IMPL
#define VD_INIX_IMPL
#define VD_INCLUDE_TESTS 1
#define VD_INCLUDE_INTERNAL_TESTS 0
#define VD_CG_INCLUDE_INTERNAL_TESTS 1
#define VD_CG_INTERNAL_TESTS_VERBOSE 1

#include "vd.h"
#include "vd_inix.h"
#include "vd_cg.h"

int main(int argc, char const *argv[])
{
    VD_UNUSED(argc);
    VD_UNUSED(argv);
    vd_test_main();
    return 0;
}

#define VD_CG_IMPL
#include "vd_cg.h"
