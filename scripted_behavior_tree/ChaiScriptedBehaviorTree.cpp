//
// Created by draghan on 17.08.18.
//

#include <utility>
#include <filesystem>
#include "ChaiScriptedBehaviorTree.hpp"

std::vector<ChaiScriptedBehaviorTree::TreeReflection> ChaiScriptedBehaviorTree::known_scripted_trees;

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

    this->script.add(type_conversion<int, bool>([](const int &i)
                                                {
                                                    return i != 0;
                                                }));
    this->script.add(type_conversion<bool, int>([](const int &b)
                                                {
                                                    if(b)
                                                    {
                                                        return 1;
                                                    }
                                                    return 0;
                                                }));

    // BehaviorState enum:
    this->script.add(user_type<BehaviorState>(), "BehaviorState");
    this->script.add(constructor<BehaviorState(const BehaviorState &)>(), "BehaviorState");
    this->script.add(constructor<BehaviorState()>(), "BehaviorState");
    this->script.add_global_const(const_var(BehaviorState::failure), "StateFailure");
    this->script.add_global_const(const_var(BehaviorState::success), "StateSuccess");
    this->script.add_global_const(const_var(BehaviorState::running), "StateRunning");
    this->script.add(fun([](const BehaviorState &lhs, const BehaviorState &rhs)
                         {
                             return (lhs == rhs);
                         }), "==");
    this->script.add(fun([](const BehaviorState &lhs, const BehaviorState &rhs)
                         {
                             return (lhs != rhs);
                         }), "!=");
    this->script.add(fun([](BehaviorState &lhs, const BehaviorState &rhs)
                         {
                             return (lhs = rhs);
                         }), "=");

    // BehaviorTree global variable:
    this->script.add_global(var(static_cast<BehaviorTree *>(this)), "BT");
    this->register_all_known_scripted_trees();

    // BehaviorTree adding nodes methods
    this->script.add(fun(&BehaviorTree::add_selector), "AddSelector");
    this->script.add(fun(&BehaviorTree::add_sequence), "AddSequence");
    this->script.add(fun(&BehaviorTree::add_action), "AddAction");
    this->script.add(fun(&BehaviorTree::add_condition), "AddCondition");
    this->script.add(fun(&BehaviorTree::add_invert), "AddInvert");
    this->script.add(fun(&BehaviorTree::add_loop), "AddLoop");
    this->script.add(fun(&BehaviorTree::add_max_N_tries), "AddMaxNTries");
    this->script.add(fun(&BehaviorTree::add_link), "AddLink");


    // BehaviorTree positioning methods:
    this->register_set_at_absolutely();
    this->register_set_at_relatively();
    this->script.add(fun(&BehaviorTree::set_at_id), "SetAtId");
}

ChaiScriptedBehaviorTree::ChaiScriptedBehaviorTree(std::string script_path, std::string tree_name)
        : IScriptedBehaviorTree{script_path},
          script{},
          tree_id
                  {
                          tree_name.empty() ?
                          std::move(prepare_id_from_path(script_path)) :
                          std::move(prepare_id(tree_name))
                  }
{
    ChaiScriptedBehaviorTree::known_scripted_trees.push_back({this, this->tree_id});
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
    this->script
        .add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script
        .add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script
        .add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script
        .add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script
        .add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script
        .add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script
        .add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script
        .add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script
        .add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script
        .add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script
        .add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script
        .add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script
        .add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script
        .add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script
        .add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script
        .add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script
        .add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script
        .add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script
        .add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
    this->script
        .add(fun(&BehaviorTree::set_at_absolutely<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_absolutely_name);
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
    this->script
        .add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script
        .add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script
        .add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script
        .add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script
        .add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script
        .add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script
        .add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script
        .add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script
        .add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script
        .add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script
        .add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script
        .add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script
        .add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script
        .add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script
        .add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script
        .add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script
        .add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script
        .add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script
        .add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
    this->script
        .add(fun(&BehaviorTree::set_at_relatively<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>), set_at_relatively_name);
}

std::string ChaiScriptedBehaviorTree::get_tree_id() const
{
    return this->tree_id;
}

ChaiScriptedBehaviorTree::~ChaiScriptedBehaviorTree()
{
    auto found = std::find_if(known_scripted_trees.begin(), known_scripted_trees.end(),
                              [&](auto &tree_data)
                              {
                                  return tree_data.pointer == this;
                              });

    known_scripted_trees.erase(found);
}

void ChaiScriptedBehaviorTree::register_all_known_scripted_trees()
{
    for(const auto &tree_data : known_scripted_trees)
    {
        if(tree_data.pointer != this)
        {
            this->script
                .add_global(chaiscript::var(static_cast<BehaviorTree *>(tree_data.pointer)), tree_data.identifier);
        }
    }
}

std::string replace_all(const std::string &source, char what, char replacement)
{
    std::string result{source};
    std::replace(result.begin(), result.end(), what, replacement);
    return result;
}

std::string replace_first(const std::string &source, char what, char replacement)
{
    std::string result{source};
    std::replace(result.begin(), result.begin() + 1, what, replacement);
    return result;
}

std::string ChaiScriptedBehaviorTree::prepare_id_from_path(const std::string &path)
{
    const std::filesystem::path p{path};
    return p.stem();
}

std::string ChaiScriptedBehaviorTree::prepare_id(const std::string &name)
{
    const std::string forbidden_characters{"./\\:#$%^&*()-+={}[]|!@,<>?;'\"~` "};
    const std::string forbidden_first_characters{"0123456789"};
    const char replacement{'_'};

    std::string result{name};

    for(const auto &character : forbidden_characters)
    {
        result = replace_all(result, character, replacement);
    }

    for(const auto &character : forbidden_first_characters)
    {
        result = replace_first(result, character, replacement);
    }

    return result;
}