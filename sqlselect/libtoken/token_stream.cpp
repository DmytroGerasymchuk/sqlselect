#include "token_stream.h"

namespace libtoken
{
	token_stream::token_stream(token_stream_settings& ts_settings, istream& is)
		: line_buf{ is }, state{ true }
	{
		settings = ts_settings;
	}

	token_stream& token_stream::operator >> (token& t)
	{
		if (!state)
			throw exception("this token_stream is not readable any more");

		t.parts.clear();

		string tmp;

		while (char c = line_buf.getch())
		{
			if (iswspace(c))
			{
				if (tmp.length() > 0)
					break;
			}
			else
				tmp.append(1, c);
		}

		if (tmp.length() == 0) // nothing found until end of stream
			state = false;
		else
			t.parts.push_back(tmp);

		return *this;
	}
}

