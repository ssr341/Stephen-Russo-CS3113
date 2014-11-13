#include "StageOne.h"

StageOne::StageOne(){
	winner = 0;
	enemyShot = 0.0f;
	enemyNum = 4;
	enemyBulletSize = 0.05f;
	enemyBulletSpeed = 0.5f;
	player1KillCount = 0;
	player2KillCount = 0;
	player1BulletSize = 0.05f;
	player1BulletSpeed = 1.0f;
	player2BulletSize = 0.05f;
	player2BulletSpeed = 1.0f;
	for (int i = 0; i < MAX_BULLETS; i++){
		bullets[i].x = -10.0f;
		bullets[i].y = -10.0f;
		bullets[i].direction = 0.0f;
		bullets[i].shooter = -1.0f;
		bullets[i].speed = 0.0f;
		bullets[i].size = 0.0f;
	}
}

GLuint StageOne::LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
	SDL_FreeSurface(surface);
	return textureID;
}

void StageOne::DrawText(int fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a) {
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float texture_size = 1.0 / 16.0f;

	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	std::vector<float> colorData;

	for (size_t i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });

		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f * size), 0.5f * size, ((size + spacing) * i) +
			(-0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), 0.5f
			* size });

		texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +
			texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);
}

void StageOne::Init(){
	// player creation
	player1.textureID = LoadTexture("playerShip1.png");
	player1.height = 0.25f;
	player1.width = 0.25f;
	player1.x = -1.13f;
	player1.y = 0.0f;
	player1.friction_y = 2.0f;
	player1.acceleration_y = 0.0f;
	player1.velocity_y = 0.0f;
	player1.visible = true;

	player2.textureID = LoadTexture("playerShip2.png");
	player2.height = 0.25f;
	player2.width = 0.25f;
	player2.x = 1.13f;
	player2.y = 0.0f;
	player2.friction_y = 2.0f;
	player2.acceleration_y = 0.0f;
	player2.velocity_y = 0.0f;
	player2.visible = true;

	// enemy creation
	float enemy1X = -0.1f;
	float enemyY = 0.3f;
	float enemy2X = 0.1f;
	for (int i = 0; i < enemyNum; i++){
		Entity* enemy1 = new Entity;
		enemy1->textureID = LoadTexture("enemyUFO.png");
		enemy1->width = 0.2f;
		enemy1->height = 0.2f;
		enemy1->x = enemy1X;
		enemy1->y = enemyY;
		enemy1->velocity_y = 0.0f;
		enemy1->acceleration_y = -1.0f;
		enemy1->friction_y = 0.5f;
		enemy1->visible = true;
		enemies1.push_back(enemy1);

		Entity* enemy2 = new Entity;
		enemy2->textureID = LoadTexture("enemyUFO.png");
		enemy2->width = 0.2f;
		enemy2->height = 0.2f;
		enemy2->x = enemy2X;
		enemy2->y = enemyY;
		enemy2->velocity_y = 0.0f;
		enemy2->acceleration_y = -1.0f;
		enemy2->friction_y = 0.5f;
		enemy2->visible = true;
		enemies2.push_back(enemy2);

		enemyY -= 0.2f;
	}
}

void StageOne::ProcessShoot(SDL_Event* event){
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	if (event->type == SDL_KEYDOWN) {
		if (event->key.keysym.scancode == SDL_SCANCODE_F) {
			shootBullet(player1.x, player1.y, 1.0, 0, player1BulletSize, player1BulletSpeed);
		}
		if (event->key.keysym.scancode == SDL_SCANCODE_KP_0) {
			shootBullet(player2.x, player2.y, -1.0, 1, player2BulletSize, player2BulletSpeed);
		}
	}
}

void StageOne::ProcessEvents(){
	player1.acceleration_y = 0.0f;
	player2.acceleration_y = 0.0f;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	// w, s and c keys used by player 1
	if (keys[SDL_SCANCODE_E]){
		// if w pressed, set acceleration positive
		if (player1KillCount >= 10)
			player1.acceleration_y = 4.0f;
		else
			player1.acceleration_y = 2.0f;
	}
	if (keys[SDL_SCANCODE_D]){			
		// if s pressed, set acceleration negative
		if (player1KillCount >= 10)
			player1.acceleration_y = -4.0f;
		else
			player1.acceleration_y = -2.0f;
	}

	// up, down and 0 keys used by player 2
	if (keys[SDL_SCANCODE_UP]){
		// if up pressed, set acceleration positive
		if (player2KillCount >= 10)
			player2.acceleration_y = 4.0f;
		else
			player2.acceleration_y = 2.0f;
	}
	if (keys[SDL_SCANCODE_DOWN]){
		// if down pressed, set acceleration negative
		if (player2KillCount >= 10)
			player2.acceleration_y = 4.0f;
		else
			player2.acceleration_y = -2.0f;
	}
	// if q is pressed quit
	if (keys[SDL_SCANCODE_Q] == 1){
		SDL_Quit();
	}

}

int StageOne::fixedUpdate(float fixedElapsed){
	// run fixed update for everything
	player1.fixedUpdate();
	player2.fixedUpdate();
	for (size_t i = 0; i < enemies1.size(); i++){
		enemies1[i]->fixedUpdate();
		enemies1[i]->y += enemies1[i]->velocity_y * FIXED_TIMESTEP;
	}
	for (size_t i = 0; i < enemies2.size(); i++){
		enemies2[i]->fixedUpdate();
		enemies2[i]->y += enemies2[i]->velocity_y * FIXED_TIMESTEP;
	}

	// screen boundaries
	if (player1.y > 0.8)
		player1.y = 0.8;
	if (player2.y > 0.8)
		player2.y = 0.8;
	if (player1.y < -0.8)
		player1.y = -0.8;
	if (player2.y < -0.8)
		player2.y = -0.8;

	// have enemies reverse when they meet edge of screen
	if (enemies1[0]->y >= 0.5){
		for (size_t i = 0; i < enemies1.size(); i++){
			enemies1[i]->acceleration_y = -1.0f;
		}
	}
	if (enemies1[enemyNum - 1]->y <= -0.5){
		for (size_t i = 0; i < enemies1.size(); i++){
			enemies1[i]->acceleration_y = 1.0f;
		}
	}
	if (enemies2[0]->y >= 0.5){
		for (size_t i = 0; i < enemies2.size(); i++){
			enemies2[i]->acceleration_y = -1.0f;
		}
	}
	if (enemies2[enemyNum - 1]->y <= -0.5){
		for (size_t i = 0; i < enemies2.size(); i++){
			enemies2[i]->acceleration_y = 1.0f;
		}
	}

	// check for enemy collision with bullets
	for (size_t i = 0; i < enemies1.size(); i++){
		for (int j = 0; j < MAX_BULLETS; j++){
			// if enemy already dead ignore
			if (enemies1[i]->visible && bullets[j].shooter != 2){
				// if bullet is visible and colliding
				if (enemies1[i]->collidesWith(bullets[j])){
					// make enemy and bullet invisible
					enemies1[i]->visible = false;
					bullets[j].visible = false;
					if (bullets[j].shooter == 0)
						player1KillCount++;
					if (bullets[j].shooter == 1)
						player2KillCount++;
				}
			}
		}
	}

	// check for enemy collision with bullets
	for (size_t i = 0; i < enemies2.size(); i++){
		for (int j = 0; j < MAX_BULLETS; j++){
			// if enemy already dead ignore
			if (enemies2[i]->visible && bullets[j].shooter != 3){
				// if bullet is visible and colliding
				if (enemies2[i]->collidesWith(bullets[j])){
					// make enemy and bullet invisible
					enemies2[i]->visible = false;
					bullets[j].visible = false;
					if (bullets[j].shooter == 0)
						player1KillCount++;
					if (bullets[j].shooter == 1)
						player2KillCount++;
				}
			}
		}
	}

	// check for bullet collision with player
	//for (int i = 0; i < MAX_BULLETS; i++){
	//	// if bullet is visible and colliding
	//	if (bullets[i].visible && bullets[i].shooter != 0 && player1.collidesWith(bullets[i])){
	//		winner = 2;
	//		return winner;
	//	}
	//}
	//for (int i = 0; i < MAX_BULLETS; i++){
	//	// if bullet is visible and colliding
	//	if (bullets[i].visible && bullets[i].shooter != 1 && player2.collidesWith(bullets[i])){
	//		winner = 1;
	//		return winner;
	//	}
	//}

	// have enemy shoot bullet
	enemyShot += fixedElapsed;
	if (enemyShot >= 0.3f){    // shoot every 30 frames
		bool shot1 = false;  // was the bullet shot?
		bool shot2 = false;  // was the bullet shot?
		while (!shot1 && !shot2){
			int enemyBulletX = rand() % (enemyNum);
			if (enemies1[enemyBulletX]->visible && !shot1){
				shootBullet(enemies1[enemyBulletX]->x, enemies1[enemyBulletX]->y, -1.0f, 2, enemyBulletSize, enemyBulletSpeed);
				shot1 = true;
			}
			if (enemies2[enemyBulletX]->visible && !shot2){
				shootBullet(enemies2[enemyBulletX]->x, enemies2[enemyBulletX]->y, 1.0f, 3, enemyBulletSize, enemyBulletSpeed);
				shot2 = true;
			}
		}
		enemyShot = 0;
	}

	// update position of each bullet
	for (int i = 0; i < MAX_BULLETS; i++){
		/*bullets[i].Update(fixedElapsed);*/
		bullets[i].fixedUpdate();
	}

	// update for powerups
	if (player1KillCount >= 3)
		player1BulletSpeed *= 2.0f;
	if (player1KillCount >= 6)
		player1BulletSize *= 2.0f;
	if (player2KillCount >= 3)
		player2BulletSpeed *= 2.0f;
	if (player2KillCount >= 6)
		player2BulletSize *= 2.0f;

	return winner;
}

void StageOne::Render(){
	glClear(GL_COLOR_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);

	player1.draw();
	player2.draw();

	for (size_t i = 0; i < enemies1.size(); i++){
		if (enemies1[i]->visible)
				enemies1[i]->draw();
	}
	for (size_t i = 0; i < enemies2.size(); i++){
		if (enemies2[i]->visible)
			enemies2[i]->draw();
	}

	for (int i = 0; i < MAX_BULLETS; i++){
		// only draw bullets that haven't collided or are on screen
		if (bullets[i].visible)
			bullets[i].Draw();
	}

}

// shoots bullet at specified location
void StageOne::shootBullet(float x, float y, float direction, float shooter, float size, float speed){
	bullets[bulletIndex].visible = true;
	bullets[bulletIndex].x = x;
	bullets[bulletIndex].y = y;
	bullets[bulletIndex].direction = direction;
	bullets[bulletIndex].size = size;
	bullets[bulletIndex].speed = speed;
	bullets[bulletIndex].shooter = shooter;
	bulletIndex++;

	// reset index if max bullet is reached and overwrite earlier bullets
	if (bulletIndex > MAX_BULLETS - 1)
		bulletIndex = 0;
}