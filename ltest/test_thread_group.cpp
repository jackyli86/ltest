#include "stdafx.h"


GTEST_TEST(boost, thread_group) {
	boost::thread_group thread_group;
	boost::mutex mutex;
	boost::atomic<int> counter(0);
	for (auto i = 0; i < 5; i++) {
		thread_group.add_thread(new boost::thread([i, &counter, &mutex]() {

			for (auto j = 0; j < 2; ++j) {
				auto c = ++counter;

				{
					boost::lock_guard<boost::mutex> lock(mutex);
					fmt::println("thread {} running, counter = {}", i, c);
				}

				boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
			}

		}));
	}

	thread_group.join_all();

	GTEST_EXPECT_TRUE(true);
}