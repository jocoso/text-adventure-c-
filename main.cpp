#include <string>
#include <iostream>
#include <conio.h>
#include <stdlib.h>

#define MAX_NUM_ITEMS 10

// Data Types
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


// Clean up memory
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
	world[0][0].visible = false;

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

void describe_space(st_space* space) {
	std::cout << space->info << std::endl;

	std::cout << "Where you can interact with: " << std::endl;
	tmp_obj = space->objects;

	while (tmp_obj != NULL) {
		std::cout << tmp_obj->name << std::endl;
		tmp_obj = tmp_obj->next;
	}
	
}

void describe_grid(st_grid *grid) {

	if (grid->visible) {
		std::cout << "At your left there is ";
		describe_space(&grid->west);
	}
	else {
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

void setup_player(void) {

	player.current_grid = &world[0][0];
	st_object *obj = add_newobject(player.inventory.items, "LATERN", "black", "sturdy plastic", "off");
	player.inventory.items = obj;
	player.inventory.num_items += 1;
}

void play_intro(void) {
	std::cout << "You open your eyes. Pitch black greets you.\n\n";
	std::cout << "The air smells like ash and decay.\n\n";
	std::cout << "You hear a noise outside and the sound of approaching steps followed by ";
	std::cout << "an opening rectangle of white light that frames familiar eyes.\n\n";
	std::cout << "You catch two dry sounds before the light vanishes.\n\n";
	std::cout << "Something rolls towards and touches your naked feet.\n\nYour fingers touch its cold, soft surface; it is a latern\n\n";
}


bool analyze_text(std::string input) {

	if (input.find("QUIT") != std::string::npos) {
		return false;
	}

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

	else if(input.find("EQUIP") != std::string::npos) {
		
		st_object *o = search(player.inventory.items, input);
		if(o != NULL && player.inventory.num_items > 0) {
			if(input.find("RIGHT") != std::string::npos) {
				player.equip_slot_rHand = o;
				player.inventory.num_items -= 1;
				std::cout << o->name << " was equippted in right hand\n\n";
			} else if(input.find("LEFT") != std::string::npos) {
				player.equip_slot_lHand = o;
				player.inventory.num_items -= 1;
				std::cout << o->name << " was equipted in left hand\n\n";
			} else {
				std::cout << "Try equipping in RIGHT or LEFT hand";
			}
		} else {
			std::cout << "Cannot EQUIP that item\n\n" << std::endl;
		}

	}

	else if(input.find("TURN") != std::string::npos) {

		if(player.equip_slot_rHand != NULL && input.find(player.equip_slot_rHand->name) != std::string::npos) {
			
			if(input.find("ON") != std::string::npos) {
				player.equip_slot_rHand->status = "on";
				std::cout << player.equip_slot_rHand->name << " now is on\n\n";
			} else if(input.find("OFF") != std::string::npos) {
				player.equip_slot_rHand->status = "off";
				std::cout << player.equip_slot_rHand->name << " now is off\n\n";
			} else {
				std::cout << "Try TURNING " << player.equip_slot_rHand->name << " ON or OFF\n\n";
			}

		} else {
			std::cout << "Turn what?" << std::endl;
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
	setup_player();

	play_intro();

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