// Лабиринт.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <windows.h> // окрашивание консоли и работа с координатами
#include <conio.h>
using namespace std;

// enumeration - перечисление
enum KeyCodes { ENTER = 13, ESCAPE = 27, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80, SPACEBAR = 32 };
enum Colors { DARKGREEN = 2, RED = 12, YELLOW = 14, BLUE = 9, PURPLE = 13 };
enum Objects { HALL, WALL, COIN, ENEMY, DRUGS };

// const int ENTER = 13;
// const int ESCAPE = 27;

int main()
{
	int coins = 0;

	system("title Бомбер!");
	// запуск алгоритма генерации случайных чисел
	srand(time(0));
	rand();

	// таблица аски для поиска символов псевдографики
	//for (int code = 0; code < 256; code++)
	//{
	//	cout << code << " - " << (char)code << "\n";
	//}

	// дескриптор окна консоли (порядковый номер окна в системе)
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	// скрыть стандартный мигающий курсор
	CONSOLE_CURSOR_INFO info;
	info.bVisible = false;
	info.dwSize = 100;
	SetConsoleCursorInfo(h, &info);

	const int WIDTH = 50;
	const int HEIGHT = 15;
	int location[HEIGHT][WIDTH] = {};

	// модель локации
	// 0 - коридоры (пустоты)
	// 1 - стена разрушаемая
	// 2 - монетки
	// 3 - враги

	// генерация локации
	for (int y = 0; y < HEIGHT; y++) // перебор строк
	{
		for (int x = 0; x < WIDTH; x++) // перебор столбцов
		{
			// по дефолту пишется случайное число
			location[y][x] = rand() % 4; // 0 1 2 3

			// стены по краям
			if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1)
				location[y][x] = WALL;

			// вход и выход
			if (x == 0 && y == 2 || x == WIDTH - 1 && y == HEIGHT - 3)
				location[y][x] = HALL;

			if (location[y][x] == ENEMY) {
				// определяется вероятность того, останется враг или нет
				// допустим, вероястность остаться на уровне - 10%
				int prob = rand() % 10; // 0-9
				if (prob != 0) // 1 2 3 4 5 6 7 8 9
					location[y][x] = HALL;
			}
		}
	}

	// показ локации
	for (int y = 0; y < HEIGHT; y++) // перебор строк
	{
		for (int x = 0; x < WIDTH; x++)
		{
			// cout << location[y][x];
			switch (location[y][x]) {
			case HALL: // коридор
				cout << " ";
				break;
			case WALL: // стена
				SetConsoleTextAttribute(h, DARKGREEN); // 0-255
				cout << (char)177; //(char)219;
				break;
			case COIN: // монетки
				SetConsoleTextAttribute(h, YELLOW);
				cout << (char)15; // 249
				break;
			case ENEMY: // враги
				SetConsoleTextAttribute(h, RED);
				cout << (char)1;
				break;
			case DRUGS: //Добавить новый тип объектов лабиринта - "лекарство"
				SetConsoleTextAttribute(h, PURPLE);
				cout << (char)211;
				break;
			default:
				cout << location[y][x];
				break;
			}
		}
		cout << "\n";
	}
	// размещение ГГ (главного героя)
	COORD position;
	position.X = 1;
	position.Y = 2;
	// функция установки курсора в любую точку на экране консоли
	SetConsoleCursorPosition(h, position);
	SetConsoleTextAttribute(h, BLUE);
	cout << (char)1;
	// игровой движок (интерактив с пользователем)
	while (true) {
		int code = _getch(); // функция приостанавливает работу программы, ждёт реакции пользователя
		if (code == 224)
			code = _getch(); // если первый код был 224, то повторный вызов функции вернёт только один, нормальный код
		// стирание ГГ в старой позиции
		SetConsoleCursorPosition(h, position);
		SetConsoleTextAttribute(h, BLUE);
		cout << " ";

		// пользователь может нажать любую кнопку (в том числе энтер, эскейп, пробел, стрелочки), после чего вернётся код нажатой клавиши
		switch (code) {
		case ENTER:
			// cout << "ENTER\n";
			break;
		case SPACEBAR:
			// cout << "SPACEBAR\n";
			break;
		case ESCAPE:
			// cout << "ESCAPE\n";
			break;
		case RIGHT:
			if (location[position.Y][position.X + 1] != WALL)
				position.X++;
			break;
		case LEFT:
			if (position.X > 0 && location[position.Y][position.X - 1] != WALL)
				position.X--;
			break;
		case UP:
			if (location[position.Y - 1][position.X] != WALL)
				position.Y--;
			break;
		case DOWN:
			if (location[position.Y + 1][position.X] != WALL)
				position.Y++;
			break;
		default:
			cout << code << "\n";
			break;
		}

		// показ ГГ в новой позиции
		SetConsoleCursorPosition(h, position);
		SetConsoleTextAttribute(h, BLUE);
		cout << (char)1;

		// взаимодействие ГГ с другими объектами в лабиринте
		if (location[position.Y][position.X] == COIN) {
			coins++;
			cout << coins << "\n";
			// cout << "COIN COLLECTED!\n";
			location[position.Y][position.X] = HALL;
		}

		// Если персонаж  дошёл до выхода из лабиринта в нижнем правом углу - игра заканчивается победой(вывести диалог с сообщением "победа - найден выход" в MessageBox).

		if (position.Y == HEIGHT - 3 && position.X == WIDTH - 1)
		{
			cout << "победа - найден выход" << "\n";
			break;
		}
	}
}