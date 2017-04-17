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
			token_stream(const token_stream_settings& ts_settings, istream& is);

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

			void syntax_assert(const bool condition, const char *msg)
			{
				if (condition)
				{
					state = false;
					throw syntax_error(msg, line_buf);
				}
			}

			bool something_read(const token& t, const token_part& tmp) const { return (tmp.body.length() != 0) || (t.parts.size() != 0); };

			enum class QualState { None, Token, Text };

			bool qualification(const char c, const char etalon, const QualState& current_state, const QualState required_state) const
			{
				return
					(c == etalon) &&
					((current_state == QualState::None) || (current_state == required_state));
			};

			bool comment_from_here(const char c);
			int special_token_from_here_index(const char c);

			bool string_from_here(const char c, const string& etalon);
	};
}