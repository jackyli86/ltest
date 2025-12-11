#include "stdafx.h"

GTEST_TEST(others, memory_barrier) {

	struct tdata {
		bool storable;
		int id;
	};

	do {		
		tdata td{ 0 };
		boost::thread_group thread_group;

		thread_group.add_thread(new boost::thread([&td]()
			{
				td.id = 1;
				td.storable = true;
			})
		);

		thread_group.add_thread(new boost::thread([&td]()
			{
				if (td.storable && td.id == 0) {
					fmt::println("reader error, id = {}", td.id);
				}
			})
		);

		thread_group.join_all();

		boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
	} while (false);

}