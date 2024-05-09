#include"readFat.h"
#include"Support.h"

void main() {
    char Input_user[10]; // M?ng d? ch?a chu?i nh?p vào
    uint32_t Start_user;
    OpenFile("floppy.img");

    if (status == Can_not_Open) {
        printf("Khong the mo file");
    } else {
        calStartPosition();
        display();
        Read_root();

        do {
            printf("\nNhap E de thoat");
            printf("\nMoi nhap lua chon cua ban: ");
            scanf(" %s", Input_user); // Ğ?c d? li?u vào m?ng chu?i

            if (Input_user[0] == 'E' || Input_user[0] == 'e') {
                break;
            } else if (Input_user[0] >= '0' && Input_user[0] <= '9') {
                int number = atoi(Input_user); // S? d?ng hàm charToInt d? chuy?n d?i
                status = Check_ip(number, Start_user);
                if (status) {
                    Handle(number, &Start_user);
                }
                if (status == Input_error) {
                    printf("Loi: Lua chon khong hop le. Moi nhap lai.\n");
                }
            } else {
                printf("Loi: Lua chon khong hop le. Moi nhap lai.\n");
            }
        } while (true);
    }
}
