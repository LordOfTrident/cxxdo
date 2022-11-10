#include <iostream> // std::cerr, std::cout
#include <cstdlib>  // EXIT_FAILURE, EXIT_SUCCESS

#include "app.hh"

int main(int p_argc, char **p_argv) {
	if (p_argc != 2) {
		if (p_argc <= 1)
			std::cerr << "Error: Expected 1 argument\n\n";
		else
			std::cerr << "Error: Unexpected argument '" << p_argv[p_argc - 1] << "'\n\n";

		App::Usage(std::cerr);

		return EXIT_FAILURE;
	}

	std::string arg = p_argv[1];

	if (arg == "-h" or arg == "--help") {
		App::Usage();

		return EXIT_SUCCESS;
	}

	try {
		App app(p_argv[1]);

		app.Run();
	} catch (const std::runtime_error &p_err) {
		TUI::End();
		std::cerr << "Error: " << p_err.what() << std::endl;

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
