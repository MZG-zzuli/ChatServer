#include "HttpConnection.h"

HttpConnection::HttpConnection(boost::asio::ip::tcp::socket& sock) :_sock(std::move(sock))
{
	
}

void HttpConnection::start()
{
	std::shared_ptr<HttpConnection> self = shared_from_this();
	boost::beast::http::async_read(_sock, _buffer, _request, [self](boost::system::error_code ec, std::size_t bytes_transferred) {
		try {
			if (ec)
			{
				std::cerr << "Error reading from socket: " << ec.message() << std::endl;
				return;
			}
			self->HandleReq();
			self->CheckDeadline();
		}catch (std::exception& e) {
			std::cerr << "Exception: " << e.what() << std::endl;
			return;
		}
		});
}

void HttpConnection::CheckDeadline()
{
	std::shared_ptr<HttpConnection> self=shared_from_this();
	deadline_.async_wait([self](const boost::system::error_code& ec) {
		if (!ec)
		{
			self->_sock.close();
		}
		});
}

void HttpConnection::WriteResponse()
{
	std::shared_ptr<HttpConnection> self = shared_from_this();
	_response.content_length(_response.body().size());
	boost::beast::http::async_write(_sock, _response, [self](boost::system::error_code ec,std::size_t bytes_transferred) {
		self->_sock.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
		self->deadline_.cancel();
		});
}

void HttpConnection::HandleReq()
{
	_response.keep_alive(false);
	_response.version(_request.version());
	if (_request.method() == boost::beast::http::verb::get)
	{
		bool success = LogicSystem::GetInstance()->HandleGet(_request.target(), shared_from_this());
		if (success)
		{
			_response.set(boost::beast::http::field::server, "GateServer");
			_response.result(boost::beast::http::status::ok);
			WriteResponse();
		}
		_response.result(boost::beast::http::status::not_found);
		_response.set(boost::beast::http::field::content_type, "text/plain");
		boost::beast::ostream (_response.body())<< "Not found";
		WriteResponse();
	}

}
