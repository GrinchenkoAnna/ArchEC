#include "../src/myBigChars.c"
#include "../src/myTerm.c"

int main()
{
    printf("123\n");
    mt_clrscr();
    printf("Экран очищен\n");

    char str[5] = "Hello";
    printf("%s: ", str);
    bc_printA(str);

    printf("\n");
    bc_box(2, 10, 15, 50);
    printf("\nневерные координаты для прямоугольника: возврат %d\n", \
    bc_box(-2, 10, -15, 50)); //0

    int bigchar[2] = {-2122213018, 405029505};
    //-2122213018 = 10000001 10000001 10011001 01100110
    //405029505 = 00011000 00100100 01000010 10000001
    printf("Bigchar:\n");
    bc_printbigchar(bigchar, 19, 2, CYAN, LIGHT_GRAY); 

    bc_setbigcharpos(bigchar, 2, 3, 1);
    bc_printbigchar(bigchar, 19, 2+9, RED, LIGHT_GRAY);

    bc_setbigcharpos(bigchar, 2, 3, 0);
    bc_setbigcharpos(bigchar, 3, 3, 1);
    bc_printbigchar(bigchar, 19, 2+17, GREEN, LIGHT_GRAY);

    bc_setbigcharpos(bigchar, 3, 3, 0);
    bc_setbigcharpos(bigchar, 4, 3, 1);
    bc_printbigchar(bigchar, 19, 2+25, BLACK, LIGHT_GRAY);

    printf("\nневерные координаты для точки внутри bigchar: возврат %d\n\n", \
    bc_setbigcharpos(bigchar, -4, 3, 1)); //0

    int value;
    printf("В последнем изображении:\n");
    printf("x = 4, y = 3: %d\n", \
    bc_getbigcharpos(bigchar, 4, 3, &value)); //1
    printf("x = 2, y = 3: %d\n", \
    bc_getbigcharpos(bigchar, 2, 3, &value)); //0

    printf("\nЗапись в файл:\n");
    printf("bigchar[0] = %d\n", bigchar[0]);
    printf("bigchar[1] = %d\n", bigchar[1]);
    //--запись--
    int fd = open("test/testfile.txt", O_RDWR | O_CREAT | O_TRUNC);
    bc_bigcharwrite(fd, bigchar, 1);
    //--чтение (проверка)--
    int test_buffer[2];
    lseek(fd, 0, SEEK_SET);
    read(fd, test_buffer, sizeof(test_buffer));
    printf("errno = %s\n", strerror(errno));
    printf("test_buffer[0] = %d\n", test_buffer[0]);
    printf("test_buffer[1] = %d\n", test_buffer[1]);
    
    close(fd);
    return 0;
}