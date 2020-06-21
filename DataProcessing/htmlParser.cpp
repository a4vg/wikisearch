#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <myhtml/api.h>
#include <fstream>

using namespace std;


void print_tree(myhtml_tree_t* tree, myhtml_tree_node_t *node, size_t inc)
{
    while (node)
    {

        const char *tag_name = myhtml_tag_name_by_id(tree, myhtml_node_tag_id(node), NULL);

        string name(tag_name);

        myhtml_tag_id_t tag_id = myhtml_node_tag_id(node);


        if(tag_id == MyHTML_TAG__TEXT || tag_id == MyHTML_TAG__COMMENT  ) {
                const char* node_text = myhtml_node_text(node, NULL);
                printf("%s", node_text);
        }

        if(name != "style" and name != "annotation"){
            print_tree(tree, myhtml_node_child(node), (inc + 1));
            node = myhtml_node_next(node);
        } else{
            node = myhtml_node_next(node);
        }

    }
}


int main(int argc, const char * argv[])
{
    //read the article html
    string buffer ;
    ifstream f("articulo.html");
    string linea{};
    while (getline(f,linea)){
        buffer.append(linea);
    }
    int size = buffer.length();
    char html[size];
    strcpy(html,buffer.c_str());


    // basic init
    myhtml_t* myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);

    // first tree init
    myhtml_tree_t* tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);


    // parse html
    myhtml_parse(tree, MyENCODING_UTF_8, html, strlen(html));

    myhtml_tree_node * body =myhtml_get_nodes_by_tag_id(tree,NULL, MyHTML_TAG_BODY,NULL)->list[0];

    //myhtml_tree_node_t *node = myhtml_tree_get_document(body);

    print_tree(tree, myhtml_node_child(body), 0);


    // release resources
    mycore_string_raw_destroy(&str, false);
    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);

    return 0;
}