#include <iostream>
#include <fstream>
#include <filesystem>
#include <boost/asio.hpp>
#include <string>
#include <sstream>

using namespace std;
using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;


string read(tcp::socket & socket) {
    boost::asio::streambuf buf;
    read_until( socket, buf, "\n" );
    string data = buffer_cast<const char*>(buf.data());
    data.pop_back();
    return data;
}

void write(const string& stringToWrite,tcp::socket & socket) {
    const string msg = stringToWrite + "\n";
    boost::asio::write( socket, boost::asio::buffer(msg));
}

void save(string worldToSave) {

    string path = "/home/kan1/semestralkaSaves/";
    string timestamp = to_string(std::time(nullptr));
    path+=timestamp;
    path+=".txt";

    ofstream myfile(path);

    if (myfile.is_open()) {
        myfile << worldToSave;
        myfile.close();
        cout << "File saved successfully: " << path << endl;
    } else {
        cout << "Error opening file: " << path << endl;
    }
}

string listSaves() {
    stringstream stream;
    string path = "/home/kan1/semestralkaSaves/";
    for (const auto & entry : filesystem::directory_iterator(path))
        stream << entry.path().string().substr(28,10) << std::endl;
    return stream.str();
}

string sendWold(string worldId) {
    string path = "/home/kan1/semestralkaSaves/";
    path+=worldId;
    path+=".txt";

    string world;

    ifstream MyReadFile(path);
    getline (MyReadFile, world);

    MyReadFile.close();

    return world;

}

int main() {
    string message;
    bool readInput = true;

    boost::asio::io_service io_service;
    tcp::socket socket(io_service);
    tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(), 17319));
    acceptor_.accept(socket);

    while (readInput) {

        message = read(socket);
        if(message == "end") {
            readInput= false;
        } else if (message == "download") {
            write(listSaves(), socket);
            message = read(socket);
            write(sendWold(message), socket);
        } else {
            save(message);
        }

    }
    return 0;
}
