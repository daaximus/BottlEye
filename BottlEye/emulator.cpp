#include "emulator.hpp"
#include "delegate.hpp"

void battleye::emulator::setup_battleye(battleye::becl_game_data* game_data,
										battleye::becl_be_data* client_data)
{
	this->console().log("Setting up game_data...");
	this->console().log_indented<1>("Game version", game_data->game_version);
	this->console().log_indented<1, true>("Address", game_data->address);
	this->console().log_indented<1, true>("Port", game_data->port);

	// CACHE RELEVANT FUNCTIONS
	battleye::delegate::o_send_packet	= game_data->send_packet;

	// SETUP CLIENT STRUCTURE
	client_data->exit					= battleye::delegate::exit;
	client_data->run					= battleye::delegate::run;
	client_data->command				= battleye::delegate::command;
	client_data->received_packet		= battleye::delegate::received_packet;
	client_data->on_receive_auth_ticket = battleye::delegate::on_receive_auth_ticket;
	client_data->add_peer				= battleye::delegate::add_peer;
	client_data->remove_peer			= battleye::delegate::remove_peer;

	this->console().log_indented<1>("Done!");
}
