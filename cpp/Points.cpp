#include <hpp/Points.hpp>
//#include <iostream>

namespace Points {
	// Point
	Point::Point(double a, Ans* name, int count) : name(name), count(count) {
		pos = { (float)cos(a), -(float)sin(a) };
	}

	Point::~Point() {
		delete name;
	}

	void Point::draw(Color color) {
		Vector2 size = { GetScreenWidth(), GetScreenHeight() }, pos_mouse = GetMousePosition();
		float scale = min(size.x, size.y) * 0.45;
		Vector2 real_pos = { pos.x * scale + size.x / 2, pos.y * scale + size.y / 2 };

		DrawCircle(real_pos.x, real_pos.y, 12, color);

		if (sqrt(pow(pos_mouse.x - real_pos.x, 2) + pow(pos_mouse.y - real_pos.y, 2)) <= 12)
			name->draw(real_pos, count);
	}

	string Point::get_name() {
		string ans = name->get_str(count);
		if (count != 0)
			ans += " + " + to_string(count * 2) + "*pi";
		return ans;
	}
	
	// Solving
	float const Solving::pi = 3.14159265358979323846;

	double Solving::mod(double a) {
		while (a < 0)
			a += pi * 2;

		while (a >= pi * 2)
			a -= pi * 2;
		return a;
	}

	int Solving::mod_count(double a) {
		int ans = 0;
		while (a < 0) {
			--ans;
			a += pi * 2;
		}

		while (a >= pi * 2) {
			++ans;
			a -= pi * 2;
		}

		return ans;
	}

	Solving::Solving(Math::MainOperator* oper) {
		this->oper = oper;
		if (oper->get_count_per() == 0)
			points.push_back(new Point(oper->get_num({}), oper->get_ans({}), 0));
		else {
			vector<double> arr = { mod(oper->get_num({1})), mod(oper->get_num({2})) };
			for (int i = 2; abs(arr[0] - arr[arr.size() - 1]) >= 0.0001 && i < 20; ++i)
				arr.push_back(mod(oper->get_num({ i + 1 })));

			if (arr.size() != 20) {
				int count = abs(mod_count(oper->get_num({ 1 })) - mod_count(oper->get_num({ (int)arr.size() })));
				points.push_back(new Point(oper->get_num({0}), oper->get_ans({0}), count));
				for (int i = 0; i < arr.size() - 2; ++i)
					points.push_back(new Point(arr[i], oper->get_ans({ i + 1 }), count));
			}
			else {
				delete oper;
				this->oper = nullptr;
			}
		}
	}

	Solving::~Solving() {
		delete_me();
	}
	
	void Solving::delete_me() {
		delete oper;
		oper = nullptr;
		for (int i = 0; i < points.size(); ++i)
			delete points[i];
		points.clear();
	}
	
	void Solving::draw() {
		if (visible)
			for (int i = 0; i < points.size(); ++i)
				points[i]->draw(color);
	}
	
	// Circle
	void Circle::draw() {
		DrawCircleLines(GetScreenWidth() / 2, GetScreenHeight() / 2, min(GetScreenWidth(), GetScreenHeight()) * 0.45, color);
	}
}