#pragma once

#include "token_stream.h"
#include "tabular_stream_settings.h"
#include "libtoken_exception.h"

#include <vector>

namespace libtoken
{
	using namespace std;

	class tabular_stream
	{
		public:
			tabular_stream(const tabular_stream_settings& ts_settings, istream& header_is, istream& data_is);
			~tabular_stream() { if (ds != nullptr) delete ds; }

			vector<token> header;

			tabular_stream& operator >> (vector<token>& line);

			operator bool() const { return state; };

			friend ostream& operator << (ostream& os, const vector<token>& line);

			class tabular_error : public libtoken_exception
			{
				public:
					tabular_error(const char *msg, const line_buffer& buf) : libtoken_exception(msg, buf) {}
			};

		private:
			tabular_stream_settings settings;
			string field_separator_string;
			bool state;

			token_stream *ds = nullptr; // data stream

			bool read_line(token_stream& ts, vector<token>& line, bool allow_separator_at_end = true);

			void tabular_assert(const bool condition, const char *msg, const line_buffer& line_buf)
			{
				if (condition)
				{
					state = false;
					throw tabular_error(msg, line_buf);
				}
			}
	};
}