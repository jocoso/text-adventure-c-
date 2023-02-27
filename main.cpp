#include <string>
#include <iostream>
#include <conio.h>
#include <stdlib.h>

struct st_object {
	std::string name, color, material, status;
	struct st_object* next;
};

struct st_space {
	bool created;
	std::string info;
	st_object* objects;
};

struct st_grid {
	bool created, visible;
	struct st_space east, west, north, south, center;
};

struct ch_player {
	std::string name;
	int position;
	struct st_grid current_grid;
};

struct st_grid world[3][3];
struct ch_player player = {.name="", .current_grid = NULL };
int id_count = 0;

st_object* add_newobject(st_object* obj, std::string name, std::string color, std::string material, std::string status) {
	st_object* o = obj;

	if (o == NULL) {
		o = new st_object;
		o->name = name;
		o->color = color;
		o->material = material;
		o->status = status;
		o->next = NULL;
		return(o);
	} else {
		while (o->next != NULL) {
			o = o->next;
		}

		o->next = (struct st_object*) malloc(sizeof(st_object));
		o->next->name = name;
		o->next->color = color;
		o->next->material = material;
		o->next->status = status;
		o->next->next = NULL;
		return(o->next);
	}
	return NULL;
}

void clean_space(st_space *spc) {
	if (spc == NULL) return;

	if (spc->created) {
		st_object *o = spc->objects;
		st_object *t = NULL;
		
		if (o == NULL) return;
		while (o->next != NULL) {
			t = o->next;
			free(o);
			o = t;
		}

		free(o);
	}
}

void clean_grid(st_grid *grid) {
	clean_space(&grid->north);
	clean_space(&grid->east);
	clean_space(&grid->south);
	clean_space(&grid->west);
}

void setup_map(void) {
	st_object *obj;
	world[0][0].created = true;

	// GRID 0, 0 NORTH
	world[0][0].north.created = true;
	world[0][0].north.info = "After the door closes behind you. with a loud BANG! You find yourself in an ill iluminated room. There is a putrid smell coating the air.";
	obj = add_newobject(world[0][0].north.objects, "door", "black", "wood", "locked");
	world[0][0].north.objects = obj;

	// GRID 0,0 EAST
	world[0][0].east.created = true;
	world[0][0].east.info = ".";
	obj = add_newobject(world[0][0].east.objects, "air", "dark", "gas", "rotten");
	world[0][0].east.objects = obj;

	// GRID 0, 0 WEST
	world[0][0].west.created = true;
	world[0][0].west.info = "a Door to the next room.";
	obj = add_newobject(world[0][0].west.objects, "door", "brown", "wooden", "destroyed");
	world[0][0].west.objects = obj;

	//// GRID 0, 0 CENTER
	world[0][0].center.created = true;
	world[0][0].center.info = "The floor is decorated with a symbol. The symbol is magical in nature.";
	obj = add_newobject(world[0][0].center.objects, "symbol", "black", "ink", "incomplete");
	world[0][0].center.objects = obj;
}

void describe_space(st_grid *grid) {
	std::cout << grid->north->info << std::endl;
	
	st_object* obj = grid->north->objects;

	if(obj != NULL) std::cout << "You can see: " << std::endl;
	while (obj != NULL) {
		std::cout << "A " << obj->status << " " << obj->color << " " << obj->name << " made out of " << obj->material << std::endl;
		obj = obj->next;
	}


}

void setup_player(void) {

	player.current_space = world[0][0];

	char name[50];
	std::cout << "What is your name, sacrifice?" << std::endl;
	std::cout << "> ";
	std::cin.getline(name, 50);
	player.name = name;

	std::cout << "My condolences, " << player.name << ". You deserved better." << std::endl;
	std::cout << "-------------- press enter to continue --------------" << std::endl;
	_getch();
}



bool analyze_text(std::string input) {

	if (input.find("QUIT") != std::string::npos) {
		return false;
	}

	else if (input.find("LOOK") != std::string::npos) {
		if (input.find("AROUND") != std::string::npos) {
			describe_space(&player.current_space);
		}
	}
	
	else if (input.find("MOVE") != std::string::npos) {
		if (input.find("NORTH") != std::string::npos) {
			// move nort
		}
		else if (input.find("EAST") != std::string::npos) {
			// move east
		}
		else if (input.find("SOUTH") != std::string::npos) {
			// move south
		}
		else if (input.find("WEST") != std::string::npos) {
			// move west
		}
		else {
			// unknown
		}
	}
	
	
	else {
		std::cout << "What?" << std::endl;
	}

	return true;
}

void init(void) {
	setup_map();
	setup_player();


	std::string input = "LOOK AROUND";

	while (analyze_text(input)) {
		std::cout << ">";
		std::getline(std::cin, input);
	}

	for (int i = 0; i < 3; i++) {

		clean_grid(world[i]);

	}
}

int main() {
	init();
	return 0;
}