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
	string account_info[4];
	int money;

	vector<string> tech;
	vector<string> appliances;
	vector<string> cloth;

	vector<int> tech_price;
	vector<int> appliances_price;
	vector<int> cloth_price;
	vector<int> cart_price;
	vector<int> fav_price;

	vector<string> fav;
	vector<string> cart;	

public:
	RZTK()
	{
		read_catalogs();
		money = 1000 + rand() % 75000;
	}

	void read_catalogs()
	{
		fstream _tech("catalogs/tech.txt");
		fstream _appliances("catalogs/appliances.txt");
		fstream _cloth("catalogs/cloth.txt");
		fstream _tech_p("catalogs/tech_price.txt");
		fstream _appliances_p("catalogs/appliances_price.txt");
		fstream _cloth_p("catalogs/cloth_price.txt");
		while (getline(_tech, line)) { tech.push_back(line); }
		while (getline(_appliances, line)) { appliances.push_back(line); }
		while (getline(_cloth, line)) { cloth.push_back(line); }
		while (getline(_tech_p, line)) { tech_price.push_back(stoi(line)); }
		while (getline(_appliances_p, line)) { appliances_price.push_back(stoi(line)); }
		while (getline(_cloth_p, line)) { cloth_price.push_back(stoi(line)); }
	}

	void order()
	{
		create_account_info(current_account);
		readCart();
		if (cart.size() > 0)
		{
			int sum = 0;
			for (int i = 0; i < cart.size(); i++) { sum += cart_price.at(i); }

			if (stoi(account_info[3]) < sum) cout << "Не хватает денег!\n\n";
			else
			{
				account_info[3] = to_string(stoi(account_info[3]) - sum);
				load_account_info();
				cart.clear();
				cart_price.clear();
				loadCart(666);

				cout << "Заказ оформлен!\n\n";
			}
		}
	}

	void readCart()
	{
		cart.clear();
		fstream read("users/account/" + account_info[0] + "_cart.txt");
		fstream readI("users/account/" + account_info[0] + "_cart_price.txt");

		while (getline(read, line))
		{
			cart.push_back(line);			
		}
		while (getline(readI, line))
		{
			cart_price.push_back(stoi(line));
		}
	}

	void readFav()
	{
		fav.clear();
		fstream read("users/account/" + account_info[0] + "_fav.txt");
		fstream readI("users/account/" + account_info[0] + "_fav_price.txt");

		while (getline(read, line))
		{
			fav.push_back(line);
		}
		while (getline(readI, line))
		{
			fav_price.push_back(stoi(line));
		}
	}

	void loadCart()
	{
		if (cart.size() > 0)
		{
			string to_cart, to_cartI;
			ofstream _loadCart;
			ofstream _loadCartI;
			_loadCart.open("users/account/" + account_info[0] + "_cart.txt", ios::app);
			_loadCartI.open("users/account/" + account_info[0] + "_cart_price.txt", ios::app);
			
			ofstream _clearCart("users/account/" + account_info[0] + "_cart.txt");
			ofstream _clearCartI("users/account/" + account_info[0] + "_cart_price.txt");
			_clearCart << "";
			_clearCartI << "";

			for (int i = 0; i < cart.size(); i++)
			{
				to_cart += cart.at(i) + "\n";
				to_cartI += to_string(cart_price.at(i)) + "\n";
			}

			_loadCart << to_cart;
			_loadCartI << to_cartI;
			_loadCart.close();
			_loadCartI.close();
		}
	}

	void loadFav()
	{
		if (fav.size() > 0)
		{
			string to_fav, to_favI;
			ofstream _loadFav;
			ofstream _loadFavI;
			_loadFav.open("users/account/" + account_info[0] + "_fav.txt", ios::app);
			_loadFavI.open("users/account/" + account_info[0] + "_fav_price.txt", ios::app);

			ofstream _clearFav("users/account/" + account_info[0] + "_fav.txt");
			ofstream _clearFavI("users/account/" + account_info[0] + "_fav_price.txt");
			_clearFav << "";
			_clearFavI << "";

			for (int i = 0; i < fav.size(); i++)
			{
				to_fav += fav.at(i) + "\n";
				to_favI += to_string(fav_price.at(i)) + "\n";
			}

			_loadFav << to_fav;
			_loadFavI << to_favI;
			_loadFav.close();
			_loadFavI.close();
		}
	}
	
	void loadCart(int a)
	{
		ofstream _clearCart("users/account/" + account_info[0] + "_cart.txt");
		ofstream _clearCartI("users/account/" + account_info[0] + "_cart_price.txt");
		_clearCart << "";
		_clearCartI << "";
	}
	
	void loadFav(int a)
	{
		ofstream _clearFav("users/account/" + account_info[0] + "_fav.txt");
		ofstream _clearFavI("users/account/" + account_info[0] + "_fav_price.txt");
		_clearFav << "";
		_clearFavI << "";
	}

	void addToCartFromFav(int id)
	{
		readCart();

		cart.push_back(fav.at(id));
			cart_price.push_back(fav_price.at(id));

		loadCart();
	}

	void addToCart(string category, int id)
	{
		readCart();		

		if (category == "tech")
		{	
			cart.push_back(tech.at(id));
			cart_price.push_back(tech_price.at(id));
		}
		else if (category == "cloth")
		{
			cart.push_back(cloth.at(id));
			cart_price.push_back(cloth_price.at(id));
		}
		else if (category == "appliances")
		{
			cart.push_back(appliances.at(id));
			cart_price.push_back(appliances_price.at(id));
		}

		loadCart();
	}

	void addToFav(string category, int id)
	{
		readFav();
		
		if (category == "tech")
		{
			fav.push_back(tech.at(id));
			fav_price.push_back(tech_price.at(id));
		}
		else if (category == "cloth")
		{
			fav.push_back(cloth.at(id));
			fav_price.push_back(cloth_price.at(id));
		}
		else if (category == "appliances")
		{
			fav.push_back(appliances.at(id));
			fav_price.push_back(appliances_price.at(id));
		}

		loadFav();
	}

	void deleteCartAt(int id)
	{
		readCart();
		cart.erase(cart.begin() + id);
		cart_price.erase(cart_price.begin() + id);
		if (cart.size() > 0) loadCart();
		else loadCart(666);
	}

	void deleteFavAt(int id)
	{
		readFav();
		fav.erase(fav.begin() + id);
		fav_price.erase(fav_price.begin() + id);
		if (fav.size() > 0) loadFav();
		else loadFav(666);
	}

	void sign_out()
	{
		ofstream f("users/current_account.txt");
		f << " ";
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
		if (category == "tech") return tech;
		else if (category == "appliances") return appliances;
		else if (category == "cloth") return cloth;
	}
	vector<int> get_price(string category)
	{
		if (category == "tech") return tech_price;
		else if (category == "appliances") return appliances_price;
		else if (category == "cloth") return cloth_price;
	}

	vector<string> get_fav() { return fav; }
	vector<string> get_cart() { return cart; }
	vector<int> get_cart_price() { return cart_price; }
	vector<int> get_fav_price() { return fav_price; }

	int cart_len() { return cart.size(); }

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

	void account() { cout << "\tИнформация об аккаунте:\nЛогин: " << rztk->get_account_info()[0] << "\nПароль: " << rztk->get_account_info()[1] << "\n" << rztk->get_account_info()[2] << "\nДенег на счету: " << rztk->get_account_info()[3] << "\n\n1. Сменить логин\n2. Сменить пароль\n3. Выйти из аккаунта\n4. Назад\n"; }

	void main_menu()
	{
		cout << "1. Товары\n2. Корзина\n3. Аккаунт\n4. Завершить работу\n";
	}

	void cart()
	{
		int sum = 0;
		for (int i = 0; i < rztk->get_cart().size(); i++)
		{
			cout << i + 1 << ". " << rztk->get_cart().at(i) << " - " << rztk->get_cart_price().at(i) << " грн\n";
			sum += rztk->get_cart_price().at(i);
		} 
		
		cout << "Итого: " << sum << " грн\nВаш баланс: " << rztk->get_account_info()[3] << " грн\n\n" << rztk->get_cart().size()+1 << ". Оформить заказ\n" << rztk->get_cart().size()+2 << ". Назад\n-> ";
	}

	void favorite()
	{
		for (int i = 0; i < rztk->get_fav().size(); i++)
		{
			cout << i + 1 << ". " << rztk->get_fav().at(i) << "(" << rztk->get_fav_price().at(i) << " грн)\n";
		}
		cout << rztk->get_fav().size() + 1 << ". Назад\n";
	}

	void catalogs(string category)
	{
		cout << "Выберите товар\n";
		for (int i = 0; i < rztk->get_by_category(category).size(); i++)
		{
			cout << i + 1 << ". " << rztk->get_by_category(category).at(i) << " - " << rztk->get_price(category).at(i) << " грн" << endl;
		}
	}
	
	void menu_catalogs() { cout << "1. Техника\n2. Одежда\n3. Бытовая техника\n4. Понравившиеся\n5. Назад\n"; }
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
	int u;

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
					while (true)
					{
						view.menu_catalogs();
						user = controller.input();

						if (user == "1")
						{
							view.catalogs("tech"); cout << "-> ";
							while (!(cin >> u) || (cin.peek() != '\n')) { cin.clear(); while (cin.get() != '\n'); cout << "\nТолько цифрами!\n\n-> "; }
							system("cls");

							if (u >= 1 && u <= rztk.get_by_category("tech").size())
							{
								while (true)
								{
									cout << "1. Добавить в корзину\n2. Добавить в понравившиеся\n3. Назад\n";
									user = controller.input();

									if (user == "1")
									{
										rztk.addToCart("tech", u-1);
										cout << "Добавлено в корзину!\n\n";
									}
									else if (user == "2")
									{
										rztk.addToFav("tech", u - 1);
										cout << "Добавлено в понравившиеся!\n\n";
									}
									else if (user == "3") break;
								}
								
							}
							else cout << "Такого товара нет!\n\n";
						}
						else if (user == "2")
						{
							view.catalogs("cloth");

							while (!(cin >> u) || (cin.peek() != '\n')) { cin.clear(); while (cin.get() != '\n'); cout << "\nТолько цифрами!\n\n-> "; }
							system("cls");

							if (u >= 1 && u <= rztk.get_by_category("cloth").size())
							{
								while (true)
								{
									cout << "1. Добавить в корзину\n2. Добавить в понравившиеся\n3. Назад\n";
									user = controller.input();

									if (user == "1")
									{
										rztk.addToCart("cloth", u - 1);
										cout << "Добавлено в корзину!\n\n";
									}
									else if (user == "2")
									{
										rztk.addToFav("cloth", u -1);
										cout << "Добавлено в понравившиеся!\n\n";
									}
									else if (user == "3") break;
								}

							}
							else cout << "Такого товара нет!\n\n";
						}
						else if (user == "3")
						{
							view.catalogs("appliances");

							while (!(cin >> u) || (cin.peek() != '\n')) { cin.clear(); while (cin.get() != '\n'); cout << "\nТолько цифрами!\n\n-> "; }
							system("cls");

							if (u >= 1 && u <= rztk.get_by_category("appliances").size())
							{
								while (true)
								{
									cout << "1. Добавить в корзину\n2. Добавить в понравившиеся\n3. Назад\n";
									user = controller.input();

									if (user == "1")
									{
										rztk.addToCart("appliances", u - 1);
										cout << "Добавлено в корзину!\n\n";
									}
									else if (user == "2")
									{
										rztk.addToFav("appliances", u - 1);
										cout << "Добавлено в понравившиеся!\n\n";
									}
									else if (user == "3") break;
								}

							}
							else cout << "Такого товара нет!\n\n";
						}
						else if (user == "4")
						{
							rztk.readFav();
							if (rztk.get_fav().size() < 1) cout << "У вас ещё нет понравившихся товаров!\n\n";
							else
							{
								view.favorite();
								while (!(cin >> u) || (cin.peek() != '\n')) { cin.clear(); while (cin.get() != '\n'); cout << "\nТолько цифрами!\n\n-> "; }
								system("cls");

								if (u >= 1 && u <= rztk.get_fav().size())
								{
									cout << "1. Удалить из понравившихся\n2. Добавить в корзину\n3. Назад\n";
									user = controller.input();

									if (user == "1")
									{
										rztk.deleteFavAt(u - 1);
										cout << "Удалено!\n\n";
									}
									else if (user == "2")
									{
										rztk.addToCartFromFav(u-1);
										cout << "Добавлено в корзину!\n\n";
									}
								}
							}
						}
						else if (user == "5") break;
					}					
				}
				else if (user == "2")
				{
					rztk.readCart();
					if (rztk.cart_len() < 1)
					{
						cout << "Корзина пуста!\n\n";
					}
					else
					{
						while (true)
						{
							view.cart();
							while (!(cin >> u) || (cin.peek() != '\n')) { cin.clear(); while (cin.get() != '\n'); cout << "\nТолько цифрами!\n\n-> "; }
							system("cls");

							if (u >= 1 && u <= rztk.cart_len())
							{
								cout << "1. Удалить из корзины\n2. Назад\n";
								user = controller.input();

								if (user == "1")
								{
									rztk.deleteCartAt(u - 1);
								}
							}
							else if (u == rztk.cart_len() + 1)
							{
								rztk.order();
							}
							else if (u == rztk.cart_len() + 2) break;
						}
					}					
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