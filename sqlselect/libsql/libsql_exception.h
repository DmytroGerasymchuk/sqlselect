#pragma once

#include <exception>

namespace libsql
{
	class libsql_exception : public std::exception
	{
	public:
		libsql_exception(const char *msg)
			: exception(msg)
		{}

		virtual ~libsql_exception() {};
	};
}