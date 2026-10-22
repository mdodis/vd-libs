#define VD_USE_CRT 1
#define VD_INCLUDE_TESTS 1
#include "vd.h"
#include "vd_cg.h"

#ifndef VD_CG_INTERNAL_TESTS_VERBOSE
#   define VD_CG_INTERNAL_TESTS_VERBOSE 0
#endif

static void vd_cg__flog4x4(F4x4 *m)
{
#if VD_CG_INTERNAL_TESTS_VERBOSE
    VD_TEST_LOG("| % 4.3f % 4.3f % 4.3f % 4.3f |\n"
                "| % 4.3f % 4.3f % 4.3f % 4.3f |\n"
                "| % 4.3f % 4.3f % 4.3f % 4.3f |\n"
                "| % 4.3f % 4.3f % 4.3f % 4.3f |\n",
                m->c[0].e[0], m->c[1].e[0], m->c[2].e[0], m->c[3].e[0],
                m->c[0].e[1], m->c[1].e[1], m->c[2].e[1], m->c[3].e[1],
                m->c[0].e[2], m->c[1].e[2], m->c[2].e[2], m->c[3].e[2],
                m->c[0].e[3], m->c[1].e[3], m->c[2].e[3], m->c[3].e[3]);
#endif
}

static void vd_cg__flog4(F4 *v) {
#if VD_CG_INTERNAL_TESTS_VERBOSE
    VD_TEST_LOG("(%3.3f %3.3f %3.3f %3.3f)", v->x, v->y, v->z, v->w);
#endif
}

static void vd_cg__flog3(F3 *v) {
#if VD_CG_INTERNAL_TESTS_VERBOSE
    VD_TEST_LOG("(%3.3f %3.3f %3.3f)", v->x, v->y, v->z);
#endif
}

VD_TEST("fabs") {
    VD_TEST_EQ("-0.f  ==  0.f", fabs(-0.f), 0.f);
    VD_TEST_EQ("+0.f  ==  0.f", fabs(+0.f), 0.f);
    VD_TEST_EQ("-10.f == 10.f", fabs(-10.f), 10.f);
    VD_TEST_EQ("+10.f == 10.f", fabs(+10.f), 10.f);
    VD_TEST_OK();
}

VD_TEST("feq") {
    VD_TEST_TRUE("0.f  ==  0.f", feq(0.f, 0.f));
    VD_TEST_TRUE("-0.f == +0.f", feq(-0.f, +0.f));
    VD_TEST_TRUE("+1.f == +1.f", feq(+1.f, +1.f));
    VD_TEST_FALSE("-1.f != +1.f", feq(-1.f, +1.f));

    VD_TEST_TRUE("(0,0,1) == (0,0,1)", feq3(fm3(0,0,1), fm3(0,0,1)));
    VD_TEST_TRUE("(1,1,1) == (1,1,1)", feq3(fall3(1), fall3(1)));

    VD_TEST_TRUE("(0,0,1,1) == (0,0,1,1)", feq4(fm4(0,0,1,1), fm4(0,0,1,1)));
    VD_TEST_OK();
}

VD_TEST("Affine Transform/Matrix Translation") {
    F4x4 m1 = ftranslation4x4(fm3(1,2,3));
    F4x4 m2 = ftranslation4x4(fm3(4,5,6));

    vd_cg__flog4x4(&m1);
    vd_cg__flog4x4(&m2);

    F4x4 m = fmul4x4(&m1, &m2);
    vd_cg__flog4x4(&m);
    VD_TEST_TRUE("Result Matrix Transformed", feq4(m.c[3], fm4(5,7,9,1)));

    VD_TEST_OK();
}

VD_TEST("Affine Transform/Vector Translation") {
    F4x4 m = ftranslation4x4(fm3(1,2,3));
    vd_cg__flog4x4(&m);
    vd_cg__flog4(&m.c[3]);

    F4 v = fm4(1,2,3,1);
    F3 tv3 = fmul4x4_3(&m, v.xyz);
    F4 tv  = fmul4x4_4(&m, v);

    vd_cg__flog4(&tv);
    VD_TEST_TRUE("Vectors Match", feq4(tv, fm4(2,4,6,1)));
    VD_TEST_TRUE("Vectors Match", feq3(tv3, fm3(2,4,6)));
    VD_TEST_OK();
}

VD_TEST("Affine Transform/Vector Rotation (Y/YAW)") {
    F4x4 m = frotation_yaw4x4(fdeg2rad(90.f));
    vd_cg__flog4x4(&m);

    F4 v = fm4(1,0,1,1);

    F4 tv = fmul4x4_4(&m, v);
    vd_cg__flog4(&tv);

    VD_TEST_TRUE("Vector rotated on XZ Plane with Math Winding of 90deg", feq4(tv, fm4(-1,0,1,1)));
    VD_TEST_OK();
}

VD_TEST("Affine Transform/Matrix Rotation (Y/YAW)") {
    F4x4 m = frotation_yaw4x4(fdeg2rad(90.f));
    vd_cg__flog4x4(&m);

    F4x4 c = fidentity4x4();
    c.c[3].xyz = fm3(5,5,5);

    F4x4 tm = fmul4x4(&m, &c);
    vd_cg__flog4x4(&tm);
    VD_TEST_OK();
}

VD_TEST("Perspective Projection") {
    F4x4 projection = fperspective4x4(fdeg2rad(60.f), 1.f, 0.01f, 10.f);

    vd_cg__flog4x4(&projection);

    F4 v = fm4(0,0,10,1);
    F4 p = fmul4x4_4(&projection, v);
    F4 c = fdiv4(p, p.w);

    vd_cg__flog4(&p);
    vd_cg__flog4(&c);
    F4 t = fm4(0,0,1,1);
    vd_cg__flog4(&t);

    VD_TEST_TRUE("Point is Projected Correctly", feq4(c, t));

    VD_TEST_OK();
}

VD_TEST("Look At Matrix") {
    F4x4 lookat = flookat4x4(fm3(0,0,-10), fm3(0,0,1), fm3(0,1,0));
    vd_cg__flog4x4(&lookat);
    VD_TEST_OK();
}

VD_TEST("Quaternion - Vector Multiplication") {
    FQuat q = fidentityquat();
    F3 v = fmulquat_3(q, fm3(0,0,1));
    vd_cg__flog3(&v);
    VD_TEST_OK();
}

int main(int argc, char const *argv[])
{
    VD_UNUSED(argc);
    VD_UNUSED(argv);
    vd_test_main();
    return 0;
}

#define VD_IMPL
#include "vd.h"

#define VD_CG_IMPL
#include "vd_cg.h"
