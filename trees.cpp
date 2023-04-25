#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cstring>

// Струтуры
struct Node {
  std::string value;
  Node* left;
  Node* right;
  Node(std::string val) : value(val), left(NULL), right(NULL) {}
};

// Имена функций, чтобы можно было их использовать до объявления
bool isOperation(std::string element);
void symPrint(Node* tree);
float calculateTree(Node* tree);
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

  // Разбить строку на вектор символов (по пробелам)
  std::vector<std::string> elements;
  std::istringstream elementsStream(input); // разделяет по пробелам
  std::string element;
  while (std::getline(elementsStream, element, ' ')) {
    elements.push_back(element);
  }

  // Преобразовать вектор в дерево
  Node* root = NULL;
  Node* prevOpNode = NULL;
  Node* nextLeft = NULL;
  Node* nextRight = NULL;
  for (auto& element : elements) {
    if (isOperation(element)) {
      // Звено операции объединяет под собой левое и правое звенья, которые содержат числа
      // (либо другие поддеревья, которые будут вычисляться от листьев к корням)
      Node* operationNode = new Node(element);
      operationNode->left = nextLeft;
      operationNode->right = nextRight;
      // Очистить значения
      nextLeft = NULL;
      nextRight = NULL;
      prevOpNode = operationNode;
    } else {
      // Ошибка, если не число
      throwIfNotNumber(element);

      // Сформировать новое звено с числом
      Node* node = new Node(element);
      // Сохранить звено так, что оно потом будет объединено под одной операцией
      if (prevOpNode) {
        nextLeft = prevOpNode;
        prevOpNode = NULL;
      }

      if (!nextLeft) {
        nextLeft = node;
      } else {
        nextRight = node;
      }
    }
  }

  root = prevOpNode;

  // Вывести дерево выражений
  std::cout << "Дерево выражения (инфиксная запись): ";
  symPrint(root);
  std::cout << "\n";

  // Расчет выражения  
  float result = calculateTree(root);
  std::cout << "Результат: " << result << "\n";

  return 0;
}

// Симметричный обход дерева с выражениями,
// чтобы вывести его в консоль
void symPrint(Node* tree) {
  if (tree != NULL) {
    // Расставляем скобки вокруг поддеревьев
    // Таким образом мы автоматически выводим инфиксную запись
    // изначального выражения
    tree->left && std::cout << "(";
    symPrint(tree->left);

    // Выводим пробелы только вокруг операций
    bool useSpaces = isOperation(tree->value);
    std::string suffixString = useSpaces ? " " : "";
    std::cout << suffixString << tree->value << suffixString;

    symPrint(tree->right);
    tree->left && std::cout << ")";
  }
}

// Рекурсивный расчет дерева-выражения
float calculateTree(Node* tree) {
  if (tree == NULL) return 0;
  
  if(isOperation(tree->value)) {
    float left = calculateTree(tree->left);
    float right = calculateTree(tree->right);

    char operation = tree->value[0];
    return calculateOperation(operation, left, right);
  } else {
    return std::stof(tree->value);
  }
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