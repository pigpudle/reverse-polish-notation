#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <sstream>

// Струтуры
struct Node {
  std::string value;
  Node* left;
  Node* right;
  Node(std::string val) : value(val), left(NULL), right(NULL) {}
};

// Имена функций, чтобы можно было их использовать до объявления
bool isOperation(std::string token);
void symPrint(Node* tree);
float calculateTree(Node* tree);

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

  // Преобразовать вектор в дерево
  Node* root = NULL;
  Node* prevOpNode = NULL;
  Node* nextLeft = NULL;
  Node* nextRight = NULL;
  for (auto& token : tokens) {
    if (isOperation(token)) {
      // Звено операции объединяет под собой левое и правое звенья, которые содержат числа
      // (либо другие поддеревья, которые будут вычисляться от листьев к корням)
      Node* operationNode = new Node(token);
      operationNode->left = nextLeft;
      operationNode->right = nextRight;
      // Очистить значения
      nextLeft = NULL;
      nextRight = NULL;
      prevOpNode = operationNode;
    } else {
      // Сформировать новое звено с числом
      Node* node = new Node(token);
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
    float operationResult;
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
  } else {
    return std::stof(tree->value);
  }
}

bool isOperation(std::string token) {
  return token == "+" || token == "-" || token == "*" || token == "/";
}