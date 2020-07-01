#include <myhtml/api.h>
#include <vector>

#include "htmlParser.h"

void cleanWikipediaHtmlTree(myhtml_tree_t* tree, myhtml_tree_node_t *body)
{
    // Remove useless tags
    std::vector<std::string> tags_remove = { "style", "annotation", "math", "script" };
    for (std::string& tag: tags_remove)
    {
        myhtml_collection_t *nodes = myhtml_get_nodes_by_name_in_scope(tree, NULL, body, tag.c_str(), tag.length(), NULL);
        for (int i=0; nodes && i<nodes->length; ++i)
            myhtml_node_remove(nodes->list[i]);
    }

    // Remove elements by id or class
    std::string id_str = "id";
    std::string class_str = "class";
    std::vector<std::string> val_remove = { "referencias", "mw-content-text", "reflink"};

    // Bye references section
    myhtml_collection_t *reference_nodes = myhtml_get_nodes_by_attribute_value(tree, NULL, body, true, id_str.c_str(), id_str.length(), val_remove[0].c_str(), val_remove[0].length(), NULL);
    if (reference_nodes->length)
    {
        myhtml_tree_node *reference_section = myhtml_node_parent(myhtml_node_parent(reference_nodes->list[0]));
        if (myhtml_node_tag_id(reference_section) == MyHTML_TAG_DETAILS)
            myhtml_node_remove(reference_section);
    }

    // Bye footer (last div in content)
    myhtml_collection_t *content_nodes = myhtml_get_nodes_by_attribute_value(tree, NULL, NULL, true, id_str.c_str(), id_str.length(), val_remove[1].c_str(), val_remove[1].length(), NULL);
    myhtml_collection_t *divs = myhtml_get_nodes_by_tag_id_in_scope(tree, NULL, content_nodes->list[0], MyHTML_TAG_DIV, NULL);
    myhtml_node_remove(divs->list[divs->length-1]);

    // Bye references links (ex: [ref 2], [2])
    myhtml_collection_t *link_nodes = myhtml_get_nodes_by_attribute_value_contain(tree, NULL, NULL, true, class_str.c_str(), class_str.length(), val_remove[2].c_str(), val_remove[2].length(), NULL);
    for (int i=0; i<link_nodes->length; ++i)
        myhtml_node_remove(link_nodes->list[i]);
    
}

std::string getTextFromTree(myhtml_tree_t* tree, myhtml_tree_node_t *body, std::string &text)
{
    myhtml_collection_t *text_nodes = myhtml_get_nodes_by_tag_id_in_scope(tree, NULL, body, MyHTML_TAG__TEXT, NULL);
    for (int i=0; i<text_nodes->length; ++i)
    {
        text += myhtml_node_text(text_nodes->list[i], NULL);
        text += " ";
    }
    return text;
}

std::string parseWikipediaHTML(char const* html, int html_size)
{
    // basic init
    myhtml_t* myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0); // 1 = thread_count, 0 = queue_size

    // first tree init
    myhtml_tree_t* tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);
    myhtml_tree_parse_flags_set(tree, MyHTML_TREE_PARSE_FLAGS_SKIP_WHITESPACE_TOKEN);

    // parse tree
    myhtml_parse(tree, MyENCODING_UTF_8, html, html_size);

    // focus on body
    myhtml_tree_node * body_node = myhtml_tree_get_node_body(tree);
    cleanWikipediaHtmlTree(tree, body_node); // remove non important wikipedia html elements

    // get text
    std::string text = "";
    getTextFromTree(tree, body_node, text);

    // release resources
    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);

    return text;
}


