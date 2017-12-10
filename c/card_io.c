#include <stdlib.h>

#include "card_io.h"

card read_card(char *card_str) {
    card c = -1;
    switch(card_str[0]) {
        case 'S': c = 0; break;
        case 'C': c = 13; break;
        case 'H': c = 26; break;
        case 'D': c = 39; break;
    }
    switch(card_str[1]) {
        case 'A': c += 0; break;
        case 'T': c += 9; break;
        case 'J': c += 10; break;
        case 'Q': c += 11; break;
        case 'K': c += 12; break;
    }
    if (card_str[1] >= '2' && card_str[1] <= '9') {
        c += card_str[1] - '1';
    }
    if (card_str[2] == 'f') {
        c += 52;
    }
    return c;
}

char *write_card(card c) {
    char *card_str = (char*) malloc(sizeof(char)*4);
    card_str[3] = 0;
    card_str[2] = (c/52 == 1) ? 'f' : '_';
    c = c%52;
    switch(c%13) {
        case  0: card_str[1] = 'A'; break;
        case  9: card_str[1] = 'T'; break;
        case 10: card_str[1] = 'J'; break;
        case 11: card_str[1] = 'Q'; break;
        case 12: card_str[1] = 'K'; break;
    }
    if (c%13 >= 1 && c%13 <= 8) {
        card_str[1] = '1' + c%13;
    }
    switch(c/13) {
        case 0: card_str[0] = 'S'; break;
        case 1: card_str[0] = 'C'; break;
        case 2: card_str[0] = 'H'; break;
        case 3: card_str[0] = 'D'; break;
    }
    return card_str;
}

