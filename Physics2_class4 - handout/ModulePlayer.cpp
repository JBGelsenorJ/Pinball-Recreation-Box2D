#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	//Loading Textures
	ball_texture = App->textures->Load("assets/ball.png");
	right_board = App->textures->Load("assets/right_board.png");
	right_block = App->textures->Load("assets/right_block.png");
	left_flipper = App->textures->Load("assets/left_flipper.png");
	right_flipper = App->textures->Load("assets/right_flipper.png");
	
	//bools
	getPoints = false;

	//Creating ball
	ball = App->physics->CreateCircle(480, 700, 11, b2_dynamicBody, 0.2f);
	
	ballSensor = App->physics->CreateRectangleSensor(472 + 10, 741 + 5, 25, 21);
	ballSensor->listener = this;

	//Creating restart sensor
	Restart = App->physics->CreateRectangleSensor(240, 870, 480, 10);
	Restart->listener = this;

	//Initialize counters
	force_counter = 0;
	score = 0;
	lives = 3;
	recentScore = 0;
	bestScore = 0;

	//Setting Flippers
	setLeftFlipper();
	setRightFlipper();

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()

{

	//Pushing the ball
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		force_counter += 1;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		
		App->audio->PlayFx(App->scene_intro->kickerfx);
		if (shoot == true)
		{
			float force = ((float)force_counter * 25) / 3;
			ball->body->ApplyForce({ 0,-force }, ball->body->GetLocalCenter(), true);
			shoot = false;
		}

		force_counter = 0;
		
	}

	//Moving Flippers
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		lFlipper->body->ApplyAngularImpulse(-2.0f, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		rFlipper->body->ApplyAngularImpulse(2.0f, true);
		//App->scene_intro->uflipper->body->ApplyAngularImpulse(2.0f, true);
		
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		App->audio->PlayFx(App->scene_intro->flipperfx);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		App->audio->PlayFx(App->scene_intro->flipperfx);
	}

	//Restarting game
	if (restart == true)
	{
		App->audio->PlayFx(App->scene_intro->startfx);
		App->audio->PlayFx(App->scene_intro->ringfx);
		ball->body->SetTransform({ PIXEL_TO_METERS(480 + 0.2f), PIXEL_TO_METERS(700 - 0.2f) }, 0.0f);
		ball->body->SetLinearVelocity({ 0,0 });

		

		if (lives > 0)
		{
			lives -= 1;
		}

		if (lives == 0)
		{
			lives = 3;
			recentScore = score;

			if(recentScore > bestScore)
			{
				bestScore = recentScore;
			}
			
			score = 0;
			
		}

		restart = false;
	}

	//Get Points
	if (getPoints == true)
	{
		score += 3;
		getPoints = false;
	}

	//All draw fuction

	//Draw ball
	int x, y;
	ball->GetPosition(x, y);

	App->renderer->Blit(ball_texture, x, y);
	App->renderer->Blit(right_board, 313, 550, NULL);
	App->renderer->Blit(right_block, 405, 212, NULL);
	
	//Draw flippers
	if (lFlipper != NULL)
	{
		int x, y;
		lFlipper->GetPosition(x, y);
		App->renderer->Blit(left_flipper, x, y, NULL, 1.0f, lFlipper->GetRotation());
	}

	if (rFlipper != NULL)
	{
		int x, y;
		rFlipper->GetPosition(x, y);
		App->renderer->Blit(right_flipper, x, y, NULL, 1.0f, rFlipper->GetRotation());
	}

	


	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA->body->GetFixtureList()->IsSensor())
	{
		if (bodyA == ballSensor && App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
		{
			shoot = true;
		}
		if (bodyA == Restart)
		{
			restart = true;
		}
		
	}
		
}

void ModulePlayer::setLeftFlipper() {

	lFlipper = App->physics->CreateRectangle(185, 782, 72, 18, b2_dynamicBody);
	lFlipperPivot = App->physics->CreateCircle(185, 782, 4, b2_staticBody, 0.0f);

	b2RevoluteJointDef revoluteJointDef;

	revoluteJointDef.bodyA = lFlipper->body;
	revoluteJointDef.bodyB = lFlipperPivot->body;
	lFlipper->body->SetGravityScale(10.0f);


	revoluteJointDef.localAnchorA.Set(PIXEL_TO_METERS(-30), 0);
	revoluteJointDef.localAnchorB.Set(0, 0);
	revoluteJointDef.collideConnected = false;

	revoluteJointDef.enableLimit = true;
	revoluteJointDef.upperAngle = 35 * DEGTORAD;
	revoluteJointDef.lowerAngle = -30 * DEGTORAD;

	revoluteJointDef.motorSpeed = 2000.0f * DEGTORAD;
	revoluteJointDef.maxMotorTorque = 1500.0f;
	revoluteJointDef.enableMotor = false;

	lFlipperJoint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&revoluteJointDef);

}

void ModulePlayer::setRightFlipper() {

	rFlipper = App->physics->CreateRectangle(330, 782, 72, 18, b2_dynamicBody);
	rFlipperPivot = App->physics->CreateCircle(330, 782, 4, b2_staticBody, 0.0f);

	b2RevoluteJointDef revoluteJointDef;

	revoluteJointDef.bodyA = rFlipper->body;
	revoluteJointDef.bodyB = rFlipperPivot->body;

	rFlipper->body->SetGravityScale(10.0f);


	revoluteJointDef.localAnchorA.Set(PIXEL_TO_METERS(30), 0);
	revoluteJointDef.localAnchorB.Set(0, 0);
	revoluteJointDef.collideConnected = false;

	revoluteJointDef.enableLimit = true;
	revoluteJointDef.upperAngle = 35 * DEGTORAD;
	revoluteJointDef.lowerAngle = -30 * DEGTORAD;

	revoluteJointDef.motorSpeed = -2000.0f * DEGTORAD;
	revoluteJointDef.maxMotorTorque = 1500.0f;
	revoluteJointDef.enableMotor = false;

	rFlipperJoint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&revoluteJointDef);
}




