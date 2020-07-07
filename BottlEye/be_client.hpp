#pragma once
#include <cstdint>

namespace battleye
{
    enum instance_status
    {
        NONE,
        NOT_INITIALIZED,
        SUCCESSFULLY_INITIALIZED,
        DESTROYING,
        DESTROYED
    };

    struct becl_game_data
    {
        char* game_version;
        std::uint32_t address;
        std::uint16_t port;

        // FUNCTIONS
        using print_message_t = void(*)(char* message);
        print_message_t print_message;

        using request_restart_t = void(*)(std::uint32_t reason);
        request_restart_t request_restart;

        using send_packet_t = void(*)(void* packet, std::uint32_t length);
        send_packet_t send_packet;

        using disconnect_peer_t = void(*)(std::uint8_t* guid, std::uint32_t guid_length, char* reason);
        disconnect_peer_t disconnect_peer;
    };

    struct becl_be_data
    {
        using exit_t = bool(*)();
        exit_t exit;

        using run_t = void(*)();
        run_t run;

        using command_t = void(*)(char* command);
        command_t command;

        using received_packet_t = void(*)(std::uint8_t* received_packet, std::uint32_t length);
        received_packet_t received_packet;

        using on_receive_auth_ticket_t = void(*)(std::uint8_t* ticket, std::uint32_t length);
        on_receive_auth_ticket_t on_receive_auth_ticket;

        using add_peer_t = void(*)(std::uint8_t* guid, std::uint32_t guid_length);
        add_peer_t add_peer;

        using remove_peer_t = void(*)(std::uint8_t* guid, std::uint32_t guid_length);
        remove_peer_t remove_peer;
    };
}