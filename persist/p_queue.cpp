#include <iostream>
#include <memory>
#include <functional>

using namespace std;

template <typename T>
struct Node {
    T value;
    shared_ptr<Node<T>> next;

    Node(T val, shared_ptr<Node<T>> nxt = nullptr)
        : value(val), next(nxt)
    {}
};

template <typename T>
struct LazyNode {
    T value;
    shared_ptr<LazyNode<T>> next;
    function<shared_ptr<LazyNode<T>>()> thunk;
    bool evaluated;

    LazyNode(T val, function<shared_ptr<LazyNode<T>>()> th)
        : value(val), next(nullptr), thunk(th), evaluated(false)
    {}
};

template <typename T>
shared_ptr<LazyNode<T>> evalNext(const shared_ptr<LazyNode<T>>& node)
{
    if (!node)
        return nullptr;
    if (!node->evaluated) {
        node->next = node->thunk ? node->thunk() : nullptr;
        node->evaluated = true;
    }
    return node->next;
}

template <typename T>
shared_ptr<LazyNode<T>> makeLazyNode(const T& value, function<shared_ptr<LazyNode<T>>()> thunk)
{
    return make_shared<LazyNode<T>>(value, thunk);
}

template <typename T>
shared_ptr<LazyNode<T>> lazyReverse(shared_ptr<Node<T>> head, shared_ptr<LazyNode<T>> acc)
{
    if (!head)
        return acc;

    return makeLazyNode<T>(head->value, [head, acc]() {
        return lazyReverse(head->next, acc);
    });
}

template <typename T>
shared_ptr<LazyNode<T>> rotate(shared_ptr<LazyNode<T>> front, shared_ptr<Node<T>> rear, shared_ptr<LazyNode<T>> acc)
{
    if (!front)
        return lazyReverse(rear, acc);

    auto newAcc = makeLazyNode<T>(rear->value, [acc]() {
        return acc;
    });

    return makeLazyNode<T>(front->value, [front, rear, newAcc]() {
        return rotate(evalNext(front), rear->next, newAcc);
    });
}

template <typename T>
class persistentQueue
{
private:
    shared_ptr<LazyNode<T>> front;
    shared_ptr<Node<T>> rear;
    shared_ptr<LazyNode<T>> schedule;
    int frontSize;
    int rearSize;

    persistentQueue(shared_ptr<LazyNode<T>> f,
                    shared_ptr<Node<T>> r,
                    shared_ptr<LazyNode<T>> s,
                    int fsize,
                    int rsize)
        : front(f), rear(r), schedule(s), frontSize(fsize), rearSize(rsize)
    {}

    persistentQueue exec() const
    {
        if (schedule)
            return persistentQueue(front, rear, evalNext(schedule), frontSize, rearSize);
        return *this;
    }

    persistentQueue check() const
    {
        if (rearSize <= frontSize)
            return exec();

        auto newFront = rotate(front, rear, shared_ptr<LazyNode<T>>());
        return persistentQueue(newFront, nullptr, newFront, frontSize + rearSize, 0).exec();
    }

public:
    persistentQueue()
        : front(nullptr), rear(nullptr), schedule(nullptr), frontSize(0), rearSize(0)
    {}

    bool isEmpty() const
    {
        return frontSize + rearSize == 0;
    }

    persistentQueue enqueue(T x) const
    {
        auto newRear = make_shared<Node<T>>(x, rear);
        return persistentQueue(front, newRear, schedule, frontSize, rearSize + 1).check();
    }

    persistentQueue dequeue() const
    {
        if (isEmpty())
            throw runtime_error("Queue is empty");

        return persistentQueue(evalNext(front), rear, schedule, frontSize - 1, rearSize).check();
    }

    T peek() const
    {
        if (isEmpty())
            throw runtime_error("Queue is empty");

        return front->value;
    }

    void print() const
    {
        auto temp = front;
        while (temp) {
            cout << temp->value << " ";
            temp = evalNext(temp);
        }

        shared_ptr<Node<T>> revRear = nullptr;
        auto rearTemp = rear;
        while (rearTemp) {
            revRear = make_shared<Node<T>>(rearTemp->value, revRear);
            rearTemp = rearTemp->next;
        }

        auto revTemp = revRear;
        while (revTemp) {
            cout << revTemp->value << " ";
            revTemp = revTemp->next;
        }

        cout << endl;
    }
};

int main()
{
    persistentQueue<int> q0;
    cout << "q0 (empty): "; q0.print();

    auto q1 = q0.enqueue(10);
    cout << "q0 (unchanged): "; q0.print();
    cout << "q1 (q0 + 10): "; q1.print();

    auto q2 = q1.enqueue(20);
    cout << "q1 (unchanged): "; q1.print();
    cout << "q2 (q1 + 20): "; q2.print();

    auto q3 = q2.enqueue(30);
    cout << "q2 (unchanged): "; q2.print();
    cout << "q3 (q2 + 30): "; q3.print();

    auto q4 = q3.dequeue();
    cout << "q3 (unchanged): "; q3.print();
    cout << "q4 (q3 dequeue): "; q4.print();

    auto q5 = q4.enqueue(40);
    cout << "q4 (unchanged): "; q4.print();
    cout << "q5 (q4 + 40): "; q5.print();

    auto q6 = q5.dequeue();
    cout << "q5 (unchanged): "; q5.print();
    cout << "q6 (q5 dequeue): "; q6.print();

    cout << "\nAll versions:\n";
    cout << "q0: "; q0.print();
    cout << "q1: "; q1.print();
    cout << "q2: "; q2.print();
    cout << "q3: "; q3.print();
    cout << "q4: "; q4.print();
    cout << "q5: "; q5.print();
    cout << "q6: "; q6.print();

    cout << "\nq3.peek() = " << q3.peek() << endl;
    cout << "q4.peek() = " << q4.peek() << endl;
    cout << "q6.peek() = " << q6.peek() << endl;

    return 0;
}
