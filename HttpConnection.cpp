#include "HttpConnection.h"

HttpConnection::HttpConnection(boost::asio::io_context& ioc) :_sock(ioc)
{
}

HttpConnection::~HttpConnection()
{
	std::cout<<"HttpConnection::~HttpConnection()"<<std::endl;
}

boost::asio::ip::tcp::socket& HttpConnection::getSocket()
{
	return _sock;
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
unsigned char HttpConnection::ToHex(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}
unsigned char HttpConnection::FromHex(unsigned char x)
{
	unsigned char y;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	else assert(0);
	return y;
}
std::string HttpConnection::UrlEncode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		//�ж��Ƿ�������ֺ���ĸ����
		if (isalnum((unsigned char)str[i]) ||
			(str[i] == '-') ||
			(str[i] == '_') ||
			(str[i] == '.') ||
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ') //Ϊ���ַ�
			strTemp += "+";
		else
		{
			//�����ַ���Ҫ��ǰ��%���Ҹ���λ�͵���λ�ֱ�תΪ16����
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4);
			strTemp += ToHex((unsigned char)str[i] & 0x0F);
		}
	}
	return strTemp;
}
std::string HttpConnection::UrlDecode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		//��ԭ+Ϊ��
		if (str[i] == '+') strTemp += ' ';
		//����%������������ַ���16����תΪchar��ƴ��
		else if (str[i] == '%')
		{
			assert(i + 2 < length);
			unsigned char high = FromHex((unsigned char)str[++i]);
			unsigned char low = FromHex((unsigned char)str[++i]);
			strTemp += high * 16 + low;
		}
		else strTemp += str[i];
	}
	return strTemp;
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
		PreParseGetParam();
		bool success = LogicSystem::GetInstance()->HandleGet(_get_url, shared_from_this());
		if (success)
		{
			_response.set(boost::beast::http::field::server, "GateServer");
			_response.result(boost::beast::http::status::ok);
			WriteResponse();
		}
		std::cout<<_request.target()<<std::endl;
		_response.result(boost::beast::http::status::not_found);
		_response.set(boost::beast::http::field::content_type, "text/plain");
		boost::beast::ostream (_response.body())<< "Not found";
		WriteResponse();
	}
	else if (_request.method() == boost::beast::http::verb::post)
	{
		std::string str = boost::beast::buffers_to_string(_request.body().data());

		std::cout<<str<<std::endl;
		bool success = LogicSystem::GetInstance()->HandlePost(_request.target(), shared_from_this());
		if (success)
		{
			_response.result(boost::beast::http::status::ok);
			_response.set(boost::beast::http::field::server, "GateServer");
		}
		else
		{
			_response.result(boost::beast::http::status::not_found);
			_response.set(boost::beast::http::field::content_type, "text/plain");
			boost::beast::ostream(_response.body()) << "Not found";
		}

		WriteResponse();
	}

}

void HttpConnection::PreParseGetParam()
{
	// ��ȡ URI  
	auto uri = _request.target();
	// ���Ҳ�ѯ�ַ����Ŀ�ʼλ�ã��� '?' ��λ�ã�  
	auto query_pos = uri.find('?');
	if (query_pos == std::string::npos) {
		_get_url = uri;
		return;
	}
	_get_url = uri.substr(0, query_pos);
	std::string query_string = uri.substr(query_pos + 1);
	std::string key;
	std::string value;
	size_t pos = 0;
	while ((pos = query_string.find('&')) != std::string::npos) {
		auto pair = query_string.substr(0, pos);
		size_t eq_pos = pair.find('=');
		if (eq_pos != std::string::npos) {
			key = UrlDecode(pair.substr(0, eq_pos)); // ������ url_decode ����������URL����  
			value = UrlDecode(pair.substr(eq_pos + 1));
			_get_params[key] = value;

		}
		query_string.erase(0, pos + 1);
	}
	// �������һ�������ԣ����û�� & �ָ�����  
	if (!query_string.empty()) {
		size_t eq_pos = query_string.find('=');
		if (eq_pos != std::string::npos) {
			key = UrlDecode(query_string.substr(0, eq_pos));
			value = UrlDecode(query_string.substr(eq_pos + 1));
			_get_params[key] = value;
		}
	}
}
