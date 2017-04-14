#pragma once

#include <vector>
#include <string>

namespace libtoken
{
	using namespace std;

	struct token_stream_settings
	{
		// if multi-part-tokens are expected (like Schema.Object etc.),
		// then specify the parts separator;
		// otherwise, leave set to 0x00!
		char multipart_token_separator = '\x00';

		// if comments at the end of line may appear (like -- or //),
		// then specify the prefix for such comments;
		// otherwise, leave empty!
		string single_line_comment_prefix;

		// if token parts may contain spaces etc. and are therefore qualified
		// (for example, with "..."), then specify here;
		// otherwise, leave set to 0x00!
		char token_part_qualifier = '\x00';

		// if text may contain spaces etc. and is therefore qualified
		// (for example, with '...'), then specify here;
		// otherwise, leave set to 0x00!
		char text_qualifier = '\x00';

		// special tokens like "+", "-", "[", "]", "(", ")", ...
		vector<string> special_tokens;

		enum class Known { Sql };

		token_stream_settings() {};
		token_stream_settings(Known ks);
	};
}