#pragma once

#include "../libtoken/libtoken.h"
#include "tabular_stream_settings.h"
#include "libtabular_exception.h"

#include <vector>
#include <memory>

namespace libtabular
{
	using namespace std;
	using namespace libtoken;

	class tabular_stream
	{
		public:
			tabular_stream(const tabular_stream_settings& ts_settings, istream& header_is, istream& data_is);

			vector<token> header;

			tabular_stream& operator >> (vector<token>& line);

			operator bool() const { return state; };

			friend ostream& operator << (ostream& os, const vector<token>& line);

			class tabularization_error : public libtabular_exception
			{
				public:
					tabularization_error(const char *msg, const line_buffer& buf)
						: libtabular_exception(msg),
						cur_line_no{ buf.get_cur_line_no() }, cur_line_pos{ buf.get_cur_line_pos() }
					{}

					int cur_line_no;
					int cur_line_pos;
			};

		private:
			tabular_stream_settings settings;
			string field_separator_string;
			bool state;

			unique_ptr<token_stream> ds; // data stream

			bool read_line(token_stream& ts, vector<token>& line, bool allow_separator_at_end = true);

			void tabular_assert(const bool condition, const char *msg, const line_buffer& line_buf)
			{
				if (condition)
				{
					state = false;
					throw tabularization_error(msg, line_buf);
				}
			}
	};
}