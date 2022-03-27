#pragma once
#include <move.h>
#include <map>

class MCNode{
public:
    int plays;
    double wins;
    MCNode * parent;
    bool is_terminal;

    std::map<Move*, MCNode*> children;

    ~MCNode();

    MCNode(MCNode * parent=nullptr);
}; 