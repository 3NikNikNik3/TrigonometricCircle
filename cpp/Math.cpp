#include <hpp/Math.hpp>

namespace Math {
	// Operator
	void Operator::optimize_it(Operator*& oper) {
		Operator* q;
		if (oper->optimize(q)) {
			delete oper;
			oper = q;
		}
	}

	// OperatorNum
	OperatorNum::OperatorNum(float num) : num(num) {}

	bool OperatorNum::optimize(Operator*& q) {
		return false;
	}

	double OperatorNum::get_num(vector<int> arr) {
		return num;
	}

	Ans* OperatorNum::get_ans(vector<int> arr) {
		return new AnsNumber(num);
	}

	string OperatorNum::get_str() {
		return to_norm_string(num);
	}

	// OperatorPer
	OperatorPer::OperatorPer(int id, string name) : id(id), name(name) {}

	bool OperatorPer::optimize(Operator*& ans) {
		return false;
	}

	double OperatorPer::get_num(vector<int> arr) {
		return arr[id];
	}

	Ans* OperatorPer::get_ans(vector<int> arr) {
		return new AnsNumber(arr[id]);
	}

	string OperatorPer::get_str() {
		return name;
	}

	// OperatorPi
	const double OperatorPi::pi = 3.14159265358979323846;

	// OperatorDouble
	OperatorDouble::~OperatorDouble() {
		delete first, second;
	}

	string OperatorDouble::get_str() {
		string ans = "";

		if (first->get_preo() < get_preo())
			ans = "(" + first->get_str() + ")";
		else
			ans = first->get_str();

		ans += get_str_oper();

		if (second->get_preo() <= get_preo())
			ans += "(" + second->get_str() + ")";
		else
			ans += second->get_str();

		return ans;
	}

	// OperatorSum
	bool OperatorSum::optimize(Operator*& ans) {
		Operator::optimize_it(first);
		Operator::optimize_it(second);

		if (first->get_name() == "num" && second->get_name() == "num") {
			ans = new OperatorNum(first->get_num(vector<int>()) + second->get_num(vector<int>()));
			return true;
		}
		return false;
	}

	double OperatorSum::get_num(vector<int> arr) {
		return first->get_num(arr) + second->get_num(arr);
	}

	Ans* OperatorSum::get_ans(vector<int> arr) {
		Ans *f = first->get_ans(arr), *s = second->get_ans(arr);

		if (f->get_type() == "num" && s->get_type() == "num" && f->with_pi == s->with_pi) {
			Ans *ans = new AnsNumber(((AnsNumber*)f)->number + ((AnsNumber*)s)->number, f->with_pi);
			delete f, s;
			return ans;
		}
		else if (f->get_type() == "/" && s->get_type() == "/" && f->with_pi == s->with_pi) {
			if (((AnsDiv*)f)->s->get_type() == "num" && ((AnsDiv*)s)->s->get_type() == "num" && OperatorDiv::is_int(((AnsNumber*)((AnsDiv*)f)->s)->number) && OperatorDiv::is_int(((AnsNumber*)((AnsDiv*)s)->s)->number)) {
				int nod_ = OperatorDiv::nod(((AnsNumber*)((AnsDiv*)f)->s)->number, ((AnsNumber*)((AnsDiv*)s)->s)->number);

				if (((AnsNumber*)((AnsDiv*)s)->s)->number / nod_ != 1) {
					if (((AnsDiv*)f)->f->get_type() == "num")
						((AnsNumber*)((AnsDiv*)f)->f)->number *= ((AnsNumber*)((AnsDiv*)s)->s)->number / nod_;
					else
						((AnsDiv*)f)->f = new AnsMul(((AnsDiv*)f)->f, new AnsNumber(((AnsNumber*)((AnsDiv*)s)->s)->number / nod_));

					if (((AnsDiv*)s)->f->get_type() == "num")
						((AnsNumber*)((AnsDiv*)s)->f)->number *= ((AnsNumber*)((AnsDiv*)f)->s)->number / nod_;
					else
						((AnsDiv*)s)->f = new AnsMul(((AnsDiv*)s)->f, new AnsNumber(((AnsNumber*)((AnsDiv*)f)->s)->number / nod_));
				}

				Ans *ans_f;
				if (((AnsDiv*)f)->f->get_type() == "num" && ((AnsDiv*)s)->f->get_type() == "num" && ((AnsDiv*)f)->f->with_pi == ((AnsDiv*)s)->f->with_pi) {
					ans_f = new AnsNumber(((AnsNumber*)((AnsDiv*)f)->f)->number + ((AnsNumber*)((AnsDiv*)s)->f)->number, ((AnsDiv*)f)->f->with_pi || f->with_pi);
					delete ((AnsDiv*)f)->f, ((AnsDiv*)s)->f;
				}
				else {
					ans_f = new AnsSum(((AnsDiv*)f)->f, ((AnsDiv*)s)->f, ((AnsDiv*)f)->f->with_pi && ((AnsDiv*)s)->f->with_pi || (f->with_pi && s->with_pi));
					if (ans_f->with_pi)
						((AnsDiv*)f)->f->with_pi = ((AnsDiv*)s)->f->with_pi = false;
				}

				int ans_number = ((AnsNumber*)((AnsDiv*)f)->s)->number * ((AnsNumber*)((AnsDiv*)s)->s)->number / nod_;
				delete ((AnsDiv*)f)->s, ((AnsDiv*)s)->s;

				bool with_pi = ans_f->with_pi;
				if (with_pi)
					ans_f->with_pi = false;

				return new AnsDiv(ans_f, new AnsNumber(ans_number), with_pi);
			}
		}
		else if (f->get_type() == "num" && ((AnsNumber*)f)->number == 0) {
			delete f;
			return s;
		}
		else if (s->get_type() == "num" && ((AnsNumber*)s)->number == 0) {
			delete s;
			return f;
		}

		bool with_pi = f->with_pi && s->with_pi;
		if (with_pi)
			f->with_pi = s->with_pi = false;
		return new AnsSum(f, s, with_pi);
	}

	// OperatorDiff
	bool OperatorDiff::optimize(Operator*& ans) {
		Operator::optimize_it(first);
		Operator::optimize_it(second);

		if (first->get_name() == "num" && second->get_name() == "num") {
			ans = new OperatorNum(first->get_num(vector<int>()) - second->get_num(vector<int>()));
			return true;
		}
		return false;
	}

	double OperatorDiff::get_num(vector<int> arr) {
		return first->get_num(arr) - second->get_num(arr);
	}

	Ans* OperatorDiff::get_ans(vector<int> arr) {
		Ans* f = first->get_ans(arr), * s = second->get_ans(arr);

		if (f->get_type() == "num" && s->get_type() == "num" && f->with_pi == s->with_pi) {
			Ans* ans = new AnsNumber(((AnsNumber*)f)->number - ((AnsNumber*)s)->number, f->with_pi);
			delete f, s;
			return ans;
		}
		else if (f->get_type() == "/" && s->get_type() == "/" && f->with_pi == s->with_pi) {
			if (((AnsDiv*)f)->s->get_type() == "num" && ((AnsDiv*)s)->s->get_type() == "num" && OperatorDiv::is_int(((AnsNumber*)((AnsDiv*)f)->s)->number) && OperatorDiv::is_int(((AnsNumber*)((AnsDiv*)s)->s)->number)) {
				int nod_ = OperatorDiv::nod(((AnsNumber*)((AnsDiv*)f)->s)->number, ((AnsNumber*)((AnsDiv*)s)->s)->number);

				if (((AnsNumber*)((AnsDiv*)s)->s)->number / nod_ != 1) {
					if (((AnsDiv*)f)->f->get_type() == "num")
						((AnsNumber*)((AnsDiv*)f)->f)->number *= ((AnsNumber*)((AnsDiv*)s)->s)->number / nod_;
					else
						((AnsDiv*)f)->f = new AnsMul(((AnsDiv*)f)->f, new AnsNumber(((AnsNumber*)((AnsDiv*)s)->s)->number / nod_));

					if (((AnsDiv*)s)->f->get_type() == "num")
						((AnsNumber*)((AnsDiv*)s)->f)->number *= ((AnsNumber*)((AnsDiv*)f)->s)->number / nod_;
					else
						((AnsDiv*)s)->f = new AnsMul(((AnsDiv*)s)->f, new AnsNumber(((AnsNumber*)((AnsDiv*)f)->s)->number / nod_));
				}

				Ans* ans_f;
				if (((AnsDiv*)f)->f->get_type() == "num" && ((AnsDiv*)s)->f->get_type() == "num" && ((AnsDiv*)f)->f->with_pi == ((AnsDiv*)s)->f->with_pi) {
					ans_f = new AnsNumber(((AnsNumber*)((AnsDiv*)f)->f)->number - ((AnsNumber*)((AnsDiv*)s)->f)->number, ((AnsDiv*)f)->f->with_pi || f->with_pi);
					delete ((AnsDiv*)f)->f, ((AnsDiv*)s)->f;
				}
				else {
					ans_f = new AnsDiff(((AnsDiv*)f)->f, ((AnsDiv*)s)->f, ((AnsDiv*)f)->f->with_pi && ((AnsDiv*)s)->f->with_pi || (f->with_pi && s->with_pi));
					if (ans_f->with_pi)
						((AnsDiv*)f)->f->with_pi = ((AnsDiv*)s)->f->with_pi = false;
				}

				int ans_number = ((AnsNumber*)((AnsDiv*)f)->s)->number * ((AnsNumber*)((AnsDiv*)s)->s)->number / nod_;
				delete ((AnsDiv*)f)->s, ((AnsDiv*)s)->s;

				bool with_pi = ans_f->with_pi;
				if (with_pi)
					ans_f->with_pi = false;

				return new AnsDiv(ans_f, new AnsNumber(ans_number), with_pi);
			}
		}
		else if (f->get_type() == "num" && ((AnsNumber*)f)->number == 0) {
			delete f;
			return s;
		}
		else if (s->get_type() == "num" && ((AnsNumber*)s)->number == 0) {
			delete s;
			return f;
		}

		bool with_pi = f->with_pi && s->with_pi;
		if (with_pi)
			f->with_pi = s->with_pi = false;
		return new AnsDiff(f, s, with_pi);
	}

	// OperatorMul
	bool OperatorMul::optimize(Operator*& ans) {
		Operator::optimize_it(first);
		Operator::optimize_it(second);

		if (first->get_name() == "num" && second->get_name() == "num") {
			ans = new OperatorNum(first->get_num(vector<int>()) * second->get_num(vector<int>()));
			return true;
		}
		else if (first->get_name() == "num" && first->get_num(vector<int>()) == 1) {
			ans = second;
			second = nullptr;
			return true;
		} else if (second->get_name() == "num" && second->get_num(vector<int>()) == 1) {
			ans = first;
			first = nullptr;
			return true;
		}
		return false;
	}

	double OperatorMul::get_num(vector<int> arr) {
		return first->get_num(arr) * second->get_num(arr);
	}

	Ans* OperatorMul::get_ans(vector<int> arr) {
		Ans *f = first->get_ans(arr), *s = second->get_ans(arr);

		if (f->get_type() == "num" && s->get_type() == "num" && !(f->with_pi && s->with_pi)) {
			Ans* ans = new AnsNumber(((AnsNumber*)f)->number * ((AnsNumber*)s)->number, f->with_pi || s->with_pi);
			delete f, s;
			return ans;
		}

		if (first->get_name() == "pi" && !s->with_pi) {
			delete f;
			s->with_pi = true;
			return s;
		}
		if (second->get_name() == "pi" && !f->with_pi) {
			delete s;
			f->with_pi = true;
			return f;
		}

		if (!(f->with_pi && s->with_pi))
			if (f->get_type() == "num" && ((AnsNumber*)f)->number == 1) {
				s->with_pi |= f->with_pi;
				delete f;
				return s;
			} else if (s->get_type() == "num" && ((AnsNumber*)s)->number == 1) {
				f->with_pi |= s->with_pi;
				delete s;
				return f;
			} else if (f->get_type() == "num" && ((AnsNumber*)f)->number == -1 && s->get_type() == "num") {
				s->with_pi |= f->with_pi;
				((AnsNumber*)s)->number *= -1;
				delete f;
				return s;
			}

		return new AnsMul(f, s);
	}

	// OperatorDiv
	bool OperatorDiv::is_int(double a) {
		return (int)a == a;
	}

	int OperatorDiv::nod(int a, int b) {
		if (a == 0 || b == 0)
			return a + b;
		if (a < b)
			swap(a, b);
		return nod(b, a % b);
	}

	bool OperatorDiv::optimize(Operator*& ans) {
		Operator::optimize_it(first);
		Operator::optimize_it(second);

		if (first->get_name() == "num" && second->get_name() == "num") {
			double a = first->get_num(vector<int>()), b = second->get_num(vector<int>());

			if (is_int(a / b)) {
				ans = new OperatorNum(a / b);
				return true;
			}
			while (!is_int(a) || !is_int(b)) {
				a *= 10;
				b *= 10;
			}
			int nod_ = nod((int)a, (int)b);

			((OperatorNum*)first)->set_num(a / nod_);
			((OperatorNum*)second)->set_num(b / nod_);
		}
		else if (second->get_name() == "num" && second->get_num(vector<int>()) == 1) {
			ans = first;
			first = nullptr;
			return true;
		}
		return false;
	}

	double OperatorDiv::get_num(vector<int> arr) {
		return first->get_num(arr) / second->get_num(arr);
	}

	Ans* OperatorDiv::get_ans(vector<int> arr) {
		Ans *f = first->get_ans(arr), *s = second->get_ans(arr);

		if (f->with_pi && s->with_pi)
			f->with_pi = s->with_pi = false;

		if (f->get_type() == "num" && s->get_type() == "num") {
			if (!s->with_pi) {
				float num = ((AnsNumber*)f)->number / ((AnsNumber*)s)->number;
				if ((int)num == num) {
					Ans* ans = new AnsNumber(num, f->with_pi);
					delete f, s;
					return ans;
				}
			}
			float a = ((AnsNumber*)f)->number, b = ((AnsNumber*)s)->number;
			while (!is_int(a) || !is_int(b)) {
				a *= 10;
				b *= 10;
			}
			int nod_ = nod((int)a, (int)b);

			((AnsNumber*)f)->number = a / nod_;
			((AnsNumber*)s)->number = b / nod_;
		}

		bool with_pi = f->with_pi;
		if (with_pi) {
			f->with_pi = false;
		}

		return new AnsDiv(f, s, with_pi);
	}

	// OperatorExp
	bool OperatorExp::optimize(Operator*& ans) {
		Operator::optimize_it(first);
		Operator::optimize_it(second);

		if (first->get_name() == "num" && second->get_name() == "num") {
			double q = pow(first->get_num(vector<int>()), second->get_num(vector<int>()));
			if (OperatorDiv::is_int(q)) {
				ans = new OperatorNum(q);
				return true;
			}
		}
		if (second->get_name() == "/") {
			if (((OperatorDiv*)second)->first->get_name() == "num" && ((OperatorDiv*)second)->first->get_num(vector<int>()) == 1) {
				ans = new OperatorMathRoot(first, ((OperatorDiv*)second)->second);
				first = nullptr;
				((OperatorDiv*)second)->second = nullptr;
				return true;
			}
		}
		else if (second->get_name() == "num" && second->get_num(vector<int>()) == 1) {
			ans = first;
			first = nullptr;
			return true;
		}
		else if (second->get_name() == "num" && second->get_num(vector<int>()) == 1) {
			ans = new OperatorNum(1);
			return true;
		}
		return false;
	}

	double OperatorExp::get_num(vector<int> arr) {
		return pow(first->get_num(arr), second->get_num(arr));
	}

	Ans* OperatorExp::get_ans(vector<int> arr) {
		Ans *f = first->get_ans(arr), *s = second->get_ans(arr);

		if (f->get_type() == "num" && s->get_type() == "num" && !f->with_pi && !s->with_pi)
			if (OperatorDiv::is_int(pow(((AnsNumber*)f)->number, ((AnsNumber*)s)->number))) {
				Ans* ans = new AnsNumber(pow(((AnsNumber*)f)->number, ((AnsNumber*)s)->number));
				delete f, s;
				return ans;
			}

		return new AnsExp(f, s);
	}

	// OperatorMathRoot
	OperatorMathRoot::OperatorMathRoot(Operator* first, Operator* second) {
		this->first = first;
		this->second = second;
	}

	OperatorMathRoot::~OperatorMathRoot() {
		delete first;
		delete second;
	}

	bool OperatorMathRoot::optimize(Operator*& ans) {
		Operator::optimize_it(first);
		Operator::optimize_it(second);

		if (first->get_name() == "num" && second->get_name() == "num") {
			double q = pow(first->get_num(vector<int>()), 1 / second->get_num(vector<int>()));
			if (OperatorDiv::is_int(q)) {
				ans = new OperatorNum(q);
				return true;
			}
		}
		if (second->get_name() == "/") {
			if (((OperatorDiv*)second)->first->get_name() == "num" && ((OperatorDiv*)second)->first->get_num(vector<int>()) == 1) {
				ans = new OperatorExp(first, ((OperatorDiv*)second)->second);
				first = nullptr;
				((OperatorDiv*)second)->second = nullptr;
				return true;
			}
		}
		else if (second->get_name() == "num" && second->get_num(vector<int>()) == 1) {
			ans = first;
			first = nullptr;
			return true;
		}
		else if (second->get_name() == "num" && second->get_num(vector<int>()) == 2) {
			ans = new OperatorSqrt(first);
			first = nullptr;
			return true;
		}
		return false;
	}

	double OperatorMathRoot::get_num(vector<int> arr) {
		return pow(first->get_num(arr), 1 / second->get_num(arr));
	}

	Ans* OperatorMathRoot::get_ans(vector<int> arr) {
		Ans *f = first->get_ans(arr), *s = second->get_ans(arr);

		if (f->get_type() == "num" && s->get_type() == "num" && !f->with_pi && !s->with_pi)
			if (OperatorDiv::is_int(pow(((AnsNumber*)f)->number, 1 / ((AnsNumber*)s)->number))) {
				int a = pow(((AnsNumber*)f)->number, 1 / ((AnsNumber*)s)->number);
				delete f, s;
				return new AnsNumber(a);
			}

		return new AnsMathRoot(f, s);
	}

	string OperatorMathRoot::get_str() {
		string ans = "";

		if (second->get_preo() <= get_preo())
			ans = "(" + second->get_str() + ")";
		else
			ans = second->get_str();

		ans += "root";

		if (first->get_preo() <= get_preo())
			ans += "(" + first->get_str() + ")";
		else
			ans += first->get_str();

		return ans;
	}

	// OperatorSqrt
	OperatorSqrt::OperatorSqrt(Operator* oper) {
		this->oper = oper;
	}

	OperatorSqrt::~OperatorSqrt() {
		delete oper;
	}

	bool OperatorSqrt::optimize(Operator*& ans) {
		optimize_it(oper);

		if (oper->get_name() == "num" && OperatorDiv::is_int(sqrt(oper->get_num(vector<int>())))) {
			ans = new OperatorNum(sqrt(oper->get_num(vector<int>())));
			return true;
		}
		return false;
	}

	double OperatorSqrt::get_num(vector<int> arr) {
		return sqrt(oper->get_num(arr));
	}

	Ans* OperatorSqrt::get_ans(vector<int> arr) {
		Ans *a = oper->get_ans(arr);

		if (a->get_type() == "num" && !a->with_pi)
			if (OperatorDiv::is_int(sqrt(((AnsNumber*)a)->number))) {
				int b = sqrt(((AnsNumber*)a)->number);
				delete a;
				return new AnsNumber(b);
			}

		return new AnsSqrt(a);
	}

	string OperatorSqrt::get_str() {
		return "sqrt(" + oper->get_str() + ")";
	}

	// OperatorArc
	OperatorArc::~OperatorArc() {
		delete oper;
	}

	string OperatorArc::get_str() {
		return get_str_oper() + "(" + oper->get_str() + ")";
	}

	// OperatorAsin
	double OperatorAsin::get_num(vector<int> arr) {
		return asin(oper->get_num(arr));
	}

	Ans* OperatorAsin::get_ans(vector<int> arr) {
		return new AnsArc(oper->get_ans(arr), "arcsin");
	}

	// OperatorAcos
	double OperatorAcos::get_num(vector<int> arr) {
		return acos(oper->get_num(arr));
	}

	Ans* OperatorAcos::get_ans(vector<int> arr) {
		return new AnsArc(oper->get_ans(arr), "arccos");
	}

	// OperatorAtan
	double OperatorAtan::get_num(vector<int> arr) {
		return atan(oper->get_num(arr));
	}

	Ans* OperatorAtan::get_ans(vector<int> arr) {
		return new AnsArc(oper->get_ans(arr), "arctg");
	}

	// OperatorActg
	double OperatorActg::get_num(vector<int> arr) {
		return 1 / atan(oper->get_num(arr));
	}

	Ans* OperatorActg::get_ans(vector<int> arr) {
		return new AnsArc(oper->get_ans(arr), "arcctg");
	}

	// MainOperator
	MainOperator::MainOperator(string str) {
		oper = get_oper(str);
	}

	MainOperator::~MainOperator() {
		delete oper;
	}

	bool MainOperator::delete_this(string str) {
		int c = 0;
		for (int i = 0; i < str.size(); ++i) {
			if (str[i] == '(')
				++c;
			else if (str[i] == ')')
				--c;
			if (c == 0 && i != str.size() - 1)
				return false;
		}
		return true;
	}

	Operator* MainOperator::get_oper(string str) {
		if (str == "")
			return nullptr;

		if (delete_this(str))
			while (str[0] == '(' && str[str.size() - 1] == ')') {
				str = str.substr(1, str.size() - 2);
			}

		// determine what operation
		bool is_num = true, is_per = true;
		int* ids = new int[4];
		ids[0] = ids[1] = ids[2] = ids[3] = -1;
		int embed = 0;

		for (int i = 0; i < str.size(); ++i) {
			if (!isdigit(str[i]) && str[i] != '.')
				is_num = false;
			if (!isalpha(str[i]))
				is_per = false;
			if (str[i] == '(')
				++embed;
			else if (str[i] == ')')
				--embed;
			else if (embed == 0) {
				if (str[i] == '+' || str[i] == '-')
					ids[0] = i;
				else if (str[i] == '*' || str[i] == '/')
					ids[1] = i;
				else if (str[i] == '^' || (i + 4 < str.size() && str.substr(i, 4) == "root") || (i + 4 < str.size() && str.substr(i, 4) == "sqrt"))
					ids[2] = i;
				else if (i + 4 < str.size() && (str.substr(i, 4) == "asin" || str.substr(i, 4) == "acos" || str.substr(i, 4) == "atan" || str.substr(i, 4) == "actg"))
					ids[3] = i;

			}
			if (embed < 0)
				return nullptr;
		}

		// get id (yes, no fun)
		int id = -1;
		if (ids[0] != -1)
			id = ids[0];
		else if (ids[1] != -1)
			id = ids[1];
		else if (ids[2] != -1)
			id = ids[2];
		else
			id = ids[3];

		// make Operation
		if (id == -1) {
			if (is_num)
				return new OperatorNum(stof(str));
			else if (str == "pi")
				return new OperatorPi();
			else if (is_per)
				return new OperatorPer(get_id_per(str), str);
			else
				return nullptr;
		}
		else {
			switch (str[id]) // I think about factory method pattern... No
			{
			case '+': {
				Operator* f = get_oper(str.substr(0, id)), * s = get_oper(str.substr(id + 1, str.size() - id - 1));
				if (f == nullptr || s == nullptr) {
					delete f;
					delete s;
					return nullptr;
				}
				return new OperatorSum(f, s);
			}
			case '-': {
				Operator* f = get_oper(str.substr(0, id)), * s = get_oper(str.substr(id + 1, str.size() - id - 1));
				if (s == nullptr) {
					delete f;
					return nullptr;
				} else if (f == nullptr)
					if (s->get_name() == "num") {
						((OperatorNum*)s)->set_num(-1 * ((OperatorNum*)s)->get_num(vector<int>()));
						return s;
					} else
						return new OperatorMul(new OperatorNum(-1), s);
				return new OperatorDiff(f, s);
			}
			case '*': {
				Operator* f = get_oper(str.substr(0, id)), * s = get_oper(str.substr(id + 1, str.size() - id - 1));
				if (f == nullptr || s == nullptr) {
					delete f;
					delete s;
					return nullptr;
				}
				return new OperatorMul(f, s);
			}
			case '/': {
				Operator* f = get_oper(str.substr(0, id)), * s = get_oper(str.substr(id + 1, str.size() - id - 1));
				if (f == nullptr || s == nullptr) {
					delete f;
					delete s;
					return nullptr;
				}
				return new OperatorDiv(f, s);
			}
			case '^': {
				Operator* f = get_oper(str.substr(0, id)), * s = get_oper(str.substr(id + 1, str.size() - id - 1));
				if (f == nullptr || s == nullptr) {
					delete f;
					delete s;
					return nullptr;
				}
				return new OperatorExp(f, s);
			}
			default:
				break;
			}
		}

		if (id + 4 < str.size() && str.substr(id, 4) == "root") {
			Operator *f = get_oper(str.substr(0, id)), *s = get_oper(str.substr(id + 4, str.size() - id - 4));
			if (f == nullptr || s == nullptr) {
				delete f;
				delete s;
				return nullptr;
			}
			return new OperatorMathRoot(s, f);
		}
		else if (id == 0 && str.size() > 3 && str.substr(0, 4) == "sqrt") {
			Operator* q = get_oper(str.substr(4, str.size() - 4));
			if (q == nullptr)
				return q;
			return new OperatorSqrt(q);
		}
		else if (id + 4 < str.size()) {
			Operator* q = get_oper(str.substr(4, str.size() - 4));
			if (q == nullptr)
				return q;

			// switch uses only numbers :(
			string what = str.substr(0, 4);
			if (what == "asin")
				return new OperatorAsin(q);
			else if (what == "acos")
				return new OperatorAcos(q);
			else if (what == "atan")
				return new OperatorAtan(q);
			else if (what == "actg")
				return new OperatorActg(q);

		}
		
		return nullptr;
	}

	int MainOperator::get_id_per(string name) {
		for (int i = 0; i < arr_per.size(); ++i)
			if (arr_per[i] == name)
				return i;
		arr_per.push_back(name);
		return arr_per.size() - 1;
	}

	void MainOperator::optimize() {
		Operator::optimize_it(oper);
	}
}