#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h> 
#include <math.h>
#include "Entities.h"
#include "Entities.cpp"

const int NUM_OBST = 3;
const int NUM_GRND = 2;
int score = 0;
int curObst = 0;
const double PI = std::atan(1) * 4;

const int MIN_OBST_Y = 150;
const int MAX_OBST_Y = 350;
const int GAP_SIZE = 200;

const std::string plyrMatPath = "assets/textures/box2.png";
const std::string deadMatPath = "assets/textures/box3.png";
const std::string grassMatPath = "assets/textures/grassg.png";
const std::string backMatPath = "assets/textures/background.png";

const std::string jumpPath = "assets/sounds/jump.wav";
const std::string deadPath = "assets/sounds/doom2.wav";
const std::string scorePath = "assets/sounds/score.wav";
const std::string score2Path = "assets/sounds/score2.wav";
sf::SoundBuffer jumpBuffer;
sf::SoundBuffer deadBuffer;
sf::SoundBuffer scoreBuffer;
sf::SoundBuffer score2Buffer;
sf::Sound jumpSound;
sf::Sound deadSound;
sf::Sound scoreSound;
sf::Sound score2Sound;


const std::string fontPath = "assets/fonts/Arial_Bold.ttf";
sf::Text scoreT;
sf::Text controlT;
sf::Font font;



const int STARTING_DIST = 1200;
const int STARTING_X = 250;
const int STARTING_Y = 225;
const float obstVel = -200;

const int ROOF_Y = 48;
const int GROUND_Y = 452;
const int DIST_BETWEEN = 450;
int framesBetweenJump = 15;
int framesBeforeReset = 120;
int jumpVel = -400;

int PLAYER_W = 48;
int PLAYER_H = 48;

SpriteEntity player = SpriteEntity(STARTING_X, STARTING_Y, 0, 0, PLAYER_W, PLAYER_H, 0, plyrMatPath);

SpriteEntity groundArr[2] =
{
    SpriteEntity(500, 475, obstVel, 0, 1000, 50, 0, grassMatPath),
    SpriteEntity(1500, 475, obstVel, 0, 1000, 50, 0, grassMatPath)
};

SpriteEntity roofArr[2] =
{
    SpriteEntity(500, 25, obstVel, 0, 1000, 50, 180, grassMatPath),
    SpriteEntity(1500, 25, obstVel, 0, 1000, 50, 180, grassMatPath)
};

SpriteEntity backArr1[2] =
{
    SpriteEntity(500, 100, obstVel/6, 0, 1000, 125, 180, backMatPath),
    SpriteEntity(1500, 100, obstVel/6, 0, 1000, 125, 180, backMatPath)

};

SpriteEntity backArr2[2] =
{
    SpriteEntity(500, 400, obstVel / 6, 0, 1000, 125, 0, backMatPath),
    SpriteEntity(1500, 400, obstVel / 6, 0, 1000, 125, 0, backMatPath)

};

ObstPair obstArr[3] =
{
ObstPair(STARTING_DIST, 250, GAP_SIZE, obstVel),
ObstPair(STARTING_DIST + DIST_BETWEEN, 250, GAP_SIZE, obstVel),
ObstPair(STARTING_DIST + DIST_BETWEEN * 2, 250, GAP_SIZE, obstVel)
};



// For counting score by keeping track of the next obstacle to be passed
static void scoreManager(ObstPair* obsts, int &curObst, float xToReach, sf::Sound &scoreSound, sf::Text &scoreT)
{
    if (obsts[curObst].getCenterX() + 50 <= xToReach) {
        score++;
        score % 10 ? scoreSound.play() : score2Sound.play();
        scoreT.setString(std::to_string(score));
        
        // Loop back to the beginning (since the obstacles will cycle
        if (curObst == NUM_OBST - 1) {
            curObst = 0;
            return;
        }

        curObst++;
    }

}

// For moving the obstacles and looping them around to the other end.
static void obstManager(ObstPair* obstArr, int len, double deltaTime, int distBetween)
{
    for (int i = 0; i < len; i++)
    {
        obstArr[i].move(deltaTime);
        if (obstArr[i].getCenterX() + 50 <= 0)
        {
            obstArr[i].setCenter(obstArr[(i + 2) % 3].getCenterX() + distBetween, rand() % (MAX_OBST_Y - MIN_OBST_Y) + MIN_OBST_Y); // Move and set the gap

        }
    }
}

static void borderManager(SpriteEntity* grounds, int len, double deltaTime)
{
    for (int i = 0; i < len; i++)
    {
        if (grounds[i].getRight() <= 0) {
            grounds[i].setPosX(grounds[(i + 1) % len].getRight() + 500);

        }
        grounds[i].move(deltaTime);
    }

}

// Draws the entities involved in the active game
static void drawGameElements(SpriteEntity& player, sf::RenderWindow& window, sf::Text &score)
{

    for (int i = 0; i < NUM_GRND; i++) {
        window.draw(backArr1[i].getSprite());
        window.draw(backArr2[i].getSprite());
    }

    window.draw(player.getSprite());
    for (int i = 0; i < NUM_OBST; i++) {
        window.draw(obstArr[i].getTopSprite());
        window.draw(obstArr[i].getBotSprite());
    }

    for (int i = 0; i < NUM_GRND; i++) {
        window.draw(groundArr[i].getSprite());
        window.draw(roofArr[i].getSprite());
    }
    window.draw(score);

}

// Sets the angle relative to the velocity. Since the player is not actually moving,
// it uses the velocity relative to the obstacles.
static void SetPlayerAngle(SpriteEntity& ent, float vx) {
    float vy = ent.getVelY();

    if (vx == 0)
    {
        ent.setAngle(abs(vy) / vy * 90);
    }


    float angle = std::atan(vy / -vx);
    angle *= 180 / PI;
    ent.setAngle(-angle);
}

// Controls the player's position
static void playerManager(SpriteEntity& player, int gravity, double deltaTime)
{
    SetPlayerAngle(player, -obstVel);
    player.setVelY(player.getVelY() + gravity*deltaTime);
    player.move(deltaTime);

    if (player.getBottom() > GROUND_Y)
    {
        player.setPosY(float(GROUND_Y - player.getHeight() / 2));
        player.setVelY(0);
        return;
    }

    if (player.getTop() < ROOF_Y)
    {
        player.setPosY(ROOF_Y + player.getHeight() / 2);
        player.setVelY(0);
        return;
    }
}


// Returns true if the player is colliding with another SpriteEntity
static bool checkPlayerCollision(SpriteEntity& player, ObstPair* obstArr, int obstL, SpriteEntity* groundArr, SpriteEntity* roofArr, int grndL) {

    for (int i = 0; i < obstL; i++) {
        if (obstArr[i].checkTopCollision(player) || obstArr[i].checkBotCollision(player)) { return true; }
    }

    for (int i = 0; i < grndL; i++) {
        if (player.checkCollision(groundArr[i]) || player.checkCollision(roofArr[i])) { return true; }
    }

    return false;
}

// Runs the game
static void doGameStuffs(sf::RenderWindow &window) {

    // RESTART CONFIG
	double deltaTime = 1.0 / 60;
	int gravity = 800;
	int curObst = 0;
	score = 0;
    scoreT.setString("-");

	int framesSinceJump = 15;
    int framesSinceCollision = 120;

	bool canJump = true;
	bool playing = true;
	bool waitingForStart = true;
    bool waitingToRestart = false;

    player.setTexture(plyrMatPath);
    player.setPos(STARTING_X, STARTING_Y);
    player.setAngle(0);

    roofArr[0].setPosX(500);
    roofArr[1].setPosX(1500);
    groundArr[0].setPosX(500);
    groundArr[1].setPosX(1500);

    obstArr[0].setCenter(STARTING_DIST, 250);
    obstArr[1].setCenter(STARTING_DIST + DIST_BETWEEN, 250);
    obstArr[2].setCenter(STARTING_DIST + DIST_BETWEEN * 2, 250);
    


	while (playing)
	{

		sf::Event event;
        // Handling inputs and window closing
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				return;
			}
			if (event.type == sf::Event::KeyPressed && canJump)
			{
				player.setVelY(jumpVel);
                if (waitingForStart) { scoreT.setString("0"); }
				waitingForStart = false;
				framesSinceJump = 0;
                jumpSound.play();

			}
		}

        // State we should be in while we wait for the starting input
		if (waitingForStart)
		{
            window.draw(controlT);
        }
        // State we should be in after colliding and waiting for the game to reset
        else if (waitingToRestart)
        {
            framesSinceCollision++;

            playerManager(player, gravity, deltaTime);
            obstManager(obstArr, NUM_OBST, deltaTime, DIST_BETWEEN);


            if (framesSinceCollision == framesBeforeReset)
            {
                waitingToRestart = false;
                waitingForStart = true;
                playing = false;
            }
        }
        // The game is running here.
		else
		{
			if (checkPlayerCollision(player, obstArr, NUM_OBST, groundArr, roofArr, NUM_GRND))
			{

				waitingToRestart = true;
                canJump = false;
                deltaTime /= 3; // Slow down time after colliding
                framesSinceCollision = 0;
                player.setTexture(deadMatPath);
                deadSound.play();

			}

            playerManager(player, gravity, deltaTime);
			scoreManager(obstArr, curObst, player.getLeft(), scoreSound, scoreT);
			obstManager(obstArr, NUM_OBST, deltaTime, DIST_BETWEEN);

			framesSinceJump++;
			if (!waitingToRestart && framesSinceJump >= framesBetweenJump) { canJump = true; }
		}

        borderManager(groundArr, NUM_GRND, deltaTime);
        borderManager(roofArr, NUM_GRND, deltaTime);
        borderManager(backArr1, NUM_GRND, deltaTime);
        borderManager(backArr2, NUM_GRND, deltaTime);
        // Draw everything in frame
        drawGameElements(player, window, scoreT);

        window.display();
        window.clear();
    } 

}


int main()
{
    int scrw = 1000;
    int scrh = 500;
    sf::RenderWindow window(sf::VideoMode(scrw, scrh), "Hoppy Box");
    window.setFramerateLimit(60);



    // Sound initialization
    if (
        !jumpBuffer.loadFromFile(jumpPath) ||
        !deadBuffer.loadFromFile(deadPath) ||
        !scoreBuffer.loadFromFile(scorePath) ||
        !score2Buffer.loadFromFile(score2Path)
        )
    {
        exit(EXIT_FAILURE);
    }

    jumpSound.setBuffer(jumpBuffer);
    deadSound.setBuffer(deadBuffer);
    scoreSound.setBuffer(scoreBuffer);
    score2Sound.setBuffer(score2Buffer);

    // Text initialization
    if (!font.loadFromFile(fontPath))
    {
        exit(EXIT_FAILURE);
    }
    scoreT.setFont(font);
    scoreT.setCharacterSize(36);
    scoreT.setString("-");
    scoreT.setPosition(950, 5);
    scoreT.setOutlineColor(sf::Color(50, 50, 50, 255));
    scoreT.setOutlineThickness(2);

    controlT.setFont(font);
    controlT.setCharacterSize(18);
    controlT.setString("Press any key to jump");
    controlT.setPosition(STARTING_X - 90, STARTING_Y + 30);



    while (window.isOpen()) {
        doGameStuffs(window);
    }
 
    return 0;
}