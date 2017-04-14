#include "token_stream.h"

#define SOMETHING_READ (tmp.body.length() != 0) || (t.parts.size() != 0)

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
		bool txt_qual_started = false;

		while (char c = line_buf.getch())
		{
			// if this is some kind of space AND NOT inside of qualified token / qualified text
			// OR this is the end of the line
			if ((iswspace(c) && (!tp_qual_started) && (!txt_qual_started)) || (c == '\n'))
			{
				if (SOMETHING_READ || tp_qual_started || txt_qual_started) // if already something has been read
					break; // or newline inside of qualified token / qualified text

				continue;
			}

			if ((!tp_qual_started) && (!txt_qual_started)) // if not inside a qualified token / qualified text
			{
				// is this a comment?
				if (comment_from_here(c))
				{
					line_buf.skip_to_eol();

					if (SOMETHING_READ) // if already something has been read
						break;

					continue;
				}

				// is this a token part separator?
				if (c == settings.multipart_token_separator)
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

				// or maybe some special token starts from here?
				int sti = special_token_from_here_index(c);
				if (sti > -1) // yes!
				{
					// if special token follows something, which has already been read
					if (SOMETHING_READ)
					{
						line_buf.ungetch(); // put first character of the special token back to be read later
						break; // and definitively end the current token
					}
					// otherwise, skip characters of the current special token
					// and return the special token as a result
					const string& st = settings.special_tokens[sti];
					for (int i = 1; i < st.size(); i++) // from the "1" index, because 1st character is has been already read!
						line_buf.getch();
					tmp.body = st;
					break;
				}
			}

			if ((c == settings.text_qualifier) && (!tp_qual_started)) // text qualifier NOT inside qualified token
			{
				if (!txt_qual_started) // start of the qualified text
				{
					if (SOMETHING_READ)
					{
						line_buf.ungetch(); // put first character of the special token back to be read later
						break; // and definitively end the current token
					}

					txt_qual_started = true;
					tmp.qualified_by = settings.text_qualifier;

					continue;
				}

				// end of the qualified text
				txt_qual_started = false;

				// we don't check for positive length here, because text may be empty (like '')!

				break; // definitively end-of-token
			}

			if ((c == settings.token_part_qualifier) && (!txt_qual_started)) // token qualifier NOT inside qualified text
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

				break; // definitively end-of-token
			}

			tmp.body.append(1, c);
		}

		if (tp_qual_started)
		{
			state = false;
			throw syntax_error("non-closed token part qualification", line_buf);
		}

		if (txt_qual_started)
		{
			state = false;
			throw syntax_error("non-closed text qualification", line_buf);
		}

		if ((tmp.body.length() == 0) && (tmp.qualified_by != settings.text_qualifier))
			// nothing found until end of stream
			// and this is not a qualified text (because text may be empty, like '')
		{
			state = false;

			if (t.parts.size() > 0)
				throw syntax_error("improper multipart token detected", line_buf);
		}
		else
			t.parts.push_back(tmp);

		return *this;
	}

	bool token_stream::comment_from_here(const char c)
	{
		return string_from_here(c, settings.single_line_comment_prefix);
	}

	int token_stream::special_token_from_here_index(const char c)
	{
		for (int i = 0; i < settings.special_tokens.size(); i++)
			if (string_from_here(c, settings.special_tokens[i]))
				return i;

		return -1;
	}

	bool token_stream::string_from_here(const char c, const string& etalon)
	{
		if (etalon.length() == 0)
			return false;

		char cur_c = c;
		int cur_pos = 0;
		bool diff_found = true;

		while (etalon[cur_pos] == cur_c)
		{
			if (cur_pos == (etalon.length() - 1)) // end of etalon reached
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

