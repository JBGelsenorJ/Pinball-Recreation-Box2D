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
	
	//Bodies
	ball = App->physics->CreateCircle(480, 700, 11, b2_dynamicBody, 0.6f);
	
	ballSensor = App->physics->CreateRectangleSensor(472 + 10, 741 + 5, 25, 21);
	ballSensor->listener = this;


	Restart = App->physics->CreateRectangleSensor(240, 870, 480, 10);
	Restart->listener = this;

	//Initialize counters
	force_counter = 0;


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
		

		if (shoot == true)
		{
			float force = ((float)force_counter * 25) / 3;
			ball->body->ApplyForce({ 0,-force }, ball->body->GetLocalCenter(), true);
			shoot = false;
		}

		force_counter = 0;
		
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->scene_intro->leftflipper->body->ApplyAngularImpulse(-2.0f, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->scene_intro->rightflipper->body->ApplyAngularImpulse(2.0f, true);
	}
	//Restarting game
	if (restart == true)
	{
		ball->body->SetTransform({ PIXEL_TO_METERS(480 + 0.2f), PIXEL_TO_METERS(700 - 0.2f) }, 0.0f);
		ball->body->SetLinearVelocity({ 0,0 });
		
		restart = false;
	}

	//Blit everything
	int x, y;
	ball->GetPosition(x, y);

	App->renderer->Blit(ball_texture, x, y);

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



