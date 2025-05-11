
#include "Window.hpp"
#include "ProgramBaseState.hpp"
#include "CreateArcState.hpp"

int main(int argc, const char* argv[])
{
	Window* window = new Window(WindowConfiguration(1024, 768, "Graph builder"), OpenGLConfiguration(3, 3, OpenGLProfile::Core, 4, OpenGLColor{0.04f, 0.06f,0.03f,1.0f}));

	std::cerr << "init successed";

	window->start();

	delete window;
}