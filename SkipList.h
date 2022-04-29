//
// Created by charles on 29/04/22.
//

#ifndef CPP_UTILS_SKIPLIST_H
#define CPP_UTILS_SKIPLIST_H

#include <vector>

template<typename T, typename V>
class SkipList {
private:
    class Node {
    public:
        Node() = default;

        Node(const T &data, const V &val) {
            _data = data;
            _val = val;
        }

        ~Node() = default;

        std::vector<Node *> _list = {};
        T _data = {};
        V _val = {};
    };

    class Iterator {

    };

public:
    explicit SkipList(bool (*comparator)(const V &, const V &)) {
        _head = new Node();
        _head->_list.push_back(nullptr);
        _list_level = 0;
        _comparator = comparator;
    }

    ~SkipList() {
        Node *tmp1;
        Node *tmp2;
        tmp1 = _head;
        while (tmp1 != nullptr) {
            tmp2 = tmp1->_list[0];
            delete tmp1;
            tmp1 = tmp2;
        }
    }

    void set_ratio(const int &ratio) {
        _ratio = ratio;
    }

    // Add, Remove, Find
    void add(const T &data, const V &val) {
        std::cout << "Inserting [" << val << ":" << data << "]\n";
        Node *new_node = new Node(data, val);

        std::vector<Node *> path_to(_list_level + 1, nullptr);
        size_t level = _list_level;

        Node *insert_after = _head;
        Node *lookup = insert_after->_list[_list_level];

        while (level != (unsigned long) -1) {
            if (lookup == nullptr || _comparator(val, lookup->_val)) {
                path_to[level] = insert_after;
                level--;
                lookup = insert_after->_list[level];
            } else {
                insert_after = lookup;
                lookup = lookup->_list[level];
            }
        }
        new_node->_list.push_back(path_to[0]->_list[0]);
        path_to[0]->_list[0] = new_node;
        level = 1;
        while (level <= _list_level && (rand() % _ratio) == 0) {
            new_node->_list.push_back(path_to[level]->_list[level]);
            path_to[level]->_list[level] = new_node;
            level++;
        }
        if (level > _list_level && (rand() % _ratio) == 0) {
            new_node->_list.push_back(nullptr);
            _head->_list.push_back(new_node);
            _list_level++;
        }
    }

    bool empty() const {
        return _head->_list[0] == nullptr;
    }

    T pop() {
        T result = _head->_list[0]->_data;
        Node *tmp = _head->_list[0];
        for (size_t i = 0; i < _head->_list.size(); ++i) {
            _head->_list[i] = tmp->_list[i];
        }
        delete tmp;
        if (_head->_list[_list_level] == nullptr) {
            _list_level--;
            _head->_list.pop_back();
        }
        return result;
    }

    void display() const {
        std::cout << "SL max level : " << _list_level << "\n";
        Node *tmp = _head;
        while (tmp != nullptr) {
            std::cout << "\t" << tmp << "[" << tmp->_val << ":" << tmp->_data
                      << ":l=" << tmp->_list.size() << "(";
            for (auto &v: tmp->_list) {
                std::cout << v << ",";
            }
            std::cout << ")]\n";
            tmp = tmp->_list[0];
        }
        std::cout << "\n";
    }

private:
    // Members
    Node *_head = {};
    size_t _list_level = {};

    bool (*_comparator)(const V &, const V &) = {};

    int _ratio = 2;
};

#endif //CPP_UTILS_SKIPLIST_H
