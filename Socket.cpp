//
// Created by Matúš on 06/01/2024.
//

#include "Socket.h"


Socket::Socket(short port, string hostname) {
    io_service io_service;
    this->socket = new tcp::socket(io_service);
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(hostname,"http");
    boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
    boost::asio::ip::tcp::endpoint endpoint = iter->endpoint();
    this->socket->connect( tcp::endpoint( endpoint.address(), port));
}

Socket::~Socket() {
    delete this->socket;
}

string Socket::read() {
    streambuf buf;
    read_until( *this->socket, buf, "\n" );
    string data = buffer_cast<const char*>(buf.data());
    return data;
}

void Socket::write(string stringToWrite) {
    const string msg = stringToWrite + "\n";
    boost::asio::write( *this->socket, boost::asio::buffer(stringToWrite));
}
