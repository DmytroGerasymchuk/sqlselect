#include "token_part.h"

namespace libtoken
{
	void token_part::clear()
	{
		type = Type::General;
		qualified_by = '\x00';
		body.clear();
	}
}