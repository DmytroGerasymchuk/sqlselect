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

		private:
			token_stream_settings settings;
			line_buffer line_buf;
			bool state;

	};
}