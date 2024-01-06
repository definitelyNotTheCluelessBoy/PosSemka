//
// Created by Matúš on 06/01/2024.
//

#ifndef SEMESTRALKACLIENT_SOCKET_H
#define SEMESTRALKACLIENT_SOCKET_H

#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;


class Socket {
    tcp::socket* socket;
public:
    Socket(short port, string hostname);
    ~Socket();
    string read();
    void write(string stringtowrite);

};


#endif //SEMESTRALKACLIENT_SOCKET_H
