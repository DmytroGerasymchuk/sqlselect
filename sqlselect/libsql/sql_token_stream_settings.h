#pragma once

#include "../libtoken/libtoken.h"

namespace libsql
{
	struct sql_token_stream_settings : public libtoken::token_stream_settings
	{
		static const char end_of_stmt = ';';

		sql_token_stream_settings() : libtoken::token_stream_settings()
		{
			multipart_token_separator = '.';
			single_line_comment_prefix = "--";
			token_part_qualifier = '"';
			text_qualifier = '\'';
			special_tokens = { "+", "-", "/", "*", "[", "]", "(", ")", "=", "<>", ",", {end_of_stmt} };
		}
	};
}