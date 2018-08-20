#include <utility>

//
// Created by draghan on 17.08.18.
//

#include "ChaiScriptedBehaviorTree.hpp"

bool ChaiScriptedBehaviorTree::load_tree()
{
    bool success = true;
    try
    {
        this->script.eval_file(this->script_path);
    }
    catch(chaiscript::exception::eval_error &ex)
    {
        std::cerr << ex.pretty_print();
        success = false;
    }
    return success;
}

void ChaiScriptedBehaviorTree::register_bt_interface()
{
    using namespace chaiscript;

    // workaround for exception bad any cast related to built-in boolean and std::function<bool()>:
    this->script.add_global_const(const_var(0), "False");
    this->script.add_global_const(const_var(1), "True");

    // BehaviorState enum:
    this->script.add(user_type<BehaviorState>(), "BehaviorState");
    this->script.add_global_const(const_var(BehaviorState::failure), "StateFailure");
    this->script.add_global_const(const_var(BehaviorState::success), "StateSuccess");
    this->script.add_global_const(const_var(BehaviorState::running), "StateRunning");
    this->script.add(fun([](const BehaviorState lhs, const BehaviorState rhs)
                         {
                             return (lhs == rhs);
                         }), "==");
    this->script.add(fun([](BehaviorState &lhs, const BehaviorState rhs)
                         {
                             return (lhs = rhs);
                         }), "=");

    // BehaviorTree global variable:
    this->script.add_global(var(static_cast<BehaviorTree *>(this)), "BT");

    // BehaviorTree adding nodes methods
    this->script.add(fun(&BehaviorTree::add_selector), "AddSelector");
    this->script.add(fun(&BehaviorTree::add_sequence), "AddSequence");
    this->script.add(fun(&BehaviorTree::add_action), "AddAction");
    this->script.add(fun(&BehaviorTree::add_condition), "AddCondition");
    this->script.add(fun(&BehaviorTree::add_invert), "AddInvert");
    this->script.add(fun(&BehaviorTree::add_loop), "AddLoop");
    this->script.add(fun(&BehaviorTree::add_max_N_tries), "AddMaxNTries");

    // BehaviorTree positioning methods:
    this->register_set_at_absolutely();
    this->register_set_at_relatively();
    this->script.add(fun(&BehaviorTree::set_at_id), "SetAtId");
}

ChaiScriptedBehaviorTree::ChaiScriptedBehaviorTree(std::string script_path)
        : IScriptedBehaviorTree{std::move(script_path)},
          script{}
{
    this->register_bt_interface();
}

void ChaiScriptedBehaviorTree::register_set_at_absolutely()
{
    using namespace chaiscript;
    const std::string set_at_absolutely_name = "SetAtAbsolutely";

    this->script.add(fun(&BehaviorTree::set_at_absolutely<>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script.add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
}

void ChaiScriptedBehaviorTree::register_set_at_relatively()
{
    using namespace chaiscript;
    const std::string set_at_relatively_name = "SetAtRelatively";

    this->script.add(fun(&BehaviorTree::set_at_relatively<>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script.add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
}
