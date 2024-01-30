#ifndef ENTITIES_H
#define ENTITIES_H


class SpriteEntity {

private:
    float vx;
    float vy;
    int w;
    int h;
    float a;
    std::string path;
    sf::Texture texture;
    sf::Sprite sprite;


public:

    // Constructors

    // Returns a SpriteEntity of default values
    inline SpriteEntity();

    // Returns a SpriteEntity
    inline SpriteEntity(float X, float Y, float velX, float velY, int width, int height, float angle, const std::string& texturePath);


    // Getters

    // Returns the X position of the center of the sprite
    float getPosX() const { return sprite.getPosition().x; }

    // Returns the Y position of the center of the sprite
    float getPosY() const { return sprite.getPosition().y; }

    // Returns the X velocity
    float getVelX() const { return vx; }

    // Returns the Y velocity
    float getVelY() const { return vy; }

    // Returns the width of the sprite
    int getWidth() const { return w; }

    // Returns the height of the sprite
    int getHeight() const { return h; }

    // Returns the angle of the sprite
    float getAngle() const { return sprite.getRotation(); }

    // Returns the path of the texture used for the sprite
    std::string getTexturePath() const { return path; }

    // Returns the Texture associated to the sprite
    sf::Texture getTexture() const { return texture; }

    // Returns the sprite of the SpriteEntity
    sf::Sprite getSprite() const { return sprite; }

    // Returns the Y coordinate of the top of the sprite
    float getTop() const { return getPosY() - h / 2; }

    // Returns the Y coordinate of the bottom of the sprite
    float getBottom() const { return getPosY() + h / 2; }

    // Returns the X coordinate of the left of the sprite
    float getLeft() const { return getPosX() - w / 2; }

    // Returns the X coordinate of the right of the sprite
    float getRight() const { return getPosX() + w / 2; }

    // Setters 

    // Sets the X position of the sprite
    void setPosX(float x) { sprite.setPosition(x, sprite.getPosition().y); }

    // Sets the Y position of the sprite
    void setPosY(float y) { sprite.setPosition(sprite.getPosition().x, y); }

    // Sets both the X and Y positions of the sprite
    void setPos(float x, float y) { sprite.setPosition(x, y); }

    // Sets the X velocity
    void setVelX(float v) { vx = v; }
    // Sets the Y velocity
    void setVelY(float v) { vy = v; }

    // Sets the angle of the sprite
    void setAngle(float a) { sprite.setRotation(a); }

    // Sets the texture of the sprite
    void setTexture(std::string newPath);

    // Other

    // Makes the sprite move according to its velocities over a change in time
    void move(double deltaTime);

    // Returns true if the given SpriteEntity intersects this one
    bool checkCollision(const SpriteEntity& ent) const;

};

class ObstPair {
private:
    float centerPosX;
    float centerPosY;
    float gapSize;
    
    SpriteEntity topObst;
    SpriteEntity botObst;

public:
    ObstPair();
    ObstPair(float centerPosX, float centerPosY, float gapSize, float velX);

    // Moves the obstacle pair according to its velocity and the change in time
    void move(double deltaTime);

    // Sets the centerpoint of the obstacle pair
    void setCenter(float newPosX, float newPosY);

    // Sets the gap between the obstacles
    void setGap(float newGap);

    // Sets the X velocity
    void setVelX(float velX);

    // Returns the X coordinate of the center of the obstacles
    float getCenterX() const { return centerPosX; }

    // Returns the Y coordinate of the center of the obstacles
    float getCenterY() const { return centerPosY; }

    // Returns the Sprite of the top obstacle
    sf::Sprite getTopSprite() const { return topObst.getSprite(); }

    // Returns the Sprite of the bottom obstacle
    sf::Sprite getBotSprite() const { return botObst.getSprite(); }

    // Check for a collision between the top SpriteEntity and the given SpriteEntity
    bool checkTopCollision(const SpriteEntity& ent) { return topObst.checkCollision(ent); }

    // Check for a collision between the bottom SpriteEntity and the given SpriteEntity
    bool checkBotCollision(const SpriteEntity& ent) { return botObst.checkCollision(ent); }
};

#endif