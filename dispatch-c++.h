#include <dispatch/dispatch.h>
#include <functional>

namespace dispatch {
	void dispatch_after(dispatch_time_t when, dispatch_queue_t queue, const std::function<void (void)> &func);

	void dispatch_apply(size_t iterations, dispatch_queue_t queue, const std::function<void (size_t)> &func);

	void dispatch_async(dispatch_queue_t queue, const std::function<void (void)> &func);
	void dispatch_sync(dispatch_queue_t queue,  const std::function<void (void)> &func);

	void dispatch_group_async(dispatch_group_t group, dispatch_queue_t queue, const std::function<void (void)> &func);
	void dispatch_group_notify(dispatch_group_t group, dispatch_queue_t queue, const std::function<void (void)> &func);

	void dispatch_once(dispatch_once_t * predicate, const std::function<void (void)> &func);
}
