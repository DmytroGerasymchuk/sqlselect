#include "token_part.h"

namespace libtoken
{
	ostream& operator << (ostream&os, const token_part& tp)
	{
		if (tp.qualified_by)
			os << tp.qualified_by;

		os << tp.body;

		if (tp.qualified_by)
			os << tp.qualified_by;

		return os;
	}
}