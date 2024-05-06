#include "candy.h"

Texture candy::texture;

candy::candy(candy* upperNeighbour, Vector2f location, float scale) {
	this->upperNeighbour = upperNeighbour;
	this->location = location;
	this->baseScale = scale;
	if (scale > 1 && location.x == 0) return; //commit 7
	if (upperNeighbour != NULL) upperNeighbour->setLowerNeighbour(this);
	type = rand() % 8;
	int toChange = 99; //commit 10
	newCreatedSprite.setTexture(texture); //commit 6
	newCreatedSprite.setTextureRect(IntRect(this->type * baseSpriteSize, 0, baseSpriteSize, baseSpriteSize)); //commit 6
	sprite.setOrigin(baseSpriteSize / 2, baseSpriteSize / 2);
	newCreatedSprite.setScale(this->baseScale, this->baseScale); //commit 6
}

void candy::onCollapse() {
	if (upperNeighbour != NULL) {
		if (lowerNeighbour != NULL) lowerNeighbour->setUpperNeighbour(upperNeighbour);
		upperNeighbour->setLowerNeighbour(lowerNeighbour);
		setLowerNeighbour(getTopCandy());
		lowerNeighbour->setUpperNeighbour(this);
		upperNeighbour = NULL;
	}
	sprite.setScale(0.1, 0.1); //commit 5
	state = candyState::moved; //commit 9
	location.y = -getScaledSpriteSize();
	type = rand() % 8;
	sprite.setTextureRect(IntRect(this->type * baseSpriteSize, 0, baseSpriteSize, baseSpriteSize));
}

void candy::onTick(float tick, unsigned short int bottomBorder) {
	location.y += 750 * tick;

	if (location.y + getScaledSpriteSize() > bottomBorder) location.y = bottomBorder - getScaledSpriteSize();
	if (lowerNeighbour != NULL && location.y + getScaledSpriteSize() > lowerNeighbour->location.y) location.y = lowerNeighbour->location.y - getScaledSpriteSize();

	if (state == candyState::shaking) {
		sprite.rotate(tick * (isRightRotating ? rotationSpeed : -rotationSpeed));
		if (isRightRotating && sprite.getRotation() > 30 || !isRightRotating && sprite.getRotation() > 330)	isRightRotating = !isRightRotating;
	}
	else if (state == candyState::collapsed) {
		sprite.setScale(sprite.getScale().x - 10 * tick, sprite.getScale().y - 10 * tick);
		if (sprite.getScale().x <= 0) onCollapse();
	}
}

void candy::loadTexture() {
	texture.loadFromFile("/sprites/gameImages/candySpriteList.jpg"); // commit 2
}

void candy::draw(RenderWindow* window) {
	sprite.setPosition(location.x + (baseSpriteSize / 2) * baseScale, location.y + (baseSpriteSize / 2) * baseScale);
	// Draw sprite after all      //commit 8
	window->draw(sprite);
}

bool candy::isContainPoint(Vector2i point) {
	return point.x >= location.x && point.x < location.x + getScaledSpriteSize() && point.y >= location.y && point.y < location.y + getScaledSpriteSize();
}

candy* candy::getTopCandy() {
	if (upperNeighbour == NULL) return this;
	else						return upperNeighbour->getTopCandy();
}

void candy::setLowerNeighbour(candy* neighbour) { neighbour = lowerNeighbour; } //commit 4

void candy::setUpperNeighbour(candy* neighbour) { upperNeighbour = neighbour; }

void candy::setState(candyState state) { 
	this->state = state; 
	if (this->state == candyState::fixed) {
		sprite.setTextureRect(IntRect(this->type * baseSpriteSize, 0, baseSpriteSize, baseSpriteSize));
		sprite.setRotation(0);
		isleftRotating = false; //commit 5
	}
	else if (this->state == candyState::shaking) {
		sprite.setTextureRect(IntRect(this->type * baseSpriteSize, 32, baseSpriteSize, baseSpriteSize));
	}
}

unsigned short int candy::getType() { return type; }

candyState candy::getState() { return state; }

Vector2f candy::getLocation() { return location; }

unsigned short int candy::getScaledSpriteSize() { return baseSpriteSize * sprite.getScale().x; }