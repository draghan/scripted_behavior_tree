//
// Created by draghan on 17.08.18.
//

#include "scripted_behavior_tree/ChaiScriptedBehaviorTree.hpp"

void evaluate_bt(BehaviorTree &bt);

int main()
{
    // The main difference against regular
    // BehaviorTree is that you have to provide
    // a path to the script file in the constructor
    // and then call load_tree() method, which could
    // throw an exception.
    // You have to be careful when evaluating an
    // ChaiScriptedBehaviorTree - if there are bugs
    // in script file, an exception will be thrown.
    const std::string script_path{"./example.chai"};
    ChaiScriptedBehaviorTree bt{script_path};
    try
    {
        bt.load_tree();

        std::cout << "#nodes: " << bt.get_node_count() << '\n';
        bt.print(std::cout);

        evaluate_bt(bt);
        evaluate_bt(bt);
    }
    catch(std::exception &ex)
    {
        std::cerr << "Oooops, your's Chai is cold now. :<\n" << ex.what();
        return 1;
    }
}

void evaluate_bt(BehaviorTree &bt)
{
    static size_t counter = 0;

    // some printing...
    std::cout << "----- " << counter++ << ". eval: \n";

    // go back to root node wherever our tree actually is:
    bt.set_at_absolutely();
    auto tree_state = bt.evaluate();

    // some more printing...
    switch(tree_state)
    {
        case BehaviorState::undefined:
        {
            std::cout << "\nundefined\n";
            break;
        }
        case BehaviorState::success:
        {
            std::cout << "\nsuccess\n";
            break;
        }
        case BehaviorState::failure:
        {
            std::cout << "\nfailure\n";
            break;
        }
        case BehaviorState::running:
        {
            std::cout << "\nrunning\n";
            break;
        }
    }
}