int isEmptyList(ITEM *head) { // checks if a list is empty
  if (head -> next -> next == NULL) {
    popmsg(1, "No items stored in the roster yet.");
    return 1;
  } else return 0;
}

int isEmptyCategory(ITEM *head, int category) { // checks if category is empty
  ITEM *reader = head;

  while (reader != NULL && reader -> category != category) reader = reader -> next; // iterates until the end or encounters the same category

  if (reader == NULL) {
    popmsg(1, "No items in the selected category yet.");
    return 1;
  } else return 0;
}

void findByItemCode(ITEM **head, char itemCode[5], ITEM **finder) { // searches itemCode
  *finder = (*head) -> next;

  while (*finder != NULL && strcmp((*finder) -> itemCode, itemCode)) *finder = (*finder) -> next; //iterates until itemCode is found
}

void findByPrice(PRICE **head, char itemSize[15], PRICE **finder) { // finds using the item size
  *finder = *head;

  while (*finder != NULL && strcmp((*finder) -> itemSize, itemSize)) *finder = (*finder) -> next; // iterates until size is found
}
