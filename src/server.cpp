#include <bits/stdc++.h>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <chrono>
#include <thread>

using namespace std::this_thread;
using namespace std::chrono_literals;
using namespace boost::asio;
using ip::udp;

typedef boost::asio::detail::socket_option::integer<SOL_SOCKET, SO_RCVTIMEO> rcv_timeout_option; //somewhere in your headers to be used everywhere you need it

int main() {
    io_service io_service;
    
    udp::endpoint local_endpoint = udp::endpoint(ip::address::from_string("127.0.0.1"), 1234);
    //socket creation 
    udp::socket socket_(io_service);
    socket_.open(udp::v4());
    socket_.bind(local_endpoint);

    const int timeout = 200;
    ::setsockopt(socket_.native_handle(), SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof timeout);//SO_SNDTIMEO for send ops
    // socket_.set_option(rcv_timeout_option{ 200 });
    
    std::vector <std::string> connection_add;
    std::vector <unsigned short> connection_port;

    boost::array<char, 128> recv_buf;
    udp::endpoint client1;
    socket_.receive_from(buffer(recv_buf), client1);
    std::cout << client1.address().to_string() << " " << client1.port() << "\n";
    connection_add.push_back(client1.address().to_string());
    connection_port.push_back(client1.port());
    
    udp::endpoint client2;
    socket_.receive_from(buffer(recv_buf), client2);
    std::cout << client2.address().to_string() << " " << client2.port() << "\n";
    connection_add.push_back(client2.address().to_string());
    connection_port.push_back(client2.port());

    socket_.send_to(buffer(connection_add[0] + " " + std::to_string(connection_port[0])), client2);
    socket_.send_to(buffer(connection_add[1] + " " + std::to_string(connection_port[1])), client1);

    while (true)
    {
        std::this_thread::sleep_for(20ms);
        socket_.send_to(buffer("Keep Alive"), client1);
        socket_.send_to(buffer("Keep Alive"), client2);
    }
}
