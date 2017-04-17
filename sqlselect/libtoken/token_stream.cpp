#include "token_stream.h"

namespace libtoken
{
	token_stream::token_stream(const token_stream_settings& ts_settings, istream& is)
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
		QualState qual_state = QualState::None;

		while (char c = line_buf.getch())
		{
			// EOL signal
			if (c == line_buf.eol_signal())
			{
				if (something_read(t, tmp)) // if already something has been read
					break;

				if (settings.newline_as_token)
				{
					tmp.type = token_part::Type::EOL;
					tmp.body.append(1, c);
					break;
				}
				else
					continue;
			}

			// if this is some kind of space AND NOT inside of qualification process
			if (iswspace(c) && (qual_state == QualState::None))
			{
				if (something_read(t, tmp)) // if already something has been read
					break;

				continue;
			}

			if (qual_state == QualState::None) // if not inside of a qualified content
			{
				// is this a comment?
				if (comment_from_here(c))
				{
					line_buf.skip_to_eol();

					if (something_read(t, tmp)) // if already something has been read
						break;

					continue;
				}

				// is this a token part separator?
				if (c == settings.multipart_token_separator)
				{
					syntax_assert(tmp.body.length() == 0, "empty part of multipart token detected");

					t.parts.push_back(tmp);
					tmp.clear();

					continue;
				}

				// or maybe some special token starts from here?
				int sti = special_token_from_here_index(c);
				if (sti > -1) // yes!
				{
					// if special token follows something, which has already been read
					if (something_read(t, tmp))
					{
						line_buf.ungetch(); // put first character of the special token back to be read later
						break; // and definitively end the current token
					}
					// otherwise, skip characters of the current special token
					// and return the special token as a result
					tmp.body = settings.special_tokens[sti];
					line_buf.skip(tmp.body.size() - 1); // one character less, because 1st character has been already read!
					break;
				}
			}

			// text qualifier NOT inside another qualification?
			if (qualification(c, settings.text_qualifier, qual_state, QualState::Text))
			{
				if (qual_state == QualState::None) // start of the qualified text
				{
					if (something_read(t, tmp))
					{
						line_buf.ungetch(); // put first character back to be read later
						break; // and definitively end the current token
					}

					qual_state = QualState::Text;
					tmp.type = token_part::Type::Text;
					tmp.qualified_by = settings.text_qualifier;

					continue;
				}

				// end of the qualified text
				qual_state = QualState::None;

				// we don't check for positive length here, because text may be empty (like '')!

				break; // definitively end-of-token
			}

			// token qualifier NOT inside another qualification?
			if (qualification(c, settings.token_part_qualifier, qual_state, QualState::Token))
			{
				if (qual_state == QualState::None) // start of the qualified token part
				{
					if (tmp.body.length() != 0) // if something is being read
					{
						line_buf.ungetch(); // put first character back to be read later
						break; // and definitively end the current token
					}

					qual_state = QualState::Token;
					tmp.qualified_by = settings.token_part_qualifier;

					continue;
				}

				// end of the qualified token part
				syntax_assert(tmp.body.length() == 0, "empty qualified token part detected");

				qual_state = QualState::None;

				// what follows the end of qualified token part?
				c = line_buf.getch();
				line_buf.ungetch(1);

				if (c == settings.multipart_token_separator) // next part of the multipart token
					continue;

				break; // definitively end-of-token
			}

			tmp.body.append(1, c);
		}

		syntax_assert(qual_state == QualState::Token, "non-closed token part qualification");
		syntax_assert(qual_state == QualState::Text, "non-closed text qualification");

		if ((tmp.body.length() == 0) && (tmp.type != token_part::Type::Text))
			// nothing found until end of stream
			// and this is not a qualified text (because text may be empty, like '')
		{
			syntax_assert(t.parts.size() > 0, "improper multipart token detected");

			state = false; // say that this stream does not contain anything
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
		for (unsigned int i = 0; i < settings.special_tokens.size(); i++)
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

