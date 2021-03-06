#include <cstring>
#include <cstdlib>
#include <iostream>

#ifdef __linux__
#include <unistd.h>
#endif

#include "State.hpp"
#include "Graphics.hpp"

#include "Snake.hpp"
#include "Wall.hpp"
#include "Fruit.hpp"

bool
	State::pause = true,
	State::running = false;

int
	State::latency = 40,
	State::latency_delta = 0,

	State::skin_id = 0,

	State::width = 50,
	State::height = 50;

double
	State::x_Display = double(WIDTH) * 1.4,
	State::y_Display = double(HEIGHT) * 0.5;

std::string
	State::folder = "";

Object
	*WALLS,
	*FRUITS,
	*SNAKE;

#ifdef __linux__
static std::string get_executable_path() {
  std::vector<char> buf(1000);
  readlink("/proc/self/exe", buf.data(), 1000);
  return std::string(buf.begin(), buf.end());
}
#endif

void State::Init(const int &argc, char **argv) {
	WIDTH = (argc >= 3) ? atoi(argv[1]) : 50;
	HEIGHT = (argc >= 3) ? atoi(argv[2]) : 50;

#ifdef __linux__
  const std::string exec = get_executable_path();
#else
  const std::string exec = argv[0];
#endif
  std::string::size_type n = exec.rfind('/');
  if(n == std::string::npos) {
    folder = "./";
  } else {
    folder = exec.substr(0, n + 1);
  }

	x_Display = WIDTH * 1.4;
	y_Display = HEIGHT + 0.5;
}


void State::Keyboard(char key) {
	switch(key) {
		case 27 :
			Graphics::Quit();
		break;
		case 'p' :
			pause = !pause;
		break;
		case 'r' :
			running = !running;
		break;
		case 'o' :
			if(latency_delta > 0)
				latency_delta = 0;
			latency = (latency > (abs(--latency_delta) >> 3) + 1) ? (latency - ((abs(latency_delta) >> 3) + 1)) : 0;
		break;
		case 'i' :
			if(latency_delta < 0)
				latency_delta = 0;
			latency += ((++latency_delta) >> 3) + 1;
		break;
	}

	if(strchr("0123456789", key)) {
		latency = (key == '0') ? 40 : 0;
		++skin_id %= NO_ICONSETS;
	}

	SNAKE->Keyboard(key);
	if(running)
		SSNAKE->DoStep();
	WALLS->Keyboard(key);
	FRUITS->Keyboard(key);
}

void State::Display() {
	char text[20];

	glColor3f(1.0f, 1.0f, 1.0f);
	sprintf(text, "Latency: %d", State::latency);
	Graphics::DisplayText(WIDTH + 1, HEIGHT * 0.89, text);

	glColor3f(0.3f, 0.3f, 1.0f);
	if(State::pause) {
		sprintf(text, "[Paused]");
		Graphics::DisplayText(WIDTH * 0.45, HEIGHT * 0.75, text);
	}

	glColor3f(1.0f, 0.3f, 0.3f);
	if(State::running) {
		sprintf(text, "[Running]");
		Graphics::DisplayText(WIDTH * 0.45, HEIGHT * 0.25, text);
	}
}

void State::Timer() {
	if(!State::pause) {
		if(!State::running)
			SSNAKE->DoStep();
		if(
			Ball::InSegment(SNAKE->GetObjects().front(), WALLS->GetObjects())
			|| Ball::InSegment(SNAKE->GetObjects().front(), SNAKE->GetObjects(), Ball(1, SNAKE->GetObjects().size()))
		)
		{
			std::cout << std::to_string(FFRUITS->frufru) << std::endl;
			Graphics::Quit();
		}
	}
	if(Ball::InSegment(SNAKE->GetObjects().front(), FRUITS->GetObjects()))
		FFRUITS->EatFruit();

	SSNAKE->SetStep(SSNAKE->GetAutoStep());
}
