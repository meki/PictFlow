#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main(int argc, char** argv)
{
	auto app = new ofApp();

	int width = 1600, height = 900;
	if (argc >= 3)
	{
		int width = std::stoi(argv[1]);
		int height = std::stoi(argv[2]);
	}
	// ofSetupOpenGL(width, height, OF_FULLSCREEN);			// <-------- setup the GL context
	ofSetupOpenGL(width, height, OF_WINDOW);			// <-------- setup the GL context

	std::string folderPath = R"(.\img)";

	if (argc >= 4)
	{
		folderPath = std::string(argv[3]);
	}

	app->SetFolderPath(folderPath);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(app);

}
