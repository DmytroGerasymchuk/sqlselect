#pragma once

#include <iostream>

namespace libtoken
{
	using namespace std;

	class line_buffer
	{
		public:
			line_buffer(istream& is);

			bool failed() const { return in_stream.bad(); };
			bool eof() const { return in_stream.eof(); };
			bool fetchable() const { return !(failed() || eof()); }

			char getch();
			void ungetch(int num_chars = 1);
			void skip_to_eol();

		private:
			istream& in_stream;
			string current_line;
			int buf_pos;

			void check_cons() const;
	};
}