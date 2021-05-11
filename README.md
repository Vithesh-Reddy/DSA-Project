
# Compilation Instructions:

Run the following commands on the terminal:

    gcc user_main.c
    ./a.out
    <instruction>

# Compilation instructions for input generation:

Run the following commands on the terminal:

    gcc input_generator.c
    ./a.out > gen_input.txt
   
# The functions in our program are:

## 1. To add a trade bank:
  - input : add_trade_bank <bankname (string)>
  - output : 

   - case 1: If a trade bank with the same name already exists

           Trade Bank already exists
           Done

   - case 2: If no trade bank exists with the same name

             Done

## 2. To delete a trade bank:
  - input : delete_trade_bank <bankname (string)>
  - output : 

   - Case 1: When trade bank with given name does not exist
     
             THERE IS NO TRADEBANK WITH NAME <bankname (string)>
             Done

   - Case 2: When trade bank with given name exists
     
             Done
     
## 3. To add a currency:
  - input : add_currency <bankname (string)> <currencyname (string)>
  - output :

   - Case 1: When trade bank with given name does not exist
            
             Trade Bank not found
             Done

   - Case 2: If a currency with the same name already exists in the given trade bank

             Currency already exists
             Done

   - Case 3: If a currency with the same name doesn't exist in the given trade bank      

             Done

 ## 4. To delete a currency
   - input : delete_currency <bankname (string)> <currencyname (string)>
   - output :

   - Case 1: When trade bank with given name does not exist
            
             Trade Bank not found
             Done

   - Case 2: If a currency with the same name doesn't exist in the given trade bank

             Currency in the given trade bank not found
             Done

   - Case 3: If a currency with the same name exists in the given trade bank      

             Done         

 ## 5. To add conversion between two currencies
   - input : add_conversion <bankname (string)> <sourcecurrency (string)> <destinationcurrency (string)> <weight (long int)>
   - output :

   - Case 1: When trade bank with given name does not exist
            
             Trade Bank not found
             Done

   - Case 2: If source currency doesn't exist in the given trade bank

             Source Currency not found
             Done

   - Case 3: If destination currency doesn't exist in the given trade bank

             Destination Currency not found
             Done         

   - Case 4: If source and destination currencies are present in the given trade bank

             Done

 ## 6. To delete conversion between two currencies         
   - input : delete_conversion <bankname (string)> <sourcecurrency (string)> <destinationcurrency (string)>
   - output :

   - Case 1: When trade bank with given name does not exist
            
             Trade Bank not found
             Done

   - Case 2: If source currency doesn't exist in the given trade bank

             Source Currency not found
             Done

   - Case 3: If conversion doesn't exist between source and destination currencies in the given trade bank

             Conversion does'nt exist
             Done         

   - Case 4: If conversion exists between source and destination currencies in the given trade bank

             Done

## 7. To print a trade bank
  - input : print_trade_bank <bankname (string)>
  - output :

   - Case 1: When trade bank with given name does not exist
            
             The given trade bank doesn't exist
             
   - Case 2: When trade bank with given name exists

             Trade Bank : <bankname (string)>

             <currencyname1 (string)>
                                         To currency <currencydestination> with conversion rate <destinationweight>
                                         .
                                         .
                                         .
             <currencyname2 (string)>
                                         To currency <currencydestination> with conversion rate <destinationweight>
                                         .
                                         .
                                         .
             .
             .
             .

## 8. To print best path between two currencies
  - input : best_path <sourcecurrency (string)> <destinationcurrency (string)>
  - output : 

   - Case 1: When path between the source to destination currencies is not available 
         
             No path available from <sourcecurrency (string)> to <destinationcurrency (string)>

   - Case 2: When path between the source to destination currencies is available 

             The best path from <sourcecurrency> to <destinationcurrency> is : <sourcecurrency> -> ... -> <destinationcurrency>
             The cost of best path is <shortestpathlength (long int)> : from the trade bank <bestbankname (string)>

## 9. To print second best path between two currencies
  - input : second_best_path <sourcecurrency (string)> <destinationcurrency (string)>
  - output : 

   - Case 1: When second best path between the source to destination currencies is not available 
         
             No 2nd best path available from <sourcecurrency (string)> to <destinationcurrency (string)>

   - Case 2: When second best path between the source to destination currencies is available 

             The 2nd best path from <sourcecurrency> to <destinationcurrency> is : <sourcecurrency> -> ... -> <destinationcurrency>
             The cost of 2nd best path is <shortestpathlength (long int)> : from the trade bank <bestbankname (string)>

## 10. To find whether a given trade bank has a cycle or not
  - input : find_cycle <bankname (string)>
  - output :

   - Case 1: When no cycle exists in the given trade bank

             There is no cycle available in the given trade bank

   - Case 2: When there exists a cycle in the given trade bank

             The cycle is: <currency1 (string)> <currency2 (string)> . . .

## 11. To exit the program
  - input : exit
  - output : Have a nice day!

# Limitations of our code

1. We can't add a conversion which has a conversion rate greater than infinity.
2. We can't add more than 10^6 trade banks, because that exceeds the memory limit of the stack on the local machine.
   (The process got killed when we tried doing so)

    
        