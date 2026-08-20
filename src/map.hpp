/**
* implement a container like std::map
*/
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<class Key, class T, class Compare = std::less<Key>>
class map {
    struct Node; // Forward declaration

public:
   /**
  * the internal type of data.
  * it should have a default constructor, a copy constructor.
  * You can use sjtu::map as value_type by typedef.
    */
   typedef pair<const Key, T> value_type;
   /**
  * see BidirectionalIterator at CppReference for help.
  *
  * if there is anything wrong throw invalid_iterator.
  *     like it = map.begin(); --it;
  *       or it = map.end(); ++end();
    */
   class const_iterator {
       friend class map;
       friend class iterator;
   protected:
       const map* container;
       Node* node;
   public:
       const_iterator() : container(nullptr), node(nullptr) {}
       const_iterator(const const_iterator &other) : container(other.container), node(other.node) {}
       const_iterator(const iterator &other);

       /**
    * TODO iter++
        */
       const_iterator operator++(int) {
           const_iterator tmp = *this;
           ++(*this);
           return tmp;
       }

       /**
    * TODO ++iter
        */
       const_iterator &operator++() {
           if (node == nullptr || node == container->nil) {
               throw invalid_iterator();
           }
           node = container->successor(node);
           return *this;
       }

       /**
    * TODO iter--
        */
       const_iterator operator--(int) {
           const_iterator tmp = *this;
           --(*this);
           return tmp;
       }

       /**
    * TODO --iter
        */
       const_iterator &operator--() {
           if (node == nullptr) {
               throw invalid_iterator();
           }
           if (node == container->nil) {
               node = container->maximum(container->root);
               if (node == container->nil) {
                   throw invalid_iterator();
               }
               return *this;
           }
           node = container->predecessor(node);
           if (node == nullptr) {
               throw invalid_iterator();
           }
           return *this;
       }

       /**
    * a operator to check whether two iterators are same (pointing to the same memory).
        */
       const value_type &operator*() const {
           if (node == nullptr || node == container->nil) {
               throw invalid_iterator();
           }
           return *(node->data);
       }

       bool operator==(const const_iterator &rhs) const {
           return node == rhs.node;
       }

       bool operator==(const iterator &rhs) const;

       /**
    * some other operator for iterator.
        */
       bool operator!=(const const_iterator &rhs) const {
           return node != rhs.node;
       }

       bool operator!=(const iterator &rhs) const;

       /**
    * for the support of it->first.
    * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
        */
       const value_type *operator->() const noexcept {
           return node->data;
       }
   };

   class iterator {
       friend class map;
       friend class const_iterator;
   private:
       /**
    * TODO add data members
    *   just add whatever you want.
        */
       map* container;
       Node* node;
   public:
       iterator() : container(nullptr), node(nullptr) {}

       iterator(const iterator &other) : container(other.container), node(other.node) {}

       /**
    * TODO iter++
        */
       iterator operator++(int) {
           iterator tmp = *this;
           ++(*this);
           return tmp;
       }

       /**
    * TODO ++iter
        */
       iterator &operator++() {
           if (node == nullptr || node == container->nil) {
               throw invalid_iterator();
           }
           node = container->successor(node);
           return *this;
       }

       /**
    * TODO iter--
        */
       iterator operator--(int) {
           iterator tmp = *this;
           --(*this);
           return tmp;
       }

       /**
    * TODO --iter
        */
       iterator &operator--() {
           if (node == nullptr) {
               throw invalid_iterator();
           }
           if (node == container->nil) {
               node = container->maximum(container->root);
               if (node == container->nil) {
                   throw invalid_iterator();
               }
               return *this;
           }
           node = container->predecessor(node);
           if (node == nullptr) {
               throw invalid_iterator();
           }
           return *this;
       }

       /**
    * a operator to check whether two iterators are same (pointing to the same memory).
        */
       value_type &operator*() const {
           if (node == nullptr || node == container->nil) {
               throw invalid_iterator();
           }
           return *(node->data);
       }

       bool operator==(const iterator &rhs) const {
           return node == rhs.node;
       }

       bool operator==(const const_iterator &rhs) const {
           return node == rhs.node;
       }

       /**
    * some other operator for iterator.
        */
       bool operator!=(const iterator &rhs) const {
           return node != rhs.node;
       }

       bool operator!=(const const_iterator &rhs) const {
           return node != rhs.node;
       }

       /**
    * for the support of it->first.
    * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
        */
       value_type *operator->() const
           noexcept {
           return node->data;
       }
   };

private:
   struct Node {
       value_type *data; // nullptr for nil
       Node *left;
       Node *right;
       Node *parent;
       bool is_red;

       // Constructor for nil node
       Node() : data(nullptr), left(this), right(this), parent(nullptr), is_red(false) {}

       // Constructor for regular node
       Node(const value_type &d, Node *nil)
           : data(new value_type(d)), left(nil), right(nil), parent(nullptr), is_red(true) {}

       ~Node() {
           delete data;
       }
   };

   Node *root;
   Node *nil;
   size_t sz;
   Compare cmp;

   // Helper methods
   Node *minimum(Node *x) const {
       if (x == nil) return nil;
       while (x->left != nil) {
           x = x->left;
       }
       return x;
   }

   Node *maximum(Node *x) const {
       if (x == nil) return nil;
       while (x->right != nil) {
           x = x->right;
       }
       return x;
   }

   Node *successor(Node *x) const {
       if (x == nil) return nil;
       if (x->right != nil) {
           return minimum(x->right);
       }
       Node *y = x->parent;
       while (y != nullptr && x == y->right) {
           x = y;
           y = y->parent;
       }
       return y ? y : nil;
   }

   Node *predecessor(Node *x) const {
       if (x == nil) return nil;
       if (x->left != nil) {
           return maximum(x->left);
       }
       Node *y = x->parent;
       while (y != nullptr && x == y->left) {
           x = y;
           y = y->parent;
       }
       return y ? y : nil;
   }

   void left_rotate(Node *x) {
       Node *y = x->right;
       x->right = y->left;
       if (y->left != nil) {
           y->left->parent = x;
       }
       y->parent = x->parent;
       if (x->parent == nullptr) {
           root = y;
       } else if (x == x->parent->left) {
           x->parent->left = y;
       } else {
           x->parent->right = y;
       }
       y->left = x;
       x->parent = y;
   }

   void right_rotate(Node *y) {
       Node *x = y->left;
       y->left = x->right;
       if (x->right != nil) {
           x->right->parent = y;
       }
       x->parent = y->parent;
       if (y->parent == nullptr) {
           root = x;
       } else if (y == y->parent->right) {
           y->parent->right = x;
       } else {
           y->parent->left = x;
       }
       x->right = y;
       y->parent = x;
   }

   void insert_fixup(Node *z) {
       while (z->parent && z->parent->is_red) {
           if (z->parent == z->parent->parent->left) {
               Node *y = z->parent->parent->right;
               if (y && y->is_red) {
                   z->parent->is_red = false;
                   y->is_red = false;
                   z->parent->parent->is_red = true;
                   z = z->parent->parent;
               } else {
                   if (z == z->parent->right) {
                       z = z->parent;
                       left_rotate(z);
                   }
                   z->parent->is_red = false;
                   z->parent->parent->is_red = true;
                   right_rotate(z->parent->parent);
               }
           } else {
               Node *y = z->parent->parent->left;
               if (y && y->is_red) {
                   z->parent->is_red = false;
                   y->is_red = false;
                   z->parent->parent->is_red = true;
                   z = z->parent->parent;
               } else {
                   if (z == z->parent->left) {
                       z = z->parent;
                       right_rotate(z);
                   }
                   z->parent->is_red = false;
                   z->parent->parent->is_red = true;
                   left_rotate(z->parent->parent);
               }
           }
       }
       root->is_red = false;
   }

   void transplant(Node *u, Node *v) {
       if (u->parent == nullptr) {
           root = v;
       } else if (u == u->parent->left) {
           u->parent->left = v;
       } else {
           u->parent->right = v;
       }
       v->parent = u->parent;
   }

   void erase_fixup(Node *x, Node *parent) {
       while (x != root && (!x || !x->is_red)) {
           if (x == parent->left) {
               Node *w = parent->right;
               if (w && w->is_red) {
                   w->is_red = false;
                   parent->is_red = true;
                   left_rotate(parent);
                   w = parent->right;
               }
               if ((!w->left || !w->left->is_red) && (!w->right || !w->right->is_red)) {
                   w->is_red = true;
                   x = parent;
                   parent = x->parent;
               } else {
                   if (!w->right || !w->right->is_red) {
                       if (w->left) w->left->is_red = false;
                       w->is_red = true;
                       right_rotate(w);
                       w = parent->right;
                   }
                   w->is_red = parent->is_red;
                   parent->is_red = false;
                   if (w->right) w->right->is_red = false;
                   left_rotate(parent);
                   x = root;
               }
           } else {
               Node *w = parent->left;
               if (w && w->is_red) {
                   w->is_red = false;
                   parent->is_red = true;
                   right_rotate(parent);
                   w = parent->left;
               }
               if ((!w->right || !w->right->is_red) && (!w->left || !w->left->is_red)) {
                   w->is_red = true;
                   x = parent;
                   parent = x->parent;
               } else {
                   if (!w->left || !w->left->is_red) {
                       if (w->right) w->right->is_red = false;
                       w->is_red = true;
                       left_rotate(w);
                       w = parent->left;
                   }
                   w->is_red = parent->is_red;
                   parent->is_red = false;
                   if (w->left) w->left->is_red = false;
                   right_rotate(parent);
                   x = root;
               }
           }
       }
       if (x) x->is_red = false;
   }

   void destroy_tree(Node *x) {
       if (x == nil || x == nullptr) return;
       destroy_tree(x->left);
       destroy_tree(x->right);
       delete x;
   }

   Node *copy_tree(Node *x, Node *parent, Node *src_nil) {
       if (x == src_nil) return nil;
       Node *new_node = new Node(*(x->data), nil);
       new_node->is_red = x->is_red;
       new_node->parent = parent;
       new_node->left = copy_tree(x->left, new_node, src_nil);
       new_node->right = copy_tree(x->right, new_node, src_nil);
       return new_node;
   }

   Node *find_node(const Key &key) const {
       Node *x = root;
       while (x != nil) {
           if (cmp(key, x->data->first)) {
               x = x->left;
           } else if (cmp(x->data->first, key)) {
               x = x->right;
           } else {
               return x;
           }
       }
       return nil;
   }

public:
   /**
  * TODO two constructors
    */
   map() : sz(0) {
       nil = new Node();
       root = nil;
   }

   map(const map &other) : sz(0) {
       nil = new Node();
       root = nil;
       if (other.root != other.nil) {
           root = copy_tree(other.root, nullptr, other.nil);
           sz = other.sz;
       }
   }

   /**
  * TODO assignment operator
    */
   map &operator=(const map &other) {
       if (this == &other) return *this;
       clear();
       if (other.root != other.nil) {
           root = copy_tree(other.root, nullptr, other.nil);
           sz = other.sz;
       }
       return *this;
   }

   /**
  * TODO Destructors
    */
   ~map() {
       destroy_tree(root);
       delete nil;
   }

   /**
  * TODO
  * access specified element with bounds checking
  * Returns a reference to the mapped value of the element with key equivalent to key.
  * If no such element exists, an exception of type `index_out_of_bound'
    */
   T &at(const Key &key) {
       Node *node = find_node(key);
       if (node == nil) {
           throw index_out_of_bound();
       }
       return node->data->second;
   }

   const T &at(const Key &key) const {
       Node *node = find_node(key);
       if (node == nil) {
           throw index_out_of_bound();
       }
       return node->data->second;
   }

   /**
  * TODO
  * access specified element
  * Returns a reference to the value that is mapped to a key equivalent to key,
  *   performing an insertion if such key does not already exist.
    */
   T &operator[](const Key &key) {
       Node *node = find_node(key);
       if (node != nil) {
           return node->data->second;
       }
       auto result = insert(value_type(key, T()));
       return result.first.node->data->second;
   }

   /**
  * behave like at() throw index_out_of_bound if such key does not exist.
    */
   const T &operator[](const Key &key) const {
       return at(key);
   }

   /**
  * return a iterator to the beginning
    */
   iterator begin() {
       iterator it;
       it.container = this;
       it.node = (root == nil) ? nil : minimum(root);
       return it;
   }

   const_iterator cbegin() const {
       const_iterator it;
       it.container = this;
       it.node = (root == nil) ? nil : minimum(root);
       return it;
   }

   /**
  * return a iterator to the end
  * in fact, it returns past-the-end.
    */
   iterator end() {
       iterator it;
       it.container = this;
       it.node = nil;
       return it;
   }

   const_iterator cend() const {
       const_iterator it;
       it.container = this;
       it.node = nil;
       return it;
   }

   /**
  * checks whether the container is empty
  * return true if empty, otherwise false.
    */
   bool empty() const {
       return sz == 0;
   }

   /**
  * returns the number of elements.
    */
   size_t size() const {
       return sz;
   }

   /**
  * clears the contents
    */
   void clear() {
       destroy_tree(root);
       root = nil;
       sz = 0;
   }

   /**
  * insert an element.
  * return a pair, the first of the pair is
  *   the iterator to the new element (or the element that prevented the insertion),
  *   the second one is true if insert successfully, or false.
    */
   pair<iterator, bool> insert(const value_type &value) {
       Node *y = nullptr;
       Node *x = root;

       while (x != nil) {
           y = x;
           if (cmp(value.first, x->data->first)) {
               x = x->left;
           } else if (cmp(x->data->first, value.first)) {
               x = x->right;
           } else {
               iterator it;
               it.container = this;
               it.node = x;
               return pair<iterator, bool>(it, false);
           }
       }

       Node *z = new Node(value, nil);
       z->parent = y;

       if (y == nullptr) {
           root = z;
       } else if (cmp(value.first, y->data->first)) {
           y->left = z;
       } else {
           y->right = z;
       }

       sz++;
       insert_fixup(z);

       iterator it;
       it.container = this;
       it.node = z;
       return pair<iterator, bool>(it, true);
   }

   /**
  * erase the element at pos.
  *
  * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
    */
   void erase(iterator pos) {
       if (pos.container != this || pos.node == nil || pos.node == nullptr) {
           throw invalid_iterator();
       }

       Node *z = pos.node;
       Node *y = z;
       Node *x;
       Node *x_parent;
       bool y_original_color = y->is_red;

       if (z->left == nil) {
           x = z->right;
           x_parent = z->parent;
           transplant(z, z->right);
       } else if (z->right == nil) {
           x = z->left;
           x_parent = z->parent;
           transplant(z, z->left);
       } else {
           y = minimum(z->right);
           y_original_color = y->is_red;
           x = y->right;
           if (y->parent == z) {
               x_parent = y;
           } else {
               x_parent = y->parent;
               transplant(y, y->right);
               y->right = z->right;
               y->right->parent = y;
           }
           transplant(z, y);
           y->left = z->left;
           y->left->parent = y;
           y->is_red = z->is_red;
       }

       delete z;
       sz--;

       if (!y_original_color) {
           erase_fixup(x, x_parent);
       }
   }

   /**
  * Returns the number of elements with key
  *   that compares equivalent to the specified argument,
  *   which is either 1 or 0
  *     since this container does not allow duplicates.
  * The default method of check the equivalence is !(a < b || b > a)
    */
   size_t count(const Key &key) const {
       return find_node(key) != nil ? 1 : 0;
   }

   /**
  * Finds an element with key equivalent to key.
  * key value of the element to search for.
  * Iterator to an element with key equivalent to key.
  *   If no such element is found, past-the-end (see end()) iterator is returned.
    */
   iterator find(const Key &key) {
       Node *node = find_node(key);
       iterator it;
       it.container = this;
       it.node = node;
       return it;
   }

   const_iterator find(const Key &key) const {
       Node *node = find_node(key);
       const_iterator it;
       it.container = this;
       it.node = node;
       return it;
   }
};

// Define const_iterator constructor from iterator
template<class Key, class T, class Compare>
map<Key, T, Compare>::const_iterator::const_iterator(const typename map<Key, T, Compare>::iterator &other)
   : container(other.container), node(other.node) {}

template<class Key, class T, class Compare>
bool map<Key, T, Compare>::const_iterator::operator==(const typename map<Key, T, Compare>::iterator &rhs) const {
   return node == rhs.node;
}

template<class Key, class T, class Compare>
bool map<Key, T, Compare>::const_iterator::operator!=(const typename map<Key, T, Compare>::iterator &rhs) const {
   return node != rhs.node;
}

}

#endif
