#pragma once

#include <SDL.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Bullet.h"
#include "Entity.h"
#include <string>
#include <vector>

#define MAX_BULLETS 32

class StageThree{
public:
	StageThree();
	~StageThree();

	GLuint LoadTexture(const char *image_path);

	void Init();
	void ProcessShoot(SDL_Event* event, bool& done);
	void ProcessEvents();
	void Render();
	int fixedUpdate(float fixedElapsed);

	void shootBullet(float x, float y, float direction, float shooter, float size, float speed);

	bool explosion(float fixedElapsed);

	void reset();

	Mix_Chunk* explosionSound; // sound when bullets collide
	Mix_Chunk* shootingSound; // sound when bullets shot
private:
	Bullet bullets[MAX_BULLETS];
	int bulletIndex;

	float enemyShot; // time between enemy shots

	int winner; // winner of stage. 0 means gameplay still happening, 1 means player 1, 2 means player 2
	bool freeze; // freeze movement when someone dies

	int hitsToKill; // number of hits to kill a player
	int player1Hits; // number of times player 1 has been shot
	int player2Hits; // number of times player 2 has been shot

	// entities
	Entity player1;
	Entity player1fire;
	Entity player1shield;
	Entity player2;
	Entity player2fire;
	Entity player2shield;
	Entity enemy;

	// used in explosion
	Entity sparkle;
	float explosionTime;

	// powerups
	float player1Shot; // current time between shots
	float player2Shot; // current time between shots
	float player1ShotTime; // time before another shot can be fired
	float player2ShotTime; // time before another shot can be fired
	float player1BulletSize;
	float player2BulletSize;
	float player1BulletSpeed;
	float player2BulletSpeed;

	// enemy bullet properties
	float enemyBulletSize;
	float enemyBulletSpeed;

};