#include <bits/stdc++.h>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using namespace boost::asio;
using ip::udp;

std::vector <std::string> split (std::string &text) {
    std::istringstream iss(text);
    std::string s;
    std::vector <std::string> res;

    while (getline(iss, s, ' ')) {
        res.push_back(s);
    }

    return res;
}

udp::endpoint find_opponent () {
    io_service io_service;
    
    //socket creation
    udp::socket socket(io_service);
    
    //connection
    socket.open(udp::v4());

    const int timeout = 200;
    ::setsockopt(socket.native_handle(), SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof timeout);//SO_SNDTIMEO for send ops
    
    // request/message from client
    const std::string msg = "Hello from Client!\n";
    udp::endpoint server = udp::endpoint{ip::address::from_string("127.0.0.1"), 1234};
    socket.send_to(buffer("Hi!"), server);
    std::cout << "Client sent hello message!" << std::endl;
    
    // getting response from server
    boost::array<char, 128> recv_buf;
    size_t len = socket.receive_from(buffer(recv_buf), server);
    
    std::string message = recv_buf.data();
    std::cout << message << "\n";

    std::vector <std::string> peer_info = split(message);
    udp::endpoint peer = udp::endpoint{ip::address::from_string(peer_info[0]), stoi(peer_info[1])};

    socket.send_to(buffer("Hello peer"), peer);
    socket.receive_from(buffer(recv_buf), peer);

    std::cout << recv_buf.data() << "\n";
    return peer;
}