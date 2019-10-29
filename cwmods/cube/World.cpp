#include "World.h"

void cube::World::SetTime(float ms) {
	this->state.time = ms;
}
void cube::World::SetTime(int ms) {
	this->state.time = (float)ms;
}
void cube::World::SetTime(int hour, int minute) {
	this->state.time = (float)((hour * 60 * 60 * 1000) + (minute * 60 * 1000));
}
float cube::World::GetTime() {
	return this->state.time;
}