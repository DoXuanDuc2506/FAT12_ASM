#include"readFat.h"
#include"Support.h"

static FILE *filePtr = NULL;
static Entry_Data_t Entry_Data = {0};
StartPosition_t Sp_data = {0};
//open file
Status_t OpenFile(char * fileName){
	filePtr = fopen(fileName, "rb");
	if (filePtr == NULL){
		status = Can_not_Open;
	}else{
		status = Open;
	}
	return status;
}


//ham lay data tai dia chi can lay
static void Read_Add(uint32_t Add, uint8_t sizeData, void *Data){
	fseek(filePtr ,Add, SEEK_SET);
	fread(Data, sizeData, 1, filePtr);
}
// tinh cac dia chi bat dau

void calStartPosition(){
	//so luong bang fat
	Read_Add(Number_of_Fat, sizeof(Sp_data.FAT), &Sp_data.FAT);
	// so block cua fat
	Read_Add(Blocks_occupied_by_FAT, sizeof(Sp_data.NumBlock_Fat), &Sp_data.NumBlock_Fat);
	// so byte cua moi block
	Read_Add(Num_byte_of_block, sizeof(Sp_data.Byte_per_block), &Sp_data.Byte_per_block);
	// dia chi root bat dau
	Sp_data.rootdirectory = (Sp_data.FAT*Sp_data.NumBlock_Fat +1)*Sp_data.Byte_per_block;
	Sp_data.Start_Fat = 0x200;
	Read_Add(Total_entry, sizeof(Sp_data.Total_entry), &Sp_data.Total_entry);
	Sp_data.Data_region = Sp_data.rootdirectory + (Sp_data.Total_entry * BYTE_PER_ENTRY);
}

// doc entry
Entry_Data_t readEntry(uint32_t adrCurrent){
	Read_Add(adrCurrent, 8, &Entry_Data.Name);
	Read_Add(adrCurrent + offset_Extension, 3, &Entry_Data.Extension);
	Read_Add(adrCurrent + offset_Time, 2, &Entry_Data.Time);
	Read_Add(adrCurrent + offset_Date, 2, &Entry_Data.Date);
	Read_Add(adrCurrent + offset_Size, 4, &Entry_Data.Size);
	Read_Add(adrCurrent + offset_SCluster, 2, &Entry_Data.Cluster);
	Read_Add(adrCurrent + offset_At, 1, &Entry_Data.Attributes);
	return Entry_Data;
}


// kiem tra entry
static Status_t Check_Entry(uint32_t Add, uint8_t Cur) {
    uint8_t value_At_Addroot;
    Read_Add(Add, sizeof(value_At_Addroot), &value_At_Addroot);
    if (value_At_Addroot == 0x00 && Cur == 0x00) {
        status =  End; 
    } else {
        status = True; 
    }
    return status;
}

static uint8_t Re_idx(uint32_t re_Adr_t){
	uint8_t re_At;
	uint8_t re_ip =1;
		do{
		Read_Add(re_Adr_t + 0xb, sizeof(re_At), &re_At);
		status = Check_Entry(re_Adr_t, re_At);
		if(status == End){ 
        break;
    	}
		switch(re_At){
			case File:
			case Folder:
				re_Adr_t +=BYTE_PER_ENTRY;
				re_ip ++;
				break;
			case Long_file_name:
				re_Adr_t += BYTE_PER_ENTRY;
				break;
		}		
	}
	while(True);
	return re_ip - 1;
}

void Read_root(){
	uint32_t Base_adr = Sp_data.rootdirectory;
	uint8_t base_at;
	Entry_Data_t base;
	uint8_t base_id =1;
		do{
		Read_Add(Base_adr + 0xb, sizeof(base_at), &base_at);
		status = Check_Entry(Base_adr, base_at);
		if(status == End){ 
        break;
    	}
		switch(base_at){
			case File:
				base = readEntry(Base_adr);
				printEntry(base_id, base);
				Base_adr +=BYTE_PER_ENTRY;
				base_id ++;
				break;
			case Folder:
				base = readEntry(Base_adr);
				printEntry(base_id, base);
				Base_adr +=BYTE_PER_ENTRY;
				base_id ++;
				break;
			case Long_file_name:
				Base_adr += BYTE_PER_ENTRY;
				break;
		}		
	}
	while(True);
}



static uint8_t Print_At_Adr(uint32_t ip_Adr_t){
	uint32_t IP_ADT = ip_Adr_t;
	uint8_t ip_At;
	Entry_Data_t ET_t2;
	uint8_t idx_ip =1;
		do{
		Read_Add(IP_ADT + 0xb, sizeof(ip_At), &ip_At);
		status = Check_Entry(IP_ADT, ip_At);
		if(status == End){ 
        break;
    	}
		switch(ip_At){
			case File:
				ET_t2 = readEntry(IP_ADT);
				printEntry(idx_ip, ET_t2);
				IP_ADT +=BYTE_PER_ENTRY;
				idx_ip ++;
				break;
			case Folder:
				ET_t2 = readEntry(IP_ADT);
				printEntry(idx_ip, ET_t2);
				IP_ADT +=BYTE_PER_ENTRY;
				idx_ip ++;
				break;
			case Long_file_name:
				IP_ADT += BYTE_PER_ENTRY;
				break;
		}		
	}
	while(True);
}

Status_t Check_ip(uint8_t IP_u, uint32_t IP_adr){ 
	uint8_t IP1;
	IP1 = Re_idx(IP_adr);
	if(IP_u> IP1 ){
		status = Input_error;
	}else{
		status = True;
	}
	return status;
}

static uint32_t User_input(uint8_t U_ip, uint32_t Now_Adr) {
	uint8_t idx_ip2 = 0; // B?t d?u t? 0 vì chúng ta dang tính t? entry th? 0
	uint32_t next_Adr = Now_Adr;
	uint8_t a = Re_idx(Now_Adr);
	Check_ip(U_ip, Now_Adr);
	if(status == True){
		if (U_ip <= a) {
			while (idx_ip2 != U_ip) {
				uint8_t c;
				Read_Add(next_Adr + offset_At, sizeof(c), &c);
				if (c == File || c == Folder) { // Ki?m tra ch? khi là file ho?c folder
					idx_ip2++; // Tang ch? s? sau khi ki?m tra
				}
				if (idx_ip2 != U_ip) {
					next_Adr += BYTE_PER_ENTRY; // Ch? tang next_Adr n?u chua d?t du?c entry c?n tìm
				}
			}
		} 		
	}	
	return next_Adr;							
}

static uint16_t fat12_cluster_tracing(uint16_t start)
{
	uint8_t buffer[4] = {0};
	uint32_t addr2 = Sp_data.Start_Fat + ((start%2 == 0) ? (start*1.5f) : (start*1.5f-1));
	fseek(filePtr,addr2,SEEK_SET);
	fgets((char*)buffer,sizeof(buffer),filePtr);
	uint16_t first_entry = (uint8_t)(buffer[1]&0x0f)*16*16+buffer[0]; /* get the first entry */
	uint16_t second_entry = (buffer[2]*16+(buffer[1]>>4)); /* get the seccond entry */

	if(start%2 == 0){
		return first_entry;
	}
	else return second_entry;
}

// ham in data region
static void access_file(uint16_t cluster,uint32_t fsize)
{
	//file data cannot storage anywhere else except data region 
	uint32_t addr = 0;
	uint16_t idx_af;
	//process data
	uint16_t next = cluster;
	uint32_t size = fsize;
	uint32_t offset = 0; //0 reach to fsize
	while((next != END_CLUSTER)&&(size>0)){
		//determine addr and offset, goto addr and get offset data
		addr = (Sp_data.Data_region + (next-CLUSTER_OFFSET)*Sp_data.Byte_per_block);
		offset = (size > Sp_data.Byte_per_block) ? Sp_data.Byte_per_block : size;
		fseek(filePtr,addr,SEEK_SET);
		for(idx_af = 0; idx_af < Sp_data.Byte_per_block; idx_af++){
			printf("%c", fgetc(filePtr));
		}
		//end loop condition
		size-= offset;
		next = fat12_cluster_tracing(next);
//		printf("%d next cluster %02X\n",size, next); 
	}
}


void Handle(uint8_t user_input, uint32_t *start) {
    static uint8_t first_call = 1; // Bi?n d? xác d?nh l?n d?u tiên g?i hàm
    static uint32_t previous_start = 0; // Bi?n d? luu tr? giá tr? start tru?c dó

    uint16_t ST_cluster,aaa;
    Entry_Data_t Entry_st;

    if (first_call) {
        *start = Sp_data.rootdirectory;
        first_call = 0;
    }

    uint32_t start_t = User_input(user_input, *start);
    Entry_st = readEntry(start_t);
    ST_cluster = Entry_st.Cluster;
    if (Entry_st.Attributes == File) {
		access_file(ST_cluster, Entry_st.Size);
		ST_cluster = fat12_cluster_tracing(ST_cluster);
    	display();
        Print_At_Adr(*start);	
    	
		}			   
    if (Entry_st.Attributes == Folder) {
            ST_cluster = Entry_st.Cluster;
            if (ST_cluster == 0x0000) {
                display();
                Print_At_Adr(Sp_data.rootdirectory);
                *start = Sp_data.rootdirectory;
            } else {
                if (!first_call) {
                *start = Sp_data.Data_region + (ST_cluster - CLUSTER_OFFSET) * Sp_data.Byte_per_block;
                display();
                Print_At_Adr(*start);
                }
            }
        }
    previous_start = *start;
}




