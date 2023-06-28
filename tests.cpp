#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"



TEST_CASE("Проверка на победу")
{
	bool finish = Check_finish(99); //отрицательный тест
	CHECK(finish == false);
	finish = Check_finish(100);
	CHECK(finish == true);
	finish = Check_finish(101);
	CHECK(finish == true);
}

TEST_CASE("Проверка дляины змейки в зависимости от кол-ва съеденный фруктов")
{
	int lengh_1 = size(0);
	int lengh_2 = size(1);
	int lengh_3 = size(53);
	CHECK(lengh_1 == 2);
	CHECK(lengh_2 == 3);
	CHECK(lengh_3 == 54); //отрицательный тест
}
