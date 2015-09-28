#include "dispatch-c++.h"

/*
 * For synchronous dispatch.
 */
static void call_func_void_void(void * context)
{
	std::function<void (void)> &func = *static_cast<std::function<void (void)> *>(context);

	func();
}

/*
 * For asynchronous dispatch.
 */
static void call_funcptr_void_void(void * context)
{
	std::function<void (void)> * func_ptr = static_cast<std::function<void (void)> *>(context);

	(*func_ptr)();

	delete func_ptr;
}

/*
 * For synchronous dispatch.
 */
static void call_func_void_size_t(void * context, size_t idx)
{
	std::function<void (size_t)> &func = *static_cast<std::function<void (size_t)> *>(context);

	func(idx);
}

namespace dispatch {
	void dispatch_after(dispatch_time_t when, dispatch_queue_t queue, const std::function<void (void)> &func)
	{
		std::function<void (void)> * func_ptr = new std::function<void (void)>(func);

		dispatch_after_f(when, queue, static_cast<void *>(func_ptr), call_funcptr_void_void);
	}

	void dispatch_apply(size_t iterations, dispatch_queue_t queue, const std::function<void (size_t)> &func)
	{
		dispatch_apply_f(iterations, queue, static_cast<void *>(const_cast<std::function<void (size_t)> *>(&func)), call_func_void_size_t);
	}

	void dispatch_async(dispatch_queue_t queue, const std::function<void (void)> &func)
	{
		std::function<void (void)> * func_ptr = new std::function<void (void)>(func);

		dispatch_async_f(queue, static_cast<void *>(func_ptr), call_funcptr_void_void);
	}
	void dispatch_sync(dispatch_queue_t queue,  const std::function<void (void)> &func)
	{
		dispatch_sync_f(queue, static_cast<void *>(const_cast<std::function<void (void)> *>(&func)), call_func_void_void);
	}

	void dispatch_group_async(dispatch_group_t group, dispatch_queue_t queue, const std::function<void (void)> &func)
	{
		std::function<void (void)> * func_ptr = new std::function<void (void)>(func);

		dispatch_group_async_f(group, queue, static_cast<void *>(func_ptr), call_funcptr_void_void);
	}
	void dispatch_group_notify(dispatch_group_t group, dispatch_queue_t queue, const std::function<void (void)> &func)
	{
		std::function<void (void)> * func_ptr = new std::function<void (void)>(func);

		dispatch_group_notify_f(group, queue, static_cast<void *>(func_ptr), call_funcptr_void_void);
	}

	void dispatch_once(dispatch_once_t * predicate, const std::function<void (void)> &func)
	{
		dispatch_once_f(predicate, static_cast<void *>(const_cast<std::function<void (void)> *>(&func)), call_funcptr_void_void);
	}
}
