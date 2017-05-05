#include "ofApp.h"
#include <algorithm>
#include <chrono>
using namespace std::chrono;


namespace {
	std::vector<std::string> GetFiles(const char* folderPath)
	{
		std::vector<std::string> out;

		char path[MAX_PATH];
		WIN32_FIND_DATAA ffd;

		if (strlen(folderPath) > MAX_PATH - 4)
		{
			return out;
		}

		strcpy(path, folderPath);
		if (path[strlen(path) - 1] != '\\')
			strcat(path, "\\");
		strcat(path, "*.*");

		HANDLE hFind = FindFirstFileA(path, &ffd);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			return out;
		}

		bool finished = false;
		while (!finished)
		{

			if (strcmp(ffd.cFileName, ".") != 0 &&
				strcmp(ffd.cFileName, "..") != 0)
			{
				out.push_back(ffd.cFileName);
			}
			
			if (!FindNextFileA(hFind, &ffd))
			{
				finished = true;
			}
		}
		return out;
	}

	std::string NarrowString(const std::wstring& str, const char* localeName = "japanese")
	{
		std::string result;
		result.resize(str.size());

		std::locale loc(localeName);

		std::use_facet<std::ctype<wchar_t> >(loc).narrow(
			str.c_str(), str.c_str() + str.size(), '?', &*result.begin());

		return result;
	}
}

struct PlayInfo
{
	size_t fileNum;
	std::vector<string> fileNames;
	std::vector<double> times;

	std::vector<string>::const_iterator itName;
	std::vector<double>::const_iterator itTime;

	system_clock::time_point curr;
	system_clock::time_point prev;


	ofImage img;

	size_t duration;	//!< millis
};

static PlayInfo playInfo;

//--------------------------------------------------------------
void ofApp::setup()
{
	auto files = GetFiles(m_folderPath.c_str());
	std::random_shuffle(files.begin(), files.end());

	playInfo.fileNum = files.size();
	playInfo.fileNames = files;
	// トータル再生時間（ミリ秒）
	playInfo.duration = 30 * 60 * 1000;

	ofSeedRandom(12345);
	for (size_t i = 0; i < playInfo.fileNum; ++i)
	{
		// 適当に画像の表示長さを決定
		double v = ofRandom(0.0, 8.0);
		cout << "noise = " << v << endl;

		playInfo.times.push_back(v);
	}

	double sum = std::accumulate(playInfo.times.begin(), playInfo.times.end(), 0.0);

	double rate = playInfo.duration / sum;

	// 尺に合うように各画像の再生時間を調整
	for (double& d : playInfo.times)
	{
		d *= rate;
	}

	// 確認
	double sum2 = std::accumulate(playInfo.times.begin(), playInfo.times.end(), 0.0);

	playInfo.itName = playInfo.fileNames.begin();
	playInfo.itTime = playInfo.times.begin();

	playInfo.curr = system_clock::now();

	
	auto path = m_folderPath + "\\" + *(playInfo.itName);
	std::cout << path << std::endl;
	playInfo.img.load(path);
}

//--------------------------------------------------------------
void ofApp::update(){

	auto span = system_clock::now() - playInfo.curr;
	auto millis = duration_cast<milliseconds>(span).count();

	if (millis > *(playInfo.itTime))
	{
		std::cout << "(s:" << millis << ", " << *(playInfo.itTime) << ")" << std::endl;
		playInfo.curr = system_clock::now();
		++(playInfo.itTime);
		++(playInfo.itName);
		playInfo.img.load(m_folderPath + "\\" + *(playInfo.itName));

		std::cout << "fname = " << *(playInfo.itName) << std::endl << std::endl;
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofClear(ofColor::white);

	ofPoint wsz = ofGetWindowSize();

	int w = wsz.x;
	int h = wsz.x * (9.0 / 16.0);

	int t = (wsz.y - h) / 2;
	
	playInfo.img.draw(0, t, w, h);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::SetFolderPath(std::string folderPath)
{
	m_folderPath = folderPath;
}

