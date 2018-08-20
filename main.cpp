//
// Created by draghan on 17.08.18.
//

#include <ChaiScriptedBehaviorTree.hpp>

void evaluate_bt(BehaviorTree& bt)
{
    static size_t counter = 0;
    bt.set_at_absolutely();
    std::cout << "----- " << counter++ << ". eval: \n";
    auto tree_state = bt.evaluate();
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

int main()
{
    const std::string script_path{"./script.chai"};
    ChaiScriptedBehaviorTree bt{script_path};

    bt.load_tree();

    std::cout << "#nodes: " << bt.get_node_count() << '\n';
    bt.print(std::cout);
    evaluate_bt(bt);
    evaluate_bt(bt);
}