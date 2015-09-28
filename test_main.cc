#include <iostream>
#include <cassert>

#include "dispatch-c++.h"

void test_dispatch_group()
{
	int a = 0;

	dispatch_group_t group = dispatch_group_create();
	dispatch_queue_t queue = dispatch_queue_create("main_test.dispatch_async", DISPATCH_QUEUE_SERIAL);

	for (int i = 1; i <= 100; i++) {
		dispatch::dispatch_group_async(group, queue, [=, &a] () {
			a += i;
		});
	}

	dispatch_group_wait(group, DISPATCH_TIME_FOREVER);

	dispatch_release(group);

	assert(a == 5050);
}

int test_main(const int argc, const char * argv[], const char * envp[])
{
	test_dispatch_group();

	std::cout << "Success!" << std::endl;

	return 0;
}
