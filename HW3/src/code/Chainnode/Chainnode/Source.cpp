#include <iostream>
#include <cmath>

// 定義 ChainNode 節點類
class ChainNode {
public:
    int coefficient; // 係數
    int exponent;    // 次數
    ChainNode* next; // 下一個節點

    ChainNode(int c, int e) : coefficient(c), exponent(e), next(nullptr) {}
};

// 定義多項式類
class Polynomial {
public:
    ChainNode* head;

    Polynomial() : head(nullptr) {}

    // 添加節點
    void addTerm(int coefficient, int exponent) {
        if (coefficient == 0) return;  // 忽略係數為0的項
        ChainNode* newNode = new ChainNode(coefficient, exponent);
        if (!head) {
            head = newNode;
            head->next = head; // 環狀
        }
        else if (head->exponent < exponent) {
            ChainNode* tail = head;
            while (tail->next != head) {
                tail = tail->next;
            }
            newNode->next = head;
            head = newNode;
            tail->next = head; // 環狀
        }
        else {
            ChainNode* current = head;
            while (current->next != head && current->next->exponent > exponent) {
                current = current->next;
            }
            if (current->exponent == exponent) {
                current->coefficient += coefficient;
            }
            else {
                newNode->next = current->next;
                current->next = newNode;
                if (newNode->next == head) {
                    newNode->next = head;
                }
            }
        }
    }

    // 多項式加法
    Polynomial operator+(const Polynomial& other) const {
        Polynomial result;
        if (!head) return other;
        if (!other.head) return *this;
        ChainNode* a = head;
        ChainNode* b = other.head;
        do {
            result.addTerm(a->coefficient, a->exponent);
            a = a->next;
        } while (a != head);
        do {
            result.addTerm(b->coefficient, b->exponent);
            b = b->next;
        } while (b != other.head);
        return result;
    }

    // 多項式減法
    Polynomial operator-(const Polynomial& other) const {
        Polynomial result;
        if (!head) return other;
        if (!other.head) return *this;
        ChainNode* a = head;
        ChainNode* b = other.head;
        do {
            result.addTerm(a->coefficient, a->exponent);
            a = a->next;
        } while (a != head);
        do {
            result.addTerm(-b->coefficient, b->exponent);
            b = b->next;
        } while (b != other.head);
        return result;
    }

    // 多項式乘法
    Polynomial operator*(const Polynomial& other) const {
        Polynomial result;
        if (!head || !other.head) return result;
        for (ChainNode* a = head; a->next != head; a = a->next) {
            for (ChainNode* b = other.head; b->next != other.head; b = b->next) {
                result.addTerm(a->coefficient * b->coefficient, a->exponent + b->exponent);
            }
        }
        return result;
    }

    // 打印多項式
    void print() const {
        if (!head) return;
        ChainNode* current = head;
        bool first = true;
        do {
            if (!first && current->coefficient > 0) std::cout << " + ";
            if (current->exponent == 0) {
                std::cout << current->coefficient;
            }
            else if (current->exponent == 1) {
                std::cout << current->coefficient << "x";
            }
            else {
                std::cout << current->coefficient << "x^" << current->exponent;
            }
            first = false;
            current = current->next;
        } while (current != head);
        std::cout << std::endl;
    }

    // 計算多項式的值
    double evaluate(double x) const {
        double result = 0.0;
        if (!head) return result;
        ChainNode* current = head;
        do {
            result += current->coefficient * std::pow(x, current->exponent);
            current = current->next;
        } while (current != head);
        return result;
    }
};

int main() {
    Polynomial poly1;
    int terms;
    std::cout << "請輸入多項式1的項數: ";
    std::cin >> terms;

    for (int i = 0; i < terms; ++i) {
        int coef, exp;
        std::cout << "請輸入係數和次數 (例如：1 2 代表 1x^2): ";
        std::cin >> coef >> exp;
        poly1.addTerm(coef, exp);
    }

    Polynomial poly2;
    std::cout << "請輸入多項式2的項數: ";
    std::cin >> terms;

    for (int i = 0; i < terms; ++i) {
        int coef, exp;
        std::cout << "請輸入係數和次數 (例如：1 2 代表 1x^2): ";
        std::cin >> coef >> exp;
        poly2.addTerm(coef, exp);
    }

    Polynomial sum = poly1 + poly2;
    Polynomial diff = poly1 - poly2;
    Polynomial prod = poly1 * poly2;

    std::cout << "多項式1: ";
    poly1.print();
    std::cout << "多項式2: ";
    poly2.print();

    std::cout << "加法結果: ";
    sum.print();
    std::cout << "減法結果: ";
    diff.print();
    std::cout << "乘法結果: ";
    prod.print();

    double x;
    std::cout << "請輸入要帶入求值的數值: ";
    std::cin >> x;
    double value = poly1.evaluate(x);
    std::cout << "多項式1在 x = " << x << " 時的值為: " << value << std::endl;

    return 0;
}

