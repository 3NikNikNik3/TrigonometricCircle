#include <hpp/NamePoint.hpp>

#include <iostream>

namespace NamePoint {
	std::string to_norm_string(float a) {
		if ((int)a == a)
			return std::to_string((int)a);
		return std::to_string(a);
	}

	// Ans
	const int Ans::first_font = 24;

	void Ans::draw(Vector2 pos, int count) {
		Vector2 size = get_size(first_font);

		if (size.x == 0 && size.y == 0) {
			if (count != 0) {
				Vector2 size_text = MeasureTextEx(GetFontDefault(), (to_norm_string(count * 2) + "*pi*k").c_str(), first_font, 2);
				size = { size_text.x, size_text.y };
			}
			else {
				size = MeasureTextEx(GetFontDefault(), "0", first_font, 2);
			}

			pos = { pos.x - size.x - 3, pos.y - size.y - 3 };

			DrawRectangle(pos.x - 3, pos.y - 3, size.x + 6, size.y + 6, WHITE);

			if (count != 0) {
				Vector2 size_text = MeasureTextEx(GetFontDefault(), (to_norm_string(count * 2) + "*pi*k").c_str(), first_font, 2);
				DrawText((to_norm_string(count * 2) + "*pi*k").c_str(), pos.x, pos.y + size.y / 2 - size_text.y / 2, first_font, BLACK);
			}
			else
				draw_real(pos, first_font);
		}
		else {
			if (count != 0) {
				Vector2 size_text = MeasureTextEx(GetFontDefault(), (" + " + to_norm_string(count * 2) + "*pi*k").c_str(), first_font, 2);
				size = { size.x + size_text.x, std::max(size.y, size_text.y) };
			}

			pos = { pos.x - size.x - 3, pos.y - size.y - 3 };

			DrawRectangle(pos.x - 3, pos.y - 3, size.x + 6, size.y + 6, WHITE);

			draw_real(pos, first_font);

			if (count != 0) {
				Vector2 size_text = MeasureTextEx(GetFontDefault(), (" + " + to_norm_string(count * 2) + "*pi*k").c_str(), first_font, 2);
				DrawText((" + " + to_norm_string(count * 2) + "*pi*k").c_str(), pos.x + get_size(first_font).x, pos.y + size.y / 2 - size_text.y / 2, first_font, BLACK);
			}
		}
	}

	Vector2 Ans::get_size(int font) {
		if (size.x == 0 || size.y == 0) {
			size = get_size_real(font);
		}
		return size;
	}

	std::string Ans::get_str(int count) {
		if (name == "") {
			name = get_str_real();
			if (count != 0)
				name += " + " + to_norm_string(count * 2) + "*pi*k";
		}
		return name;
	}

	// AnsNumber
	void AnsNumber::draw_real(Vector2 pos, int font) {
		if (!with_pi)
			DrawText(to_norm_string(number).c_str(), pos.x, pos.y, font, BLACK);
		else if (number == 1)
			DrawText("pi", pos.x, pos.y, font, BLACK);
		else if (number != 0)
			DrawText((to_norm_string(number) + "*pi").c_str(), pos.x, pos.y, font, BLACK);
	}

	std::string AnsNumber::get_str_real() {
		if (!with_pi)
			return to_norm_string(number);
		else if (number == 1)
			return "pi";
		else if (number != 0)
			return to_norm_string(number) + "*pi";
		else
			return "";
	}

	Vector2 AnsNumber::get_size_real(int font) {
		if (!with_pi)
			return MeasureTextEx(GetFontDefault(), to_norm_string(number).c_str(), font, 2);
		else if (number == 1)
			return MeasureTextEx(GetFontDefault(), "pi", font, 2);
		else if (number != 0)
			return MeasureTextEx(GetFontDefault(), (to_norm_string(number) + "*pi").c_str(), font, 2);
		else
			return { 0, 0 };
	}

	// AnsDouble
	void AnsDouble::draw_real(Vector2 pos, int font) {
		Vector2 size = get_size(font), size_f = f->get_size(font), size_s = s->get_size(font);
		Vector2 size_oper = MeasureTextEx(GetFontDefault(), get_str_oper().c_str(), font, 2), size_char = MeasureTextEx(GetFontDefault(), "(", font, 2);

		if (with_pi) {
			DrawText("(", pos.x, pos.y + size.y / 2 - size_oper.y / 2, font, BLACK);
			pos.x += size_oper.x;
		}
		
		if (f->get_preo() < get_preo()) {
			DrawText("(", pos.x, pos.y + size.y / 2 - size_oper.y / 2, font, BLACK);
			pos.x += size_oper.x;
		}
		f->draw_real({ pos.x, pos.y + size.y / 2 - size_f.y / 2 }, font);
		pos.x += size_f.x;
		if (f->get_preo() < get_preo()) {
			DrawText(")", pos.x, pos.y + size.y / 2 - size_oper.y / 2, font, BLACK);
			pos.x += size_oper.x;
		}

		DrawText(get_str_oper().c_str(), pos.x, pos.y + size.y / 2 - size_oper.y / 2, font, BLACK);
		pos.x += size_oper.x;

		if (s->get_preo() <= get_preo()) {
			DrawText("(", pos.x, pos.y + size.y / 2 - size_oper.y / 2, font, BLACK);
			pos.x += size_oper.x;
		}
		s->draw_real({ pos.x, pos.y + size.y / 2 - size_s.y / 2 }, font);
		pos.x += size_s.x;
		if (s->get_preo() <= get_preo()) {
			DrawText(")", pos.x, pos.y + size.y / 2 - size_oper.y / 2, font, BLACK);
			pos.x += size_oper.x;
		}

		if (with_pi) {
			DrawText(")*pi", pos.x, pos.y + size.y / 2 - MeasureTextEx(GetFontDefault(), ")*pi", font, 2).y / 2, font, BLACK);
		}
	}

	std::string AnsDouble::get_str_real() {
		std::string ans = "";
		if (with_pi)
			ans += "(";

		if (f->get_preo() < get_preo())
			ans += "(" + f->get_str_real() + ")";
		else
			ans += f->get_str_real();

		ans += get_str_oper();

		if (s->get_preo() <= get_preo())
			ans += "(" + s->get_str_real() + ")";
		else
			ans += s->get_str_real();

		if (with_pi)
			ans += ")*pi";

		return ans;
	}

	Vector2 AnsDouble::get_size_real(int font) {
		Vector2 size_oper = MeasureTextEx(GetFontDefault(), get_str_oper().c_str(), font, 2), size_f = f->get_size(font), size_s = s->get_size(font);

		std::string add = "";
		if (with_pi)
			add += "()*pi";
		if (f->get_preo() < get_preo())
			add += "()";
		if (s->get_preo() <= get_preo())
			add += "()";
		int add_x = MeasureText(add.c_str(), font);

		return { size_f.x + size_oper.x + size_s.x + add_x, std::max(std::max(size_f.y, size_s.y), size_oper.y) };
	}

	AnsDouble::~AnsDouble() {
		delete f, s;
	}

	// AnsDiv
	void AnsDiv::draw_real(Vector2 pos, int font) {
		Vector2 size_f = f->get_size(font), size_s = s->get_size(font);
		int size_x = std::max(size_f.x, size_s.x);

		f->draw_real({ pos.x + size_x / 2 - size_f.x / 2, pos.y }, font);

		DrawLineEx({ pos.x, pos.y + size_f.y + 2 }, { pos.x + size_x, pos.y + size_f.y + 2 }, 1, BLACK);

		s->draw_real({ pos.x + size_x / 2 - size_s.x / 2, pos.y + size_f.y + 3 }, font);

		if (with_pi)
			DrawText("*pi", pos.x + size_x, pos.y + (size_f.y + size_s.y + 3) / 2 - MeasureTextEx(GetFontDefault(), "*pi", font, 2).y / 2, font, BLACK);
	}

	Vector2 AnsDiv::get_size_real(int font) {
		Vector2 size_f = f->get_size(font), size_s = s->get_size(font);

		Vector2 ans = { std::max(size_f.x, size_s.x) + 2, size_f.y + size_s.y + 3 };

		if (with_pi)
			ans.x += MeasureText("*pi", font);

		return ans;
	}

	std::string AnsDiv::get_str_real() {
		std::string ans = "";
		if (with_pi)
			ans += "(";

		if (f->get_preo() < get_preo())
			ans += "(" + f->get_str_real() + ")";
		else
			ans += f->get_str_real();

		ans += "/";

		if (s->get_preo() <= get_preo())
			ans += "(" + s->get_str_real() + ")";
		else
			ans += s->get_str_real();

		if (with_pi)
			ans += ")*pi";

		return ans;
	}

	AnsDiv::~AnsDiv() {
		delete f, s;
	}

	// AnsExp
	void AnsExp::draw_real(Vector2 pos, int font) {
		if (f->get_type() != "num") {
			DrawText("(", pos.x, pos.y + s->get_size(font / 2).y / 2 + f->get_size(font).y / 2 - MeasureTextEx(GetFontDefault(), "(", font, 2).y / 2, font, BLACK);
			pos.x += MeasureText("(", font);
		}
		f->draw_real({ pos.x, pos.y + s->get_size(font / 2).y / 2 }, font);
		if (f->get_type() != "num") {
			DrawText(")", pos.x + f->get_size(font).x, pos.y + s->get_size(font / 2).y / 2 + f->get_size(font).y / 2 - MeasureTextEx(GetFontDefault(), ")", font, 2).y / 2, font, BLACK);
			pos.x += MeasureText(")", font);
		}
		s->draw_real({ pos.x + f->get_size(font).x, pos.y }, font / 2);
		if (with_pi)
			DrawText("*pi", pos.x + f->get_size(font).x + s->get_size(font / 2).x, pos.y + get_size(font).y / 2 - MeasureTextEx(GetFontDefault(), "*pi", font, 2).y, font, BLACK);
	}

	std::string AnsExp::get_str_real() {
		std::string ans = "";
		if (f->get_type() != "num")
			ans = "(" + f->get_str_real() + ")";
		else
			ans = f->get_str_real();

		ans += "^";

		if (s->get_type() != "num")
			ans += "(" + s->get_str_real() + ")";
		else
			ans += s->get_str_real();

		if (with_pi)
			ans += "*pi";

		return ans;
	}

	Vector2 AnsExp::get_size_real(int font) {
		Vector2 size_f = f->get_size(font), size_s = s->get_size(font / 2);
		Vector2 ans = { size_f.x + size_s.x, size_f.y + size_s.y / 2 };
		
		if (with_pi)
			ans.x += MeasureText("*pi", font);

		if (f->get_type() != "num")
			ans.x += MeasureText("()", font);

		return ans;
	}

	AnsExp::~AnsExp() {
		delete f, s;
	}

	// AnsMathRoot
	void AnsMathRoot::draw_real(Vector2 pos, int font) {
		pos.y += 2;

		s->draw_real({ pos.x, pos.y + (get_size(font).y - 2) / 2 - s->get_size(font / 2).y}, font / 2);
		pos.x += s->get_size(font / 2).x;

		int size_char = MeasureText("O", font);

		DrawLineEx({ pos.x, pos.y + get_size(font).y / 2}, {pos.x + size_char / 2, pos.y + get_size(font).y }, 2, BLACK);
		DrawLineEx({ pos.x + size_char / 2, pos.y + get_size(font).y }, { pos.x + size_char, pos.y - 2 }, 2, BLACK);
		pos.x += size_char;
		DrawLineEx({ pos.x, pos.y - 2 }, { pos.x + f->get_size(font).x, pos.y - 2 }, 2, BLACK);

		f->draw_real({ pos.x, pos.y + (get_size(font).y - 2) / 2 - f->get_size(font).y / 2 }, font);

		if (with_pi) {
			DrawText("*pi", pos.x + f->get_size(font).x, pos.y + get_size(font).y / 2 - MeasureTextEx(GetFontDefault(), "*pi", font, 2).y, font, BLACK);
		}
	}

	std::string AnsMathRoot::get_str_real() {
		return s->get_str_real() + "root(" + f->get_str_real() + ")" + (with_pi ? "*pi" : "");
	}

	Vector2 AnsMathRoot::get_size_real(int font) {
		Vector2 ans = { f->get_size(font).x + s->get_size(font / 2).x + MeasureText("O", font), std::max(f->get_size(font).y, s->get_size(font / 2).y) + 2 };

		if (with_pi)
			ans.x += MeasureText("*pi", font);

		return ans;
	}

	AnsMathRoot::~AnsMathRoot() {
		delete f, s;
	}

	// AnsSqrt
	void AnsSqrt::draw_real(Vector2 pos, int font)
	{
		pos.y += 2;

		int size_char = MeasureText("O", font);

		DrawLineEx({ pos.x, pos.y + get_size(font).y / 2 }, { pos.x + size_char / 2, pos.y + get_size(font).y }, 2, BLACK);
		DrawLineEx({ pos.x + size_char / 2, pos.y + get_size(font).y }, { pos.x + size_char, pos.y - 2 }, 2, BLACK);
		pos.x += size_char;
		DrawLineEx({ pos.x, pos.y - 2 }, { pos.x + oper->get_size(font).x, pos.y - 2 }, 2, BLACK);

		oper->draw_real({ pos.x, pos.y + (get_size(font).y - 2) / 2 - oper->get_size(font).y / 2 }, font);

		if (with_pi) {
			DrawText("*pi", pos.x + oper->get_size(font).x, pos.y + get_size(font).y / 2 - MeasureTextEx(GetFontDefault(), "*pi", font, 2).y, font, BLACK);
		}
	}

	std::string AnsSqrt::get_str_real() {
		return "sqrt(" + oper->get_str_real() + ")" + (with_pi ? "*pi" : "");
	}

	Vector2 AnsSqrt::get_size_real(int font) {
		Vector2 ans = { oper->get_size(font).x + MeasureText("O", font), oper->get_size(font).y + 2 };

		if (with_pi)
			ans.x += MeasureText("*pi", font);

		return ans;
	}

	AnsSqrt::~AnsSqrt() {
		delete oper;
	}

	// AnsArc
	void AnsArc::draw_real(Vector2 pos, int font) {
		DrawText((name + "(").c_str(), pos.x, pos.y + get_size(font).y / 2 - MeasureTextEx(GetFontDefault(), (name + "(").c_str(), font, 2).y / 2, font, BLACK);

		oper->draw_real({ pos.x + MeasureText((name + "(").c_str(), font), pos.y + get_size(font).y / 2 - oper->get_size(font).y / 2 }, font);

		DrawText(")", pos.x + MeasureText((name + "(").c_str(), font) + oper->get_size(font).x, pos.y + get_size(font).y / 2 - MeasureTextEx(GetFontDefault(), ")", font, 2).y / 2, font, BLACK);

		if (with_pi) {
			DrawText("*pi", pos.x + MeasureText((name + "()").c_str(), font) + oper->get_size(font).x, pos.y + get_size(font).y / 2 - MeasureTextEx(GetFontDefault(), "*pi", font, 2).y, font, BLACK);
		}
	}

	std::string AnsArc::get_str_real() {
		return name + "(" + oper->get_str_real() + ")";
	}

	Vector2 AnsArc::get_size_real(int font) {
		Vector2 ans = { MeasureText((name + "()").c_str(), font) + oper->get_size(font).x, oper->get_size(font).y };

		if (with_pi)
			ans.x += MeasureText("*pi", font);

		return ans;
	}

	AnsArc::~AnsArc() {
		delete oper;
	}
}