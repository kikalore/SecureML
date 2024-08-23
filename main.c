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

volatile unsigned int overflow_counter = 0;
int16_t cipherkey[LENGTH] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9,
                              0xA, 0xB, 0xC, 0xD, 0xE, 0xF, 0x0, 0x1, 0x2, 0x3,
                              0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD,
                              0xE, 0xF };
void setup_timerB()
{

    PM5CTL0 &= ~LOCKLPM5;
    TB0CTL |= TBCLR;
    TB0CTL |= TBSSEL__ACLK;
    TB0CTL |= MC__UP;
    TB0CCR0 = 50000;
    TB0CCTL0 |= CCIE;
    __enable_interrupt();
    TB0CTL &= ~TBIFG;
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void ISR_TB0_CCR0(void)
{
    overflow_counter++;
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    setup_timerB();
    __bis_SR_register(GIE);

    uint8_t I[I_R][I_C] = { 0 };
    uint8_t W[I_C][W_C] = { 0 };
    uint8_t O[I_R][W_C] = { 0 };
    uint8_t Otest[ROUND_UP_TO_MULTIPLE_OF_4(I_R)][ROUND_UP_TO_MULTIPLE_OF_4(W_C)];
    Initialiaze_Matrix(I, I_R, I_C);
    Initialiaze_Matrix(W, I_C, W_C);

//    unsigned int start_time_enc = TB0R;
//    unsigned int overflow_enc_start=overflow_counter;
    I_encrypted_ECB = AES256_encryptMatrix_ECB(I, I_encrypted_ECB.matrix, I_R,I_C);
//    unsigned int end_time_enc = TB0R;
//    unsigned int elapsed_time_enc;
//    unsigned int overflow_enc_end=overflow_counter;
//    unsigned int overflow_enc=overflow_enc_end-overflow_enc_start;
//    printf("Start time: %d, End time: %d, Overflow_enc: %d\n",start_time_enc, end_time_enc, overflow_enc);
//    if(start_time_enc>end_time_enc){
//        elapsed_time_enc = (TB0CCR0 - start_time_enc) + end_time_enc;
//    }else{
//        elapsed_time_enc = end_time_enc - start_time_enc;
//    }
//    unsigned long total_ticks_enc = ((unsigned long) overflow_enc * TB0CCR0) + elapsed_time_enc;
//    float time=(((float) total_ticks_enc / 32768.0) * 1000);
//    printf("Time to encrypt: %f ms\n",time);

    W_encrypted_ECB = AES256_encryptMatrix_ECB(W, I_encrypted_ECB.matrix, I_C,
    W_C);

//    unsigned int start_time_dec = TB0R;
//    unsigned int overflow_dec_start=overflow_counter;
//    AES256_decryptMatrix_ECB(I_encrypted_ECB.matrix, I_dec, sizeof(I_dec),sizeof(I_dec));
//    unsigned int end_time_dec = TB0R;
//    unsigned int overflow_dec_end=overflow_counter;
//    unsigned int overflow_dec=overflow_dec_end-overflow_dec_start;
////    printf("Start time: %d, End time: %d, Overflow_dec: %d\n",start_time_dec, end_time_dec, overflow_dec);
//    unsigned int elapsed_time_dec;
//    if(start_time_dec>end_time_dec){
//        elapsed_time_dec = (TB0CCR0 - start_time_dec) + end_time_dec;
//    }else{
//        elapsed_time_dec = end_time_dec - start_time_dec;
//    }
//    unsigned long total_ticks_dec = ((unsigned long) overflow_dec * TB0CCR0) + elapsed_time_dec;
//    float time_in_seconds_dec = ((float) total_ticks_dec / 32768.0);
//    printf("Time to decrypt %f ms\n", time_in_seconds_dec * 1000);

    O_encrypted_ECB = Tiled_Decryption_Multiplication(I_encrypted_ECB, W_encrypted_ECB, Otest);

    free(I_encrypted_ECB.matrix);
    free(W_encrypted_ECB.matrix);
    free(O_encrypted_ECB.matrix);

//    printf("Overflow totali: %d\n", overflow_counter);
//    unsigned int end_time = TB0R;
//    unsigned long total_ticks = ((unsigned long) overflow_counter * TB0CCR0)+ end_time;
//    float time_in_seconds = ((float) total_ticks / 32768.0);
//    printf("Total time: %f ms\n", time_in_seconds * 1000);

    return 0;
}
