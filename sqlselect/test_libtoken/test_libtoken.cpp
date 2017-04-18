// test_libtoken.cpp : Defines the entry point for the console application.

#include <fstream>

#include "../libtoken/libtoken.h"

using namespace std;
using namespace libtoken;

void dump_raw_chars()
{
	cout << "line_buffer test: read whole file and dump raw characters" << endl << endl;

	ifstream ifs{ "../sqlselect/test_data/libtoken_test.txt" };
	if (!ifs)
		throw std::exception("ifs.open failed!");

	line_buffer line_buf{ ifs };

	cout << "[";

	while (char c = line_buf.getch())
		if (c == '\n')
			cout << "\\n";
		else
			cout << c;

	cout << "]" << endl << endl;
}

void dump_tokens()
{
	cout << "token_stream test: read whole file and dump tokens" << endl << endl;

	ifstream ifs{ "../sqlselect/test_data/libtoken_test.txt" };
	if (!ifs)
		throw std::exception("ifs.open failed!");

	token_stream ts{ token_stream_settings(token_stream_settings::Known::Sql), ifs };

	token t;

	while (ts >> t)
		cout << t << " ";

	cout << endl << endl;
}

void dump_tabular_onefile()
{
	cout << "tabular_stream test: read and dump CSV file with inside-header" << endl << endl;

	ifstream ifs{ "../sqlselect/test_data/tabstream_onefile.txt" };
	if (!ifs)
		throw std::exception("ifs.open failed!");

	tabular_stream_settings tss;
	tabular_stream ts{ tss, ifs, ifs };

	cout << ts.header << endl;

	vector<token> line;

	while (ts >> line)
		cout << line << endl;

	cout << endl;
}

int main()
{
	try
	{
		dump_raw_chars();
		dump_tokens();
		dump_tabular_onefile();
	}
	catch (const libtoken_exception& ex)
	{
		cout << "LIBTOKEN EXCEPTION: " << typeid(ex).name() << ": " << ex.what() << " at source line " << ex.cur_line_no << " pos " << ex.cur_line_pos << std::endl;
	}
	catch (const std::exception& ex)
	{
		cout << "EXCEPTION: " << ex.what() << std::endl;
	}

    return 0;
}

