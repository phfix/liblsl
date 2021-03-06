#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include "api_config.h"
#include <boost/system/system_error.hpp>
#include <boost/asio/detail/chrono.hpp>

namespace lsl {
	inline lslboost::asio::chrono::milliseconds timeout_sec(double timeout_seconds) {
		return lslboost::asio::chrono::milliseconds(static_cast<unsigned int>(1000*timeout_seconds));
	}

    /// Bind a socket (or acceptor) to a free port in the configured port range or throw an error otherwise.
	template <class Socket, class Protocol>
	uint16_t bind_port_in_range(Socket &sock, Protocol protocol) {
		for (int k=0,e=api_config::get_instance()->port_range(); k<e; k++) {
			try {
				sock.bind(typename Protocol::endpoint(protocol,(uint16_t)(k + api_config::get_instance()->base_port())));
				return k + api_config::get_instance()->base_port();
			} catch (lslboost::system::system_error &) { /* port occupied */ }
		}
		if (api_config::get_instance()->allow_random_ports()) {
			for (int k=0; k < 100; ++k) {
				uint16_t port = 1025 + rand()%64000;
				try {
					sock.bind(typename Protocol::endpoint(protocol,port));
					return port;
				} catch (lslboost::system::system_error &) { /* port occupied */ }
			}
		}
		throw std::runtime_error(
			"All local ports were found occupied. You may have more open outlets on this machine "
			"than your PortRange setting allows (see "
			"https://labstreaminglayer.readthedocs.io/info/network-connectivity.html) or you "
			"have a problem with your network configuration.");
	}

	/// Bind to and listen at a socket (or acceptor) on a free port in the configured port range or throw an error otherwise.
	template <class Socket, class Protocol>
	uint16_t bind_and_listen_to_port_in_range(Socket &sock, Protocol protocol, int backlog) {
		for (int k=0,e=api_config::get_instance()->port_range(); k<e; k++) {
			try {
				sock.bind(typename Protocol::endpoint(protocol,(uint16_t)(k + api_config::get_instance()->base_port())));
				sock.listen(backlog);
				return k + api_config::get_instance()->base_port();
			} catch (lslboost::system::system_error &) { /* port occupied */ }
		}
		if (api_config::get_instance()->allow_random_ports()) {
			for (int k = 0; k < 100; ++k) {
				uint16_t port = 1025 + rand()%64000;
				try {
					sock.bind(typename Protocol::endpoint(protocol,port));
					sock.listen(backlog);
					return port;
				} catch (lslboost::system::system_error &) { /* port occupied */ }
			}
		}
		throw std::runtime_error(
			"All local ports were found occupied. You may have more open outlets on this machine "
			"than your PortRange setting allows (see "
			"https://labstreaminglayer.readthedocs.io/info/network-connectivity.html) or you "
			"have a problem with your network configuration.");
	}

	/// Measure the endian conversion performance of this machine.
	double measure_endian_performance();
}

#endif

