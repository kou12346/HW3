#include <iostream>
#include <cmath>
using namespace std;

////////////////////////////ChainNode//////////////////////////////////////
template<class T>
class Chain;
template<class T>
class ChainNode {
    friend class Chain<T>;
private:
    T data;
    ChainNode<T>* link;
public:
    ChainNode() {}
    ChainNode(const T& data) {
        this->data = data;
        this->link = nullptr;
    }
    ChainNode(const T& data, ChainNode<T>* link) {
        this->data = data;
        this->link = link;
    }
};

////////////////////////////Chain//////////////////////////////////////
template<class T>
class Chain {
public:
    Chain() {
        first = new ChainNode<T>;
        first->link = first;
        itemCount = 0;
    }
    ~Chain() { clear(); delete first; }

    bool IsEmpty() const { return first->link == first; }
    void sethead(const T& e) { first->data = e; first->link = first; }

    class ChainIterator {
    public:
        friend class ChainNode<T>;
        ChainIterator(ChainNode<T>* startNode = nullptr) { current = startNode; }
        T& operator*() const { return current->data; }
        T* operator->() const { return &current->data; }
        ChainIterator& operator++();
        ChainIterator operator++(int);
        bool operator!=(const ChainIterator& right) const {
            return current != right.current;
        }
        bool operator==(const ChainIterator& right) const {
            return current == right.current;
        }
    private:
        ChainNode<T>* current;
    };

    ChainIterator begin() const { return ChainIterator(first->link); }
    ChainIterator end() const { return ChainIterator(first); }

    void InsertBack(const T& e);
    int getCount() const { return itemCount; }
    void clear();

private:
    ChainNode<T>* first;
    int itemCount;
};

template<class T>
typename Chain<T>::ChainIterator& Chain<T>::ChainIterator::operator++() {
    current = current->link;
    return *this;
}

template<class T>
typename Chain<T>::ChainIterator Chain<T>::ChainIterator::operator++(int) {
    ChainIterator old = *this;
    current = current->link;
    return old;
}

template<class T>
void Chain<T>::InsertBack(const T& e) {
    itemCount++;
    ChainNode<T>* newNode = new ChainNode<T>(e, first);
    ChainNode<T>* temp = first;
    while (temp->link != first) {
        temp = temp->link;
    }
    temp->link = newNode;
}

template<class T>
void Chain<T>::clear() {
    ChainNode<T>* current = first->link;
    while (current != first) {
        ChainNode<T>* temp = current;
        current = current->link;
        delete temp;
    }
    first->link = first;
    itemCount = 0;
}

////////////////////////////Polynomial//////////////////////////////////////
class Polynomial;

class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream& os, const Polynomial& p);
    friend istream& operator>>(istream& is, Polynomial& p);
    Term Set(float c, int e) { coef = c; exp = e; return *this; }
private:
    float coef;
    int exp;
};

class Polynomial {
private:
    Chain<Term> poly;
public:
    Polynomial() {
        Term tmp;
        poly.sethead(tmp.Set(-1, -1));
    }
    Polynomial(const Polynomial& a) : poly(a.poly) {}
    ~Polynomial() {}

    void newPoly(float c, int e) {
        Term tmp;
        poly.InsertBack(tmp.Set(c, e));
    }

    Polynomial operator+(const Polynomial& b) const;
    Polynomial operator-(const Polynomial& b) const;
    Polynomial operator*(const Polynomial& b) const;
    float Evaluate(float x) const;

    friend ostream& operator<<(ostream& os, const Polynomial& p);
    friend istream& operator>>(istream& is, Polynomial& p);
};

Polynomial Polynomial::operator+(const Polynomial& b) const {
    Term tmp;
    Chain<Term>::ChainIterator ai = poly.begin(), bi = b.poly.begin();
    Polynomial c;
    while (1) {
        if (ai->exp == bi->exp) {
            if (ai->exp == -1) return c;
            float sum = ai->coef + bi->coef;
            if (sum) {
                c.poly.InsertBack(tmp.Set(sum, ai->exp));
            }
            ai++;
            bi++;
        }
        else if (ai->exp < bi->exp) {
            c.poly.InsertBack(tmp.Set(bi->coef, bi->exp));
            bi++;
        }
        else {
            c.poly.InsertBack(tmp.Set(ai->coef, ai->exp));
            ai++;
        }
    }
    return c;
}

Polynomial Polynomial::operator-(const Polynomial& b) const {
    Term tmp;
    Chain<Term>::ChainIterator ai = poly.begin(), bi = b.poly.begin();
    Polynomial c;
    while (1) {
        if (ai->exp == bi->exp) {
            if (ai->exp == -1) return c;
            float sum = ai->coef - bi->coef;
            if (sum) {
                c.poly.InsertBack(tmp.Set(sum, ai->exp));
            }
            ai++;
            bi++;
        }
        else if (ai->exp < bi->exp) {
            c.poly.InsertBack(tmp.Set(-bi->coef, bi->exp));
            bi++;
        }
        else {
            c.poly.InsertBack(tmp.Set(ai->coef, ai->exp));
            ai++;
        }
    }
    return c;
}

int hash_function(int x, int t, int* u) {
    if (u[x % t] == 0 || u[x % t] - 1 == x) {
        u[x % t] = x + 1;
        return x % t;
    }
    else {
        for (int i = 1; i < t; i++) {
            if (u[(x + i) % t] == 0) {
                u[(x + i) % t] = x + 1;
                return (x + i) % t;
            }
        }
    }
    return 0;
}

Polynomial Polynomial::operator*(const Polynomial& b) const {
    Polynomial res;
    int use_cap = poly.getCount() * b.poly.getCount();
    bool* use = new bool[use_cap]();
    int* hash = new int[use_cap]();
    Chain<Term>::ChainIterator ai = poly.begin(), bi = b.poly.begin();
    for (int i = 0; i < poly.getCount(); i++) {
        bi = b.poly.begin();
        for (int j = 0; j < b.poly.getCount(); j++) {
            int mult_exp = ai->exp + bi->exp;
            int hash_index = hash_function(mult_exp, use_cap, hash);
            if (use[hash_index]) {
                Term tmp;
                tmp.coef = ai->coef * bi->coef;
                tmp.exp = mult_exp;
                Polynomial temp;
                temp.poly.InsertBack(tmp);
                res = res + temp;
            }
            else {
                Term tmp;
                tmp.coef = ai->coef * bi->coef;
                tmp.exp = mult_exp;
                res.poly.InsertBack(tmp);
                use[hash_index] = true;
            }
            bi++;
        }
        ai++;
    }
    delete[] use;
    delete[] hash;
    return res;
}

float Polynomial::Evaluate(float x) const {
    Chain<Term>::ChainIterator ai = poly.begin();
    float total = 0;
    while (ai != poly.end()) {
        total += ai->coef * powf(x, ai->exp);
        ai++;
    }
    return total;
}

ostream& operator<<(ostream& os, const Polynomial& p) {
    Chain<Term>::ChainIterator o = p.poly.begin();
    bool first = true;
    while (o != p.poly.end()) {
        if (!first && o->coef > 0) os << "+";
        if (o->exp == 0)
            os << abs(o->coef);
        else if (o->exp == 1)
            os << abs(o->coef) << "x";
        else
            os << abs(o->coef) << "x^" << o->exp;
        ++o;
        first = false;
    }
    return os;
}

istream& operator>>(istream& is, Polynomial& p) {
    int numTerms;
    is >> numTerms;
    for (int i = 0; i < numTerms; ++i) {
        float coeftmp;
        int exptmp;
        is >> coeftmp >> exptmp;
        p.newPoly(coeftmp, exptmp);
    }
    return is;
}

int main() {
    // 宣告三個多項式物件
    Polynomial poly1; Polynomial poly2; Polynomial poly3;
    cout << "輸入poly1 (格式: 5 3 4 2 3 6 2 1 1 2 0):";
    cin >> poly1;
    cout << "輸入poly2 (格式: 4 6 5 3 3 2 1 -1 0):";
    cin >> poly2; // 相加
    poly3 = (poly1 + poly2); cout << "(" << poly1 << ") + (" << poly2 << ") = ";
    cout << poly3 << endl; // 相減
    cout << "(" << poly1 << ") - (" << poly2 << ") = ";
    poly3 = (poly1 - poly2); cout << poly3 << endl; // 相乘
    cout << "(" << poly1 << ") * (" << poly2 << ") = ";
    poly3 = (poly1 * poly2); cout << poly3 << endl; // 求值
    cout << "Evaluate(2):";
    cout << poly3.Evaluate(2) << endl;
    cout << "poly1=poly2:" << endl;
    poly1 = poly2;
    cout << poly1 << endl << poly2;
    return 0;
}