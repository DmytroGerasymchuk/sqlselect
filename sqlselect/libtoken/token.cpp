#include "token.h"

namespace libtoken
{
	void token::clear(char new_parts_separator)
	{
		parts_separator = new_parts_separator;
		parts.clear();
	}

	token::Type token::get_type() const
	{
		if (parts.size() == 0) // empty token
			return token::Type::Undefined;

		if (parts.size() > 1) // token consists of more than one part
			return token::Type::Multipart;

		if (parts[0].type == token_part::Type::Text) // token consists of one text part
			return token::Type::Text;

		if (parts[0].type == token_part::Type::EOL) // token consists of one part with newline character
			return token::Type::EOL;

		return token::Type::Atomic; // single-part token without any special meanings
	}
}