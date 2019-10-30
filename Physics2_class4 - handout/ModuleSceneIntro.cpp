#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	//Loading textures
	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	background = App->textures->Load("assets/pinball_board1.png");

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	
	//scene elements
	planet_1 = new PhysBody();
	planet_1 = App->physics->CreateCircle(302, 297, 33, b2_staticBody);
	
	planet_2 = new PhysBody();
	planet_2 = App->physics->CreateCircle(245, 373, 13, b2_staticBody);

	planet_3 = new PhysBody();
	planet_3 = App->physics->CreateCircle(276, 435, 13, b2_staticBody);

	planet_4 = new PhysBody();
	planet_4 = App->physics->CreateCircle(215, 465, 13, b2_staticBody);

	planet_5 = new PhysBody();
	planet_5 = App->physics->CreateCircle(344, 516, 32, b2_staticBody);

	planet_6 = new PhysBody();
	planet_6 = App->physics->CreateCircle(93, 624, 34, b2_staticBody);

	int top_block[72] = {
	259, 217,
	243, 224,
	227, 234,
	214, 244,
	200, 254,
	186, 263,
	173, 269,
	160, 272,
	152, 271,
	146, 267,
	142, 259,
	143, 251,
	148, 244,
	157, 235,
	168, 225,
	182, 212,
	196, 202,
	211, 192,
	227, 182,
	244, 174,
	258, 170,
	278, 169,
	296, 172,
	312, 177,
	328, 182,
	342, 188,
	347, 193,
	349, 201,
	345, 206,
	337, 211,
	325, 216,
	310, 219,
	297, 220,
	284, 218,
	274, 216,
	266, 215
	};


	App->physics->CreateChain( 0, 1, top_block, 72, b2_staticBody);

	int right_block[54] = {
	65, 231,
	68, 47,
	64, 34,
	56, 22,
	48, 14,
	38, 7,
	26, 3,
	16, 3,
	8, 9,
	4, 19,
	3, 40,
	6, 51,
	10, 59,
	17, 66,
	22, 74,
	26, 84,
	26, 93,
	22, 103,
	20, 115,
	20, 141,
	23, 161,
	26, 178,
	32, 196,
	38, 209,
	45, 220,
	52, 228,
	58, 233
	};

	App->physics->CreateChain(405, 213, right_block, 54, b2_staticBody);

	int left_block_two[50] = {
	17, 85,
	10, 79,
	4, 66,
	2, 50,
	2, 28,
	3, 19,
	5, 11,
	10, 3,
	15, 3,
	18, 5,
	20, 10,
	22, 18,
	25, 28,
	29, 40,
	34, 51,
	40, 61,
	47, 72,
	56, 81,
	62, 86,
	65, 93,
	64, 98,
	59, 100,
	47, 100,
	33, 96,
	23, 90
	};

	App->physics->CreateChain(109, 675, left_block_two, 50, b2_staticBody);

	int right_block_two[50] = {
	57, 3,
	51, 3,
	48, 7,
	45, 15,
	43, 23,
	40, 33,
	37, 42,
	31, 55,
	25, 64,
	18, 74,
	10, 83,
	5, 87,
	2, 93,
	3, 98,
	8, 100,
	15, 100,
	27, 98,
	41, 91,
	53, 83,
	60, 74,
	64, 59,
	65, 48,
	64, 27,
	63, 16,
	60, 8
	};

	App->physics->CreateChain(337, 675, right_block_two, 50, b2_staticBody);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{

	App->renderer->Blit(background, 0, 0, NULL);

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25, b2_dynamicBody));
		circles.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
	
	int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64, b2_dynamicBody));
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
