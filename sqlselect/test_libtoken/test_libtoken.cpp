// test_libtoken.cpp : Defines the entry point for the console application.

#include <fstream>

#include "../libtoken/libtoken.h"
#include "../libtabular/libtabular.h"

using namespace std;

void dump_raw_chars()
{
	using namespace libtoken;

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
	using namespace libtoken;

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
	using namespace libtabular;

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

void dump_tabular_sepfiles()
{
	using namespace libtabular;

	cout << "tabular_stream test: read and dump space-separated file with separate header" << endl << endl;

	ifstream ifs_h{ "../sqlselect/test_data/tabstream_sep_header.txt" };
	if (!ifs_h) throw std::exception("ifs_h.open failed!");
	ifstream ifs_d{ "../sqlselect/test_data/tabstream_sep_data.txt" };
	if (!ifs_d) throw std::exception("ifs_d.open failed!");

	tabular_stream_settings tss;
	tss.field_separator = '\x00';
	tabular_stream ts{ tss, ifs_h, ifs_d };

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
		dump_tabular_sepfiles();

		cout << "End-of-program." << endl;
	}
	catch (const libtoken::libtoken_exception& ex)
	{
		cout << "LIBTOKEN EXCEPTION: " << typeid(ex).name() << ": " << ex.what() << " at source line " << ex.cur_line_no << " pos " << ex.cur_line_pos << std::endl;
	}
	catch (const libtabular::tabular_stream::tabularization_error& ex)
	{
		cout << "TABULARIZATION ERROR: " << ex.what() << " at source line " << ex.cur_line_no << " pos " << ex.cur_line_pos << std::endl;
	}
	catch (const libtabular::libtabular_exception& ex)
	{
		cout << "LIBTABULAR EXCEPTION: " << typeid(ex).name() << ": " << ex.what() << std::endl;
	}
	catch (const std::exception& ex)
	{
		cout << "EXCEPTION: " << ex.what() << std::endl;
	}

    return 0;
}

