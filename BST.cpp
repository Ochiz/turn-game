#include <iostream>
#include <vector>
#include <queue>

// Definition of a TreeNode in a binary tree
template <typename T>
class TreeNode
{
public:
    T data;
    TreeNode<T>* left;
    TreeNode<T>* right;

    TreeNode(T val) : data(val), left(nullptr), right(nullptr)
    {
    }
};

// Definition of a BinarySearchTree class
template <typename T>
class BinarySearchTree
{
private:
    TreeNode<T>* root;

    // Private function to recursively insert a value into the tree
    TreeNode<T>* insertRecursive(TreeNode<T>* root, T val)
    {
        if (root == nullptr)
        {
            return new TreeNode<T>(val);
        }

        if (val < root->data)
        {
            root->left = insertRecursive(root->left, val);
        }
        else if (val > root->data)
        {
            root->right = insertRecursive(root->right, val);
        }

        return root;
    }

    // Private function to find the node with the minimum value in a subtree
    TreeNode<T>* findMinNode(TreeNode<T>* node)
    {
        TreeNode<T>* current = node;
        while (current && current->left != nullptr)
        {
            current = current->left;
        }
        return current;
    }

    // Private function to recursively delete a node with a given value from the tree
    TreeNode<T>* deleteRecursive(TreeNode<T>* root, T val)
    {
        if (root == nullptr)
        {
            return root;
        }
        if (val < root->data)
        {
            root->left = deleteRecursive(root->left, val);
        }
        else if (val > root->data)
        {
            root->right = deleteRecursive(root->right, val);
        }
        else
        {
            if (root->left == nullptr)
            {
                TreeNode<T>* temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == nullptr)
            {
                TreeNode<T>* temp = root->left;
                delete root;
                return temp;
            }

            TreeNode<T>* temp = findMinNode(root->right);
            root->data = temp->data;
            root->right = deleteRecursive(root->right, temp->data);
        }
        return root;
    }

    // Private function to perform an inorder traversal of the tree (used for printing)
    void inorderTraversalRecursive(TreeNode<T>* root, std::vector<T>& result)
    {
        if (root == nullptr)
        {
            return;
        }
        inorderTraversalRecursive(root->left, result);
        result.push_back(root->data);
        inorderTraversalRecursive(root->right, result);
    }

public:
    BinarySearchTree() : root(nullptr)
    {
    }

    // Public function to insert a value into the tree
    void insert(T val)
    {
        root = insertRecursive(root, val);
    }

    // Public function to remove a value from the tree
    bool remove(T val)
    {
        TreeNode<T>* prev = nullptr;
        TreeNode<T>* curr = root;

        while (curr)
        {
            if (val == curr->data)
            {
                root = deleteRecursive(root, val);
                return true;
            }
            prev = curr;
            if (val < curr->data)
            {
                curr = curr->left;
            }
            else
            {
                curr = curr->right;
            }
        }
        return false;
    }

    // Public function to find the maximum value in the tree
    T findMax()
    {
        TreeNode<T>* current = root;
        while (current && current->right != nullptr)
            current = current->right;
        return current->data;
    }

    // Public function to perform an inorder traversal of the tree
    std::vector<T> traverseInorder()
    {
        std::vector<T> result;
        inorderTraversalRecursive(root, result);
        return result;
    }

    void traversePreorderRecursive(TreeNode<T>* root, std::vector<T>& result) 
    {
        if (root == nullptr)
            return;
        result.push_back(root->data);
        traversePreorderRecursive(root->left, result);
        traversePreorderRecursive(root->right, result);
    }

    std::vector<T> traversePreorder() 
    {
        std::vector<T> result;
        traversePreorderRecursive(root, result);
        return result;
    }

    void traversePostorderRecursive(TreeNode<T>* root, std::vector<T>& result) 
    {
        if (root == nullptr)
            return;
        traversePostorderRecursive(root->left, result);
        traversePostorderRecursive(root->right, result);
        result.push_back(root->data);
    }

    std::vector<T> traversePostorder() 
    {
        std::vector<T> result;
        traversePostorderRecursive(root, result);
        return result;
    }
};

// Main program
int main()
{
    BinarySearchTree<int> bst;

    int values[] = { 5, 3, 7, 2, 4, 6, 8 };
    int n = sizeof(values) / sizeof(values[0]);

    // Insert values into the binary search tree
    for (int i = 0; i < n; ++i)
    {
        bst.insert(values[i]);
    }

    // Print inorder traversal
    std::cout << "Inorder traversal: ";
    std::vector<int> inorder = bst.traverseInorder();
    for (int val : inorder)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "Preorder traversal: ";
    std::vector<int> preorder = bst.traversePreorder();
    for (int val : preorder) 
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "Postorder traversal: ";
    std::vector<int> postorder = bst.traversePostorder();
    for (int val : postorder) 
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "Max value in BST: " << bst.findMax() << std::endl;

    // Remove nodes from the tree
    std::cout << "Removing 7: " << (bst.remove(7) ? "Success" : "Failed") << std::endl;
    std::cout << "Removing 2: " << (bst.remove(2) ? "Success" : "Failed") << std::endl;
    std::cout << "Removing 9: " << (bst.remove(9) ? "Success" : "Failed") << std::endl;

    // Print inorder traversal after removing nodes
    std::cout << "Inorder traversal after removing 7 and 2: ";
    inorder = bst.traverseInorder();
    for (int val : inorder)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
