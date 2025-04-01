#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <vector>
using namespace std;

template <typename KeyType, typename ValueType>
class TreeMultimap
{
public:
    // O(V) for iterating through all V values associated with a given key
    class Iterator
    {
    public:
        // invalid iterator constructor
        Iterator()
        {
            it = nullptr; 
            currIndex = 1;
            maxIndex = 0;
        }

        // valid iterator constructor
        Iterator(vector<ValueType> &values) 
        {
            it = &values[0];
            currIndex = 0;
            maxIndex = values.size() - 1;
        }

        // get the value that the iterator points to
        ValueType& get_value() const
        {
            return *it;
        }
        
        // if iterator points to a value, it's true. otherwise, false
        bool is_valid() const
        {
            if (currIndex > maxIndex) // if it passes the vector's size
            {
                return false;
            }
            return true;
        }

        // moves iterator by one
        void advance()
        {
            it = it + 1;
            currIndex++;
        }

    private:
        ValueType* it; 
        int currIndex;
        size_t maxIndex;
    };

    TreeMultimap()
    {
        root = nullptr;
    }

    ~TreeMultimap()
    {
        node* curr = root;
        freeTree(curr); 
    }

    // inserts a node to the BST unless the key already exists
    void insert(const KeyType& key, const ValueType& value)
    {
        if (root == nullptr) // if tree is empty
        {
            root = new node(key, value);
            return;
        }
        
        node* curr = root;
        for (;;) // orders efficiently (left is less, right is greater)
        {
            // if key is already in tree, add value to key's values
            if (key == curr->key) 
            {
                curr->values.push_back(value); 
                return;
            }

            if (key < curr->key)
            {
                if (curr->left != nullptr)
                {
                    curr = curr->left; 
                }
                else
                {
                    curr->left = new node(key, value);
                    return;
                }
            }
            else if (key > curr->key)
            {
                if (curr->right != nullptr)
                {
                    curr = curr->right;
                }
                else
                {
                    curr->right = new node(key, value);
                    return;
                }
            }
        }
    }

    // O(log K) time, where K is the number of distinct keys in the tree
    // finds a specific key in the map. returns an iterator obj to its values
    Iterator find(const KeyType& key) const
    {
        // if tree is empty, return an invalid pointer
        if (root == nullptr)
        {
            return Iterator();
        }

        node* curr = root;
        for (;;) // finds efficiently (BST)
        {
            if (key == curr->key)
            {
                return Iterator(curr->values);
            }

            if (key < curr->key)
            {
                if (curr->left != nullptr)
                {
                    curr = curr->left;
                }
                else
                {
                    return Iterator();
                }
            }
            else if (key > curr->key)
            {
                if (curr->right != nullptr)
                {
                    curr = curr->right;
                }
                else
                {
                    return Iterator();
                }
            }
        }
    }

private:
    struct node
    {
        // node constructor
        node(KeyType keyName, ValueType value)
        {
            key = keyName;
            values.push_back(value);
            left = nullptr;
            right = nullptr;
        }
        KeyType key;
        vector<ValueType> values;
        node* left;
        node* right;
    };

    node* root;

    // helper function for destructor
    void freeTree(node* curr)
    {
        if (curr == nullptr)
        {
            return; // no nodes to delete
        }

        freeTree(curr->left);
        freeTree(curr->right);
        delete curr;
    }
};

#endif // TREEMULTIMAP_INCLUDED
