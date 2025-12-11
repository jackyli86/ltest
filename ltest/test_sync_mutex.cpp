#include "stdafx.h"

GTEST_TEST(sync, mutex) {
	int n = 0;
	HANDLE hMutex = CreateMutex(nullptr, false, nullptr);
	GTEST_ASSERT_NE(hMutex, nullptr);

	boost::thread_group thread_group;
	thread_group.add_thread(new boost::thread([&hMutex, &n]() {
		for (int i = 0; i < 100; ++i) {
			WaitForSingleObject(hMutex, INFINITE);
			fmt::println("thread: {}, n: {}", 0, ++n);
			ReleaseMutex(hMutex);
			boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
		}

	}));

	thread_group.add_thread(new boost::thread([&hMutex, &n]() {
		for (int i = 0; i < 100; ++i) {
			WaitForSingleObject(hMutex, INFINITE);
			fmt::println("thread: {}, n: {}", 1, ++n);
			ReleaseMutex(hMutex);
			boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
		}
	}));

	thread_group.join_all();

	CloseHandle(hMutex);

	hMutex = nullptr;
}