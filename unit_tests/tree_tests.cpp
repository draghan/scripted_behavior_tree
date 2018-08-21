/*
    This file is distributed under MIT License.

    Copyright (c) 2018 draghan

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

//
// Created by draghan on 2017-10-17.
//

#include <fstream>
#include "../external/catch/single_include/catch2/catch.hpp"
#include "../scripted_behavior_tree/ChaiScriptedBehaviorTree.hpp"
#include "../behavior_system/primitive/BehaviorAction.hpp"
#include "../behavior_system/composite/BehaviorSequence.hpp"

enum MockState
{
    StateSuccess,
    StateFailure,
    StateRunning
};

struct MockTree
{
    void AddSequence();
    void AddSelector();
    void AddAction(std::function<MockState()>);
    void AddCondition(std::function<bool()>);
    void SetAtAbsolutely(int);
};


class ScriptedTreeForTest: public ChaiScriptedBehaviorTree
{
public:
    explicit ScriptedTreeForTest(std::string script)
            : ChaiScriptedBehaviorTree{"unit_test.chai"}
    {
        std::ofstream script_file{script_path};
        script_file << script;
        script_file.close();
    }

    void register_global_int(int &variable, std::string name)
    {
        this->script.add_global(chaiscript::var(&variable), name);
    }

    void execute(const std::string &code)
    {
        this->script.eval(code);
    }
};

TEST_CASE("Testing tree interface of BehaviorTree class", "[Tree]")
{
    SECTION("Test if \'running\' evaluation is correctly done")
    {
        std::string script = R"chaiscript(
        def action1()
        {
            a1 = a1 + 1;
            return StateSuccess;
        }
        def action2()
        {
            ++a2;
            if(a2 < 3)
            {
                return StateRunning;
            }
            else
            {
                return StateSuccess;
            }
        }

        def action3()
        {
            ++a3;
            return StateSuccess;
        }

        def action_false()
        {
            return StateFailure;
        };

        BT.AddSelector();
        BT.SetAtAbsolutely();
        BT.AddAction(action_false);
        BT.AddSequence();
        BT.SetAtAbsolutely(1);
        BT.AddAction(action1);
        BT.AddAction(action2);
        BT.AddAction(action3);
        )chaiscript";


        int a1 = 0;
        int a2 = 0;
        int a3 = 0;
        ScriptedTreeForTest tree{script};
        tree.register_global_int(a1, "a1");
        tree.register_global_int(a2, "a2");
        tree.register_global_int(a3, "a3");

        tree.load_tree();
        tree.set_at_absolutely();
        REQUIRE(tree.get_node_count() == 6);

        REQUIRE(tree.evaluate() == BehaviorState::running);
        REQUIRE(a1 == 1);
        REQUIRE(a2 == 1);
        REQUIRE(a3 == 0);

        REQUIRE(tree.evaluate() == BehaviorState::running);
        REQUIRE(a1 == 1);
        REQUIRE(a2 == 2);
        REQUIRE(a3 == 0);

        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(a1 == 1);
        REQUIRE(a2 == 3);
        REQUIRE(a3 == 1);

        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(a1 == 2);
        REQUIRE(a2 == 4);
        REQUIRE(a3 == 2);
    }

    SECTION("Test parent-related methods of nodes")
    {
        /*
                 selector-root-0
              __________|____________
              |         |           |
            sequence-1  action-4  action-5
              ___|_______
              |         |
            action-2  action-3

         */
        std::string script = R"chaiscript(
        BT.AddSelector();
        BT.AddSequence();
        BT.SetAtRelatively(0);
        BT.AddCondition(fun()
                           {
                               return False;
                           });
        BT.AddCondition(fun()
                           {
                               return True;
                           });
        BT.SetAtAbsolutely();
        BT.AddCondition(fun()
                           {
                               return True;
                           });
        BT.AddCondition(fun()
                           {
                               return True;
                           });

        )chaiscript";

        ScriptedTreeForTest tree{script};
        tree.load_tree();
        REQUIRE(tree.get_node_count() == 6);
        auto root = tree.get();
        REQUIRE(root->get_id() == 0);

        tree.set_at_absolutely(0);
        auto seq_1 = tree.get();
        REQUIRE(seq_1->get_id() == 1);

        tree.set_at_absolutely(1);
        auto act_4 = tree.get();
        REQUIRE(act_4->get_id() == 4);

        tree.set_at_absolutely(2);
        auto act_5 = tree.get();
        REQUIRE(act_5->get_id() == 5);

        tree.set_at_absolutely(0, 0);
        auto act_2 = tree.get();
        REQUIRE(act_2->get_id() == 2);

        tree.set_at_absolutely(0, 1);
        auto act_3 = tree.get();
        REQUIRE(act_3->get_id() == 3);


        REQUIRE(root->get_parent() == nullptr);
        REQUIRE(seq_1->get_parent() == root);
        REQUIRE(act_4->get_parent() == root);
        REQUIRE(act_5->get_parent() == root);
        REQUIRE(act_2->get_parent() == seq_1);
        REQUIRE(act_3->get_parent() == seq_1);

        REQUIRE(act_2->draw_helper.get_left_most_sibling() == act_2);
        REQUIRE(act_2->draw_helper.get_next_sibling() == act_3);
        REQUIRE(act_5->draw_helper.get_left_most_sibling() == seq_1);
        REQUIRE(act_5->draw_helper.get_next_sibling() == nullptr);
        REQUIRE(act_5->draw_helper.get_previous_sibling() == act_4);

        REQUIRE(root->draw_helper.get_left_most_sibling() == nullptr);
        REQUIRE(root->draw_helper.get_next_sibling() == nullptr);
        REQUIRE(root->draw_helper.get_previous_sibling() == nullptr);
    }

    SECTION("Test correct behavior after adding 2 children to root")
    {
        std::string script = R"chaiscript(
        BT.AddSequence();
        BT.AddAction(fun()
                        {
                            ++counter;
                            return StateSuccess;
                        });
        BT.AddAction(fun()
                        {
                            ++counter;
                            return StateSuccess;
                        });

        )chaiscript";
        int counter = 0;
        ScriptedTreeForTest tree{script};
        tree.register_global_int(counter, "counter");
        tree.load_tree();
        REQUIRE(tree.get_node_count() == 3);

        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(counter == 2);

        REQUIRE(tree.set_at_absolutely(0) == true);
        REQUIRE(dynamic_cast<BehaviorAction *>(tree.get()) != nullptr);

        REQUIRE(tree.set_at_absolutely() == true);
        REQUIRE(dynamic_cast<BehaviorSequence *>(tree.get()) != nullptr);

        counter = 0;
        REQUIRE(tree.set_at_absolutely(1) == true);
        REQUIRE(tree.get_id() == 2);
        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(counter == 1);

        REQUIRE(tree.set_at_absolutely() == true);
        REQUIRE(tree.set_at_relatively(0) == true);
        REQUIRE(tree.get_id() == 1);

        REQUIRE(tree.set_at_relatively(0) == false);
        REQUIRE(tree.set_at_relatively(-1) == false);
        REQUIRE(tree.set_at_relatively(1) == false);

    }


    SECTION("Testing \'set_at_absolutely\' with {0-> {1-> {3(), 4()}} {2()}}")
    {

        std::string script = R"chaiscript(
        BT.AddSequence();
        BT.AddSequence();
        BT.AddAction(fun()
                        {
                            ++counter;
                            id = 2;
                            return StateSuccess;
                        });

        BT.SetAtAbsolutely(0);

        BT.AddAction(fun()
                        {
                            ++counter;
                            id = 3;
                            return StateSuccess;
                        });
        BT.AddAction(fun()
                        {
                            ++counter;
                            id = 4;
                            return StateSuccess;
                        });

        )chaiscript";

        int id = 0;
        int counter = 0;

        ScriptedTreeForTest tree{script};
        tree.register_global_int(id, "id");
        tree.register_global_int(counter, "counter");
        tree.load_tree();

        REQUIRE(tree.set_at_absolutely() == true);
        REQUIRE(dynamic_cast<BehaviorSequence *>(tree.get()) != nullptr);
        REQUIRE(tree.get_id() == 0);

        REQUIRE(tree.set_at_absolutely(0) == true);
        REQUIRE(tree.set_at_absolutely(1) == true);
        REQUIRE(tree.set_at_absolutely(1, 0) == false);
        REQUIRE(tree.set_at_absolutely(2) == false);
        REQUIRE(tree.set_at_absolutely(0, 0) == true);
        REQUIRE(tree.set_at_absolutely(0, 1) == true);
        REQUIRE(tree.set_at_absolutely(0, 2) == false);
        REQUIRE(tree.set_at_absolutely(-1) == false);
        REQUIRE(tree.set_at_absolutely(100) == false);

        REQUIRE(tree.set_at_absolutely(0, 0));
        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(counter == 1);
        REQUIRE(id == 3);
        REQUIRE(tree.get_id() == 3);
        counter = 0;
        REQUIRE(tree.set_at_absolutely(0, 1));
        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(counter == 1);
        REQUIRE(id == 4);
        REQUIRE(tree.get_id() == 4);

        counter = 0;
        REQUIRE(tree.set_at_absolutely(1));
        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(counter == 1);
        REQUIRE(id == 2);
        REQUIRE(tree.get_id() == 2);
    }

    SECTION("Testing \'set_at_relatively\' with {0-> {1-> {3(), 4()}} {2()}}")
    {
        std::string script = R"chaiscript(
        BT.AddSequence();
        BT.AddSequence();
        BT.AddAction(fun()
                        {
                            ++counter;
                            id = 2;
                            return StateFailure;
                        });

        BT.SetAtAbsolutely(0);
        BT.AddAction(fun()
                        {
                            ++counter;
                            id = 3;
                            return StateFailure;
                        });
        BT.AddAction(fun()
                        {
                            ++counter;
                            id = 4;
                            return StateFailure;
                        });
        )chaiscript";

        int id = 0;
        int counter = 0;
        ScriptedTreeForTest tree{script};
        tree.register_global_int(id, "id");
        tree.register_global_int(counter, "counter");
        tree.load_tree();

        REQUIRE(tree.set_at_absolutely() == true);
        REQUIRE(dynamic_cast<BehaviorSequence *>(tree.get()) != nullptr);
        REQUIRE(tree.get_id() == 0);

        REQUIRE(tree.set_at_relatively(0) == true);
        REQUIRE(dynamic_cast<BehaviorSequence *>(tree.get()) != nullptr);
        REQUIRE(tree.get_id() == 1);

        REQUIRE(tree.set_at_relatively(0) == true);
        REQUIRE(dynamic_cast<BehaviorAction *>(tree.get()) != nullptr);
        REQUIRE(tree.get_id() == 3);
        REQUIRE(tree.evaluate() == BehaviorState::failure);
        REQUIRE(counter == 1);
        REQUIRE(id == 3);

        REQUIRE(tree.set_at_relatively(0) == false);
        REQUIRE(tree.set_at_absolutely() == true);
        REQUIRE(tree.set_at_relatively(0, 1) == true);
        REQUIRE(dynamic_cast<BehaviorAction *>(tree.get()) != nullptr);
        REQUIRE(tree.get_id() == 4);
        counter = 0;
        REQUIRE(tree.evaluate() == BehaviorState::failure);
        REQUIRE(counter == 1);
        REQUIRE(id == 4);
        REQUIRE(tree.set_at_relatively(0) == false);
        REQUIRE(tree.set_at_absolutely() == true);

        REQUIRE(tree.set_at_relatively(1) == true);
        REQUIRE(dynamic_cast<BehaviorAction *>(tree.get()) != nullptr);
        REQUIRE(tree.get_id() == 2);
        counter = 0;
        REQUIRE(tree.evaluate() == BehaviorState::failure);
        REQUIRE(counter == 1);
        REQUIRE(id == 2);
        REQUIRE(tree.set_at_relatively(0) == false);
        REQUIRE(tree.set_at_relatively(1) == false);
        REQUIRE(tree.set_at_relatively(-1) == false);
    }
}

TEST_CASE("Testing \'behavior part\' interface of BehaviorTree class", "[Tree]")
{
    SECTION("Tree with sequences of success actions")
    {
        int action_count = 0;
        MockTree BT;
        std::string script = R"chai(
        BT.AddSequence();
        BT.AddAction(fun()
                     {
                         ++action_count;
                         return StateSuccess;
                     });
        BT.AddAction(fun()
                     {
                         ++action_count;
                         return StateSuccess;
                     });
        BT.AddAction(fun()
                     {
                         ++action_count;
                         return StateSuccess;
                     });
        BT.AddAction(fun()
                     {
                         ++action_count;
                         return StateSuccess;
                     });
        )chai";
        ScriptedTreeForTest tree{script};
        tree.register_global_int(action_count, "action_count");
        tree.load_tree();

        REQUIRE(tree.get_node_count() == 5);
        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(action_count == 4);
        action_count = 0;

        tree.add_sequence();
        REQUIRE(tree.set_at_relatively(4) == true);
        tree.add_action([&]()
                        {
                            ++action_count;
                            return BehaviorState::success;
                        });
        tree.add_action([&]()
                        {
                            ++action_count;
                            return BehaviorState::success;
                        });

        REQUIRE(tree.get_node_count() == 8);
        REQUIRE(tree.set_at_absolutely());
        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(action_count == 6);
    }

    SECTION("Tree with sequences of success and failure actions")
    {
        int action_count = 0;
        MockTree BT;
        std::string script = R"chai(
        BT.AddSequence();
        BT.AddAction(fun()
                        {
                            ++action_count;
                            return StateSuccess;
                        });
        BT.AddAction(fun()
                        {
                            ++action_count;
                            return StateSuccess;
                        });
        BT.AddAction(fun()
                        {
                            ++action_count;
                            return StateSuccess;
                        });
        BT.AddAction(fun()
                        {
                            ++action_count;
                            return StateSuccess;
                        });
        )chai";

        ScriptedTreeForTest tree{script};
        tree.register_global_int(action_count, "action_count");
        tree.load_tree();

        REQUIRE(tree.get_node_count() == 5);
        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(action_count == 4);
        action_count = 0;

        std::string code = R"chai(
        BT.AddSequence();
        BT.SetAtAbsolutely(4);
        BT.AddAction(fun()
                        {
                            ++action_count;
                            return StateFailure;
                        });
        BT.AddAction(fun()
                        {
                            ++action_count;
                            return StateSuccess;
                        });
        )chai";
        tree.execute(code);

        REQUIRE(tree.get_node_count() == 8);
        REQUIRE(tree.set_at_absolutely());
        REQUIRE(tree.evaluate() == BehaviorState::failure);
        REQUIRE(action_count == 5);
    }
}

TEST_CASE("Testing factor part of BehaviorTree class", "[Tree]")
{
    SECTION("Adding one copy of each behavior node")
    {
        MockTree BT;
        ScriptedTreeForTest tree{""};
        tree.load_tree();
        REQUIRE(tree.set_at_absolutely() == false);
        REQUIRE(tree.get_id() == BehaviorTree::undefined_id);
        REQUIRE(tree.get_node_count() == 0);
        REQUIRE(tree.get() == nullptr);

        tree.execute("BT.AddSelector();");
        REQUIRE(tree.get_id() == 0);
        REQUIRE(tree.get_node_count() == 1);
        REQUIRE(dynamic_cast<BehaviorSelector *>(tree.get()) != nullptr);

        tree.execute("BT.AddSequence();");
        REQUIRE(tree.get_id() == 0);
        REQUIRE(tree.get_node_count() == 2);
        REQUIRE(dynamic_cast<BehaviorSelector *>(tree.get()) != nullptr);
        REQUIRE(tree.set_at_relatively(0) == true);
        REQUIRE(tree.get_id() == 1);
        REQUIRE(dynamic_cast<BehaviorSequence *>(tree.get()) != nullptr);
        REQUIRE(tree.set_at_absolutely(1) == false);

        REQUIRE(tree.set_at_absolutely(0) == true);

        tree.execute(R"(BT.AddCondition(fun()
                               {
                                   return True;
                               });
        )");

        int action_counter = 0;
        tree.register_global_int(action_counter, "action_counter");
        tree.execute(R"(BT.AddAction(fun()
                                {
                                    ++action_counter;
                                    return StateSuccess;
                                });
        )");
        REQUIRE(tree.get_node_count() == 4);

        REQUIRE(tree.set_at_absolutely(0, 0));
        REQUIRE(tree.get_id() == 2);
        REQUIRE(dynamic_cast<BehaviorCondition *>(tree.get()) != nullptr);

        REQUIRE(tree.set_at_absolutely(0, 1));
        REQUIRE(tree.get_id() == 3);
        REQUIRE(dynamic_cast<BehaviorAction *>(tree.get()) != nullptr);

        REQUIRE(tree.set_at_absolutely(0, 2) == false);

        REQUIRE(tree.set_at_absolutely());
        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(action_counter == 1);
    }
}