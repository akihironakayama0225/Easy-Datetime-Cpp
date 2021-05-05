#ifndef _MY_DATETIME_EXCEPTIONS_
#define _MY_DATETIME_EXCEPTIONS_

#include <stdexcept>
#include <string>
namespace EZ
{
	class DatetimeException : std::exception
	{
		const char *m_msg;

	public:
		DatetimeException(const char *msg) : m_err(msg)
		{
		}
		DatetimeException(const std::string str) : m_err(str.c_str())
		{
		}

		const char *what() const throw()
		{
			return m_err.what();
		}

	private:
		// use runtime_error to store error messages
		std::runtime_error m_err;
	};
}
#endif