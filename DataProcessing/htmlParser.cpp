#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <myhtml/api.h>
#include <fstream>
#include "htmlParser.h"

using namespace std;

void traverse_tree(myhtml_tree_t* tree, myhtml_tree_node_t *node, size_t inc, string &text)
{
    while (node)
    {

        const char *tag_name = myhtml_tag_name_by_id(tree, myhtml_node_tag_id(node), NULL);

        string name(tag_name);

        myhtml_tag_id_t tag_id = myhtml_node_tag_id(node);


        if(tag_id == MyHTML_TAG__TEXT ) {
                const char* node_text = myhtml_node_text(node, NULL);
                std::istringstream iss(node_text);
                std::string tmp_str;
                while(iss>>tmp_str)
                    text += tmp_str + ' ';
        }

        if(name != "style" and name != "annotation" and name != "script"){
            traverse_tree(tree, myhtml_node_child(node), (inc + 1), text);
            node = myhtml_node_next(node);
        } else{
            node = myhtml_node_next(node);
        }

    }
}

string parse_html(string html_s){

    int size = html_s.length();
    char html[size];
    strcpy(html,html_s.c_str());

    string parsed_text;

    // basic init
    myhtml_t* myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);


    // first tree init
    myhtml_tree_t* tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);

    // parse html
    myhtml_parse(tree, MyENCODING_UTF_8, html, strlen(html));

//    myhtml_tree_node * body =myhtml_get_nodes_by_tag_id(tree,NULL, MyHTML_TAG_BODY,NULL)->list[0];

    myhtml_tree_node_t *node = myhtml_tree_get_document(tree);

    traverse_tree(tree, myhtml_node_child(node), 0, parsed_text);

    // release resources
    //mycore_string_raw_destroy(&str, false);
    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);

    return parsed_text;
}


