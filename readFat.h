#ifndef _READ_FAT_H_
#define _READ_FAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define BYTE_PER_ENTRY 32U
#define END_CLUSTER 0x0FFF
#define CLUSTER_OFFSET 2
// kieu no rong la gi
typedef enum {   
    File =0x00,
    Folder =0x10,
    Long_file_name =0x0F,
    Entry_att = 0x0b,
}AttributeState_t;




typedef struct{
	uint8_t Name[8];// 8bytes
	uint8_t Extension[3];//3bytes
	uint8_t Attributes;//1byte
	uint8_t Reserved[10];//10bytes
	uint16_t Time;//2bytes
	uint16_t Date;//2bytes
	uint16_t Cluster;//2bytes
	uint32_t Size;//4bytes	
}Entry_Data_t;

typedef enum{
	offset_Extension = 0x08,
	offset_Time = 0x16,
	offset_Date = 0x18,
	offset_Size = 0x1C,
	offset_At = 0x0b,
	offset_SCluster = 0x1A,
}Offset_entry;

typedef enum{
	Number_of_Fat = 0x10,
	Blocks_occupied_by_FAT = 0x16,
	Num_byte_of_block = 0x0b,
	Total_entry = 0x11,
}Offset_t;

typedef struct{
    uint8_t FAT;
    uint16_t NumBlock_Fat;
    uint32_t rootdirectory;
    uint16_t Byte_per_block;
    uint16_t Start_Fat;
    uint16_t Total_entry;
    uint32_t Data_region;
}StartPosition_t;


typedef enum{
	Can_not_Open,
	Open,
	True,
	End,
	Input_error,
}Status_t;
Status_t status;

void calStartPosition();
void Handle(uint8_t user_input, uint32_t *start);
void Read_root();
Status_t Check_ip(uint8_t IP_u, uint32_t IP_adr);

#endif
