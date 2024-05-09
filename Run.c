//#include"Run.h"
//void run(){
//	uint8_t Input_user;
//	uint32_t *Start_user = 0x2600;
//	display();
//	Read_root();
//	calStartPosition();
//	Start_user = Hanlde(Input_user, Start_user );
//	
//	do {
//		
//		printf("\nNhap E de thoat");
//		printf("\nMoi nhap lua chon cua ban: ");
//		status = scanf("%d", &Input_user) == 1; // Ki?m tra xem l?a ch?n có ph?i là m?t s? hay không
//		if (status) { // N?u là m?t s?
//			status = Check_ip(Input_user, Start_user); // Ki?m tra s? nh?p vào
//			if (status) { // N?u ki?m tra thành công
//				Hanlde(Input_user, &Start_user); // Th?c hi?n x? lý
//			} else {
//				printf("Loi: Lua chon khong hop le. Moi nhap lai.\n");
//			}
//		} else if (Input_user != 'e') { // N?u không ph?i là ký t? "e"
//			printf("Loi: Lua chon khong hop le. Moi nhap lai.\n");
//			while (getchar() != '\n'); // Xóa b? nh? d?m
//		}
//	} while (Input_user != 'e'); // L?p l?i cho d?n khi nh?p "e"
//}
//
//

