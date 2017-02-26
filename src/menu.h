#include "ui.h"
#include "string.h"

void inventoryMenu(int *menuChoice) { // inventory menu
  while (1) {
    animateIn("INVENTORY", defrow);
    headerTitle("INVENTORY");
    formattingDef(10, 1);

    printf(c "Please choose what you want to do.\n" q);
    printf(y "[1] " q "Add Item\n");
    printf(y "[2] " q "Edit Item\n");
    printf(y "[3] " q "Delete Item\n");
    printf(y "[4] " q "Display Items\n");
    printf(y "[5] " q "Display Receipts\n");
    printf(y "[6] " q "Compute Sales\n");
    printf(m "[7] " q "Load File\n");
    printf(m "[8] " q "Save File\n");
    printf(m "[9] " q "Exit to Main Menu\n");

    formattingDef(10, 0);
    printf("Enter Choice: ");
    scanf("%i", menuChoice);
    getchar();
    animateOut("MENU", defrow);

    if (*menuChoice < 1 || *menuChoice > 9) popmsg(1, "You entered an invalid response.");
    else break;
  }
}

void choiceMenu(int choiceCount, char question[25], char choices[100], int *menuChoice) { // screen for menu choices
  char *token = NULL, choiceList[100];
  int i;

  while (1) {
    strcpy(choiceList, choices);
    animateIn("CHOICE", 6 + choiceCount);
    spacing(8 - (choiceCount / 2));
    headerTitle("CHOICE");
    spacing(1);
    margin(strlen(question), 0);
    printf(b "%s\n" q, question);

    token = strtok(choiceList, "\n"); // separates choices
    for (i = 0; i < choiceCount; i++) {
      printf(y "[%i] " q "%s\n", i + 1, token);
      token = strtok(NULL, "\n");
    }

    spacing(1);
    printBar(defcol);
    spacing(8 - (choiceCount / 2));

    printf("Enter Choice: ");
    scanf("%i", menuChoice);
    getchar();

    if (*menuChoice < 1 || *menuChoice > choiceCount) popmsg(1, "You entered an invalid response.");
    else break;
  }
}

void itemScreen(char header[20], ITEM *item) { // prints items
  int i;
  PRICE *viewer = NULL;

  animateIn(header, 8 + item -> priceCount);
  spacing(8 - item -> priceCount);
  headerTitle(header);
  spacing(1);
  if (item -> category == 1) printf(r "FOOD\n" q);
  else if (item -> category == 2) printf(r "DRINK\n" q);
  else printf(r "DESSERT\n" q);

  printf(y "Item Code: " q "%s\n", item -> itemCode);
  printf(y "Description: " q "%s\n", item -> itemDescription);

  for (i = 0, viewer = item -> itemPrice; i < item -> priceCount; i++, viewer = viewer -> next) { // scrolls through items
    printf(m "[%s] " b "Solo: " q "Php %.2f " b "Combo: " q "Php %.2f " b "Upgrade: " q "Php %.2f " y "(x%i)\n" q,
           viewer -> itemSize, viewer -> base, viewer -> combo, viewer -> upgrade, viewer -> qty);
  }
  spacing(1);
  printBar(defcol);
  spacing(7 - item -> priceCount);
}

void displayByCategory(ITEM **head, int category) { // display by category screen
  int categoryCount = 0, i;
  char enter;
  ITEM *reader = (*head) -> next;

  while (reader -> next != NULL && reader -> category <= category) {
    if (reader -> category == category) categoryCount++;
    reader = reader -> next;
  }

  animateIn("VIEW ITEMS", categoryCount + 5);
  spacing((defrow - categoryCount - 6) / 2);
  headerTitle("VIEW ITEMS");
  spacing(1);
  printf("Inventory Items:\n");
  for (reader = (*head) -> next; reader -> next != NULL && reader -> category <= category; reader = reader -> next) {
    if (reader -> category == category) printf(y "[%s] " q "%s\n", reader -> itemCode, reader -> itemDescription);
  }
  spacing(1);
  printBar(defcol);
  spacing((defrow - categoryCount - 6) / 2);
}

void fileMenu(char header[20], char *filename) { // asks the filename from user
  animateIn(header, 6);
  spacing(8);
  headerTitle(header);
  spacing(1);
  margin(43, 0);
  printf(b "Please enter the filename of the inventory.\n" q);
  spacing(1);
  printBar(defcol);
  spacing(8);

  printf("Enter Filename: ");
  scanf("%[^\n]s", filename);
  getchar();
}

void salesMenu(int *menuChoice) { // sales menu
  while (1) {
    animateIn("SALES", defrow);
    headerTitle("SALES");
    formattingDef(6, 1);

    printf(c "Please choose what you want to do.\n" q);
    printf(y "[1] " q "Order\n");
    printf(y "[2] " q "Void\n");
    printf(m "[3] " q "Checkout\n");
    printf(m "[4] " q "Exit to Main Menu\n");

    formattingDef(5, 0);
    printf("Enter Choice: ");
    scanf("%i", menuChoice);
    getchar();
    animateOut("MENU", defrow);

    if (*menuChoice < 1 || *menuChoice > 4) popmsg(1, "You entered an invalid response.");
    else break;
  }
}

void viewItem(ITEM *reader) { // view a specific item node
  PRICE *viewer = reader -> itemPrice;
  int i;

  printf(y "[%s] " q "%s\n", reader -> itemCode, reader -> itemDescription);
  for (i = 0; i < reader -> priceCount; i++, viewer = viewer -> next) {
    printf(m "%s " b "Solo: " q "Php %.2f " b "Combo: " q "Php %.2f " b "Upgrade: " q "Php %.2f " y "(x%i)\n" q,
           viewer -> itemSize, viewer -> base, viewer -> combo, viewer -> upgrade, viewer -> qty);
  }
}

void splashMenu() { // splash menu
  animateIn("JOLLY DO", defrow);
  printBar(defcol);
  spacing(5);

  margin(66, 0);
  printf(y "   oooo           oooo  oooo              " r "oooooooooo.             \n" q); margin(66, 0);
  printf(y "   '888           '888  '888              " r "'888'   'Y8b            \n" q); margin(66, 0);
  printf(y "    888  .ooooo.   888   888  oooo    ooo " r " 888      888  .ooooo.  \n" q); margin(66, 0);
  printf(y "    888 d88' '88b  888   888   '88.  .8'  " r " 888      888 d88' '88b \n" q); margin(66, 0);
  printf(y "    888 888   888  888   888    '88..8'   " r " 888      888 888   888 \n" q); margin(66, 0);
  printf(y "    888 888   888  888   888     '888'    " r " 888     d88' 888   888 \n" q); margin(66, 0);
  printf(y ".o. 88P 'Y8bod8P' o888o o888o     .8'     " r "o888bood8P'   'Y8bod8P' \n" q); margin(66, 0);
  printf(y "'Y888P                        .o..P'      " r "                        \n" q); margin(66, 0);
  printf(y "                              'Y8P'       " r "                        \n" q);
  printf("\n");

  spacing(5);
  printBar(defcol);
  sleep(7);
  animateOut("JOLLY DO", defrow);
}

void credits() { // credits
  animateIn("DEVELOPER", defrow);
  headerTitle("DEVELOPER");
  spacing(2);

  margin(60, 0);
  printf(c " ,ggggggggggg,                                             \n"); margin(60, 0);
  printf("dP\"\"\"88\"\"\"\"\"\"Y8,              ,dPYb,             ,dPYb,    \n"); margin(60, 0);
  printf("Yb,  88      `8b              IP'`Yb             IP'`Yb    \n"); margin(60, 0);
  printf(" `\"  88      ,8P              I8  8I             I8  8I    \n"); margin(60, 0);
  printf("     88aaaad8P\"               I8  8'             I8  8'    \n"); margin(60, 0);
  printf("     88\"\"\"\"Yb,      ,gggg,gg  I8 dP  gg,gggg,    I8 dPgg,  \n"); margin(60, 0);
  printf("     88     \"8b    dP\"  \"Y8I  I8dP   I8P\"  \"Yb   I8dP\" \"8I \n"); margin(60, 0);
  printf("     88      `8i  i8'    ,8I  I8P    I8'    ,8i  I8P    I8 \n"); margin(60, 0);
  printf("     88       Yb,,d8,   ,d8b,,d8b,_ ,I8 _  ,d8' ,d8     I8,\n"); margin(60, 0);
  printf("     88        Y8P\"Y8888P\"`Y88P'\"Y88PI8 YY88888P88P     `Y8\n"); margin(60, 0);
  printf("                                     I8                    \n"); margin(60, 0);
  printf("                                     I8                    \n"); margin(60, 0);
  printf("                                     I8                    \n"); margin(60, 0);
  printf("                                     I8                    \n"); margin(60, 0);
  printf("                                     I8                    \n\n" q);

  margin(37, 0);
  printf(c "Github: " q "http://rrsilaya.github.io/\n");

  spacing(2);
  printBar(defcol);
  sleep(5);
  animateOut("DEVELOPER", defrow);
}
