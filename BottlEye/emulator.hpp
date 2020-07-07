#pragma once
#include "loggr.hpp"
#include "be_client.hpp"

namespace battleye
{
	class emulator
	{
	public:
		emulator() : m_logger() {}

		void setup_battleye(battleye::becl_game_data* game_data,
							battleye::becl_be_data* client_data);

		inline loggr& console()
		{
			return this->m_logger;
		}

	private:
		loggr m_logger;

	};
}