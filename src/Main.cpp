// *****************************************
// 
// No headers, no other files, only Main.cpp
// 
// *****************************************

#define DISABLED_WARNINGS 6001 4715 4083

#pragma warning(disable : DISABLED_WARNINGS)

#define LOG(x) std::cout << x << std::endl;
#define DEBUG_MODE
#define UNEXPECTED_EXCEPTION throw_exception(TypeOfError::UnExpected, er.what());

// Why not
#ifdef DEBUG_MODE
#if __cplusplus != 199711L
#define LOG_VERION_WARNING std::cout << "Warning: original code was written in C++98. Now you are using " << __cplusplus << " verion" << std::endl;
#endif
#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <exception>

std::fstream fs;

enum class TypeOfVar {
	Integer,
	Boolean,
	String
};

enum class TypeOfLibrary {
	Base,
	Credits
};

enum class TypeOfConditionalStatement {
	If,
	Else
};

enum TypeOfError {
	LibraryParse,
	SyntaxParse,
	MethodParse,
	ConditionalStatementParse,
	UnExpected
};

// Method for showing exceptions
void throw_exception(TypeOfError error, std::string message) {
	switch (error) {
	case TypeOfError::LibraryParse: {
		LOG("[Library Parse Error!] " << message);
		break;
	}
	case TypeOfError::SyntaxParse: {
		LOG("[Variable Parse Error!] " << message);
		break;
	}
	case TypeOfError::MethodParse: {
		LOG("[Method Parse Error!] " << message);
		break;
	}
	case TypeOfError::ConditionalStatementParse: {
		LOG("[Conditional Statement Parse Error!] " << message);
		break;
	}
	case TypeOfError::UnExpected: {
		LOG("[UnExpected Error!] " << message);
		break;
	}
	}
	exit(1);
}

bool wasIfStatement = false;
bool ifStatementValue = false;

#pragma region Vars
std::map<std::string, int> intVars;
std::map<std::string, bool> boolVars;
std::map<std::string, std::string> stringVars;

// Method for parsing Boolean variables
bool get_var_bool(std::string boolString) {
	try {
		if (boolString == "true") return true;
		else if (boolString == "false") return false;
		else throw_exception(TypeOfError::SyntaxParse, "Incorrect boolean value");
	}
	catch (std::exception& er) {
		UNEXPECTED_EXCEPTION;
	}
}

// Method for parsing variables
void parse_var(TypeOfVar type, std::string output) {
	try {
		wasIfStatement = false;
		int i;
		if (type == TypeOfVar::Integer) i = 4;
		else if (type == TypeOfVar::String) i = 4;
		else if (type == TypeOfVar::Boolean) i = 5;

		int i2 = i;
		while (output[i2] != '=') i2++;

		int i3 = i2 - 1;
		std::string finalVarName = "";
		while (output[i] != output[i3]) {
			finalVarName += output[i];
			i++;
		}

		std::string value = "";
		i += 3;
		while (output[i] != ';') {
			value += output[i];
			i++;
		}

		if (type == TypeOfVar::Integer) intVars[finalVarName] = std::stoi(value);
		else if (type == TypeOfVar::Boolean) boolVars[finalVarName] = get_var_bool(value);
		else if (type == TypeOfVar::String) stringVars[finalVarName] = value;
	}
	catch (std::exception& er) {
		UNEXPECTED_EXCEPTION;
	}
}

bool use_library_base = false;
bool use_library_credits = false;

// Method for parsing libraries
void parse_library(std::string output) {
	try {
		wasIfStatement = false;
		int i = 5;
		std::string libName = "";
		while (output[i] != ';') {
			libName += output[i];
			i++;
		}
		if (libName == "base") use_library_base = true;
		else if (libName == "credits") use_library_credits = true;
		else throw_exception(TypeOfError::LibraryParse, "No library with this name was found!");
	}
	catch (std::exception& er) {
		UNEXPECTED_EXCEPTION;
	}
}

// Method for parsing base.log()
void parse_base_log_method(int& i, std::string output) {
	try {
		std::string varName = "";
		i += 1;
		while (output[i] != ')') {
			varName += output[i];
			i++;
		}

		for (const auto& element : intVars) {
			if (element.first == varName) {
				LOG(element.second);
				return;
			}
		}
		for (const auto& element : boolVars) {
			if (element.first == varName) {
				LOG(element.second);
				return;
			}
		}
		for (const auto& element : stringVars) {
			if (element.first == varName) {
				LOG(element.second);
				return;
			}
		}
	}
	catch (std::exception& er) {
		UNEXPECTED_EXCEPTION;
	}
}

// Method for parsing credits.show()
void parse_credits_show_method() {
	LOG("Author: HardCodeDev");
	LOG("License: MIT");
	LOG("Why did I do this?: Cuz I was boring");
	LOG("Btw this method more for testing libraries and methods. So yeah, that's show() method");
}

// Method for parsing methods from libraries
void parse_library_method(TypeOfLibrary type, std::string output) {
	try {
		wasIfStatement = false;
		if (type == TypeOfLibrary::Base) {
			if (use_library_base) {
				int i = 5;
				std::string methodName = "";
				while (output[i] != '(') {
					methodName += output[i];
					i++;
				}
				if (methodName == "log") parse_base_log_method(i, output);
				else throw_exception(TypeOfError::MethodParse, "No method with this name was found in base library!");
			}
			else throw_exception(TypeOfError::MethodParse, "base was not imported! Add \"#use base\"");
		}
		if (type == TypeOfLibrary::Credits) {
			if (use_library_credits) {
				int i = 8;
				std::string methodName = "";
				while (output[i] != '(') {
					methodName += output[i];
					i++;
				}
				if (methodName == "show") parse_credits_show_method();
				else throw_exception(TypeOfError::MethodParse, "No method with this name was found in credits library!");
			}
			else throw_exception(TypeOfError::MethodParse, "credits was not imported! Add \"#use credits\"");
		}
	}
	catch (std::exception& er) {
		UNEXPECTED_EXCEPTION;
	}
}

// Method for parsing If and Else
void parse_conditional_statement(TypeOfConditionalStatement type, std::string output) {
	try {
		if (type == TypeOfConditionalStatement::If) {
			wasIfStatement = true;
			int i = 3;
			std::string firstVar = "";
			while (output.substr(i, 3) != " ==") {
				firstVar += output[i];
				i++;
			}
			i += 4;
			std::string secondVar = "";

			while (output[i] != ')')
			{
				secondVar += output[i];
				i++;
			}

			ifStatementValue = false;

			//
			if (intVars.count(secondVar) && intVars.count(firstVar)) {
				if (intVars[secondVar] == intVars[firstVar]) ifStatementValue = true;
				else ifStatementValue = false;
			}
			else if (stringVars.count(secondVar) && stringVars.count(firstVar)) {
				if (stringVars[secondVar] == stringVars[firstVar]) ifStatementValue = true;
				else ifStatementValue = false;
			}
			else if (boolVars.count(secondVar) && boolVars.count(firstVar)) {
				if (boolVars[secondVar] == boolVars[firstVar]) ifStatementValue = true;
				else ifStatementValue = false;
			}

			if (ifStatementValue) {

				std::string methodName = "";
				i += 2;
				while (output[i] != '(') {
					methodName += output[i];
					i++;
				}

				if (methodName == "base.log") {
					if (use_library_base) parse_base_log_method(i, output);
					else throw_exception(TypeOfError::MethodParse, "base was not imported! Add \"#use base\"");
				}
				else if (methodName == "credits.show") {
					if (use_library_base) parse_credits_show_method();
					else throw_exception(TypeOfError::MethodParse, "credits was not imported! Add \"#use credits\"");
				}
				else throw_exception(TypeOfError::ConditionalStatementParse, "No method with this name was found!");
			}
		}
		else if (type == TypeOfConditionalStatement::Else) {
			if (wasIfStatement) {
				if (!ifStatementValue) {
					std::string methodName = "";
					int i = 5;
					while (output[i] != '(') {
						methodName += output[i];
						i++;
					}

					if (methodName == "base.log") {
						if (use_library_base) parse_base_log_method(i, output);
						else throw_exception(TypeOfError::MethodParse, "\"base\" was not imported! Add \"#use base\"");
					}
					else if (methodName == "credits.show") {
						if (use_library_base) parse_credits_show_method();
						else throw_exception(TypeOfError::MethodParse, "credits was not imported! Add \"#use credits\"");
					}
					else throw_exception(TypeOfError::ConditionalStatementParse, "No method with this name was found in base library!");
				}
				wasIfStatement = false;
			}
			else throw_exception(TypeOfError::ConditionalStatementParse, "No \"if\" statement was found before!");
		}
	}
	catch (std::exception& er) {
		UNEXPECTED_EXCEPTION;
	}
}

void parse(std::string path) {
	try {
		fs.open(path, std::fstream::in);

		std::string output;

		while (std::getline(fs, output)) {
			std::string firstThree = output.substr(0, 3);
			std::string firstFour = output.substr(0, 4);
			std::string firstFive = output.substr(0, 5);
			std::string firstTwo = output.substr(0, 2);
			std::string firstSeven = output.substr(0, 8);

			if (firstFour == "#use") parse_library(output);
			else if (firstFive == "base.") parse_library_method(TypeOfLibrary::Base, output);
			else if (firstSeven == "credits.") parse_library_method(TypeOfLibrary::Credits, output);

			else if (firstThree == "int") parse_var(TypeOfVar::Integer, output);
			else if (firstThree == "str") parse_var(TypeOfVar::String, output);
			else if (firstFour == "bool") parse_var(TypeOfVar::Boolean, output);

			else if (firstTwo == "if") parse_conditional_statement(TypeOfConditionalStatement::If, output);
			else if (firstFour == "else") parse_conditional_statement(TypeOfConditionalStatement::Else, output);

			else if (firstTwo == "//") continue;
		}
		fs.close();
	}
	catch (std::exception& er) {
		UNEXPECTED_EXCEPTION;
	}
}
int main() {
#ifdef DEBUG_MODE
	std::cout << std::boolalpha;
#endif

#ifdef LOG_VERION_WARNING
	LOG_VERION_WARNING;
#endif

	parse("test.txt");

	return 0;
}