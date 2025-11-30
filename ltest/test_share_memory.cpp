#include "stdafx.h"

constexpr auto SM_NAME = "gsm";
constexpr auto PLAYER_NUMBER = 10;

GTEST_TEST(others, share_memory) {
	struct TablePlayer {
		int id;
		int age;
		char name[32];
	};
	auto handle_sharememory = ::CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, sizeof(TablePlayer) * PLAYER_NUMBER, SM_NAME);

	boost::thread_group threads;
	threads.add_thread(new boost::thread(
		[]() {
			auto handle_sharememory = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, SM_NAME);
			auto table_players = (TablePlayer*)::MapViewOfFile(
				handle_sharememory,               // 你好
				FILE_MAP_ALL_ACCESS,     
				0, 0,                    
				sizeof(TablePlayer) * PLAYER_NUMBER                    
			);

			auto running = true;
			while (running) {
				running = false;

				for (auto i = 0; i < PLAYER_NUMBER; i++) {
					auto player = table_players + i;
					if (player->id <= 0) {
						running = true;
						break;
					}
					fmt::println("index {}, id {}, age {}, name {}", i, player->id, player->age, player->name);
				}

				boost::this_thread::sleep_for(boost::chrono::milliseconds(500));

			}

			::UnmapViewOfFile(table_players);
			::CloseHandle(handle_sharememory);
		})
	);


	threads.add_thread(new boost::thread(
		[]() {
			auto handle_sharememory = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, SM_NAME);
			auto table_players = (TablePlayer*)::MapViewOfFile(
				handle_sharememory,               // 文件映射对象
				FILE_MAP_ALL_ACCESS,     // 读写权限
				0, 0,                    // 偏移量
				sizeof(TablePlayer) * PLAYER_NUMBER                    // 映射大小
			);

			auto running = true;
			while (running) {
				running = false;
				for (auto i = 0; i < PLAYER_NUMBER; i++) {
					auto player = table_players + i;
					if (player->id <= 0) {						
						player->age = i + 1;
						strcpy_s(player->name, fmt::format("name{}", i + 1).c_str());
						player->id = i + 1;
						running = true;
						break;
					}
				}

				boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
			}

			::UnmapViewOfFile(table_players);
			::CloseHandle(handle_sharememory);
		})
	);

	threads.join_all();

	::CloseHandle(handle_sharememory);

}