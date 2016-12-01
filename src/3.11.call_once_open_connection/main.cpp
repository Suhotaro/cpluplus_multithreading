#include <iostream>
#include <memory>
#include <mutex>

class Networking {
private:
	std::once_flag connection_init_flag;

	void open_connection() {
		std::cout << "open connection\n";
	}

public:
	Networking() = default;

	void send_data(std::string const& packet) {
		std::call_once(connection_init_flag, &Networking::open_connection, this);
		std::cout << "send packet\n";
	}

	std::string receive_data() {
		std::call_once(connection_init_flag, &Networking::open_connection, this);
		std::cout << "receive packet\n";

		return "packet_3";
	}
};

int main()
{
	Networking net;

	net.send_data("paket_1");
	net.send_data("paket_2");
	std::string res = net.receive_data();

	return 0;
}
