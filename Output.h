#pragma once

#include <fstream>

class Output
{
public:
	static void open() { file.open("output.txt", std::ios::out); }

	static void print(float a, float b, float c, float d) 
	{
		file << a << "\t" << b << "\t" << c << "\t" << d << std::endl;
	}

	static void close() { file.close(); }
	static bool isOpen() { return file.is_open(); }
private:
	static std::fstream file;
};

std::fstream Output::file;