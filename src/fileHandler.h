#include <dirent.h> // for directory checking
#include <time.h> // for time printing in report

void writeInventory(ITEM *itemRoster, int itemCount, char *filename) { // saves inventory
  FILE *writeHandler = NULL;
  int i, j;
  ITEM *reader = itemRoster -> next;
  PRICE *priceRead = NULL;

  writeHandler = fopen(filename, "w");
  fprintf(writeHandler, "%i\n", itemCount); // prints item count
  for (i = 0; i < itemCount; i++, reader = reader -> next) {
    fprintf(writeHandler, "%i%%%s%%%s%%%i%%",
            reader -> category,
            reader -> itemCode,
            reader -> itemDescription,
            reader -> priceCount);

    for (j = 0, priceRead = reader -> itemPrice; j < reader -> priceCount; j++, priceRead = priceRead -> next) {
      fprintf(writeHandler, "%s%%%.2f%%%.2f%%%.2f%%%i%%",
              priceRead -> itemSize,
              priceRead -> base,
              priceRead -> combo,
              priceRead -> upgrade,
              priceRead -> qty);
    }

    fprintf(writeHandler, "\n");
  }
  fclose(writeHandler);
}

void getInventoryWrite(ITEM *itemRoster, int itemCount, char *filename) { // save inventory screen
  if (!isEmptyList(itemRoster)) { // if the roster is not empty
    fileMenu("SAVE INVENTORY", filename);
    writeInventory(itemRoster, itemCount, filename);
    popmsg(0, "Inventory successfully saved to file.");
  }
}

int fileExist(char *filename) { // checks if the file exists
  FILE *test = fopen(filename, "r"); // test open

  if (test != NULL) { // file exists
    fclose(test);
    return 1;
  } else {
    popmsg(1, "File entered not found.");
    return 0;
  }
}

void loadInventory(ITEM **roster, int *itemCount, char *filename) { // loads inventory
  FILE *fileHandler = NULL;
  char line[100], *token = NULL;
  int i, j;

  ITEM *newItem = NULL;
  PRICE *newPrice = NULL, *tail = NULL;

  if (fileExist(filename)) { // checks if file exists
    clean(roster); // cleans roster
    initItemNodes(roster);

    fileHandler = fopen(filename, "r");
    fscanf(fileHandler, "%i\n", itemCount); // item count

    for (i = 0; i < *itemCount; i++) { // iterates through all items
      makeItem(&newItem); // makes new node
      fgets(line, 100, fileHandler);

      token = strtok(line, "%%");
      newItem -> category = atoi(token);

      token = strtok(NULL, "%%");
      strcpy(newItem -> itemCode, token);

      token = strtok(NULL, "%%");
      strcpy(newItem -> itemDescription, token);

      token = strtok(NULL, "%%");
      newItem -> priceCount = atoi(token);

      for (j = 0; j < newItem -> priceCount; j++) { // iterates through item prices and sizes
        makePrice(&newPrice); // makes new node
        token = strtok(NULL, "%%");

        if (j == 0) { // first item
          newItem -> itemPrice = newPrice;
          strcpy(newItem -> itemPrice -> itemSize, token);
          token = strtok(NULL, "%%");

          newItem -> itemPrice -> base = atof(token);
          token = strtok(NULL, "%%");

          newItem -> itemPrice -> combo = atof(token);
          token = strtok(NULL, "%%");

          newItem -> itemPrice -> upgrade = atof(token);
          token = strtok(NULL, "%%");

          newItem -> itemPrice -> qty = atoi(token);
          //token = strtok(NULL, "%%");
        } else {
          tail = newItem -> itemPrice;
          while (tail -> next != NULL) tail = tail -> next;
          tail -> next = newPrice;

          strcpy(newPrice -> itemSize, token);
          token = strtok(NULL, "%%");

          newPrice -> base = atof(token);
          token = strtok(NULL, "%%");

          newPrice -> combo = atof(token);
          token = strtok(NULL, "%%");

          newPrice -> upgrade = atof(token);
          token = strtok(NULL, "%%");

          newPrice -> qty = atoi(token);
          //token = strtok(NULL, "%%");
        }
      }
      positionItem(roster, &newItem); // positions node to the list
    }
    fclose(fileHandler);
  }
}

void getInventoryLoad(ITEM **roster, int *itemCount, char *filename) { // load inventory screen
  fileMenu("LOAD INVENTORY", filename);
  loadInventory(roster, itemCount, filename);
  if ((*roster) -> next -> next != NULL) popmsg(0, "Successfully loaded inventory file.");
}

void chkDir() { // checks if Receipt/ directory is existing
  DIR *receipts = opendir("Receipts");

  if (receipts) closedir(receipts);
  else system("mkdir Receipts");
}

int receiptNumber() { // iterates through files, returns the last receiptnumber
  int i = 0;
  char filename[25], buffer[3];
  FILE *opener = NULL;

  do {
    strcpy(filename, "Receipts/receipt-");
    sprintf(buffer, "%i", i);
    strcat(filename, buffer);
    strcat(filename, ".txt");

    opener = fopen(filename, "r"); // test open file
    if (opener != NULL) {
      fclose(opener);
      i++;
    }
  } while (opener != NULL);

  return i;
}

void writeReceipt(ITEM **orderHead, int orderCount, float userMoney, float total) { // saves recipt
  char filename[25] = "Receipts/receipt-", buffer[3];
  int i;
  FILE *writer = NULL;
  ITEM *reader = (*orderHead) -> next;

  sprintf(buffer, "%i", receiptNumber()); // latest receipt file name and count
  strcat(filename, buffer);
  strcat(filename, ".txt");

  writer = fopen(filename, "w");
  if (writer != NULL) { // if file I/O is possible
    fprintf(writer, "Items: %i\n", orderCount);
    fprintf(writer, "Total: Php %.2f\n\n", total);
    for (i = 0; i < orderCount; i++) {
      fprintf(writer, "Item Code: %s\nItem Size: %s\nQty: %i\nTotal: Php%.2f\n\n", reader -> itemCode, reader -> itemPrice -> itemSize, reader -> itemPrice -> qty, reader -> itemPrice -> base * reader -> itemPrice -> qty);
      reader = reader -> next;
    }
    fprintf(writer, "\n");
    fprintf(writer, "Received Amt: Php %.2f\n", userMoney);
    fprintf(writer, "Change: Php %.2f\n", userMoney - total);
    fclose(writer);
  } else printf(r "\n[-] Error in file I/O.\n" q);
}

float totalSales() { // computes total sales using existing receipts
  int i, foo;
  float grandTotal = 0, price;
  char filename[25], buffer[3];
  FILE *reader = NULL;

  for (i = 0; i < receiptNumber(); i++) { // iterates through receipts
    strcpy(filename, "Receipts/receipt-");
    sprintf(buffer, "%i", i);
    strcat(filename, buffer);
    strcat(filename, ".txt");

    reader = fopen(filename, "r");
    fscanf(reader, "Items: %*i\nTotal: Php %f\n\n", &price);
    grandTotal += price;
  }

  if (reader != NULL) fclose(reader);

  return grandTotal;
}

void writeSales(char *filename) { // save the sales report to file
  // declarations for current time
  time_t t = time(NULL); // time
  struct tm tm = *localtime(&t);

  FILE *opener = NULL;

  opener = fopen(filename, "w");
  fprintf(opener, "Sales @ %d/%d/%d %d:%d\n\n", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900, tm.tm_hour, tm.tm_min); // writes time to file
  fprintf(opener, "Total Customers: %i\n", receiptNumber());
  fprintf(opener, "Total Sales: Php %.2f\n", totalSales());
  fclose(opener);
}

int loadSys(char *password) { // load system file sys.conf
  FILE *loader = NULL;
  int key;

  loader = fopen("sys.conf", "r");
  if (loader == NULL) {
    popmsg(1, "System file needed is deleted. Please contact your system administrator.");
    return 0;
  } else { // sets the decrypted to the saved password
    fscanf(loader, "%i\n", &key);
    fgets(password, 30, loader);
    password[strlen(password) - 1] = '\0';

    decrypt(key, password);
  }
}
