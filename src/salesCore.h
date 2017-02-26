void order(ITEM **itemRoster, ITEM **orderRoster, int *orderCount) { // make an order based on the items in the inventory
  int category, comboChoice;
  ITEM *newOrder = NULL, *finalOrder = NULL;
  PRICE *orderPrice = NULL, *totalOrder = NULL;

  choiceMenu(3, "Please choose item category.", "Food\nDrink\nDessert\n", &category);
  if (!isEmptyCategory(*itemRoster, category)) { // checks if category is empty
    makeItem(&finalOrder); // make item node
    while (1) {
      displayByCategory(itemRoster, category);
      printf("Enter Item to Order: ");
      scanf("%s", finalOrder -> itemCode);
      getchar();

      findByItemCode(itemRoster, finalOrder -> itemCode, &newOrder);
      if (newOrder == NULL) popmsg(1, "Item code does not exist.");
      else break;
    }

    strcpy(finalOrder -> itemDescription, newOrder -> itemDescription); // copies the description from the item inventory to orders

    makePrice(&totalOrder); // uses one node of price only
    finalOrder -> itemPrice = totalOrder;

    while (1) {
      itemScreen("ORDER", newOrder);
      printf("Enter Order Size: ");
      scanf("%[^\n]s", totalOrder -> itemSize);
      getchar();
      animateOut("ORDER", 8 + newOrder -> priceCount);

      findByPrice(&(newOrder-> itemPrice), totalOrder -> itemSize, &orderPrice);
      if (orderPrice == NULL) popmsg(1, "Item size does not exist.");
      else break;
    }

    strcpy(totalOrder -> itemSize, orderPrice -> itemSize);

    while (1) {
      itemScreen("ORDER", newOrder);
      printf("Enter Order Qty: ");
      scanf("%i", &(totalOrder -> qty));
      getchar();

      if (totalOrder -> qty > orderPrice -> qty) popmsg(1, "Not enough stock items in the inventory."); // sends error message if the entered value is greater than the inventory qty
      else break;
    }

    (orderPrice -> qty) -= (totalOrder -> qty);

    if (category == 1) { // food, combo is possible
      choiceMenu(3, "Please choose food combo.", "Drink\nDessert\nNo Combo\n", &comboChoice);

      if (comboChoice == 3) totalOrder -> base = orderPrice -> base;
      else totalOrder -> base = orderPrice -> combo;
      positionItem(orderRoster, &finalOrder);
      (*orderCount)++; // adds the order count

      if ((comboChoice == 1 || comboChoice == 2)) { // combo
        while(isEmptyCategory(*itemRoster, comboChoice + 1)) choiceMenu(3, "Please choose food combo.", "Drink\nDessert\nNo Combo\n", &comboChoice);
        makeItem(&finalOrder); // combos are stand alone node, but with combo price
        while (1) {
          displayByCategory(itemRoster, comboChoice + 1);
          printf("Enter Item to Order: ");
          scanf("%s", finalOrder -> itemCode);
          getchar();

          findByItemCode(itemRoster, finalOrder -> itemCode, &newOrder);
          if (newOrder == NULL) popmsg(1, "Item code does not exist.");
          else break;
        }

        strcpy(finalOrder -> itemDescription, newOrder -> itemDescription);

        makePrice(&totalOrder);
        finalOrder -> itemPrice = totalOrder;

        while (1) {
          itemScreen("ORDER", newOrder);
          printf("Enter Order Size: ");
          scanf("%[^\n]s", totalOrder -> itemSize);
          getchar();
          animateOut("ORDER", 8 + newOrder -> priceCount);

          findByPrice(&(newOrder-> itemPrice), totalOrder -> itemSize, &orderPrice);
          if (orderPrice == NULL) popmsg(1, "Item size does not exist.");
          else break;
        }

        strcpy(totalOrder -> itemSize, orderPrice -> itemSize);

        while (1) {
          itemScreen("ORDER", newOrder);
          printf("Enter Order Qty: ");
          scanf("%i", &(totalOrder -> qty));
          getchar();

          if (totalOrder -> qty > orderPrice -> qty) popmsg(1, "Not enough stock items in the inventory.");
          else break;
        }

        (orderPrice -> qty) -= (totalOrder -> qty);

        totalOrder -> base = orderPrice -> base;
        positionItem(orderRoster, &finalOrder);
        (*orderCount)++;
      }
    } else {
      totalOrder -> base = orderPrice -> base;
      positionItem(orderRoster, &finalOrder);
      (*orderCount)++;
    }

    popmsg(0, "Successfully added item order.");
  }
}

void voidItems(ITEM *itemRoster, ITEM **orderRoster, char password[]) { // deletes ordered items
  char itemCode[5], enter;
  ITEM *finder = NULL, *itemFinder;
  PRICE *priceFinder = NULL;
  int choice;

  if (!isEmptyList(*orderRoster) && enterPassword(password)) {
    animateIn("VOID ITEM", defrow);
    headerTitle("VOID ITEM");
    spacing(1);
    viewAll(*orderRoster); // displays orders
    spacing(1);
    printBar(defcol);
    printf("Press enter to proceed... ");
    scanf("%c", &enter);
    animateOut("VOID ITEM", defrow);

    animateIn("VOID ITEM", 6);
    spacing(8);
    headerTitle("VOID ITEM");
    spacing(1);
    margin(43, 0);
    printf(b "Please enter the item code of the item to void.\n" q);
    spacing(1);
    printBar(defcol);
    spacing(8);

    printf("Enter Item Code: ");
    scanf("%[^\n]s", itemCode);
    getchar();

    findByItemCode(orderRoster, itemCode, &finder);
    if (finder == NULL) popmsg(1, "Item not found.");
    else {
      choiceMenu(2, "Are you sure you want to void this order?", "Void Order\nCancel\n", &choice);
      if (choice == 1) {
        findByItemCode(&itemRoster, itemCode, &itemFinder);
        findByPrice(&(itemFinder -> itemPrice), finder -> itemPrice -> itemSize, &priceFinder);

        priceFinder -> qty += finder -> itemPrice -> qty;

        free(finder -> itemPrice); // deletes prices
        finder -> itemPrice = NULL;
        deleteItem(orderRoster, itemCode);
        popmsg(0, "Item successfully removed from orders.");
      }
    }
  }
}

void checkOut(ITEM **itemRoster, ITEM **orderRoster, int orderCount, int itemCount, char *filename) { // checks out
  char enter;
  float totalBill, orderBill;

  if (!isEmptyList(*orderRoster)) {
    totalBill = computeTotal(*orderRoster, orderCount); // total bill of the customer

    animateIn("ORDER REVIEW", defrow);
    headerTitle("ORDER REVIEW");
    spacing(1);
    viewAllOrders(*orderRoster); // displays order items
    spacing(1);
    printBar(defcol);
    printf("Press enter to proceed... ");
    scanf("%c", &enter);
    animateOut("ORDER REVIEW", defrow);

    while (1) {
      animateIn("CHECKOUT", 8);
      spacing(6);
      headerTitle("CHECKOUT");
      spacing(1);
      margin(43, 0);
      printf(b "Please enter customer bill to continue.\n" q);
      printf(y "Total Items: " q "%i\n", orderCount);
      printf(y "Total Bill: " q "Php %.2f\n", totalBill);
      spacing(1);
      printBar(defcol);
      spacing(6);

      printf("Enter Customer Bill: ");
      scanf("%f", &orderBill);
      getchar();

      if (orderBill < totalBill) popmsg(1, "Customer bill is less than the total order bill."); // errors if the customer bill is less than what's needed
      else break;
    }

    animateIn("CHECKOUT", 7);
    spacing(7);
    headerTitle("CHECKOUT");
    spacing(1);
    margin(52, 0);
    printf(b "Thank you for purchasing in " r "Jolly" y "Do" b "! Enjoy your meal!\n");
    printf(y "Change: " q "Php %.2f\n", orderBill - totalBill); // outputs change
    spacing(1);
    printBar(defcol);
    spacing(7);

    writeReceipt(orderRoster, orderCount, orderBill, totalBill); // writes receipt to Receipts/

    printf("Press enter to continue... ");
    scanf("%c", &enter);

    writeInventory(*itemRoster, itemCount, filename); // decrements orders to the inventory
    clean(orderRoster); // resets order roster
    initItemNodes(orderRoster);
  }
}
