#include "Aimer.hpp"
#include "Functions.hpp"

Aimer::Aimer(Snake *s, Wall *w, Fruit *f):
	s(s), w(w), f(f)
{}

char *Aimer::GetName(int aim) {
	switch(aim) {
		case 0:
			return "Hand Brake";
		case 1:
			return "Closest Fruit";
		case 2:
			return "Newest fruit";
		case 3:
			return "Furthest fruit";
		case 4:
			return "Tail";
		default:
			return NULL;
	}
}

Ball Aimer::SetTarget(Ball &target, int aim) {
	for(const auto &obst : {w->walls, s->snake})
		AddObstacle(obst);
	DeleteSnakeObstacles();
	sonar.erase(s->snake.back());
	range = INVALID_INT;
	way_to = bfs(sonar, *from);
	switch(aim) {
		case 0:
		break;
		case 1:
			target = GetTargetClosest(target);
		break;
		case 2:
			target = GetTargetNewest(target);
		break;
		case 3:
			target = GetTargetFurthest(target);
		break;
		case 4:
			target = GetSnakeTail(target);
		break;
		default:
			return;
	}
}

void Aimer::AddObstacle(std::vector <Ball> &objects) {
	for(const auto &ball : objects)
		sonar[ball] = true;
}

void Aimer::DeleteSnakeObstacles() {
	*from = s->snake.front();
	std::map <Ball, int> way_to = bfs(sonar, *from);
	for(int i = 1; i < s->snake.size(); ++i) {
		if(way_to[s->snake[i]] > s->snake.size() - i)
			sonar.erase(s->snake[i]);
	}
}

Ball Aimer::GetTargetFurthest(Ball &target) {
	for(const auto &f : f->fruit_Storage) {
		if(way_to.count(f))
			if(
				range == INVALID_INT
				|| range > way_to[f]
			)
			{
				target = f;
				range = way_to[target];
			}
	}
	return target;
}

Ball Aimer::GetTargetClosest(Ball &target) {
	for(const auto &f : f->fruit_Storage) {
		if(way_to.count(f)) {
			if(range < way_to[f]) {
				*target = f;
				range = way_to[*target];
			}
		}
	}
	return *target;
}

Ball Aimer::GetTargetNewest(Ball &target) {
	range = way_to[*target];
	return *target;
}

Ball Aimer::GetSnakeTail(Ball &target) {
	*target = s->snake.back();
	for(int i = s->snake.size() - 1; i >= 0; --i) {
		if(way_to.count(*target)) {
			*target = s->snake[i];
			range = way_to[*target];
			return *target;
		}
	}
	return *target;
}
