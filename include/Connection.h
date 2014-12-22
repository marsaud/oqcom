#ifndef CONNECTION_H
#define CONNECTION_H

#include <sstream>
#include <vector>

#include <boost/asio.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/bind.hpp>

class Connection
{
public:
    Connection(boost::asio::io_service& io_service);
    virtual ~Connection();

    boost::asio::ip::tcp::socket& socket();

    template <typename T, typename Handler>
    void async_write(const T& t, Handler handler)
    {
        // On sérialise
        std::ostringstream archive_stream;
        boost::archive::text_oarchive archive(archive_stream);
        archive << t;
        m_outbound_data = archive_stream.str();

        // On écrit un header
        std::ostringstream header_stream;
        header_stream << std::setw(header_length)
                      << std::hex << m_outbound_data.size();
        if (!header_stream || header_stream.str().size() != header_length)
        {
            // En cas de problème, on informe l'appelant.
            boost::system::error_code error(boost::asio::error::invalid_argument);
            m_socket.get_io_service().post(boost::bind(handler, error));
            return;
        }
        m_outbound_header = header_stream.str();

        // On écrit les données sérialisées dans le socket
        std::vector<boost::asio::const_buffer> buffers;
        buffers.push_back(boost::asio::buffer(m_outbound_header));
        buffers.push_back(boost::asio::buffer(m_outbound_data));
        boost::asio::async_write(m_socket, buffers, handler);
    }

    template <typename T, typename Handler>
    void async_read(T& t, Handler handler)
    {
        // On récupère le header (10)
        void (Connection::*f)( const boost::system::error_code&, T&, boost::tuple<Handler>)
            = &Connection::handle_read_header<T, Handler>;
        boost::asio::async_read(m_socket, boost::asio::buffer(m_inbound_header),
                                boost::bind(f,
                                            this, boost::asio::placeholders::error, boost::ref(t),
                                            boost::make_tuple(handler)));
    }

    template <typename T, typename Handler>
    void handle_read_header(const boost::system::error_code& e,
                            T& t, boost::tuple<Handler> handler)
    {
        if (e)
        {
            boost::get<0>(handler)(e);
        }
        else
        {
            // Détermine la longueur du vrai message (11)
            std::istringstream is(std::string(m_inbound_header, header_length));
            std::size_t m_inbound_datasize = 0;
            if (!(is >> std::hex >> m_inbound_datasize))
            {
                // Header non valide, on informe la fonction appelante
                boost::system::error_code error(boost::asio::error::invalid_argument);
                boost::get<0>(handler)(error);
                return;
            }

            // On récupère les données (12)
            m_inbound_data.resize(m_inbound_datasize);
            void (Connection::*f)(const boost::system::error_code&, T&, boost::tuple<Handler>)
                = &Connection::handle_read_data<T, Handler>;

            boost::asio::async_read(m_socket, boost::asio::buffer(m_inbound_data),
                                    boost::bind(f, this,
                                                boost::asio::placeholders::error, boost::ref(t), handler));
        }
    }

    // Les données reçues, on les désérialise (13)
    template <typename T, typename Handler>
    void handle_read_data(const boost::system::error_code& e,
                          T& t, boost::tuple<Handler> handler)
    {
        if (e)
        {
            boost::get<0>(handler)(e);
        }
        else
        {
            // On extrait (14)
            try
            {
                std::string archive_data(&m_inbound_data[0], m_inbound_data.size());
                std::istringstream archive_stream(archive_data);
                boost::archive::text_iarchive archive(archive_stream);
                archive >> t;
            }
            catch (std::exception& e)
            {
                // En cas d'échec
                boost::system::error_code error(boost::asio::error::invalid_argument);
                boost::get<0>(handler)(error);
                return;
            }

            // On informe l'appelant que tout s'est bien passé. (15)
            boost::get<0>(handler)(e);
        }
    }
protected:
private:
    boost::asio::ip::tcp::socket m_socket; // (1)

    // Taille de l'header.
    enum { header_length = 8 }; // (2)

    std::string m_outbound_header;
    std::string m_outbound_data;
    char m_inbound_header[header_length];
    std::vector<char> m_inbound_data;
};

typedef boost::shared_ptr<Connection> connection_ptr;

#endif // CONNECTION_H

