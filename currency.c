#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include "currency.h"
#include "priority_queue.h"
#include "priority_queue.c"

#define hash_table_size 105337
#define check_size 1000005
#define infinite 100005

// creates an empty graph with the given table size
graph initialize_graph(length table_size)
{
    graph g = (graph)malloc(sizeof(hash_table_trade_banks));                             // allocates memory to the graph struct
    assert(g != NULL);                                                                   // throws an error if memory not available
    g->table_size = table_size;                                                          // assigns the given table size to the table size of graph
    g->list_trade_banks = (ptr_trade_bank *)malloc(table_size * sizeof(ptr_trade_bank)); // allocates memory to the hash table of trade banks
    assert(g->list_trade_banks != NULL);                                                 // throws an error if memory not available
    g->check = (length *)calloc(table_size, sizeof(length));                             // allocates memory to the check array
    assert(g->check != NULL);                                                            // throws an error if memory not available
    g->last_index = 0;                                                                   // initializes last index to 0
    return g;
}

// to compute string hash of bank name as well as currency name using horners rule
length compute_hash(length table_size, char *name)
{
    length p = 37;
    length hashv = 0;
    for (int i = 0; i < strlen(name); i++)
    {
        hashv = (hashv * p + name[i]);
    }
    return hashv % (table_size);
}

//This function is used to add a trade bank in the data structure
//this will be added in the tradebank hashtable in the index equal to the string hash of bank name
void add_trade_bank(graph g, char *bank_name)
{
    //computes the string hash of the bank name
    long int bank_hash = compute_hash(g->table_size, bank_name);
    //position will point to the seperate chaining of the banks which have the same hash value
    ptr_trade_bank position = g->list_trade_banks[bank_hash];
    //this while loop will traverse through the whole seperate chaining
    while (position != NULL)
    {
        //checks whether the given bank already exists
        if (strcmp(position->name_trade_bank, bank_name) == 0)
        {
            printf("Trade Bank already exists\n");
            return;
        }
        //traversal of separate chaining
        position = position->next;
    }
    //checks whether there is no bank available earlier with same hash value
    if (g->list_trade_banks[bank_hash] == NULL)
    {
        //updates the check array which stores the filled indices of the hashtable
        //adds the current hashvalue to the check array as the last element
        //as there is no bank stored with this hashvalue earlier
        g->check[g->last_index] = bank_hash;
        //increases the size of the check array as we added an extra element
        g->last_index++;
    }
    //allocation of space to a new trade bank to insert it
    ptr_trade_bank new = (ptr_trade_bank)malloc(sizeof(trade_bank));
    assert(new != NULL); // throws an error if memory not available
    //initalizes the hashtable size of the currencies in this bank
    new->table_size = hash_table_size;
    //nvertices denotes to number of currencies added
    //as there is no currency in the bank initially it's value is zero
    new->nvertices = 0;
    //sets the name of the bank in the tradebank struct to the given bankname
    strcpy(new->name_trade_bank, bank_name);
    //allocates memory to the hashtable of the currencies
    new->list_currency = (ptr_currency *)malloc(hash_table_size * sizeof(ptr_currency));
    assert(new->list_currency != NULL); // throws an error if memory not available
    //allocates memory to the check array which stores the filled indices of the currency hashtable in this bank
    new->check = (length *)calloc(new->table_size, sizeof(length));
    assert(new->check != NULL); // throws an error if memory not available
    //initiates the number of indices filled in the check array(last_index) to zero
    new->last_index = 0;
    //makes the next pointer of this bank to point to the bank where hashtable with same hashvalue points to
    new->next = g->list_trade_banks[bank_hash];
    //makes the hashvalue index in the hashtable to point to this new bank
    //this will insert this new bank at the start of the separate chaining
    g->list_trade_banks[bank_hash] = new;
}

//this function is used to delete a whole tradebank
//it also frees memory allocated to all currency nodes in the trade bank
void delete_trade_bank(graph g, char *bank_name)
{
    //compurtes the stringhash of bank name
    length hash_value = compute_hash(g->table_size, bank_name);
    //checks whether there is a separate chaining for the same hashvalue
    if (g->list_trade_banks[hash_value] == NULL)
    {

        printf("THERE IS NO TRADEBANK WITH NAME %s\n", bank_name);
        return;
    }

    else
    {
        //Trade_bank points to the seperate chaining in the hashvalue index
        ptr_trade_bank Trade_bank = g->list_trade_banks[hash_value];
        //prev_bank stores the bank stored at the front of our bank
        ptr_trade_bank prev_bank = Trade_bank;

        //traverses through the separate chaining
        while (Trade_bank != NULL)
        {
            //checks whether Trade_bank pointer contains the required bank
            if (strcmp(Trade_bank->name_trade_bank, bank_name) == 0)
            {
                //breaks when Trade_bank points to the desired bank
                //and prev_bank will point to the previous bank in the separate chaining
                break;
            }
            //updates the previous bank
            prev_bank = Trade_bank;
            //traversal through separate chaining
            Trade_bank = Trade_bank->next;
        }
        //checks if given tradebank does not exist
        if (Trade_bank == NULL)
        {

            printf("THERE IS NO TRADEBANK WITH NAME %s\n", bank_name);
            return;
        }
        //traverses through all the filled indices of the currency hashtable
        for (int i = 0; i < Trade_bank->last_index; i++)
        {
            //current points to the separate chain in the check[i] index of hashtable
            ptr_currency current = Trade_bank->list_currency[Trade_bank->check[i]];
            ptr_currency temp_next = current;
            //traverses through the separate chaining of currency hashtable
            while (temp_next != NULL)
            {
                //temp_adj points to adjacency list of the temp_next currency node
                ptr_currency temp_adj = temp_next->adj_list;
                //traverses through the adjacency list of temp_next currency node
                while (temp_adj != NULL)
                {
                    //stores the present node in the del_adj
                    ptr_currency del_adj = temp_adj;
                    //traversal through the adjacency list
                    temp_adj = temp_adj->adj_list;
                    //frees the memory allocated to a node in the adjacency list
                    free(del_adj);
                }
                //stores the present currency in del_next
                ptr_currency del_next = temp_next;
                //travesal through separate chaining of the currency hashtable
                temp_next = temp_next->next;
                //frees a currency node in the separate chaining
                free(del_next);
            }
        }
        //check if prev_bank and Trade_bank points to same tradebank node
        //this happens when the required tradebank is at the start of the separate chaining
        if (prev_bank == Trade_bank)
        {
            //making the hashtable point to the next bank in the separate chaining
            //as we are deleting the first tradebank node in the separate chaining
            g->list_trade_banks[hash_value] = prev_bank->next;
        }
        else
        {
            //making the prevbank's next pointer to the bank which is next to the given trade bank
            //i.e, removing the given tradebank in the separate chaining
            prev_bank->next = Trade_bank->next;
        }
        //frees the memory allocated to given trade bank
        free(Trade_bank);
        Trade_bank = NULL;
    }
    return;
}

void add_currency(graph g, char *bank_name, char *currency_name)
{
    long int bank_name_hash = compute_hash(g->table_size, bank_name);

    //Checking if Trade Bank named "bank_name" exists
    if (g->list_trade_banks[bank_name_hash] != NULL) //Trade Bank exists
    {
        //Creating New Currency Node
        ptr_currency new_currency = malloc(sizeof(currency));
        assert(new_currency != NULL); // throws an error if memory not available
        strcpy(new_currency->id, currency_name);
        new_currency->adj_list = NULL;
        new_currency->next = NULL;
        new_currency->dest_wt = 0;

        //Iterating through all trade banks with Hash Value: bank_name_hash to find the trade bank with Name: "bank_name"
        ptr_trade_bank read_trade_bank = g->list_trade_banks[bank_name_hash];
        while (read_trade_bank)
        {
            if (strcmp(read_trade_bank->name_trade_bank, bank_name) == 0)
                break;
            read_trade_bank = read_trade_bank->next;
        }
        if (read_trade_bank == NULL)
        {
            printf("Trade Bank not found\n");
            return;
        }

        //Iterating through all currencies with Hash Value: currency_name_hash to check if currency with Name: "currency_name" already exists
        long int currency_name_hash = compute_hash(read_trade_bank->table_size, currency_name);
        ptr_currency read_currency = read_trade_bank->list_currency[currency_name_hash];
        while (read_currency != NULL)
        {
            if (strcmp(read_currency->id, currency_name) == 0)
            {
                printf("Currency already exists\n");
                return;
            }
            read_currency = read_currency->next;
        }

        if (read_trade_bank->list_currency[currency_name_hash] == NULL)
        {
            read_trade_bank->check[read_trade_bank->last_index] = currency_name_hash;
            read_trade_bank->last_index++;
        }
        new_currency->vertex_id = read_trade_bank->nvertices; // assigning vertex id to the new currency
        read_trade_bank->nvertices++;                         // increments the number of currencies in the trade bank

        //Adding New Currency (to the front of the list) by separate chaining
        new_currency->next = read_trade_bank->list_currency[currency_name_hash];
        read_trade_bank->list_currency[currency_name_hash] = new_currency;
    }
    else
        printf("Trade Bank Not Found\n");
}

void delete_currency(graph g, char *bank_name, char *currency_name)
{
    int found = 0;
    long int bank_name_hash = compute_hash(g->table_size, bank_name);

    //Iterating through all trade banks with Hash Value: bank_name_hash to check if trade bank with Name: "bank_name" exists
    ptr_trade_bank read_trade_bank = g->list_trade_banks[bank_name_hash];
    while (read_trade_bank)
    {
        if (strcmp(read_trade_bank->name_trade_bank, bank_name) == 0)
        {
            found = 1;
            break;
        }
        read_trade_bank = read_trade_bank->next;
    }
    if (found) //Trade bank with Name: "bank_name" exists
    {
        found = 0;
        long int currency_name_hash = compute_hash(g->table_size, currency_name);

        //Iterating through all currencies with Hash Value: currency_name_hash to check if currency with Name: "currency_name" exists
        ptr_currency read_currency = read_trade_bank->list_currency[currency_name_hash];
        ptr_currency prev_read_currency = read_currency;
        if (read_currency)
        {
            while (read_currency)
            {
                if (strcmp(read_currency->id, currency_name) == 0)
                {
                    found = 1;
                    break;
                }
                prev_read_currency = read_currency;
                read_currency = read_currency->next;
            }
        }
        if (found) //Currency with Name: "currency_name" exists
        {
            if (read_currency->adj_list != NULL)
            {
                //Step 1: Iterate through check array
                //        For every currency that can be converted to "currency_name", remove "currency_name" from their adjacency list
                for (long int i = 0; i < read_trade_bank->last_index; i++)
                {
                    //Go to currency list of index stored in check
                    long int hash_val = read_trade_bank->check[i];
                    ptr_currency check_currency = read_trade_bank->list_currency[hash_val];

                    while (check_currency)
                    {
                        //Check if "currency_name" present in their adjacency list
                        ptr_currency temp = check_currency->adj_list;
                        ptr_currency temp_prev = check_currency;
                        while (temp)
                        {
                            if (strcmp(temp->id, currency_name) == 0)
                            {
                                temp_prev->adj_list = temp->adj_list;
                                free(temp);
                                break;
                            }
                            temp_prev = temp;
                            temp = temp->adj_list;
                        }
                        check_currency = check_currency->next;
                    }
                }

                //Step 2: Delete all currencies connected to "currency_name"
                ptr_currency del_ptr = read_currency->adj_list;
                read_currency->adj_list = NULL;
                while (del_ptr)
                {
                    ptr_currency temp_del_ptr = del_ptr;
                    del_ptr = del_ptr->adj_list;
                    free(temp_del_ptr);
                }
            }
            if (read_currency == prev_read_currency)
                read_trade_bank->list_currency[currency_name_hash] = read_currency->next;
            else
                prev_read_currency->next = read_currency->next;

            free(read_currency);
            read_currency = NULL;
        }
        else
        {
            printf("Currency Not Found\n");
            return;
        }
    }
    else
    {
        printf("Trade Bank Not Found\n");
        return;
    }
}

// This function adds a weighted edge between 2 specified currencies in the specified trade bank
void add_conversion(graph g, char *bank_name, char *currency_name_s, char *currency_name_d, weight wt)
{
    long int hash_tb = compute_hash(g->table_size, bank_name); // find the required trade bank
    ptr_trade_bank tb = NULL;
    ptr_trade_bank temp = g->list_trade_banks[hash_tb];

    while (temp != NULL)
    {
        if (strcmp(temp->name_trade_bank, bank_name) == 0)
        {
            tb = temp;
            break;
        }
        else
        {
            temp = temp->next;
        }
    }
    if (temp == NULL)
    {
        // Print an error message if the specified trade bank is not found
        printf("Trade Bank not found\n");
        return;
    }

    long int hash_curr = compute_hash(tb->table_size, currency_name_s); // find the source currency in the trade bank
    ptr_currency curr = NULL;
    ptr_currency temp2 = tb->list_currency[hash_curr];

    while (temp2 != NULL)
    {
        if (strcmp(temp2->id, currency_name_s) == 0)
        {
            curr = temp2;
            break;
        }
        else
        {
            temp2 = temp2->next;
        }
    }
    if (temp2 == NULL)
    {
        // print an error message if the source currency is not found in the trade bank
        printf("Source Currency not found\n");
        return;
    }

    long int hash_curr_2 = compute_hash(tb->table_size, currency_name_d); // find the destination currency
    ptr_currency curr2 = NULL;
    ptr_currency temp3 = tb->list_currency[hash_curr_2];

    while (temp3 != NULL)
    {
        if (strcmp(temp3->id, currency_name_d) == 0)
        {
            curr2 = temp3;
            break;
        }
        else
        {
            temp3 = temp3->next;
        }
    }
    if (temp3 == NULL)
    {
        // print an error message if the destination currency is not found in the trade bank
        printf("Destination Currency not found\n");
        return;
    }
    ptr_currency position = temp2->adj_list;
    // traverse through the adjacency list of the source currency to check if there already exists a conversion between the
    // specified source and destination currencies. If it already exists, update the cost of the conversion to the new value
    while (position != NULL)
    {
        if (strcmp(position->id, currency_name_d) == 0)
        {
            position->dest_wt = wt;
            return;
        }
        position = position->adj_list;
    }

    // if the conversion does not exist already, then create a new currency node and add it in the adjacency list of the source
    // currency with the appropriate conversion cost
    ptr_currency dest_curr = (ptr_currency)malloc(sizeof(currency));
    assert(dest_curr != NULL); // throws an error if memory not available

    dest_curr->adj_list = curr->adj_list;
    dest_curr->next = NULL;
    dest_curr->vertex_id = temp3->vertex_id;
    strcpy(dest_curr->id, currency_name_d);
    dest_curr->dest_wt = wt;
    curr->adj_list = dest_curr;
}

// This function deletes the conversion between 2 currencies in a specified trade bank if it exists
void delete_conversion(graph g, char *bank_name, char *currency_name_s, char *currency_name_d)
{
    long int hash_tb = compute_hash(g->table_size, bank_name); // find the required trade bank
    ptr_trade_bank tb = NULL, temp = g->list_trade_banks[hash_tb];

    while (temp != NULL)
    {
        if (strcmp(temp->name_trade_bank, bank_name) == 0)
        {
            tb = temp;
            break;
        }
        else
        {
            temp = temp->next;
        }
    }
    if (temp == NULL)
    {
        // Print an error message if the trade bank is not found
        printf("Trade Bank not found\n");
        return;
    }

    long int hash_curr = compute_hash(tb->table_size, currency_name_s); // find the source currency
    ptr_currency curr = NULL;
    ptr_currency temp2 = tb->list_currency[hash_curr];

    while (temp2 != NULL)
    {
        if (strcmp(temp2->id, currency_name_s) == 0)
        {
            curr = temp2;
            break;
        }
        else
        {
            temp2 = temp2->next;
        }
    }
    if (temp2 == NULL)
    {
        // print an error message if the source currency is not found
        printf("Source Currency not found\n");
        return;
    }

    ptr_currency previous = curr;
    ptr_currency present = curr->adj_list;

    // Traverse through the adjacency list of the source currency and compare the currency names with the required destination currency name
    while (present != NULL)
    {
        // if the destination currency is found, delete the conversion (hence, remove that node from the adjacency list)
        if (strcmp(present->id, currency_name_d) == 0)
        {
            break;
        }
        previous = present;
        present = present->adj_list;
    }
    // if after traversing the entire list, you do not find the destination currency node, print an error message
    if (present == NULL)
    {
        printf("Conversion doesn't exist\n");
        return;
    }
    previous->adj_list = present->adj_list;
    free(present);
}

//this prints all the currencies and the conversions present in the given bank
void print_trade_bank(graph g, char *bank)
{
    //computes the stringhash of the bank name
    long int bank_hash = compute_hash(g->table_size, bank);
    //making current point to the separate chaining of the bank with same hashvalue
    ptr_trade_bank current = g->list_trade_banks[bank_hash];
    //while loop traverses through the separate chaining
    while (current != NULL)
    {
        //checks whether the bank exist in the separate chaining
        if (strcmp(current->name_trade_bank, bank) == 0)
        {
            //breaks from the while loop when we finally traversed to the given bank
            //current will point to our desired bank struct
            break;
        }
        //traversal of separate chaining
        current = current->next;
    }
    //checks whether the bank exists
    if (current == NULL)
    {

        printf("the given trade bank doesn't exist.\n");
        return;
    }
    //prints tradebank name
    printf("Trade Bank : %s\n\n", current->name_trade_bank);

    //for loop traverses through every filled index in the currency hashtable of the given bank
    for (int i = 0; i < current->last_index; i++)
    {
        //presentcurrency points to the separate chaining of the index in the hashtable which is equal to check[i] in the bank struct
        ptr_currency presentcurrency = current->list_currency[current->check[i]];
        //traversal through the separate chaining presentcurrency
        while (presentcurrency != NULL)
        {
            //prints a currency name
            printf("%s\n", presentcurrency->id);
            //edge points to a linked list of all outgoing conversions from presentcurrency node
            ptr_currency edge = presentcurrency->adj_list;
            //while loop traverses through the linkedlist of outgoing edges
            while (edge != NULL)
            {
                //prints the outgoing conversion with destination currency name and conversion rate
                printf("\t\tTo currency %s with conversion rate %ld.\n", edge->id, edge->dest_wt);
                //traversal of the linkedlist
                edge = edge->adj_list;
            }
            //traversal of separate chaining of currency hashtable
            presentcurrency = presentcurrency->next;
        }
    }
}

// This function prints the currency nodes present in the adjacency list of the given currency in the specified trade bank along
// with the conversion rates
void print_currency(graph g, char *TradeBank_name, char *Currency_name, length table_size)
{

    length hash_value = compute_hash(table_size, TradeBank_name); // find the required trade bank
    if (g->list_trade_banks[hash_value] == NULL)
    {
        // throw an error message if the hash value computed in the last step points to NULL
        printf("THERE IS NO TRADEBANK WITH NAME %s\n", TradeBank_name); 
        return;
    }

    ptr_trade_bank Trade_bank = g->list_trade_banks[hash_value];
    // It may so happen that more than 1 trade banks are stored at the same hash value (since we have used separate chaining), in that case
    // traverse the linked list at the computed hash value whilst comparing the names with the name of the given trade bank
    while (Trade_bank != NULL)
    {

        if (strcmp(Trade_bank->name_trade_bank, TradeBank_name) == 0)
        {
            break;
        }
        Trade_bank = Trade_bank->next;
    }
    if (Trade_bank == NULL)
    {
        // if the linked list ends but the trade bank is not found, print an error message
        printf("THERE IS NO TRADEBANK WITH NAME %s\n", TradeBank_name);
        return;
    }
    length hash_value1 = compute_hash(table_size, Currency_name); // compute the hash value of the required currency node
    ptr_currency Currency = Trade_bank->list_currency[hash_value1];
    if (Currency == NULL)
    {
        // if the hash table cell at the computed hash value points to NULL, then print an error message
        printf("THERE IS NO CURRECNY WITH NAME %s IN THIS TRADEBANK\n", Currency_name);
        return;
    }

    // It may so happen that more than 1 currencies are stored at the same hash value (since we have used separate chaining), in that case
    // traverse the linked list at the computed hash value whilst comparing the names with the name of the given currency 
    while (Currency != NULL)
    {

        if (strcmp(Currency->id, Currency_name) == 0)
        {
            break;
        }
        Currency = Currency->next;
    }
    if (Currency == NULL)
    {
        // if the linked list ends but the currency is not found, print an error message
        printf("THERE IS NO CURRENCY WITH NAME %s IN THIS TRADEBANK\n", TradeBank_name);
        return;
    }

    ptr_currency current = Currency;
    printf("Name of currency is %s\n", current->id);
    // if the currency is found, traverse through its adjacency list and print the currency names and the conversion rates for the currency nodes 
    // present in the adjacency list
    while (current->adj_list != NULL)
    {
        current = current->adj_list;
        printf("%s->%s : Conversion_Rate->%ld\n", Currency->id, current->id, current->dest_wt);
    }

    return;
}

// This function finds the trade bank with the given name and returns a pointer to it
ptr_trade_bank find_trade_bank(graph g, char *bank_name)
{
    long int hash_tb = compute_hash(g->table_size, bank_name); // compute the hash value of the bank name using the compute_hash() function
    ptr_trade_bank tb = NULL;
    ptr_trade_bank temp = g->list_trade_banks[hash_tb];

    // It may happen that more than 1 trade banks possess the same hash value, so traverse the separate chaining linked list of the computed
    // hash value whilst comparing the bank names with the specified bank name
    while (temp != NULL)
    {
        if (strcmp(temp->name_trade_bank, bank_name) == 0)
        {
            tb = temp;
            break;
        }
        else
        {
            temp = temp->next;
        }
    }
    // if the trade bank is not found even after traversing the entire separate chaining linked list, print an error message
    if (temp == NULL)
    {
        printf("Trade Bank not found\n");
        return NULL;
    }
    return tb;
}

// This function finds the currency with the specified name and returns a pointer to it
ptr_currency find_currency(graph g, ptr_trade_bank tb, char *currency_name)
{
    long int hash_curr = compute_hash(tb->table_size, currency_name); // compute the hash value of the given currency name using the
                                                                      // compute_hash() function
    ptr_currency curr = NULL;
    ptr_currency temp = tb->list_currency[hash_curr];

    // It may happen that more than 1 currencies possess the same hash value, so traverse the separate chaining linked list of the computed
    // hash value whilst comparing the currency names with the specified currency name
    while (temp != NULL)
    {
        if (strcmp(temp->id, currency_name) == 0)
        {
            curr = temp;
            break;
        }
        else
        {
            temp = temp->next;
        }
    }
    // if the currency is not found even after traversing the entire separate chaining linked list, return NULL
    // (this block can also be used to print an error message, but that prints unnecessary lines in the output, so we have commented the printf)
    if (temp == NULL)
    {
        // printf("Currency not found\n");
        return NULL;
    }
    return curr;
}

/* This is the driver function to find the best path between two currencies (using best_path_TB())
   This function prints the best path (using print_path()) as well as the best cost and chosen trade bank */
void best_path(graph g, char *src, char *dest)
{

    ptr_currency curr_dest = NULL; // pointer to the destination currency in the best trade bank
    ptr_currency temp_dest = NULL; // pointer to the destination currency in each trade bank
    ptr_currency temp_src = NULL;  // pointer to the source currency in each trade bank
    length best_cost = infinite;   // to store the shortest path cost
    length temp_cost = 0;          // to store the shortest path cost in each trade bank
    ptr_sp_table best_temp = NULL; // to store the SP Table array of each trade bank
    ptr_sp_table best_path = NULL; // to store the SP Table array of best trade bank
    char best_tb[100];             // to store the name of best trade bank
    ptr_trade_bank temp = NULL;
    for (int j = 0; j < g->last_index; j++) // iterates through all trade banks present in the hash table
    {
        temp = g->list_trade_banks[g->check[j]]; // temp points to the trade bank with given hash value
        while (temp != NULL)
        {
            temp_dest = find_currency(g, temp, dest);  // finds destination currency in temp trade bank
            temp_src = find_currency(g, temp, src);    // finds source currency in temp trade bank
            if (temp_dest != NULL && temp_src != NULL) /* finds best path only if both source and destination currencies
                                                          are present in temp trade bank */
            {
                best_temp = best_path_TB(g, temp, src, dest);
                temp_cost = best_temp[temp_dest->vertex_id].dv;
                if (temp_cost < best_cost) // if current best cost is greater than the best cost from temp trade bank
                {
                    curr_dest = temp_dest;                  // Updates the destination currency
                    best_path = best_temp;                  // Updates the best path
                    best_cost = temp_cost;                  // Updates the best cost
                    strcpy(best_tb, temp->name_trade_bank); // Updates the best trade bank name
                }
            }
            temp = temp->next; // travels through the separate chaining linked list in case of collisions
        }
    }

    if (best_cost == infinite)
    {
        printf("No path available from %s to %s\n", src, dest);
        return;
    }

    printf("The best path from %s to %s is : ", src, dest);
    print_path(best_path, curr_dest->vertex_id); // prints the best path from source to destination currency
    printf("\n");
    printf("The cost of best path is %ld  :  from the trade bank %s\n\n", best_cost, best_tb);
}

void second_best_path(graph g, char *src, char *dest)
{

    ptr_currency second_curr_dest = NULL; // pointer to the destination currency in the trade bank of 2nd shortest path

    ptr_currency curr_dest11 = NULL; // pointer to the destination currency in the best trade bank in the shortest path
    ptr_currency curr_dest12 = NULL; // pointer to the destination currency in the 2nd best trade bank in the shortest path
    ptr_currency curr_dest2 = NULL;  // pointer to the destination currency in the best trade bank in the 2nd shortest path

    ptr_currency temp_dest = NULL; // pointer to the destination currency in each trade bank
    ptr_currency temp_src = NULL;  // pointer to the source currency in each trade bank

    length second_best_cost = infinite; // to store the 2nd shortest path cost

    length best_cost11 = infinite; // to store the shortest path cost in shortest path
    length best_cost12 = infinite; // to store the 2nd shortest path cost in shortest path
    length best_cost2 = infinite;  // to store the shortest path cost in 2nd shortest path

    length temp_cost1 = 0; // to store the shortest path cost in each trade bank
    length temp_cost2 = 0; // to store the 2nd shortest path cost in each trade bank

    ptr_sp_table best_temp = NULL;  // to store the SP Table array of each trade bank corresponding to best path
    ptr_sp_table best_temp2 = NULL; // to store the SP Table array of each trade bank corresponding to 2nd best path

    ptr_sp_table second_best_path = NULL; // to store the SP Table array of the trade bank in 2nd shortest path

    ptr_sp_table best_path11 = NULL; // to store the SP Table array of best trade bank in shortest path
    ptr_sp_table best_path12 = NULL; // to store the SP Table array of 2nd best trade bank in shortest path
    ptr_sp_table best_path2 = NULL;  // to store the SP Table array of best trade bank in 2nd shortest path

    char second_best_tb[100]; // to store the name of the trade bank in 2nd shortest path

    char best_tb11[100]; // to store the name of best trade bank in shortest path
    char best_tb12[100]; // to store the name of 2nd best trade bank in shortest path
    char best_tb2[100];  // to store the name of best trade bank in 2nd shortest path

    ptr_trade_bank temp = NULL;
    for (int j = 0; j < g->last_index; j++) // iterates through all trade banks present in the hash table
    {
        temp = g->list_trade_banks[g->check[j]]; // temp points to the trade bank with given hash value
        while (temp != NULL)
        {
            temp_dest = find_currency(g, temp, dest);  // finds destination currency in temp trade bank
            temp_src = find_currency(g, temp, src);    // finds source currency in temp trade bank
            if (temp_dest != NULL && temp_src != NULL) /* finds best paths only if both source and destination currencies
                                                          are present in temp trade bank */
            {
                best_temp = best_path_TB(g, temp, src, dest);
                best_temp2 = best_path_TB2(g, temp, src, dest);
                temp_cost1 = best_temp[temp_dest->vertex_id].dv;
                temp_cost2 = best_temp2[temp_dest->vertex_id].dv;
                if (temp_cost1 < best_cost11) // if current best cost11 is greater than the best cost1 from temp trade bank
                {
                    curr_dest12 = curr_dest11; // Updates the destination currency12
                    curr_dest11 = temp_dest;   // Updates the destination currency11

                    best_path12 = best_path11; // Updates the best path12
                    best_path11 = best_temp;   // Updates the best path11

                    best_cost12 = best_cost11; // Updates the best cost12
                    best_cost11 = temp_cost1;  // Updates the best cost11

                    strcpy(best_tb12, best_tb11);             // Updates the best trade bank name12
                    strcpy(best_tb11, temp->name_trade_bank); // Updates the best trade bank name11
                }
                if (temp_cost2 < best_cost2) // if current best cost2 is greater than the best cost2 from temp trade bank
                {
                    curr_dest2 = temp_dest;                  // Updates the destination currency2
                    best_path2 = best_temp2;                 // Updates the best path2
                    best_cost2 = temp_cost2;                 // Updates the best cost2
                    strcpy(best_tb2, temp->name_trade_bank); // Updates the best trade bank name2
                }
            }

            temp = temp->next; // travels through the separate chaining linked list in case of collisions
        }
    }
    if (best_cost12 == best_cost11) // checks if best cost11 and best cost 12 are same
    {
        // second best parameters are updates to best 2 parameters
        second_best_cost = best_cost2;
        second_best_path = best_path2;
        second_curr_dest = curr_dest2;
        strcpy(second_best_tb, best_tb2);
    }
    else
    {
        if (best_cost2 < best_cost12)
        {
            // second best parameters are updates to best 2 parameters
            second_best_cost = best_cost2;
            second_best_path = best_path2;
            second_curr_dest = curr_dest2;
            strcpy(second_best_tb, best_tb2);
        }
        else
        {
            // second best parameters are updates to best 12 parameters
            second_best_cost = best_cost12;
            second_best_path = best_path12;
            second_curr_dest = curr_dest12;
            strcpy(second_best_tb, best_tb12);
        }
    }

    if (second_best_cost == infinite)
    {
        printf("No 2nd best path available from %s to %s\n", src, dest);
        return;
    }

    printf("The 2nd best path from %s to %s is : ", src, dest);
    print_path(second_best_path, second_curr_dest->vertex_id); // prints the 2nd best path from source to destination currency

    printf("\n");
    printf("The cost of 2nd best path is %ld  :  from the trade bank %s\n\n", second_best_cost, second_best_tb);
}

/* To find the best as well as 2nd best path from source to destination currency in a particular trade bank
   If both source and destination currencies are present in the trade bank
   Uses Dijkstra's Algorithm 
   Uses Priority Queue (Min Heap) */
ptr_sp_table best_path_TB(graph g, ptr_trade_bank tb, char *src, char *dest)
{

    pqueue Q = create_pqueue(tb->nvertices); // creates a priority queue with size = number of currencies in the trade bank

    length i[tb->nvertices]; // array to store the index of each currency in the priority queue
                             // i[currency->vertex id] gives index of currency in the priority queue
    for (length t = 0; t < tb->nvertices; t++)
        i[t] = -1; // initializing the indices to -1

    ptr_sp_table path = (ptr_sp_table)malloc(tb->nvertices * sizeof(SPtable)); // SP Table array
    assert(path != NULL); // throws an error if memory not available           // Has the SP table of every currency
    for (int i = 0; i < tb->nvertices; i++)                                    // initializing the SP Table of each currency
    {
        path[i].known = 0;
        path[i].dv = infinite;
        path[i].prev_vertex = -1;
    }

    ptr_currency curr_src = find_currency(g, tb, src);   // points to source currency in tb trade bank
    ptr_currency curr_dest = find_currency(g, tb, dest); // points to destination currency in tb trade bank

    // initializes SP Table of source currency
    strcpy(path[curr_src->vertex_id].currency_name, curr_src->id);
    path[curr_src->vertex_id].known = 1;
    path[curr_src->vertex_id].dv = 0;

    ptr_currency temp = curr_src->adj_list; // points to the adjacency list of source currency

    // inserts the outgoing edge currencies of source currency to the priority queue
    while (temp != NULL)
    {
        insert_pqueue(Q, temp->vertex_id, temp->dest_wt, i);
        path[temp->vertex_id].dv = temp->dest_wt;
        path[temp->vertex_id].prev_vertex = curr_src->vertex_id;
        strcpy(path[temp->vertex_id].currency_name, temp->id);
        temp = temp->adj_list;
    }

    //Dijkstra's Algorithm implemented
    while (!(is_pqueue_empty(Q))) // checks if the priority queue is empty
    {
        element_type curr = extract_min(Q, i); // extracts the currency with minimum dv
                                               // curr is a struct
                                               // contains vertex id of currency and its dv as key
        i[curr.vertex_id] = -1;                /* changes the index of extracted currency to -1 
                                                  as its deleted from the priority queue */
        path[curr.vertex_id].dv = curr.key;
        path[curr.vertex_id].known = 1;             // marking the extracted currency as visited
        if (curr_dest->vertex_id == curr.vertex_id) // checks if the destination currency is visited
        {
            return path; // returns the SP Table array
        }
        ptr_currency current = find_currency(g, tb, path[curr.vertex_id].currency_name); // points to the extracted currency
        temp = current->adj_list;
        while (temp != NULL) // travels through the adjacency list of current currency
        {
            if (strcmp(path[temp->vertex_id].currency_name, temp->id) != 0)
            {
                strcpy(path[temp->vertex_id].currency_name, temp->id); // stores the current currency name in its SP Table
            }

            // If a shorter dv is possible to the current currency
            if (path[temp->vertex_id].known == 0 && path[curr.vertex_id].dv + temp->dest_wt < path[temp->vertex_id].dv)
            {
                // Updates the dv previous vertex of current currency

                path[temp->vertex_id].prev_vertex = curr.vertex_id;
                path[temp->vertex_id].dv = path[curr.vertex_id].dv + temp->dest_wt;

                if (i[temp->vertex_id] >= 0) // if current currency is present in the priority queue
                {

                    decrease_key(Q, temp->vertex_id, path[temp->vertex_id].dv, i);
                }
                else // if current currency is not present in the priority queue
                {
                    insert_pqueue(Q, temp->vertex_id, path[temp->vertex_id].dv, i);
                }
            }

            temp = temp->adj_list; // travels through the adjacency list of current currency
        }
    }
    return path; // returns the SP Table array
}

// prints path from source to destination currency
// vertex id is the destination currency's vertex id
// path is the SP Table array of all currencies
void print_path(ptr_sp_table path, length vertex_id)
{
    if (path[vertex_id].prev_vertex != -1)
    {
        print_path(path, path[vertex_id].prev_vertex);
        printf(" -> ");
    }
    printf("%s", path[vertex_id].currency_name);
}

// This function checks if the vertexId of the destination node is equal to the king of the source node and traverses the
// subset array till such a node is found whilst updating the kings of the other nodes
length find(subsets A[], length index)
{
    if (A[index].king != index)
    {
        A[index].king = find(A, A[index].king);
    }
    return A[index].king;
}

// This function compares the kings of 2 given nodes and updates the values of the parameters king and rank
void Union(subsets A[], length src, length dest)
{
    length kingofsrc = find(A, src); // find the king of the 2 given nodes using the find() function
    length kingofdest = find(A, dest);

    if (A[kingofsrc].rank < A[kingofdest].rank) // update the king of the node whose king is smaller
    {
        A[kingofsrc].king = kingofdest;
    }
    else if (A[kingofsrc].rank > A[kingofdest].rank)
    {
        A[kingofdest].king = kingofsrc;
    }
    else // if kingofsrc = kingofdest, update the king of one of them and increment the rank of the other
    {
        A[kingofdest].king = kingofsrc;
        A[kingofsrc].rank++;
    }
}

// This function prints the nodes that constitue a cycle in the trade bank
void print_stack(char stack[][50], length nfilled, length start)
{
    printf("The cycle is:");
    for (int i = start; i < nfilled; i++)
    {
        printf(" %s", stack[i]);
    }
    printf("\n");
}

// This function performs DFS on the currency nodes and takes the help of the Union() and find() functions to update the parameters
// king and rank which detect cycles
length DFS(graph g, ptr_trade_bank bank, ptr_currency currency_src, length pVisited[], char stack[][50], subsets A[], length nfilled)
{
    pVisited[currency_src->vertex_id] = 1;             // for the current node which is being visited, set the corresponding pVisited value to 1
    strcpy(stack[nfilled++], currency_src->id);        // add the name of the current node in the stack array and increment the index pointer
    A[currency_src->vertex_id].position = nfilled - 1; // update the position parameter in the subset array to point at the current position
                                                       // which is filled in the stack array

    ptr_currency temp_adj = currency_src->adj_list;
    while (temp_adj != NULL) // recursively call the DFS() function for all the nodes in the adjacency list of the current node
    {
        length x = find(A, currency_src->vertex_id); // call find function for the src and dest currencies
        length y = find(A, temp_adj->vertex_id);
        if (x == y) // if the kings are the same for src and destination, it means a cycle has been detected
        {
            length start = A[temp_adj->vertex_id].position;
            print_stack(stack, nfilled, start); // call print_stack() function to print the detected cycle
            return 1;                           // return 1 if a cycle is detected
        }

        if (pVisited[temp_adj->vertex_id] != 1) // if the destination node has not already been visited, call DFS() recursively for the dest node
        {
            Union(A, x, y); // call the Union function to update the king and rank of the src and dest currencies
            ptr_currency new = find_currency(g, bank, temp_adj->id);
            if (DFS(g, bank, new, pVisited, stack, A, nfilled) == 1)
            {
                return 1;
            }
        }
        temp_adj = temp_adj->adj_list;
    }
    nfilled--;                                                 // pop the current vertex from the cycle stack when you return from the DFS()
    A[currency_src->vertex_id].king = currency_src->vertex_id; // reset the king of the vertex as the vertexId while returning from DFS
    return 0;                                                  // return 0 if a cycle is not detected
}

//checks whether there is a cycle in the given trade bank
void Find_cycle(graph g, char *bank_name)
{
    length flag = 0;
    //bank points to the required tradebank
    ptr_trade_bank bank = find_trade_bank(g, bank_name);
    //checks if bank exists
    if (bank == NULL)
    {
        return;
    }

    //used to detect and print a cycle
    subsets A[bank->nvertices];
    //stores the currency names. useful while printing a cycle
    char stack[bank->nvertices][50];
    //stores whether a vertex is visited or not in the rest of the function
    length pVisited[bank->nvertices];
    //stores the number of currency names stored in the 2D stack array
    length nfilled = 0;
    //forloop initializes the subsets array(A) and pVisited
    for (int i = 0; i < bank->nvertices; i++)
    {
        A[i].king = i;
        A[i].rank = 0;
        pVisited[i] = 0;
    }
    //traverses through all the currency nodes
    for (int i = 0; i < bank->last_index; i++)
    {
        //current points to the separate chaining of the currency hashtable in the check[i] index
        ptr_currency current = bank->list_currency[bank->check[i]];
        //traverses through the separate chaining
        while (current != NULL)
        {
            //checks whether the currency node is already visited
            if (pVisited[current->vertex_id] != 1)
            {
                //this does DFS for this currency node
                //DFS will detect a cycle if it exists and will print one cycle
                //returns 1 if there is a cycle
                //else returns 0
                flag = DFS(g, bank, current, pVisited, stack, A, nfilled);
                //checks whether the cycle is detected
                if (flag == 1)
                {
                    return;
                }
            }
            //traversal of separate chaining
            current = current->next;
        }
    }
    //checks whether there is no cycle in whole tradebank graph
    if (flag == 0)
    {
        printf("There is no cycle available in the given trade bank\n");
    }
}

ptr_sp_table best_path_TB2(graph g, ptr_trade_bank tb, char *src, char *dest)
{
    ptr_sp_table best = best_path_TB(g, tb, src, dest);  // finds the best path from src to dest currency in trade bank tb
    ptr_sp_table temp = NULL;                            // stores the temporary best path after making a conversion rate infinite
    ptr_sp_table best2 = NULL;                           // stores the 2nd best path from src to dest currency in trade bank tb
    ptr_currency curr_dest = find_currency(g, tb, dest); // pointer to destination currency in tb trade bank
    length vertex_id = curr_dest->vertex_id;             // stores the vertex id of each currency in the best path
    length prev_vertex_id = best[vertex_id].prev_vertex; // previous vertex id of destination currency in tb trade bank
    length original_wt = 0;                              // stores the deleted conversion rate
    length dest_vertex_id = curr_dest->vertex_id;        // vertex id of destination currency in tb trade bank

    /* travels through each currency in the best path and 
       1. makes that particular conversion rate infinite
       2. finds the best path from source to destination currency in the tb tradebank
       3. restores the changed conversion rate to its original conversion rate
       This process is done for all the edges (conversion rates) in the best path
    */
    while (best[vertex_id].prev_vertex != -1)
    {
        original_wt = best[vertex_id].dv - best[prev_vertex_id].dv;

        //makes that particular conversion rate infinite
        add_conversion(g, tb->name_trade_bank, best[prev_vertex_id].currency_name, best[vertex_id].currency_name, infinite);
        temp = best_path_TB(g, tb, src, dest);
        if (best2 == NULL) // initial check if best2 is NULL
            best2 = temp;

        // checks if temp path is shorter than best2 path as well as if temp is greater than the best path
        else if (temp[dest_vertex_id].dv < best2[dest_vertex_id].dv && temp[dest_vertex_id].dv > best[dest_vertex_id].dv)
        {
            best2 = temp; // updates best2 path
        }

        // restores the changed conversion rate to its original conversion rate
        add_conversion(g, tb->name_trade_bank, best[prev_vertex_id].currency_name, best[vertex_id].currency_name, original_wt);
        vertex_id = prev_vertex_id;
        prev_vertex_id = best[vertex_id].prev_vertex;
    }
    return best2;
}