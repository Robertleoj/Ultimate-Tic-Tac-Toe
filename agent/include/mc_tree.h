#pragma once
#include <mc_node.h>

class MCTree {
public:
    MCNode * root;

    MCTree();
    ~MCTree();

    void move(Move *m);

private:
    void delete_tree(MCNode * node);
};
