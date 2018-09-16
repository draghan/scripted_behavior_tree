//
// Created by draghan on 17.08.18.
//

#include <utility>
#include <filesystem>
#include "ChaiScriptedBehaviorTree.hpp"

std::vector<ChaiScriptedBehaviorTree *> ChaiScriptedBehaviorTree::known_scripted_trees;
const ChaiScriptedBehaviorTree::extended_array_id_t ChaiScriptedBehaviorTree::identifiers
        {
                "AddSequence",
                "AddSelector",
                "AddCondition",
                "AddAction",
                "AddLink",
                "AddInvert",
                "AddLoop",
                "AddMaxNTries",
                "BT",
                "BehaviorState",
                "StateFailure",
                "StateSuccess",
                "StateRunning",
                "SetAtRelatively",
                "SetAtAbsolutely",
                "SetAtId",
        };

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
    this->register_boolean();

    this->register_behavior_state();

    this->register_this_tree();

    this->register_all_known_scripted_trees();

    this->register_adding_nodes();

    this->register_positioning();
}

void ChaiScriptedBehaviorTree::register_positioning()
{
    this->script.add(chaiscript::fun(&BehaviorTree::set_at_id), identifiers[ScriptIdentifier::set_at_id]);
    this->register_set_at_absolutely();
    this->register_set_at_relatively();
}

void ChaiScriptedBehaviorTree::register_adding_nodes()
{
    this->script.add(chaiscript::fun(&BehaviorTree::add_selector), identifiers[ScriptIdentifier::selector]);
    this->script.add(chaiscript::fun(&BehaviorTree::add_sequence), identifiers[ScriptIdentifier::sequence]);
    this->script.add(chaiscript::fun(&BehaviorTree::add_action), identifiers[ScriptIdentifier::action]);
    this->script.add(chaiscript::fun(&BehaviorTree::add_condition), identifiers[ScriptIdentifier::condition]);
    this->script.add(chaiscript::fun(&BehaviorTree::add_invert), identifiers[ScriptIdentifier::invert]);
    this->script.add(chaiscript::fun(&BehaviorTree::add_loop), identifiers[ScriptIdentifier::loop]);
    this->script.add(chaiscript::fun(&BehaviorTree::add_max_N_tries), identifiers[ScriptIdentifier::max_n_tries]);
    this->script.add(chaiscript::fun(&BehaviorTree::add_link), identifiers[ScriptIdentifier::link]);
}

void ChaiScriptedBehaviorTree::register_this_tree()
{
    this->script
        .add_global(chaiscript::var(static_cast<BehaviorTree *>(this)), identifiers[ScriptIdentifier::this_tree]);
}

void ChaiScriptedBehaviorTree::register_boolean()
{
    // workaround for exception bad any cast related to built-in boolean and std::function<bool()>:
    this->script.add_global_const(chaiscript::const_var(0), "False");
    this->script.add_global_const(chaiscript::const_var(1), "True");

    this->script.add(chaiscript::type_conversion<int, bool>([](const int &i)
                                                            {
                                                                return i != 0;
                                                            }));
    this->script.add(chaiscript::type_conversion<bool, int>([](const int &b)
                                                            {
                                                                if(b)
                                                                {
                                                                    return 1;
                                                                }
                                                                return 0;
                                                            }));
}

void ChaiScriptedBehaviorTree::register_behavior_state()
{
    this->script.add(chaiscript::user_type<BehaviorState>(), identifiers[ScriptIdentifier::behavior_state]);
    this->script
        .add(chaiscript::constructor<BehaviorState(const BehaviorState &)>(), identifiers[ScriptIdentifier::behavior_state]);
    this->script.add(chaiscript::constructor<BehaviorState()>(), identifiers[ScriptIdentifier::behavior_state]);
    this->script
        .add_global_const(chaiscript::const_var(BehaviorState::failure), identifiers[ScriptIdentifier::state_failure]);
    this->script
        .add_global_const(chaiscript::const_var(BehaviorState::success), identifiers[ScriptIdentifier::state_success]);
    this->script
        .add_global_const(chaiscript::const_var(BehaviorState::running), identifiers[ScriptIdentifier::state_running]);
    this->script.add(chaiscript::fun([](const BehaviorState &lhs, const BehaviorState &rhs)
                                     {
                                         return (lhs == rhs);
                                     }), "==");
    this->script.add(chaiscript::fun([](const BehaviorState &lhs, const BehaviorState &rhs)
                                     {
                                         return (lhs != rhs);
                                     }), "!=");
    this->script.add(chaiscript::fun([](BehaviorState &lhs, const BehaviorState &rhs)
                                     {
                                         return (lhs = rhs);
                                     }), "=");
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
    ChaiScriptedBehaviorTree::known_scripted_trees.push_back(this);
    this->register_bt_interface();
}

void ChaiScriptedBehaviorTree::register_set_at_absolutely()
{
    using namespace chaiscript;
    const std::string set_at_absolutely_name = identifiers[ScriptIdentifier::set_at_absolutely];

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
    const std::string set_at_relatively_name = identifiers[ScriptIdentifier::set_at_relatively];

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
                                  return tree_data == this;
                              });

    known_scripted_trees.erase(found);
}

void ChaiScriptedBehaviorTree::register_all_known_scripted_trees()
{
    for(const auto &tree_data : known_scripted_trees)
    {
        if(tree_data != this)
        {
            this->script.add_global(chaiscript::var(static_cast<BehaviorTree *>(tree_data)), tree_data->get_tree_id());
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