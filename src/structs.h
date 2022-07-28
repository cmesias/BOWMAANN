///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////// ARROWS /////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------//
struct Arrow {
public:
	double x, y, w, h, r, a;
	double bmx, bmy;
	double vX, vY, distance;
	double speed, speedX, speedY;
	bool alive;
	bool collide;
	bool collideP1;
	bool collideP2;
	bool inVision;
	bool onScreen;
	string side;
	string gravityState;
	string type;

	int damage;
	int time;
	int alpha;
	int alphaspeed;
	int deathTimer;
	int deathTimerSpeed;
	bool camFollow;
};
void initArrow(Arrow arrow[]) {
	for (int i = 0; i < 200; i++) {
		arrow[i].camFollow = true;
		arrow[i].onScreen = false;
		arrow[i].collide = false;
		arrow[i].alive = false;
		arrow[i].x = 0.00;
		arrow[i].y = 0.00;
		arrow[i].w = 3;
		arrow[i].h = 25;
		arrow[i].speed = 25.00;
		arrow[i].time = 0;
		arrow[i].damage = 50;
		arrow[i].deathTimer = 100;
		arrow[i].deathTimerSpeed = 1;
		arrow[i].alpha = 255;
		arrow[i].alphaspeed = 1;
	}
}
void fireArrow(Arrow arrow[], int damage, string type,
									   double radianCos, double radianSin,
									   int spawnx, int spawny, int w, int h,
									   float vX, float vY, double speed,
									   int alphaspeed, int alpha,
									   int deathTimer, int deathTimerSpeed) {
	for (int i = 0; i < 200; i++) {
		if (!arrow[i].alive) {
			//Bullet type
			arrow[i].a = atan2 ( vY, vX) * 180 / PI;
			arrow[i].damage = damage;
			arrow[i].type = type;
			arrow[i].x = spawnx +radianCos;
			arrow[i].y = spawny +radianSin;
			arrow[i].w = w;
			arrow[i].h = h;
			arrow[i].speed = speed;
			arrow[i].vX = vX * speed;
			arrow[i].vY = vY * speed;
			arrow[i].alpha = alpha;
			arrow[i].alphaspeed = alphaspeed;
			arrow[i].deathTimer = deathTimer;
			arrow[i].deathTimerSpeed = deathTimerSpeed;
			arrow[i].alive = true;

			//Bullet Rotationa
			ARROWS++;
			break;
		}
	}
}
void updateArrow(Arrow arrow[], int camX, int camY) {
	for (int i = 0; i < 200; i++) {
		if (arrow[i].alive) {
			//if (arrow[i].type == "arrow") {


				/*if (arrow[i].time > arrow[i].deathTimer) {
					arrow[i].alive = false;
					arrow[i].onScreen = false;
					arrow[i].collide = false;
					arrow[i].damage = 0;
					arrow[i].time = 0;
					arrow[i].alpha = 0;;
					arrow[i].alphaspeed = 0;;
					arrow[i].deathTimer = 100;
					arrow[i].deathTimerSpeed = 1;
					arrow[i].alpha = 0;
					arrow[i].bmx = 0.00;
					arrow[i].bmy = 0.00;
					arrow[i].distance = 0.00;
					arrow[i].vX = 0.00;
					arrow[i].vY = 0.00;
					arrow[i].speedX = 0.00;
					arrow[i].speedY = 0.00;
					arrow[i].speed = 0.00;
					ARROWS--;
				}*/
			//}
		}

		// IF BULLET IS ON SCREN
		if (arrow[i].x + arrow[i].w >= camX-64
				&& arrow[i].x <= camX-64 + gWindow.getWidth()+128
				&& arrow[i].y + arrow[i].h >= camY-64
				&& arrow[i].y <= camY-64 + gWindow.getHeight()+128) {
			arrow[i].onScreen = true;
		} else {
			arrow[i].onScreen = false;
		}
	}
}
void renderArrow(Arrow arrow[], int camX, int camY) {
	for (int i = 0; i < 200; i++) {
		if (arrow[i].alive) {

			//Render arrow
			//if (arrow[i].onScreen){
			gArrow.render(arrow[i].x - camX, arrow[i].y-8 - camY, arrow[i].w,32, 0, arrow[i].a);

			if (arrow[i].collide){
				SDL_Rect player = { arrow[i].x-camX, arrow[i].y-camY, arrow[i].w, arrow[i].h };
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &player);
			} else{
				SDL_Rect player2 = { arrow[i].x-camX, arrow[i].y-camY, arrow[i].w, arrow[i].h };
				SDL_SetRenderDrawColor(gRenderer, 255, 00, 0, 255);
				SDL_RenderDrawRect(gRenderer, &player2);
			}
			if (debug){

				std::stringstream timer;
				timer << arrow[i].time;
				gText.loadFromRenderedText(timer.str().c_str(), {0,0,0,255}, gFont13);
				gText.render(arrow[i].x-gText.getWidth() - camX, arrow[i].y-gText.getHeight() - camY, gText.getWidth(), gText.getHeight());

				std::stringstream velocityeh;
				velocityeh << arrow[i].vX << "                                        " <<arrow[i].vY << "                          " <<arrow[i].time;
				gText.loadFromRenderedText(velocityeh.str().c_str(), {0,0,0,255}, gFont13);
				gText.render(arrow[i].x+arrow[i].w/2-gText.getWidth()/2 - camX,
							 arrow[i].y-gText.getHeight() - camY,
							 gText.getWidth(), gText.getHeight());
			}
			//}
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////// ARROWS /////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// Block VARIABLES //////////////////////////////////////
//-----------------------------------------------------------------------------------------//
struct Block {
public:
	int x,y,w,h;
	double speedX, speedY;
	int spawnTimer;
	bool alive;
	bool mouseOnBlock;
	string type;
	double distance;
	double bmx, bmy;
	double vX, vY;
};
void initBlock(Block block[]) {
	for (int i = 0; i < 200; i++) {
		block[i].x = 0;
		block[i].y = 0;
		block[i].w = 0;
		block[i].h = 0;
		block[i].spawnTimer = 0;
		block[i].type = "";
		block[i].alive = false;
		block[i].mouseOnBlock = false;
	}
}
void spawnBlock(Block block[], int x, int y, int w, int h) {
	for (int i = 0; i < 200; i++){
		if (!block[i].alive){
			if (blockIndex == 1){
				block[i].type = "grass";
			}
			if (blockIndex == 2){
				block[i].type = "stone";
			}
			if (blockIndex == 3){
				block[i].type = "easteregg";
			}
			block[i].x = x;
			block[i].y = y;
			block[i].w = w;
			block[i].h = h;
			block[i].alive = true;
			BLOCKS++;
			break;
		}
	}
}
void copyBlock(Block block[]) {
	for (int i = 0; i < 200; i++) {
		if (block[i].alive) {
			if (block[i].mouseOnBlock) {
				BLOCKW = block[i].w;
				BLOCKH = block[i].h;
				if (block[i].type == "grass"){
					blockIndex = 1;
				}
				if (block[i].type == "stone"){
					blockIndex = 2;
				}
				if (block[i].type == "easteregg"){
					blockIndex = 3;
				}
			}
		}
	}
}
void addMinusBlockWH(Block block[], string what) {
	for (int i = 0; i < 200; i++) {
		if (block[i].alive) {
			if (block[i].mouseOnBlock) {
				if (what == "minusW"){
					BLOCKW -= block[i].w;
				}
				if (what == "addW"){
					BLOCKW += block[i].w;
				}
				if (what == "minusH"){
					BLOCKH -= block[i].h;
				}
				if (what == "addH"){
					BLOCKH += block[i].h;
				}
			}
		}
	}
}
void removeBlock(Block block[]) {
	for (int i = 0; i < 200; i++) {
		if (block[i].alive) {
			if (block[i].mouseOnBlock) {
				block[i].x = 0;
				block[i].y = 0;
				block[i].w = 0;
				block[i].h = 0;
				block[i].type = "";
				block[i].alive = false;
				BLOCKS--;
			}
		}
	}
}
int randEnemyNumBlock;
void updateBlock(Block block[], int mx, int my) {
	for (int i = 0; i < 200; i++) {
		if (block[i].alive) {
			//If the mouse is on the block
			if (mx > block[i].x && mx < block[i].x + block[i].w &&
				my > block[i].y && my < block[i].y + block[i].h) {
				block[i].mouseOnBlock = true;
			} else {
				block[i].mouseOnBlock = false;
			}
		}
	}
}
void renderBlock(Block block[], int camx, int camy) {
	for (int i = 0; i < 200; i++) {
		if (block[i].alive) {
			if (block[i].type == "grass"){
				gGrass.render(block[i].x-camx, block[i].y-camy, block[i].w, block[i].h);
			}
			if (block[i].type == "stone"){
				gStone.render(block[i].x-camx, block[i].y-camy, block[i].w, block[i].h);
			}
			if (block[i].type == "easteregg"){
				gEasterEgg.render(block[i].x-camx, block[i].y-camy, block[i].w, block[i].h);
			}
			//std::stringstream aaa;
			//aaa << floor(block[i].vX *10+0.5)/10 << " " << floor(block[i].vY *10+0.5)/10;
			//gText.loadFromRenderedText(aaa.str().c_str(), {255,255,255,255}, gFont13);
			//gText.render(block[i].x+block[i].w/2-gText.getWidth()/2 - camx, block[i].y+block[i].h/2-gText.getHeight()/2 - camy, gText.getWidth(), gText.getHeight());
		}
	}
}
//-----------------------------------------------------------------------------------------//
///////////////////////////////////// Block VARIABLES //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


void checkCollisionArrow(Arrow arrow[], Block block[],
							  double x, double y, int w, int h, int &health,
							  double x2, double y2, int w2, int h2, int &health2) {
	for (int i = 0; i < 200; i++) {
		if (arrow[i].alive) {
			for (int j = 0; j < 200; j++) {
				if (block[j].alive) {
					//Collision Check - Player, monster[i].lock; With Added Jumpstates
					if (arrow[i].x + 32 >= block[j].x && arrow[i].x <= block[j].x + block[j].w &&
						arrow[i].y + 16 >= block[j].y && arrow[i].y <= block[j].y + block[j].h) {
						arrow[i].collide = true;
					}
				}
			}
			if (arrow[i].type == "arrowKillPlayer1") {
				//Body shot
				if (arrow[i].x + 32 >= x && arrow[i].x <= x + w &&
					arrow[i].y + 32  >= y && arrow[i].y <= y + h) {
					arrow[i].collide = true;
					if (applyDamage){
						applyDamage = false;
						health -= 20;
					}
				}
			}
			if (arrow[i].type == "arrowKillPlayer2") {
				//Head shot
				if (arrow[i].x + 32 >= x2 && arrow[i].x <= x2 + w2 &&
					arrow[i].y + 32 >= y2&& arrow[i].y <= y2 + h2) {
					arrow[i].collide = true;
					if (applyDamage){
						applyDamage = false;
						health2 -= 20;
					}
				}
			}
			if (arrow[i].collide){
				arrow[i].time += 1;
				if (arrow[i].time > 120){
					arrow[i].camFollow = false;
				}
				if (arrow[i].time > 150){
					shootArrow = true;
					applyDamage = true;
					if (Turn == "1"){
						Turn = "2";
						//After death of arrow if the camera is locked then center camera on next player
						if (player.camlocked){
							cam.x = player2.x + player2.w/2 - gWindow.getWidth()/2;
							cam.y = player2.y + player2.h/2 - gWindow.getHeight()/2;
						}
					}else{
						Turn = "1";
						//After death of arrow if the camera is locked then center camera on next player
						if (player.camlocked){
							cam.x = player.x + player.w/2 - gWindow.getWidth()/2;
							cam.y = player.y + player.h/2 - gWindow.getHeight()/2;
						}
					}
					arrow[i].collide = false;
					arrow[i].alive = false;
					arrow[i].camFollow = true;
					arrow[i].onScreen = false;
					arrow[i].collide = false;
					arrow[i].damage = 0;
					arrow[i].time = 0;
					arrow[i].alpha = 0;;
					arrow[i].alphaspeed = 0;;
					arrow[i].deathTimer = 100;
					arrow[i].deathTimerSpeed = 1;
					arrow[i].alpha = 0;
					arrow[i].bmx = 0.00;
					arrow[i].bmy = 0.00;
					arrow[i].distance = 0.00;
					arrow[i].vX = 0.00;
					arrow[i].vY = 0.00;
					arrow[i].speedX = 0.00;
					arrow[i].speedY = 0.00;
					arrow[i].speed = 0.00;
					ARROWS--;
				}
			}
			//If arrow collides with something then start death timer
			if (!arrow[i].collide){
				if (arrow[i].vX >= 0){
					arrow[i].vX -= 0.01;
				}
				if (arrow[i].vX < 0){
					arrow[i].vX += 0.01;
				}
				arrow[i].vY += 0.4;
				arrow[i].x += arrow[i].vX;
				arrow[i].y += arrow[i].vY;
				arrow[i].a = atan2 ( arrow[i].vY, arrow[i].vX) * 180 / PI;
			}
		}
	}
}



//Debug stuff
void renderBlockGhosts(int mx, int my){
	std::stringstream pIndex2;
	pIndex2 << PLACING_TYPE;
	gText.loadFromRenderedText(pIndex2.str().c_str(), {255, 255, 255, 255}, gFont13);
	gText.render(5, 31, gText.getWidth(), gText.getHeight());
	if (PLACING_TYPE == 2){
		SDL_Rect placeblockborder2 = { BLOCKX, BLOCKY, BLOCKW * HOWMANYBLOCKSW, BLOCKH* HOWMANYBLOCKSH };
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(gRenderer, &placeblockborder2);

		std::stringstream pIndex;
		pIndex << blockIndex;
		gText.loadFromRenderedText(pIndex.str().c_str(), {244, 144, 25, 255}, gFont13);
		gText.render(5, 44, gText.getWidth(), gText.getHeight());
	}
	if (PLACING_TYPE == 3){
		SDL_Rect placeblockborder2 = { BLOCKX, BLOCKY, BLOCKW * HOWMANYBLOCKSW, BLOCKH* HOWMANYBLOCKSH };
		SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(gRenderer, &placeblockborder2);

		std::stringstream pIndex;
		pIndex << arrowIndex;
		gText.loadFromRenderedText(pIndex.str().c_str(), {0, 191, 255, 255}, gFont13);
		gText.render(5, 44, gText.getWidth(), gText.getHeight());
	}
}








