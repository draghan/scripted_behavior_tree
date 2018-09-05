//
// Created by draghan on 17.08.18.
//

#pragma once

#include <chaiscript.hpp>
#include "IScriptedBehaviorTree.hpp"

class ChaiScriptedBehaviorTree: public IScriptedBehaviorTree
{
public:
    explicit ChaiScriptedBehaviorTree(std::string script_path, std::string tree_name = "");
    ~ChaiScriptedBehaviorTree() override;

    bool load_tree() override;
    std::string get_tree_id() const;

protected:
    struct TreeReflection
    {
        ChaiScriptedBehaviorTree *pointer;
        std::string identifier;
    };

    void register_bt_interface() override;

    chaiscript::ChaiScript script;
    std::string tree_id;

    static std::vector<TreeReflection> known_scripted_trees;

private:
    void register_all_known_scripted_trees();
    void register_set_at_absolutely();
    void register_set_at_relatively();

    static std::string prepare_id(const std::string &name);
    static std::string prepare_id_from_path(const std::string &path);
};



