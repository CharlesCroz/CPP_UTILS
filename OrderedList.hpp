//
// Created by charles on 11/08/22.
//

#ifndef LISTSGRAPH_ORDEREDLIST_HPP
#define LISTSGRAPH_ORDEREDLIST_HPP

template<typename T>
class OrderedList {
private:
    struct Cell {
        T val;
        Cell *next = nullptr;
    };
    Cell *head = nullptr;
public:
    class iterator {
    public:
        Cell *current;
        Cell **source;
    public:
        iterator &operator++() {
            if (current != nullptr) {
                source = &(current->next);
                current = current->next;
            }
            return *this;
        }

        void remove() {
            if (current != nullptr) {
                (*source) = current->next;
                delete current;
                current = *source;
            }
        }

        bool operator==(const iterator &other) const {
            return current == other.current;
        }

        bool operator!=(const iterator &other) const {
            return current != other.current;
        }

        T &operator*() {
            return current->val;
        }

        void insertBefore(const T &elem) {
            Cell *newCell = new Cell;
            newCell->val = elem;
            newCell->next = current;
            (*source) = newCell;
            source = &(newCell->next);
        }

        void insertAfter(const T &elem) {
            Cell *newCell = new Cell;
            newCell->val = elem;
            if (current != nullptr) {
                newCell->next = current->next;
                current->next = newCell;
            } else {
                *source = newCell;
            }
        }
    };

    class const_iterator {
    public:
        Cell *current;
    public:
        const_iterator &operator++() {
            if (current != nullptr) {
                current = current->next;
            }
            return *this;
        }

        bool operator==(const const_iterator &other) const {
            return current == other.current;
        }

        bool operator!=(const const_iterator &other) const {
            return current != other.current;
        }

        const T &operator*() const {
            return current->val;
        }
    };

public:
    OrderedList() = default;

    OrderedList(const OrderedList &other);

    OrderedList(OrderedList &&other) noexcept;

    OrderedList &operator=(const OrderedList &other);

    OrderedList &operator=(OrderedList &&other) noexcept;

    ~OrderedList();

    void add(const T &elem);

    void addUniquely(const T &elem);

    bool removeOne(const T &elem);

    bool removeAll(const T &elem);

    void addAll(const OrderedList &other);

    bool contains(const T &elem) const;

    bool isEmpty() const;

    T &getFirst() const;

    bool popFirst();

    iterator begin();

    inline iterator end() const;

    const_iterator cbegin() const;

    inline const_iterator cend() const;

    void display() const;

};

template<typename T>
OrderedList<T>::~OrderedList() {
    Cell *current = head;
    while (current != nullptr) {
        Cell *tmp = current->next;
        delete (current);
        current = tmp;
    }
}

template<typename T>
void OrderedList<T>::add(const T &elem) {
    Cell **insertAt = &head;
    while (*insertAt != nullptr && (**insertAt).val < elem) {
        insertAt = &((**insertAt).next);
    }
    Cell *newCell = new Cell;
    newCell->val = elem;
    if (*insertAt != nullptr) {
        newCell->next = *insertAt;
    }
    (*insertAt) = newCell;
}

template<typename T>
void OrderedList<T>::addUniquely(const T &elem) {

    Cell **insertAt = &head;
    while (*insertAt != nullptr && (**insertAt).val < elem) {
        insertAt = &((**insertAt).next);
    }
    if (*insertAt == nullptr || (**insertAt).val != elem) {
        Cell *newCell = new Cell;
        newCell->val = elem;
        if (*insertAt != nullptr) {
            newCell->next = *insertAt;
        }
        (*insertAt) = newCell;
    }
}

template<typename T>
bool OrderedList<T>::removeOne(const T &elem) {
    Cell **lookAt = &head;
    while (*lookAt != nullptr && (**lookAt).val < elem) {
        lookAt = &((**lookAt).next);
    }
    if (*lookAt != nullptr && (**lookAt).val == elem) {
        Cell *tmp = (*lookAt)->next;
        delete *lookAt;
        *lookAt = tmp;
        return true;
    }

    return false;
}

template<typename T>
bool OrderedList<T>::removeAll(const T &elem) {
    bool result = false;
    Cell **lookAt = &head;
    while (*lookAt != nullptr && (**lookAt).val < elem) {
        lookAt = &((**lookAt).next);
    }
    while (*lookAt != nullptr && (**lookAt).val == elem) {
        Cell *tmp = (*lookAt)->next;
        delete *lookAt;
        *lookAt = tmp;
        result = true;
    }

    return result;
}

template<typename T>
void OrderedList<T>::display() const {
    Cell *current = head;
    while (current != nullptr) {
        std::cout << current->val << " ";
        current = current->next;
    }
    std::cout << "\n";
}

template<typename T>
bool OrderedList<T>::contains(const T &elem) const {
    Cell *const *lookAt = &head;
    while (*lookAt != nullptr && (**lookAt).val < elem) {
        lookAt = &((**lookAt).next);
    }
    return (*lookAt != nullptr && (**lookAt).val == elem);
}

template<typename T>
typename OrderedList<T>::iterator OrderedList<T>::begin() {
    iterator result;
    result.source = &head;
    result.current = head;
    return result;
}

template<typename T>
typename OrderedList<T>::iterator OrderedList<T>::end() const {
    return iterator();
}

template<typename T>
bool OrderedList<T>::isEmpty() const {
    return head == nullptr;
}

template<typename T>
T &OrderedList<T>::getFirst() const {
    return head->val;
}

template<typename T>
bool OrderedList<T>::popFirst() {
    if (isEmpty()) {
        return false;
    }
    Cell *tmp = head->next;
    delete head;
    head = tmp;
    return true;
}

template<typename T>
OrderedList<T>::OrderedList(const OrderedList &other) {
    Cell **me = &head;
    Cell *them = other.head;
    while (them != nullptr) {
        *me = new Cell;
        (*me)->val = them->val;
        me = &((*me)->next);
        them = them->next;
    }
}

template<typename T>
OrderedList<T>::OrderedList(OrderedList &&other) noexcept {
    head = other.head;
    other.head = nullptr;
}

template<typename T>
OrderedList<T> &OrderedList<T>::operator=(const OrderedList &other) {
    if (&other != this) {
        Cell *shuttle = head;
        Cell *tmp;
        while (shuttle != nullptr) {
            tmp = shuttle->next;
            delete shuttle;
            shuttle = tmp;
        }

        Cell **me = &head;
        Cell *them = other.head;
        while (them != nullptr) {
            *me = new Cell;
            (*me)->val = them->val;
            me = &((*me)->next);
            them = them->next;
        }
    }
    return *this;
}

template<typename T>
OrderedList<T> &OrderedList<T>::operator=(OrderedList &&other) noexcept {
    if (&other != this) {
        Cell *shuttle = head;
        Cell *tmp;
        while (shuttle != nullptr) {
            tmp = shuttle->next;
            delete shuttle;
            shuttle = tmp;
        }

        head = other.head;
        other.head = nullptr;
    }
    return *this;
}

template<typename T>
void OrderedList<T>::addAll(const OrderedList &other) {
    iterator itThis = begin();
    const_iterator itOther = other.cbegin();
    while (itOther != cend()) {
        while (itThis != end() && *itThis < *itOther) {
            ++itThis;
        }
        itThis.insertBefore(*itOther);
        ++itOther;
    }
}

template<typename T>
typename OrderedList<T>::const_iterator OrderedList<T>::cbegin() const {
    const_iterator result;
    result.current = head;
    return result;
}

template<typename T>
typename OrderedList<T>::const_iterator OrderedList<T>::cend() const {
    return const_iterator();
}


#endif //LISTSGRAPH_ORDEREDLIST_HPP
