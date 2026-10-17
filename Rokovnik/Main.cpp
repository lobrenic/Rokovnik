#include <iostream>
#include <stdexcept>
#include "database.h"
#include "notes.h"
#include "ui.h"
#include "reminder.h"
#include "notifier.h"
int main(int argc, char* argv[]) {
	bool remindMode = (argc > 1 && std::strcmp(argv[1], "--remind") == 0);
	try {
		Database db("127.0.0.1", "rokovnik_app", "", "rokovnik");
		db.open();
		Notes notes(db);
		if (remindMode) {
			auto exams = notes.getExamsWithin(7);
			if (!exams.empty()) {
				std::string text = buildReminderText(exams);
				std::cout << "Predstojeci ispiti:\n" <<text;
				if (!showToast("Predstojeci ispiti", text))
					std::cerr << "Toast nije prikazan\n";

			}
				
			return 0;
		}
		UI ui(notes);
		ui.run();
	}
	catch (const std::exception& e) {
		std::cout << "Greska: " << e.what() << "\n";
		if(!remindMode)
			std::cin.get();
		return 1;
	}
}