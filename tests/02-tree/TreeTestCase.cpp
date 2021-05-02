//
// Created by akhtyamovpavel on 5/1/20.
//


#include "TreeTestCase.h"
#include "Tree.h"
#include<iostream>

void Print(const FileNode& node) {
  std::cout << node.name << '\n';
  std::cout << node.is_dir << '\n';
  for (auto&& elem : node.children) {
    Print(elem);
  }
}

TEST(GetTreeTest, NotExist) {
  ASSERT_ANY_THROW(GetTree("task5", false));
  ASSERT_ANY_THROW(GetTree("task5", true));
  
  boost::filesystem::remove_all( "task6" );
  boost::filesystem::create_directory( "task6" );
  std::ofstream file( "task6/example.cpp" );
  file << "Hello!\n";
  file.close();
  
  ASSERT_ANY_THROW(GetTree("task6/example.cpp", false));
  ASSERT_ANY_THROW(GetTree("task6/example.cpp", true));
  boost::filesystem::remove_all( "task6" );
}

TEST(GetTreeTest, Exist) {
  boost::filesystem::remove_all( "tasks" );
  boost::filesystem::create_directory( "tasks" );
  boost::filesystem::create_directory( "tasks/task" );
  boost::filesystem::create_directory( "tasks/task1" );
  boost::filesystem::create_directory( "tasks/task2" );
  std::ofstream file( "tasks/task/example.cpp" );
  file << "Hello!\n";
  file.close();
  auto node_full = GetTree("tasks", false);
  auto node_only_dir = GetTree("tasks", true);
  std::cout << "NODE FULL";
  Print(node_full);
  FileNode res_full = {"tasks", true, {
    {"task",true,{{"example.cpp", false, {}}}},
    {"task1",true,{}},
    {"task2",true,{}}
  }};
  ASSERT_EQ(res_full, node_full);
  
  std::cout << "NODE only_dir";
  Print(node_only_dir);
  FileNode res_dir_only = {"tasks", true, {
    {"task",true,{}},
    {"task1",true,{}},
    {"task2",true,{}}
  }};
  ASSERT_EQ(res_dir_only , node_only_dir);
  
  FilterEmptyNodes(node_full, "tasks");
  auto newnode = GetTree("tasks", false);
  std::cout << "NODE clear";
  Print(newnode);
  FileNode clear_dir = {"tasks", true, {
    {"task",true,{{"example.cpp", false, {}}}}
  }};
  ASSERT_EQ(clear_dir, newnode);
  ASSERT_FALSE(clear_dir == node_full);
  
  boost::filesystem::remove_all( "tasks" );
}

TEST(GetTreeTest, Operator) {
  FileNode temp1 = {"temp1", true, {}};
  FileNode temp2 = {"temp2", true, {}};
  ASSERT_FALSE(temp1 == temp2);
  
  FileNode temp3 = {"temp1", true, {}};
  FileNode temp4 = {"temp1", false, {}};
  ASSERT_FALSE(temp3 == temp4);
  
  FileNode temp5 = {"temp1", true, {{"temp2", false, {}}}};
  FileNode temp6 = {"temp1", true, {}};
  ASSERT_FALSE(temp5 == temp6);
  
}



