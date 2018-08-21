cmake_minimum_required(VERSION 3.12)
project(scripted_behavior_tree)

add_compile_options(-std=c++1z -Os)

include_directories(external/bt/behavior_system)
include_directories(external/bt/behavior_system/composite)
include_directories(external/bt/behavior_system/decorator)
include_directories(external/bt/behavior_system/primitive)
include_directories(external/bt/behavior_system/tree)
include_directories(external/chaiscript/include/chaiscript)
include_directories(scripted_behavior_tree)

set(SOURCE_FILES external/bt/behavior_system/composite/BehaviorSelector.cpp
        external/bt/behavior_system/composite/BehaviorSelector.hpp
        external/bt/behavior_system/composite/BehaviorSequence.cpp
        external/bt/behavior_system/composite/BehaviorSequence.hpp
        external/bt/behavior_system/decorator/DecoratorInvert.cpp
        external/bt/behavior_system/decorator/DecoratorInvert.hpp
        external/bt/behavior_system/decorator/DecoratorLink.cpp
        external/bt/behavior_system/decorator/DecoratorLink.hpp
        external/bt/behavior_system/decorator/DecoratorLoop.cpp
        external/bt/behavior_system/decorator/DecoratorLoop.hpp
        external/bt/behavior_system/decorator/DecoratorMaxNTries.cpp
        external/bt/behavior_system/decorator/DecoratorMaxNTries.hpp
        external/bt/behavior_system/primitive/BehaviorAction.cpp
        external/bt/behavior_system/primitive/BehaviorAction.hpp
        external/bt/behavior_system/primitive/BehaviorCondition.cpp
        external/bt/behavior_system/primitive/BehaviorCondition.hpp
        external/bt/behavior_system/tree/BehaviorTree.cpp
        external/bt/behavior_system/tree/BehaviorTree.hpp
        external/bt/behavior_system/BehaviorEmpty.cpp
        external/bt/behavior_system/BehaviorEmpty.hpp
        external/bt/behavior_system/EditorMetadata.cpp
        external/bt/behavior_system/EditorMetadata.hpp
        external/bt/behavior_system/IBehavior.cpp
        external/bt/behavior_system/IBehavior.hpp
        scripted_behavior_tree/IScriptedBehaviorTree.cpp
        scripted_behavior_tree/IScriptedBehaviorTree.hpp
        scripted_behavior_tree/ChaiScriptedBehaviorTree.cpp
        scripted_behavior_tree/ChaiScriptedBehaviorTree.hpp
        )

add_executable(scripted_behavior_tree
        ${SOURCE_FILES}
        main.cpp
        )

add_executable(unit_tests
        ${SOURCE_FILES}
        unit_tests/main_tests.cpp
        unit_tests/tree_tests.cpp
        )


target_link_libraries(scripted_behavior_tree -ldl -lpthread)
target_link_libraries(unit_tests -ldl -lpthread)
