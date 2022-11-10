#include "app.hh"

namespace TUI {
	void Start() {
		std::setlocale(LC_CTYPE, "");

		initscr();
		raw();
		noecho();
		keypad(stdscr, true);

		curs_set(0);

		ESCDELAY = 0;
	}

	void End() {
		curs_set(1);

		endwin();
	}
}

void App::Usage(std::ostream &p_stream) {
	p_stream << "Usage: " APP_NAME " [FILE | OPTION]\n"
	         << "OPTIONS:\n"
	         << " --help, -h    Show this message"
	         << std::endl;
}

App::App(const std::string &p_path, std::ostream &p_logStream):
	m_logStream(p_logStream),

	m_path(p_path),
	m_pos(0),

	m_modified(false),
	m_quit(false)
{
	TUI::Start();

	getmaxyx(stdscr, m_winY, m_winX);

	putp(("\033]0;cxxdo - '" + p_path + "'\007\n").c_str());

	std::ifstream file(m_path);

	std::string line;
	size_t      row = 0;
	while (std::getline(file, line)) {
		try {
			Todo todo = Todo::FromLine(line, row);

			m_list.push_back(todo);
		} catch (const ParseError &p_err) {
			m_logStream << p_err.Format() << std::endl;
		}

		m_content.push_back(line);

		++ row;
	}

	file.close();

	if (m_list.empty())
		throw std::runtime_error("File '" + p_path + "' does not contain a todo");

	CalcOffset();
}

App::~App() {
	if (m_modified) {
		for (const auto &todo : m_list) {
			m_content.at(todo.line) = std::string("[") + (todo.checked? "X" : " ") +
			                          "] " + todo.desc;

			if (todo.hasDash)
				m_content.at(todo.line) = "- " + m_content.at(todo.line);
		}

		std::ofstream file(m_path);

		for (const auto &line : m_content)
			file << line << '\n';
	}

	TUI::End();
}

void App::Run() {
	while (not m_quit) {
		Render(m_startX, m_startY);
		Input();
	}
}

void App::Render(int p_x, int p_y) {
	erase();
	move(0, 0);

	addstr(" Q - Quit   Space - Mark");

	move(p_y, p_x);
	addstr("TODO:");

	++ p_y;

	size_t i = 0;
	for (const auto &todo : m_list) {
		move(p_y + i, p_x);

		if (i == m_pos)
			attron(A_REVERSE);

		addch('[');
		addch(todo.checked? 'X' : ' ');
		addstr("] ");
		addstr(todo.desc.c_str());

		if (i == m_pos)
			attroff(A_REVERSE);

		++ i;
	}
}

void App::Input() {
	int in = getch();

	switch (in) {
	case 'q': m_quit = true; break;

	case KEY_RESIZE:
		getmaxyx(stdscr, m_winY, m_winX);
		CalcOffset();

		break;

	case KEY_UP:
		if (m_pos > 0)
			-- m_pos;

		break;

	case KEY_DOWN:
		if (m_pos < m_list.size() - 1)
			++ m_pos;

		break;

	case ' ':
		if (not m_modified)
			m_modified = true;

		m_list.at(m_pos).checked = not m_list.at(m_pos).checked;

		break;

	default: break;
	}
}

void App::CalcOffset() {
	size_t longest = 0;
	for (const auto &todo : m_list) {
		if (todo.desc.length() > longest)
			longest = todo.desc.length();
	}

	m_startX = m_winX / 2 - longest / 2 - 2; // 2 is half of the length of the prefix "[ ] "
	m_startY = m_winY / 2 - m_list.size() / 2 - 1;

	if (m_startX < 0 or m_startX >= static_cast<int>(m_winX) or
	    m_startY < 0 or m_startY >= static_cast<int>(m_winY))
		throw std::runtime_error("Window size is too small for the TODOs to fit in");
}
