#pragma once

#include "../libtoken/libtoken.h"
#include "sql_token_stream_settings.h"
#include "libsql_exception.h"

namespace libsql
{
	using namespace std;
	using namespace libtoken;

	class stmt_part_stream
	{
		public:
			stmt_part_stream(istream& is) :
				ts{ sql_token_stream_settings(), is }, state{ true } {}

			stmt_part_stream& operator >> (vector<token>& stmt_parts);

			operator bool() const { return state; };

			class statement_error : public libsql_exception
			{
				public:
					statement_error(const char *msg, const line_buffer& buf)
						: libsql_exception(msg),
						cur_line_no{ buf.get_cur_line_no() }, cur_line_pos{ buf.get_cur_line_pos() }
					{}

					int cur_line_no;
					int cur_line_pos;
			};

		private:
			token_stream ts;
			bool state;

			void stmt_assert(const bool condition, const char *msg)
			{
				if (condition)
				{
					state = false;
					throw statement_error(msg, ts.get_buffer());
				}
			}
	};
}