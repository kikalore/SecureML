#include "driverlib.h"
#include <stdio.h>
#include <stdlib.h>
#include "matrix_multiplication.h"
#include "AESoperations.h"

#pragma PERSISTENT(I_encrypted_ECB)
encryptedMatrix I_encrypted_ECB;
#pragma PERSISTENT(W_encrypted_ECB)
encryptedMatrix W_encrypted_ECB;
#pragma PERSISTENT(O_encrypted_ECB)
encryptedMatrix O_encrypted_ECB;
#pragma PERSISTENT(isFRAMInitialized)
bool isFRAMInitialized = 0; // Flag to check if FRAM is initialized
#pragma PERSISTENT(cipherkey)
uint16_t cipherkey[LENGTH] = { 0 };
volatile unsigned int overflow_counter = 0;

void setup_timerB()
{
    //setup ports->P1.0 to an output
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    PM5CTL0 &= ~LOCKLPM5;
    //setup timer
    TB0CTL |= TBCLR; //reset timer
    TB0CTL |= TBSSEL__ACLK; //clock source ACLK
    TB0CTL |= MC__CONTINUOUS;
    //Setup TB0 overflow IRQ
    TB0CTL |= TBIE;
    __enable_interrupt();
    TB0CTL &= ~TBIFG;
}

// Interrupt service routine for Timer B overflow
#pragma vector=TIMER0_B1_VECTOR
__interrupt void ISR_TB0_Overflow(void)
{
    P1OUT ^= BIT0;
    printf("overflow\n");
    overflow_counter++;
    TB0CTL &= ~TBIFG;
}
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer
    setup_timerB();            // Set up Timer B
    __bis_SR_register(GIE);    // Enable global interrupts

    unsigned int start_time = TB0R;
    printf("Start time: %d\n", start_time);

    // Simulation of work (like encryption, decryption, etc.)
    // volatile unsigned long i ;
    // for (i= 0; i < 1000000; i++);
    //     //generateRandomKey(cipherkey);

// // //TESTING TILED MULTIPLICATION
    printf("*******TESTING TILED MULTIPLICATION*******\n");
    uint8_t I[I_R][I_C] = { 0 };
    uint8_t W[I_C][W_C] = { 0 };
    uint8_t O[I_R][W_C] = { 0 };
    Initialiaze_Matrix(I, I_R, I_C);
    Initialiaze_Matrix(W, I_C, W_C);

    Basic_Multiplication(I, W, O);
    // printf("Result of multiplication is:\n");

    // Print_Matrix(O, I_R, W_C);

    // printf("*******TESTING NEW TILED TECHNIQUE*******\n");

    // printf("Input matrix plaintext is:\n");

    // Print_Matrix(I, I_R, I_C);

    I_encrypted_ECB = AES256_encryptMatrix_ECB(I, I_encrypted_ECB.matrix, I_R,
                                               I_C);
    // printf("Weight matrix plaintext is:\n");
    // Print_Matrix(W, I_C, W_C);
    W_encrypted_ECB = AES256_encryptMatrix_ECB(W, I_encrypted_ECB.matrix, I_C,
                                               W_C);
    uint8_t Otest[ROUND_UP_TO_MULTIPLE_OF_4(I_R)][ROUND_UP_TO_MULTIPLE_OF_4(W_C)];

    O_encrypted_ECB = Tiled_Decryption_Multiplication(I_encrypted_ECB,
                                                      W_encrypted_ECB, Otest);
    //printf("Correct output matrix is:\n");
    //Print_Matrix(O, I_R, W_C);

    printf("Output matrix encrypted is:\n");
    //Print_Matrix(O_encrypted_ECB.matrix, O_encrypted_ECB.matrixRows, O_encrypted_ECB.matrixCols);
    decryptAndStoreInSRAM(O_encrypted_ECB.matrix, Otest, cipherkey);

    unsigned int end_time = TB0R;
    printf("End time: %d\n", end_time);

    unsigned int elapsed_time;
    if (end_time >= start_time)
    {
        // No overflow
        elapsed_time = end_time - start_time;
    }
    else
    {
        // Overflow has occurred
        elapsed_time = (0xFFFF - start_time) + end_time + 1;
    }

    printf("Elapsed time: %u\n", elapsed_time);

    // Calculate total ticks
    unsigned long total_ticks = ((unsigned long)overflow_counter * 65536UL) + elapsed_time;

    // Convert ticks to seconds
    float time_in_seconds = (float)total_ticks / 32768.0;

    printf("Number of overflow: %d\n", overflow_counter);
    printf("Total ticks: %lu\n", total_ticks);
    printf("Total time: %f seconds\n", time_in_seconds);
    free(I_encrypted_ECB.matrix);
    free(W_encrypted_ECB.matrix);
    free(O_encrypted_ECB.matrix);

    return 0;
}


// int main(void)
// {
//     WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer
//     // Set up Timer A
//     setup_timerB();
//     // Enable global interrupts
//     __bis_SR_register(GIE);


//     unsigned int start_time = TB0R;
//     printf("Start time: %d\n", start_time);
    
    
//     //generateRandomKey(cipherkey);

// // // //TESTING TILED MULTIPLICATION
//     printf("*******TESTING TILED MULTIPLICATION*******\n");
//     uint8_t I[I_R][I_C] = { 0 };
//     uint8_t W[I_C][W_C] = { 0 };
//     uint8_t O[I_R][W_C] = { 0 };
//     Initialiaze_Matrix(I, I_R, I_C);
//     Initialiaze_Matrix(W, I_C, W_C);
//     __no_operation();

//     Basic_Multiplication(I, W, O);
//     printf("Result of multiplication is:\n");
//         __no_operation();

//     Print_Matrix(O, I_R, W_C);
//         __no_operation();


//     printf("*******TESTING NEW TILED TECHNIQUE*******\n");
//         __no_operation();

//     printf("Input matrix plaintext is:\n");
//         __no_operation();

//     Print_Matrix(I, I_R, I_C);
//         __no_operation();


//     I_encrypted_ECB = AES256_encryptMatrix_ECB(I, I_encrypted_ECB.matrix, I_R,
//                                                I_C);
//     printf("Weight matrix plaintext is:\n");
//     Print_Matrix(W, I_C, W_C);
//     W_encrypted_ECB = AES256_encryptMatrix_ECB(W, I_encrypted_ECB.matrix, I_C,
//                                                W_C);
//     uint8_t Otest[ROUND_UP_TO_MULTIPLE_OF_4(I_R)][ROUND_UP_TO_MULTIPLE_OF_4(W_C)];

//     O_encrypted_ECB = Tiled_Decryption_Multiplication(I_encrypted_ECB,
//                                                       W_encrypted_ECB, Otest);
//     printf("Correct output matrix is:\n");
//     Print_Matrix(O, I_R, W_C);
//     int i;
//     for ( i = 0; i < 1000; i++)
//     {
//         __no_operation();
//     }
    

//     printf("Output matrix encrypted is:\n");
//     Print_Matrix(O_encrypted_ECB.matrix, O_encrypted_ECB.matrixRows,
//                  O_encrypted_ECB.matrixCols);
//     decryptAndStoreInSRAM(O_encrypted_ECB.matrix, Otest, cipherkey);

//     unsigned int end_time = TB0R;
//     printf("End time: %d\n", end_time);
//     printf("Number of overflow: %d\n", overflow_counter);
//     unsigned int elapsed_time = end_time - start_time;
//     printf("Elapsed time: %d\n", elapsed_time);
//     unsigned long total_ticks = ((unsigned long)overflow_counter * 65536UL) + end_time - start_time;

// //converts ticks to seconds
//     float time_in_seconds = (float)total_ticks / 32768.0;

//     printf("Total ticks: %d\n", total_ticks);
//     printf("Total time: %f seconds\n", time_in_seconds);
// // //     free(I_encrypted_ECB.matrix);
// // //     free(W_encrypted_ECB.matrix);
// // //     free(O_encrypted_ECB.matrix);
//     return 0;
// }
