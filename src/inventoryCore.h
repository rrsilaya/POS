#include "ctype.h"

void addItem(ITEM **itemRoster, int *itemCount) { // adds item to the inventory
  ITEM *newItem = NULL, *itemMatch = NULL;
  PRICE *newPrice = NULL, *tail = NULL;

  makeItem(&newItem); // make new node
  makePrice(&newPrice);
  newItem -> itemPrice = newPrice; // connect the price node to the item node

  choiceMenu(3, "Please choose item category.", "Food\nDrinks\nDessert\n", &(newItem -> category));

  itemScreen("ADD ITEM", newItem);
  printf("Enter Item Code (4 chars max): ");
  scanf("%s", &(newItem -> itemCode));
  getchar();
  animateOut("ADD ITEM", 8 + newItem -> priceCount);
  findByItemCode(itemRoster, newItem -> itemCode, &itemMatch); // finds the item by code entered by user
  if (itemMatch != NULL) { // if only adding new sizes
    newItem -> itemPrice = NULL;
    free(newItem); // frees dynamically created node
    newItem = NULL;

    tail = itemMatch -> itemPrice; // finds tail of prices
    while (tail -> next != NULL) tail = tail -> next;
    tail -> next = newPrice; // attaches new price to tail
    (itemMatch -> priceCount)++; // increments price count

    askPrice(&itemMatch, &(newPrice));
    popmsg(0, "Successfully added item size.");
  } else { // if user adds entirely new product
    itemScreen("ADD ITEM", newItem);
    printf("Enter Item Description: ");
    scanf("%[^\n]s", &(newItem -> itemDescription));
    getchar();
    animateOut("ADD ITEM", 8 + newItem -> priceCount);
    (newItem -> priceCount)++;

    askPrice(&newItem, &(newItem -> itemPrice));
    positionItem(itemRoster, &newItem);

    popmsg(0, "Successfully added item.");
    (*itemCount)++;
  }
}

void editItem(ITEM **itemRoster) { // edit items
  int category, i;
  char editCode[5];
  ITEM *match = NULL;
  PRICE *reader = NULL;

  if (!isEmptyList(*itemRoster)) { // checks if the roster is empty
    choiceMenu(3, "Please choose item category.", "Food\nDrink\nDessert\n", &category);

    if (!isEmptyCategory(*itemRoster, category)) { // checks if category is empty
      while (1) {
        displayByCategory(itemRoster, category); // displays all items uner the category
        printf("Enter Item Code to Edit: ");
        scanf("%s", editCode);
        getchar();

        findByItemCode(itemRoster, editCode, &match); // find the item code
        if (match == NULL) popmsg(1, "Item code does not exist.");
        else break;
      }

      for (i = 0, reader = match -> itemPrice; i < match -> priceCount; i++, reader = reader -> next) { // iterates through all item sizes
        askPrice(&match, &reader);
      }

      popmsg(0, "Successfully edited item.");
    }
  }
}

void delete(ITEM **itemRoster, int *itemCount) { // delete an item
  char itemSize[15], editCode[5];
  int category, confirm;
  ITEM *match = NULL;
  PRICE *priceDel = NULL;

  if (!isEmptyList(*itemRoster)) { // checks if roster is empty
    choiceMenu(3, "Please choose item category.", "Food\nDrink\nDessert\n", &category);

    if (!isEmptyCategory(*itemRoster, category)) { // checks if category is emtpy
      while (1) {
        displayByCategory(itemRoster, category);
        printf("Enter Item Code to Delete: ");
        scanf("%s", editCode);
        getchar();

        findByItemCode(itemRoster, editCode, &match);
        if (match == NULL) popmsg(1, "Item code does not exist.");
        else break;
      }

      while (1) {
        itemScreen("DELETE ITEM", match);
        printf("Enter Item Size to be Deleted: ");
        scanf("%[^\n]s", itemSize);
        getchar();
        animateOut("ADD ITEM", 8 + match -> priceCount);

        findByPrice(&(match -> itemPrice), itemSize, &priceDel); // find the price to be deleted
        if (priceDel == NULL) popmsg(1, "Item size does not exist.");
        else break;
      }

      choiceMenu(2, "Are you sure you want to delete this item?", "Delete\nCancel\n", &confirm); // deletion confirmation
      if (confirm == 1) {
        deletePrice(&(match -> itemPrice), itemSize); // delete price
        (match -> priceCount)--; // decrements from price count

        if (match -> priceCount == 0) { // if there is only one left and then deleted, deletes the item node
          deleteItem(itemRoster, editCode);
          (*itemCount)--;
        }
        popmsg(0, "Successfully deleted item.");
      } else popmsg(0, "Deleting item cancelled.");
    }
  }
}

void displayItems(ITEM *head) { // display items accd to user
  ITEM *finder = NULL;
  int choice;
  char enter, query[5];

  if (!isEmptyList(head)) { // if not empty roster
    choiceMenu(3, "Please choose which items to view.", "All\nCategory\nSearch\n", &choice);
    if (choice == 1) { // view all
      animateIn("VIEW ITEMS", defrow);
      headerTitle("VIEW ITEMS");
      spacing(1);
      viewAll(head);
      printBar(defcol);

      printf("Press enter to continue... ");
      scanf("%c", &enter);
    } else if (choice == 2) { // view by cateogry
      do {
        choiceMenu(3, "Please choose item category.", "Food\nDrink\nDessert\n", &choice);
      } while (isEmptyCategory(head, choice));

      animateIn("VIEW ITEMS", defrow);
      headerTitle("VIEW ITEMS");
      spacing(1);
      viewByCategory(head, choice);
      printBar(defcol);

      printf("Press enter to continue... ");
      scanf("%c", &enter);
    } else { // view by search
      animateIn("VIEW ITEMS", 6);
      spacing(8);
      headerTitle("VIEW ITEMS");
      spacing(1);
      margin(43, 0);
      printf(b "Search items in the inventory.\n" q);
      spacing(1);
      printBar(defcol);
      spacing(8);

      printf("Enter Item Code: ");
      scanf("%[^\n]s", query);
      getchar();

      findByItemCode(&head, query, &finder); // sets the pointer to the search node
      if (finder != NULL) {
        itemScreen("VIEW ITEMS", finder);

        printf("Press enter to continue... ");
        scanf("%c", &enter);
      } else popmsg(1, "Item not found.");
    }
  }
}

void computeSales() { // compute all sales screen
  char choice, filename[50];

  while (1) {
    animateIn("COMPUTE SALES", 6);
    spacing(7);
    headerTitle("COMPUTE SALES");
    spacing(1);
    margin(16, 0);
    printf(b "Sales Statistics\n" q);
    printf(y "Total Customers: " q "%i\n", receiptNumber());
    printf(y "Total Sales: " q "Php %.2f\n", totalSales());
    spacing(1);
    printBar(defcol);
    spacing(7);

    printf("Do you want to save as file? "); // if save file
    scanf("%c", &choice);
    getchar();

    if (tolower(choice) != 'y' && tolower(choice) != 'n') popmsg(1, "You have entered an invalid response.");
    else break;
  }

  if (tolower(choice) == 'y') {
    fileMenu("SAVE SALES", filename);
    writeSales(filename);
  }
}

void displayReceipts() { // display receipt given by user
  int receiptNum, items, i;
  FILE *receipt = NULL;
  char filename[25] = "Receipts/receipt-", buffer[3], enter;
  float total, received;

  ITEM *receiptItem = NULL;
  PRICE *receiptPrice = NULL;

  animateIn("DISPLAY RECEIPT", 6);
  spacing(8);
  headerTitle("DISPLAY RECEIPT");
  spacing(1);
  margin(54, 0);
  printf(b "Please enter the receipt number that you want to view.\n" q);
  spacing(1);
  printBar(defcol);
  spacing(8);

  printf("Enter Receipt Number: ");
  scanf("%i", &receiptNum);
  getchar();
  animateOut("DISPLAY RECEIPT", 6);

  if (receiptNumber() < receiptNum) popmsg(1, "Receipt does not exist."); // if receipt does not exist
  else {
    sprintf(buffer, "%i", receiptNum - 1);
    strcat(filename, buffer);
    strcat(filename, ".txt");

    animateIn("DISPLAY RECEIPT", defrow);
    headerTitle("DISPLAY RECEIPT");
    spacing(1);

    receipt = fopen(filename, "r");
    fscanf(receipt, "Items: %i\n", &items);
    fscanf(receipt, "Total: Php %f\n\n", &total);

    printf(r "RECEIPT #%i:\n" q, receiptNum);
    printf(y "Items: " q "%i\n", items);
    printf(y "Total: " q "Php %.2f\n\n", total);

    for (i = 0; i < items; i++) { // iterates through orders
      makeItem(&receiptItem);
      makePrice(&receiptPrice);
      receiptItem -> itemPrice = receiptPrice;

      fscanf(receipt, "Item Code: %s\n", &(receiptItem -> itemCode));
      fscanf(receipt, "Item Size: %s\n", &(receiptItem -> itemPrice -> itemSize));
      fscanf(receipt, "Qty: %i\n", &(receiptItem -> itemPrice -> qty));
      fscanf(receipt, "Total: Php %f\n\n", &(receiptItem -> itemPrice -> base));

      printf("[%s] (%s) x%i Php %.2f\n", receiptItem -> itemCode, receiptItem -> itemPrice -> itemSize, receiptItem -> itemPrice -> qty, receiptItem -> itemPrice -> base);
    }
    fscanf(receipt, "\nReceived Amt: Php %f\n", &received);
    printf("\nReceived Amt: Php %.2f\n", received);
    printf("Change: Php %.2f", received - total);

    fclose(receipt);

    // delets nodes used
    free(receiptItem -> itemPrice);
    receiptItem -> itemPrice = NULL;
    free(receiptItem);
    receiptItem = NULL;
    receiptPrice = NULL;

    spacing(1);
    printBar(defcol);
    printf("Press enter to continue... ");
    scanf("%c", &enter);
  }
}
