//
// Created by draghan on 17.08.18.
//

#pragma once

#include <BehaviorTree.hpp>
#include <chaiscript.hpp>

class ScriptedBT
{
public:

    explicit ScriptedBT(const std::string& path);

    BehaviorTree behavior_tree;

    void run_script();
private:
    std::string path;
    chaiscript::ChaiScript script;


    void export_bt_interface();
};



