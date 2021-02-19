#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Variables for counting
    float letter = 0; float word = 1; float sentence = 0;
    
    //promp user for a text
    string achr = get_string("Text: ");
    
    //analize each ascii character
    for (int j = 0; achr [j] != 0; j++) // each charcacter of the string is analized by doing achr[j]
    {
        //letter count
        if ((65 <= achr[j] && achr[j] <= 90)|| (97 <= achr[j] && achr[j] <= 122)) //by conditioning it between numbers (intergers) the program deduces that achr must be regarded as it's numeric value
        {
            letter++;
        }
        //word count
        else if (achr[j] == 32 && achr[j+1] != 32)
        {
            word++;
        }
        //sentence count
        else if (achr[j] == 46 || achr[j] == 33 || achr[j] == 63) 
        {
            sentence++;
        }
    }
    //to ensure at least one sentence is counted
    if (sentence == 0)
        {
            sentence++;
        }
    //calculating the algorithm
    float L = (100 * letter) / word; //avarage letters for 100 words
    float S = (100 * sentence) / word; //avarage sentences for 100 words
    
    float grade = 0.0588 * L - 0.296 * S - 15.8;
        

    if (grade >= 16)  //if grade is 16 or higher
    {
        printf("Grade 16+\n");
    }
    else if (grade <= 1)  //if grade is 1 or lower
    {
        printf("Before Grade 1\n");
    }
    else  //if grade is beetwen 2 and 15
    {
    printf("Grade %.f\n", grade);
    }
}