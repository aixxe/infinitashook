#pragma once

#include <zmq.hpp>
#include "../message/base_message.h"

namespace infinitashook
{
	class event_server
	{
		public:
			event_server();

			void bind(const char* addr);
			void publish(base_message* message);
			void close();
		private:
			zmq::context_t _context;
			zmq::socket_t _socket;
	};
}