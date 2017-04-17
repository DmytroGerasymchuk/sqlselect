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
			void ungetch(unsigned int num_chars = 1);
			void skip(unsigned int num_chars = 1);
			void skip_to_eol();

			int get_cur_line_no() const { return line_no; }
			int get_cur_line_pos() const { return buf_pos;  }

			char eol_signal() const { return '\n'; }

		private:
			istream& in_stream;
			string current_line;
			int buf_pos;
			int line_no;

			void check_cons() const;
	};
}