#include <iostream>
#include <list>
#include <queue>
using namespace std;

template<typename T>
class MyList {
public:
    class Iterator;
    MyList() : size(0), head(nullptr), tail(nullptr) {}
    ~MyList() { while (head != nullptr) pop_front(); }

    T GetSize() { return size; }
    T GetHead() { return head->data; }
    bool isEmpty() { return(head == nullptr); }
    void push_back(T data);
    void pop_front();
    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }

private:

    template<typename T>
    struct Node {
        T data;
        Node* next;
        Node* pred;

        Node(T data = T(), Node* pNext = nullptr, Node* pPred = nullptr) {
            this->data = data;
            this->next = pNext;
            this->pred = pPred;
        }
    };
    int size;
    Node<T>* head;
    Node<T>* tail;
};
template<typename T>
class MyList<T>::Iterator
{
    Node<T>* ptr;
public:

    Iterator() : ptr(nullptr) {};
    Iterator(Node<T>* p) : ptr(p) {}
    Iterator& operator ++ ()
    {
        if (ptr)
        {
            ptr = ptr->next; //Может быть еще и здесь кинуть исключение
            return *this;
        }
    }
    Iterator& operator -- ()
    {
        if (ptr)
        {
            ptr = ptr->pred; //Может быть еще и здесь кинуть исключение
            return *this;
        }
        else
        {
            ptr = tail;
            return *this;
        }
    }
    T& operator * ()
    {
        if (ptr)
            return ptr->data;

    }
    bool operator != (const Iterator& it) { return ptr != it.ptr; }
    bool operator == (const Iterator& it) { return ptr == it.ptr; }
};

template<typename T>
void MyList<T>::push_back(T data)
{
    Node<T>* temp = new Node<T>(data);
    temp->next = 0;
    temp->data = data;
    temp->pred = tail;

    if (tail != nullptr)
        tail->next = temp;

    if (size == 0) {
        head = tail = temp;
    }
    else {
        tail = temp;
    }
    size++;
}

template<typename T>
void MyList<T>::pop_front()
{
    Node<T>* temp = head;
    head = head->next;
    delete temp;
    size--;
    (size == 0) ? head = tail = nullptr : head->pred = nullptr;
}


class Graph {
private:
    bool* visited;
    bool* visit;
    bool* v;
    /////////////////
    bool** adjMatrix;
    MyList<int>* adjLists;
    int numVertices;

public:

    Graph(int numVertices)
    {
        this->numVertices = numVertices;
        adjMatrix = new bool* [numVertices];
        visited = new bool[numVertices];

        adjLists = new MyList<int>[numVertices];
        visit = new bool[numVertices];

        v = new bool[numVertices];

        for (int i = 0; i < numVertices; i++) {
            adjMatrix[i] = new bool[numVertices];
            visited[i] = false;
            visit[i] = false;
            v[i] = false;
            for (int j = 0; j < numVertices; j++)
                adjMatrix[i][j] = false;
        }
    }
    ~Graph() {
        for (int i = 0; i < numVertices; i++)
            delete[] adjMatrix[i];
        delete[] adjMatrix;
        delete[]visited;
        delete[]visit;
        delete[]v;
    }
    void toMatrix() {
        for (int i = 0; i < numVertices; i++) {
            cout << i << " : ";
            for (int j = 0; j < numVertices; j++) {
                cout << adjMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }
    void addEdge(int i, int j) {
        adjMatrix[i][j] = adjMatrix[j][i] = true;

        adjLists[i].push_back(j);
        adjLists[j].push_back(i);
    }

    void BFS_M(int start);
    void BFS_S(int start);
    void BFS(int start);

};

void Graph::BFS_M(int start) {
    queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        start = q.front();
        q.pop();
        cout << start << " ";

        for (int i = 0; i < numVertices; i++) {
            if (adjMatrix[start][i] == 1 && (!visited[i])) {
                q.push(i);
                visited[i] = true;
            }
        }
    }
}

void Graph::BFS_S(int start)
{
    queue<int> q;
    q.push(start);
    v[start] = true;

    while (!q.empty())
    {
        start = q.front();
        q.pop();
        cout << start << " ";

        for (auto i = adjLists[start].begin(); i != adjLists[start].end(); ++i)
            if (!v[*i])
            {
                v[*i] = true;
                q.push(*i);
            }
    }
}

void Graph::BFS(int start)
{
    MyList <int> q;
    q.push_back(start);
    visit[start] = true;

    while (!q.isEmpty()) {
        start = q.GetHead();
        cout << start << " ";
        q.pop_front();

        for (int i = 0; i < numVertices; i++) {
            if (adjMatrix[start][i] == 1 && (!visit[i])) {
                q.push_back(i);
                visit[i] = true;
            }
        }
    }
}


int main()
{
    setlocale(0, "");
    srand(time(0));

    int z = 7;
    Graph c(z);

    for (int i = 0; i < z; i++)
    {
        for (int j = i + 1; j < z; j++)
        {
            if (bool(rand() % 2))
                c.addEdge(i, j);
        }
    }
    cout << "Матрица смежности" << endl;
    c.toMatrix();
    int start;

    cout << endl << "Стартовая вершина > "; cin >> start;
    cout << "Порядок обхода: ";
    double time1 = clock();
    c.BFS_M(start);
    double time2 = clock();
    cout << endl << "Матрица смежности(queue): "<< (time2 - time1) / CLOCKS_PER_SEC;

    cout<<endl << "Порядок обхода: ";
    time1 = clock();
    c.BFS_S(start);
    time2 = clock();
    cout << endl << "Список смежности(queue): " << (time2 - time1) / CLOCKS_PER_SEC;

    cout<<endl << "Порядок обхода: ";
    time1 = clock();
    c.BFS(start);
    time2 = clock();
    cout << endl << "Матрица смежности(MyList): " << (time2 - time1) / CLOCKS_PER_SEC;

    return 0;
}

