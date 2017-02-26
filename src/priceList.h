void makePrice(PRICE **newPrice) { // make price node
  *newPrice = (PRICE*) malloc(sizeof(PRICE));
  (*newPrice) -> next = NULL;

  strcpy((*newPrice) -> itemSize, "\0");
  (*newPrice) -> base = 0;
  (*newPrice) -> combo = 0;
  (*newPrice) -> upgrade = 0;
  (*newPrice) -> qty = 0;
}

void askPrice(ITEM **newItem, PRICE **itemPrice) { // asks price from user
  itemScreen("ADD ITEM", *newItem);
  printf("Enter Item Size: "); // item size
  scanf("%[^\n]s", &((*itemPrice) -> itemSize));
  getchar();
  animateOut("ADD ITEM", 8 + (*newItem) -> priceCount);

  itemScreen("ADD ITEM", *newItem);
  printf("Enter Item Base Price: ");
  scanf("%f", &((*itemPrice) -> base)); // solo price
  getchar();
  animateOut("ADD ITEM", 8 + (*newItem) -> priceCount);

  if ((*newItem) -> category == 1) { // if food item, asks for cmobo price
    itemScreen("ADD ITEM", *newItem);
    printf("Enter Item Combo Price: ");
    scanf("%f", &((*itemPrice) -> combo));
    getchar();
    animateOut("ADD ITEM", 8 + (*newItem) -> priceCount);
  }

  if ((*newItem) -> category != 3) { // if not dessert, asks for upgrade price
    itemScreen("ADD ITEM", *newItem);
    printf("Enter Item Upgrade Price: ");
    scanf("%f", &((*itemPrice) -> upgrade));
    getchar();
    animateOut("ADD ITEM", 8 + (*newItem) -> priceCount);
  }

  itemScreen("ADD ITEM", *newItem);
  printf("Enter Qty: ");
  scanf("%i", &((*itemPrice) -> qty)); // qty
  getchar();
  animateOut("ADD ITEM", 8 + (*newItem) -> priceCount);
}

void deletePrice(PRICE **roster, char itemSize[15]) { // deletes price
  PRICE *stalker = NULL, *mark = NULL;

  if ((*roster) -> next == NULL) { // if only one price
    free(*roster);
    *roster = NULL;
  } else {
    stalker = *roster;
    mark = (*roster) -> next;

    while (strcmp(mark -> itemSize, itemSize)) {
      stalker = stalker -> next;
      mark = mark -> next;
    }

    stalker -> next = NULL;
    free(mark);
  }
}
