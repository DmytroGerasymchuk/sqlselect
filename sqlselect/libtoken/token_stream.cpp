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

		t.clear(settings.multipart_token_separator);

		token_part tmp;
		bool tp_qual_started = false;

		while (char c = line_buf.getch())
		{
			if ((iswspace(c) && (!tp_qual_started)) || (c == '\n'))
			{
				if ((tmp.body.length() != 0) || tp_qual_started) // if already something has been read
					break; // or newline inside of qualified token

				continue;
			}

			if (comment_from_here(c))
			{
				line_buf.skip_to_eol();

				if (tmp.body.length() != 0) // if already something has been read
					break;

				continue;
			}

			if ((c == settings.multipart_token_separator) && (!tp_qual_started))
			{
				if (tmp.body.length() == 0)
				{
					state = false;
					throw syntax_error("empty part of multipart token detected", line_buf);
				}

				t.parts.push_back(tmp);
				tmp.clear();

				continue;
			}

			if (c == settings.token_part_qualifier)
			{
				if (!tp_qual_started) // start of the qualified token part
				{
					if (tmp.body.length() != 0)
					{
						state = false;
						throw syntax_error("attempt to start token part qualification inside the token part", line_buf);
					}

					tp_qual_started = true;
					tmp.qualified_by = settings.token_part_qualifier;

					continue;
				}

				// end of the qualified token part
				if (tmp.body.length() == 0)
				{
					state = false;
					throw syntax_error("empty qualified token part detected", line_buf);
				}

				tp_qual_started = false;

				// what follows the end of qualified token part?
				c = line_buf.getch();
				line_buf.ungetch(1);

				if (c == settings.multipart_token_separator) // next part of the multipart token
					continue;

				break;
			}

			tmp.body.append(1, c);
		}

		if (tp_qual_started)
		{
			state = false;
			throw syntax_error("non-closed token part qualification", line_buf);
		}

		if (tmp.body.length() == 0) // nothing found until end of stream
		{
			state = false;

			if (t.parts.size() > 0)
				throw syntax_error("improper multipart token detected", line_buf);
		}
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

