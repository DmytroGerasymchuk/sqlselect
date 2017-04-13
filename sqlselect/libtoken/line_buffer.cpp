#include "line_buffer.h"

#include <string>

namespace libtoken
{
	line_buffer::line_buffer(istream& is)
		: in_stream{ is }, current_line{ "" }, buf_pos{ -1 } { }

	char line_buffer::getch()
	{
		if ((buf_pos == -1) || (buf_pos == current_line.length())) // empty buffer or EOL reached?
		{
			buf_pos = -1;

			while (fetchable())
			{
				getline(in_stream, current_line);

				if (failed())
					throw exception("error in the input stream");

				if (current_line.length() > 0) // stop only if read non-empty line
				{
					current_line.append(1, ' '); // add a blank space at the end of line to mimic EOL
					buf_pos = 0;
					break;
				}
			}
		}

		if (buf_pos == -1) // nothing in buffer
			return 0;
		else
		{
			char res = current_line[buf_pos];
			buf_pos++;
			return res;
		}
	}

	void line_buffer::ungetch(int num_chars)
	{
		check_cons();

		if (num_chars < 1)
			throw exception("cannot unget less than 1 character");

		if ((buf_pos - num_chars) < 0)
			throw exception("too much characters to unget");

		buf_pos -= num_chars;
	}

	void line_buffer::skip_to_eol()
	{
		check_cons();

		buf_pos = current_line.length();
	}

	void line_buffer::check_cons() const
	{
		if (buf_pos == -1)
			throw exception("buffer is empty or in invalid state, cannot unget");
	}
}