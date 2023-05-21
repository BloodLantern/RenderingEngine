#pragma once

#include "physics/transform.hpp"

template<class T>
concept Node = std::is_base_of<Transform, T>::value;

template<Node T>
class Graph
{
public:
    T* root;

    Graph() = default;
    Graph(T* const root)
        : root(root) {}
    ~Graph() { delete root; }

    void Update() { if (root) root->Update(); }
    void Draw() { if (root) root->DrawObject(); }
};
