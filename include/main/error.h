#pragma once
#include <string>

class Error 
{
private:
	std::string message;
protected:
	Error(std::string const& msg) :message(msg) {}
public:
	Error() = delete;
	const std::string GetMsg()const { return message.empty() ? "No error found" : message; }
};