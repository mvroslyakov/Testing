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
  auto temp_path = boost::filesystem::temp_directory_path();
  ASSERT_ANY_THROW(GetTree("task5", false));
  ASSERT_ANY_THROW(GetTree("task5", true));
  
  boost::filesystem::remove_all(temp_path / "task6" );
  boost::filesystem::create_directory(temp_path / "task6" );
  auto test_path = (temp_path / "task6/example.cpp").string();
  std::ofstream file(test_path);
  file << "Hello!\n";
  file.close();
  
  ASSERT_ANY_THROW(GetTree(test_path, false));
  ASSERT_ANY_THROW(GetTree(test_path, true));
  boost::filesystem::remove_all(temp_path / "task6" );
}

TEST(GetTreeTest, Exist) {
  auto temp_path = boost::filesystem::temp_directory_path();
  std::cout << temp_path.string();
  boost::filesystem::remove_all(temp_path / "tasks" );
  boost::filesystem::create_directory(temp_path / "tasks" );
  boost::filesystem::create_directory(temp_path / "tasks/task" );
  boost::filesystem::create_directory(temp_path / "tasks/task1" );
  boost::filesystem::create_directory(temp_path / "tasks/task2" );
  auto file_path = (temp_path / "tasks/task/example.cpp").string();
  std::ofstream file(file_path);
  file << "Hello!\n";
  file.close();
  auto test_path = (temp_path / "tasks").string();
  auto node_full = GetTree(test_path, false);
  auto node_only_dir = GetTree(test_path, true);
  std::cout << "NODE FULL\n";
  Print(node_full);
  FileNode res_full = {"tasks", true, {
    {"task2",true,{}},
    {"task",true,{{"example.cpp", false, {}}}},
    {"task1",true,{}},
  }};
  ASSERT_EQ(res_full, node_full);
  
  std::cout << "NODE only_dir";
  Print(node_only_dir);
  FileNode res_dir_only = {"tasks", true, {
    {"task2",true,{}},
    {"task",true,{}},
    {"task1",true,{}}
  }};
  ASSERT_EQ(res_dir_only , node_only_dir);
  
  FilterEmptyNodes(node_full, test_path);
  auto newnode = GetTree(test_path, false);
  std::cout << "NODE clear";
  Print(newnode);
  FileNode clear_dir = {"tasks", true, {
    {"task",true,{{"example.cpp", false, {}}}}
  }};
  ASSERT_EQ(clear_dir, newnode);
  ASSERT_FALSE(clear_dir == node_full);
  
  boost::filesystem::remove_all(temp_path / "tasks");
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



