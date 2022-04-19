#include <mc_node.h>


MCNode::MCNode(MCNode * parent) {
    this->parent = parent;
    this->plays = 0;
    this->wins = 0;
    is_terminal = false;
}


MCNode::~MCNode(){
    // Nothing to destroy
}