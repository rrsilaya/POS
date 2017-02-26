void makeItem(ITEM **newItem) { // make item node
  *newItem = (ITEM*) malloc(sizeof(ITEM));

  // resets all pointers
  (*newItem) -> next = NULL;
  (*newItem) -> prev = NULL;
  (*newItem) -> itemPrice = NULL;
  (*newItem) -> priceCount = 0;
  strcpy((*newItem) -> itemCode, "\0");
  strcpy((*newItem) -> itemDescription, "\0");
}

void initItemNodes(ITEM **itemRoster) { // initializes dummy nodes
  ITEM *head = NULL, *tail = NULL;

  makeItem(&head);
  makeItem(&tail);

  *itemRoster = head;
  head -> next = tail;
  tail -> prev = head;
}

void positionItem(ITEM **head, ITEM **newItem) { // positions item in the list
  ITEM *before = *head, *after = (*head) -> next;

  while (after -> next != NULL && (*newItem) -> category >= after -> category && strcmp((*newItem) -> itemCode, after -> itemCode) > 0) {
    before = before -> next;
    after = after -> next;
  } // chronologically

  before -> next = *newItem;
  (*newItem) -> prev = before;
  after -> prev = *newItem;
  (*newItem) -> next = after;
}

void deleteItem(ITEM **roster, char itemCode[5]) { // assumption is that item -> itemPrice == NULL
  ITEM *stalker = *roster, *mark = (*roster) -> next;

  while (strcmp(mark -> itemCode, itemCode)) { // itemCode found
    stalker = stalker -> next;
    mark = mark -> next;
  }

  stalker -> next = mark -> next; // reconnect pointers
  stalker -> next -> prev = stalker;
  mark -> next = NULL; // isolate node
  mark -> prev = NULL;
  free(mark);
}

void viewByCategory(ITEM *head, int category) { // view depending on the category entered by the user
  ITEM *reader = NULL;

  if (!isEmptyCategory(head, category)) { // checks if category is empty
    for (reader = head -> next; reader -> next != NULL; reader = reader -> next) {
      if (reader -> category == category) {
        viewItem(reader);
        printf("\n\n");
      }
    }
  }
}

void viewAll(ITEM *head) { // view all items
  ITEM *reader = NULL;

  for (reader = head -> next; reader -> next != NULL; reader = reader -> next) {
    viewItem(reader);
    printf("\n\n");
  }
}

void viewAllOrders(ITEM *head) { // view all orders (with prices)
  ITEM *reader = NULL;

  for (reader = head -> next; reader -> next != NULL; reader = reader -> next) {
    viewItem(reader);
    printf(b "(x%i) " q "Php %.2f", reader -> itemPrice -> qty, reader -> itemPrice -> qty * reader -> itemPrice -> base);
    printf("\n\n");
  }
}

float computeTotal(ITEM *head, int orderCount) { // returns total order
  ITEM *reader = NULL;
  float totalPrice = 0;
  int i;

  for (i = 0, reader = head -> next; i < orderCount; i++, reader = reader -> next) { // iterates through orders
    totalPrice += ((reader -> itemPrice -> qty) * (reader -> itemPrice -> base));
  }

  return totalPrice;
}

void clean(ITEM **roster) { // deletes all nodes in the list
  while (*roster != NULL) {
    if ((*roster) -> next == NULL) { // only one item
      free(*roster);
      *roster= NULL;
    } else {
      *roster = (*roster) -> next;
      free((*roster) -> prev);
      (*roster) -> prev = NULL;
    }
  }
}
