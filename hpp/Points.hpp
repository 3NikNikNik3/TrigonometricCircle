#pragma once

#include <hpp/Math.hpp>

namespace Points {
	class Point {
		Vector2 pos;
		Ans *name;
		int count;

	public:
		Point(double a, Ans* name, int count);

		~Point();

		void draw(Color color);

		string get_name();
	};

	class Solving {
		Math::MainOperator* oper;
		vector<Point*> points;

		Color color = BLUE;
		bool visible = true;

		static float const pi;

		static double mod(double a);

		static int mod_count(double a);

	public:
		Solving(Math::MainOperator* oper);

		~Solving();

		void draw();

		void hide() { visible = false; }

		void show() { visible = true; }

		void set_color(Color color) { this->color = color; }

		bool is_ok() { return oper != nullptr; }

		void delete_me();

		int get_count_points() { return points.size(); }

		Point* get_point(int id) { return points[id]; }

		string get_name() { return oper->get_str(); }
	};

	class Circle {
		Color color = GREEN;

	public:
		void draw();

		void set_color(Color color) { this->color = color; }
	};
}