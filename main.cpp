#include <opencv2/opencv.hpp> 
#include <iostream>

#include <thread>
#include <chrono>

#include <sys/ioctl.h>
#include <unistd.h>
#include <getopt.h>

using namespace std;
using namespace cv; 

string convertToAscii(int intensity, const string &ASCII_CHARACTERS = "*+=-:.     ", bool rev = false) {
	string REV_ASCII_CHARACTERS;
	if(rev) {
		REV_ASCII_CHARACTERS = string(ASCII_CHARACTERS.rbegin(), ASCII_CHARACTERS.rend());
	}

	return rev == false ? string(2, ASCII_CHARACTERS[intensity * ASCII_CHARACTERS.size() / 256]) : string(2, REV_ASCII_CHARACTERS[intensity * REV_ASCII_CHARACTERS.size() / 256]);
}

void getTerminalSize(int &WIDTH, int &HEIGHT) {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	WIDTH = w.ws_col;
	HEIGHT = w.ws_row;
}

void printHelp() {
    cout << "Usage: ./bin/main [options] <Video_Path>\n"
         << "\nOptions:\n"
         << "  -h              Display this help message\n"
         << "  -v              Show the actual video alongside ASCII output (default: false)\n"
         << "  -r              Reverse the ASCII character set (default: false)\n"
         << "  -c <charset>    Specify the ASCII character set to use (default: '*+=-:.     ')\n"
         << "\nExample:\n"
         << "  ./bin/main -v -r -c '@%#*+=-:.     ' video.mp4\n";
}

int main(int argc, char** argv)  { 
	int option;
	bool showVideo = false;
	bool reverse = false;
	string asciiCharacters = "*+=-:.     ";

	while((option = getopt(argc, argv, "hvrc:")) != -1) {
		switch(option) {
			case 'h':
				printHelp();
				return 0;
			case 'v':
				showVideo = true;
				break;
			case 'r':
				reverse = true;
				break;
			case 'c':
				asciiCharacters = optarg;
				break;
			default:
			cout << "Usage: ./bin/main [-v] [-r] [-c <Ascii Characters Set>] <Video_Path>" << endl;
				return -1;
		}
	}

	if (optind >= argc) {
		printHelp();
		return -1;
	}

	string videoPath = argv[optind];
	VideoCapture cap(videoPath);
	
	if (!cap.isOpened()) { 
		cout << "Error opening video stream or file" << endl; 
		return -1; 
	}

	if(showVideo) {
		namedWindow("Frame", WINDOW_OPENGL);
	}

	// Find the fps of the video
	double fps = cap.get(CV_CAP_PROP_FPS);
	int frame_duration = 1000 / fps;

	int terminalWidth, terminalHeight;
	getTerminalSize(terminalWidth, terminalHeight);

	int frameWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int frameHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	int WIDTH = showVideo ? terminalWidth / 4 : terminalWidth / 2;	
	int HEIGHT = (WIDTH * frameHeight) / frameWidth * 0.5;

	Mat frame, resized_frame; 

	while (1) { 
		cap >> frame; 

		if (frame.empty()) { 
			break; 
		}

		// Resize the frame
		resize(frame, resized_frame, Size(WIDTH, HEIGHT), 0, 0, INTER_CUBIC);

		if(showVideo) {
			imshow("Frame", frame); 
		}

		string ascii_frame = "";
		
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				ascii_frame += convertToAscii(resized_frame.at<uchar>(i, j), asciiCharacters, reverse);
			}
			ascii_frame += "\n";
		}
		
		system("clear");
		cout << ascii_frame << endl;

		// this_thread::sleep_for(chrono::milliseconds(frame_duration));

		// Press  ESC on keyboard to exit
		char c = (char)waitKey(25); 
		if (c == 27) { 
			break; 
		}
	}

	return 0; 
}
