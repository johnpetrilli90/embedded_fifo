/** 
 * @file cmd_fifo_test.c
 * @brief unit test for the command fifo
*/

#include "cmd_fifo.h"
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

const char* one_char_command = " ";
const char* command_reg[] = {"status", "readreg1", "writereg3 50",
                            "idn", "getval", "setval", "getcurrent",
                            "getvolt"};
const char* too_many_char_cmd = "123456789101234567890";

int main(void){
    // Creating fifo instance
    fifo_handle_t fifo;

    // initializing the fifo
    fifo = fifo_init();
    printf("fifo created\r\n");

    //filling the fifo up with commands
    printf("pushing the max number of commands (%d) onto fifo\r\n", FIFO_MAX_NUM_CMDS);
    for(uint8_t i=0; i<FIFO_MAX_NUM_CMDS; i++){
        assert(fifo_push(fifo, (uint8_t*)command_reg[i], strlen(command_reg[i])));
        printf("[%s] added to fifo \r\n", command_reg[i]);
    }
    printf("Test Passed: the fifo count is %d \r\n", fifo_count(fifo));

    // Tyring to push too many commands onto fifo
    bool result = fifo_push(fifo, (uint8_t*)one_char_command, strlen(one_char_command));
    if(result){
        printf("Test Failed: added item to fifo.\r\n");
    }
    else{
        printf("Test Passed: item not added to fifo.\r\n");
    }

    // checking fifo full 
    printf("testing fifo full function...\r\n");
    result = fifo_full(fifo);
    if(!result){
        printf("Test Failed: fifo should read full.\r\n");
    }
    else{
        printf("Test Passed: fifo read full as expected.\r\n");
    }

    // checking fifo empty 
    printf("testing fifo empty function...\r\n");
    result = fifo_empty(fifo);
    if(result){
        printf("Test Failed: fifo should not read empty.\r\n");
    }
    else{
        printf("Test Passed: fifo did not read empty.\r\n");
    }

    // testing fifo pop
    printf("popping all the commands off the fifo\r\n");
    uint8_t pop_array[FIFO_MAX_CMD_SIZE];
    for(uint8_t i=0; i<FIFO_MAX_NUM_CMDS; i++){

        assert(fifo_pop(fifo, pop_array, FIFO_MAX_CMD_SIZE));
        printf("[%s] command popped off the fifo \r\n", pop_array);
    }
    printf("Test Passed: the fifo count is %d \r\n", fifo_count(fifo));

    // Trying to push too many commands onto fifo
    result = fifo_pop(fifo, pop_array, FIFO_MAX_CMD_SIZE);
    if(result){
        printf("Test Failed: popped garbage off the fifo\r\n");
    }
    else{
        printf("Test Passed: fifo pop stayed within bounds.\r\n");
    }

     // checking fifo full 
    printf("testing fifo full function...\r\n");
    result = fifo_full(fifo);
    if(result){
        printf("Test Failed: fifo should not read full.\r\n");
    }
    else{
        printf("Test Passed: fifo did not read full.\r\n");
    }

     // checking fifo empty 
    printf("testing fifo empty function...\r\n");
    result = fifo_empty(fifo);
    if(!result){
        printf("Test Failed: fifo should read empty.\r\n");
    }
    else{
        printf("Test Passed: fifo read empty as expected.\r\n");
    }

    // try to add too long of a command to the fifo
    result = fifo_push(fifo, (uint8_t*)too_many_char_cmd, strlen(too_many_char_cmd));
    if(result){
        printf("Test Failed: fifo added command out of bounds.\r\n");
    }
    else{
        printf("Test Passed: fifo rejected long command.\r\n");
    }

    // releasing the fifo from dynamic memory allocation
    fifo_free(fifo);
    printf("fifo destroyed\r\n");

    return 0;
}