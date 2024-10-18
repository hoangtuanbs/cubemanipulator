// Include standard headers
#include <iostream>

#include "engine.hpp"


int main( void )
{
	Engine engine("Cube Demo", 1024, 768);

	if (!engine.initialize())
	{
		std::cout << "Unable to initialize Cube Engine." << std::endl;

		return -1;
	}

	if (!engine.load())
	{
		std::cout << "Unable to load Cube Resources." << std::endl;

		return -1;
	}
	
    engine.run();

	return 0;
}
