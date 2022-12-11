#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <clocale>
using namespace std;

typedef int T;
#define compLT(a,b) (a < b)
#define compEQ(a,b) (a == b)


typedef enum { BLACK, RED } nodeColor;

typedef struct Node_ {
    struct Node_* left;
    struct Node_* right;
    struct Node_* parent;
    nodeColor color;
    T data;
} Node;

#define NIL &sentinel           
Node sentinel = { NIL, NIL, 0, BLACK, 0 };

Node* root = NIL;

void rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != NIL) y->left->parent = x;
    if (y != NIL) y->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
    }
    else {
        root = y;
    }
    y->left = x;
    if (x != NIL) x->parent = y;
}

void rotateRight(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right != NIL) y->right->parent = x;

    if (y != NIL) y->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
    }
    else {
        root = y;
    }

    y->right = x;
    if (x != NIL) x->parent = y;
}

void insertFixup(Node* x)
{
    while (x != root && x->parent->color == RED) {
        if (x->parent == x->parent->parent->left) {
            Node* y = x->parent->parent->right;
            if (y->color == RED) {
                x->parent->color = BLACK;
                y->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            }
            else {
                if (x == x->parent->right) {
                    x = x->parent;
                    rotateLeft(x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rotateRight(x->parent->parent);
            }
        }
        else {
            Node* y = x->parent->parent->left;
            if (y->color == RED) {
                x->parent->color = BLACK;
                y->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            }
            else {
                if (x == x->parent->left) {
                    x = x->parent;
                    rotateRight(x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rotateLeft(x->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

Node* insertNode(T data)
{
    Node* current, * parent, * x;
    current = root;
    parent = 0;
    while (current != NIL) {
        if (compEQ(data, current->data)) return (current);
        parent = current;
        current = compLT(data, current->data) ?
            current->left : current->right;
    }

    x = new Node;
    x->data = data;
    x->parent = parent;
    x->left = NIL;
    x->right = NIL;
    x->color = RED;

    if (parent) {
        if (compLT(data, parent->data))
            parent->left = x;
        else
            parent->right = x;
    }
    else {
        root = x;
    }

    insertFixup(x);
    return(x);
}

void deleteFixup(Node* x)
{
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateLeft(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotateRight(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rotateLeft(x->parent);
                x = root;
            }
        }
        else {
            Node* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateRight(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rotateLeft(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rotateRight(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

void deleteNode(Node* z) {
    Node* x, * y;

    if (!z || z == NIL) return;


    if (z->left == NIL || z->right == NIL) {
        y = z;
    }
    else {
        y = z->right;
        while (y->left != NIL) y = y->left;
    }

    if (y->left != NIL)
        x = y->left;
    else
        x = y->right;

    x->parent = y->parent;
    if (y->parent)
        if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
    else
        root = x;

    if (y != z) z->data = y->data;


    if (y->color == BLACK)
        deleteFixup(x);

    free(y);
}

Node* findNode(T data) {


    Node* current = root;
    while (current != NIL)
        if (compEQ(data, current->data))
            return (current);
        else
            current = compLT(data, current->data) ?
            current->left : current->right;
    return(0);
}

void info(Node* a)
{
    if (a == NIL) return;
    info(a->left);
    cout << a->data << " ";
    info(a->right);
}

void rbtree_print(Node* x, int level)
{
    if (x)
    {
        if (x == NIL)
            return;
        rbtree_print(x->left, level + 1);

        for (int i = 0; i < level; i++)
            cout << "   ";
        if (x->color == 0) cout << "(b)";
        else cout << "(r)";
        cout << x->data << endl;
        rbtree_print(x->right, level + 1);
    }
}

int GetIntFromCin(string_view prompt) {
    double input{ 0x0 };
    while (true) {
        input = 0x0;
        cout << prompt << " : " << flush;
        if ((std::cin >> input).good()) {
            if (input == static_cast<int>(input)) {
                break;
            }
            cout << std::endl << "Вы ввели не целое значение." << endl << flush;
        }
        if (cin.fail()) {
            cin.clear();
            cout << endl << "Неверно попробуйте еще раз" << endl << flush;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return static_cast<int>(input);
}

int GetIntFromCinWithLimits(string_view prompt, int min = numeric_limits<int>::min(), int max = numeric_limits<int>::max()) {
    int ret{ 0 };
    while (true) {
        ret = GetIntFromCin(prompt);
        if (ret < min || ret > max) {
            cout << endl << "Недопустимый предел!" << endl << flush;
            continue;
        }
        break;
    }

    return ret;
}
void main() {
    Node* t = NULL;
    setlocale(LC_ALL, "RUS");
    int x, y, q;

    cout << "Выберите действие\n"
        "1 - Вывести дерево\n"
        "2 - Вставить элемент\n"
        "3 - Удалить элемент\n"
        "4 - Выход\n";
    cout << endl;
    x = GetIntFromCinWithLimits("Введите цифру 1 - 4", 1, 4);
    cout << endl;
    while (x != 4)
    {
        switch (x)
        {
        case 1:
            if (root == nullptr)
                cout << "Дерево не сформировано" << endl << endl;
            else
            {
                rbtree_print(root, 0);
                cout << endl;
            }
            break;
        case 2:
            cout << endl;
            y = GetIntFromCinWithLimits("Введите вершину", -9999999, 99999999);
            insertNode(y);
            while (root->parent != NULL)
                root = root->parent;
            cout << endl;
            break;
        case 3:
            cout << endl;
            if (root == nullptr)
                cout << "Дерево не сформировано" << endl << endl;
            else
            {
                q = GetIntFromCinWithLimits("Введите какую вершину хотите удалить", -9999999, 99999999);
                while ((t = findNode(q)) == NULL)
                {
                    cout << "Такой вершины нет" << endl << endl;
                    q = GetIntFromCinWithLimits("Введите какую вершину хотите удалить", -9999999, 99999999);
                }
                deleteNode(t);
            }
            break;
        }
        cout << "Выберите действие\n"
            "1 - Вывести дерево\n"
            "2 - Вставить элемент\n"
            "3 - Удалить элемент\n"
            "4 - Выход\n";
        cout << endl;
        x = GetIntFromCinWithLimits("Введите цифру 1 - 4", 1, 4);
    }

}