/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
 and may not be redistributed without written permission.*/

//https://www.youtube.com/watch?v=LBkKbpKHRas
//Using SDL, SDL_image, standard IO, vectors, and strings
#if 0
#!/bin/sh

gcc -Wall `sdl-config --cflags` udpc.c -o udpc `sdl-config --libs` -lSDL_net

exit
#endif

#undef UNICODE

#define _WIN32_WINNT 0x501
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mouse.h>
#include <string.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif

#include <ctime>
#include <time.h>       /* time */

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream, std::stringbuf

#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <math.h>       /* atan2 */

#define PI 3.14159265


using namespace std;

//int SCREEN_WIDTH = 1920;
//int SCREEN_HEIGHT = 1080;

//int SCREEN_WIDTH = 960;
//int SCREEN_HEIGHT = 640;

int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 720;
int BLOCKS = 0;
int ARROWS = 0;
bool leftclick = false;

int BLOCKCOUNT = 0;
bool debug = false;

int PLACING_TYPE = 1;	 //Determine type of block category
int arrowIndex = 1;		 //Determine type of arrow
int blockIndex = 1;		 //Determine type of block to place
int HOWMANYBLOCKSW = 1;
int HOWMANYBLOCKSH = 1;
int BLOCKX;
int BLOCKY;
int BLOCKW = 32;
int BLOCKH = 32;
int BLOCKRADIUS = 500;
string Turn = "1"; //Whose turn it is
bool shootArrow = true;
bool applyDamage = true;

const int HALF = 2;
const double BLANK_ANGLE = 0.0;

//The frames per second
const int FRAMES_PER_SECOND = 60;
SDL_Surface *message = NULL;

#include "init.h"

//double round(double d)
//{
  //return floor(d + 0.5);
//}


//Player 1
struct Cam {
public:
	double x = 0, y = 0;
	bool moveup = false, movedown = false, moveleft = false, moveright = false;
} cam;





//Player 1
struct Player {
public:
	double x = 0, y = 0;
	int w = 32, h = 64;
	int health = 100;
	double speedX, speedY;
	double angle = 0.0, radians = 0.0, radianCos = 0.0, radianSin = 0.0;
	double rotateSpeed = 0.0;
	bool rotateLeft, rotateRight;
	int time = 0;
	int centerx, centery;
	int visionX, visionY, visionRadius = 600;
	int rotateSpeedAccel = 0.0, rotateSpeedDecel = 0.0;
	bool collide = false;
	bool camlocked = true;
	bool moveup   = false, movedown  = false,
		 moveleft = false, moveright = false;
	double AttackSpeed = 2.50000;
	double delayT = 0;
	bool delay = false;
	bool initialshot = false;
	bool canceled = false;
	bool shift = false;
	string side = "r";
	string powerup = "arrow";
	string mode = "player";
	string travelMode = "fight";
} player, player2;

void checkMaxLimitDouble(double &value, double maxLimitNeg, double maxLimitNegNew, double maxLimitPos, double maxLimitPosNew){
	//Set max limits for one number (double)
	if (value <= maxLimitNeg){
		value = maxLimitNegNew;
	}
	if (value >= maxLimitPos){
		value= maxLimitPosNew;
	}
}

//Arrows and blocks
#include "structs.h"

int main(int argc, char **argv) {

	bool quit = false;

	init();
	loadMedia();

	SDL_Color textColor = { 0, 0, 0xFF, 0 };
	SDL_Event e;


	//Center camera on player 1 in the beginning
	cam.x = player.x + player.w/2;
	cam.y = player.y + player.h/2;

	//Starting positions
	player.x = 1000;
	player.y = 508;
	player2.x = 3000;
	player2.y = 508;

	//Mouse
	int mx, my;
    int mxDum, myDum;
    double mxDistance;
	int mouseWheelX;
	int mouseWheelY;
	int mouseWheelXMotion;
	int mouseWheelYMotion;

	int mx1 = 0, my1 = 0;
	int mx2 = 0, my2 = 0;
	double mDistance = 0.0, speed = 0.0, mvX = 0.0, mvY = 0.0, mAngle = 0.0,
		   radian = 0.0, rCos = 0.0, rSin = 0.0;
	double mSpeedOld = 0.0, mvXOld = 0.0, mvYOld = 0.0, mAngleOld = 0.0;


	SDL_Color black = { 0, 0, 0, 0 };
	SDL_Color red = { 255, 99, 71, 255 };
	SDL_Color green = { 0, 255, 0, 255 };
	SDL_Color blue = { 100, 149, 237, 255 };
	SDL_Color white = { 255, 255, 255, 255 };

	static Arrow arrow[200];
	static Block block[200];
	initArrow(arrow);
	initBlock(block);

	//////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////-----------------------------------------------------/////////////////////
	//////////////////////////////////// Loading Block data /////////////////////////////////////
	//Remove ALL Block(s), before loading a new file
	BLOCKS = 0;
	for (int i = 0; i < 200; i++) {
		if (block[i].alive) {
			block[i].alive = false;
		}
	}
	//Load Block(s)
	fstream fileBlocksc("levels/level01_block_num.dat");
	if (fileBlocksc.is_open()) {
		fileBlocksc >> BLOCKS;
	}
	fileBlocksc.close();
	//Place BLANK Block(s)
	for (int i = 0; i < BLOCKS; i++) {
		spawnBlock(block,0,0,0,0);
		BLOCKS--;		//I have no idea why but if i don't keep this here, it places ALOT of blocks
	}
	//Load Block(s) data and apply to BLANK Block(s)
	fstream fileBlockCoor("levels/level01_block.dat");
	if (fileBlockCoor.is_open()) {
		for (int i = 0; i < BLOCKS; ++i) {
			fileBlockCoor   >> block[i].type
						    >> block[i].x  	>> block[i].y
						    >> block[i].w  	>> block[i].h ;
		}
	}
	fileBlockCoor.close();
	//////////////////////////////////// Loading Block data /////////////////////////////////////
	////////////////////-----------------------------------------------------/////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////




	//Auto-save
	int savtimer = 0;
	bool saving = false;
	bool autosav = false;

	//Particles
	int rand1;
	int rand2;
	int rand3;


	//Unit measurements
	double meter = 1;
	double second = 1;


	int timerL = 0;
	int timerR = 0;
	int timerU = 0;
	int timerD = 0;

    //Keep track of the current frame
    int frame = 0;

    //Whether or not to cap the frame rate
    bool cap = true;

    //The frame rate regulator
    Timer fps;

	while (!quit) {
		//If not debug mode set placing type = 1 so we are not placing anything
		if (!debug){
			//PLACING_TYPE = 1;
		}

		//SDL_SetWindowBordered(gWindow.mWindow,SDL_FALSE);
		//SDL_SetWindowFullscreen(gWindow.mWindow,true);

        //Start the frame timer
        fps.start();

		//Center of player
		player.centerx = player.x + player.w / 2-gWindow.getWidth()/2;
		player.centery = player.y + player.h / 2-gWindow.getHeight()/2;

		//Center of player vision
		player.visionX = player.x + player.w / 2;
		player.visionY = player.y + player.h / 2;

		//Random number
		rand1 = rand() % 74+26;
		rand2 = rand() % 74+26;
		rand3 = rand() % 74+26;


		//Get mouse X, and Y position and store it on BLOCKX, and BLOCKY
		SDL_GetMouseState(&mx, &my);
		BLOCKX = mx-BLOCKW * HOWMANYBLOCKSW;
		BLOCKY = my-BLOCKH * HOWMANYBLOCKSW;

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////// MOVING /////////////////////////////////////////////////////////
		//--------------------------------------------------------------------------------------------------------------------------//
		if (debug){
			player.mode = "editor";
		}
		if (!debug){
			player.mode = "player";
		}
		//Moving Player
		if (player.moveleft) {
			player.speedX -= 1;
		}else if (player.moveright) {
			player.speedX += 1;
		}
		if (player.moveup) {
			player.speedY -= 1;
		} else if (player.movedown) {
			player.speedY += 1;
		}

		//Add player speed (more so turn speed, I can make perfect circles with this)
		player.x += player.speedX;
		player.y += player.speedY;

		//Decelerate player
		if (!player.moveleft) {
			if (player.speedX < 0) {
				player.speedX += floor(0.5 *10+0.5)/10;
			}
		}
		if (!player.moveright) {
			if (player.speedX > 0) {
				player.speedX -= floor(0.5 *10+0.5)/10;
			}
		}
		if (!player.moveup) {
			if (player.speedY < 0) {
				player.speedY += floor(0.5 *10+0.5)/10;
			}
		}
		if (!player.movedown) {
			if (player.speedY > 0) {
				player.speedY -= floor(0.5 *10+0.5)/10;
			}
		}

		//Ship follows mouse
		if (player.travelMode == "fight"){
			player.angle = atan2(my+cam.y - player.y-player.h/2,mx+cam.x - player.x-player.w/2);
			player.angle = player.angle * (180 / PI);
			//Set player angle max limits
			if (player.angle < 0) {
				player.angle = 360 - (-player.angle);
			}
		}
		//Ship follows rotation
		if (player.travelMode == "travel"){
			//Add players angle with the rotation speed; Calculate players radian
			player.angle += player.rotateSpeed;
			//Angle/Rotation of Player
			if (player.rotateLeft) {
				if (player.rotateSpeed > -3) {
					player.rotateSpeedAccel += 1;
					if (player.rotateSpeedAccel > 5){
						player.rotateSpeedAccel = 0;
						player.rotateSpeed -= floor(0.5 *10+0.5)/10;
					}
				}
			} else if (player.rotateRight) {
				if (player.rotateSpeed < 3) {
					player.rotateSpeedAccel += 1;
					if (player.rotateSpeedAccel > 5){
						player.rotateSpeedAccel = 0;
						player.rotateSpeed += floor(0.5 *10+0.5)/10;
					}
				}
			}
			//Player thrust decay speed
			if (!player.rotateLeft) {
				if (player.rotateSpeed < 0) {
					player.rotateSpeedDecel += 1;
					if (player.rotateSpeedDecel > 5){
						player.rotateSpeedDecel = 0;
						player.rotateSpeed += floor(0.5 *10+0.5)/10;
					}
				}
			}
			if (!player.rotateRight) {
				if (player.rotateSpeed > 0) {
					player.rotateSpeedDecel += 1;
					if (player.rotateSpeedDecel > 5){
						player.rotateSpeedDecel = 0;
						player.rotateSpeed -= floor(0.5 *10+0.5)/10;
					}
				}
			}
		}

		player.radians = (PI/180)*(player.angle);
		player.radianCos = floor(cos(player.radians)*10+0.5)/10;
		player.radianSin = floor(sin(player.radians)*10+0.5)/10;

		//Max limits
		checkMaxLimitDouble(player.speedX, -10, -10.0000000000000, 10, 10.0000000000000);
		checkMaxLimitDouble(player.speedY, -10, -10.0000000000000, 10, 10.0000000000000);

		//If near zero, turn it to -0.10 or 0.10
		if (player.speedX >= -0.10 && player.speedX < 0.00){
			player.speedX = -0.10;
		}
		if (player.speedX > 0.00 && player.speedX <= 0.10){
			player.speedX = 0.10;
		}
		if (player.speedY >= -0.10 && player.speedY < 0.00){
			player.speedY = -0.10;
		}
		if (player.speedY > 0.00 && player.speedY <= 0.10){
			player.speedY = 0.10;
		}

		//Calculate distance of mouse and player relative to camera
		mxDum = mx;
		myDum = my;
        mxDistance =sqrt((mxDum+cam.x - player.x-player.w/2-cos(player.radians)*16) * (mxDum+cam.x - player.x-player.w/2-cos(player.radians)*16)
					   + (myDum+cam.y - player.y-player.h/2-sin(player.radians)*16) * (myDum+cam.y - player.y-player.h/2-sin(player.radians)*16));
		//--------------------------------------------------------------------------------------------------------------------------//
		///////////////////////////////////////////////////////////// MOVING /////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}

			//Handle window events
			gWindow.handleEvent(e);

			//If Key Pressed
			if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
				switch (e.key.keysym.sym) {
				case SDLK_a:
					player.moveleft = true;
					break;
				case SDLK_d:
					player.moveright = true;
					break;
				case SDLK_w:
					//player.moveup = true;
					break;
				case SDLK_s:
					//player.movedown = true;
					break;
				case SDLK_z:
					player.rotateLeft = true;
					break;
				case SDLK_x:
					player.rotateRight = true;
					break;
				case SDLK_q:
					PLACING_TYPE += 1;
					break;
				case SDLK_c:
					if (PLACING_TYPE == 2){
						copyBlock(block);
					}
					break;
				case SDLK_e:
					if (player.travelMode == "travel"){
						player.travelMode = "fight";
					}else{
						player.travelMode = "travel";
					}
					break;
				case SDLK_h:
					if (debug == false){
						debug = true;
					}else{
						debug = false;
					}
					break;
				case SDLK_y:
					if (!player.camlocked){
						player.camlocked = true;
					}else{
						player.camlocked = false;
					}
					break;
				case SDLK_UP:
					cam.moveup = true;
					break;
				case SDLK_DOWN:
					cam.movedown = true;
					break;
				case SDLK_LEFT:
					cam.moveleft = true;
					break;
				case SDLK_RIGHT:
					cam.moveright = true;
					break;
				case SDLK_SPACE:
					//
					break;
				case SDLK_RETURN:
					//
					break;
				case SDLK_0:
					saving = true;
					break;
				case SDLK_1:
					if (PLACING_TYPE == 2){
						addMinusBlockWH(block, "addW");
					}
					break;
				case SDLK_2:
					if (PLACING_TYPE == 2){
						addMinusBlockWH(block, "addH");
					}
					break;
				case SDLK_3:
					if (PLACING_TYPE == 2){
						addMinusBlockWH(block, "minusW");
					}
					break;
				case SDLK_4:
					if (PLACING_TYPE == 2){
						addMinusBlockWH(block, "minusH");
					}
					break;
				case SDLK_LSHIFT:
					player.shift = true;
					break;
				case SDLK_LEFTBRACKET:
					if (HOWMANYBLOCKSW > 1) {
						HOWMANYBLOCKSW -= 1;
					}
					break;
				case SDLK_RIGHTBRACKET:
					HOWMANYBLOCKSW += 1;
					break;
				case SDLK_MINUS:
					if (HOWMANYBLOCKSH > 1) {
						HOWMANYBLOCKSH -= 1;
					}
					break;
				case SDLK_EQUALS:
					HOWMANYBLOCKSH += 1;
					break;
				case SDLK_ESCAPE:
					BLOCKW += 16;
					BLOCKH += 16;
					BLOCKW -= 16;
					BLOCKH -= 16;
					break;
				}
			}
			//If Key Released
			else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
				switch (e.key.keysym.sym) {
				case SDLK_a:
					player.moveleft = false;
					break;
				case SDLK_d:
					player.moveright = false;
					break;
				case SDLK_w:
					//player.moveup = false;
					break;
				case SDLK_s:
					//player.movedown = false;
					break;
				case SDLK_z:
					player.rotateLeft = false;
					break;
				case SDLK_x:
					player.rotateRight = false;
					break;
				case SDLK_q:
					player.rotateLeft = false;
					break;
				case SDLK_e:
					player.rotateRight = false;
					break;
				case SDLK_UP:
					cam.moveup = false;
					break;
				case SDLK_DOWN:
					cam.movedown = false;
					break;
				case SDLK_LEFT:
					cam.moveleft = false;
					break;
				case SDLK_RIGHT:
					cam.moveright = false;
					break;
				case SDLK_c:
					//
					break;
				case SDLK_LSHIFT:
					player.shift = false;
					break;
				case SDLK_RETURN:
                    //
					break;
				case SDLK_SPACE:
					//
					break;
				}
			}

			//Mouse wheel
			if (e.type == SDL_MOUSEWHEEL) {
				mouseWheelX = e.wheel.x;
				mouseWheelY = e.wheel.y;
				mouseWheelXMotion = e.motion.x;
				mouseWheelYMotion = e.motion.x;
				if (PLACING_TYPE == 2){
					blockIndex -= mouseWheelY;
				}
				if (PLACING_TYPE == 3){
					arrowIndex -= mouseWheelY;
				}
			}

			if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					if (PLACING_TYPE == 1) {
						mx1 = mx;
						my1 = my;
					}
					leftclick = true;
				}
			}

			if (e.type == SDL_MOUSEBUTTONUP) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					leftclick = false;
					//Start shooting
					if (PLACING_TYPE == 1) {
						if (shootArrow){
							//We do this part here so that if the player decides to shoot again even though the camera is on the arrow,
							//it will instead go to the newly spawned arrow
							//for (int i = 0; i < 200; i++) {
							///	if (arrow[i].alive) {
							//		arrow[i].camFollow = false;
							//	}
							//}
							//Shoot your arrow if you didnt cancel
							if (!player.canceled){
								player.initialshot = true;

								//Store last shooting data
								mSpeedOld = speed;
								mAngleOld = mAngle;
							}
							player.canceled = false;
						}
					}
				}
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (e.button.button == SDL_BUTTON_RIGHT) {
					if (PLACING_TYPE==1){
						if (shootArrow){
							player.canceled = true;
						}
					}
					if (PLACING_TYPE==2){
						for (int i = 0; i < HOWMANYBLOCKSW; i++) {
							for (int h = 0; h < HOWMANYBLOCKSH; h++) {
								spawnBlock(block,BLOCKX + i * BLOCKW +cam.x,BLOCKY + h * BLOCKW +cam.y,BLOCKW, BLOCKH);
							}
						}
					}
				}
			}
		} // end controls

		if (!gWindow.isMinimized()) {
			//////////////////////////////////////////////////
			/////////////////// MAX LIMITS ///////////////////
			//----------------------------------------------//
			if (PLACING_TYPE > 4){
				PLACING_TYPE = 1;
			}
			if (blockIndex <= 1){
				blockIndex = 1;
			}
			if (arrowIndex <= 1){
				arrowIndex = 1;
			}
			if (BLOCKW <= 8){
				BLOCKW = 8;
			}
			if (BLOCKH <= 8){
				BLOCKH = 8;
			}
			//----------------------------------------------//
			/////////////////// MAX LIMITS ///////////////////
			//////////////////////////////////////////////////




			///////////////////////////////////////////////////
			//////////////////// CAMERA ///////////////////////
			//-----------------------------------------------//
			if (!player.camlocked){
				if (mx <= 0){
					cam.x -= 20;
				}
				if (mx >= gWindow.getWidth()-1){
					cam.x += 20;
				}
				if (my <= 0){
					cam.y -= 20;
				}
				if (my >= gWindow.getHeight()-1){
					cam.y += 20;
				}
			}
			//-----------------------------------------------//
			//////////////////// CAMERA ///////////////////////
			///////////////////////////////////////////////////



			//Update structs
			updateBlock(block, mx+cam.x, my+cam.y);
			updateArrow(arrow, cam.x, cam.y);
			checkCollisionArrow(arrow,block,player.x, player.y, player.w, player.h, player.health,player2.x, player2.y, player2.w, player2.h, player2.health);

			if (PLACING_TYPE == 1) {
				//If there is an arrow that is shot then disable players from shooting until arrow has disappeared
				for (int i = 0; i < 200; i++) {
					if (arrow[i].alive) {
						if (arrow[i].camFollow){
							shootArrow = false;
						}
					}
				}

				//Center camera on player that is shooting
				if (shootArrow){

				}

				//Center camera on arrow that player shot
				if (!shootArrow){
					for (int i = 0; i < 200; i++) {
						if (arrow[i].alive) {
							if (arrow[i].type == "arrowKillPlayer1") {
								if (player.camlocked){
									cam.x = arrow[i].x + arrow[i].w/2 - gWindow.getWidth()/2;
									cam.y = arrow[i].y + arrow[i].h/2 - gWindow.getHeight()/2;
								}
							}
							if (arrow[i].type == "arrowKillPlayer2") {
								if (player.camlocked){
									cam.x = arrow[i].x + arrow[i].w/2 - gWindow.getWidth()/2;
									cam.y = arrow[i].y + arrow[i].h/2 - gWindow.getHeight()/2;
								}
							}
						}
					}
				}
			}



			//Kep camera of player, unless changed by other force
			if (leftclick) {
				if (PLACING_TYPE == 1) {
					mx2 = mx;
					my2 = my;

					//Calculate distance of enemy and player
					mDistance = sqrt((mx2-5 - mx1)* (mx2-5 - mx1)
								   + (my2-5 - my1)* (my2-5 - my1));
					if (mDistance < 1){
						mDistance = 1;
					}

					//Find velocity from both points //THIS IS THE VELOCITY WE ARE GOING TO GIVE OUR ARROW
					mvX = 1 * (mx1 - mx2) / mDistance;
					mvY = 1 * (my1 - my2) / mDistance;

					//Limit distance of both points to 50
					speed = (50*mDistance)/400;	//What this does is take a value (mDistance), and it takes 400 real pixels to reach the value 50
					if (speed < 1){
						speed = 1;
					}
					if (speed > 50){
						speed = 50;
					}

					//Convert to units of 0.5 //THIS IS OUR NEW SPEED TO APPLY TO OUR ARROW
					speed = floor(speed *10+0.5)/10;
				}
			}


				//Only shoot if we are not placing anything
				if (PLACING_TYPE == 1) {
					//If there is no delay then shoot!
					if (!player.delay) {
						if (player.initialshot) {
							player.delay = true;
							player.initialshot = false;
							//Arrow shot
							if (player.powerup == "arrow") {
								if (shootArrow){
									//Who's turn it is
									if (Turn == "1"){
										int randSound;
										randSound = rand() % 4 + 1;
										cout << randSound << endl;
										if (randSound == 1){
											Mix_PlayChannel(-1, sBow01, 0);
										}
										if (randSound == 2){
											Mix_PlayChannel(-1, sBow02, 0);
										}
										if (randSound == 3){
											Mix_PlayChannel(-1, sBow03, 0);
										}
										if (randSound == 4){
											Mix_PlayChannel(-1, sBow04, 0);
										}
										fireArrow(arrow, 100, "arrowKillPlayer2",
												   cos(player.radians),
												   sin(player.radians),
												   player.x+player.w-16,
												   player.y+player.h/2-16,
												   32, 16,
												   mvX, mvY, speed,
												   1, 255,
												   100, 1);
									}
									if (Turn == "2"){
										int randSound;
										randSound = rand() % 4 + 1;
										cout << randSound << endl;
										if (randSound == 1){
											Mix_PlayChannel(-1, sBow01, 0);
										}
										if (randSound == 2){
											Mix_PlayChannel(-1, sBow02, 0);
										}
										if (randSound == 3){
											Mix_PlayChannel(-1, sBow03, 0);
										}
										if (randSound == 4){
											Mix_PlayChannel(-1, sBow04, 0);
										}
										fireArrow(arrow, 100, "arrowKillPlayer1",
												   cos(player.radians),
												   sin(player.radians),
												   player2.x-16,
												   player2.y+player2.h/2-16,
												   32, 16,
												   mvX, mvY, speed,
												   1, 255,
												   100, 1);
									}
								}
							}
						}
					} // end  (!player.delay)

					//Start delay timer after our first shot!
					if (player.delay) {
						player.delayT += player.AttackSpeed;
						if (player.delayT > 30) {
							player.delay = false;
							player.delayT = 0;
						}
					}
				}

				if (leftclick) {
					if (PLACING_TYPE == 2) {
						removeBlock(block);
					}
					if (PLACING_TYPE == 3) {
						//removeArrow(arrow);
					}
				}

			//Clear Screen
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderClear(gRenderer);

			//Background
			//gBG.render(0,0, gBG.getWidth(), gBG.getHeight());

			//Render structs
			renderArrow(arrow, cam.x, cam.y);
			renderBlock(block, cam.x, cam.y);

			//Render our player(s)
			gPlayer.render(player.x-cam.x, player.y-cam.y, 32, player.h, NULL,0);
			SDL_Rect playerReal = { player.x-cam.x, player.y-cam.y, player.w, player.h };
			SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(gRenderer, &playerReal);

			gPlayer.render(player2.x-cam.x, player2.y-cam.y, 32, player2.h, NULL,0, NULL, SDL_FLIP_HORIZONTAL);
			SDL_Rect playerReal2 = { player2.x-cam.x, player2.y-cam.y, player2.w, player2.h };
			SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(gRenderer, &playerReal2);

			//Render Player shoot dirction
			SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0xFF );
			SDL_RenderDrawLine( gRenderer, player.x+player.w/2-cam.x,
										   player.y+player.h/2-cam.y,
										   player.x+player.w/2+(250*rCos)-cam.x,
										   player.y+player.h/2+(250*rSin)-cam.y );

			if (leftclick){
				if (PLACING_TYPE == 1) {
					if (shootArrow){

						//This is the center of the 2 points
						int newx, newy;
						newx = (mx2+mx1)/2;
						newy = (my2+my1)/2;

						SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0xFF );
						SDL_RenderDrawLine( gRenderer, mx1,my1,mx2,my2 );

						std::stringstream mDis;
						mDis << speed;
						gText.loadFromRenderedText(mDis.str().c_str(), {150,150,150,255}, gFont26);
						gText.render(newx, newy-gText.getHeight(), gText.getWidth(), gText.getHeight());

						std::stringstream mAng;
						mAng << mAngle <<"°";
						gText.loadFromRenderedText(mAng.str().c_str(), {150,150,150,255}, gFont26);
						gText.render(mx1, my1-gText.getHeight(), gText.getWidth(), gText.getHeight());

						//Calculate angle based on velocity(X,Y)
						mAngle = atan2 ( mvY, mvX) * 180 / PI;
						if (mAngle < 0) {
							mAngle = 360 - (-mAngle);
						}

						//Get radians from Angle
						radian = (PI/180)*(mAngle);
						rCos = floor(cos(radian)*10+0.5)/10;
						rSin = floor(sin(radian)*10+0.5)/10;

						if (Turn == "1"){
							gArrow.render(player.x+player.w/2 - cam.x,player.y+player.h/2-16 - cam.y,32,32, 0, mAngle);
						}
						if (Turn == "2"){
							gArrow.render(player2.x  - cam.x, player2.y+player2.h/2-16 - cam.y,32,32, 0, mAngle);
						}
					}
				}
			}

            //If player has canceled shot then reset variables
			if (PLACING_TYPE == 1) {
				if (player.canceled){
					leftclick =false;
					mvX = 0;
					mvY = 0;
					mDistance = 0;
					speed = 0;
				}
			}


			std::stringstream mAngOld;
			mAngOld << "Last °: "  << rCos;
			gText.loadFromRenderedText(mAngOld.str().c_str(), black, gFont26);
			gText.render(10, gWindow.getHeight()-gText.getHeight()-30, gText.getWidth(), gText.getHeight());

			std::stringstream mSpedOld;
			mSpedOld << "Last Speed: " << rSin;
			gText.loadFromRenderedText(mSpedOld.str().c_str(), black, gFont26);
			gText.render(10, gWindow.getHeight()-gText.getHeight()-10, gText.getWidth(), gText.getHeight());

			std::stringstream Health;
			Health << player.health;
			gText.loadFromRenderedText(Health.str().c_str(), blue, gFont26);
			gText.render(300, 10, gText.getWidth(), gText.getHeight());

			std::stringstream Health2;
			Health2 << player2.health;
			gText.loadFromRenderedText(Health2.str().c_str(), red, gFont26);
			gText.render(gWindow.getWidth()-gText.getWidth()-300, 10, gText.getWidth(), gText.getHeight());







			//FPS String
			if (debug){
				if (player.travelMode == "travel"){
					gText.loadFromRenderedText("travel", {0,255,0,0}, gFont13);
					gText.render(5, 5, gText.getWidth(), gText.getHeight());
				}
				if (debug){
					gText.loadFromRenderedText("Debug", {0,255,0,0}, gFont13);
					gText.render(55, 5, gText.getWidth(), gText.getHeight());
				}
				if (!debug){
					gText.loadFromRenderedText("Debug", {255,0,0,0}, gFont13);
					gText.render(55, 5, gText.getWidth(), gText.getHeight());
				}
				if (player.travelMode == "fight"){
					gText.loadFromRenderedText("fight", {255,0,0,0}, gFont13);
					gText.render(5, 5, gText.getWidth(), gText.getHeight());
				}
				if (player.camlocked){
					gText.loadFromRenderedText("cam: locked", {0,255,0,0}, gFont13);
					gText.render(5, 18, gText.getWidth(), gText.getHeight());
				}

				if (!player.camlocked){
					gText.loadFromRenderedText("cam: unlocked", {255,0,0,0}, gFont13);
					gText.render(5, 18, gText.getWidth(), gText.getHeight());
				}
				//Top left corner data
				renderBlockGhosts(mx,my);

				std::stringstream X;
				X << mx1 << " "<< my1;
				gText.loadFromRenderedText(X.str().c_str(), black, gFont13);
				gText.render(0, 66, gText.getWidth(), gText.getHeight());

				std::stringstream Y;
				Y << mx2 << " "<< my2;
				gText.loadFromRenderedText(Y.str().c_str(), black, gFont13);
				gText.render(0, 78, gText.getWidth(), gText.getHeight());

				std::stringstream mvxy;
				mvxy << mvX << " " << mvY;
				gText.loadFromRenderedText(mvxy.str().c_str(), black, gFont13);
				gText.render(0, 100, gText.getWidth(), gText.getHeight());

				std::stringstream mDis;
				mDis <<"Mouse X: " << mx+cam.x << "Mouse Y: "<< my+cam.y;
				gText.loadFromRenderedText(mDis.str().c_str(), black, gFont13);
				gText.render(0, 89, gText.getWidth(), gText.getHeight());
			}


			std::stringstream TURN;
			TURN <<"Turn - Player: " << Turn;
			gText.loadFromRenderedText(TURN.str().c_str(), black, gFont13);
			gText.render(gWindow.getWidth()/2-gText.getWidth()/2, 20, gText.getWidth(), gText.getHeight());

			/*

			if (debug){

				std::stringstream X;
				X << "x: " << player.x;
				gText.loadFromRenderedText(X.str().c_str(), white, gFont13);
				gText.render(player.x, player.y, gText.getWidth(), gText.getHeight());

				std::stringstream Y;
				Y << "y: " << player.y;
				gText.loadFromRenderedText(Y.str().c_str(), white, gFont13);
				gText.render(player.x, player.y+21, gText.getWidth(), gText.getHeight());

				std::stringstream MX;
				MX << "mx: " << mx;
				gText.loadFromRenderedText(MX.str().c_str(), white, gFont13);
				gText.render(mx-gText.getWidth(), my-gText.getHeight()*2, gText.getWidth(), gText.getHeight());

				std::stringstream MY;
				MY << "my: " << my;
				gText.loadFromRenderedText(MY.str().c_str(), white, gFont13);
				gText.render(mx-gText.getWidth(), my-gText.getHeight(), gText.getWidth(), gText.getHeight());

				std::stringstream bw;
				bw << BLOCKW*HOWMANYBLOCKSW;
				gText.loadFromRenderedText(bw.str().c_str(), white, gFont13);
				gText.render(mx, my, gText.getWidth(), gText.getHeight());

				std::stringstream bh;
				bh << BLOCKH*HOWMANYBLOCKSH;
				gText.loadFromRenderedText(bh.str().c_str(), white, gFont13);
				gText.render(mx, my+gText.getHeight(), gText.getWidth(), gText.getHeight());

				std::stringstream COLLIDE;
				COLLIDE << player.collide;
				gText.loadFromRenderedText(COLLIDE.str().c_str(), white, gFont13);
				gText.render(player.x, player.y+42, gText.getWidth(), gText.getHeight());

				std::stringstream X2;
				X2 << "x2: " << x2;
				gText.loadFromRenderedText(X2.str().c_str(), white, gFont13);
				gText.render(x2, y2, gText.getWidth(), gText.getHeight());

				std::stringstream Y2;
				Y2 << "y2: " << y2;
				gText.loadFromRenderedText(Y2.str().c_str(), white, gFont13);
				gText.render(x2, y2+21, gText.getWidth(), gText.getHeight());

				std::stringstream SIDE2;
				SIDE2 << side2;
				gText.loadFromRenderedText(SIDE2.str().c_str(), white, gFont13);
				gText.render(x2, y2+42, gText.getWidth(), gText.getHeight());

				std::stringstream COLLIDE3;
				COLLIDE3 << collide3;
				gText.loadFromRenderedText(COLLIDE3.str().c_str(), white, gFont13);
				gText.render(x3, y3, gText.getWidth(), gText.getHeight());

				std::stringstream SIDE3;
				SIDE3 << side3;
				gText.loadFromRenderedText(SIDE3.str().c_str(), white, gFont13);
				gText.render(x3, y3+21, gText.getWidth(), gText.getHeight());

			}*/





			//Hide Cursor
			//SDL_ShowCursor(0);
			SDL_RenderPresent(gRenderer);

	        //Increment the frame counter
	        frame++;

	        //If we want to cap the frame rate
	        if( ( cap == true ) && ( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) )
	        {
	            //Sleep the remaining frame time
	            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
	        }

			//If auto-save is enabled, start the auto-save timer for saving
			if (autosav) {
				savtimer += 1;
				if (savtimer > 100) { //Save every 5 minutes
					savtimer = 0;
					saving = true;
				}
			}

			if (saving){

				////////////////////////////////////////////
				////////////////// Block ///////////////////
				//Save # of Blocks
				ofstream myfileBlockc;
				myfileBlockc.open(
						"levels/level01_block_num.dat");
				myfileBlockc << BLOCKS;
				myfileBlockc.close();

				//Save X,Y coordinate of Enemy
				ofstream myfileblocksc;
				myfileblocksc.open("levels/level01_block.dat");
				for (int i = 0; i < 200; i++) {
					if (block[i].alive) {
						myfileblocksc  << block[i].type << "\n"
										<< block[i].x 	  << "\n"
										<< block[i].y 	  << "\n"
										<< block[i].w 	  << "\n"
										<< block[i].h 	  << "\n";
					}
				}
				myfileblocksc.close();
				////////////////// Block ///////////////////
				////////////////////////////////////////////


				saving = false;
			}

		} // end if gWindow Minimized

	}
	//Free resources and close SDL
	close();
	return 0;
}











//How to shoot
/*
fireParticle(particle, 50, "PLAYALAZER", cos(player.radians)*16,
									sin(player.radians)*16,
									player.x+player.w/2-13,
								    player.y+player.h/2-2,
									26, 4,
									cos(player.radians),sin(player.radians),
									30,
									1, 255, 200, 4, 0, 1, 1, 1);*/
