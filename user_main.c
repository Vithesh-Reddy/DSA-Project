// This is the driver program to run the Currency Exchange Manager

#include "currency.h"
#include "currency.c"

int main()
{
    graph g = initialize_graph(hash_table_size); // initialize a graph to hold the hash table of the trade banks

    printf("Welcome to Currency Exchange manager. You can perform the following operations:\n\nadd_trade_bank (name)\ndelete_trade_bank (name)\nadd_currency (bank name) (currency name)\ndelete_currency (bank name) (currency name)\nadd_conversion (bank name) (src currency name) (dest currency name) (cost)\ndelete_conversion (bank name) (src currency name) (dest currency name)\nprint_trade_bank (name)\nprint_currency (bank name) (currency name)\nbest_path (src currency name) (dest currency name)\nsecond_best_path (src currency name) (dest currency name)\nfind_cycle (bank name)\nexit\n\n");

    // char arrays to read and store the names of the various instructions, trade banks and currencies
    char operation[100];
    char bank_name[100];
    char currency_name[100];
    char currency_name_dest[100];
    length wt = 0;
    while (1)
    {
        // read the operation (eg. add_trade_bank)
        scanf("%s", operation);
        getchar();

        // compare the operation entered by the user with the possible operations and redirect control flow to the 
        // corresponding code block

        if (strcmp("add_trade_bank", operation) == 0) 
        {
            scanf("%s", bank_name);
            getchar();
            add_trade_bank(g, bank_name);
            printf("Done\n");
        }
        else if (strcmp("delete_trade_bank", operation) == 0)
        {
            scanf("%s", bank_name);
            getchar();
            delete_trade_bank(g, bank_name);
            printf("Done\n");
        }
        else if (strcmp("add_currency", operation) == 0)
        {
            scanf("%s", bank_name);
            getchar();
            scanf("%s", currency_name);
            getchar();
            add_currency(g, bank_name, currency_name);
            printf("Done\n");
        }
        else if (strcmp("delete_currency", operation) == 0)
        {
            scanf("%s", bank_name);
            getchar();
            scanf("%s", currency_name);
            getchar();
            delete_currency(g, bank_name, currency_name);
            printf("Done\n");
        }
        else if (strcmp("add_conversion", operation) == 0)
        {
            scanf("%s", bank_name);
            getchar();
            scanf("%s", currency_name);
            getchar();
            scanf("%s", currency_name_dest);
            getchar();
            scanf("%ld", &wt);
            getchar();
            add_conversion(g, bank_name, currency_name, currency_name_dest, wt);
            printf("Done\n");
        }
        else if (strcmp("delete_conversion", operation) == 0)
        {
            scanf("%s", bank_name);
            getchar();
            scanf("%s", currency_name);
            getchar();
            scanf("%s", currency_name_dest);
            getchar();
            delete_conversion(g, bank_name, currency_name, currency_name_dest);
            printf("Done\n");
        }
        else if (strcmp("print_trade_bank", operation) == 0)
        {
            scanf("%s", bank_name);
            getchar();
            printf("\n");
            print_trade_bank(g, bank_name);
            printf("\n");
        }

        else if (strcmp("print_currency", operation) == 0)
        {
            scanf("%s", bank_name);
            getchar();
            scanf("%s", currency_name);
            getchar();
            printf("\n");
            print_currency(g, bank_name, currency_name, hash_table_size);
        }
        else if (strcmp("best_path", operation) == 0)
        {
            scanf("%s", currency_name);
            getchar();
            scanf("%s", currency_name_dest);
            getchar();
            best_path(g, currency_name, currency_name_dest);
        }
        else if (strcmp("second_best_path", operation) == 0)
        {
            scanf("%s", currency_name);
            getchar();
            scanf("%s", currency_name_dest);
            getchar();
            second_best_path(g, currency_name, currency_name_dest);
        }
        else if (strcmp("find_cycle", operation) == 0)
        {
            scanf("%s", bank_name);
            getchar();
            Find_cycle(g, bank_name);
        }
        else if (strcmp("exit", operation) == 0)
        {
            printf("\nHave a nice day!\n");
            exit(0);
        }
        else
        {
            // print an error message if the entered operation is incorrect
            printf("Invalid Operation. Please try again\n");
        }
    }
    return 0;
}