#include "../util/log.h"
#include "event_server.h"

namespace infinitashook
{
	event_server::event_server()
	{
		_socket = zmq::socket_t(_context, zmq::socket_type::pub);
	}

	void event_server::bind(const char* addr)
	{
		log_info("Binding event server to '%s'..", addr);

		_socket.bind(addr);
	}

	void event_server::publish(base_message* message)
	{
		const auto str = message->dump();

		log_debug(">> '%s'", str.c_str());

		_socket.send(zmq::message_t { str.c_str(), str.size() }, zmq::send_flags::none);
	}

	void event_server::close()
	{
		_socket.close();
		_context.close();
	}
}