#pragma once

#include <exception>

namespace libtabular
{
	class libtabular_exception : public std::exception
	{
		public:
			libtabular_exception(const char *msg)
				: exception(msg)
			{}

			virtual ~libtabular_exception() {};
	};
}