//
// Created by draghan on 17.08.18.
//

#include "ScriptedBT.hpp"

int main()
{
    const std::string script_path{"./script.chai"};
    ScriptedBT bt{script_path};
    bt.run_script();
    std::cout << bt.behavior_tree.get_node_count();
}