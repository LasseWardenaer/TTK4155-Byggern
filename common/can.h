#ifndef COMMON_CAN_ID_H
#define COMMON_CAN_ID_H


#include <stdint.h>


typedef struct can_message_t {
	uint16_t id;
	char data_length;
	char data[8];
} CAN_MESSAGE;


typedef enum {
    USER_INPUT_ID = 1,
    FSM_STATE_ID,
    GAME_LIVES_LEFT_ID,
    CONTROLLER_ID,
    GAME_SCORE_ID,
    MUSIC_SONG_ID,
    GAME_DIFFICULTY_ID
} CAN_MESSAGE_ID;


#endif