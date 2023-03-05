#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <Windows.h>
#include <stdio.h>
using namespace std;

//Перенести интерент магазин розетка в консоль.
//Сделать возможность создавать пользователя,
//выбирать категории осеществлять покупки.
//Все данные хранить в файле.
//(как вариант создать несколько файлов в которых будет продукция по этим категориями)
//Реализовать корзину и понравившееся продукты.
//И при выходе сохранять всю информацию в файле.

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void set_color(int text)
{
	SetConsoleTextAttribute(h, (0 << 4) + text);
}

class RZTK
{
	string line, current_account, _account_info;
	string account_info[3];
	int money;

	vector<string> tech;
	vector<string> appliances;
	vector<string> cloth;

	vector<string> favorite;
	vector<string> cart;

public:
	RZTK()
	{
		read_catalogs();
	}

	void read_catalogs()
	{
		fstream _tech("catalogs/tech.txt");
		fstream _appliances("catalogs/appliances.txt");
		fstream _cloth("catalogs/cloth.txt");
		while (getline(_tech, line)) { tech.push_back(line); }
		while (getline(_appliances, line)) { appliances.push_back(line); }
		while (getline(_cloth, line)) { cloth.push_back(line); }
	}

	void sign_out()
	{
		ofstream f("users/current_account.txt");
		f << "";
	}
	
	void sign_in(string login)
	{
		ifstream account("users/account/" + login + ".txt");
		int i = 0;
		while (getline(account, line))
		{
			account_info[i++] = line;
		}
		cout << ("Выполнен авто-вход по логину: " + login + "\n\n");
	}

	void create_account_info(string login)
	{
		ifstream account("users/account/" + login + ".txt");
		int i = 0;
		while (getline(account, line))
		{
			account_info[i++] = line;
		}
	}

	void load_account_info()
	{
		ofstream account("users/account/" + account_info[0] + ".txt");

		_account_info = account_info[0] + "\n" + account_info[1] + "\n" + account_info[2] + "\n" + account_info[3] + "\n";
		account << _account_info;
	}

	void change_login(string login)
	{
		string path = "users/account/" + account_info[0] + ".txt";
		const char* _path = path.c_str();

		ofstream set_current_login("users/current_account.txt");
		set_current_login << login;

		remove(_path);
		account_info[0] = login;
		load_account_info();
	}

	void change_password(string password)
	{
		account_info[1] = password;
		load_account_info();
	}

	bool check_password(string login, string password)
	{	
		create_account_info(login);

		if (password == account_info[1])
		{
			ofstream set_current_login("users/current_account.txt");
			set_current_login << login;
			return true;
		}
		else
		{
			cout << "Пароли не совпадают!\n\n";
			return false;
		}
	}

	bool entry_confirmed()
	{
		ifstream check("users/current_account.txt");
		while (getline(check, line))
		{ 
			sign_in(line);
			return true; 
		}
		return false;
	}

	bool exist_login(string login, bool show)
	{
		ifstream check("users/account/" + login + ".txt");
		while (getline(check, line))
		{
			if(show) cout << ("Такой логин уже существует!\n\n");
			return true;
		}
		return false;
	}	

	bool register_user(string login, string password)
	{
		if (!exist_login(login, true))
		{
			ofstream set_current_login("users/current_account.txt");
			set_current_login << login;

			auto now = chrono::system_clock::now();
			time_t end_time = chrono::system_clock::to_time_t(now);
			ofstream f("users/account/" + login + ".txt");			
			string date = ctime(&end_time);
			_account_info = login + "\n" + password + "\nДата регистрации: " + date + to_string(money);
			f << _account_info;
			f.close();

			create_account_info(login);

			cout << ("Аккаунт успешно создан!\n\n");
			return true;
		}		
		return false;
	}

	vector<string> get_by_category(string category) 
	{	
		cout << tech.size() << endl;
		cout << tech.at(0) << endl;

		if (category == "tech") return tech;
		if (category == "appliances") return appliances;
		if (category == "cloth") return cloth;
	}

	string* get_account_info() { return account_info; }
};

class View
{
	RZTK* rztk;
	static string current;

public:
	View() {};
	View(RZTK* rztk) : rztk(rztk) {}

	void menu_login() { cout << "1. Войти\n2. Зарегистрироваться\n3. Завершить работу\n"; }

	void set_current(string value) { current = value; }
	void current_message() { cout << current; }

	void account() { cout << "\tИнформация об аккаунте:\nЛогин: " << rztk->get_account_info()[0] << "\nПароль: " << rztk->get_account_info()[1] << "\n" << rztk->get_account_info()[2] << "\n\n1. Сменить логин\n2. Сменить пароль\n3. Выйти из аккаунта\n4. Назад\n"; }

	void main_menu()
	{
		cout << "1. Товары\n2. Корзина\n3. Аккаунт\n4. Завершить работу\n";
	}

	void cart()
	{

	}

	void catalogs(string category)
	{
		for (int i = 0; i < rztk->get_by_category(category).size(); i++)
		{
			cout << i + 1 << ". " << rztk->get_by_category(category).at(i) << endl;
		} cout << "\n\n";
	}
};

string View::current;

class Controller
{
	string txt;
	RZTK* rztk;

public:
	Controller(RZTK* rztk) : rztk(rztk) {}
	
	string input()
	{
		cout << "-> ";
		cin >> txt;
		system("cls");
		return txt;
	}

	void control()
	{

	}
};

int main()
{
	system("chcp 1251");
	system("cls");

	cout << "Вас приветствует консольный интернет-магазин "; set_color(2); cout << "ROZETKA!"; set_color(7); cout << "\n\n";

	string user, login, password;
	bool first_time = true;

	RZTK rztk;
	Controller controller(&rztk);
	View view(&rztk);

	bool stage1 = false;

	while (true)
	{
		

		if (first_time && rztk.entry_confirmed())
		{			
			stage1 = true;
			view.current_message();
		}

		first_time = false;

		if (!stage1)
		{
			view.menu_login();
			user = controller.input();

			if (user == "1")
			{
				bool back = false;

				while (!back)
				{
					cout << "Введите логин (0 - выход) ";
					login = controller.input();

					if (login == "0") back = true;
					if (rztk.exist_login(login, false)) break;
					else cout << "Такого логина нет!\n\n";
				}

				while (!back)
				{
					cout << "Введите пароль (0 - выход) ";
					password = controller.input();

					if (password == "0") back = true;
					if (password.size() >= 5)
					{
						if (rztk.check_password(login, password))
						{
							rztk.sign_in(login);
							stage1 = true;
						}
						break;
					}
					else cout << "Минимум 5 символов!\n\n";
				}
			}
			else if (user == "2")
			{
				bool back = false;

				while (!back)
				{
					cout << "Введите логин (0 - выход) ";
					login = controller.input();

					if (login == "0") back = true;
					if (rztk.exist_login(login, true)) view.current_message();
					else break;
				}

				while (!back)
				{
					cout << "Введите пароль (мин. 5 симв, 0 - выход) ";
					password = controller.input();

					if (password == "0") { back = true; break; }
					if (password.size() >= 5)break;
					else cout << "Минимум 5 символов!\n\n";
				}

				if (back) continue;
				if (rztk.register_user(login, password))
				{
					stage1 = true;
					view.current_message();
				}
				else view.current_message();
			}
			else if (user == "3")
			{
				break;
			}
		}
		else
		{
			while (stage1)
			{
				view.main_menu();
				user = controller.input();

				if (user == "1")
				{
					view.catalogs("tech");
				}
				else if (user == "3")
				{
					while (true)
					{
						view.account();
						user = controller.input();

						if (user == "1")
						{
							cout << "Введите новый логин (0 - выход) -> ";
							login = controller.input();

							if (!rztk.exist_login(login, true))
							{
								rztk.change_login(login);
								cout << "Логин изменён!\n\n";
							}
						}
						else if (user == "2")
						{
							cout << "Введите новый пароль (0 - выход) -> ";
							password = controller.input();

							if (password.size() >= 5)
							{
								rztk.change_password(password);
								cout << "Пароль изменён!\n\n";
							}
							else cout << "Минимум 5 символов!\n\n";
						}
						else if (user == "3")
						{
							rztk.sign_out();
							stage1 = false;
							break;
						}
						else if (user == "4") break;
					}					
				}
				else if (user == "4")
				{
					exit(EXIT_SUCCESS);
				}
			}
		}
	}
}