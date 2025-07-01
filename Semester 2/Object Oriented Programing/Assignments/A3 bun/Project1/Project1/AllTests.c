#include "AllTests.h"
#include "dynamic_array_test.h"
#include "product_test.h"
#include "services_test.h"
#include <stdio.h>

void testAll()
{
    testDynamicArrayAll();
    testProductAll();
    testAllService();
}
