#include<iostream>
#include<Windows.h>
#include<conio.h>
#include <time.h>
#include<fstream>

bool game_over;
int count;
const int width = 60;
const int height = 20;
int x, y, Apple_x[5], Apple_y[5];
int coord_X[100], coord_Y[100];

/*
* направление движения змейки 
*/
enum side_go {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

side_go to_go;

/* Функция проверки статуса победы
    @return состояние победы:
        true - победа
        false - поражение
    @param a - счёт набранный игроком (кол-во съеденных яблок)
*/
bool Check_finish(int a) {
    if (a >= 100)
        return true;
    else
        return false;
}

/* Функция актуальной длины змейки
* @param a - игровой счет (кол-во схеденных яблок)
* @return длина змейки
*/
int size(int a) {
    return a + 2;
}


/*
* Функция проверки доступности координаты сгенерированного яблока.
    @return статус ячейки
        true - ячейка под яблоко свободна
        false - ячейка под яблоко занята
    @param k - номер проверяемого яблока
*/
bool check_apple(int k) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (Apple_x[k] == coord_X[j] and Apple_y[k] == coord_Y[i])
                return true;
        }
    }
    return false;
}



/*
* Настройка игры, размеров поля, начального положения змейки и фруктов. Выполняется 1 раз.
*/
void Settings() {
    srand(time(NULL)); 

    game_over = false; 
    to_go = RIGHT;
    //задание начального положения змейки и направления её движения
    x = 2;
    y = 0;
    coord_X[1] = 1;
    coord_X[2] = 0;
    coord_Y[2] = 0;
    coord_Y[1] = 0;
    //цикл заполнения координат поле яблоками (включает в себя проверку клеток под яблоко)
    for (int i = 0; i < 5; i++) {
        do {
            Apple_x[i] = rand() % (width - 2);
            Apple_y[i] = rand() % height;
        } while (check_apple(i));
    }

    count = 0; //игровой счёт
}

/*
* Функция отрисовки игрового поля - выполняет только графические задачи.
*/
void Print() {
    system("cls");
    for (int i = 0; i < width + 1; i++) //верхняя граница
        std::cout << (char)219;
    std::cout << std::endl;

    // заполнение внутренней части поля
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            bool ex = false;

            for (int k = 0; k < 5; k++)
                if (j == Apple_x[k] and i == Apple_y[k])
                    ex = true;
            

            if (j == 0 || j == width - 1) //боковые границы
                std::cout << (char)219;

            if (i == y && j == x) //рисовка змеи + яблока
                std::cout << "0";
            else if (ex)
                std::cout << (char)254;
            else {
                bool print = false;
                for (int z = 0; z < count + 2; z++) {
                    if (coord_X[z] == j and coord_Y[z] == i) {
                        std::cout << 'o';
                        print = true;
                    }
                }
                if (!print)
                    std::cout << " ";
            }

        }
        std::cout << std::endl;
    }
    for (int i = 0; i < width + 1; i++)
        std::cout << (char)219;
    std::cout << std::endl;
    std::cout << "Score: " << count;
    Sleep(100);

}

/*
* Фукнция обработки нажатой клавиши. Меняет направление змейки в зависимости от
исходного направления движения и нажатой клавиши. Так же обработка остановки игры.
*/
void Input() {
    if (_kbhit()) {
        char symbol = tolower(_getch());

        switch (symbol) //обработка поворота змейки:
        {
        case 'a':       //налево относительно направления движения
            if (to_go == UP)
                to_go = LEFT;
            else if (to_go == RIGHT)
                to_go = UP;
            else if (to_go == DOWN)
                to_go = RIGHT;
            else if (to_go == LEFT)
                to_go = DOWN;
            break;
        case 'd':           //направо относительно направления движения
            if (to_go == UP)
                to_go = RIGHT;
            else if (to_go == RIGHT)
                to_go = DOWN;
            else if (to_go == DOWN)
                to_go = LEFT;
            else if (to_go == LEFT)
                to_go = UP;
            break;

        case ' ':       //выход из игры
            game_over = true;
            break;
        }
    }
}

/*
* Основная логика игры: механика перемещения змейки, увелечения, генерации новых яблок.
*/
void Game() {
    int last_x = coord_X[0];
    int last_y = coord_Y[0];

    int pred_x, pred_y;

    coord_X[0] = x;
    coord_Y[0] = y;

    for (int i = 1; i < size(count); i++) { //цикл сдвига вправо
        pred_x = coord_X[i];
        pred_y = coord_Y[i];

        coord_X[i] = last_x;
        coord_Y[i] = last_y;

        last_x = pred_x;
        last_y = pred_y;
    }
    
    //движение головы
    switch (to_go)
    {
    case LEFT:
        x = x - 1;
        break;
    case RIGHT:
        x = x + 1;
        break;
    case UP:
        y = y - 1;
        break;
    case DOWN:
        y = y + 1;
        break;
    }
    //проверка на выход за границы
    if (x >= width - 1)
        x = 0;
    else if (x < 0)
        x = width - 2;

    if (y >= height)
        y = 0;
    else if (y < 0)
        y = height - 1;

    //проверка пересечения себя самой
    for (int i = 0; i < size(count); i++) {
        if (coord_X[i] == x and coord_Y[i] == y)
            game_over = true;
    }
    //заполнение пустых позиций яблоками
    for (int i = 0; i < 5; i++) {
        if (x == Apple_x[i] and y == Apple_y[i]) {
            count += 1;
            do {
                Apple_x[i] = rand() % (width - 2);
                Apple_y[i] = rand() % height;
            } while (check_apple(i));
        }
    }
}

//*главная функция
// @return 0 - при поражении или победе

int main()
{

    Settings();
    //основной цикл игры
    while (!game_over and !Check_finish(count)) {
        Print();
        Input();
        Game();
    }
    //обработка результата игры
    system("cls");
    if (!Check_finish(count))
        std::cout << "You loosed! Your schore: " << count << std::endl;
    else
        std::cout << "You won! Your schore: " << count << std::endl;
    return 0;
}
