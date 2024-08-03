#include <hpp/MyCommand.hpp>

namespace MyCommands {
	bool ok_id(string str, vector<Points::Solving*>& arr) {
		if (!Commands::is_int(str) || arr.size() <= stoi(str))
			return false;
		return arr[stoi(str)]->is_ok();
	}

	// Color
	void Color::run(vector<string> arg, Console* console) {
		if (arg.size() == 1)
			console->add_text("not valid");
		else {
			if (arg[1] == "circle" && arg.size() == 5 && ok_int(arg[2]) && ok_int(arg[3]) && ok_int(arg[4]))
				cir->set_color({ (unsigned char)stoi(arg[2]), (unsigned char)stoi(arg[3]), (unsigned char)stoi(arg[4]), 255 });
			else if (arg[1] == "console" && arg.size() == 5 && ok_int(arg[2]) && ok_int(arg[3]) && ok_int(arg[4]))
				console->color = { (unsigned char)stoi(arg[2]), (unsigned char)stoi(arg[3]), (unsigned char)stoi(arg[4]), 255 };
			else if (arg[1] == "points" && arg.size() == 6 && ok_id(arg[2], arr_sol) && ok_int(arg[3]) && ok_int(arg[4]) && ok_int(arg[5]))
				arr_sol[stoi(arg[2])]->set_color({ (unsigned char)stoi(arg[3]), (unsigned char)stoi(arg[4]), (unsigned char)stoi(arg[5]), 255 });
			else
				console->add_text("not valid");
		}
	}

	// AddPoint
	void AddPoint::run(vector<string> arg, Console* console) {
		if (arg.size() == 1)
			console->add_text("not valid arg");
		else {
			string name = "";
			for (int i = 1; i < arg.size(); ++i)
				name += arg[i];
			Math::MainOperator* q = new Math::MainOperator(name);
			if (q->is_ok()) {
				if (q->get_count_per() < 2) {
					q->optimize();
					Points::Solving* w = new Points::Solving(q);
					if (w->is_ok()) {
						for (int i = 0; i < arr.size(); ++i)
							if (!arr[i]->is_ok()) {
								delete arr[i];
								arr[i] = w;
								console->add_text("ok, points: " + to_string(w->get_count_points()) + ", id: " + to_string(i));
								return;
							}
						arr.push_back(w);
						console->add_text("ok, points: " + to_string(w->get_count_points()) + ", id: " + to_string(arr.size() - 1));
					}
					else {
						delete w;
						console->add_text("Too much");
					}
				}
				else {
					delete q;
					console->add_text("0 or 1 variable, sorry");
				}
			}
			else {
				delete q;
				console->add_text("I can't understand");
			}
		}
	}

	// List
	void List::run(vector<string> arg, Console* console) {
		if (arg.size() != 2)
			console->add_text("not valid count arg");
		else {
			if (arg[1] == "points") {
				for (int i = 0; i < arr.size(); ++i)
					if (arr[i]->is_ok())
						for (int j = 0; j < arr[i]->get_count_points(); ++j)
							console->add_text(arr[i]->get_point(j)->get_name());
			} else if (arg[1] == "solving") {
				for (int i = 0; i < arr.size(); ++i)
					if (arr[i]->is_ok())
						console->add_text(to_string(i) + " id: " + arr[i]->get_name());
			} else
				console->add_text("points or solving");
		}
	}

	// Delete
	void Delete::run(vector<string> arg, Console* console) {
		if (arg.size() != 2)
			console->add_text("not valid size arg");
		else if (ok_id(arg[1], arr))
			arr[stoi(arg[1])]->delete_me();
		else if (arg[1] == "all") {
			for (int i = 0; i < arr.size(); ++i)
				delete arr[i];
			arr.clear();
		}
		else
			console->add_text("not valid id");
	}

	// Hide
	void Hide::run(vector<string> arg, Console* console) {
		if (arg.size() != 2)
			console->add_text("not valid size arg");
		else if (!ok_id(arg[1], arr))
			console->add_text("not valid id");
		else
			arr[stoi(arg[1])]->hide();
	}

	// Show
	void Show::run(vector<string> arg, Console* console) {
		if (arg.size() != 2)
			console->add_text("not valid size arg");
		else if (!ok_id(arg[1], arr))
			console->add_text("not valid id");
		else
			arr[stoi(arg[1])]->show();
	}
}