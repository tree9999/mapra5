#include "GreyScale.h"
#include <fstream>
#include <iostream>

int main () {
	std::ifstream ifs;
	ifs.open("/bilder/mapra.pgm");
	GreyScale test;
	ifs >> test;
	ifs.close();

	std::ofstream ofs("result1.pgm");
	ofs << test;
	ofs.close();
	return 0;
}
