#include <cstdint>
#include "be_client.hpp"
#include "singleton.hpp"

extern "C"
{
	// BEClient_x64!Init
	__declspec(dllexport)
	battleye::instance_status Init(std::uint64_t integration_version,
								   battleye::becl_game_data* game_data,
								   battleye::becl_be_data* client_data)
	{
		singleton::emulator.console().log("Initialising BottlEye!");
		singleton::emulator.console().log_indented<1>("Brought to you by: https://secret.club/");
		singleton::emulator.setup_battleye(game_data, client_data);

		return battleye::instance_status::SUCCESSFULLY_INITIALIZED;
	}

	// BEClient_x64!GetVer
	__declspec(dllexport)
	std::uint32_t GetVer()
	{
		return 0xFF;
	}
}


std::uint32_t __stdcall DllMain(const void* mod_instance		[[maybe_unused]], 
								const std::uint32_t call_reason [[maybe_unused]], 
								const void* reserved			[[maybe_unused]])
{
	return 1;
}