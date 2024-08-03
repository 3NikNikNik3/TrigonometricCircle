#pragma once

#include <raylib.h>
#include <string>

namespace NamePoint {
	std::string to_norm_string(float a);

	class Ans {
	private:
		Vector2 size = { 0, 0 };
		std::string name = "";

	protected:
		Ans(bool with_pi) : with_pi(with_pi){}

	public:
		static const int first_font;

		bool with_pi;

		void draw(Vector2 pos, int count);

		std::string get_str(int count);

		Vector2 get_size(int font);

		virtual void draw_real(Vector2 pos, int font) = 0;

		virtual std::string get_str_real() = 0;

		virtual Vector2 get_size_real(int font) = 0;

		virtual std::string get_type() { return "none"; }

		virtual char get_preo() = 0;
	};

	class AnsNumber : public Ans {
		void draw_real(Vector2 pos, int font);

		std::string get_str_real();

		Vector2 get_size_real(int font);

	public:
		float number;

		AnsNumber(float number, bool is_pi = false) : Ans(is_pi), number(number) {}

		std::string get_type() { return "num"; }

		char get_preo() { return 127; }
	};

	class AnsDouble : public Ans{
	protected:
		void draw_real(Vector2 pos, int font);

		std::string get_str_real();

		Vector2 get_size_real(int font);

		virtual std::string get_str_oper() = 0;

	public:
		Ans *f, *s;

		AnsDouble(Ans *f, Ans *s, bool with_pi = false) : Ans(with_pi), f(f), s(s) {}

		~AnsDouble();
	};

	class AnsSum : public AnsDouble {
	public:
		AnsSum(Ans* f, Ans* s, bool with_pi = false) : AnsDouble(f, s, with_pi) {}

		std::string get_type() { return "+"; }

		std::string get_str_oper() { return "+"; }

		char get_preo() { return 0; }
	};

	class AnsDiff : public AnsDouble {
	public:
		AnsDiff(Ans* f, Ans* s, bool with_pi = false) : AnsDouble(f, s, with_pi) {}

		std::string get_type() { return "-"; }

		std::string get_str_oper() { return "-"; }

		char get_preo() { return 0; }
	};

	class AnsMul : public AnsDouble {
	public:
		AnsMul(Ans* f, Ans* s, bool with_pi = false) : AnsDouble(f, s, with_pi) {}

		std::string get_type() { return "*"; }

		std::string get_str_oper() { return "*"; }

		char get_preo() { return 1; }
	};

	class AnsDiv : public Ans {
		void draw_real(Vector2 pos, int font);

		std::string get_str_real();

		Vector2 get_size_real(int font);

	public:
		Ans *f, *s;

		AnsDiv(Ans *f, Ans *s, bool is_pi = false) : Ans(is_pi), f(f), s(s) {}

		~AnsDiv();

		std::string get_type() { return "/"; }

		char get_preo() { return 1; }
	};

	class AnsExp : public Ans {
		void draw_real(Vector2 pos, int font);

		std::string get_str_real();

		Vector2 get_size_real(int font);

	public:
		Ans *f, *s;

		AnsExp(Ans* f, Ans* s) : Ans(false), f(f), s(s) {}

		~AnsExp();

		std::string get_type() { return "^"; }

		char get_preo() { return 2; }
	};

	class AnsMathRoot : public Ans {
		void draw_real(Vector2 pos, int font);

		std::string get_str_real();

		Vector2 get_size_real(int font);

	public:
		Ans* f, * s;

		AnsMathRoot(Ans* f, Ans* s) : Ans(false), f(f), s(s) {}

		~AnsMathRoot();

		std::string get_type() { return "root"; }

		char get_preo() { return 2; }
	};

	class AnsSqrt : public Ans {
		void draw_real(Vector2 pos, int font);

		std::string get_str_real();

		Vector2 get_size_real(int font);

	public:
		Ans* oper;

		AnsSqrt(Ans *oper) : Ans(false), oper(oper) {}

		~AnsSqrt();

		std::string get_type() { return "sqrt"; }

		char get_preo() { return 2; }
	};

	class AnsArc : public Ans {
		void draw_real(Vector2 pos, int font);

		std::string get_str_real();

		Vector2 get_size_real(int font);

	public:
		Ans* oper;
		std::string name;

		AnsArc(Ans* oper, std::string name) : Ans(false), oper(oper), name(name) {}

		~AnsArc();

		std::string get_type() { return "arc"; }

		char get_preo() { return 3; }
	};
}