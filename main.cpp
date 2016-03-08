#include <iostream>

#include "opsys.hpp"

int main(int argc, char **argv)
{
    OpParam fooParams1[3] =
    {
        {OPT_INT, 2},
        {OPT_FLT, 1},
        {OPT_STR}
    };

    // cannot be directly initialized in declaration
    // above, because of the way unions are treated
    // in aggregate initializion
    fooParams1[0].ints[0] = 512;
    fooParams1[0].ints[1] = 256;
    fooParams1[1].flts[0] = 3.4f;
    fooParams1[2].str = "test";

    Op op("op1", ptrFromMember(&Op::FooOpExec1), fooParams1, 3);
    op.execute();

    //------------------------------------------------------------------------------

    OpParam fooParams2[5] =
    {
        { OPT_INT, 2 },
        { OPT_FLT, 1 },
        { OPT_INT, 4 },
        { OPT_FLT, 1 },
        { OPT_STR }
    };

    // cannot be directly initialized in declaration
    // above, because of the way unions are treated
    // in aggregate initializion
    fooParams2[0].ints[0] = 123;
    fooParams2[0].ints[1] = 456;
    fooParams2[1].flts[0] = 6.05f;
    fooParams2[2].ints[0] = 111;
    fooParams2[2].ints[1] = 222;
    fooParams2[2].ints[2] = 333;
    fooParams2[2].ints[3] = 444;
    fooParams2[3].flts[0] = 12.123f;
    fooParams2[4].str = "opmaster";

    Op op2("op2", ptrFromMember(&Op::FooOpExec2), fooParams2, 5);
    op2.execute();

    //------------------------------------------------------------------------------

    OpParam fooParams3[1] =
    {
        { OPT_FLT, 1 }
    };

    // cannot be directly initialized in declaration
    // above, because of the way unions are treated
    // in aggregate initializion
    fooParams3[0].flts[0] = 1.4f;

    Op op3("op3", ptrFromMember(&Op::FooOpExec3), 0, 0);
    op3.execute();

    return 0;
}