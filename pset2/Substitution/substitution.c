#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    
    //string character count
    int s = strlen(argv[1]);
    if (s < 26)
    {
        printf("Key must contain 26 characters\n");
        return 1;
    }
    else if (s > 26)
    {
        printf("Key must contain 26 characters\n");
        return 1;
    }
    
    //giving each character a value
    char value[26];
    for (int j = 0; j < 26 ; j++) // argv[1][j] != 0
    {
        if (argv[1][j] >= 65 && argv[1][j] <= 90) // if character is MAYUS
        {
            value [j] = argv[1][j];
            
            //check if character is already assigned
            for (int i = 0; i < j; i++)
            {
                 int h = value[j] - argv[1][i];
                 if (h == 0)
                 {
                     printf("characters must be unique\n");
                     return 1;
                 }
            }
        }    
        else if (argv[1][j] >= 97 && argv[1][j] <= 122) // if character is minus
        {
            value [j] = argv[1][j] - 32;
            
            //check if character is already assigned
            for (int i = 0; i < j; i++)
            {
                 int h = value[j] - argv[1][i];
                 if (h == 0)
                 {
                     printf("characters must be unique\n");
                     return 1;
                 }
            }
        }
        else // if character is not an ABC char 
        {
            printf("value must be alphabetic\n");
            return 1;
        }
    }
    
    
    //ask user for text
    string text = get_string("plaintext:");
    
    //sending encoded message
    printf("ciphertext: ");
    int m = strlen(text);
    for (int k = 0; k < m; k++)
    {
        if (text[k] >= 65 && text[k] <= 90) //changing mayus characters
        {
            int l = text[k] - 65;
            text[k] = value[l];
            printf("%c", text[k]);
        }
        else if (text[k] >= 97 && text[k] <= 122)//changing minus chacacters
        {
            int l = text[k] - 97;
            text[k] = value[l] + 32;
            printf("%c", text[k]);        
        }
        else //not changing alphabetic characters
        {
            printf("%c", text[k]);
        }
    }
    printf("\n");
    return 0;
}