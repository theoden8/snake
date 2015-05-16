#include <cstdlib>
#include <iostream>
#include <cmath>

#include "Functions.h"

std::map <unsigned char, Ball> GetSteps() {
	std::map <unsigned char, Ball> steps;
	steps[0] = Ball(-1, 0);
	steps[1] = Ball(0, 1);
	steps[2] = Ball(0, -1);
	steps[3] = Ball(1, 0);
	return steps;
}

std::map <Ball, int> bfs(std::map <Ball, bool> &sonar, const Ball &from) {
	std::queue <Ball> spiral;
	std::map <Ball, int>  way_to;
	spiral.push(from);
	way_to[from] = 0;
	while (!spiral.empty()) {
		Ball pointer = spiral.front();
		spiral.pop();
		for (auto it : GetSteps()) {
			Ball movv = pointer + it.second;
			if(sonar.count(movv) == 0 && (way_to.count(movv) == 0)) {
				way_to[movv] = way_to[pointer] + 1;
				spiral.push(movv);
			}
		}
	}
	return way_to;
}

bool process_key(std::string keys, char key) {
	for(int i = 0; i < keys.length(); ++i) {
		if(keys[i] == key) {
			return true;
		}
	}
	return false;
}

std::string str(unsigned int x) {
	if (!x)
		return "0";
	std::string tmp;
	while (x) {
		tmp = char('0' + x % 10) + tmp;
		x /= 10;
	}
	return tmp;
}
