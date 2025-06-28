#include "../hdr/data_struct.h"


Node* create_node(const char *line, int line_number) {
    Node *new_node = malloc(sizeof(Node));
    /* check if allocated properly */
    if (!new_node) {
        return NULL;
    }

    new_node->line = strdup(line);  /* copy line */
    new_node->line_number = line_number;
    new_node->next = NULL;
    return new_node;
}


void delete_node(Node *node) {
    free(node->line);
    free(node);
}


Node* find_node() {

}


void insert_node() {

}


void free_list(Node *head) {
    while (head) {
        Node *temp = head;
        head = head->next;
        delete_node(temp);
    }
}
