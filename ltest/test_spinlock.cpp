#include "stdafx.h"


GTEST_TEST(others, spinlock) {
	struct tdata {
		volatile long lock;
		int id;
		int age;
	};

	tdata td{ 0 };

	boost::thread_group thread_group;
	thread_group.add_thread(new boost::thread([&td]() 
		{
			do {
				// lock
				while (InterlockedCompareExchange(&td.lock, 1L, 0L) != 0L) {
					boost::this_thread::sleep_for(boost::chrono::milliseconds(0));
				}

				td.id += 1;
				td.age += 1;
				fmt::print("writer, id = {}, age = {}", td.id, td.age);

				// unlock
				InterlockedExchange(&td.lock, 0L);

				boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
			} while (true);
		})
	);

	thread_group.add_thread(new boost::thread([&td]() 
		{
			do {
				// lock
				while (InterlockedCompareExchange(&td.lock, 1L, 0L) != 0L) {
					boost::this_thread::sleep_for(boost::chrono::milliseconds(0));
				}

				fmt::print("reader, id = {}, age = {}", td.id, td.age);

				// unlock
				InterlockedExchange(&td.lock, 0L);

				boost::this_thread::sleep_for(boost::chrono::milliseconds(10));

			} while (true);
		})
	);

	thread_group.join_all();
}