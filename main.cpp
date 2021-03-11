#include <string>
#include "bst.h"

using Dict = BST<int, std::string>;

int main(int, char**) {

    Dict *test = new Dict();

    test->insert(22, "test1");
    test->insert(26, "test2");
    test->insert(13, "test3");
    test->insert(14, "test4");
    test->insert(16, "test5");
    test->insert(15, "test6");

    test->displayTree();

    delete test;
}
