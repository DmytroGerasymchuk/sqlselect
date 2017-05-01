#pragma once

#include <vector>
#include <string>

namespace libtoken
{
	using namespace std;

	struct token_stream_settings
	{
		/*
			Usage hints:

			if newline signals should not be skipped, but parsed as tokens,
			set "newline_as_token" to true; otherwise, leave it set to false!

			if multi-part-tokens are expected (like Schema.Object etc.),
			then specify own "multipart_token_separator"; otherwise, leave it set to 0x00!

			if comments at the end of line may appear (like -- or //),
			then specify the own "single_line_comment_prefix" for such comments; otherwise, leave it empty!


			if token parts may contain spaces etc. and are therefore qualified
			(for example, with "..."), then specify own "token_part_qualifier"; otherwise, leave it set to 0x00!


			if text may explicitly contain spaces etc. and therefore must be qualified
			(for example, with '...'), then specify own "text_qualifier"; otherwise, leave it set to 0x00!
		*/

		bool newline_as_token = false;
		char multipart_token_separator = '\x00';
		string single_line_comment_prefix;
		char token_part_qualifier = '\x00';
		char text_qualifier = '\x00';

		// special tokens like "+", "-", "[", "]", "(", ")", ...
		vector<string> special_tokens;

		token_stream_settings() {};
		virtual ~token_stream_settings() {};
	};
}