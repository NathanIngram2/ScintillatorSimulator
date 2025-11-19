#include <array>
#include <iostream>
#include "math.h"
using namespace std;

template <size_t K>
class KDTree
{
private:
    struct Node
    {
        array<float, K> point; 
        Node* left;          
        Node* right;            

        // construct a node
        Node(const array<float, K>& pt) : point(pt), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* insertRecursive(Node* node, const array<float, K>& point, int depth)
    {
        if (node == nullptr) return new Node(point);

        // get the current dimension
        int cd = depth % K;

        // determine if this node should go left or right child
        if (point[cd] < node->point[cd])
            node->left = insertRecursive(node->left, point, depth + 1);
        else
            node->right = insertRecursive(node->right, point, depth + 1);

        return node;
    }

    bool searchRecursive(Node* node, const array<float, K>& point, int depth) const
    {
        if (node == nullptr) return false;
        if (node->point == point) return true;

        int cd = depth % K;
        if (point[cd] < node->point[cd])
            return searchRecursive(node->left, point, depth + 1);
        else
            return searchRecursive(node->right, point, depth + 1);
    }

    // TODO: compare distance of points - is this doing a cartesian dist correctly? Been a minute since I looked at this
    float squaredDistance(const array<float, K>& a, const array<float, K>& b) const
    {
        float dist = 0.0f;
        for (size_t i = 0; i < K; i++)
        {
            dist += (a[i] - b[i]) * (a[i] - b[i]);
        }
        return dist;
    }

    array<float, K> searchRecursiveClosest(Node* node, const array<float, K>& target, int depth, array<float, K> bestPoint) const
    {
        if (node == nullptr)
        {
            return bestPoint;
        }

        // update bestPoint if the current node is closer
        if (squaredDistance(node->point, target) < squaredDistance(bestPoint, target))
        {
            bestPoint = node->point;
        }

        int cd = depth % K;

        // deteermine which subtree to search first and query it
        Node* nextNode = (target[cd] < node->point[cd]) ? node->left : node->right;
        Node* otherNode = (target[cd] < node->point[cd]) ? node->right : node->left;
        bestPoint = searchRecursiveClosest(nextNode, target, depth + 1, bestPoint);

        // check the other subtree for a closer point
        float splitDistance = fabs(target[cd] - node->point[cd]);
        if (splitDistance * splitDistance < squaredDistance(bestPoint, target))
        {
            bestPoint = searchRecursiveClosest(otherNode, target, depth + 1, bestPoint);
        }

        return bestPoint;
    }

    // print the tree recursively - useful for debug
    void printRecursive(Node* node, int depth, char branch = ' ') const
    {
        if (node == nullptr) return;

        for (int i = 0; i < depth; i++) cout << "  ";
        if (branch != ' ') cout << branch << ": ";
        cout << "(";
        for (size_t i = 0; i < K; i++) {
            cout << node->point[i];
            if (i < K - 1) cout << ", ";
        }
        cout << ")" << endl;

        printRecursive(node->left, depth + 1, 'L');
        printRecursive(node->right, depth + 1, 'R');
    }

public:

    KDTree() : root(nullptr) {}

    void insert(const array<float, K>& point) {
        root = insertRecursive(root, point, 0);
    }

    array<float, 2> search(const array<float, K>& point) const {
        return searchRecursiveClosest(root, point, 0, root->point);
    }

    void print() const {
        printRecursive(root, 0);
    }
};