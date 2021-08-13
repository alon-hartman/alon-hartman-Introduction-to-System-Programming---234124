#ifndef SORTED_LIST_H
#define SORTED_LIST_H
#include <iostream>
#include <cassert>
#include <stdexcept>

namespace mtm {

    /** NODE IMPLEMENTATION START */
    template <class T>
    class Node
    {
    public:
        Node* next;
        Node* prev;
        const T* data;
        /**
        * Node - constracts a new node
        */
        Node() : next(nullptr), prev(nullptr), data(nullptr) {   };
        /**
        * Node - constracts a copy of a node
        *
        * @param src - node to copy
        */
        Node(const Node& src) {
            const Node<T> *iterator_src = &src;
            this->data = nullptr;
            this->prev = nullptr;
            this->next = nullptr;
            Node<T>* tmp_itr = this;
            while(iterator_src) {
                tmp_itr->push_after(*iterator_src->data);
                if(tmp_itr->next) {
                    tmp_itr = tmp_itr->next;
                }
                iterator_src = iterator_src->next;
            }
        };
        /**
        * Node - constructs a new Node and initiallizes its data with (copy of) element
        *
        * @param element - copied element to the new node data
        */
        Node(const T& element)  : next(nullptr), prev(nullptr), data(new T(element)) {   };
        Node& operator=(const Node& src) {
            delete this;
            next = src.next;
            prev = src.prev;
            T* copyobj = new T(*src.data);
            data = copyobj;
            return *this;
        };
        /**
        * ~Node - destractor of node
        */
        ~Node() {
            if(this->next != nullptr) {
                delete this->next;
            }
            delete data;
        };
        /**
        * push_before - pushes element to a new Node, before current Node
        *
        * @param element- copied element to new node data
        */
        void push_before(const T& element)
        {
            T* copyobj = new T(element);
            if(data == nullptr) {
                data = copyobj;
                return;
            }
            Node<T>* new_node = new Node<T>;
            new_node->data = copyobj;
            new_node->next = this;
            new_node->prev = this->prev;
            if(this->prev) {
                this->prev->next = new_node;
            }
            this->prev = new_node;
        };
        /**
        * push_after - pushes element to a new Node,after current Node
        *
        * @param element- copied element to new node data
        */
        void push_after(const T& element)
        {
            T* copyobj = new T(element);
            if(data == nullptr) {
                data = copyobj;
                return;
            }
            Node<T>* new_node = new Node<T>;
            new_node->data = copyobj;
            new_node->next = this->next;
            new_node->prev = this;
            if(this->next) {
                this->next->prev = new_node;
            }
            this->next = new_node;
        };
    };

    /* NODE IMPLEMENTATION END */
    /*******************************************************************************************/
    /*******************************************************************************************/

    /** LIST IMPLEMENTATION START */
    template <class T>
    class SortedList
    {
    private:
        Node<T>* head;
        friend class Node<T>;
        
        /**
        * contains - checks if sortedlist contains given element
        *
        * @param element - element to search in sortedlist
        */
        bool contains(const T &element) const;

    public:
        /**
        * SortedList - constructs a new empty list
        */
        SortedList() : head(nullptr) {};
        /**
        * SortedList - constructs a copy of given sorted list
        *
        * @param sortedlist - a sorted list requested to copy
        */
        SortedList(const SortedList&);
        /**
        * ~SortedList - destructs a sorted list
        */
        ~SortedList() { delete head; };

        class const_iterator;

        /**
        * insert - inserts element while keeping the list sorted.
        * 
        * @param element - element requested to insert
        */
        void insert(const T &element);

        /**
        * remove - removes the element to which the iterator points to
        *
        * @param it - iterator which points to removed element
        */
        void remove(const_iterator it);
        
        /**
        * length - returns the length of the list (number of elements).
        *
        * @return - length of the list
        *
        * @throws out_of_range - for invalid iterator
        */
        int length() const;
        
        /**
        * Operator= -  inserts given sorted list to a sorted list
        * 
        * @param src_list - given sort list requested to insert 
        * 
        * @return - reference to copied list
        */
        SortedList<T>& operator=(const SortedList<T>& src_list);

        /**
        * filter - returns a new SortedList with elements for which pred(element) returned true
        * 
        * @param pred - boolean function over T kind elements
        *
        * @return SortedList - new sorted list with elements which pred returned true for
        */
        template <typename Predicate>
        SortedList filter(Predicate pred) const;

        /**
        * apply -applies func to all elements of the list and returns a new SortedList with the results.
        *
        * @param func - function over T elements which returns T elements
        *
        * @return SortedList - a new sorted list with affected elements
        */
       template <typename Apply>
        SortedList apply(Apply func) const;
        /**
        * begin - returns a const_iterator to the first element in the list.
        * 
        * @return const_iterator - to the first element in list
        */
        const_iterator begin() const;
        /**
        * end - returns a const_iterator to the end of the list (after the last element).
        * 
        * @return const_iterator - iterator to one after the end of the list
        */
        const_iterator end() const;
    };

    template<class T>
    SortedList<T>::SortedList(const SortedList<T>& src_list) 
    {
        if(src_list.head == nullptr)
        {
            this->head = nullptr;
            return;
        }
        this->head = new Node<T>();
        Node<T>* tmp_itr = this->head;
        for(SortedList<T>::const_iterator it = src_list.begin(); it != src_list.end(); ++it)
        {
            const T& element = *(it);
            tmp_itr->push_after(element);
            if(tmp_itr->next != nullptr) {
                tmp_itr = tmp_itr->next;
            }
        }
    }

    template<class T>
    SortedList<T>& SortedList<T>::operator=(const SortedList<T>& src_list)
    {
        if(this->head == src_list.head) {
            return *this;  // protect against self assignment
        }
        Node<T> *new_head = nullptr;
        if(src_list.head != nullptr) {
            new_head = new Node<T>(*src_list.head);
        }
        delete this->head;
        this->head = new_head;
        return *this;
    }

    /* T must have operator == */
    template<class T>
    bool SortedList<T>::contains(const T& element) const
    {
        Node<T>* iterator = head;
        while(iterator && iterator->data)
        {
            if(*(iterator->data) == element)
            {
                return true;
            }
            iterator = iterator->next;
        }
        return false;
    }

    template<class T>
    int SortedList<T>::length() const
    {
        int size = 0;
        for(const_iterator it = this->begin(); it != this->end(); ++it) {
            size++;
        }
        return size;
    }

    template<class T>
    void SortedList<T>::remove(SortedList<T>::const_iterator it)
    {
        if(it.node_ptr == nullptr) {
            throw std::out_of_range("Invalid iterator");
        }
        if(head == it.node_ptr/* && (head->data) == (it)*/) {
            Node<T>* temp = head;
            head = head->next;
            temp->next = nullptr;
            delete temp;
            return;
        }
        Node<T>* iterator = this->head;
        while(iterator && iterator->next)
        {
            if(it.node_ptr == iterator->next/* && (iterator->next->data) == (it)*/) {
                iterator->next = it.node_ptr->next;
                it.node_ptr->next = nullptr;
                delete it.node_ptr;
                return;
            }
            iterator = iterator->next;
        }
    }

    template<class T>
    void SortedList<T>::insert(const T &element)
    {
        if(head == nullptr) {
            head = new Node<T>(element);
            return;
        }
        Node<T>* iterator = head;
        while(iterator) {
            if(iterator->data == nullptr || !(*(iterator->data) < element)) {
                iterator->push_before(element);
                if(iterator == head) {
                    head = iterator->prev;
                }
                return;
            }
            if(iterator->next == nullptr) {
                iterator->push_after(element);
                return;
            }
            iterator = iterator->next;
        }
    }

    template <class T>
    template <typename Predicate>
    SortedList<T> SortedList<T>::filter(Predicate pred) const
    {
        SortedList<T> new_list;
        for(typename SortedList<T>::const_iterator it = this->begin(); it != this->end(); ++it)
        {
            if(pred(*it))
            {
                new_list.insert(*it);
            }
        }
        return new_list;
    }

    template <class T>
    template <typename Apply>
    SortedList<T> SortedList<T>::apply(Apply func) const
    {
        SortedList<T> new_list;
        for(typename SortedList<T>::const_iterator it = this->begin(); it != this->end(); ++it)
        {
            new_list.insert(func(*it));
        }
        return new_list;
    }


    template<class T>
    typename SortedList<T>::const_iterator SortedList<T>::begin() const
    {
        return const_iterator(this->head);
    }

    template<class T>
    typename SortedList<T>::const_iterator SortedList<T>::end() const
    {
        return const_iterator(nullptr);
    }


    /** LIST IMPLEMENTATION END */
/*******************************************************************************************/
/*******************************************************************************************/


    /** CONST_ITERATOR IMPLEMENTATION START */
    template<class T>
    class SortedList<T>::const_iterator
    {
        private:
            Node<T>* node_ptr;
            friend class SortedList<T>;
            const_iterator() = delete;
            /**
            * const_iterator - constructs an iterator to a list
            *
            * @param p - node for iterator to point to
            */
            const_iterator( Node<T>* p) : node_ptr(p){};
        public:
            /**
            * const_iterator - constructs a copy of given iterator
            *
            * @param it - iterator required to copy
            */
            const_iterator(const const_iterator& it) = default;            
            /**
            * operator= - sets an iterator to given iterator
            *
            * @param it - given iterator to copy
            *
            * @return const_iterator - reference to newly set iterator
            */
            const_iterator& operator=(const const_iterator& it);
            /**
            * operator++ - farwards iterator to next Node in list
            *
            * @throws Out_Of_Range - if reached to the end of the list
            */
            const_iterator& operator++();
            /**
            * operator== - compares two iterators
            *
            * @param iterator - iterator to compare to
            *
            * @return returns true if the iterators point to the same place in the list
            */
            bool operator==(const const_iterator &iterator) const;
            /**
            * operator* - returns a reference to iterator node data 
            *
            * @throws Out_Of_range - if reached to the end of the list
            */
            const T& operator*(); 
            /**
            * operator!= - checks if given iterator points to different node
            * 
            * @param it - iterator to compare to
            *
            * @return true if the iterators point to different nodes
            */           
            bool operator!=(const const_iterator &it) const;
    };

    template<class T>
    typename SortedList<T>::const_iterator& SortedList<T>::const_iterator::operator=(const typename
                                                                                    SortedList<T>::const_iterator& it)
    {
        this->node_ptr = it.node_ptr;
        return *this;
    }
    
    template<class T>
    typename SortedList<T>::const_iterator& SortedList<T>::const_iterator::operator++()
    {
        if(node_ptr == nullptr) {
            throw std::out_of_range("reached end of list");
        }
        node_ptr = node_ptr->next;
        return *this;
    }

    template<class T>
    bool SortedList<T>::const_iterator::operator==(const SortedList<T>::const_iterator &iterator) const
    {
        return this->node_ptr == iterator.node_ptr;
    }
    
    template<class T>
    bool SortedList<T>::const_iterator::operator!=(const SortedList<T>::const_iterator &iterator) const
    {
        return !(*this == iterator);
    }

    template<class T>
    const T& SortedList<T>::const_iterator::operator*()
    {
        if(node_ptr == nullptr) {
            throw std::out_of_range("reached end of list");
        }
        return *(this->node_ptr->data);
    }
    
}
#endif