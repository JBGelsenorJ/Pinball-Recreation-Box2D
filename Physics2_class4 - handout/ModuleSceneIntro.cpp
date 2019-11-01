#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleFonts.h"

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

	b2RevoluteJointDef lflipperdef;
	b2RevoluteJointDef rflipperdef;
	b2RevoluteJointDef uflipperdef;
	b2RevoluteJoint* lflipper_joint;
	b2RevoluteJoint* rflipper_joint;
	b2RevoluteJoint* uflipper_joint;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	//Loading textures
	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	background = App->textures->Load("assets/pinball_board_empty.png");
	leftFlipper = App->textures->Load("assets/left_flipper.png");
	rightFlipper = App->textures->Load("assets/right_flipper.png");
	upperFlipper = App->textures->Load("assets/upper_flipper.png");


	startfx = App->audio->LoadFx("assets/audio/start.wav");
	flipperfx = App->audio->LoadFx("assets/audio/flipper.wav");
	App->audio->PlayFx(startfx);

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	//Loading fonts
	font_score = App->fonts->Load("assets/score.png", "0123456789", 1);
	
	//scene elements
	planet_1 = new PhysBody();
	planet_1 = App->physics->CreateCircle(302, 297, 32, b2_staticBody, 0.8f);
	planet_1_sensor = App->physics->CreateCircle(302, 297, 35, b2_staticBody, 2.0f, true);
	planet_1_sensor->listener = this;

	planet_2 = new PhysBody();
	planet_2 = App->physics->CreateCircle(245, 373, 12, b2_staticBody, 0.8f);
	planet_2_sensor = App->physics->CreateCircle(245, 373, 15, b2_staticBody, 2.0f, true);
	planet_2_sensor->listener = this;

	planet_3 = new PhysBody();
	planet_3 = App->physics->CreateCircle(276, 435, 12, b2_staticBody, 0.8f);
	planet_3_sensor = App->physics->CreateCircle(276, 435, 15, b2_staticBody, 2.0f, true);
	planet_3_sensor->listener = this;

	planet_4 = new PhysBody();
	planet_4 = App->physics->CreateCircle(215, 465, 12, b2_staticBody, 0.8f);
	planet_4_sensor = App->physics->CreateCircle(215, 465, 15, b2_staticBody, 2.0f, true);
	planet_4_sensor->listener = this;

	planet_5 = new PhysBody();
	planet_5 = App->physics->CreateCircle(344, 516, 31, b2_staticBody, 0.8f);
	planet_5_sensor = App->physics->CreateCircle(344, 516, 34, b2_staticBody, 2.0f, true);
	planet_5_sensor->listener = this;

	planet_6 = new PhysBody();
	planet_6 = App->physics->CreateCircle(93, 624, 33, b2_staticBody, 0.8f);
	planet_6_sensor = App->physics->CreateCircle(93, 624, 36, b2_staticBody, 2.0f, true);
	planet_6_sensor->listener = this;

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


	App->physics->CreateChain(0, 1, top_block, 72, b2_staticBody, 0.5);

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

	App->physics->CreateChain(109, 676, left_block_two, 50, b2_staticBody, 0.5);

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

	App->physics->CreateChain(337, 674, right_block_two, 50, b2_staticBody, 0.5);

	int pinball_board_top[98] = {
	16, 338,
	17, 82,
	498, 83,
	498, 239,
	492, 214,
	485, 193,
	474, 174,
	462, 158,
	449, 145,
	435, 134,
	419, 125,
	404, 119,
	382, 112,
	358, 108,
	334, 107,
	311, 107,
	290, 108,
	268, 110,
	243, 113,
	216, 119,
	187, 126,
	160, 135,
	134, 148,
	111, 163,
	88, 186,
	76, 206,
	92, 225,
	92, 230,
	84, 243,
	74, 257,
	66, 273,
	65, 295,
	70, 311,
	81, 328,
	91, 342,
	102, 357,
	109, 373,
	111, 393,
	110, 414,
	102, 433,
	99, 442,
	99, 458,
	103, 471,
	108, 482,
	109, 489,
	104, 496,
	76, 504,
	31, 505,
	17, 484
	};

	App->physics->CreateChain(0, 0, pinball_board_top, 98, b2_staticBody, 0.5);

	int pinball_board_bottom_left[74] = {
	192, 856,
	30, 855,
	23, 849,
	18, 835,
	15, 566,
	20, 561,
	31, 553,
	50, 544,
	68, 539,
	73, 540,
	74, 545,
	67, 551,
	55, 560,
	42, 570,
	32, 582,
	25, 594,
	23, 609,
	23, 631,
	27, 639,
	31, 655,
	35, 672,
	38, 687,
	41, 700,
	44, 713,
	49, 728,
	56, 741,
	62, 752,
	75, 766,
	93, 781,
	111, 791,
	129, 798,
	153, 808,
	170, 817,
	185, 830,
	198, 843,
	202, 850,
	202, 854
	};

	App->physics->CreateChain(0, 0, pinball_board_bottom_left, 74, b2_staticBody, 0.5);

	int pinball_board_bottom_right[60] = {
	453, 857,
	316, 856,
	313, 853,
	316, 847,
	324, 838,
	339, 823,
	355, 813,
	374, 804,
	394, 796,
	410, 790,
	419, 785,
	421, 771,
	425, 759,
	433, 746,
	439, 733,
	439, 714,
	436, 698,
	426, 662,
	422, 645,
	421, 632,
	423, 620,
	427, 612,
	434, 607,
	433, 596,
	435, 582,
	441, 569,
	449, 559,
	457, 553,
	464, 551,
	464, 857
	};


	App->physics->CreateChain(0, 0, pinball_board_bottom_right, 60, b2_staticBody, 0.5);


	int tube[58] = {
	463, 551,
	463, 448,
	455, 439,
	445, 427,
	438, 411,
	432, 394,
	428, 375,
	425, 354,
	424, 333,
	426, 322,
	431, 306,
	430, 292,
	423, 281,
	414, 268,
	409, 258,
	408, 240,
	411, 224,
	421, 216,
	434, 216,
	446, 222,
	455, 228,
	455, 399,
	468, 448,
	468, 737,
	497, 738,
	495, 221,
	499, 240,
	499, 763,
	463, 763
	};

	App->physics->CreateChain(0, 0, tube, 58, b2_staticBody, 0.0f);

	int wooden_planks[26]{
	110, 481,
	148, 520,
	129, 539,
	118, 529,
	105, 541,
	95, 531,
	80, 544,
	42, 505,
	69, 504,
	92, 501,
	101, 497,
	106, 493,
	110, 490

	};
	App->physics->CreateChain(0, 0, wooden_planks, 26, b2_staticBody, 0.5);

	int pinball_board_middle_right[60] = {
	373, 444,
	374, 437,
	380, 432,
	385, 430,
	401, 430,
	407, 433,
	414, 442,
	426, 464,
	432, 481,
	435, 498,
	435, 512,
	431, 524,
	426, 537,
	419, 549,
	411, 560,
	404, 568,
	400, 570,
	395, 569,
	394, 565,
	397, 555,
	401, 546,
	404, 537,
	408, 524,
	410, 508,
	409, 499,
	406, 492,
	399, 480,
	390, 466,
	381, 454,
	376, 447
	};

	App->physics->CreateChain(0, 0, pinball_board_middle_right, 60, b2_staticBody, 0.5f);
	


	rflipper = App->physics->CreateRectangle(297, 773, 69, 14, b2_dynamicBody);
	lflipper = App->physics->CreateRectangle(210, 776, 69, 14, b2_dynamicBody);
	uflipper = App->physics->CreateRectangle(395, 324, 50, 14, b2_dynamicBody);

	l_flipper_joint = App->physics->CreateCircle(182, 775, 5, b2_staticBody, 0.5f);
	r_flipper_joint = App->physics->CreateCircle(332, 775, 5, b2_staticBody, 0.5f);
	u_flipper_joint = App->physics->CreateCircle(416, 326, 5, b2_staticBody, 0.5f);

	lflipperdef.Initialize(lflipper->body, l_flipper_joint->body, l_flipper_joint->body->GetWorldCenter());
	rflipperdef.Initialize(r_flipper_joint->body, rflipper->body, r_flipper_joint->body->GetWorldCenter());
	uflipperdef.Initialize(u_flipper_joint->body, uflipper->body, u_flipper_joint->body->GetWorldCenter());

	lflipperdef.enableLimit = true;
	rflipperdef.enableLimit = true;
	uflipperdef.enableLimit = true;

	lflipperdef.lowerAngle = -30 * DEGTORAD;
	lflipperdef.upperAngle = 30 * DEGTORAD;
	rflipperdef.lowerAngle = -30 * DEGTORAD;
	rflipperdef.upperAngle = 30 * DEGTORAD;
	uflipperdef.lowerAngle = -30 * DEGTORAD;
	uflipperdef.upperAngle = 30 * DEGTORAD;

	lflipper_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&lflipperdef);
	rflipper_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&rflipperdef);
	uflipper_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&uflipperdef);

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
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25, b2_dynamicBody, 0.6f));
		circles.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50, b2_dynamicBody));
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

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64, b2_dynamicBody, 1.0f));
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

	if (lflipper != NULL)
	{
		int x, y;
		lflipper->GetPosition(x, y);
		App->renderer->Blit(leftFlipper, x, y, NULL, 1.0f, lflipper->GetRotation());
	}

	if (rflipper != NULL)
	{
		int x, y;
		rflipper->GetPosition(x, y);
		App->renderer->Blit(rightFlipper, x, y, NULL, 1.0f, rflipper->GetRotation());
	}

	if (uflipper != NULL)
	{
		int x, y;
		uflipper->GetPosition(x, y);
		App->renderer->Blit(upperFlipper, x, y, NULL, 1.0f, uflipper->GetRotation());
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

	

	
	//Score
	char score_text[10];
	sprintf_s(score_text, 10, "%d", App->player->score);
	App->fonts->BlitText(250, 43, font_score, score_text);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (App->player->getPoints == false && (bodyA == planet_1_sensor || bodyA == planet_2_sensor || bodyA == planet_3_sensor || bodyA == planet_4_sensor || bodyA == planet_5_sensor || bodyA == planet_6_sensor))
	{
		App->player->getPoints = true;
	}
	
}
