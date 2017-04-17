#include "tabular_stream.h"

namespace libtoken
{
	tabular_stream::tabular_stream(const tabular_stream_settings& ts_settings, istream& header_is, istream& data_is)
	{
		settings = ts_settings;

		token_stream_settings tss;

		tss.text_qualifier = settings.text_qualifier;
		if (settings.field_separator) // if field separator not just space
			tss.special_tokens.push_back(string().append(1, settings.field_separator));
		tss.single_line_comment_prefix = settings.single_line_comment_prefix;

		tss.newline_as_token = true;

		token_stream h_stream = token_stream(tss, header_is);
		read_line(h_stream, header);
	}

	void tabular_stream::read_line(token_stream& ts, vector<token>& line)
	{

	}
}