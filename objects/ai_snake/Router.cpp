#include "Router.hpp"
#include "Functions.hpp"
#include "Graphics.hpp"
#include "State.hpp"
#include "Snake.hpp"
#include "Aimer.hpp"

#include <cassert>
#include <iostream>

const int Router::NO_ROUTES = 5;

Router::Router(Aimer *a):
	aimer(a),
	snake(a->GetSnake())
{}

const char *Router::Name() const {
	switch(route) {
		case 0:
			return "Hand Brake";
		case 1:
		case 2:
			if(strategy == 1)
				return "Heuristics (Horizontal)";
			else if(strategy == 1e9)
				return "Heuristics (Vertical)";
			return "Heuristics (Random)";
		case 3:
			return "Path Finder";
		case 4:
			return "Underground";
		default:
			return NULL;
	}
}


Ball Router::GetStep() const {
	/* std::cout << "router: route " << route << std::endl; */
	assert(0 <= route && route < NO_ROUTES);
	switch(route) {
		case 0:
			return snake->currentDirection;
		case 1:
		case 2:
			return GetStepHeuristically();
		case 3:
			return GetStepShortestRoute();
		case 4:
			return GetStepSpaciestRoute();
	}
}


Ball Router::GetStepHeuristically() const {
	const Ball &from = snake->head();
	if(aimer->GetTarget() == UNDEF_BALL)
		return snake->currentDirection;

	Ball step = aimer->GetTarget() - from;
	step = Ball(
		(step.x) ? step.x / abs(step.x) : 0,
		(step.y) ? step.y / abs(step.y) : 0
	);

	if(step.x && step.y) {
		if(
			Ball::InSegment(from + Ball(step.x, 0), WALLS->GetObjects())
			|| Ball::InSegment(from + Ball(step.x, 0), snake->GetObjects())
		)
		{
			step.x = 0;
		}
		if(
			Ball::InSegment(from + Ball(0, step.y), WALLS->GetObjects())
			|| Ball::InSegment(from + Ball(0, step.y), snake->GetObjects())
		)
		{
			step.y = 0;
		}

		if(step.x && step.y)
			((rand() % strategy) ? step.x : step.y) = 0;
		else if(step == Ball(0, 0))
			return snake->currentDirection;
	}

	if(from + step == snake->GetObjects()[1]) {
		step.x = !step.x * ((rand() & 1) ? -1 : 1);
		step.y = !step.y * ((rand() & 1) ? -1 : 1);
	}

	return step;
}

Ball Router::GetStepShortestRoute() const {
	Ball step = GetStepShortestRoute(GetSteps());

	if(step == UNDEF_BALL)
		return snake->currentDirection;
	return step;
}

Ball Router::GetStepShortestRoute(const std::vector <Ball> &steps) const {
	const Ball from = snake->head();

	const std::map <Ball, int> &distances = bfs(aimer->Sonar(), aimer->GetTarget());
	if(distances.count(from) != 1)
		return UNDEF_BALL;

	const int range = distances.at(from);

	/* for(int i = 0; i < HEIGHT; ++i) { */
	/* 	for(int j = 0; j < WIDTH; ++j) { */
	/* 		std::cout << " "; */
	/* 		Ball pos(j, HEIGHT - i); */
	/* 		if(distances.count(pos) == 1) */
	/* 			if(pos == from) */
	/* 				std::cout << "\033[1;93m" << distances[pos] << "\033[0m"; */
	/* 			else */
	/* 				std::cout << distances[pos]; */
	/* 		else */
	/* 			std::cout << UNDEF_INT; */
	/* 	} */
	/* 	std::cout << std::endl; */
	/* } */

	for(const auto &step : steps) {
		if(
			distances.count(from + step) == 1
			&& distances.at(from + step) == range - 1
		)
		{
			return step;
		}
	}

	return UNDEF_BALL;
}

Ball Router::GetStepSpaciestRoute() const {
	int max_space = UNDEF_INT;
	const Ball &from = snake->head();
	std::vector <Ball> steps;

	for(const auto &step : GetSteps()) {
		if(aimer->Sonar().count(from + step) == 0) {
			const std::map <Ball, int> &distances = bfs(aimer->Sonar(), from + step);
			if(max_space == UNDEF_INT
			   || max_space < distances.size())
			{
				max_space = distances.size();
				steps.clear();
			}
			if(max_space == distances.size()) {
				steps.push_back(step);
			}
		}
	}

	/* std::cout << "steps " << std::endl; */
	/* for(const auto &s : steps) */
	/* 	std::cout << "\t" << s << std::endl; */

	Ball step = GetStepShortestRoute(steps);
	if(step == UNDEF_BALL)
		return steps.front();
	return step;
}

/* void Snake::AutoCD_C() { */
/* 	int MAX = 0, FRUIT = 1e9; */
/* 	Ball point = currentDirection; */
/* 	for(const auto &it_step : GetSteps()) { */
/* 		Ball movv = snake.front() + it_step; */
/* 		if(aimer->Sonar().count(movv) == 0) { */
/* 			const std::map <Ball, int> distances = bfs(sonar, movv); */
/* 			int NEW_MAX = distances.size(); */
/* 			if(NEW_MAX >= MAX) { */
/* 				if(NEW_MAX > MAX) { */
/* 					FRUIT = 1e9; */
/* 					point = it_step; */
/* 				} */
/* 				MAX = NEW_MAX; */
/* 				for(const auto &it_fruit : FRUITS->objects) { */
/* 					if(distances.count(it_fruit) > 0) { */
/* 						int NEW_FRUIT = distances[it_fruit]; */
/* 						if(NEW_FRUIT < FRUIT) { */
/* 							FRUIT = NEW_FRUIT; */
/* 							point = it_step; */
/* 						} */
/* 					} */
/* 				} */
/* 			} */
/* 		} */
/* 	} */
/* 	SetStep(point); */
/* } */
