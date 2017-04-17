#pragma once

#include "token_stream.h"
#include "tabular_stream_settings.h"

#include <vector>

namespace libtoken
{
	using namespace std;

	class tabular_stream
	{
		public:
			tabular_stream(const tabular_stream_settings& ts_settings, istream& header_is, istream& data_is);

			vector<token> header;

		private:
			tabular_stream_settings settings;

			void read_line(token_stream& ts, vector<token> &line);
	};
}