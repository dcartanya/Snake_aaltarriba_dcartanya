#pragma once
#include <map>
#include <string>	// Needed to process error messages

#include <iostream>
#include <stdlib.h> 
#include <ctime>

#include <fstream>
#include <sstream>
#include <iterator>
#include <XML/rapidxml_utils.hpp>

#define RESOURCE_PATH "../../res/cfg/"
#define RESOURCE_FILE(filename) (RESOURCE_PATH + (filename)).c_str()

using namespace std;


class Player{
public:
	enum class DIR {
		UP,		// Menu Scene 
		DOWN,	// select level dificulty
		LEFT,		// Play game
		RIGHT		// ranking scene
	};

	int get_x() {
		return pos_x;
	}
	int get_y() {
		return pos_y;
	}
	void set_x_y(int x, int y) {
		pos_x = x;
		pos_y = y;
	}
	float get_speed() {
		return speed;
	}
	void set_speed(float _speed) {
		speed = _speed;
	}
	int get_score() {
		return score;
	}
	void set_score(int _score) {
		score = _score;
	}

	int get_fruits() {
		return fruits;
	}
	void set_fruits(int _fruits) {
		fruits = _fruits;
	}

	int get_augment_fruits() {
		return augment_fruits;
	}
	void set_augment_fruits(int _augment_fruits) {
		augment_fruits = _augment_fruits;
	}

	float get_time() {
		return time;
	}
	void set_time(float _time, float minus_time) {
		time = _time - minus_time;
	}

	int get_player_level() {
		return player_level;
	}
	void augment_player_level() {
		player_level = get_player_level() + 1;
	}
	void set_player_level(int _palyer_level) {
		player_level = _palyer_level;
	}

	int get_initial_fruits() {
		return initial_fruits;
	}
	void set_initial_fruits(int _initial_fruits) {
		initial_fruits = _initial_fruits;
	}

	int get_initial_time() {
		return initial_time;
	}

	void set_initial_time(int _initial_time) {
		initial_time = _initial_time;
	}


	void set_dir_sons(int sons) {
		player_dir = new int[sons];
	}
	void set_dir(int dir, int son) {
		player_dir[son] = dir;
	}
	int get_dir(int son) {
		return player_dir[son];
	}

	int get_actual_fruits() {
		return actual_fruits;
	}
	void set_actual_fruits(int fruits) {
		actual_fruits = actual_fruits + fruits;
	}
	void set_int_f(int f) {
		actual_fruits = f;
	}

	char *get_Name() {
		return _Name;
	}

	void set_Name(char Name[20]) {
		strcpy(_Name, Name);

	}

	int get_live() {
		return live;
	}
	void set_live(int l) {
		live = l;
	}

	int text_to_int(char *text) {
		int num = atoi(text);
		return num;
	}

	int text_to_float(char *text) {
		float num = atoi(text);
		return num;
	}

	char *float_to_text(float num) {
		char text[10];
		_itoa_s(num, text, 10);
		return text;
	}
	void TestXML(std::string &&filename, const char *level_dificult) {
		rapidxml::file<> xmlFile(RESOURCE_FILE(filename));
		rapidxml::xml_document<> doc;
		doc.parse<0>(xmlFile.data());
		rapidxml::xml_node<> *root_node = doc.first_node(level_dificult);
		for (rapidxml::xml_node<> * node = root_node->first_node("node"); node; node = node->next_sibling()) {
			cout << "Node: " <<
				node->first_attribute("dificult")->value() <<
				" - " <<
				node->first_attribute("x")->value() <<
				" - " <<
				node->first_attribute("y")->value() <<
				" - " <<
				node->first_attribute("time")->value() <<
				endl;
			node->first_attribute("dificult")->value();
			set_initial_fruits(text_to_int(node->first_attribute("x")->value()));
			set_augment_fruits(text_to_int(node->first_attribute("y")->value()));
			set_time(text_to_float(node->first_attribute("time")->value()), 0);
		}
		set_fruits(get_initial_fruits());
		set_player_level(0);
		set_score(0);
		set_initial_time(get_time());
		set_speed(2.0000);
		set_dir_sons(get_initial_fruits() + (get_augment_fruits() * get_player_level()) - get_fruits());
		set_int_f(0);
	}

	void crear_mapa(const char *dificult) {
		for (int x = 0; x < 40; x++) {
			for (int y = 0; y < 30; y++) {
				casillas[x][y] = 0;
			}
		}

		if (dificult == "easy") {
			//40 30
			//5*8 x ; 5*6 y
			for (int x = 16; x < 25; x++) {
				casillas[x][12] = 1;
				casillas[x][19] = 1;
			}
			casillas[14][11] = 1;
			casillas[14][20] = 1;
			casillas[27][11] = 1;
			casillas[27][20] = 1;

		}
		else if (dificult == "medium") {
			//16 12
			//2*8 x ; 2*6 y
			for (int x = 12; x < 29; x++) {
				casillas[x][9] = 1;
				casillas[x][22] = 1;
			}
			for (int x = 9; x < 22; x++) {
				casillas[12][x] = 1;
				casillas[29][x] = 1;
			}

			casillas[14][11] = 1;
			casillas[14][20] = 1;
			casillas[27][11] = 1;
			casillas[27][20] = 1;
		}
		else if (dificult == "hard") {
			//1*8 x ; 1*6 y
			for (int x = 16; x < 25; x++) {
				casillas[x][12] = 1;
				casillas[x][19] = 1;
			}
			for (int x = 12; x < 19; x++) {
				casillas[16][x] = 1;
				casillas[25][x] = 1;
			}
		}

	}

	void set_mapa(int x, int y, int stat) {
		casillas[x][y] = stat;
	}

	int get_casillas(int x, int y) {
		return casillas[x][y];
	}

	bool get_m_live() {
		return m_live;
	}
	void set_m_live(bool live) {
		m_live = live;
	}
private:
	int score; 
	float speed = 2.0000;
	int pos_x;
	int pos_y;
	float time;
	int initial_time;
	int fruits;
	int augment_fruits;

	int player_level = 0;
	int initial_fruits;

	int *player_dir;

	int actual_fruits = 0;
	char _Name[20];

	int live = 3;
	int casillas[39][31];

	bool m_live = false;
};