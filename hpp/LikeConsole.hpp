#pragma once

#include <vector>
#include <string>
#include <map>
#include <raylib.h>

using namespace std;

namespace LikeConsole {
	class Console;

	vector<string> split(string, char);

	class Command {
	public:
		virtual void run(vector<string> arg, Console* console) = 0;

		virtual string get_small_help() { return "none"; }

		virtual string get_help() { return "none"; }
	};

	class Console {
		vector<string> text;
		string input = ">";
		int back_line = 0;

		string get_text(string str, int size_x, bool left);

	public:
		map<string, Command*> arr;

		Color color = BLACK;

		int font_size;

		Console(map<string, Command*>, int);

		~Console();

		void updata_back_line(int change);

		void draw(int pos_x, int pos_y, int size_x, int size_y);

		void logic(int pos_x, int pos_y, int size_x, int size_y);

		void add_text(string mes);

		void clear() { text.clear(); }
	};

	namespace Commands {
		bool is_int(string);

		class Clear : public Command {
		public:
			void run(vector<string> arg, Console* console) { console->clear(); }

			string get_small_help() { return "clear this screen (text)"; }

			string get_help() { return "clear\ndelete all text (old, new text will be)"; }
		};

		class SetFontSize : public Command {
		public:
			void run(vector<string> arg, Console* console);

			string get_small_help() { return "set font's size (this text)"; }

			string get_help() { return "set_font_size <size:int>\nset font's size on size"; }
		};

		class Help : public Command {
		public:
			void run(vector<string> arg, Console* console);

			string get_small_help() { return "get help list (this)"; }

			string get_help() { return "help\nget list all commands\n  help <command:string>\nget help on <command>"; }
		};

		class Exit : public Command {
			bool& play;
		public:
			Exit(bool& play) : play(play) {}

			void run(vector<string> arg, Console* console) { play = false; }

			string get_small_help() { return "quit"; }

			string get_help() { return "exit\nquit ._."; }
		};
	}
}