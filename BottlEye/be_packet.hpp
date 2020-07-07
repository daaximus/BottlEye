#pragma once
#include <cstdint>

namespace battleye
{
	enum packet_id : std::uint8_t
	{
		INIT		= 0x00,
		START		= 0x02,
		REQUEST		= 0x04,
		RESPONSE	= 0x05,
		HEARTBEAT	= 0x09,
	};


#pragma pack(push, 1)
	struct be_fragment
	{
		std::uint8_t count;
		std::uint8_t index;
	};

	struct be_packet_header
	{
		std::uint8_t id;
		std::uint8_t sequence;
	};

	struct be_packet : be_packet_header
	{
		union 
		{
			be_fragment fragment;

			// DATA STARTS AT body[1] IF PACKET IS FRAGMENTED
			struct
			{
				std::uint8_t no_fragmentation_flag;
				std::uint8_t body[0];			
			};
		};

		inline bool fragmented()
		{
			return this->fragment.count != 0x00;
		}
	};
#pragma pack(pop)
}