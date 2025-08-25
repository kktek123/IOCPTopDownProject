#pragma once
class FileUtils
{
public:
	static Vector<BYTE>		ReadFile(const WCHAR* path);
	static String			Convert(std::string str);
};

