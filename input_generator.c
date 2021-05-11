#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
#include <math.h>

#define N 750

typedef struct string
{
    char *s;
} string;
int main()
{
    srand(time(0));

    //List of bank names, currencies and instruction strings
    string bank_name[] = {"HSBC", "ICICI", "ICC", "FABB", "DASS"};
    string curr[] = {"AFN", "ALL", "AOA", "ARM", "BSD", "BDT", "EUR", "INR", "USD"};
    string inst_string[] = {"add_trade_bank", "delete_trade_bank", "add_currency", "delete_currency", "add_conversion", "delete_conversion", "best_path", "second_best_path", "find_cycle"};

    printf("add_trade_bank HSBC\n");
    printf("add_currency HSBC ALL\n");
    printf("add_currency HSBC AFN\n");

    int size_currency = sizeof(curr) / sizeof(curr[0]);                  //number of available currencies
    int size_inst_string = sizeof(inst_string) / sizeof(inst_string[0]); //number of available instructions from user
    int size_bank_name = sizeof(bank_name) / sizeof(bank_name[0]);       //number of available banks

    for (int i = 0; i < N; i++)
    {
        int instruction = rand() % size_inst_string; // to select a random instruction
        // bank_name_index - to select random bank
        // string_index1 - to select random source currency
        // string_index2 - to select random destination currency
        // w             - to select random conversion rate
        if (strcmp("add_trade_bank", inst_string[instruction].s) == 0)
        {
            int bank_name_index = rand() % size_bank_name;
            printf("%s %s\n", inst_string[instruction].s, bank_name[bank_name_index].s);
        }
        else if (strcmp("delete_trade_bank", inst_string[instruction].s) == 0)
        {
            int bank_name_index = rand() % size_bank_name;
            printf("%s %s\n", inst_string[instruction].s, bank_name[bank_name_index].s);
        }
        else if (strcmp("add_currency", inst_string[instruction].s) == 0)
        {
            int bank_name_index = rand() % size_bank_name;
            int string_index1 = rand() % size_currency;
            printf("%s %s %s\n", inst_string[instruction].s, bank_name[bank_name_index].s, curr[string_index1].s);
        }
        else if (strcmp("delete_currency", inst_string[instruction].s) == 0)
        {
            int bank_name_index = rand() % size_bank_name;
            int string_index1 = rand() % size_currency;
            printf("%s %s %s\n", inst_string[instruction].s, bank_name[bank_name_index].s, curr[string_index1].s);
        }
        else if (strcmp("add_conversion", inst_string[instruction].s) == 0)
        {
            int bank_name_index = rand() % size_bank_name;
            int string_index1 = rand() % size_currency;
            int string_index2 = rand() % size_currency;

            int w = rand() % N;

            if (string_index1 == string_index2)
                string_index2 = (string_index2 + 1) % (size_currency);

            printf("%s %s %s %s %d\n", inst_string[instruction].s, bank_name[bank_name_index].s, curr[string_index1].s, curr[string_index2].s, w);
        }
        else if (strcmp("delete_conversion", inst_string[instruction].s) == 0)
        {
            int bank_name_index = rand() % size_bank_name;
            int string_index1 = rand() % size_currency;
            int string_index2 = rand() % size_currency;

            if (string_index1 == string_index2)
                string_index2 = (string_index2 + 1) % (size_currency);

            printf("%s %s %s %s\n", inst_string[instruction].s, bank_name[bank_name_index].s, curr[string_index1].s, curr[string_index2].s);
        }
        else if (strcmp("best_path", inst_string[instruction].s) == 0)
        {
            int bank_name_index = rand() % size_bank_name;

            int string_index1 = rand() % size_currency;
            int string_index2 = rand() % size_currency;

            if (string_index1 == string_index2)
                string_index2 = (string_index2 + 1) % (size_currency);

            printf("%s %s %s\n", inst_string[instruction].s, curr[string_index1].s, curr[string_index2].s);
        }
        else if (strcmp("second_best_path", inst_string[instruction].s) == 0)
        {
            int bank_name_index = rand() % size_bank_name;

            int string_index1 = rand() % size_currency;
            int string_index2 = rand() % size_currency;

            if (string_index1 == string_index2)
                string_index2 = (string_index2 + 1) % (size_currency);

            printf("%s %s %s\n", inst_string[instruction].s, curr[string_index1].s, curr[string_index2].s);
        }
        else if (strcmp("find_cycle", inst_string[instruction].s) == 0)
        {
            int bank_name_index = rand() % size_bank_name;
            printf("%s %s\n", inst_string[instruction].s, bank_name[bank_name_index].s);
        }
    }

    printf("exit\n");

    return 0;
}