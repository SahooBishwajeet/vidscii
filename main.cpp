#include <opencv2/opencv.hpp> 
#include <iostream> 

#include <thread>
#include <chrono>
#include <sys/ioctl.h>
#include <unistd.h>

using namespace std;
using namespace cv; 

string convertToAscii(int intensity) {
	const string ASCII_CHARACTERS = "*+=-:.     ";
	// const string ASCII_CHARACTERS = "     .:-+=*";
	// const string ASCII_CHARACTERS = "@%#*+=-:.     ";
	// const string ASCII_CHARACTERS = "     .:-+=*#%@";

	return string(2, ASCII_CHARACTERS[intensity * ASCII_CHARACTERS.size() / 256]);
}

void getTerminalSize(int &WIDTH, int &HEIGHT) {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	WIDTH = w.ws_col;
	HEIGHT = w.ws_row;
}

int main(int argc, char** argv)  { 
	if (argc != 2) { 
		printf("usage: ./main <Video_Path>\n"); 
		return -1; 
	}

	VideoCapture cap(argv[1]);
	if (!cap.isOpened()) { 
		cout << "Error opening video stream or file" << endl; 
		return -1; 
	}

	namedWindow("Frame", WINDOW_OPENGL);

	// Find the fps of the video
	double fps = cap.get(CV_CAP_PROP_FPS);
	int frame_duration = 1000 / fps;

	int terminalWidth, terminalHeight;
	getTerminalSize(terminalWidth, terminalHeight);

	// WIDTH = terminalWidth / 4;
	// HEIGHT = terminalHeight / 4;

	int frameWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int frameHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	// int WIDTH = terminalWidth / 2;
	int WIDTH = terminalWidth / 4; // When using namedWindow & imshow
	int HEIGHT = (WIDTH * frameHeight) / frameWidth * 0.5;

	Mat frame, resized_frame; 

	while (1) { 
		cap >> frame; 

		if (frame.empty()) { 
			break; 
		}

		// Resize the frame
		resize(frame, resized_frame, Size(WIDTH, HEIGHT), 0, 0, INTER_CUBIC);

		imshow("Frame", frame); 

		// Press  ESC on keyboard to exit
		char c = (char)waitKey(25); 
		if (c == 27) { 
			break; 
		}

		// Ascii print
		string ascii_frame = "";
		
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				ascii_frame += convertToAscii(resized_frame.at<uchar>(i, j));
			}
			ascii_frame += "\n";
		}
		
		system("clear");
		cout << ascii_frame << endl;

		// this_thread::sleep_for(chrono::milliseconds(frame_duration));
	}

	return 0; 
}
