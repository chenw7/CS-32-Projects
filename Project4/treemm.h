#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <iostream>

template <typename KeyType, typename ValueType>
class TreeMultimap
{
  public:
    class Iterator
    {
      public:
        Iterator()
        {
            m_list = nullptr;
            currentIndex = 0;
        }
        
        //OTHER CONSTRUCTORS FOR ITERATOR
        Iterator(std::vector<ValueType>* otherList)
        {
            m_list = otherList;
            currentIndex = 0;
        }
        
        ValueType& get_value() const
        {
            return m_list->at(currentIndex);
        }

        bool is_valid() const
        {
            if(m_list == nullptr || currentIndex >= m_list->size())
                return false;
            return true;
        }

        void advance()
        {
            currentIndex++;
        }

      private:
        std::vector<ValueType>* m_list;
        int currentIndex;
    };

    TreeMultimap()
    {
        root = nullptr;
    }

    ~TreeMultimap()
    {
        deleteEverything(root);
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        if(root == nullptr)
            root = new Node(key, value);
        
        else{
            bool inserted = false;
            Node* current = root;
            
            while(!inserted){
                if(key == current->m_key){
                    current->m_values.push_back(value);
                    inserted = true;
                }
                else if(key < current->m_key){
                    if(current->left == nullptr){
                        current->left = new Node(key, value);
                        inserted = true;
                    }
                    else{
                        current = current->left;
                    }
                }
                else{
                    if(current->right == nullptr){
                        current->right = new Node(key, value);
                        inserted = true;
                    }
                    else{
                        current = current->right;
                    }
                }
            }
        }
    }

    Iterator find(const KeyType& key) const
    {
        Node* current = root;
        while(current != nullptr){
            if(key == current->m_key){
                return Iterator(&(current->m_values));
            }
            else if(key < current->m_key){
                current = current->left;
            }
            else{
                current = current->right;
            }
        }
        
        return Iterator();
    }

  private:
    struct Node{
        KeyType m_key;
        std::vector<ValueType> m_values;
        Node* left;
        Node* right;
            
        Node(KeyType key, ValueType value){
            m_key = key;
            m_values.push_back(value);
            left = nullptr;
            right = nullptr;
        }
    };
    
    void deleteEverything(Node* cur){
        if(cur == nullptr) return;
        deleteEverything(cur->left);
        deleteEverything(cur->right);
        //WHAT ABOUT DYNAMICALLY ALLOCATED OBJECTS INSIDE THE VECTORS
        delete cur;
    }
    
    Node* root;
    
    //MAYBE IMPLEMENT RECURSIVE HELPER FUNCTION TO HELP DELETE EVERYTHING
};

#endif // TREEMULTIMAP_INCLUDED
