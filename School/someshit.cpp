#include <iostream>
#include <memory>

class LinkedList {
private:
    struct Node {
        int data;
        std::unique_ptr<Node> next;
        Node(int d) : data(d), next(nullptr) {}
    };

    std::unique_ptr<Node> head;

public:
    // push to front (like your add/add1)
    void push_front(int value) {
        auto newNode = std::make_unique<Node>(value);
        newNode->next = std::move(head);
        head = std::move(newNode);
    }

    // print list
    void print() const {
        const Node* curr = head.get();
        while (curr) {
            std::cout << curr->data << " ";
            curr = curr->next.get();
        }
        std::cout << "\n";
    }

    // append another list to THIS list
    void append_list(LinkedList& other) {
        if (!head) {
            head = std::move(other.head);
            return;
        }

        Node* curr = head.get();
        while (curr->next) {
            curr = curr->next.get();
        }
        curr->next = std::move(other.head);
    }

    // remove first occurrence of a value
    void remove_value(int value) {
        // case 1: deleting the head
        while (head && head->data == value) {
            head = std::move(head->next);
            return;
        }

        // case 2: deleting deeper in the list
        Node* curr = head.get();
        while (curr && curr->next) {
            if (curr->next->data == value) {
                curr->next = std::move(curr->next->next);
                return;
            }
            curr = curr->next.get();
        }
    }
};

int main() {
    LinkedList list_p;
    LinkedList list_q;

    // build your lists
    list_p.push_front(10);
    list_p.push_front(12);

    list_q.push_front(2);
    list_q.push_front(16);
    list_q.push_front(11);

    std::cout << "List q: ";
    list_q.print();

    std::cout << "List p: ";
    list_p.print();

    // join lists
    list_q.append_list(list_p);

    std::cout << "Joined list: ";
    list_q.print();

    // remove 10
    list_q.remove_value(10);

    std::cout << "After removing 10: ";
    list_q.print();

    return 0;
}
