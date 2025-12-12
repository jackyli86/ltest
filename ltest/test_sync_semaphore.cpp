#include "stdafx.h"

GTEST_TEST(sync, semaphore) {
    int n = 0;
    HANDLE hSemaphore = CreateSemaphore(nullptr, 10, 10, nullptr);
    GTEST_ASSERT_NE(hSemaphore, nullptr);

    // 临界区
    RTL_CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);


    boost::thread_group thread_group;
    for (int i = 0; i < 32; ++i) {
        thread_group.add_thread(new boost::thread([&hSemaphore, &cs, &n, i]() {
                for (int j = 0; j < 10; ++j) {
                    WaitForSingleObject(hSemaphore, INFINITE);
                    {
                        EnterCriticalSection(&cs);
                        fmt::println("thread: {}, n: {}", i, ++n);
                        LeaveCriticalSection(&cs);
                    }
                    ReleaseSemaphore(hSemaphore, 1, nullptr);
                    boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
                }
            }));
    }

    thread_group.join_all();

    DeleteCriticalSection(&cs);

    CloseHandle(hSemaphore);

    hSemaphore = nullptr;
}