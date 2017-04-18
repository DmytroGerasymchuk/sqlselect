#pragma once

#include <string>

namespace libtoken
{
	using namespace std;

	struct tabular_stream_settings
	{
		char qualifier = '"';
		char field_separator = ';';
		string single_line_comment_prefix = "#";
	};
}