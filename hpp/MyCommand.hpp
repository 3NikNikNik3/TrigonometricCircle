#pragma once
#include <hpp/LikeConsole.hpp>
#include <hpp/Points.hpp>

using namespace LikeConsole;

namespace MyCommands {
	bool ok_id(string, vector<Points::Solving*>&);

	class Color : public Command {
		Points::Circle* cir;
		vector<Points::Solving*>& arr_sol;

		bool ok_int(string str) { return Commands::is_int(str) && 0 <= stoi(str) && stoi(str) <= 255; }

	public:
		Color(Points::Circle* cir, vector<Points::Solving*>& arr_sol) : cir(cir), arr_sol(arr_sol) {}

		void run(vector<string> arg, Console* console);

		string get_small_help() { return "select color"; }

		string get_help() { return "color circle <r:int(0-255)> <g:int(0-255)> <b:int(0-255)>\n" +
			(string)"set circle's color to <r><b><b>\n" +
			(string)"  color points <id:int> <r:int(0-255)> <g:int(0-255)> <b:int(0-255)>\n" +
			(string)"set points color with id is <id> to <r><g><b>\n" +
			(string)"  color console <r:int(0-255)> <g:int(0-255)> <b:int(0-255)>\n" +
			(string)"set console's color to <r><g><b>"; }
	};

	class AddPoint : public Command {
		vector<Points::Solving*>& arr;

	public:
		AddPoint(vector<Points::Solving*>& arr) : arr(arr) {}

		void run(vector<string> arg, Console* console);

		string get_small_help() { return "add points"; }

		string get_help() { return "add <soling:string>\nadd <soling>"; }
	};

	class List : public Command {
		vector<Points::Solving*>& arr;

	public:
		List(vector<Points::Solving*>& arr) : arr(arr) {}

		void run(vector<string> arg, Console* console);

		string get_small_help() { return "get list points or solving"; }

		string get_help() { return "list points\nget list points ._.\n  list solving\nget list solving with id"; }
	};

	class Delete : public Command {
		vector<Points::Solving*>& arr;

	public:
		Delete(vector<Points::Solving*>& arr) : arr(arr) {}

		void run(vector<string> arg, Console* console);

		string get_small_help() { return "delete solving"; }

		string get_help() { return "delete <id:int>\ndelete solving with id = <id>\n  delete all\ndelete all solving ._."; }
	};

	class Hide : public Command {
		vector<Points::Solving*>& arr;

	public:
		Hide(vector<Points::Solving*>& arr) : arr(arr) {}

		void run(vector<string> arg, Console* console);

		string get_small_help() { return "hide solving"; }

		string get_help() { return "hide <id:int>\nhide solving with id = <id>"; }
	};

	class Show : public Command {
		vector<Points::Solving*>& arr;

	public:
		Show(vector<Points::Solving*>& arr) : arr(arr) {}

		void run(vector<string> arg, Console* console);

		string get_small_help() { return "show solving"; }

		string get_help() { return "show <id:int>\nhide solving with id = <id>"; }
	};
}