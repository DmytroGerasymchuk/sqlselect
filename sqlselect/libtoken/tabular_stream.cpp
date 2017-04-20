#include "tabular_stream.h"

namespace libtoken
{
	tabular_stream::tabular_stream(const tabular_stream_settings& ts_settings, istream& header_is, istream& data_is)
		: state{ true }
	{
		settings = ts_settings;
		
		if (settings.field_separator)
			field_separator_string = { settings.field_separator };

		token_stream_settings tss;

		tss.text_qualifier = settings.qualifier;
		if (settings.field_separator) // if field separator not just space
			tss.special_tokens.push_back(field_separator_string);
		tss.single_line_comment_prefix = settings.single_line_comment_prefix;

		tss.newline_as_token = true;

		token_stream hs = token_stream(tss, header_is);
		ds = unique_ptr<token_stream>(new token_stream(tss, data_is));

		if (!read_line(hs, header, false))
			throw tabular_error("cannot read header", hs.get_buffer());
	}

	tabular_stream& tabular_stream::operator >> (vector<token>& line)
	{
		if (!state)
			throw tabular_error("this tabular_stream is not readable any more", ds->get_buffer());

		if (!read_line(*ds, line))
			state = false;
		else
			tabular_assert(header.size() != line.size(), "data row has another number of tokens as header row", ds->get_buffer());

		return *this;
	}

	ostream& operator << (ostream& os, const vector<token>& line)
	{
		bool first_token = true;

		for (const auto& t : line)
		{
			if (first_token)
				first_token = false;
			else
				os << ' ';

			os << t;
		}

		return os;
	}

	bool tabular_stream::read_line(token_stream& ts, vector<token>& line, bool allow_separator_at_end)
	{
		line.clear();

		token t;
		int token_number = 0;

		while (ts >> t)
		{
			if (t.get_type() == token::Type::EOL)
				if (line.size() == 0) // this is an empty / completely out-commented line?
					continue; // then try next line
				else
					break;

			// if separator is specified
			if (settings.field_separator)
			{
				bool token_is_separator = (t.parts[0].body.compare(field_separator_string) == 0);

				if ((token_number % 2) == 1) // then every even token must be a separator
				{
					tabular_assert(!token_is_separator, "every even token must be a separator", ts.get_buffer());
					token_number++;
					continue;
				}
				else // and every odd token must be NOT a separator
				{
					if (token_is_separator) // if nonetherless a separator:
						// this means, that two separators follow each other, that is,
						// there is an "empty" token between two separators
					{
						// so put separator token back to stream to be read later
						// and say that current token is an "empty string"
						ts << t;
						t = token(token::CreationOpt::AddEmptyPart);
					}
				}
			}

			token_number++;
			line.push_back(t);
		}

		if (token_number > 0) // if anything meaningful has been read
			if (settings.field_separator)
			{
				tabular_assert((!allow_separator_at_end) && ((token_number % 2) == 0), "separator token at the very end of the line", ts.get_buffer());
			
				// if separator at end is allowed AND is there, then add empty string as the very last token
				if ((token_number % 2) == 0)
					line.push_back(token(token::CreationOpt::AddEmptyPart));
			}

		return (line.size() > 0);
	}
}