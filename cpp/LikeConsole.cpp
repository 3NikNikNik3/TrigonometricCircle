#include <hpp/LikeConsole.hpp>

namespace LikeConsole {
	vector<string> split(string input, char what) {
		vector<string> ans(1, "");
		for (int i = 0; i < input.size(); ++i)
			if (input[i] == what)
				ans.push_back("");
			else
				ans[ans.size() - 1] += input[i];
		return ans;
	}

	// Console
	Console::Console(map<string, Command*> arr, int font_size) : arr(arr), font_size(font_size) {
		text.push_back("Hello, to help write 'help' :)");
	}

	Console::~Console() {
		for (pair<string, Command*> i : arr)
			delete i.second;
	}

	void Console::updata_back_line(int change) {
		back_line = min((int)text.size() - 1, max(back_line - change, 0));
	}

	string Console::get_text(string str, int size_x, bool left) {
		string ans = str;
		while (size_x < MeasureText(ans.c_str(), font_size))
			if (left)
				ans.pop_back();
			else
				ans = ans.substr(1, ans.size() - 1);
		return ans;
	}

	void Console::add_text(string mes) {
		if (back_line != 0)
			++back_line;
		text.push_back(mes);
	}

	void Console::draw(int pos_x, int pos_y, int size_x, int size_y) {
		int size_text_y = MeasureTextEx(GetFontDefault(), "W", font_size, 2).y;
		int line_count = (int)(size_y / size_text_y - 1);

		for (int i = 0; i < line_count && text.size() > i + back_line; ++i)
			DrawText(get_text(text[text.size() - back_line - i - 1], size_x, true).c_str(), pos_x, pos_y + size_y - size_text_y * (i + 2), font_size, color);

		DrawText(get_text(input, size_x, false).c_str(), pos_x, pos_y + size_y - size_text_y, font_size, color);
	}

	void Console::logic(int pos_x, int pos_y, int size_x, int size_y) {
		int key = GetCharPressed();
		if (32 <= key && key <= 122)
			input += (char)key;

		if (IsKeyPressed(KEY_BACKSPACE) && input.size() != 1)
			input.pop_back();

		if (IsKeyPressed(KEY_ENTER)) {
			add_text(input);

			vector<string> text_input = split(input.substr(1, input.size() - 1), ' ');
			if (arr.count(text_input[0]) == 1)
				arr[text_input[0]]->run(text_input, this);
			else
				add_text("error: not command " + text_input[0]);

			input = ">";
		}

		Vector2 pos_mouse = GetMousePosition();
		if (pos_x <= pos_mouse.x && pos_mouse.x <= pos_x + size_x &&
			pos_y <= pos_mouse.y && pos_mouse.y <= pos_y + size_y)
			updata_back_line(-GetMouseWheelMove());
	}

	namespace Commands {
		bool is_int(string str) {
			for (char i : str)
				if (!isdigit(i))
					return false;
			return true;
		}

		// SetFontSize
		void SetFontSize::run(vector<string> arg, Console* console) {
			if (arg.size() == 2 && is_int(arg[1]))
				console->font_size = stoi(arg[1]);
			else
				console->add_text("error: not valid");
		}

		// Help
		void Help::run(vector<string> arg, Console* console) {
			if (arg.size() == 1) {
				for (pair<string, Command*> u : console->arr)
					console->add_text(u.first + " " + u.second->get_small_help());
			} else if (arg.size() == 2)
				if (console->arr.count(arg[1]) == 1) {
					string text = "  " + console->arr[arg[1]]->get_help();
					vector<string> arr_text = split(text, '\n');
					for (int i = 0; i < arr_text.size(); ++i)
						console->add_text(arr_text[i]);
				}
				else
					console->add_text("not command " + arg[1]);
		}
	}
}