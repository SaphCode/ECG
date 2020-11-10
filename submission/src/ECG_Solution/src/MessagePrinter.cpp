#include "MessagePrinter.h"

void MessagePrinter::PrintLog(std::vector<GLchar> log)
{
	std::string msg;
	for (auto& glc : log) {
		msg += glc;
	}
	std::cout << msg << "\n";
}
