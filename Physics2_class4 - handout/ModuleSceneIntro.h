#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void MapChecker();
	void CreateElements();

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	PhysBody* planet_1 = nullptr;
	PhysBody* planet_2 = nullptr;
	PhysBody* planet_3 = nullptr;
	PhysBody* planet_4 = nullptr;
	PhysBody* planet_5 = nullptr;
	PhysBody* planet_6 = nullptr;
	PhysBody* satelite = nullptr;
	PhysBody* kickercloser = nullptr;

	PhysBody* kickercloser_sensor = nullptr;
	PhysBody* woodensensor = nullptr;
	PhysBody* sensor = nullptr;
	PhysBody* planet_1_sensor = nullptr;
	PhysBody* planet_2_sensor = nullptr;
	PhysBody* planet_3_sensor = nullptr;
	PhysBody* planet_4_sensor = nullptr;
	PhysBody* planet_5_sensor = nullptr;
	PhysBody* planet_6_sensor = nullptr;
	PhysBody* satelite_sensor = nullptr;
	PhysBody* alienSensor = nullptr;
	PhysBody* kickerSensor = nullptr;
	PhysBody* miniPlanetSensor = nullptr;
	PhysBody* miniPlanetSensor2 = nullptr;
	PhysBody* miniPlanetSensor3 = nullptr;
	PhysBody* redPlanetSensor1 = nullptr;
	PhysBody* redPlanetSensor2 = nullptr;
	PhysBody* redPlanetSensor3 = nullptr;
	PhysBody* greendot = nullptr;
	PhysBody* greendot2 = nullptr;
	PhysBody* greendot3 = nullptr;
	PhysBody* greendot4 = nullptr;
	PhysBody* greendot5 = nullptr;
	PhysBody* greendot6 = nullptr;
	PhysBody* greendot7 = nullptr;
	PhysBody* greendot8 = nullptr;
	PhysBody* greendot9 = nullptr;
	PhysBody* greendot10 = nullptr;
	PhysBody* greendot11 = nullptr;
	PhysBody* greenBigSensor1 = nullptr;
	PhysBody* greenBigSensor2 = nullptr;
	PhysBody* greenBigSensor3 = nullptr;
	PhysBody* yellowstar = nullptr;
	PhysBody* yellowstar2 = nullptr;
	PhysBody* yellowstar3 = nullptr;
	PhysBody* yellowstar4 = nullptr;
	PhysBody* yellowstar5 = nullptr;
	PhysBody* yellowstar6 = nullptr;
	PhysBody* yellowstar7 = nullptr;

	bool sensed = false;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* background;
	SDL_Texture* planet_1_shine;
	SDL_Texture* planet_2_shine;
	SDL_Texture* planet_5_shine;
	SDL_Texture* planet_6_shine;
	SDL_Texture* satellite;
	SDL_Texture* alien_texture;
	SDL_Texture* miniplanet_texture;
	SDL_Texture* red_planet_texture;
	SDL_Texture* green_light_texture1;
	SDL_Texture* green_light_texture2;
	SDL_Texture* green_light_texture3;
	SDL_Texture* green_dot_texture;
	SDL_Texture* yellowstar_texture;
	
	uint kickerfx = 0;
	uint flipperfx = 0;
	uint startfx = 0;
	uint ringfx = 0;
	uint dingfx = 0;
	uint satelitefx = 0;
	uint alienfx = 0;
	uint woodeninfx = 0;
	uint woodenoutfx = 0;
	uint miniplanetfx = 0;
	uint pointfx = 0;
	uint yellowstarfx = 0;
	uint bonusfx = 0;
	uint greenlightfx = 0;
	uint redplanetfx = 0;

	p2Point<int> ray;
	
	bool ray_on;
	bool lightPlanet1;
	bool lightPlanet2;
	bool lightPlanet3;
	bool lightPlanet4;
	bool lightPlanet5;
	bool lightPlanet6;
	bool lightSatellite;
	bool lightAlien;
	bool closekicker;
	bool woodentransport;
	bool lightMiniWhitePlanet;
	bool lightMiniWhitePlanet2;
	bool lightMiniWhitePlanet3;
	bool lightRedPlanet1;
	bool lightRedPlanet2;
	bool lightRedPlanet3;
	bool lightgreendot;
	bool lightgreendot2;
	bool lightgreendot3;
	bool lightgreendot4;
	bool lightgreendot5;
	bool lightgreendot6;
	bool lightgreendot7;
	bool lightgreendot8;
	bool lightgreendot9;
	bool lightgreendot10;
	bool lightgreendot11;
	bool lightGreenBig1;
	bool lightGreenBig2;
	bool lightGreenBig3;
	bool lightyellowstar;
	bool lightyellowstar2;
	bool lightyellowstar3;
	bool lightyellowstar4;
	bool lightyellowstar5;
	bool lightyellowstar6;
	bool lightyellowstar7;

	int font_score = -1;
	uint move = 0;
	uint cont = 0;

};
