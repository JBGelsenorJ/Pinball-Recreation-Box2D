#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2List.h"
#include "p2Point.h"
#include "Box2D/Box2D/Box2D.h"

#define PIXELS_PER_METER 50.0f 
#define METER_PER_PIXEL 0.02f

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

private:
	void setLeftFlipper();
	void setRightFlipper();
	void setUpFlipper();

public:
	
	SDL_Texture* ball_texture;
	SDL_Texture* right_board;
	SDL_Texture* right_block;
	SDL_Texture* left_flipper;
	SDL_Texture* right_flipper;
	SDL_Texture* upper_flipper;

	PhysBody* ball = nullptr;;
	PhysBody* ballSensor = nullptr;;
	PhysBody* Restart = nullptr;;
	PhysBody* lFlipper = nullptr;;
	PhysBody* lFlipperPivot = nullptr;;
	PhysBody* rFlipper = nullptr;;
	PhysBody* rFlipperPivot = nullptr;;
	PhysBody* uFlipper = nullptr;;
	PhysBody* uFlipperPivot = nullptr;;

	b2RevoluteJoint* lFlipperJoint;
	b2RevoluteJoint* rFlipperJoint;
	b2RevoluteJoint* uFlipperJoint;

	bool shoot;
	bool restart;
	bool getPoints;
	bool getBonus;
	bool extralife;

	uint force_counter = 0;
	uint score = 0;
	uint lives = 0;
	uint recentScore = 0;
	uint bestScore = 0;
};