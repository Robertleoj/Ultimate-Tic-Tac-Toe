#include <mc_tree.h>

MCTree::MCTree(){
    root = new MCNode(nullptr);
}

MCTree::~MCTree(){
    delete_tree(this->root);
}

void MCTree::delete_tree(MCNode * node){
    // base case
    if(node == nullptr){
        return;
    }

    // recursively delete children
    for(auto &c: node->children){
        delete_tree(c.second);
    }

    // delete the node
    delete node;
}


void MCTree::move(Move *m){

    auto child_map = &this->root->children;

    if(child_map->find(m) == child_map->end() ){
        // Delete root and all children
        delete_tree(this->root);

        // Create new node to be root
        this->root = new MCNode(nullptr);

    } else {
        // Get the new root
        MCNode *new_root = child_map->at(m);

        // Remove new root from root children
        child_map->erase(m);

        // Remove parent from new root
        new_root->parent = nullptr;

        // Delete rest of tree
        delete_tree(this->root);

        // replace root
        this->root = new_root;
    }
}
