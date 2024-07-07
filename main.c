#include "driverlib.h"
#include <stdio.h>
#include <stdlib.h>
#include "matrix_multiplication.h"
#include "AESoperations.h"

#pragma PERSISTENT(inputMatrixEncrypted)
uint8_t inputMatrixEncrypted[I_R][I_C] = { 0 };
#pragma PERSISTENT(weightMatrixEncrypted)
uint8_t weightMatrixEncrypted[I_C][W_C] = { 0 };
#pragma PERSISTENT(encryptedOutputMatrix)
uint8_t encryptedOutputMatrix[I_R][W_C] = { 0 };
#pragma PERSISTENT(isFRAMInitialized)
bool isFRAMInitialized = 0; // Flag to check if FRAM is initialized
#pragma PERSISTENT(cipherkey)
uint16_t cipherkey[LENGTH] = { 0 };

int main(void)
{
    WDT_A_hold(WDT_A_BASE);

    generateRandomKey(cipherkey);

    encryptedMatrix I_encrypted_ECB;
    encryptedMatrix W_encrypted_ECB;

//TESTING TILED MULTIPLICATION
    printf("*******TESTING TILED MULTIPLICATION*******\n");
    uint8_t I[I_R][I_C] = { 0 };
    uint8_t W[I_C][W_C] = { 0 };
    uint8_t O[I_R][W_C] = { 0 };
    Initialiaze_Matrix(I, I_R, I_C);
    Initialiaze_Matrix(W, I_C, W_C);
    Basic_Multiplication(I, W, O);
    printf("Result of multiplication is:\n");
    Print_Matrix(O, I_R, W_C);

//TESTING FRAM SAVINGS
//    printf("*******TESTING FRAM SAVINGS*******\n");
//    if (!isFRAMInitialized)
//    {
//        //Initialize FRAM here
//        //Only runs once after a full system reset or first time after programming
//        // Set the flag to indicate initialization is done
//        isFRAMInitialized = 1;
//        encryptAndStoreInFRAM(I, inputMatrixEncrypted, cipherkey);
//        encryptAndStoreInFRAM(W, weightMatrixEncrypted, cipherkey);
//        printf("Input matrix encrypted is:\n");
//        Print_Matrix(inputMatrixEncrypted, 4, 4);
//        printf("Weight matrix encrypted is:\n");
//        Print_Matrix(weightMatrixEncrypted, 4, 4);
//    }
//    else
//    {
//        // Normal operation, FRAM data is preserved
//        // You can safely read from inputMatrixEncrypted as it contains preserved data
//        printf("Data already initialized and saved in FRAM. These are the data:\nVariable isFRAMInitialized=%s\n",
//               isFRAMInitialized ? "true" : "false");
//        printf("Encrypted matrix is:\n");
//        Print_Matrix(inputMatrixEncrypted, 4, 4);
//        //decryptAndStoreInSRAM(inputMatrixEncrypted, decryptedMatrix, 4, 4,
//        //cipherkey);
//        printf("Decrypted matrix is:\n");
//        Print_Matrix(decryptedMatrix, 4, 4);
//    }

//TESTING ECB MODE
//     printf("*******TESTING ECB MODE*******\n");
    // size_t I_plaintext_size = sizeof(I);
    // I_encrypted_ECB.encryptedSize = ((I_plaintext_size + BLOCK_SIZE - 1)
    //         / BLOCK_SIZE) * BLOCK_SIZE;
    // printf("encrypted size: %d\n", I_encrypted_ECB.encryptedSize);

//     size_t W_plaintext_size = sizeof(W);
//     size_t W_encrypted_size = ((W_plaintext_size + BLOCK_SIZE - 1) / BLOCK_SIZE)
//             * BLOCK_SIZE;

//     //printf("Plaintext matrix I size: %d\n", I_plaintext_size);
    // I_encrypted_ECB.matrix = (uint8_t*) malloc(I_encrypted_ECB.encryptedSize);
//     //printf("Encrypted matrix I size: %d\n", I_encrypted_size);
//     uint16_t *I_decrypted_ECB = (uint8_t*) malloc(I_plaintext_size);

//     //printf("Plaintext matrix W size: %d\n", W_plaintext_size);
//     uint8_t *W_encrypted_ECB = (uint8_t*) malloc(W_encrypted_size);
//     //printf("Encrypted matrix W size: %d\n", W_encrypted_size);
//     uint16_t *W_decrypted_ECB = (uint8_t*) malloc(W_plaintext_size);

// if (I_encrypted_ECB == NULL || I_decrypted_ECB == NULL
//         || W_encrypted_ECB == NULL || W_decrypted_ECB == NULL)
    // if (I_encrypted_ECB.matrix == NULL)
    // {
    //     printf("Memory allocation failed\n");
    //     return 1;
    // }
    // printf("Memory allocation successful\n");

//     // Encrypt the matrix
//     I_encrypted_size = AES256_encryptMatrix_ECB(I, I_encrypted_ECB,
//                                                 I_plaintext_size);
//     // printf("Encrypted matrix I with ECB is:\n");
//     // Print_Matrix(I_encrypted_ECB, I_R, I_C);

//     W_encrypted_size = AES256_encryptMatrix_ECB(W, W_encrypted_ECB,
//                                                 W_plaintext_size);
//     // printf("Encrypted matrix W with ECB is:\n");
//     // Print_Matrix(W_encrypted_ECB, I_C, W_C);

//     // Decrypt the matrix
//     AES256_decryptMatrix_ECB(I_encrypted_ECB, I_decrypted_ECB, I_encrypted_size,
//                              I_plaintext_size);
//     // printf("Decrypted matrix with ECB is:\n");
//     // Print_Matrix(I_decrypted_ECB, I_R, I_C);
//     // printf("Input matrix == Decrypted matrix I: %s\n",
//     //        Matrix_Equality(I, I_decrypted_ECB, I_R, I_C) ? "true" : "false");

//     AES256_decryptMatrix_ECB(W_encrypted_ECB, W_decrypted_ECB, W_encrypted_size,
//                              W_plaintext_size);
//     // printf("Decrypted matrix with ECB is:\n");
//     // Print_Matrix(W_decrypted_ECB, I_C, W_C);

//     // printf("Weight matrix == Decrypted matrix W: %s\n",
//     //        Matrix_Equality(W, W_decrypted_ECB, I_C, W_C) ? "true" : "false");

//     // Free heap memory

// // TESTING TILED DECRYPTION
//     printf("*******TESTING TILED DECRYPTION*******\n");

//     printf("Encrypted input size: %d\n", I_encrypted_size);

//     uint8_t O_test[I_R][I_C] = { 0 };

//     Tiled_Decryption(I_encrypted_ECB, W_encrypted_ECB, O_test,
//                                    I_encrypted_size, W_encrypted_size);

//     // Free heap memory
//     free(I_decrypted_ECB);
//     free(W_encrypted_ECB);
//     free(W_decrypted_ECB);

    printf("*******TESTING NEW TILED TECHNIQUE*******\n");
    printf("Input matrix plaintext is:\n");
    Print_Matrix(I, I_R, I_C);
    I_encrypted_ECB = AES256_encryptMatrix_ECB(I, I_encrypted_ECB.matrix, I_R, I_C);
    printf("Weight matrix plaintext is:\n");
    Print_Matrix(W, I_C, W_C);
    W_encrypted_ECB = AES256_encryptMatrix_ECB(W, I_encrypted_ECB.matrix, I_C, W_C);

    uint8_t Oprova[ROUND_UP_TO_MULTIPLE_OF_4(I_R)][ROUND_UP_TO_MULTIPLE_OF_4(W_C)];

    Tiled_Decryption_Multiplication(I_encrypted_ECB, W_encrypted_ECB, Oprova);
    printf("Output matrix is:\n");
    Print_Matrix(Oprova, ROUND_UP_TO_MULTIPLE_OF_4(I_R), ROUND_UP_TO_MULTIPLE_OF_4(W_C));
    printf("Corrcet output matrix is:\n");  
    Print_Matrix(O, I_R, W_C);
    free(I_encrypted_ECB.matrix);
    free(W_encrypted_ECB.matrix);
    return 0;
}
