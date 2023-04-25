#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <sstream>

// Имена функций, чтобы можно было их использовать до объявления
bool isOperation(std::string token);

int main() {
  // Мы ставим условие на разделение запятой,
  // так как числа могут состоять из нескольких символов,
  // поэтому просто разбить посимвольно не получится.
  // Для разделения дробных чисел следует использовать точку (.)
  std::cout << "Введите выражение в обратной польской записи (разделяя запятой): ";
  std::string expression;
  std::getline(std::cin, expression); // берет полностью строку до конца, а не до пробелов
  std::cout << "\n";

  // Разбить строку на вектор символов (по запятым)
  std::vector<std::string> tokens;
  std::istringstream tokensStream(expression); // разделяет по пробелам
  std::string token;
  while (std::getline(tokensStream, token, ',')) {
    tokens.push_back(token);
  }

  // Вывести введенные элементы (для дебага)
  std::cout << "Введенные элементы: \n";
  for (auto& token : tokens) {
    std::cout << token << std::endl;
  }
  std::cout << "\n";

  // Посчитать результат

  // Стэк для хранения операций,
  // тип string, так как в нем могут храниться и числа, и операции
  // а числа могут состоять из нескольких символов, поэтому тип char не подходит
  std::stack<std::string> stack;

  while(!tokens.empty()) {
    // Обратная польская запись просматривается слева направо
    // Получить первый элемент
    std::string nextToken = tokens.front();
    // Удалить первый элемент из вектора символов
    tokens.erase(tokens.begin());

    if (isOperation(nextToken)) {
      // Если рассматриваемый элемент — знак операции,
      // то выполняется эта операция над операндами (числами), записанными левее знака операции

      float rightNumber = std::stof(stack.top()); // получить верхний элемент и перевести его в float
      stack.pop(); // удалить верхний элемент

      float leftNumber = std::stof(stack.top()); // получить верхний элемент и перевести его в float
      stack.pop(); // удалить верхний элемент

      // Провести операцию над числами
      char operation = nextToken[0];
      float operationResult;
      switch (operation)
      {
        case '+':
          operationResult = leftNumber + rightNumber;
          break;
        case '-':
          operationResult = leftNumber - rightNumber;
          break;
        case '*':
          operationResult = leftNumber * rightNumber;
          break;
        case '/':
          operationResult = leftNumber / rightNumber;
          break;
      }

      stack.push(std::to_string(operationResult));
    } else {
      // Если рассматриваемый элемент - операнд (число),
      // то рассматривается следующий элемент
      stack.push(nextToken);
    }
  }

  float result = std::stof(stack.top());

  std::cout << "Результат: " << result << "\n";
}

bool isOperation(std::string token) {
  return token == "+" || token == "-" || token == "*" || token == "/";
}