//
// Created by draghan on 17.08.18.
//

#include "ScriptedBT.hpp"

ScriptedBT::ScriptedBT(const std::string &path)
        : path{path},
          script{}
{
    this->export_bt_interface();
}

void ScriptedBT::export_bt_interface()
{
    using namespace chaiscript;

    // BehaviorState enum:
    this->script.add(user_type<BehaviorState>(), "BehaviorState");
    this->script.add_global_const(const_var(BehaviorState::failure), "BehaviorStateFailure");
    this->script.add_global_const(const_var(BehaviorState::success), "BehaviorStateSuccess");
    this->script.add_global_const(const_var(BehaviorState::running), "BehaviorStateRunning");
    this->script.add(fun([](const BehaviorState lhs, const BehaviorState rhs)
                         {
                             return (lhs == rhs);
                         }), "==");
    this->script.add(fun([](BehaviorState &lhs, const BehaviorState rhs)
                         {
                             return (lhs = rhs);
                         }), "=");

    // BehaviorTree global variable
    this->script.add_global(var(&(this->behavior_tree)), "behavior_tree");

    // BehaviorTree adding nodes methods
    this->script.add(fun(&BehaviorTree::add_selector),"add_selector");
    this->script.add(fun(&BehaviorTree::add_sequence),"add_sequence");
    this->script.add(fun(&BehaviorTree::add_action),"add_action");
    this->script.add(fun(&BehaviorTree::add_condition),"add_condition");
    this->script.add(fun(&BehaviorTree::add_invert),"add_invert");
    this->script.add(fun(&BehaviorTree::add_loop),"add_loop");
    this->script.add(fun(&BehaviorTree::add_max_N_tries),"add_max_N_tries");
}

void ScriptedBT::run_script()
{
    this->script.eval_file(this->path);
}
