#pragma once
#include "be_client.hpp"
#include "be_packet.hpp"
#include <vector>

namespace battleye::delegate
{
	extern battleye::becl_game_data::send_packet_t o_send_packet;

	bool exit();
	void run();
	void command(char* command);
	void received_packet(std::uint8_t* received_packet, std::uint32_t length);
	void on_receive_auth_ticket(std::uint8_t* ticket, std::uint32_t length);
	void add_peer(std::uint8_t* guid, std::uint32_t guid_length);
	void remove_peer(std::uint8_t* guid, std::uint32_t guid_length);

	void respond(std::uint8_t response_index, std::initializer_list<std::uint8_t> data);
	void respond_fragmented(std::uint8_t response_index, battleye::be_fragment fragment, std::initializer_list<std::uint8_t> data);
}