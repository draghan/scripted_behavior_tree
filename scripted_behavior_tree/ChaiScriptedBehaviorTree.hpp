//
// Created by draghan on 17.08.18.
//

#pragma once

#include <chaiscript.hpp>
#include "IScriptedBehaviorTree.hpp"

class ChaiScriptedBehaviorTree: public IScriptedBehaviorTree
{
public:
    enum class ScriptIdentifier
    {
        sequence,
        selector,
        condition,
        action,
        link,
        invert,
        loop,
        max_n_tries,
        this_tree,
        behavior_state,
        state_failure,
        state_success,
        state_running,
        set_at_relatively,
        set_at_absolutely,
        set_at_id,
        MAX
    };

    explicit ChaiScriptedBehaviorTree(std::string script_path, std::string tree_name = "");
    ~ChaiScriptedBehaviorTree() override;

    bool load_tree() override;
    std::string get_tree_id() const;

protected:
    using std_array_id_t = std::array<std::string, static_cast<size_t>(ScriptIdentifier::MAX)>;

    // Extending std::array with operator[] using explicit strongly typed enum
    // in order to prevent casting every time when indexing:
    class extended_array_id_t: public std_array_id_t
    {
    public:
        const std::string &operator[](ScriptIdentifier index) const
        {
            return std_array_id_t::operator[](static_cast<size_t>(index));
        }

        std::string &operator[](ScriptIdentifier index)
        {
            return std_array_id_t::operator[](static_cast<size_t>(index));
        }
    };

    void register_bt_interface() override;

    chaiscript::ChaiScript script;
    std::string tree_id;

    static std::vector<ChaiScriptedBehaviorTree *> known_scripted_trees;
    static const extended_array_id_t identifiers;

private:
    void register_all_known_scripted_trees();
    void register_set_at_absolutely();
    void register_set_at_relatively();
    void register_behavior_state();
    void register_boolean();
    void register_this_tree();
    void register_adding_nodes();
    void register_positioning();

    static std::string prepare_id(const std::string &name);
    static std::string prepare_id_from_path(const std::string &path);
};
