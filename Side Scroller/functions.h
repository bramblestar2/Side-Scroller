#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <fstream>
#include "Block.h"

using namespace std;
using namespace sf;


Texture blankTexture(int, int, Color);

bool collisionCheck(Sprite&, Block&);

void movement(Sprite&, double&, double&);

float lerp(float, float, float);

void scale(Sprite&, double, double);

vector<Block*> getMap();

bool animatePlayer(double vX, double vY, Texture& playerTex, Sprite& player);

void editor(RenderWindow&, vector<Block*>&);

void deathState(Sprite&, Texture&, bool&);