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

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	PhysBody* planet_1;
	PhysBody* planet_2;
	PhysBody* planet_3;
	PhysBody* planet_4;
	PhysBody* planet_5;
	PhysBody* planet_6;
	PhysBody* satelite;
	PhysBody* alien;
	PhysBody* lflipper;
	PhysBody* rflipper;
	PhysBody* uflipper;
	PhysBody* u_flipper_joint;
	PhysBody* l_flipper_joint;
	PhysBody* r_flipper_joint;
	PhysBody* sensor;
	PhysBody* planet_1_sensor;
	PhysBody* planet_2_sensor;
	PhysBody* planet_3_sensor;
	PhysBody* planet_4_sensor;
	PhysBody* planet_5_sensor;
	PhysBody* planet_6_sensor;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* background;
	SDL_Texture* leftFlipper;
	SDL_Texture* rightFlipper;
	SDL_Texture* upperFlipper;
	SDL_Texture* planet_1_shine;
	SDL_Texture* planet_2_shine;
	SDL_Texture* planet_5_shine;
	SDL_Texture* planet_6_shine;
	

	
	uint kickerfx;
	uint flipperfx;
	uint startfx;
	uint ringfx;
	uint dingfx;
	uint satelitefx;
	uint alienfx;

	p2Point<int> ray;
	
	bool ray_on;
	bool lightPlanet1;
	bool lightPlanet2;
	bool lightPlanet3;
	bool lightPlanet4;
	bool lightPlanet5;
	bool lightPlanet6;

	int font_score = -1;
	
	

};
