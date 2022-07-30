
//Texture wrapper class
class LTexture {
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	#ifdef _SDL_TTF_H
	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor,
			TTF_Font *gFont);
	#endif

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, int w, int h, SDL_Rect* clip = NULL,
			double angle = 0.0, SDL_Point* center = NULL,
			SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

class LWindow {
public:
	//Intializes internals
	LWindow();

	//Creates window
	bool init();

	//Creates renderer from internal window
	SDL_Renderer* createRenderer();

	//Handles window events
	void handleEvent(SDL_Event& e);

	//Deallocates internals
	void free();

	//Window dimensions
	int getWidth();
	int getHeight();

	//Window focii
	bool hasMouseFocus();
	bool hasKeyboardFocus();
	bool isMinimized();

	//Window data
	SDL_Window* mWindow;

private:

	//Window dimensions
	int mWidth;
	int mHeight;

	//Window focus
	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mFullScreen;
	bool mMinimized;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
//SDL_Window* gWindow = NULL;

//Our custom window
LWindow gWindow;

//The window renderer
SDL_Renderer* gRenderer = NULL;

const int PARTICLE_COLORS_COUNT = 6;
float zoomFactor=1.0;

//Globally used font
TTF_Font *gFont = NULL;
TTF_Font *gFont13 = NULL;
TTF_Font *gFont26 = NULL;

//LTexture gTextImage;
LTexture gText;
LTexture gBG;
LTexture gArrow;
LTexture gAxe;
LTexture gGrass;
LTexture gStone;
LTexture gEasterEgg;




LTexture gTower1Stand;
LTexture gTower1;
LTexture gTowerBullet;

LTexture gPlayer;
LTexture gParticles;	//
SDL_Rect cParticles[ PARTICLE_COLORS_COUNT ];
LTexture gINDICATORORANGE;
LTexture gVisionRadius;
LTexture gPlanetGreen;
LTexture gPlanetPurple;
LTexture gPlanetGravity;

Mix_Chunk *sBow01 = NULL;
Mix_Chunk *sBow02 = NULL;
Mix_Chunk *sBow03 = NULL;
Mix_Chunk *sBow04 = NULL;


SDL_Surface *bmpSurf = NULL;
SDL_Texture *bmpTex = NULL;
SDL_Texture *texTarget = NULL;
SDL_Event event;
SDL_Rect zoomRect;

#define MAX_ZOOM_OUT    1
#define MAX_ZOOM_IN     4

//The timer
class Timer
{
    private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

    public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};


Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}









LTexture::LTexture() {
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture() {
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path) {
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(),
		IMG_GetError());
	} else {
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE,
				SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n",
					path.c_str(), SDL_GetError());
		} else {
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText(std::string textureText,
		SDL_Color textColor, TTF_Font *gFont) {
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(),
			textColor);
	if (textSurface != NULL) {
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL) {
			printf(
					"Unable to create texture from rendered text! SDL Error: %s\n",
					SDL_GetError());
		} else {
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	} else {
		printf("Unable to render text surface! SDL_ttf Error: %s\n",
		TTF_GetError());
	}

	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free() {
	//Free texture if it exists
	if (mTexture != NULL) {
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha) {
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, int w, int h, SDL_Rect* clip, double angle,
		SDL_Point* center, SDL_RendererFlip flip) {
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, w, h };

	//Set clip rendering dimensions
	if (clip != NULL) {
		//Switch to this if you wish to set the Width and Height manually
		renderQuad.w = w;
		renderQuad.h = h;

		//Switch to this if you wish to set the Width and Height to the Clipping Width and Height
		//renderQuad.w = clip->w;
		//renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center,
			flip);
}

int LTexture::getWidth() {
	return mWidth;
}

int LTexture::getHeight() {
	return mHeight;
}

LWindow::LWindow() {
	//Initialize non-existant window
	mWindow = NULL;
	mMouseFocus = false;
	mKeyboardFocus = false;
	mFullScreen = false;
	mMinimized = false;
	mWidth = 0;
	mHeight = 0;
}

bool LWindow::init() {
	//Create window
	mWindow = SDL_CreateWindow("Adventures of Space Wars",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE ); //| SDL_WINDOW_BORDERLESS);/// | SDL_WINDOW_MAXIMIZED);
	if (mWindow != NULL) {
		mMouseFocus = true;
		mKeyboardFocus = true;
		mWidth = SCREEN_WIDTH;
		mHeight = SCREEN_HEIGHT;
	}

	return mWindow != NULL;
}

SDL_Renderer* LWindow::createRenderer() {
	return SDL_CreateRenderer(mWindow, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void LWindow::handleEvent(SDL_Event& e) {
	//Set window mode to borderless at all times
	//SDL_SetWindowBordered(gWindow.mWindow,SDL_FALSE);

	//Window event occured
	if (e.type == SDL_WINDOWEVENT) {
		//Caption update flag
		bool updateCaption = false;

		switch (e.window.event) {
		//Get new dimensions and repaint on winwdow size change
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			mWidth = e.window.data1;
			mHeight = e.window.data2;
			SDL_RenderPresent(gRenderer);
			break;

			//Repaint on exposurew
		case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent(gRenderer);
			break;

			//Mouse entered window
		case SDL_WINDOWEVENT_ENTER:
			mMouseFocus = true;
			updateCaption = true;
			break;

			//Mouse left window
		case SDL_WINDOWEVENT_LEAVE:
			mMouseFocus = false;
			updateCaption = true;
			break;

			//Window has keyboard focus
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			mKeyboardFocus = true;
			updateCaption = true;
			break;

			//Window lost keyboard focus
		case SDL_WINDOWEVENT_FOCUS_LOST:
			mKeyboardFocus = false;
			updateCaption = true;
			break;

			//Window minimized
		case SDL_WINDOWEVENT_MINIMIZED:
			mMinimized = true;
			break;

			//Window maxized
		case SDL_WINDOWEVENT_MAXIMIZED:
			mMinimized = false;
			break;

			//Window restored
		case SDL_WINDOWEVENT_RESTORED:
			mMinimized = false;
			break;
		}

		//Update window caption with new data
		if (updateCaption) {
			std::stringstream caption;
			caption << "Adventures of Space Wars - MouseFocus:"
					<< ((mMouseFocus) ? "On" : "Off") << " KeyboardFocus:"
					<< ((mKeyboardFocus) ? "On" : "Off");
			SDL_SetWindowTitle(mWindow, caption.str().c_str());
		}
		//SDL_RenderPresent(gRenderer);
	}
	//Enter exit full screen on return key
	//else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
	else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F11) {
		if (mFullScreen) {
			SDL_SetWindowFullscreen(mWindow, SDL_FALSE);
			mFullScreen = false;
		} else {
			SDL_SetWindowFullscreen(mWindow, SDL_TRUE);
			mFullScreen = true;
			mMinimized = false;
		}
	}
}

void LWindow::free() {
	if (mWindow != NULL) {
		SDL_DestroyWindow(mWindow);
	}

	mMouseFocus = false;
	mKeyboardFocus = false;
	mWidth = 0;
	mHeight = 0;
}

int LWindow::getWidth() {
	return mWidth;
}

int LWindow::getHeight() {
	return mHeight;
}

bool LWindow::hasMouseFocus() {
	return mMouseFocus;
}

bool LWindow::hasKeyboardFocus() {
	return mKeyboardFocus;
}

bool LWindow::isMinimized() {
	return mMinimized;
}

bool init() {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init( SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	} else {
		//Set texture filtering to linear
		SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1");

		//Create window
		//gWindow = SDL_CreateWindow("TestMe", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		gWindow.init();

		//Create vsynced renderer for window
		//gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);// "| SDL_RENDERER_PRESENTVSYNC);//original
		//gRenderer = SDL_CreateRenderer(gWindow, -1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); //original
		gRenderer = gWindow.createRenderer();

		//Initialize renderer color
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

		//Initialize PNG loading
		int imgFlags = IMG_INIT_PNG;
		IMG_Init(imgFlags);
		TTF_Init();

		//SDL_Init(SDL_INIT_EVERYTHING);

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
			printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n",
			Mix_GetError());
			success = false;
		}

	}

	return success;
}


//Set clips to Particular rect
void setClipsParticlesRect( int i, int x, int y, int w, int h){
	cParticles[ i ].x = x;
	cParticles[ i ].y = y;
	cParticles[ i ].w = w;
	cParticles[ i ].h = h;
}


bool loadMedia() {
	//Loading success flag
	bool success = true;

	//gFont = TTF_OpenFont("fonts/SDONE_0.ttf", 50);
	//gFont = TTF_OpenFont("fonts/Roboto-Regular.ttf", 20);
	gFont = TTF_OpenFont("fonts/PT_Sans-Narrow-Web-Regular.ttf", 26);
	gFont13 = TTF_OpenFont("fonts/PT_Sans-Narrow-Web-Regular.ttf", 16);
	gFont26 = TTF_OpenFont("fonts/PT_Sans-Narrow-Web-Regular.ttf", 26);


	sBow01 = Mix_LoadWAV("sounds/bow01.wav");
	sBow02 = Mix_LoadWAV("sounds/bow02.wav");
	sBow03 = Mix_LoadWAV("sounds/bow03.wav");
	sBow04 = Mix_LoadWAV("sounds/bow04.wav");

	gBG.loadFromFile("img/bg.png");
	gArrow.loadFromFile("img/arrow.png");
	gAxe.loadFromFile("img/axe.png");
	gGrass.loadFromFile("img/grass.png");
	gStone.loadFromFile("img/stone.png");
	gEasterEgg.loadFromFile("img/easterEgg.png");




	gTower1Stand.loadFromFile("img/tower1_stand.png");
	gTower1.loadFromFile("img/tower1.png");
	gTowerBullet.loadFromFile("img/tower1_bullet.png");

	gPlayer.loadFromFile("img/player.png");

	gINDICATORORANGE.loadFromFile("img/indicator_orange.png");
	gVisionRadius.loadFromFile("img/vr.png");
	gPlanetGreen.loadFromFile("img/planets/planet_green_large.png");
	gPlanetPurple.loadFromFile("img/planets/planet_purple_large.png");
	gPlanetGravity.loadFromFile("img/planets/planet_gravity.png");

    //Load then set sprite clips
	gParticles.loadFromFile("img/particle/particles.png");
	setClipsParticlesRect(0, 0, 0, 8, 8);  //Blue Particle
	setClipsParticlesRect(1, 8, 0, 8, 8);  //Green Particle
	setClipsParticlesRect(2, 16, 0, 8, 8); //Orange Particle
	setClipsParticlesRect(3, 24, 0, 8, 8); //Red Particle
	setClipsParticlesRect(4, 32, 0, 8, 8); //White Particle
	setClipsParticlesRect(5, 48, 0, 8, 8); //Yellow Particle


	//Rendering images on images
	texTarget = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 200, 200);
	bmpSurf = IMG_Load("img/planets/planet_purple_large.png");
	bmpTex = SDL_CreateTextureFromSurface(gRenderer, bmpSurf);





	return success;
}
//Function to make it easier to setup rectangles
void setupRect(SDL_Rect *rect,int x,int y,int w,int h)
{
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}

void close() {
	TTF_CloseFont(gFont);
	gFont = NULL;
	gFont13 = NULL;
	gFont26 = NULL;

	gText.free();
	gBG.free();
	gArrow.free();
	gAxe.free();
	gGrass.free();
	gStone.free();
	gEasterEgg.free();


	gTower1Stand.free();
	gTower1.free();
	gTowerBullet.free();
	gPlayer.free();
	gParticles.free();
	gINDICATORORANGE.free();
	gVisionRadius.free();
	gPlanetGreen.free();
	gPlanetPurple.free();
	gPlanetGravity.free();

	Mix_FreeChunk(sBow01);
	Mix_FreeChunk(sBow02);
	Mix_FreeChunk(sBow03);
	Mix_FreeChunk(sBow04);
	sBow01 = NULL;
	sBow02 = NULL;
	sBow03 = NULL;
	sBow04 = NULL;

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	gWindow.free();

	//Quit SDL subsystems
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
