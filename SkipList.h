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

public:
    class Iterator {
    public:
        explicit Iterator(Node *at) : _at(at) {}

        Iterator(const Iterator &other) : _at(other._at) {}

        ~Iterator() = default;

        Iterator &operator++() {
            _at = _at->_list[0];
            return *this;
        }

        bool operator==(const Iterator &other) const {
            return _at == other._at;
        }

        bool operator!=(const Iterator &other) const {
            return _at != other._at;
        }

        T &operator*() {
            return _at->_data;
        }

    private:
        Node *_at;
    };

public:
    explicit SkipList() {
        _head = new Node();
        _head->_list.push_back(nullptr);
        _list_level = 0;
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

    Iterator begin() {
        return Iterator(_head->_list[0]);
    }

    Iterator end() {
        return Iterator(nullptr);
    }

    SkipList &set_ratio(const int &ratio) {
        _ratio = ratio;
        return *this;
    }

    // Add, Pop
    void add(const T &data, const V &val) {
        Node *new_node = new Node(data, val);

        std::vector<Node *> path_to(_list_level + 1, nullptr);
        size_t level = _list_level;

        Node *insert_after = _head;
        Node *lookup = insert_after->_list[_list_level];

        while (level != (unsigned long) -1) {
            if (lookup == nullptr || val < lookup->_val) {
                path_to[level] = insert_after;
                level--;
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
        for (size_t i = 0; i < tmp->_list.size(); ++i) {
            _head->_list[i] = tmp->_list[i];
        }
        delete tmp;
        if (_head->_list[_list_level] == nullptr && _list_level > 0) {
            _list_level--;
            _head->_list.pop_back();
        }
        return result;
    }

private:
    Node *_head = {};
    size_t _list_level = {};
    int _ratio = 10;
};

#endif //CPP_UTILS_SKIPLIST_H
