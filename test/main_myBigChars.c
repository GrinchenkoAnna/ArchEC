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
    bc_getbigcharpos(bigchar, 4, 3, &value);
    printf("x = 4, y = 3: %d\n", value); //1
    bc_getbigcharpos(bigchar, 2, 3, &value);
    printf("x = 2, y = 3: %d\n", value); //0

    printf("\n---Запись в файл: успех\n");
    int count = 3;
    int bigchar_2symbols_to_write[2*count]; //массив для записи в файл
    for (int i = 0; i < 2*count; i++) 
    { bigchar_2symbols_to_write[i] = bigchar[i%2]; }
    printf("bigchar_2symbols_to_write[0] = %d\n", bigchar_2symbols_to_write[0]);
    printf("bigchar_2symbols_to_write[1] = %d\n", bigchar_2symbols_to_write[1]);
    printf("bigchar_2symbols_to_write[2] = %d\n", bigchar_2symbols_to_write[2]);
    printf("bigchar_2symbols_to_write[3] = %d\n", bigchar_2symbols_to_write[3]);
    printf("bigchar_2symbols_to_write[4] = %d\n", bigchar_2symbols_to_write[4]);
    printf("bigchar_2symbols_to_write[5] = %d\n", bigchar_2symbols_to_write[5]);
    //--запись--
    int fd = open("test/testfile.txt", O_RDWR | O_CREAT | O_TRUNC);
    if (bc_bigcharwrite(fd, bigchar_2symbols_to_write, count) == 0)
    { printf("успешно записано\n"); }
    else { printf("запись не осуществлена\n"); }
    //--чтение (проверка)--
    int test_buffer_0[2*count];
    lseek(fd, 0, SEEK_SET);
    read(fd, test_buffer_0, sizeof(test_buffer_0)*count);
    printf("errno = %s\n", strerror(errno));
    printf("test_buffer_0[0] = %d\n", test_buffer_0[0]);
    printf("test_buffer_0[1] = %d\n", test_buffer_0[1]);
    printf("test_buffer_0[2] = %d\n", test_buffer_0[2]);
    printf("test_buffer_0[3] = %d\n", test_buffer_0[3]);
    printf("test_buffer_0[4] = %d\n", test_buffer_0[4]);
    printf("test_buffer_0[5] = %d\n", test_buffer_0[5]);
    //--неудачная запись--
    printf("---Запись в файл: неудача\n");
    if (bc_bigcharwrite(16, bigchar_2symbols_to_write, count) == 0)
    { printf("успешно записано\n"); }
    else { printf("запись не осуществлена\n"); }
    errno = 0;

    printf("\n---Чтение из файла: успех\n");    
    int need_count = 2;
    int test_buffer_1[2*need_count];
    if (bc_bigcharread(fd, test_buffer_1, need_count, &count) == 0)
    { printf("успешно считано\n"); }
    else { printf("считано 0 символов\n"); }
    printf("errno = %s\n", strerror(errno));
    printf("count = %d\n", count);
    printf("test_buffer_1[0] = %d\n", test_buffer_1[0]);
    printf("test_buffer_1[1] = %d\n", test_buffer_1[1]);
    printf("test_buffer_1[2] = %d\n", test_buffer_1[2]);
    printf("test_buffer_1[3] = %d\n", test_buffer_1[3]);
    //--считано 0 символов--
    printf("---Чтение из файла: неудача\n"); 
    int fd_empty = open("test/testfile_empty.txt", O_RDWR | O_CREAT | O_TRUNC);
    if (bc_bigcharread(fd_empty, test_buffer_1, need_count, &count) == 0)
    { printf("успешно считано\n"); }
    else { printf("считано 0 символов\n"); }

    close(fd);
    close(fd_empty);
    return 0;
}