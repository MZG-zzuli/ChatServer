#include "CServer.h"

CServer::CServer(boost::asio::io_context& ioc, int port):_ioc(ioc),_accept(ioc,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),port)),_sock(ioc)
{

}

void CServer::start()
{
	std::shared_ptr<CServer> self = shared_from_this();

	_accept.async_accept(_sock, [self](boost::system::error_code ec) {
		try {
			if (ec)
			{
				std::cerr << "Error accepting connection: " << ec.message() << std::endl;
				self->start();
				return;
			}
			std::make_shared<HttpConnection>(self->_sock)->start();
			self->start();

		}
		catch (std::exception& e)
		{
			std::cerr << "Exception: " << e.what() << std::endl;
			self->start();
		}
		});
}
