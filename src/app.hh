#ifndef APP_HH__HEADER_GUARD__
#define APP_HH__HEADER_GUARD__

#include <string>    // std::string, std::getline
#include <vector>    // std::vector
#include <cstdlib>   // std::size_t
#include <ostream>   // std::ostream
#include <iostream>  // std::cerr, std::cout, std::endl
#include <fstream>   // std::ifstream, std::ofstream
#include <clocale>   // std::setlocale
#include <stdexcept> // std::runtime_error

#include <ncurses.h>

#include "todo.hh"

#define APP_NAME "cxxdo"

using std::size_t;

namespace TUI {
	void Start();
	void End();
}

class App {
public:
	static void Usage(std::ostream &p_stream = std::cout);

	App(const std::string &p_path, std::ostream &p_logStream = std::cerr);
	~App();

	void Run();

private:
	void Render(int p_x, int p_y);
	void Input();

	void CalcOffset();

	std::ostream &m_logStream;

	std::string m_path;

	std::vector<Todo>        m_list;
	std::vector<std::string> m_content;

	size_t m_pos;
	size_t m_winX, m_winY;

	int m_startX, m_startY;

	bool m_modified, m_quit;
};

#endif
