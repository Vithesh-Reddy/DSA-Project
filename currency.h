#ifndef CURRENCY_H
#define CURRENCY_H

typedef long int length; 
typedef long int weight;

typedef struct trade_bank trade_bank;
typedef trade_bank *ptr_trade_bank;

typedef struct hash_table_trade_banks hash_table_trade_banks;
typedef hash_table_trade_banks *graph;

typedef struct currency currency;
typedef currency *ptr_currency;

// NOTE - We are using separate chaining in our hash tables to resolve collisions (if any)

// This struct contains the hash table to store the trade banks 
struct hash_table_trade_banks
{
  length table_size; //maximum size of the hash table
  ptr_trade_bank *list_trade_banks; //pointer to the hash table array
  length *check; // array to store the filled indices of the hash table of trade banks
  length last_index; // size of the check array
};

// This struct contains the hash table to store the currencies in a particular trade bank 
// This struct contains the parameters of each trade bank
struct trade_bank
{
  length table_size; //maximum size of the hash table
  char name_trade_bank[100]; //to store the name of trade bank
  length nvertices; //to store the nunmber of currencies in the trade bank
  ptr_currency *list_currency; //pointer to the hash table array
  length *check; // array to store the filled indices of the hash table of currencies
  length last_index; // size of the check array
  ptr_trade_bank next; // pointer to the next trade bank having the same hash value, in case of collisions
};

// This struct contains the parameters of each currency
struct currency
{
  char id[100]; // to store the name of currency
  length vertex_id; // to store the vertex id of the currency in the trade bank 
  ptr_currency adj_list; // pointer to the adjaceny list of the currency
                         // adjacency list contains the outgoing edges from the currency
  ptr_currency next; // pointer to the next currency having the same hash value, in case of collisions
  weight dest_wt; // to store the outgoing edge weight
};

typedef struct SPtable SPtable;
typedef SPtable *ptr_sp_table;

//This struct is for dijkstra implementation and printing the path
// Each currency has an SPtable associated with it
struct SPtable
{
  char currency_name[100]; // to store the name of currency
  length dv; // to store the shortest distance from source
  length prev_vertex; // to store the previous vertex in the shortest path
  int known; // to mark if a currency is visites or not
};

typedef struct subsets subsets;

// This struct is used to detect cycles
struct subsets
{
  length king; // Used to detect cycles
  length rank; // Used to detect cycles
  length position; // helper variable for printing the cycle
};

// These are the functions whose detailed procedure and implementation is provided in <currency.c>
graph initialize_graph(length table_size);
length compute_hash(length table_size, char *name);
void add_trade_bank(graph g, char *bank_name);
void delete_trade_bank(graph g, char *bank_name);
void add_currency(graph g, char *bank_name, char *currency_name);
void delete_currency(graph g, char *bank_name, char *currency_name);
void add_conversion(graph g, char *bank_name, char *currency_name_s, char *currency_name_d, weight wt);
void delete_conversion(graph g, char *bank_name, char *currency_name_s, char *currency_name_d);
void best_path(graph g, char *currency_name_s, char *currency_name_d);
void second_best_path(graph g, char *src, char *dest);
void print_graph(graph g);
void print_trade_bank(graph g, char *bank);
ptr_sp_table best_path_TB(graph g, ptr_trade_bank tb, char *src, char *dest);
ptr_sp_table best_path_TB2(graph g, ptr_trade_bank tb, char *src, char *dest);
ptr_trade_bank find_trade_bank(graph g, char *bank_name);
ptr_currency find_currency(graph g, ptr_trade_bank tb, char *currency_name);
void print_path(ptr_sp_table path, length vertex_id);
length find(subsets A[], length index);
void Union(subsets A[], length src, length dest);
void Find_cycle(graph g, char *bank_name);
void print_stack(char stack[][50], length nfilled, length start);
length DFS(graph g, ptr_trade_bank bank, ptr_currency currency_src, length pVisited[], char stack[][50], subsets A[], length nfilled);

#endif
