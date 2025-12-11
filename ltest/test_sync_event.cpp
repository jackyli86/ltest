#include "stdafx.h"

GTEST_TEST(sync, event) {
	int n = 0;
	HANDLE hEventT1 = CreateEvent(nullptr, false, false, nullptr);
	GTEST_ASSERT_NE(hEventT1, nullptr);

	HANDLE hEventT2 = CreateEvent(nullptr, false, false, nullptr);
	GTEST_ASSERT_NE(hEventT2, nullptr);

	boost::thread_group thread_group;

	thread_group.add_thread(new boost::thread([hEventT1, hEventT2]() {

			WaitForSingleObject(hEventT1, INFINITE);
			fmt::println("event1 has signal");

			SetEvent(hEventT2);

		}));

	thread_group.add_thread(new boost::thread([hEventT2]() {

			WaitForSingleObject(hEventT2, INFINITE);

			fmt::println("event2 has signal");

			boost::this_thread::sleep_for(boost::chrono::milliseconds(10));

		}));

	SetEvent(hEventT1);

	thread_group.join_all();


	CloseHandle(hEventT1);
	CloseHandle(hEventT2);
}