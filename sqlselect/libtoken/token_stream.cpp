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

		t.clear();

		token_part tmp;

		while (char c = line_buf.getch())
		{
			if (iswspace(c))
			{
				if (tmp.body.length() > 0)
					break;
			}
			else
			{
				if (comment_from_here(c))
				{
					line_buf.skip_to_eol();
					if (tmp.body.length() != 0) // if already something has been read
						break;
				}
				else
					tmp.body.append(1, c);
			}
		}

		if (tmp.body.length() == 0) // nothing found until end of stream
			state = false;
		else
			t.parts.push_back(tmp);

		return *this;
	}

	bool token_stream::comment_from_here(char c)
	{
		string& prefix = settings.single_line_comment_prefix;

		if (prefix.length() == 0)
			return false;

		char cur_c = c;
		int cur_pos = 0;
		bool diff_found = true;

		while (prefix[cur_pos] == cur_c)
		{
			if (cur_pos == (prefix.length() - 1)) // end of prefix reached
			{
				diff_found = false;
				break;
			}

			if (!(cur_c = line_buf.getch()))
				break;

			cur_pos++;
		}

		if (cur_pos != 0) // rewind buffer back if checked more than current character
			line_buf.ungetch(cur_pos);

		return (!diff_found);
	}
}

