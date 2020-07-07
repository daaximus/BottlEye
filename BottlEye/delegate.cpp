#include "delegate.hpp"
#include "singleton.hpp"
#include "be_packet.hpp"
#include <array>
#include <future>
#include <thread>

battleye::becl_game_data::send_packet_t battleye::delegate::o_send_packet = nullptr;

bool battleye::delegate::exit()
{
	singleton::emulator.console().log("BottlEye exiting.");
	return true;
}

void battleye::delegate::run()
{
	//singleton::emulator.console().log("Executed 'run'");
}

void battleye::delegate::command(char* command)
{
	singleton::emulator.console().log("Executed 'command'");
}

void battleye::delegate::received_packet(std::uint8_t* received_packet, std::uint32_t length)
{
	auto header = reinterpret_cast<battleye::be_packet*>(received_packet);

	switch (header->id)
	{
	case battleye::packet_id::INIT:
	{
		singleton::emulator.console().log("INIT");
		singleton::emulator.console().log_indented<1, true>("Size (bytes)", length);

		auto info_packet = battleye::be_packet{};
		info_packet.id		 = battleye::packet_id::INIT;
		info_packet.sequence = 0x05;

		battleye::delegate::o_send_packet(&info_packet, sizeof(info_packet));
		break;
	}

	case battleye::packet_id::START:
	{
		singleton::emulator.console().log("START");
		singleton::emulator.console().log_indented<1, true>("Size (bytes)", length);

		battleye::delegate::o_send_packet(received_packet, sizeof(battleye::be_packet_header));
		break;
	}

	case battleye::packet_id::REQUEST:
	{
		singleton::emulator.console().log<true>("REQUEST", header->sequence);
		
		// HANDLE PACKET FRAGMENTATION
		if (header->fragmented())
		{
			singleton::emulator.console().log_indented<1, true>("Fragment count", header->fragment.count);
			singleton::emulator.console().log_indented<1, true>("Fragment index", header->fragment.index);
		}

		// IF NOT FRAGMENTED RESPOND IMMEDIATELY, ELSE ONLY RESPOND TO THE LAST FRAGMENT
		const auto respond = !header->fragmented() || (header->fragment.index == header->fragment.count - 1);
		if (!respond)
		{
			singleton::emulator.console().log_indented<1>("Ignoring!");
			return;
		}

		// SEND BACK HEADER
		battleye::delegate::o_send_packet(received_packet, sizeof(battleye::be_packet_header));
		singleton::emulator.console().log_indented<1>("Sending back header...");

		switch (header->sequence)
		{
		case 0x01:
		{
			singleton::emulator.console().log_indented<1>("Replaying!");
		
			battleye::delegate::respond(header->sequence,
				{
					// REDACTED
				});

			break;
		}
		case 0x02:
		{
			singleton::emulator.console().log_indented<1>("Replaying!");
			
			battleye::delegate::respond(header->sequence, 
				{	
					// REDACTED
				});
				
			break;
		}

		default:
		{
			singleton::emulator.console().log_indented<1>("Replying!");
			break;
		}
		}

		break;
	}

	case battleye::packet_id::RESPONSE:
	{
		singleton::emulator.console().log<true>("Acknowledgement of packet", header->sequence);
		break;
	}

	case battleye::packet_id::HEARTBEAT:	
	{
		singleton::emulator.console().log("Heartbeat");
		battleye::delegate::o_send_packet(received_packet, length);
		break;
	}

	default:
	{
		//battleye::delegate::o_send_packet(received_packet, sizeof(battleye::be_packet));

		singleton::emulator.console().log<true>("Unhandled packet", header->id);
		break;
	}
	}
}

void battleye::delegate::on_receive_auth_ticket(std::uint8_t* ticket, std::uint32_t length)
{
	singleton::emulator.console().log("Executed 'on_receive_auth_ticket'");
}

void battleye::delegate::add_peer(std::uint8_t* guid, std::uint32_t guid_length)
{
	singleton::emulator.console().log("Executed 'add_peer'");
}

void battleye::delegate::remove_peer(std::uint8_t* guid, std::uint32_t guid_length)
{
	singleton::emulator.console().log("Executed 'remove_peer'");
}

void battleye::delegate::respond(std::uint8_t response_index, std::initializer_list<std::uint8_t> data)
{
	// SETUP RESPONSE PACKET WITH TWO-BYTE HEADER + NO-FRAGMENTATION TOGGLE

	const auto size = sizeof(battleye::be_packet_header) + sizeof(battleye::be_fragment::count) + data.size();

	auto packet = std::make_unique<std::uint8_t[]>(size);
	auto packet_buffer = packet.get();

	packet_buffer[0] = (battleye::packet_id::RESPONSE);	// PACKET ID
	packet_buffer[1] = (response_index - 1);			// RESPONSE INDEX
	packet_buffer[2] = (0x00);							// FRAGMENTATION DISABLED


	for (size_t i = 0; i < data.size(); i++)
	{
		packet_buffer[3 + i] = data.begin()[i];
	}

	battleye::delegate::o_send_packet(packet_buffer, size);
}

void battleye::delegate::respond_fragmented(std::uint8_t response_index, battleye::be_fragment fragment, std::initializer_list<std::uint8_t> data)
{
	// SETUP RESPONSE PACKET WITH TWO-BYTE HEADER + FRAGMENTATION HEADER
	
	const auto size = sizeof(battleye::packet_id) + sizeof(response_index) + sizeof(fragment) + data.size();
	
	if (size != 0x400 && fragment.index != fragment.count - 1) // ALL FRAGMENTS BUT THE LAST IS 0x400 LONG
	{
		singleton::emulator.console().log_error("Sending too little fragment...");
	}

	auto packet = std::make_unique<std::uint8_t[]>(size);
	auto packet_buffer = packet.get();

	packet_buffer[0] = battleye::packet_id::RESPONSE;	// PACKET ID
	packet_buffer[1] = response_index - 1;				// RESPONSE INDEX
	packet_buffer[2] = fragment.count;					// FRAGMENTATION COUNT
	packet_buffer[3] = fragment.index;					// FRAGMENTATION INDEX

	for (size_t i = 0; i < data.size(); i++)
	{
		packet_buffer[4 + i] = data.begin()[i];
	}

	// DEBUG PRINT
	battleye::delegate::o_send_packet(packet_buffer, size);
}