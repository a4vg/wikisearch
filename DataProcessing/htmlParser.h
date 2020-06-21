#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <myhtml/api.h>
#include <fstream>

std::string parse_html(std::string);

void traverse_tree(myhtml_tree_t*, myhtml_tree_node_t *, size_t , std::string &);
