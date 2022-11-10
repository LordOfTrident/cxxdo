#ifndef TODO_HH__HEADER_GUARD__
#define TODO_HH__HEADER_GUARD__

#include <string>    // std::string
#include <vector>    // std::vector
#include <stdexcept> // std::exception
#include <cstdlib>   // std::size_t

#define CHECK_STR   "X"
#define UNCHECK_STR " "

using std::size_t;

class ParseError : std::exception {
public:
	ParseError(const std::string &p_msg, size_t p_line);

	const std::string &What() const;
	size_t             Line() const;

	std::string Format() const;

private:
	std::string m_msg;
	size_t      m_line;
};

struct Todo {
	static Todo FromLine(const std::string &p_str, size_t p_line);

	Todo(const std::string &p_desc, bool p_checked, size_t p_line, bool p_hasDash = false);

	std::string desc;
	bool        checked;

	size_t line;
	bool   hasDash;
};

#endif
