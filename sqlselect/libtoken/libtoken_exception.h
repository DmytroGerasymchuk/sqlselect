#pragma once

#include "line_buffer.h"

namespace libtoken
{
	class libtoken_exception : public exception
	{
		public:
			libtoken_exception(const char *msg, const line_buffer& buf)
				: exception(msg),
				cur_line_no{ buf.get_cur_line_no() }, cur_line_pos{ buf.get_cur_line_pos() }
			{}

			virtual ~libtoken_exception() {};

			int cur_line_no;
			int cur_line_pos;
	};
}