#include<iostream>
#include<vector>
#include<random>
#include<conio.h>

int ENVIRONMENT_TEMP = 30;
int EMPTY = 0;
int MAP_SIZE = 10;

class Resource {
public:
	int posX;
	int posY;


	Resource(int x, int y) {
		this->posX = x;
		this->posY = y;
	}
};

class Food {

public:
	int water;
	int nutrients;
	int protein;
	int fiber;
	int posX;
	int posY;

	Food(int water, int nutrients, int protein, int fiber) {
		this->water = water;
		this->nutrients = nutrients;
		this->protein = protein;
		this->fiber = fiber;
	}

	void setPos(int x, int y) {
		this->posX = x;
		this->posY = y;
	}

};

class Player {
	public:
	int hunger;
	int thirst;
	int health;
	bool isAlive = true;
	int nutrient;
	int protein;
	int fiber;
	int posX;
	int posY;

	Player() {
		thirst = 60;
		health = 100;
		nutrient = 20;
		protein = 20;
		fiber = 20;
		hunger = 40;
	}

	void EatFood(Food f) {
		if (this->nutrient + f.nutrients <= 100) {
			this->nutrient += f.nutrients;
		}
		else {
			this->nutrient = 100;
		}

		if (this->protein + f.protein <= 100) {
			this->protein += f.protein;
		}
		else {
			this->protein = 100;
		}

		if (this->fiber + f.fiber <= 100) {
			this->fiber += f.fiber;
		}
		else {
			this->fiber = 100;
		}

		if (this->hunger + (this->protein + this->fiber + this->nutrient) / 3 <= 100) {
			this->hunger += (this->protein + this->fiber + this->nutrient) / 3;
		}
		else {
			this->hunger = 100;
		}
		this->thirst += f.water;
	}

	void reduceHunger(int amt) {
		if (hunger - amt > 0) hunger -= amt;
		else isAlive = false;
	}
};

class Tile {
public:
	bool hasFood = false;
	bool hasResource = false;
	int foodIndex;
	int resourceIndex;
};

class TilePos {
	
	int x, y;

	public:
		TilePos(int x, int y) {
			this->x = x;
			this->y = y;
		}

		int getX() { return x; }
		int getY() {return y;}
};

void showMap(std::vector<std::vector<Tile>> map, Player p) {
	for (int i = 0;i<map.size();i++) {
		for (int j = 0;j < map[i].size();j++) {

			if (i == p.posX && j == p.posY) { 
				std::cout << " P "; 
			}
			else if (map[i][j].hasFood) {
				std::cout << " f ";
			}
			else if (map[i][j].hasResource) {
				std::cout << " r ";
			}
			else{
				std::cout << " . ";
			}
		}
		std::cout << std::endl;
	}

	std::cout << "Health: " << p.health << std::endl;
	std::cout << "Hunger: " << p.hunger << std::endl;
	std::cout << "Alive: " << p.isAlive << std::endl;
}

void showGameOver() {
	std::cout << "Game Over" << std::endl;
}

Food generateFood() {

	int tempNum = rand();

	int water = (tempNum / 100)%10;
	int nutrients = (tempNum / 200) % 10;
		int protein = (tempNum / 300) % 10;
		int fiber = (tempNum / 400) % 10;

	Food fTemp(water, nutrients, protein, fiber);

	return fTemp;
}


void generateMap(std::vector<std::vector<Tile>>& map, std::vector<Food>& foods, std::vector<Resource>& resources, std::vector<TilePos>& freeTiles) {
	for (int i = 0;i < MAP_SIZE;i++) {
		
		std::vector<Tile> temp;

		for (int j = 0;j < MAP_SIZE;j++) {
			Tile t;

			int r = rand() % 10;
			if (r >= 0 && r <= 1) {
				t.hasFood = true;
				Food f = generateFood();

				f.setPos(i, j);
				foods.push_back(f);
				t.foodIndex = foods.size() - 1;
			}

			else if (r > 1 && r < 6) {
				t.hasResource = true;
				Resource r(i, j);
				resources.push_back(r);
				t.resourceIndex = resources.size() - 1;
			}
			else {
				TilePos t(i, j);
				freeTiles.push_back(t);
			}
			
			temp.push_back(t);
		}

		map.push_back(temp);
	}
}

void spawnPlayer(std::vector<std::vector<Tile>>& map, std::vector<TilePos>& freeTiles, Player& p) {
	int rPos = rand() % freeTiles.size();

	int playerPosX = freeTiles[rPos].getX();
	int playerPosY = freeTiles[rPos].getY();

	p.posX = playerPosX;
	p.posY = playerPosY;
}

void movePlayer(std::vector<std::vector<Tile>>& map, Player& p, std::vector<Food>& foods, std::vector<Resource>& resources, char input ) {
	/*std::mt19937 dirrng(std::random_device{}());
	std::uniform_int_distribution<int> dirDist(0, 3);*/
	int dir;

	if (input == 'w' || input == 'W') dir = 0;
	else if (input == 'd' || input == 'D') dir = 1;
	else if (input == 's' || input == 'S') dir = 2;
	else if (input == 'a' || input == 'A') dir = 3;

	// I'll use this later for distance
	/*std::mt19937 distrng(std::random_device{}());
	std::uniform_int_distribution<int> distDist(0, 1);
	int dis = distDist(distrng);*/

	int newX = p.posX;
	int newY = p.posY;

	if (dir == 0 && p.posX >0) {
		newX = p.posX - 1;
		newY = p.posY;
	}
	else if (dir == 1 && p.posY < MAP_SIZE -1) {
		newX = p.posX;
		newY = p.posY+1;
	}
	else if (dir == 2 && p.posX < MAP_SIZE -1) {
		newX = p.posX+1;
		newY = p.posY;
	}
	else if(dir == 3 && p.posY >0){
		newX = p.posX;
		newY = p.posY - 1;
	}

	if (map[newX][newY].hasFood) {
		p.posX = newX;
		p.posY = newY;
		p.reduceHunger(1);

		Food f = foods[map[newX][newY].foodIndex];

		p.EatFood(f);
		//foods.erase(foods.begin() + map[newX][newY].foodIndex); // Just need to update index after food erasing :)
		map[newX][newY].hasFood = false;
	}
	else if (map[newX][newY].hasResource) {
		p.posX = newX;
		p.posY = newY;
		p.reduceHunger(5);

		//resources.erase(resources.begin() + map[newX][newY].resourceIndex);
		map[newX][newY].hasResource = false;
	}
	else {
		p.posX = newX;
		p.posY = newY;
		p.reduceHunger(1);
	}

}

int main() {

	bool gameOver = false;

	Player p1;
	Player p2;
	Player p3;

	std::vector<std::vector<Tile>> map;
	std::vector<Food> foods;
	std::vector<Resource> resources;

	// Free Tiles
	std::vector<TilePos> freeTiles;

	generateMap(map, foods, resources, freeTiles);
	spawnPlayer(map,freeTiles,p1);

	while (!gameOver) {
		system("cls");
		if (p1.isAlive) showMap(map, p1);
		else showGameOver();

		char input = _getch();

		movePlayer(map, p1, foods, resources, input);
	}
}