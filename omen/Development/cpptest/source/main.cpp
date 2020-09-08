#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>

struct Vector { double x, y; };
enum Keys { KEY_W, KEY_A, KEY_S, KEY_D, KEY_L, KEY_R, KEY_TOTAL };
enum Dir { UP, RIGHT, DOWN, LEFT };

inline double DegToRad (double _deg)
{
	return (_deg * M_PI) / 180.0;
}
inline double RadToDeg (double _rad)
{
	return (_rad * 180.0) / M_PI;
}

inline double Length (double _x1, double _y1, double _x2, double _y2)
{
	return std::sqrt(std::pow((_x1 - _x2), 2) + std::pow((_y1 - _y2), 2));
}

inline bool InBounds (double _val, double _min, double _max)
{
	return ((_val >= _min) && (_val < _max));
}

inline void Rotate (Vector& _vec, double _deg)
{
	double rad = DegToRad(_deg);
	double c = std::cos(rad), s = std::sin(rad);

	double nx = _vec.x * c - _vec.y * s;
	double ny = _vec.x * s + _vec.y * c;

	_vec.x = nx;
	_vec.y = ny;
}

const double PLAYER_FOV = 60.0;
const double PLAYER_HEIGHT = 24.0;
const double PLAYER_MOVE_SPEED = 3.5;
const double PLAYER_TURN_SPEED = 6.0;
const double PROJPLANE_WIDTH = 1280.0, PROJPLANE_HEIGHT = 960.0;
const double PROJPLANE_DISTANCE = (PROJPLANE_WIDTH / 2) / std::tan(DegToRad(PLAYER_FOV / 2));
const double RAY_LENGTH = 32;
const double RAY_ANGLE = PLAYER_FOV / PROJPLANE_WIDTH;
const double TILE_WIDTH = 32;
const double MAP_WIDTH = 10, MAP_HEIGHT = 8;
/*/
const int MAP_DATA[8][10] = {
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
	{ 1, 1, 1, 1, 1, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 0, 0, 0, 0, 1, 1, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 1, 0, 0, 1, 1, 1, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};
//*/
//*
const int MAP_DATA[8][10] = {
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};
//*/

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Event event;
bool running = true;
bool viewdebug = false;

class Player
{
public:

Player (double _x, double _y, double _angle)
{
	x = _x, y = _y, angle = _angle;
	vel = { 0.0, 0.0 };
	torque = 0.0;
	for (int i = 0; i < KEY_TOTAL; ++i) { keys[i] = false; }
	rays.assign(PROJPLANE_WIDTH, 0.0);
	debug.assign(PROJPLANE_WIDTH * 2, { 0.0, 0.0 });
}

void Handle ()
{
	switch (event.type) {
		case (SDL_KEYDOWN):
		case (SDL_KEYUP): {
			switch (event.key.keysym.sym) {
				case (SDLK_w):     { keys[KEY_W]     = !keys[KEY_W];     break; }
				case (SDLK_a):     { keys[KEY_A]     = !keys[KEY_A];     break; }
				case (SDLK_s):     { keys[KEY_S]     = !keys[KEY_S];     break; }
				case (SDLK_d):     { keys[KEY_D]     = !keys[KEY_D];     break; }
				case (SDLK_LEFT):  { keys[KEY_L]     = !keys[KEY_L];     break; }
				case (SDLK_RIGHT): { keys[KEY_R]     = !keys[KEY_R];     break; }
			}
			break;
		}
	}
}

void Update ()
{
	if (keys[KEY_L]) { torque = PLAYER_TURN_SPEED; }
	else if (keys[KEY_R]) { torque = -PLAYER_TURN_SPEED; }
	else { torque = 0.0; }

	angle += torque;

	if (angle < 0.0) { angle += 360.0; }
	else if (angle > 360.0) { angle -= 360.0; }

	/*
	if (keys[KEY_W]) { vel.x = std::cos(DegToRad(angle)) * PLAYER_MOVE_SPEED; vel.y = std::sin(DegToRad(angle)) * PLAYER_MOVE_SPEED; }
	else if (keys[KEY_S]) { vel.x = -(std::cos(DegToRad(angle)) * PLAYER_MOVE_SPEED); vel.y = -(std::sin(DegToRad(angle)) * PLAYER_MOVE_SPEED); }
	else { vel.x = 0.0, vel.y = 0.0; }

	if (keys[KEY_W]) { x += std::abs(vel.x), y += std::abs(vel.y); }
	else if (keys[KEY_S]) { x += vel.x, y += vel.y; }
	//*/

	/*
	double velx = 0.0, vely = 0.0;
	if (keys[KEY_W]) {
		velx = std::cos(DegToRad(angle)) * PLAYER_MOVE_SPEED;
		vely = std::sin(DegToRad(angle)) * PLAYER_MOVE_SPEED;

		x += -velx, y += -vely;
	}
	//*/

	if (keys[KEY_W]) { vel.y = -PLAYER_MOVE_SPEED; }
	else if (keys[KEY_S]) { vel.y = PLAYER_MOVE_SPEED; }
	else { vel.y = 0.0; }
	if (keys[KEY_A]) { vel.x = -PLAYER_MOVE_SPEED; }
	else if (keys[KEY_D]) { vel.x = PLAYER_MOVE_SPEED; }
	else { vel.x = 0.0; }

	x += vel.x, y += vel.y;

	double rayangle = angle + (PLAYER_FOV / 2);
	double index = 0;

	for (auto& it: rays) {
		if (rayangle < 0.0) { rayangle += 360.0; }
		else if (rayangle > 360.0) { rayangle -= 360.0; }

		bool horzmax = false;
		bool vertmax = false;

		Dir horzdir;
		if ((rayangle >= 90.0) && (rayangle < 270.0)) { horzdir = LEFT; }
		else { horzdir = RIGHT; }
		Dir vertdir;
		if ((rayangle >= 0.0) && (rayangle < 180.0)) { vertdir = UP; }
		else { vertdir = DOWN; }

		Vector horzpos;
		if (vertdir == UP) { horzpos.y = std::floor(y / TILE_WIDTH) * (TILE_WIDTH) - 1.0; }
		else { horzpos.y = std::floor(y / TILE_WIDTH) * (TILE_WIDTH) + TILE_WIDTH; }
		horzpos.x = x + (y - horzpos.y) / std::tan(DegToRad(rayangle));

		Vector horztile = { std::floor(horzpos.x / TILE_WIDTH), std::floor(horzpos.y / TILE_WIDTH) };
		if (InBounds(horztile.x, 0, MAP_WIDTH) && InBounds(horztile.y, 0, MAP_HEIGHT)) {
			if (MAP_DATA[(int)horztile.y][(int)horztile.x] == 0) {
				double horzjump, vertjump;
				if (vertdir == UP) { vertjump = -TILE_WIDTH, horzjump = TILE_WIDTH / std::tan(DegToRad(rayangle)); }
				else { vertjump = TILE_WIDTH, horzjump = -(TILE_WIDTH / std::tan(DegToRad(rayangle))); }
				for (double i = 1; i <= RAY_LENGTH; ++i) {
					if (i == RAY_LENGTH) { horzmax = true; }
					horzpos.x += horzjump;
					horzpos.y += vertjump;
					horztile.x = std::floor(horzpos.x / TILE_WIDTH);
					horztile.y = std::floor(horzpos.y / TILE_WIDTH);
					if (InBounds(horztile.x, 0, MAP_WIDTH) && InBounds(horztile.y, 0, MAP_HEIGHT)) {
						if (MAP_DATA[(int)horztile.y][(int)horztile.x] == 1) { break; }
					}
				}
			}
		}

		Vector vertpos;
		if (horzdir == LEFT) { vertpos.x = std::floor(x / TILE_WIDTH) * (TILE_WIDTH) - 1.0; }
		else { vertpos.x = std::floor(x / TILE_WIDTH) * (TILE_WIDTH) + TILE_WIDTH; }
		vertpos.y = y + (x - vertpos.x) * std::tan(DegToRad(rayangle));

		Vector verttile = { std::floor(vertpos.x / TILE_WIDTH), std::floor(vertpos.y / TILE_WIDTH) };
		if (InBounds(verttile.x, 0, MAP_WIDTH) && InBounds(verttile.y, 0, MAP_HEIGHT)) {
			if (MAP_DATA[(int)verttile.y][(int)verttile.x] == 0) {
				double vertjump, horzjump;
				if (horzdir == LEFT) { horzjump = -TILE_WIDTH, vertjump = TILE_WIDTH * std::tan(DegToRad(rayangle)); }
				else { horzjump = TILE_WIDTH, vertjump = -(TILE_WIDTH * std::tan(DegToRad(rayangle))); }
				for (double i = 1; i <= RAY_LENGTH; ++i) {
					if (i == RAY_LENGTH) { vertmax = true; }
					vertpos.x += horzjump;
					vertpos.y += vertjump;
					verttile.x = std::floor(vertpos.x / TILE_WIDTH);
					verttile.y = std::floor(vertpos.y / TILE_WIDTH);
					if (InBounds(verttile.x, 0, MAP_WIDTH) && InBounds(verttile.y, 0, MAP_HEIGHT)) {
						if (MAP_DATA[(int)verttile.y][(int)verttile.x] == 1) { break; }
					}
				}
			}
		}

		double horzlen = Length(x, y, horzpos.x, horzpos.y);
		double vertlen = Length(x, y, vertpos.x, vertpos.y);

		if (std::abs(horzlen) <= std::abs(vertlen)) {
			if (horzmax) { it = 99999; }
			else { it = std::abs(horzlen * std::cos(DegToRad(-(PLAYER_FOV / 2) + (RAY_ANGLE * index)))); }
		}
		else {
			if (vertmax) { it = 99999; }
			else { it = std::abs(vertlen * std::cos(DegToRad(-(PLAYER_FOV / 2) + (RAY_ANGLE * index)))); }
		}

		debug.at(index) = { horzpos.x, horzpos.y };
		debug.at(PROJPLANE_WIDTH + index) = { vertpos.x, vertpos.y };

		rayangle -= RAY_ANGLE;
		++index;
	}
}

void Render ()
{
	if (!viewdebug) {
		double index = 0;
		for (auto& it: rays) {
			if (it == 99999) { continue; }
			double height = std::ceil(TILE_WIDTH / it * PROJPLANE_DISTANCE);
			SDL_SetRenderDrawColor(renderer, 0xFF - (it / 2), 0xFF - (it / 2), 0xFF - (it / 2), 0xFF );
			SDL_RenderDrawLine(
				renderer,
				index, (PROJPLANE_HEIGHT / 2) - (height / 2),
				index, ((PROJPLANE_HEIGHT / 2) - (height / 2)) + height
			);
			++index;
		}
	}
	else {
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x0F);
		for (int i = 0; i < PROJPLANE_WIDTH; ++i) { SDL_RenderDrawLine(renderer, x, y, debug.at(i).x, debug.at(i).y); }
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0x0F);
		for (int i = PROJPLANE_WIDTH; i < PROJPLANE_WIDTH * 2; ++i) { SDL_RenderDrawLine(renderer, x, y, debug.at(i).x, debug.at(i).y); }

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderDrawLine(renderer, x, y, x - vel.x, y - vel.y);

		/*/
		Vector test = { 24.0, 0.0 };
		SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
		SDL_RenderDrawLine(renderer, x, y, x + test.x, y + test.y);
		Rotate(test, 180.0);
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
		SDL_RenderDrawLine(renderer, x, y, x + test.x, y + test.y);
		//*/
	}
}

private:

double x, y, angle;
Vector vel;
double torque;
std::vector<double> rays;
bool keys[KEY_TOTAL];
std::vector<Vector> debug;

}; // Player

int main (int _argc, char* _argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Raycast Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, PROJPLANE_WIDTH, PROJPLANE_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	Player player(48, 48, 270.0);

	while (running) {
		uint32_t start = SDL_GetTicks();
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) { running = false; }
			else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE) { running = false; }
			else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE) { viewdebug = !viewdebug; }
			else { player.Handle(); }
		}
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(renderer);
		player.Update();
		player.Render();
		SDL_RenderPresent(renderer);
		uint32_t elapsed = SDL_GetTicks() - start;
		if (elapsed < 16) { SDL_Delay(16 - elapsed); }
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}