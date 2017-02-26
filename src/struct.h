typedef struct pricetag { // price structure
  char itemSize[15];
  float base, combo, upgrade;
  int qty;
  struct pricetag *next;
} PRICE;

typedef struct itemtag { // item structure
  struct itemtag *prev, *next;
  char itemCode[5], itemDescription[40];
  int category, priceCount;
  PRICE *itemPrice;
} ITEM;
