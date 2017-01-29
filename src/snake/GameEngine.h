#pragma once

#include "Scene.h"
#include "ImputManager.h"

using namespace std::string_literals; // Enables to use an array of characters with an "s" at the end as an string literal
using namespace std;

struct names_score { char name[20]; int score; };
names_score list_players[10];


namespace GameEngine {

	Scene _Scene;
	int *temp_x;
	int *temp_y;
	ImputManager IM;

	// ------------------------------------------------------------------------------ Varies funcions innicials ----------------------------------

	// -------- colisio de les cordenades del player amb les fruites o amb les parets -------
	bool collision_fruit(int player_x, int player_y, SDL_Rect fruit) {
		bool click = false;
		if (player_x == fruit.x && player_y == fruit.y) {
			click = true;
		}
		return click;
	}

	// -------- cresta el temps, i si arriba a 0 no perds una vida, és Game Over -------
	void consol(){
		player.set_time(player.get_time(), 0.2);
		if (player.get_time() <= 0) {
			player.set_m_live(true);
		}
	}

	// -------- transforma un numero en text (int to char) -------
	char *int_to_text(int num) {
		char text[10];
		_itoa_s(num, text, 10);
		return text;
	}


	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------ Game Loop del joc ----------------------------------------------------------
	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Gameloop() {
		// ----------------------------------------------------------- Inicialitza les variables ------------------------------------------------------------
		SDL_Log("Executable built in %s", SDL_GetBasePath());
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) throw "Unable to initialize SDL subsystems"s;
		const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
		if (!(IMG_Init(imgFlags) & imgFlags)) throw "Unable to initialize SDL_image"s;
		const Uint8 mixFlags{ MIX_INIT_MP3 | MIX_INIT_OGG };
		if (!(Mix_Init(mixFlags) & mixFlags)) throw "Unable to initialize SDL_mixer"s;
		if (TTF_Init() != 0) throw "Unable to initialize SDL_ttf"s;

		// --- -------------------------------------Direction ----------------
		direction = Player::DIR::DOWN;

		// ---------------------------------------- WINDOW -------------------
		const int WIDTH{ 1000 }, HEIGHT{ 750 };
		SDL_Window *window{ SDL_CreateWindow("Hello SDL",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			WIDTH, HEIGHT,
			SDL_WINDOW_SHOWN) };
		if (window == nullptr) throw "Unable to initialize the SDL_Window"s;

		// --- -------------------------------------RENDERER ---------------------
		SDL_Renderer *renderer{ SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };
		if (renderer == nullptr) throw "Unable to initialize the SDL_Renderer"s;

		// ---------------------------------------- SPRITES ----------------------

		SDL_Texture *bgTexture{ IMG_LoadTexture(renderer, "../../res/gfx/back_menu.jpg") };
		SDL_Texture *playerTexture{ IMG_LoadTexture(renderer, "../../res/gfx/blue.png") };
		SDL_Texture *fruitTexture{ IMG_LoadTexture(renderer, "../../res/gfx/apple.png") };

		SDL_Texture *up{ IMG_LoadTexture(renderer, "../../res/gfx/up.png") };
		SDL_Texture *down{ IMG_LoadTexture(renderer, "../../res/gfx/down.png") };
		SDL_Texture *left{ IMG_LoadTexture(renderer, "../../res/gfx/left.png") };
		SDL_Texture *right{ IMG_LoadTexture(renderer, "../../res/gfx/right.png") };
		SDL_Texture *cuerpo_up_down{ IMG_LoadTexture(renderer, "../../res/gfx/cuerpo_up_down.png") };
		SDL_Texture *cuerpo_left_right{ IMG_LoadTexture(renderer, "../../res/gfx/cuerpo.png") };

		SDL_Texture *curva1{ IMG_LoadTexture(renderer, "../../res/gfx/1.png") };
		SDL_Texture *curva2{ IMG_LoadTexture(renderer, "../../res/gfx/2.png") };
		SDL_Texture *curva3{ IMG_LoadTexture(renderer, "../../res/gfx/3.png") };
		SDL_Texture *curva4{ IMG_LoadTexture(renderer, "../../res/gfx/4.png") };

		SDL_Rect bgRect{ 0, 0, WIDTH, HEIGHT };
		SDL_Rect playerRect{ 0, 0, 25, 25 };
		SDL_Rect playerTarget{ 0, 0, 25, 25 };
		SDL_Rect fruitRect{ 0, 0, 25, 25 };
		SDL_Rect time_box{ 100, 5, 0, 15 };
		SDL_Rect pared{ 0, 0, 25, 25 };

		// ---------------------------------------------- TEXT --------------------------------------

		TTF_Font *font{ TTF_OpenFont("../../res/fnt/arial.ttf", 50) };

			//---------------------------------------Play text
			SDL_Surface *play_surface{ TTF_RenderText_Blended(font, "PLAY", SDL_Color{ 255, 0, 255, 255 }) };
			SDL_Texture *play_texture{ SDL_CreateTextureFromSurface(renderer, play_surface) };
			SDL_Rect play_text{ static_cast<int>(WIDTH*.15f),
				static_cast<int>(HEIGHT*.85f),
				play_surface->w, play_surface->h };
			SDL_FreeSurface(play_surface);

			//---------------------------------------Exit text
			SDL_Surface *exit_surface{ TTF_RenderText_Blended(font, "EXIT", SDL_Color{ 255, 0, 255, 255 }) };
			SDL_Texture *exit_texture{ SDL_CreateTextureFromSurface(renderer, exit_surface) };
			SDL_Rect exit_text{ static_cast<int>(WIDTH*.75f),
				static_cast<int>(HEIGHT*.85f),
				exit_surface->w, exit_surface->h };
			SDL_FreeSurface(exit_surface);

			//--------------------------------------Rancking text
			SDL_Surface *ranking_surface{ TTF_RenderText_Blended(font, "RANKING", SDL_Color{ 255, 0, 255, 255 }) };
			SDL_Texture *ranking_texture{ SDL_CreateTextureFromSurface(renderer, ranking_surface) };
			SDL_Rect ranking_text{ static_cast<int>(WIDTH*.40f),
				static_cast<int>(HEIGHT*.85f),
				ranking_surface->w, ranking_surface->h };
			SDL_FreeSurface(ranking_surface);

			//--------------------------------------Easy text
			SDL_Surface *easy_surface{ TTF_RenderText_Blended(font, "EASY", SDL_Color{ 255, 0, 255, 255 }) };
			SDL_Texture *easy_texture{ SDL_CreateTextureFromSurface(renderer, easy_surface) };
			SDL_Rect easy_text{ static_cast<int>(WIDTH*.50f),
				static_cast<int>(HEIGHT*.15f),
				easy_surface->w, easy_surface->h };
			SDL_FreeSurface(easy_surface);

			//--------------------------------------Medium text
			SDL_Surface *medium_surface{ TTF_RenderText_Blended(font, "MEDIUM", SDL_Color{ 255, 0, 255, 255 }) };
			SDL_Texture *medium_texture{ SDL_CreateTextureFromSurface(renderer, medium_surface) };
			SDL_Rect medium_text{ static_cast<int>(WIDTH*.50f),
				static_cast<int>(HEIGHT*.40f),
				medium_surface->w, medium_surface->h };
			SDL_FreeSurface(medium_surface);

			//-------------------------------------Hard text
			SDL_Surface *hard_surface{ TTF_RenderText_Blended(font, "HARD", SDL_Color{ 255, 0, 255, 255 }) };
			SDL_Texture *hard_texture{ SDL_CreateTextureFromSurface(renderer, hard_surface) };
			SDL_Rect hard_text{ static_cast<int>(WIDTH*.50f),
				static_cast<int>(HEIGHT*.75f),
				hard_surface->w, hard_surface->h };
			SDL_FreeSurface(hard_surface);

		TTF_CloseFont(font);

		
		// -------------------------------------------------------- AUDIO--------------------------- 
		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) throw "Unable to initialize SDL_mixer audio systems"s;
		Mix_Music *soundtrack{ Mix_LoadMUS("../../res/sfx/song.mp3") };
		if (!soundtrack) throw "Unable to load the Mix_Music soundtrack"s;
		Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
		Mix_PlayMusic(soundtrack, -1);
		

		_Scene = Scene::MENU;

		ifstream score;
		score.open("ranking.bin", std::ios::binary);
		if (score.is_open()) {
			score.read((char*)(names_score*)list_players, sizeof(list_players));
		}
		score.close();

		// --------------------------------------------------------------------------------- GAME LOOP -----------------------------------------------------------------------------------------------
		SDL_Event evnt;
		while (_Scene != Scene::EXIT) {
			// H---------------------------------------------ANDLE EVENTS
			//------------------------------------------------------------------------------------------ MENU ------------------------------------------------------------------------------

			if (_Scene == Scene::MENU) {
				while (SDL_PollEvent(&evnt)) {
					switch (evnt.type) {
					case SDL_MOUSEMOTION:	playerTarget.x = evnt.motion.x; playerTarget.y = evnt.motion.y; break;
					case SDL_MOUSEBUTTONDOWN:
						if (IM.Click_button(evnt, play_text) == true) {
							_Scene = Scene::LEVEL;
						}
						else if (IM.Click_button(evnt, exit_text) == true) {
							_Scene = Scene::EXIT;
						}
						else if (IM.Click_button(evnt, ranking_text) == true) {
							_Scene = Scene::RANKING;
						}

					default:;
					}
				}
				// ------------------------------------------------------UPDATE
				playerRect.x += (playerTarget.x - playerRect.x) / 10;
				playerRect.y += (playerTarget.y - playerRect.y) / 10;
				// ------------------------------------------------------DRAW
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, bgTexture, nullptr, &bgRect);
				SDL_RenderCopy(renderer, playerTexture, nullptr, &playerRect);
				SDL_RenderCopy(renderer, play_texture, nullptr, &play_text);
				SDL_RenderCopy(renderer, exit_texture, nullptr, &exit_text);
				SDL_RenderCopy(renderer, ranking_texture, nullptr, &ranking_text);

				SDL_RenderPresent(renderer);

			}

			//------------------------------------------------------------------------------------------ RANKING ------------------------------------------------------------------------------

			else if (_Scene == Scene::RANKING) {
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, bgTexture, nullptr, &bgRect);
				ifstream score;
				TTF_Font *font{ TTF_OpenFont("../../res/fnt/arial.ttf", 30) };
				score.open("ranking.bin", std::ios::binary);
				if (score.is_open()) {
					score.read((char*)(names_score*)list_players, sizeof(list_players));
				}
				score.close();

				int Y_p = 100;
				for (int p = 0; p < 10; p++) {
					if (*list_players[p].name == '\0') {
					}
					else {
						
						SDL_Surface *name_surface{ TTF_RenderText_Blended(font, list_players[p].name, SDL_Color{ 255, 255, 255, 255 }) };
						SDL_Texture *name_texture{ SDL_CreateTextureFromSurface(renderer, name_surface) };
						SDL_Rect name_text{ static_cast<int>(400),
							static_cast<int>(Y_p),
							name_surface->w, name_surface->h };
						SDL_FreeSurface(name_surface);

						SDL_RenderCopy(renderer, name_texture, nullptr, &name_text);
					}


					SDL_Surface *score_surface{ TTF_RenderText_Blended(font, int_to_text(list_players[p].score), SDL_Color{ 255, 255, 255, 255 }) };
					SDL_Texture *score_texture{ SDL_CreateTextureFromSurface(renderer, score_surface) };
					SDL_Rect score_text{ static_cast<int>(500),
						static_cast<int>(Y_p),
						score_surface->w, score_surface->h };
					SDL_FreeSurface(score_surface);

					SDL_RenderCopy(renderer, score_texture, nullptr, &score_text);

					Y_p = Y_p + 50;
				}
				TTF_CloseFont(font);

				while (SDL_PollEvent(&evnt)) {
					switch (evnt.type) {
					case SDL_MOUSEMOTION:	playerTarget.x = evnt.motion.x; playerTarget.y = evnt.motion.y; break;
					case SDL_MOUSEBUTTONDOWN:
						if (IM.Click_button(evnt, play_text) == true) {
							_Scene = Scene::LEVEL;
						}
						else if (IM.Click_button(evnt, exit_text) == true) {
							_Scene = Scene::EXIT;
						}

					default:;
					}
				}
				// --------------------------------------UPDATE
				playerRect.x += (playerTarget.x - playerRect.x) / 10;
				playerRect.y += (playerTarget.y - playerRect.y) / 10;
				// --------------------------------------DRAW
				
				SDL_RenderCopy(renderer, playerTexture, nullptr, &playerRect);
				SDL_RenderCopy(renderer, play_texture, nullptr, &play_text);
				SDL_RenderCopy(renderer, exit_texture, nullptr, &exit_text);

				SDL_RenderPresent(renderer);

			}

			//------------------------------------------------------------------------------------------ LEVEL ------------------------------------------------------------------------------
			else if (_Scene == Scene::LEVEL) {
				while (SDL_PollEvent(&evnt)) {
					switch (evnt.type) {
					case SDL_MOUSEMOTION:	playerTarget.x = evnt.motion.x - 25; playerTarget.y = evnt.motion.y - 25; break;
					case SDL_MOUSEBUTTONDOWN:

						//----------- LEVEL EASY ---------
						if (IM.Click_button(evnt, easy_text) == true) {
							player.TestXML("level.xml", "easy");
							player.set_x_y(WIDTH / 2, HEIGHT / 2);
							player.set_speed(player.get_initial_fruits());
							fruitRect.x = (rand() % (40)) * 25;
							fruitRect.y = (rand() % (30)) * 25;
							player.set_live(3);
							player.crear_mapa("easy");
							_Scene = Scene::INGAME;
						}

						//----------- LEVEL MEDIUM ---------
						else if(IM.Click_button(evnt, medium_text) == true){
							player.TestXML("level.xml", "medium");
							player.set_x_y(WIDTH / 2, HEIGHT / 2);
							player.set_speed(player.get_initial_fruits());
							fruitRect.x = (rand() % 16 + 13) * 25;
							fruitRect.y = (rand() % 12 + 10) * 25;
							player.set_live(2);
							player.crear_mapa("medium");
							_Scene = Scene::INGAME;
						}

						//----------- LEVEL HARD ---------
						else if (IM.Click_button(evnt, hard_text) == true) {
							player.TestXML("level.xml", "hard");
							player.set_x_y(WIDTH / 2, HEIGHT / 2);
							player.set_speed(player.get_initial_fruits());
							fruitRect.x = (rand() % 8 + 17) * 25;
							fruitRect.y = (rand() % 6 + 13) * 25;
							player.set_live(1);
							player.crear_mapa("hard");
							_Scene = Scene::INGAME;
						}
					default:;
					}
				}
				// -------------------------------------UPDATE
				playerRect.x += (playerTarget.x - playerRect.x) / 10;
				playerRect.y += (playerTarget.y - playerRect.y) / 10;
				// -------------------------------------DRAW
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, bgTexture, nullptr, &bgRect);
				SDL_RenderCopy(renderer, playerTexture, nullptr, &playerRect);
				SDL_RenderCopy(renderer, easy_texture, nullptr, &easy_text);
				SDL_RenderCopy(renderer, medium_texture, nullptr, &medium_text);
				SDL_RenderCopy(renderer, hard_texture, nullptr, &hard_text);
				SDL_RenderPresent(renderer);
			}

			//------------------------------------------------------------------------------------------ IN GAME ------------------------------------------------------------------------------
			else if (_Scene == Scene::INGAME) {
				consol();
				time_box.w = player.get_time() * 3;
				IM.Consold_keys();
				playerTarget.x = player.get_x();
				playerTarget.y = player.get_y();

				playerRect.x = playerTarget.x;
				playerRect.y = playerTarget.y;
				

				int *temp_dir;
				temp_dir = new int[player.get_actual_fruits()];
				for (int s = 0; s < player.get_actual_fruits(); s++) {
					temp_dir[s] = player.get_dir(s);
				}

				player.set_dir_sons(player.get_actual_fruits());

				if (direction == Player::DIR::UP) {
					player.set_dir(0, 0);
				}
				else if (direction == Player::DIR::DOWN) {
					player.set_dir(1, 0);
				}
				else if (direction == Player::DIR::LEFT) {
					player.set_dir(2, 0);
				}
				else if (direction == Player::DIR::RIGHT) {
					player.set_dir(3, 0);
				}
				for (int s = 1; s <= player.get_actual_fruits(); s++) {
					player.set_dir(temp_dir[s-1], s);
				}


				if (collision_fruit(player.get_x(), player.get_y(), fruitRect) == true) {
					int random_x;
					int random_y;
					if (player.get_initial_fruits() == 10) {
						random_x = (rand() % (40));
						random_y = (rand() % (30));
						
					}
					else if (player.get_initial_fruits() == 20) {
						random_x = (rand() % 16 + 13);
						random_y = (rand() % 12 + 10);
						
					}
					else if (player.get_initial_fruits() == 30) {
						random_x = (rand() % 8 + 17);
						random_y = (rand() % 6 + 13);
						
					}
					
					while (player.get_casillas(random_x, random_y) == 1) {
						if (player.get_initial_fruits() == 10) {
							random_x = (rand() % (40)) * 25;
							random_y = (rand() % (30)) * 25;
						}
						else if (player.get_initial_fruits() == 20) {
							random_x = (rand() % 16 + 13) * 25;
							random_y = (rand() % 12 + 10) * 25;
						}
						else if (player.get_initial_fruits() == 30) {
							random_x = (rand() % 8 + 17) * 25;
							random_y = (rand() % 6 + 13) * 25;
						}
					}
					

					fruitRect.x = random_x * 25;
					fruitRect.y = random_y * 25;

					player.set_fruits(player.get_fruits() - 1);
					player.set_score(player.get_score() + ((player.get_initial_fruits() + (player.get_augment_fruits() * player.get_player_level())) * 100));
					player.set_speed(player.get_speed() + (player.get_score() / 10000.00));
					cout << " Speed:  " << player.get_speed() << endl;
					player.set_actual_fruits(1);
					if (player.get_fruits() <= 0) {
						player.augment_player_level();
						player.set_fruits(player.get_initial_fruits() + (player.get_augment_fruits() * player.get_player_level()));
						player.set_time(player.get_initial_time(), 0.0);
					}
				}

				// -------colisio amb la paret
				if (player.get_x() < 0 || player.get_x() > WIDTH - playerRect.w || player.get_y() < 0 || player.get_y() > HEIGHT - playerRect.h) {
					player.set_m_live(true);
				}
				

				// -------Escriure el numero de fruites restants i el score.
				TTF_Font *font{ TTF_OpenFont("../../res/fnt/arial.ttf", 50) };

				SDL_Surface *fruits_surface{ TTF_RenderText_Blended(font, int_to_text(player.get_fruits()), SDL_Color{ 255, 0, 255, 255 }) };
					SDL_Texture *fruits_texture{ SDL_CreateTextureFromSurface(renderer, fruits_surface) };
					SDL_Rect fruits_text{ static_cast<int>(WIDTH*.90f),
						static_cast<int>(HEIGHT*.05f),
						fruits_surface->w, fruits_surface->h };
					SDL_FreeSurface(fruits_surface);

					SDL_Surface *score_surface{ TTF_RenderText_Blended(font, int_to_text(player.get_score()), SDL_Color{ 255, 0, 255, 255 }) };
					SDL_Texture *score_texture{ SDL_CreateTextureFromSurface(renderer, score_surface) };
					SDL_Rect score_text{ static_cast<int>(WIDTH*.45f),
						static_cast<int>(HEIGHT*.05f),
						score_surface->w, score_surface->h };
					SDL_FreeSurface(score_surface);

					SDL_Surface *lives_surface{ TTF_RenderText_Blended(font, int_to_text(player.get_live()), SDL_Color{ 255, 0, 255, 255 }) };
					SDL_Texture *lives_texture{ SDL_CreateTextureFromSurface(renderer, lives_surface) };
					SDL_Rect lives_text{ static_cast<int>(WIDTH*.90f),
						static_cast<int>(HEIGHT*.10f),
						lives_surface->w, lives_surface->h };
					SDL_FreeSurface(lives_surface);

				TTF_CloseFont(font);

				// ------ Dibuixar els diferents sprits (serp, fruita....)
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, bgTexture, nullptr, &bgRect);

				if (direction == Player::DIR::UP) {
					SDL_RenderCopy(renderer, up, nullptr, &playerRect);
				}
				else if (direction == Player::DIR::DOWN) {
					SDL_RenderCopy(renderer, down, nullptr, &playerRect);
				}
				else if (direction == Player::DIR::LEFT) {
					SDL_RenderCopy(renderer, left, nullptr, &playerRect);
				}
				else if (direction == Player::DIR::RIGHT) {
					SDL_RenderCopy(renderer, right, nullptr, &playerRect);
				}
				
				for (int s = 0; s < player.get_actual_fruits(); s++) {
					int temp_x = 0;
					int temp_y = 0;
					for (int n = 0; n <= s; n++) {
						if (player.get_dir(n) == 0) {
							temp_y = temp_y + 25;
						}
						else if (player.get_dir(n) == 1) {
							temp_y = temp_y - 25;
						}
						else if (player.get_dir(n) == 2) {
							temp_x = temp_x + 25;
						}
						else if (player.get_dir(n) == 3) {
							temp_x = temp_x - 25;
						}
					}
					
					if (player.get_x() + temp_x == playerRect.x && player.get_y() + temp_y == playerRect.y) {
						player.set_m_live(true);
					}

					playerRect.x = player.get_x() + temp_x;
					playerRect.y = player.get_y() + temp_y;
					
					if (player.get_dir(s) != player.get_dir(s+1)) {
						if (player.get_dir(s) == 0 && player.get_dir(s+1) == 2){
							SDL_RenderCopy(renderer, curva4, nullptr, &playerRect);
						}
						else if (player.get_dir(s) == 0 && player.get_dir(s + 1) == 3) {
							SDL_RenderCopy(renderer, curva3, nullptr, &playerRect);
						}


						else if (player.get_dir(s) == 1 && player.get_dir(s + 1) == 2) {
							SDL_RenderCopy(renderer, curva2, nullptr, &playerRect);
						}
						else if (player.get_dir(s) == 1 && player.get_dir(s + 1) == 3) {
							SDL_RenderCopy(renderer, curva1, nullptr, &playerRect);
						}


						else if (player.get_dir(s) == 2 && player.get_dir(s + 1) == 0) {
							SDL_RenderCopy(renderer, curva1, nullptr, &playerRect);
						}
						else if (player.get_dir(s) == 2 && player.get_dir(s + 1) == 1) {
							SDL_RenderCopy(renderer, curva3, nullptr, &playerRect);
						}


						else if (player.get_dir(s) == 3 && player.get_dir(s + 1) == 0) {
							SDL_RenderCopy(renderer, curva2, nullptr, &playerRect);
						}
						else if (player.get_dir(s) == 3 && player.get_dir(s + 1) == 1) {
							SDL_RenderCopy(renderer, curva4, nullptr, &playerRect);
						}
					}
					else {
					
						if (player.get_dir(s) == 0 || player.get_dir(s) == 1) {
							SDL_RenderCopy(renderer, cuerpo_up_down, nullptr, &playerRect);
						}
						else if (player.get_dir(s) == 2 || player.get_dir(s) == 3) {
							SDL_RenderCopy(renderer, cuerpo_left_right, nullptr, &playerRect);
						}
					}
					playerRect.x = player.get_x();
					playerRect.y = player.get_y();
				}
				for (int x = 0; x < 40; x++) {
					for (int y = 0; y < 30; y++) {
						if (player.get_casillas(x, y) == 1) {
							pared.x = x * 25;
							pared.y = y * 25;
							if (collision_fruit(player.get_x(), player.get_y(), pared) == true) {
								player.set_m_live(true);
							}

							SDL_RenderCopy(renderer, playerTexture, nullptr, &pared);
						}
					}
				}
				




				if (player.get_m_live() == true) {
					player.set_x_y(WIDTH / 2, HEIGHT / 2);
					player.set_score(0);
					player.set_speed(player.get_initial_fruits());
					if (player.get_player_level() == 0) {
						player.set_int_f(0);
					}
					else {
						player.set_int_f((player.get_initial_fruits() * player.get_player_level())+ (player.get_augment_fruits() * (player.get_player_level() - 1)));
						for (int l = 0; l < player.get_player_level(); l++) {
							player.set_score( player.get_score() +(((player.get_initial_fruits() + (player.get_augment_fruits() * l ) )* 100) * (player.get_initial_fruits() + (player.get_augment_fruits() * l))));
							player.set_speed(player.get_speed() + (player.get_score() / 10000.00));
						}
					}
					player.set_live(player.get_live() - 1);
					player.set_fruits(player.get_initial_fruits() + (player.get_augment_fruits() * player.get_player_level()));
					player.set_time(player.get_initial_time(), 0);
					player.set_m_live(false);
				}










				SDL_RenderCopy(renderer, fruitTexture, nullptr, &fruitRect);
				SDL_RenderCopy(renderer, playerTexture, nullptr, &time_box);

				SDL_RenderCopy(renderer, fruits_texture, nullptr, &fruits_text);
				SDL_RenderCopy(renderer, score_texture, nullptr, &score_text);
				SDL_RenderCopy(renderer, lives_texture, nullptr, &lives_text);

				SDL_RenderPresent(renderer);

				SDL_DestroyTexture(fruits_texture);
				SDL_DestroyTexture(score_texture);

				// ------------- Afegir la puntuació, i guardar-la si és una de les 10 millors
				if (player.get_live() <= 0) {
					names_score t;
					for (int x = 0; x < 10; x++) {
						for (int y = 0; y < 10 - 1; y++) {
							if (list_players[y].score < list_players[y + 1].score) {
								t = list_players[y];

								list_players[y] = list_players[y + 1];

								list_players[y + 1] = t;

							}
						}
					}
					// ----ordenar la puntuació
					char name_p[20];
					std::cout << "Introdueix nom del player (maxim 20 characters)" << std::endl;
					std::cin >> name_p;
					player.set_Name(name_p);
					if (player.get_score() > list_players[9].score){
						list_players[9].score = player.get_score();
						strcpy(list_players[9].name, player.get_Name());
						for (int x = 0; x < 10; x++) {
							for (int y = 0; y < 10 - 1; y++) {
								if (list_players[y].score < list_players[y + 1].score) {
									t = list_players[y];

									list_players[y] = list_players[y + 1];

									list_players[y + 1] = t;

								}
							}
						}
					}
					// --- Pasar-la al fitxer
					ofstream filescore;
					filescore.open("ranking.bin", std::ios::binary);
					if (filescore.is_open()) {
						filescore.write((char*)(names_score*)list_players, sizeof(list_players));

						filescore.close();
					}
					_Scene = Scene::MENU;
				}
				Sleep(500 - player.get_speed());
			}
		}
			
		// --- DESTROY ---
		Mix_CloseAudio();

		SDL_DestroyTexture(bgTexture);
		SDL_DestroyTexture(play_texture);
		SDL_DestroyTexture(exit_texture);
		SDL_DestroyTexture(easy_texture);
		SDL_DestroyTexture(medium_texture);
		SDL_DestroyTexture(hard_texture);
		SDL_DestroyTexture(playerTexture);
		SDL_DestroyTexture(fruitTexture);

		SDL_DestroyTexture(up);
		SDL_DestroyTexture(down);
		SDL_DestroyTexture(left);
		SDL_DestroyTexture(right);



		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		// --- EXIT ---
		Mix_Quit();
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

}

