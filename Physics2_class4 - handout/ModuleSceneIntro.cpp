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
#include "ModuleWindow.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
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
	background = App->textures->Load("assets/pinball_board_definitivo.png");
	planet_1_shine = App->textures->Load("assets/planet1.png");
	planet_2_shine = App->textures->Load("assets/planet2.png");
	planet_5_shine = App->textures->Load("assets/planet5.png");
	planet_6_shine = App->textures->Load("assets/planet6.png");
	satellite = App->textures->Load("assets/satellite.png");
	alien_texture = App->textures->Load("assets/alien.png");
	miniplanet_texture = App->textures->Load("assets/whitelight.png");
	red_planet_texture = App->textures->Load("assets/redlight.png");
	green_dot_texture = App->textures->Load("assets/greendot.png");
	green_light_texture1 = App->textures->Load("assets/greenbiglight1.png");
	green_light_texture2 = App->textures->Load("assets/greenbiglight2.png");
	green_light_texture3 = App->textures->Load("assets/greenbiglight3.png");
	yellowstar_texture = App->textures->Load("assets/yellowstar.png");

	//Loading FX
	kickerfx = App->audio->LoadFx("assets/audio/start.wav");
	flipperfx = App->audio->LoadFx("assets/audio/flipper.wav");
	startfx = App->audio->LoadFx("assets/audio/noisestart.wav");
	App->audio->PlayFx(startfx);
	ringfx = App->audio->LoadFx("assets/audio/ring.wav");
	dingfx = App->audio->LoadFx("assets/audio/ding.wav");
	satelitefx = App->audio->LoadFx("assets/audio/satelite.wav");
	alienfx = App->audio->LoadFx("assets/audio/alien.wav");
	woodeninfx = App->audio->LoadFx("assets/audio/boost.wav");
	woodenoutfx = App->audio->LoadFx("assets/audio/lasershot.wav");
	miniplanetfx = App->audio->LoadFx("assets/audio/dum.wav");
	pointfx = App->audio->LoadFx("assets/audio/beeppoint.wav");
	yellowstarfx = App->audio->LoadFx("assets/audio/smalllight.wav");
	bonusfx = App->audio->LoadFx("assets/audio/bonus.wav");
	greenlightfx = App->audio->LoadFx("assets/audio/dinghigh.wav");
	redplanetfx = App->audio->LoadFx("assets/audio/dinglow.wav");
	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	//Loading fonts
	font_score = App->fonts->Load("assets/score.png", "0123456789", 1);
	
	//scene elements
	CreateElements();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	//Unloading all textures just in case
	App->textures->Unload(background);
	App->textures->Unload(planet_1_shine);
	App->textures->Unload(planet_2_shine);
	App->textures->Unload(planet_5_shine);
	App->textures->Unload(planet_6_shine);
	App->textures->Unload(satellite);
	App->textures->Unload(alien_texture);
	App->textures->Unload(miniplanet_texture);
	App->textures->Unload(red_planet_texture);
	App->textures->Unload(green_light_texture1);
	App->textures->Unload(green_light_texture2);
	App->textures->Unload(green_light_texture3);
	App->textures->Unload(green_dot_texture);
	App->textures->Unload(yellowstar_texture);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{

	App->renderer->Blit(background, 0, 0, NULL);

	//Mouse and Raycast
	MouseandRaycast();

	// Function to check all colisions with sensors 
	MapChecker();

	//Score
	Score();

	//Setting Title
	char lives[4];
	char Title[64] = "Pinball in Space | ";
	char numLives[32] = "Lives: ";
	
	sprintf_s(lives, "%d", App->player->lives);
	strcat_s(Title, numLives);
	strcat_s(Title, lives);

	App->window->SetTitle(Title);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (App->player->getPoints == false && (bodyA == planet_1_sensor))
	{
		App->player->getPoints = true;
		lightPlanet1 = true;
	}

	if (App->player->getPoints == false && (bodyA == planet_2_sensor))
	{
		App->player->getPoints = true;
		lightPlanet2 = true;
	}

	if (App->player->getPoints == false && (bodyA == planet_3_sensor))
	{
		App->player->getPoints = true;
		lightPlanet3 = true;
	}

	if (App->player->getPoints == false && (bodyA == planet_4_sensor))
	{
		App->player->getPoints = true;
		lightPlanet4 = true;
	}

	if (App->player->getPoints == false && (bodyA == planet_5_sensor))
	{
		App->player->getPoints = true;
		lightPlanet5 = true;
	}

	if (App->player->getPoints == false && (bodyA == planet_6_sensor))
	{
		App->player->getPoints = true;

		lightPlanet6 = true;
	}

	if ((bodyA == planet_1_sensor) || (bodyA == planet_2_sensor) || (bodyA == planet_3_sensor) || (bodyA == planet_4_sensor) || (bodyA == planet_5_sensor) || (bodyA == planet_6_sensor)) {
		App->audio->PlayFx(dingfx);
	}

	if (App->player->getPoints == false && (bodyA == satelite_sensor))
	{
		App->player->getPoints = true;
		App->audio->PlayFx(satelitefx);
		lightSatellite = true;
	}

	if ((bodyA == alienSensor))
	{
		App->player->getPoints = true;
		App->audio->PlayFx(alienfx);
		lightAlien = true;
	}

	if ((bodyA == woodensensor)) {
		App->audio->PlayFx(woodeninfx);
		woodentransport = true;
	}

	if (bodyA == kickerSensor && closekicker == false)
	{
		closekicker = true;
	}

	if ((bodyA == miniPlanetSensor)) {
		lightMiniWhitePlanet = true;
		App->player->getPoints = true;
		App->audio->PlayFx(miniplanetfx);
	}

	if ((bodyA == miniPlanetSensor2)) {
		lightMiniWhitePlanet2 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(miniplanetfx);
	}

	if ((bodyA == miniPlanetSensor3)) {
		lightMiniWhitePlanet3 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(miniplanetfx);
	}

	if ((bodyA == redPlanetSensor1)) {
		lightRedPlanet1 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(redplanetfx);
	}

	if ((bodyA == redPlanetSensor2)) {
		lightRedPlanet2 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(redplanetfx);
	}

	if ((bodyA == redPlanetSensor3)) {
		lightRedPlanet3 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(redplanetfx);
	}

	if ((bodyA == greendot))
	{
		lightgreendot = true;
		App->audio->PlayFx(pointfx);
		App->player->getPoints = true;
	}
	if ((bodyA == greendot2))
	{
		lightgreendot2 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(pointfx);
	}
	if ((bodyA == greendot3))
	{
		lightgreendot3 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(pointfx);
	}
	if ((bodyA == greendot4))
	{
		lightgreendot4 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(pointfx);
	}
	if ((bodyA == greendot5))
	{
		lightgreendot5 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(pointfx);
	}
	if ((bodyA == greendot6))
	{
		lightgreendot6 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(pointfx);
	}
	if ((bodyA == greendot7))
	{
		lightgreendot7 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(pointfx);
	}
	if ((bodyA == greendot8))
	{
		lightgreendot8 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(pointfx);
	}
	if ((bodyA == greendot9))
	{
		lightgreendot9 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(pointfx);
	}
	if ((bodyA == greendot10))
	{
		lightgreendot10 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(pointfx);
	}
	if ((bodyA == greendot11))
	{
		lightgreendot11 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(pointfx);
	}

	if ((bodyA == greenBigSensor1))
	{
		lightGreenBig1 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(greenlightfx);
	}

	if ((bodyA == greenBigSensor2))
	{
		lightGreenBig2 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(greenlightfx);
	}

	if ((bodyA == greenBigSensor3))
	{
		lightGreenBig3 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(greenlightfx);
	}
	if ((bodyA == yellowstar))
	{
		lightyellowstar = true;
		App->player->getPoints = true;
		App->audio->PlayFx(yellowstarfx);
	}
	if ((bodyA == yellowstar2))
	{
		lightyellowstar2 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(yellowstarfx);
	}
	if ((bodyA == yellowstar3))
	{
		lightyellowstar3 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(yellowstarfx);
	}
	if ((bodyA == yellowstar4))
	{
		lightyellowstar4 = true;
		App->audio->PlayFx(yellowstarfx);
		App->player->getPoints = true;
	}
	if ((bodyA == yellowstar5))
	{
		lightyellowstar5 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(yellowstarfx);
	}
	if ((bodyA == yellowstar6))
	{
		lightyellowstar6 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(yellowstarfx);
	}
	if ((bodyA == yellowstar7))
	{
		lightyellowstar7 = true;
		App->player->getPoints = true;
		App->audio->PlayFx(yellowstarfx);
	}

}





void ModuleSceneIntro::MapChecker()
{

	//Light all planets
	if (lightPlanet1 == true)
	{
		App->renderer->Blit(planet_1_shine, 267, 263, NULL);
	}

	lightPlanet1 = false;

	if (lightPlanet2 == true)
	{
		App->renderer->Blit(planet_2_shine, 230, 358, NULL);
	}

	lightPlanet2 = false;

	if (lightPlanet3 == true)
	{
		App->renderer->Blit(planet_2_shine, 262, 421, NULL);
	}

	lightPlanet3 = false;

	if (lightPlanet4 == true)
	{
		App->renderer->Blit(planet_2_shine, 199, 451, NULL);
	}

	lightPlanet4 = false;

	if (lightPlanet5 == true)
	{
		App->renderer->Blit(planet_5_shine, 307, 480, NULL);
	}

	lightPlanet5 = false;

	if (lightPlanet6 == true)
	{
		App->renderer->Blit(planet_6_shine, 57, 589, NULL);
	}

	lightPlanet6 = false;

	if (lightSatellite == true)
	{
		App->renderer->Blit(satellite, 131, 349, NULL);
	}

	lightSatellite = false;

	if (lightAlien == true)
	{
		App->renderer->Blit(alien_texture, 83, 292, NULL);
	}

	if (closekicker == true)
	{
		closekicker = false;
		/*kickercloser = App->physics->CreateRectangle(449, 205, 10, 100,b2_dynamicBody);*/
	}

	if (woodentransport)
	{
		cont += 10;
		App->player->ball->body->SetTransform({ PIXEL_TO_METERS(1000 + 0.2f), PIXEL_TO_METERS(1000) }, 0.0f);
		App->player->ball->body->SetLinearVelocity({ 0,0 });
		if (cont > 1000)
		{
			App->audio->PlayFx(woodenoutfx);
			woodentransport = false;
			App->player->ball->body->SetTransform({ PIXEL_TO_METERS(399 + 0.2f), PIXEL_TO_METERS(130) }, 0.0f);
			App->player->ball->body->SetLinearVelocity({ 0,40 });
			cont = 0;
			App->player->score += 100;
		}
	}

	lightAlien = false;

	if (lightMiniWhitePlanet == true)
	{
		App->renderer->Blit(miniplanet_texture, 185, 544, NULL);
	}

	if (lightMiniWhitePlanet2 == true)
	{
		App->renderer->Blit(miniplanet_texture, 214, 506, NULL);
	}

	if (lightMiniWhitePlanet3 == true)
	{
		App->renderer->Blit(miniplanet_texture, 161, 583, NULL);
	}

	if (lightRedPlanet1 == true)
	{
		App->renderer->Blit(red_planet_texture, 289, 554, NULL);
	}

	if (lightRedPlanet2 == true)
	{
		App->renderer->Blit(red_planet_texture, 325, 592, NULL);
	}

	if (lightRedPlanet3 == true)
	{
		App->renderer->Blit(red_planet_texture, 356, 635, NULL);
	}

	if (lightGreenBig1 == true)
	{
		App->renderer->Blit(green_light_texture1, 81, 203, NULL);
	}

	lightGreenBig1 = false;

	if (lightGreenBig2 == true)
	{
		App->renderer->Blit(green_light_texture2, 102, 440, NULL);
	}

	lightGreenBig2 = false;

	if (lightGreenBig3 == true)
	{
		App->renderer->Blit(green_light_texture3, 378, 418, NULL);
	}

	lightGreenBig3 = false;

	// Light little dots
	if (lightgreendot == true)

	{
		App->renderer->Blit(green_dot_texture, 420, 130, NULL);
	}
	if (lightgreendot2 == true)
	{
		App->renderer->Blit(green_dot_texture, 382, 116, NULL);
	}

	if (lightgreendot3 == true)
	{
		App->renderer->Blit(green_dot_texture, 345, 109, NULL);
	}

	if (lightgreendot4 == true)
	{
		App->renderer->Blit(green_dot_texture, 299, 109, NULL);
	}

	if (lightgreendot5 == true)
	{
		App->renderer->Blit(green_dot_texture, 257, 119, NULL);
	}

	if (lightgreendot6 == true)
	{
		App->renderer->Blit(green_dot_texture, 214, 134, NULL);
	}

	if (lightgreendot7 == true)
	{
		App->renderer->Blit(green_dot_texture, 178, 151, NULL);
	}

	if (lightgreendot8 == true)
	{
		App->renderer->Blit(green_dot_texture, 148, 175, NULL);
	}

	if (lightgreendot9 == true)
	{
		App->renderer->Blit(green_dot_texture, 123, 204, NULL);
	}

	if (lightgreendot10 == true)
	{
		App->renderer->Blit(green_dot_texture, 100, 233, NULL);
	}

	if (lightgreendot11 == true)
	{
		App->renderer->Blit(green_dot_texture, 81, 263, NULL);
	}

	if (lightyellowstar)
	{
		App->renderer->Blit(yellowstar_texture, 381, 228, NULL);
	}
	lightyellowstar = false;

	if (lightyellowstar2)
	{
		App->renderer->Blit(yellowstar_texture, 365, 268, NULL);
	}
	lightyellowstar2 = false;

	if (lightyellowstar3)
	{
		App->renderer->Blit(yellowstar_texture, 352, 307, NULL);
	}
	lightyellowstar3 = false;

	if (lightyellowstar4)
	{
		App->renderer->Blit(yellowstar_texture, 339, 344, NULL);
	}
	lightyellowstar4 = false;

	if (lightyellowstar5)
	{
		App->renderer->Blit(yellowstar_texture, 328, 382, NULL);
	}
	lightyellowstar5 = false;

	if (lightyellowstar6)
	{
		App->renderer->Blit(yellowstar_texture, 319, 427, NULL);
	}
	lightyellowstar6 = false;

	if (lightyellowstar7)
	{
		App->renderer->Blit(yellowstar_texture, 312, 470, NULL);
	}
	lightyellowstar7 = false;

}

void ModuleSceneIntro::CreateElements(){

	planet_1 = new PhysBody();
	planet_1 = App->physics->CreateCircle(302, 297, 32, b2_staticBody, 0.8f);
	planet_1_sensor = App->physics->CreateCircle(302, 297, 36, b2_staticBody, 2.0f, true);
	planet_1_sensor->listener = this;

	planet_2 = new PhysBody();
	planet_2 = App->physics->CreateCircle(245, 373, 12, b2_staticBody, 0.8f);
	planet_2_sensor = App->physics->CreateCircle(245, 373, 16, b2_staticBody, 2.0f, true);
	planet_2_sensor->listener = this;

	planet_3 = new PhysBody();
	planet_3 = App->physics->CreateCircle(276, 435, 12, b2_staticBody, 0.8f);
	planet_3_sensor = App->physics->CreateCircle(276, 435, 16, b2_staticBody, 2.0f, true);
	planet_3_sensor->listener = this;

	planet_4 = new PhysBody();
	planet_4 = App->physics->CreateCircle(215, 465, 12, b2_staticBody, 0.8f);
	planet_4_sensor = App->physics->CreateCircle(215, 465, 16, b2_staticBody, 2.0f, true);
	planet_4_sensor->listener = this;

	planet_5 = new PhysBody();
	planet_5 = App->physics->CreateCircle(344, 516, 31, b2_staticBody, 0.8f);
	planet_5_sensor = App->physics->CreateCircle(344, 516, 35, b2_staticBody, 2.0f, true);
	planet_5_sensor->listener = this;

	planet_6 = new PhysBody();
	planet_6 = App->physics->CreateCircle(93, 624, 33, b2_staticBody, 0.8f);
	planet_6_sensor = App->physics->CreateCircle(93, 624, 37, b2_staticBody, 2.0f, true);
	planet_6_sensor->listener = this;

	satelite = new PhysBody();
	satelite = App->physics->CreateCircle(155, 365, 8, b2_staticBody, 0.8f);
	satelite_sensor = App->physics->CreateCircle(155, 365, 12, b2_staticBody, 2.0f, true);
	satelite_sensor->listener = this;

	alienSensor = App->physics->CreateCircle(96, 305, 14, b2_staticBody, 2.0f, true);
	alienSensor->listener = this;

	kickerSensor = App->physics->CreateRectangleSensor(439, 205, 10, 100);
	kickerSensor->listener = this;

	miniPlanetSensor = App->physics->CreateCircle(196, 553, 11, b2_staticBody, 2.0f, true);
	miniPlanetSensor->listener = this;

	miniPlanetSensor2 = App->physics->CreateCircle(223, 516, 11, b2_staticBody, 2.0f, true);
	miniPlanetSensor2->listener = this;

	miniPlanetSensor3 = App->physics->CreateCircle(172, 593, 11, b2_staticBody, 2.0f, true);
	miniPlanetSensor3->listener = this;

	redPlanetSensor1 = App->physics->CreateCircle(298, 565, 11, b2_staticBody, 2.0f, true);
	redPlanetSensor1->listener = this;

	redPlanetSensor2 = App->physics->CreateCircle(334, 602, 11, b2_staticBody, 2.0f, true);
	redPlanetSensor2->listener = this;

	redPlanetSensor3 = App->physics->CreateCircle(367, 645, 11, b2_staticBody, 2.0f, true);
	redPlanetSensor3->listener = this;

	// Little green dots
	greendot = App->physics->CreateCircle(425, 136, 2, b2_staticBody, 2.0f, true);
	greendot->listener = this;

	greendot2 = App->physics->CreateCircle(387, 121, 2, b2_staticBody, 2.0f, true);
	greendot2->listener = this;

	greendot3 = App->physics->CreateCircle(350, 113, 2, b2_staticBody, 2.0f, true);
	greendot3->listener = this;

	greendot4 = App->physics->CreateCircle(305, 114, 2, b2_staticBody, 2.0f, true);
	greendot4->listener = this;

	greendot5 = App->physics->CreateCircle(261, 123, 2, b2_staticBody, 2.0f, true);
	greendot5->listener = this;

	greendot6 = App->physics->CreateCircle(218, 137, 2, b2_staticBody, 2.0f, true);
	greendot6->listener = this;

	greendot7 = App->physics->CreateCircle(182, 155, 2, b2_staticBody, 2.0f, true);
	greendot7->listener = this;

	greendot8 = App->physics->CreateCircle(152, 179, 2, b2_staticBody, 2.0f, true);
	greendot8->listener = this;

	greendot9 = App->physics->CreateCircle(127, 208, 2, b2_staticBody, 2.0f, true);
	greendot9->listener = this;

	greendot10 = App->physics->CreateCircle(104, 237, 2, b2_staticBody, 2.0f, true);
	greendot10->listener = this;

	greendot11 = App->physics->CreateCircle(85, 267, 2, b2_staticBody, 2.0f, true);
	greendot11->listener = this;

	greenBigSensor1 = App->physics->CreateCircle(90, 215, 10, b2_staticBody, 2.0f, true);
	greenBigSensor1->listener = this;

	greenBigSensor2 = App->physics->CreateCircle(102, 450, 10, b2_staticBody, 2.0f, true);
	greenBigSensor2->listener = this;

	greenBigSensor3 = App->physics->CreateCircle(388, 427, 10, b2_staticBody, 2.0f, true);
	greenBigSensor3->listener = this;

	yellowstar = App->physics->CreateCircle(384, 232, 4, b2_staticBody, 2.0f, true);
	yellowstar->listener = this;

	yellowstar2 = App->physics->CreateCircle(367, 272, 4, b2_staticBody, 2.0f, true);
	yellowstar2->listener = this;

	yellowstar3 = App->physics->CreateCircle(355, 311, 4, b2_staticBody, 2.0f, true);
	yellowstar3->listener = this;

	yellowstar4 = App->physics->CreateCircle(342, 348, 4, b2_staticBody, 2.0f, true);
	yellowstar4->listener = this;

	yellowstar5 = App->physics->CreateCircle(331, 386, 4, b2_staticBody, 2.0f, true);
	yellowstar5->listener = this;

	yellowstar6 = App->physics->CreateCircle(322, 431, 4, b2_staticBody, 2.0f, true);
	yellowstar6->listener = this;

	yellowstar7 = App->physics->CreateCircle(315, 474, 4, b2_staticBody, 2.0f, true);
	yellowstar7->listener = this;

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


	App->physics->CreateChain(0, 1, top_block, 72, b2_staticBody, 0.5, false);

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

	App->physics->CreateChain(109, 676, left_block_two, 50, b2_staticBody, 0.5, false);

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

	App->physics->CreateChain(337, 674, right_block_two, 50, b2_staticBody, 0.5, false);

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

	App->physics->CreateChain(0, 0, pinball_board_top, 98, b2_staticBody, 0.5, false);

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

	App->physics->CreateChain(0, 0, pinball_board_bottom_left, 74, b2_staticBody, 0.5, false);

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


	App->physics->CreateChain(0, 0, pinball_board_bottom_right, 60, b2_staticBody, 0.5, false);


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

	App->physics->CreateChain(0, 0, tube, 58, b2_staticBody, 0.0f, false);

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
	App->physics->CreateChain(0, 0, wooden_planks, 26, b2_staticBody, 0.5, false);
	woodensensor = App->physics->CreateRectangleSensor(108, 526, 20, 40);
	woodensensor->listener = this;

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

	App->physics->CreateChain(0, 0, pinball_board_middle_right, 60, b2_staticBody, 0.5f, false);

}
void ModuleSceneIntro::Score() {

	char score_text[10];
	char bestScore_text[10];
	char recentScore_text[10];

	sprintf_s(score_text, 10, "%d", App->player->score);
	sprintf_s(bestScore_text, 10, "%d", App->player->bestScore);
	sprintf_s(recentScore_text, 10, "%d", App->player->recentScore);

	App->fonts->BlitText(250, 43, font_score, score_text);
	App->fonts->BlitText(250, 20, font_score, bestScore_text);
	App->fonts->BlitText(420, 43, font_score, recentScore_text);


}
void ModuleSceneIntro::MouseandRaycast() {
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	// Prepare for raycast ------------------------------------------------------

	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// ray -----------------
	if (ray_on == true)
	{
		fVector destination(mouse.x - ray.x, mouse.y - ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if (normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}
}