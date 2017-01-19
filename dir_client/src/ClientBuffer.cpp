#include "../includes/Client.hpp"

ClientBuffer::ClientBuffer()
{
	len = 0;
	start = 0;
	buffer[0] = 0;
}

ClientBuffer::~ClientBuffer()
{}

/*
**	return first cmd whithout end char ('\n').
*/
std::string ClientBuffer::getFirstMsg()
{
	std::string buff;
	char *end;
	int lenCmd;

	buff = getBuffer();
	std::size_t found = buff.find(END);
	if (found != std::string::npos)
	{
		buff = buff.substr(0, found + LEN_END);
		std::size_t lenCmd = buff.size();
		erase(lenCmd);
	}
	else
		buff.clear();
	return (buff);
}

void ClientBuffer::pushMsg(std::string msg)
{
	pushBuffer((char *)msg.c_str(), msg.size());
}

std::string ClientBuffer::getBuffer()
{
	std::string	retBuff(len, 0);
	int			i;

	if (len > 0)
	{
		i = 0;
		while (i < len)
		{
			retBuff[i] = buffer[(start + i) % BUFF_SIZE];
			i++;
		}
	}
	return (retBuff);
}

void	ClientBuffer::erase(int len)
{
	start = (start + len) % BUFF_SIZE;
	if (len <= this->len)
		this->len -= len;
	else
		this->len = 0;
}

int		ClientBuffer::getLen()
{
	return (this->len);
}

void	ClientBuffer::pushBuffer(char *to_write, int size)
{
	int		i;

	if (size > 0)
	{
		i = 0;
		if (len + size > BUFF_SIZE)
		{
			throw (CustomException("Buffer full"));
			return ;
		}
		while (i < size)
		{
			buffer[(start + len + i) % BUFF_SIZE] = to_write[i];
			i++;
		}
		len += i;
	}
}