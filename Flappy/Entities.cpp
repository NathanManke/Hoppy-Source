#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "Entities.h"
const std::string obstMatPath = "assets/textures/obstacle.png";

inline SpriteEntity::SpriteEntity()
    : vx(0), vy(0),
    w(50), h(50), a(0), path("assets/textures/obstacle.png")
{
    if (!texture.loadFromFile(path)) 
    {
        exit(EXIT_FAILURE);
    }

    texture.setRepeated(true);
    sprite.setOrigin(float(w / 2), float(h / 2));
    sprite.setPosition(0, 0);
    sprite.setRotation(a);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, w, h));
}

inline SpriteEntity::SpriteEntity(float X, float Y, float velX, float velY, int width, int height, float angle, const std::string &texturePath) :
    vx(velX), vy(velY),
    w(width), h(height), a(angle), path(texturePath)
{
    if (!texture.loadFromFile(path)) {
        exit(EXIT_FAILURE);
    }

    texture.setRepeated(true);
    sprite.setOrigin(float(w / 2), float(h / 2));
    sprite.setPosition(X, Y);
    sprite.setRotation(a);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, w, h));
}


inline void SpriteEntity::move(double deltaTime)
{
    setPosX(float(getPosX() + vx * deltaTime));
    setPosY(float(getPosY() + vy * deltaTime));
};


inline bool SpriteEntity::checkCollision(const SpriteEntity& ent) const
{
    return getSprite().getGlobalBounds().intersects(ent.getSprite().getGlobalBounds());
}

inline void SpriteEntity::setTexture(std::string newPath) {
    path = newPath;
    texture.loadFromFile(path);
    sprite.setTexture(texture);
}





inline ObstPair::ObstPair() :
    centerPosX(500), centerPosY(250), gapSize(0),
    topObst(centerPosX, centerPosY - gapSize / 2 - 250, 0, 0, 100, 500, 180, obstMatPath),
    botObst(centerPosX, centerPosY + gapSize / 2 + 250, 0, 0, 100, 500, 0, obstMatPath) 
{}

inline ObstPair::ObstPair(float centerX, float centerY, float gap, float velX) :
    centerPosX(centerX), centerPosY(centerY), gapSize(gap),
    topObst(centerPosX, centerPosY - gapSize / 2 - 250, velX, 0, 100, 500, 180, obstMatPath),
    botObst(centerPosX, centerPosY + gapSize / 2 + 250, velX, 0, 100, 500, 0, obstMatPath)
{}

inline void ObstPair::move(double deltaTime)
{
    topObst.move(deltaTime);
    botObst.move(deltaTime);
    centerPosX = topObst.getPosX();
    centerPosY = topObst.getPosY() + gapSize / 2 + 250;
}

inline void ObstPair::setCenter(float newPosX, float newPosY)
{
    centerPosX = newPosX;
    centerPosY = newPosY;
    topObst.setPos(newPosX, centerPosY - gapSize / 2 - 250);
    botObst.setPos(newPosX, centerPosY + gapSize / 2 + 250);
}

inline void ObstPair::setGap(float newGap) 
{
    gapSize = newGap;
    topObst.setPosY(centerPosY - gapSize / 2 - 250);
    botObst.setPosY(centerPosY + gapSize / 2 + 250);
}

inline void ObstPair::setVelX(float velX)
{
    topObst.setVelX(velX);
    botObst.setVelX(velX);
}

