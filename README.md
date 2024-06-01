# vidscii

## Description

vidscii is a command-line tool that converts video files into ASCII art in real-time. It offers various options to customize the ASCII characters used, display the actual video alongside the ASCII output, and reverse the character set. This project leverages the OpenCV library for video processing and terminal control libraries for rendering ASCII art.

## Features

- Convert video files to ASCII art in real-time.
- Customize the ASCII character set.
- Option to display the actual video alongside the ASCII output.
- Option to reverse the ASCII character set.

## Requirements

- C++11 or later
- OpenCV 4.x or later
- Terminal control libraries (unistd.h, ioctl.h)

## Installation

1. Ensure you have OpenCV installed on your system.

2. Clone this repository:

   ```bash
   git clone git@github.com:SahooBishwajeet/vidscii.git
   ```

3. Navigate to the project directory:

   ```bash
   cd vidscii
   ```

4. Generate the Makefile:

   ```bash
   cmake .
   ```

5. Build the project:

   ```bash
    make
   ```

## Usage

```bash
    ./bin/main [options] <Video_Path>\n
```

Options

1. `-h`: Display this help message.
2. `-v`: Show the actual video alongside the ASCII output (default: false).
3. `-r`: Reverse the ASCII character set (default: false).
4. `-c <"charset">`: Specify the ASCII character set to use (default: '*+=-:. ').
