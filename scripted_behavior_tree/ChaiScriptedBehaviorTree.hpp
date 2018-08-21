//
// Created by draghan on 17.08.18.
//

#pragma once

#include <chaiscript.hpp>
#include "IScriptedBehaviorTree.hpp"

class ChaiScriptedBehaviorTree: public IScriptedBehaviorTree
{
public:
    explicit ChaiScriptedBehaviorTree(std::string script_path);
    bool load_tree() override;

    ~ChaiScriptedBehaviorTree() override = default;

protected:
    void register_bt_interface() override;

    chaiscript::ChaiScript script;

    void register_set_at_absolutely();
    void register_set_at_relatively();
};



