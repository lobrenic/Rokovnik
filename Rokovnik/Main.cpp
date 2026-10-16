#include <iostream>
#include <stdexcept>
#include "database.h"
#include "notes.h"
#include "ui.h"

int main() {
	try {
		Database db("127.0.0.1", "rokovnik_app", "", "rokovnik");
		db.open();
		Notes notes(db);
		UI ui(notes);
		ui.run();
	}
	catch (const std::exception& e) {
		std::cout << "Greska: " << e.what() << "\n";
		std::cin.get();
		return 1;
	}
}