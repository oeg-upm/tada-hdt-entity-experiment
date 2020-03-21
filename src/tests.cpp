#include <gtest/gtest.h>
#include "t2dv2.h"
#include<tada_hdt_entity/entity.h>

namespace {


    TEST(T2Dv2, Hello) {
        T2Dv2* ea = new T2Dv2();
        ASSERT_NE(ea, nullptr);
        delete ea;
    }
}//namespace




int main(int argc, char** argv) {
    //    ttl_to_hdt(input_file);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
