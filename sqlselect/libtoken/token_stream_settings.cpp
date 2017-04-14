#include "token_stream_settings.h"

namespace libtoken
{
	token_stream_settings::token_stream_settings(Known ks)
	{
		switch (ks)
		{
			case Known::Sql:
				multipart_token_separator = '.';
				single_line_comment_prefix = "--";
				token_part_qualifier = '"';
				text_qualifier = '\'';
				special_tokens = { "+", "-", "/", "*", "[", "]", "(", ")", "=", "<>", ",", ";" };
				break;
			default:
				break;
		}
	}
}