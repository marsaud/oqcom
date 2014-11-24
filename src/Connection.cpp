#include "Connection.h"

Connection::Connection(boost::asio::io_service& io_service)
    : m_socket(io_service)
{
    //ctor
}

Connection::~Connection()
{
    //dtor
}

boost::asio::ip::tcp::socket& Connection::socket()
{
    return m_socket;
}
