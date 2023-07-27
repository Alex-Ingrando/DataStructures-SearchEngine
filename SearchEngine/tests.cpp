#ifndef CATCH_CONFIG_RUNNER
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "queryprocessor.hpp"
#include <cstring>
#include <string>
using namespace std;

//AVL Tests by Alexandra Ingrando
TEST_CASE("AVL Tree Class", "[avltree]"){

    string a = "a";
    string aa = "aa";
    string aaa = "aaa";
    string b = "b";
    string f = "f";
    string d = "d";
    string j = "j";
    string i = "i";
    string k = "k";
    string l = "l";
    AVLIndex<string> tree;
    tree.add(b);
    tree.add(aa);
    tree.add(f);
    tree.add(a);
    tree.add(d);
    tree.add(aaa);
    tree.add(j);
    tree.add(i);
    tree.add(k);
    tree.add(l);

    //left rotation where every rotated node has both children
    //to make sure no nodes get cut off
    SECTION("Left Rotation"){
        REQUIRE(tree.inTree(a) == true);
        REQUIRE(tree.inTree(aa) == true);
        REQUIRE(tree.inTree(aaa) == true);
        REQUIRE(tree.inTree(b) == true);
        REQUIRE(tree.inTree(d) == true);
        REQUIRE(tree.inTree(f) == true);
        REQUIRE(tree.inTree(j) == true);
        REQUIRE(tree.inTree(i) == true);
        REQUIRE(tree.inTree(k) == true);
        REQUIRE(tree.inTree(l) == true);
    }

    tree.deleteIndex();
    SECTION("Delete Tree"){
        REQUIRE(tree.inTree(a) == false);
        REQUIRE(tree.inTree(aa) == false);
        REQUIRE(tree.inTree(aaa) == false);
        REQUIRE(tree.inTree(b) == false);
        REQUIRE(tree.inTree(d) == false);
        REQUIRE(tree.inTree(f) == false);
        REQUIRE(tree.inTree(j) == false);
        REQUIRE(tree.inTree(i) == false);
        REQUIRE(tree.inTree(k) == false);
        REQUIRE(tree.inTree(l) == false);
    }

    tree.add(i);
    tree.add(d);
    tree.add(k);
    tree.add(aaa);
    tree.add(j);
    tree.add(f);
    tree.add(l);
    tree.add(aa);
    tree.add(b);
    tree.add(a);

    //right rotation where every rotated node has both children
    //to make sure no nodes get cut off
    SECTION("Right Rotation"){
        REQUIRE(tree.inTree(a) == true);
        REQUIRE(tree.inTree(aa) == true);
        REQUIRE(tree.inTree(aaa) == true);
        REQUIRE(tree.inTree(b) == true);
        REQUIRE(tree.inTree(d) == true);
        REQUIRE(tree.inTree(f) == true);
        REQUIRE(tree.inTree(j) == true);
        REQUIRE(tree.inTree(i) == true);
        REQUIRE(tree.inTree(k) == true);
        REQUIRE(tree.inTree(l) == true);
    }

    tree.deleteIndex();

    //left rotation when no parent exists, ie head must change
    tree.add(d);
    tree.add(f);
    tree.add(b);
    tree.add(i);
    tree.add(j);
    tree.add(k);

    SECTION("Right-head rotation"){
        REQUIRE(tree.inTree(j) == true);
        REQUIRE(tree.inTree(f) == true);
        REQUIRE(tree.inTree(k) == true);
        REQUIRE(tree.inTree(d) == true);
        REQUIRE(tree.inTree(i) == true);
        REQUIRE(tree.inTree(b) == true);
    }

    tree.deleteIndex();

    //right rotation when no parent exists, ie head must change
    tree.add(j);
    tree.add(f);
    tree.add(k);
    tree.add(d);
    tree.add(i);
    tree.add(b);

    SECTION("Right-head rotation"){
        REQUIRE(tree.inTree(j) == true);
        REQUIRE(tree.inTree(f) == true);
        REQUIRE(tree.inTree(k) == true);
        REQUIRE(tree.inTree(d) == true);
        REQUIRE(tree.inTree(i) == true);
        REQUIRE(tree.inTree(b) == true);
    }

    tree.deleteIndex();

    tree.add(d);
    tree.add(a);
    tree.add(b);

    SECTION("Left-right rotation"){
        REQUIRE(tree.inTree(d) == true);
        REQUIRE(tree.inTree(a) == true);
        REQUIRE(tree.inTree(b) == true);
    }
}

#endif
