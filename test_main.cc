#include <iostream>
#include <cassert>

#include "dispatch-c++.h"

void test_dispatch_group()
{
	int a = 0;

	dispatch::group group;
	dispatch::queue queue("main_test.dispatch_async", dispatch::queue::attr::CONCURRENT);

	for (int i = 1; i <= 100; i++) {
		group.async(queue, [=, &a] () {
			a += i;
		});
	}

	group.wait(DISPATCH_TIME_FOREVER);

	assert(a == 5050);
}

int test_main(const int argc, const char * argv[], const char * envp[])
{
	test_dispatch_group();

	std::cout << "Success!" << std::endl;

	return 0;
}
