#include <unordered_map>

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

	queue::queue() : priQueue(nullptr)
	{
	}

	queue::queue(const std::string &label, const attr attr)
	{
		static const std::unordered_map<queue::attr, dispatch_queue_attr_t> mapAttr = {
			{attr::SERIAL,     DISPATCH_QUEUE_SERIAL},
			{attr::CONCURRENT, DISPATCH_QUEUE_CONCURRENT}
		};

		this->priQueue = dispatch_queue_create(label.c_str(), mapAttr.at(attr));
	}

	queue::queue(const queue &q)
	{
		this->priQueue = q.priQueue;

		dispatch_retain(this->priQueue);
	}

	queue::queue(queue &&q)
	{
		this->priQueue = q.priQueue;

		q.priQueue = nullptr;
	}

	queue::~queue()
	{
		if (this->priQueue != nullptr) {
			dispatch_release(this->priQueue);
		}
	}


	std::string queue::label()
	{
		return dispatch_queue_get_label(this->priQueue);
	}

	queue queue::globalQueue(const priority priority)
	{
		static const std::unordered_map<queue::priority, long> mapPriority = {
			{priority::HIGH,       DISPATCH_QUEUE_PRIORITY_HIGH},
			{priority::DEFAULT,    DISPATCH_QUEUE_PRIORITY_DEFAULT},
			{priority::LOW,        DISPATCH_QUEUE_PRIORITY_LOW},
			{priority::BACKGROUND, DISPATCH_QUEUE_PRIORITY_BACKGROUND},
		};

		queue q;

		 q.priQueue = dispatch_get_global_queue(mapPriority.at(priority), 0);

		return q;
	}

	queue queue::mainQueue()
	{
		queue q;

		q.priQueue = dispatch_get_main_queue();

		return q;
	}

	void queue::apply(const size_t iterations, const std::function<void (size_t)> &func)
	{
		dispatch_apply(iterations, this->priQueue, func);
	}


	void queue::async(const std::function<void (void)> &func)
	{
		dispatch_async(this->priQueue, func);
	}

	void queue::sync(const std::function<void (void)> &func)
	{
		dispatch_sync(this->priQueue, func);
	}

	void queue::suspend()
	{
		dispatch_suspend(this->priQueue);
	}

	void queue::resume()
	{
		dispatch_resume(this->priQueue);
	}

	group::group() : priGroup(dispatch_group_create())
	{
	}

	group::group(const group &grp)
	{
		this->priGroup = grp.priGroup;

		dispatch_retain(this->priGroup);
	}

	group::group(group &&grp)
	{
		this->priGroup = grp.priGroup;

		grp.priGroup = nullptr;
	}

	group::~group()
	{
		if (this->priGroup != nullptr) {
			dispatch_release(this->priGroup);
		}
	}

	void group::enter()
	{
		dispatch_group_enter(this->priGroup);
	}

	void group::leave()
	{
		dispatch_group_leave(this->priGroup);
	}

	long group::wait(dispatch_time_t timeout)
	{
		return dispatch_group_wait(this->priGroup, timeout);
	}

	void group::notify(queue &q, const std::function<void (void)> &func)
	{
		dispatch_group_notify(this->priGroup, q.priQueue, func);
	}

	void group::async(queue &q, const std::function<void (void)> &func)
	{
		dispatch_group_async(this->priGroup, q.priQueue, func);
	}

	semaphore::semaphore() : priSemaphore(dispatch_semaphore_create(0))
	{
	}

	semaphore::semaphore(long count) : priSemaphore(dispatch_semaphore_create(count))
	{
	}

	semaphore::semaphore(const semaphore &sema)
	{
		this->priSemaphore = sema.priSemaphore;

		dispatch_retain(this->priSemaphore);
	}

	semaphore::semaphore(semaphore &&sema)
	{
		this->priSemaphore = sema.priSemaphore;

		sema.priSemaphore = nullptr;
	}

	semaphore::~semaphore()
	{
		if (this->priSemaphore != nullptr) {
			dispatch_release(this->priSemaphore);
		}
	}

	long semaphore::signal()
	{
		return dispatch_semaphore_signal(this->priSemaphore);
	}

	long semaphore::wait(dispatch_time_t timeout)
	{
		return dispatch_semaphore_wait(this->priSemaphore, timeout);
	}
}
