#define VD_USE_CRT 1
#define VD_INCLUDE_TESTS 1
#include "vd.h"
#include "vd_docuspec.h"
#include <stdio.h>

VD_TEST("Basic Syntax - 1") {
    VdStr s = VD_LIT(R"STR(
@section("hello") {
    text {
        the text
    }
}
    )STR");
    VdDspcDocument doc;
    vd_dspc_document_init(&doc, 0);
    vd_dspc_document_add(&doc, s.s, s.len, 0);
    VdDspcTree *tree = vd_dspc_document_first_tree(&doc);
    VdDspcSection *section = vd_dspc_tree_first_section(tree)->first;

    VdDspcError *err = vd_dspc_document_first_error(&doc);
    while (vd_dspc_error_valid(err)) {
        char message[128];
        int message_size = vd_dspc_error_get_message(err, sizeof(message), message);

        fprintf(stderr, "%zu:%zu %.*s\n",
                        err->line, err->column,
                        message_size, message);

        err = vd_dspc_error_next(err);
    }

    VD_TEST_NONZERO("Section", section);

    VD_TEST_EQ("Section id is \"section\"", strncmp(section->section_id.s, "section", section->section_id.l), 0);

    int count = 0;
    VdDspcTag *tag = vd_dspc_section_first_tag(section);
    while (tag != 0) {
        count++;
        tag = vd_dspc_section_next_tag(section, tag); 
    }

    VD_TEST_EQ("Exactly one tag", count, 1);


    VD_TEST_OK();
}

VD_TEST("Inline Text Content") {
    VdStr s = VD_LIT("text {normal*bold*_italic__*bolditalic*_#link#} ");
    VdDspcDocument doc;
    vd_dspc_document_init(&doc, 0);
    vd_dspc_document_add(&doc, s.s, s.len, 0);
    VdDspcTree *tree = (VdDspcTree*)vd_dspc_document_first_tree(&doc);
    VdDspcSection *section = vd_dspc_tree_first_section(tree)->first;

    VdDspcStrNode *node = vd_dspc_str_list_first_node(&section->text_content);
    VD_TEST_EQ("1st node is normal (i.e. no flags)", node->flags, 0);
    VD_TEST_EQ("1st node is text is \'normal\'", strlen("normal"), node->len);
    VD_TEST_EQ("1st node is text is \'normal\'", strncmp(node->str, "normal", node->len), 0);

    VD_TEST_EQ("2nd node is bold", node->next->flags, VD_DSPC_STR_NODE_FLAGS_BOLD);
    VD_TEST_EQ("2nd node is text is \'bold\'", strlen("bold"), node->next->len);
    VD_TEST_EQ("2nd node is text is \'bold\'", strncmp(node->next->str, "bold", node->next->len), 0);

    VD_TEST_EQ("3rd node is italic", node->next->next->flags, VD_DSPC_STR_NODE_FLAGS_ITALIC);
    VD_TEST_EQ("3rd node is text is \'italic\'", strlen("italic"), node->next->next->len);
    VD_TEST_EQ("3rd node is text is \'italic\'", strncmp(node->next->next->str, "italic", node->next->next->len), 0);

    VD_TEST_EQ("4th node is bolditalic", node->next->next->next->flags, VD_DSPC_STR_NODE_FLAGS_BOLD | VD_DSPC_STR_NODE_FLAGS_ITALIC);
    VD_TEST_EQ("4th node is text is \'bolditalic\'", strlen("bolditalic"), node->next->next->next->len);
    VD_TEST_EQ("4th node is text is \'bolditalic\'", strncmp(node->next->next->next->str, "bolditalic", node->next->next->next->len), 0);

    VD_TEST_EQ("5th node is link", node->next->next->next->next->flags, VD_DSPC_STR_NODE_FLAGS_LINK);
    VD_TEST_EQ("5th node is text is \'link\'", strlen("link"), node->next->next->next->next->len);
    VD_TEST_EQ("5th node is text is \'link\'", strncmp(node->next->next->next->next->str, "link", node->next->next->next->next->len), 0);
    VD_TEST_OK();
}

VD_TEST("Inline Placements - 1 Character long") {
    VdStr s = VD_LIT("text {a*b*c} ");
    VdDspcDocument doc;
    vd_dspc_document_init(&doc, 0);
    vd_dspc_document_add(&doc, s.s, s.len, 0);
    VdDspcTree *tree = (VdDspcTree*)vd_dspc_document_first_tree(&doc);
    VdDspcSection *section = vd_dspc_tree_first_section(tree)->first;

    VdDspcStrNode *node = vd_dspc_str_list_first_node(&section->text_content);
    VD_TEST_EQ("a len", node->len, 1);
    VD_TEST_EQ("a str", strncmp(node->str, "a", node->len), 0);
    VD_TEST_EQ("a sty", node->flags, 0);

    VD_TEST_EQ("b len", node->next->len, 1);
    VD_TEST_EQ("b str", strncmp(node->next->str, "b", node->next->len), 0);
    VD_TEST_EQ("b sty", node->next->flags, VD_DSPC_STR_NODE_FLAGS_BOLD);

    VD_TEST_EQ("c len", node->next->next->len, 1);
    VD_TEST_EQ("c str", strncmp(node->next->next->str, "c", node->next->next->len), 0);
    VD_TEST_EQ("c sty", node->next->next->flags, 0);
    VD_TEST_OK();
}

VD_TEST("Inline Placements - Escapes") {
    VdStr s = VD_LIT("text {`\\#text with \\_ stuf`} ");
    VdDspcDocument doc;
    vd_dspc_document_init(&doc, 0);
    vd_dspc_document_add(&doc, s.s, s.len, 0);
    VdDspcTree *tree = (VdDspcTree*)vd_dspc_document_first_tree(&doc);
    VdDspcSection *section = vd_dspc_tree_first_section(tree)->first;

    VdDspcStrNode *node = vd_dspc_str_list_first_node(&section->text_content);

    VD_TEST_EQ("Single node flags", node->flags, VD_DSPC_STR_NODE_FLAGS_CODE);

    VD_TEST_OK();
}

VD_TEST("Multistring Inline Link") {
    VdStr s = VD_LIT("text {#my\nlink#<linkname>} ");
    VdDspcDocument doc;
    vd_dspc_document_init(&doc, 0);
    vd_dspc_document_add(&doc, s.s, s.len, 0);
    VdDspcTree *tree = (VdDspcTree*)vd_dspc_document_first_tree(&doc);
    VdDspcSection *section = vd_dspc_tree_first_section(tree)->first;

    VdDspcStrNode *node = vd_dspc_str_list_first_node(&section->text_content);

    VD_TEST_EQ("Part 1 of link is a link", node->flags, VD_DSPC_STR_NODE_FLAGS_SPACE | VD_DSPC_STR_NODE_FLAGS_LINK);
    VD_TEST_EQ("Part 2 of link is a link", node->next->flags, VD_DSPC_STR_NODE_FLAGS_LINK);

    VD_TEST_EQ("Part 1 of link is strlen(linkname)", node->link_str.l, 8);
    VD_TEST_EQ("Part 1 of link is linkname", strncmp(node->link_str.s, "linkname", node->link_str.l), 0);

    VD_TEST_EQ("Part 2 of link is strlen(linkname)", node->next->link_str.l, 8);
    VD_TEST_EQ("Part 2 of link is linkname", strncmp(node->next->link_str.s, "linkname", node->next->link_str.l), 0);
    VD_TEST_OK();
}

int main(int argc, char const *argv[])
{
    VD_UNUSED(argc);
    VD_UNUSED(argv);
    vd_test_main();
    return 0;
}

#define VD_IMPL
#include "vd.h"

#define VD_DSPC_IMPL
#include "vd_docuspec.h"
