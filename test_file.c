#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define CARD_ERROR (unsigned char) 0xFF;

unsigned char convert(int card, int suit);


int main(void) {

    unsigned char testhand[7]={0};

    // new test hand
    testhand[0] = convert(1,1);  // Ace of Hearts
    testhand[1] = convert(1,2);  // Ace of Diamonds
    testhand[2] = convert(1,3);  // Ace of Clubs
    testhand[3] = convert(1,4);  // Ace of Spades
    testhand[4] = convert(11,2); // Jack of Diamonds
    testhand[5] = convert(12,2); // Queen of Diamonds
    testhand[6] = convert(13,4); // King of Spades

    int test = testhand[0];
    int test1 = testhand[1];
    int test2 = testhand[2];
    int test3 = testhand[3];
    printf("%d\n", test); printf("%d\n", test1); printf("%d\n", test2); printf("%d\n", test3);

    return 0;
}

unsigned char convert(int card, int suit) {
    unsigned char character;

    // shift the bottom 4 bits of card to overlap with top 4 bits of 
    // the character. 
    // Then, bitwise OR it with the bottom 4 bits being bitwise ANDed with suit bits
    character = (0xF0 & (card << 4)) | (0x0F & suit);

    if ((suit > 4) || (suit < 1) || (card > 13) || (card < 1)) {
      return CARD_ERROR;
    } 

    return character;
}