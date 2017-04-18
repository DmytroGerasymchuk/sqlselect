#include "line_buffer.h"
#include "libtoken_exception.h"

#include <string>

namespace libtoken
{
	line_buffer::line_buffer(istream& is)
		: in_stream{ is }, current_line{ "" }, buf_pos{ -1 }, line_no{ 0 } { }

	char line_buffer::getch()
	{
		if ((buf_pos == -1) || (buf_pos == current_line.length())) // empty buffer or EOL reached?
		{
			buf_pos = -1;

			while (fetchable())
			{
				getline(in_stream, current_line);

				if (failed())
					throw libtoken_exception("error in the input stream", *this);

				line_no++;

				if (current_line.length() > 0) // stop only if read non-empty line
				{
					current_line.append(1, eol_signal()); // mimic EOL
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

	void line_buffer::ungetch(unsigned int num_chars)
	{
		check_cons();

		if (num_chars < 1)
			throw libtoken_exception("cannot unget less than 1 character", *this);

		if ((buf_pos - num_chars) < 0)
			throw libtoken_exception("too much characters to unget", *this);

		buf_pos -= num_chars;
	}

	void line_buffer::skip(unsigned int num_chars)
	{
		check_cons();

		if (num_chars < 0)
			throw libtoken_exception("cannot skip less than 0 characters", *this);

		if ((buf_pos + num_chars) >= current_line.size())
			throw libtoken_exception("too much characters to skip", *this);
	}

	void line_buffer::skip_to_eol()
	{
		check_cons();

		buf_pos = current_line.length() - 1; // point to "\n" signal at the end of line
	}

	void line_buffer::check_cons() const
	{
		if (buf_pos == -1)
			throw libtoken_exception("buffer is empty or in invalid state, cannot proceed with required operation", *this);
	}
}