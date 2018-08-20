#include <utility>

//
// Created by draghan on 17.08.18.
//

#pragma once


#include <string>
#include <BehaviorTree.hpp>

class IScriptedBehaviorTree: public BehaviorTree
{
public:
    explicit IScriptedBehaviorTree(std::string script_path);

    ~IScriptedBehaviorTree() override = default;
    virtual bool load_tree() = 0;

protected:
    std::string script_path;
    virtual void register_bt_interface() = 0;
};



