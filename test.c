#define VD_IMPL
#define VD_INIX_IMPL
#define VD_INCLUDE_TESTS 1
#define VD_INCLUDE_INTERNAL_TESTS 1

#include "vd.h"
#include "vd_inix.h"

int main(int argc, char const *argv[])
{
    vd_test_main(argc, (char**)argv);
    return 0;
}
