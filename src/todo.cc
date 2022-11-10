#include "todo.hh"

ParseError::ParseError(const std::string &p_msg, size_t p_line):
	m_msg(p_msg),
	m_line(p_line)
{}

const std::string &ParseError::What() const {
	return m_msg;
}

size_t ParseError::Line() const {
	return m_line;
}

std::string ParseError::Format() const {
	return "Line " + std::to_string(m_line) + ": " + m_msg;
}

Todo::Todo(const std::string &p_desc, bool p_checked, size_t p_line, bool p_hasDash):
	desc(p_desc),
	checked(p_checked),

	line(p_line),
	hasDash(p_hasDash)
{}

#define WHITESPACES " \t\r\v\f"

static void SkipWhitespaces(size_t &idx, const std::string &p_str) {
	idx = p_str.find_first_not_of(WHITESPACES, idx);
}

static bool Advance(size_t &idx, const std::string &p_str) {
	++ idx;

	return idx < p_str.length();
}

Todo Todo::FromLine(const std::string &p_str, size_t p_line) {
	bool   hasDash = false;
	size_t idx     = 0;

	SkipWhitespaces(idx, p_str);
	if (idx == std::string::npos)
		throw ParseError("Line is empty", p_line);

	switch (p_str.at(idx)) {
	case '[': break;
	case '-':
		hasDash = true;

		if (not Advance(idx, p_str))
			throw ParseError("Line is not a todo", p_line);

		SkipWhitespaces(idx, p_str);
		if (idx == std::string::npos or p_str.at(idx) != '[')
			throw ParseError("Line is not a todo", p_line);

		break;

	default: throw ParseError("Line is not a todo", p_line);
	}

	if (not Advance(idx, p_str))
		throw ParseError("Todo is missing a matching ']'", p_line);

	bool checked;

	switch (p_str.at(idx)) {
	case 'X': case 'x': checked = true;  break;
	case ' ': case '.': checked = false; break;

	default:
		throw ParseError(std::string("Invalid character '") + p_str.at(idx) +
		                  "', expected 'x'/'X', '.'/' '", p_line);
	}

	if (not Advance(idx, p_str))
		throw ParseError("Todo is missing a matching ']'", p_line);

	if (p_str.at(idx) != ']')
		throw ParseError(std::string("Invalid character '") + p_str.at(idx) +
		                 "', expected matching ']'", p_line);

	if (not Advance(idx, p_str))
		throw ParseError("Todo is missing a description", p_line);

	SkipWhitespaces(idx, p_str);
	if (idx == std::string::npos)
		throw ParseError("Todo is missing a description", p_line);

	std::string desc = p_str.substr(idx);

	return Todo(desc, checked, p_line, hasDash);
}
