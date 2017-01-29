#pragma once
#include <string>	// Needed to process error messages

#include <iostream>
#include <stdlib.h> 
#include <ctime>
#include <time.h>
#include <windows.h>

#include <SDL\SDL.h>		// Always needs to be included for an SDL app
#include <SDL\SDL_ttf.h>	// Used for loading fonts and rendering text
#include <SDL\SDL_image.h>	// Used for loading and drawing sprites
#include <SDL\SDL_mixer.h>	// Used for loading and playing audio files
#include "Player.h"

Player player;
Player::DIR direction; 

class ImputManager {
public:
	// -------- colisio de les cordenades del mouse amb els bottons de: play, exit... -------
	bool Click_button(SDL_Event mouse, SDL_Rect button) {
		bool click = false;
		if (mouse.motion.x >= button.x && mouse.motion.x <= button.x + button.w && mouse.motion.y >= button.y && mouse.motion.y <= button.y + button.h) {
			click = true;
		}
		return click;
	}

	// -------- les tecles canvien la direccció del jugador, depenent de la tecla que clics i si no és una contradicció com canviar d'amunt cap abaix-------
	void Consold_keys() {
		SDL_Event evnt;
		while (SDL_PollEvent(&evnt)) {
			switch (evnt.type) {
			case SDL_KEYDOWN:
				switch (evnt.key.keysym.sym) {
				case SDLK_UP:
					if (direction != Player::DIR::DOWN) {
						direction = Player::DIR::UP;
					}
					break;
				case SDLK_DOWN:
					if (direction != Player::DIR::UP) {
						direction = Player::DIR::DOWN;
					}
					break;
				case SDLK_LEFT:
					if (direction != Player::DIR::RIGHT) {
						direction = Player::DIR::LEFT;
					}
					break;
				case SDLK_RIGHT:
					if (direction != Player::DIR::LEFT) {
						direction = Player::DIR::RIGHT;
					}
					break;
				default:
					break;
				}
				break;
			}
		}
		if (direction == Player::DIR::UP) {
			player.set_x_y(player.get_x(), player.get_y() - 25);
		}
		else if (direction == Player::DIR::DOWN) {
			player.set_x_y(player.get_x(), player.get_y() + 25);
		}
		else if (direction == Player::DIR::LEFT) {
			player.set_x_y(player.get_x() - 25, player.get_y());
		}
		else if (direction == Player::DIR::RIGHT) {
			player.set_x_y(player.get_x() + 25, player.get_y());
		}
	}
private:
};