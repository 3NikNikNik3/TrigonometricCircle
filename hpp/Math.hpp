#pragma once
#include <cmath>
#include <vector>
#include <string>

#include <hpp/NamePoint.hpp>

using namespace std;
using namespace NamePoint;

namespace Math {
	class Operator {
	public:
		virtual bool optimize(Operator*&) = 0;

		virtual double get_num(vector<int>) = 0;

		virtual Ans* get_ans(vector<int>) = 0;

		virtual string get_str() = 0;

		virtual string get_name() { return "null"; }

		virtual char get_preo() = 0;

		static void optimize_it(Operator*&);
	};

	class OperatorNum : public Operator {
		float num;

	public:
		OperatorNum(float num);

		bool optimize(Operator*&);

		double get_num(vector<int>);

		Ans* get_ans(vector<int>);

		string get_str();

		string get_name() { return "num"; }

		char get_preo() { return 127; }

		void set_num(int num) { this->num = num; }
	};

	class OperatorPer : public Operator {
		int id;
		string name;

	public:
		OperatorPer(int, string);

		bool optimize(Operator*&);

		double get_num(vector<int>);

		Ans* get_ans(vector<int>);

		string get_str();

		string get_name() { return "per"; }

		char get_preo() { return 127; }
	};

	class OperatorPi : public Operator {
	public:
		static const double pi;

		bool optimize(Operator*& arr) { return false; }

		double get_num(vector<int> arr) { return pi; }

		Ans* get_ans(vector<int>) { return new AnsNumber(1, true); }

		string get_str() { return "pi"; }

		string get_name() { return "pi"; }

		char get_preo() { return 127; }
	};

	class OperatorDouble: public Operator {
	public:
		Operator *first, *second;

		OperatorDouble(Operator* first, Operator* second) : first(first), second(second) {}

		~OperatorDouble();

		string get_str();

		virtual string get_str_oper() = 0;
	};

	class OperatorSum : public OperatorDouble {
	public:
		OperatorSum(Operator* q, Operator* w) : OperatorDouble(q, w) {}

		bool optimize(Operator*&);

		double get_num(vector<int>);

		Ans* get_ans(vector<int>);

		string get_str_oper() { return "+"; }

		string get_name() { return "+"; }

		char get_preo() { return 1; }
	};

	class OperatorDiff : public OperatorDouble {
	public:
		OperatorDiff(Operator* q, Operator* w) : OperatorDouble(q, w) {}

		bool optimize(Operator*&);

		double get_num(vector<int>);

		Ans* get_ans(vector<int>);

		string get_str_oper() { return "-"; }

		string get_name() { return "-"; }

		char get_preo() { return 1; }
	};

	class OperatorMul : public OperatorDouble {
	public:
		OperatorMul(Operator* q, Operator* w) : OperatorDouble(q, w) {}

		bool optimize(Operator*&);

		double get_num(vector<int>);

		Ans* get_ans(vector<int>);

		string get_str_oper() { return "*"; }

		string get_name() { return "*"; }

		char get_preo() { return 2; }
	};

	class OperatorDiv : public OperatorDouble {
	public:
		OperatorDiv(Operator* q, Operator* w) : OperatorDouble(q, w) {}

		bool optimize(Operator*&);

		double get_num(vector<int>);

		Ans* get_ans(vector<int>);

		string get_str_oper() { return "/"; }

		string get_name() { return "/"; }

		static bool is_int(double a);

		static int nod(int a, int b);

		char get_preo() { return 2; }
	};

	class OperatorExp : public OperatorDouble {
	public:
		OperatorExp(Operator* q, Operator* w) : OperatorDouble(q, w) {}

		bool optimize(Operator*&);

		double get_num(vector<int>);

		Ans* get_ans(vector<int>);

		string get_str_oper() { return "^"; }

		string get_name() { return "^"; }

		char get_preo() { return 3; }
	};

	class OperatorMathRoot : public Operator {
		Operator *first, *second;

	public:
		OperatorMathRoot(Operator* first, Operator* second);

		~OperatorMathRoot();

		bool optimize(Operator*&);

		double get_num(vector<int>);

		Ans* get_ans(vector<int>);

		string get_str();

		string get_name() { return "root"; }

		char get_preo() { return 3; }
	};

	class OperatorSqrt : public Operator {
		Operator* oper;

	public:
		OperatorSqrt(Operator* oper);

		~OperatorSqrt();

		bool optimize(Operator*&);

		double get_num(vector<int>);

		Ans* get_ans(vector<int>);

		string get_str();

		string get_name() { return "sqrt"; }

		char get_preo() { return 3; }
	};

	class OperatorArc : public Operator {
	public:
		Operator* oper;

		OperatorArc(Operator* q) : oper(q) {}

		~OperatorArc();

		bool optimize(Operator*&) { return false; }

		string get_str();

		virtual string get_str_oper() = 0;

		char get_preo() { return 4; }
	};

	class OperatorAsin : public OperatorArc {
	public:
		OperatorAsin(Operator *q) : OperatorArc(q) {}

		double get_num(vector<int>);

		Ans* get_ans(vector<int>);

		string get_str_oper() { return "arcsin"; }

		string get_name() { return "asin"; }
	};

	class OperatorAcos : public OperatorArc {
	public:
		OperatorAcos(Operator* q) : OperatorArc(q) {}

		double get_num(vector<int>);

		Ans* get_ans(vector<int>);

		string get_str_oper() { return "arccos"; }

		string get_name() { return "acos"; }
	};

	class OperatorAtan : public OperatorArc {
	public:
		OperatorAtan(Operator* q) : OperatorArc(q) {}

		double get_num(vector<int>);

		Ans* get_ans(vector<int>);

		string get_str_oper() { return "arctg"; }

		string get_name() { return "atan"; }
	};

	class OperatorActg : public OperatorArc {
	public:
		OperatorActg(Operator* q) : OperatorArc(q) {}

		double get_num(vector<int>);

		Ans* get_ans(vector<int>);

		string get_str_oper() { return "arcctg"; }

		string get_name() { return "acrg"; }
	};

	class MainOperator {
		Operator* get_oper(string str);

		int get_id_per(string name);

		static bool delete_this(string str);

		Operator* oper;

		vector<string> arr_per;

	public:
		MainOperator(string str);

		~MainOperator();

		void optimize();

		double get_num(vector<int> arr) { return oper->get_num(arr); }

		Ans* get_ans(vector<int> arr) { return oper->get_ans(arr); }

		string get_str() { return oper->get_str(); }

		bool is_ok() { return oper != nullptr; }

		int get_count_per() { return arr_per.size(); }
	};
}
