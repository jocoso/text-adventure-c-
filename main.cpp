#include <string.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <stdlib.h>

#define MAX_NUM_ITEMS 10

enum types {TURNABLE = 0, SCENERY = 1, READABLE = 2, USABLE = 3};

struct st_grid;

// Data Types
struct st_object {
	std::string name, color, material, status;
	int type;
	struct st_object* next;
};

struct st_door {
	bool created, open;
	st_grid *next;
};

struct st_grid {
	bool created, visible;
	std::string info;
	st_object* objects;
	struct st_door *east, *west, *north, *south, *center;
};

struct ch_inventory {
	int num_items;
	struct st_object* items;
};

struct ch_player {
	std::string name;
	struct st_grid *current_grid;
	struct ch_inventory inventory;
	struct st_object *equip_slot_rHand;
	struct st_object *equip_slot_lHand;
};

// Implementation
struct st_grid world[3][3];
struct ch_player player = { 
	.name = "", 
	.current_grid = NULL, 
	.inventory = { .num_items = 0, .items = NULL }, 
	.equip_slot_rHand = NULL,
	.equip_slot_lHand = NULL
};

struct st_object *tmp_obj = NULL;
int id_count = 0;


// Adds a new object to the linked list
st_object* add_newobject(st_object* obj, std::string name, std::string color, std::string material, std::string status, int type) {
	st_object* o = obj;

	if (o == NULL) {
		o = new st_object();
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

		o->next = new st_object();
		o->next->name = name;
		o->next->color = color;
		o->next->material = material;
		o->next->status = status;
		o->next->next = NULL;
		return(o->next);
	}
	return NULL;
}


// Clean up memory
void clean_grid(st_grid *grid) {
	if (grid == NULL) return;

	if (grid->created) {
		st_object *o = grid->objects;
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

void setup_map(void) {
	st_object *obj;
	world[0][0].created = true;
	world[0][0].visible = false;
	world[0][0].objects = NULL;

	// Create Map
	// World[0][0] - CELL
	world[0][0].info = "A small square cell a couple of steps long. The cell has a bed and a toilet otherwise it is barren and empty.\n";
	// add_newobject(st_object* obj, std::string name, std::string color, std::string material, std::string status, std::string type)
	obj = add_newobject(world[0][0].objects, "toilet", "white", "ceramic", "dirty", types::USABLE);
	world[0][0].objects = obj;
	add_newobject(world[0][0].objects, "walls", "gray", "concrete", "impenetrable", types::SCENERY);
	world[0][0].objects = obj;
	add_newobject(world[0][0].objects, "notebook", "black", "leather", "light", types::READABLE);

	world[0][0].north  = NULL;
	world[0][0].east   = NULL;
	world[0][0].south  = NULL;
	world[0][0].west   = NULL;
	world[0][0].center = NULL;

}

void describe_grid(st_grid* grid) {
	if (grid->visible) {
		std::cout << grid->info << std::endl;

		if(grid->objects != NULL) {
			std::cout << "Where you can interact with: " << std::endl;
			tmp_obj = grid->objects;

			while (tmp_obj != NULL) {
				std::cout << "\t- *" << tmp_obj->name << std::endl;
				tmp_obj = tmp_obj->next;
			}
		}
	} else {
		std::cout << "It is dark around. You will have to find a source of light if you'd like to see something.\n\n";
	}
	
}

st_object *search(st_object *root, std::string name) {
	if(name.empty() || root == NULL) return NULL;

	st_object *o = root;

	while(o != NULL) {

		if(name.find(o->name) != std::string::npos) return o;
		o = o->next;

	} 

	return NULL;
}

void describe_inventory(const ch_inventory* inventory) {

	if(inventory->num_items > 0) {
		std::cout << "=== INVENTORY ===\n";
		
		st_object* ob = inventory->items;

		for(int i = 0; i < inventory->num_items; i++) {
			std::cout << "-" << ob->name << std::endl << "\n";
			ob = ob->next;
		}
	} else {
		std::cout << "The inventory is empty" << std::endl;
	}
}

void add_to_inventory(std::string name, std::string color, std::string material, std::string state, int type) {
	st_object *obj = add_newobject(player.inventory.items, name, color, material, state, type);
	player.inventory.items = obj;
	player.inventory.num_items += 1;

	std::cout << "====== " << name << " added to your inventory ======\n\n";
}

void setup_player(void) {

	player.current_grid = &world[0][0];
	add_to_inventory("FLASHLIGHT", "black", "hard plastic", "off", types::TURNABLE);
	
}

void play_intro(void) {
	std::cout << "You open your eyes. Pitch black greets you.\n\n";
	std::cout << "The air smells like decay and ash.\n\n";
	std::cout << "You hear a noise outside and the sound of approaching steps.\n\n";
	std::cout << "In the distance, a porthole opens where white light pours plentiful.\n\n";
	std::cout << "Two dry sounds you hear before the light suddenly vanishes.\n\n";
	std::cout << "Something rolls towards and touches your bare feet.\n\nYour fingers touch its cold, soft surface; it is a FLASHLIGHT.\n\n";
}


bool analyze_text(std::string input) {

	// QUIT main loop
	if (input.find("QUIT") != std::string::npos) {
		return false;
	}

	// Describe current room
	else if (input.find("LOOK") != std::string::npos) {
		if (input.find("AROUND") != std::string::npos) {
			describe_grid(player.current_grid);
		}
	}

	else if (input.find("CHECK") != std::string::npos) {
		if (input.find("INVENTORY") != std::string::npos) {
			describe_inventory(&player.inventory);
		}
	}

	// EQUIP ITEM_NAME RIGHT/LEFT
	else if(input.find("EQUIP") != std::string::npos) {
		
		st_object *o = search(player.inventory.items, input);
		if(o != NULL && player.inventory.num_items > 0) {
			if(input.find("RIGHT") != std::string::npos) {
				player.equip_slot_rHand = o;
				player.inventory.num_items -= 1;
				std::cout << o->name << " was equipped in right hand\n\n";
			} else if(input.find("LEFT") != std::string::npos) {
				player.equip_slot_lHand = o;
				player.inventory.num_items -= 1;
				std::cout << o->name << " was equipped in left hand\n\n";
			} else {
				std::cout << "Try equipping in RIGHT or LEFT hand\n\n";
			}
		} else {
			std::cout << "Cannot EQUIP that item\n\n" << std::endl;
		}

	}

	else if(input.find("TURN") != std::string::npos) {

		st_object *turnable = NULL;

		// Nothing equipped
		if(player.equip_slot_lHand == NULL && player.equip_slot_rHand == NULL) {
			std::cout << "Try EQUIPping something first\n\n";
		}

		// If there is something equipped in left hand and it was mentioned in the player mandate
		if(player.equip_slot_lHand != NULL && input.find(player.equip_slot_lHand->name) != std::string::npos) {
			turnable = player.equip_slot_lHand;
		} else if (player.equip_slot_rHand != NULL && input.find(player.equip_slot_rHand->name) != std::string::npos) {
			turnable = player.equip_slot_rHand;
		} else {
			std::cout << "Couldn't find the item requested\n\n";
		}

		// Not type turnable
		if(turnable != NULL && turnable->type != types::TURNABLE) {
			std::cout << turnable->name << " cannot be turned on or off\n\n";
		// Turning on
		} else if(turnable != NULL && input.find("ON") != std::string::npos) {
			if(player.current_grid->visible == false) player.current_grid->visible = true;
			turnable->status = "on";
			std::cout << turnable->name << " is now ON\n\n";
		// Turning off
		} else if(turnable != NULL && input.find("OFF") != std::string::npos) {
			if(player.current_grid->visible == true && strcmp(turnable->status.c_str(), "on") == 0) player.current_grid->visible = false;
			turnable->status = "off";
			std::cout << turnable->name << " is now OFF\n\n";
		} else {
			std::cout << "Try TURNING something ON or OFF\n\n";
		}
		
	}
	
	else if (input.find("MOVE") != std::string::npos) {
		if (input.find("NORTH") != std::string::npos) {
			// move north
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
	play_intro();
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