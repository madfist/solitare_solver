#include <stdio.h>
#include <stdlib.h>

#include "card.h"
#include "card_io.h"

int main(int argc, char **argv) {
    printf("Solitare Solver\n");
    card c = read_card("D7f");
    printf("C %u\n", c);
    char *c_str = write_card(c);
    printf("c %s\n", c_str);
    free(c_str);
    return 0;
}
