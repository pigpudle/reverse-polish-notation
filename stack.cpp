#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cstring>

// Имена функций, чтобы можно было их использовать до объявления
bool isOperation(std::string element);
double calculateOperation(char operation, double left, double right);
void throwIfNotNumber(std::string value);

int main() {
  // Мы ставим условие на разделение пробелом,
  // так как числа могут состоять из нескольких символов,
  // поэтому просто разбить посимвольно не получится.
  // Для разделения дробных чисел следует использовать точку или запятую (зависит от локали компьютера)
  std::cout << "Замечание: для отделения дробной части используйте точку или запятую (зависит от локали компьютера). \n";
  std::cout << "Введите выражение в обратной польской записи (разделяя пробелом): ";
  std::string input;
  std::getline(std::cin, input); // берет полностью строку до конца, а не до пробелов
  std::cout << "\n";

  // Проверка пустой строки
  if (input.empty()) {
    throw std::invalid_argument( "Строка не должна быть пустой!" );
  }

  // Разбить строку на вектор символов (по пробелам)
  std::vector<std::string> elements;
  std::istringstream elementsStream(input); // разделяет по пробелам
  std::string element;
  while (std::getline(elementsStream, element, ' ')) {
    elements.push_back(element);
  }

  // Посчитать результат

  // Стэк для хранения операций,
  // тип string, так как в нем могут храниться и числа, и операции
  // а числа могут состоять из нескольких символов, поэтому тип char не подходит
  std::stack<std::string> stack;

  while(!elements.empty()) {
    // Обратная польская запись просматривается слева направо
    // Получить первый элемент
    std::string nextElement = elements.front();
    // Удалить первый элемент из вектора символов
    elements.erase(elements.begin());

    if (isOperation(nextElement)) {
      // Если рассматриваемый элемент — знак операции,
      // то выполняется эта операция над операндами (числами), записанными левее знака операции

      double right = std::stof(stack.top()); // получить верхний элемент и перевести его в double
      stack.pop(); // удалить верхний элемент

      double left = std::stof(stack.top()); // получить верхний элемент и перевести его в double
      stack.pop(); // удалить верхний элемент

      // Провести операцию над числами
      char operation = nextElement[0];
      double operationResult = calculateOperation(operation, left, right);
      stack.push(std::to_string(operationResult));
    } else {
      throwIfNotNumber(nextElement);
      // Если рассматриваемый элемент - операнд (число),
      // то рассматривается следующий элемент
      stack.push(nextElement);
    }
  }

  double result = std::stof(stack.top());

  std::cout << "Результат: " << result << "\n";
}

bool isOperation(std::string element) {
  return element == "+" || element == "-" || element == "*" || element == "/";
}

double calculateOperation(char operation, double left, double right) {
  double operationResult;
  switch (operation)
  {
    case '+':
      operationResult = left + right;
      break;
    case '-':
      operationResult = left - right;
      break;
    case '*':
      operationResult = left * right;
      break;
    case '/':
      operationResult = left / right;
      break;
  }
  return operationResult;
}

// Покажет ошибку и выйдет из программы, если введено не число
void throwIfNotNumber(std::string value) {
  const int length = value.length();
 
  // Конвертация std::string в char*
  char* charArray = new char[length + 1];
  strcpy(charArray, value.c_str());

  char* end;
  double converted = strtod(charArray, &end);
  if (*end) {
    // Это не число
    throw std::invalid_argument( "Строка содержит недопустимые символы!" );
  }
  else {
    // Это число
  }
}