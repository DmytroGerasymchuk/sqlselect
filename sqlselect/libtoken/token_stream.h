#pragma once

#include <iostream>

#include "token.h"
#include "token_stream_settings.h"
#include "line_buffer.h"

namespace libtoken
{
	using namespace std;

	class token_stream
	{
		public:
			token_stream(token_stream_settings& ts_settings, istream& is);

			token_stream& operator >> (token& t);

			operator bool() const { return state; };

			class syntax_error : public exception
			{
				public:
					syntax_error(const char *msg, const line_buffer& buf)
						: exception(msg),
							cur_line_no{ buf.get_cur_line_no() }, cur_line_pos{ buf.get_cur_line_pos() }
					{}

					int cur_line_no;
					int cur_line_pos;
			};

		private:
			token_stream_settings settings;
			line_buffer line_buf;
			bool state;

			bool comment_from_here(char c);
	};
}