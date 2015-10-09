#include <dispatch/dispatch.h>
#include <functional>
#include <string>

namespace dispatch {

	void dispatch_after(dispatch_time_t when, dispatch_queue_t queue, const std::function<void (void)> &func);

	void dispatch_apply(size_t iterations, dispatch_queue_t queue, const std::function<void (size_t)> &func);

	void dispatch_async(dispatch_queue_t queue, const std::function<void (void)> &func);
	void dispatch_sync(dispatch_queue_t queue,  const std::function<void (void)> &func);

	void dispatch_group_async(dispatch_group_t group, dispatch_queue_t queue, const std::function<void (void)> &func);
	void dispatch_group_notify(dispatch_group_t group, dispatch_queue_t queue, const std::function<void (void)> &func);

	void dispatch_once(dispatch_once_t * predicate, const std::function<void (void)> &func);

	class queue {
		friend class group;
	public:
		enum class attr {
			SERIAL, CONCURRENT,
		};

		queue();
		queue(const std::string &label, const attr attr);
		queue(const queue &q);
		queue(queue &&q);

		virtual ~queue();

		std::string label();

		enum class priority {
			HIGH, DEFAULT, LOW, BACKGROUND,
		};

		static queue globalQueue(const priority priority);		// assert flags == 0 now

		static queue mainQueue();

		void apply(const size_t iterations, const std::function<void (size_t)> &func);

		void async(const std::function<void (void)> &func);
		void sync(const std::function<void (void)> &func);

		void suspend();
		void resume();

	private:
		dispatch_queue_t priQueue;
	};

	class group {
	public:
		group();
		group(const group &grp);
		group(group &&grp);

		virtual ~group();

		void enter();
		void leave();

		long wait(dispatch_time_t timeout);

		void notify(queue &q, const std::function<void (void)> &func);

		void async(queue &q, const std::function<void (void)> &func);

	private:
		dispatch_group_t priGroup;
	};

	class semaphore {
	public:
		semaphore();
		semaphore(long count);
		semaphore(const semaphore &sema);
		semaphore(semaphore &&sema);

		virtual ~semaphore();

		long signal();
		long wait(dispatch_time_t timeout);

	private:
		dispatch_semaphore_t priSemaphore;
	};
}
