#include <windows.h> 
#include"Support.h"
#include "readFat.h"
void loading() {
    int i;
    int consoleWidth_2 = 80; 
    int textWidth_2 = 18;
	int consoleWidth_1 = 80; 
    int textWidth_1 = 17; 
    int padding_1 = (consoleWidth_1 - textWidth_1) / 2; 
	int padding_2 = (consoleWidth_2 - textWidth_2) / 2; 
    printf("\nLoading: ");
    for (i = 0; i <= 100; i++) {
        printf("\r%*sDang Loading: %d%%", padding_1, "", i); 
        fflush(stdout);
        Sleep(20); 
    }
    system("cls");
    printf("%*sLoading completed!\n", padding_2, "");
    Sleep(1000);
    system("cls");
}

void display(){
	printf("\n");
	printf("-----------------------------------------------------------\n");
    printf("| %-3s | %-8s | %-3s | %-8s | %-10s | %-8s |\n", "STT", "Name", "Ext", "Time", "Date", "Size");
    printf("-----------------------------------------------------------\n");
}


void printEntry(uint8_t idx,Entry_Data_t Entry ) {
    printf("| %3d | %.8s | %-3s | %02d:%02d:%02d | %02d/%02d/%04d | %-8d |\n",
           idx,
		   Entry.Name, 
		   Entry.Extension,
           (Entry.Time >> 11) & 0x1F, 
		   (Entry.Time >> 5) & 0x3F, 
		   (Entry.Time & 0x1F) * 2,
           Entry.Date & 0x1F, 
		   (Entry.Date >> 5) & 0xF, 
		   1980 + ((Entry.Date >> 9) & 0x7F),
           Entry.Size);
}







