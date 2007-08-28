#include "../raytrace_defs.h"
#include "cxxtest/TestSuite.h"

using namespace std;

class JitterTest : public CxxTest::TestSuite {
    public:
        void test_jitter();
};

void JitterTest::test_jitter() {
    for(int i = 0; i < 413249; i++) {
        float jval = jitter((float)i);
        TS_ASSERT((float)-i < jval && jval <= (float)i);
    }
}

