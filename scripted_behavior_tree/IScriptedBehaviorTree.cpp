//
// Created by draghan on 17.08.18.
//

#include "IScriptedBehaviorTree.hpp"

IScriptedBehaviorTree::IScriptedBehaviorTree(std::string script_path)
        : script_path{std::move(script_path)}
{
}
