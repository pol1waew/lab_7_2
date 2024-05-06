#include "candy.h"

Texture candy::texture;

candy::candy(candy* upperNeighbour, Vector2f location, float scale) {
	this->upperNeighbour = upperNeighbour;
	this->location = location;
	this->baseScale = scale;
	
	if (upperNeighbour != NULL) upperNeighbour->setLowerNeighbour(this);
	type = rand() % 8;

	sprite.setTexture(texture);
	sprite.setTextureRect(IntRect(this->type * baseSpriteSize, 0, baseSpriteSize, baseSpriteSize));
	sprite.setOrigin(baseSpriteSize / 2, baseSpriteSize / 2);
	sprite.setScale(this->baseScale, this->baseScale);
}

void candy::onCollapse() {
	if (upperNeighbour != NULL) {
		if (lowerNeighbour != NULL) lowerNeighbour->setUpperNeighbour(upperNeighbour);
		upperNeighbour->setLowerNeighbour(lowerNeighbour);
		setLowerNeighbour(getTopCandy());
		lowerNeighbour->setUpperNeighbour(this);
		upperNeighbour = NULL;
	}
	sprite.setScale(baseScale, baseScale);
	setState(candyState::fixed);
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
	texture.loadFromFile("resources/candySpriteList.png"); // commit 1
}

void candy::draw(RenderWindow* window) {
	sprite.setPosition(location.x + (baseSpriteSize / 2) * baseScale, location.y + (baseSpriteSize / 2) * baseScale);

	window->draw(sprite);
}

bool candy::isContainPoint(Vector2i point) {
	return point.x >= location.x && point.x < location.x + getScaledSpriteSize() && point.y >= location.y && point.y < location.y + getScaledSpriteSize();
}

candy* candy::getTopCandy() {
	if (upperNeighbour == NULL) return this;
	else						return upperNeighbour->getTopCandy();
}

void candy::setLowerNeighbour(candy* neighbour) { lowerNeighbour = neighbour; }

void candy::setUpperNeighbour(candy* neighbour) { upperNeighbour = neighbour; }

void candy::setState(candyState state) { 
	this->state = state; 
	if (this->state == candyState::fixed) {
		sprite.setTextureRect(IntRect(this->type * baseSpriteSize, 0, baseSpriteSize, baseSpriteSize));
		sprite.setRotation(0);
		isRightRotating = true;
	}
	else if (this->state == candyState::shaking) {
		sprite.setTextureRect(IntRect(this->type * baseSpriteSize, 32, baseSpriteSize, baseSpriteSize));
	}
}

unsigned short int candy::getType() { return type; }

candyState candy::getState() { return state; }

Vector2f candy::getLocation() { return location; }

unsigned short int candy::getScaledSpriteSize() { return baseSpriteSize * sprite.getScale().x; }