#include "Connection.h"

Connection::Connection(boost::asio::io_service& io_service)
    : m_socket(io_service)
{
    std::cout << "Connection opened" << std::endl;
}

Connection::~Connection()
{
    std::cout << "Connection closed" << std::endl;
}

boost::asio::ip::tcp::socket& Connection::socket()
{
    return m_socket;
}

Connection::connection_ptr Connection::create(boost::asio::io_service& ios)
{
    return Connection::connection_ptr(new Connection(ios));
}
