#include <sys/ioctl.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
void ShowMainMenu()
{
    system("clear"); // 清屏函数
    printf("\n\n\n\n");
    // settextcolor(RED);
    printf("\033[1;33m\t\t\t|************************************|\n");
    printf("\t\t\t|*******欢迎进入图书馆管理系统*******|\n");
    printf("\t\t\t|***************主菜单***************|\n");
    printf("\t\t\t|*************0.退出系统*************|\n");
    printf("\t\t\t|*************1.图书管理*************|\n");
    printf("\t\t\t|*************2.读者管理*************|\n");
    printf("\t\t\t|*************3.借书还书管理*********|\n");
    printf("\t\t\t|************************************|\n");
    printf("\n");
    printf("\t\t\t请选择:0-3\n");
}

void ShowBookMenu() // 显示书籍管理的子菜单
{
    system("clear");
    printf("\n\n\n\n");
    printf("\t\t\t|****************************************|\n");
    printf("\t\t\t|*******欢迎进入图书信息管理菜单*********|\n");
    printf("\t\t\t|*************0.返回图书管理*************|\n");
    printf("\t\t\t|*************1.显示图书信息*************|\n");
    printf("\t\t\t|*************2.增加图书信息*************|\n");
    printf("\t\t\t|*************3.查询图书信息*************|\n");
    printf("\t\t\t|*************4.删除图书信息*************|\n");
    printf("\t\t\t|*************5.修改图书信息*************|\n");
    printf("\t\t\t|****************************************|\n");
    printf("\n");
    printf("\t\t\t请选择:0-5\n");
}

void ShowBook()
{
    MYSQL mysql_connect;
    MYSQL *mysql = mysql_init(&mysql_connect);
    if (mysql == NULL)
    {
        printf("init error\n");
        exit(1);
    }
    mysql = mysql_real_connect(mysql, "localhost", "root", NULL, "db_book", 3306, NULL, 0);
    if (mysql == NULL)
    {
        printf("connect error");
        exit(1);
    }

    system("clear");
    MYSQL_RES *result;
    MYSQL_ROW row;
    char *sql = "SELECT * FROM Book;";
    if (mysql_query(mysql, sql) != 0)
    {
        printf("querry error:%s\n", mysql_error(mysql));
        exit(3);
    }

    printf("\t               ***** 显示图书信息 *****                   \n");
    printf("图书编号   图书名     图书作者    图书出版社    图书库存量\n");
    printf(" -------------------------------------------------------- \n");

    result = mysql_store_result(mysql);
    if (result == NULL)
    {
        printf("res err:%s\n", mysql_error(mysql));
        exit(1);
    }

    int num = mysql_num_rows(result);

    int count = mysql_field_count(mysql);
    // printf("book书有%d行,%d列\n", num, count);

    // 行列的信息输出还可以见下面删除的输出方法(列直接输出,因为没有几列)
    for (int i = 0; i < num; i++)
    {
        row = mysql_fetch_row(result);
        for (int j = 0; j < count; j++)
        {
            printf("%s           ", row[j]);
        }
        printf("\n");
    }

    mysql_free_result(result);
    mysql_close(mysql);
}

void AddBook() // 新增图书信息
{
    MYSQL mysql_connect;
    MYSQL *mysql = mysql_init(&mysql_connect);
    if (mysql == NULL)
    {
        printf("init error\n");
        exit(1);
    }
    mysql = mysql_real_connect(mysql, "localhost", "root", NULL, "db_book", 3306, NULL, 0);
    if (mysql == NULL)
    {
        printf("connect error");
        exit(1);
    }

    system("clear");
    while (1)
    {
        char dest[500] = {" "};
        int iNum; // 图书编号
        char acName[15];
        char acAuthor[15];
        char acPress[15];
        int iAmout;
        char ch;
        printf("\t 图书编号:");
        scanf("%d", &iNum);
        printf("\t 图书名称:");
        scanf("%s", acName); // 输入图书名称
        printf("\t 图书作者:");
        scanf("%s", acAuthor); // 输入图书作者
        printf("\t 图书出版社:");
        scanf("%s", acPress); // 输入图书出版社
        printf("\t 图书库存量:");
        scanf("%d", &iAmout); // 输入图书库存量
        getchar();
        // 将键盘的数据往数据库中输入
        sprintf(dest, "INSERT INTO Book (iNum,acName,acAuthor,acPress,iAmout)VALUES (%d,'%s','%s','%s',%d);", iNum, acName, acAuthor, acPress, iAmout);
        if (mysql_query(mysql, dest) != 0)
        {
            printf("query error:%s\n", mysql_error(mysql));
            exit(3);
        }
        else
        {
            printf("\t 插入成功\n");
            memset(dest, 0, 500);
            printf("是否继续插入数据?Y|N\n");
            char select;
            scanf("%c", &select);
            getchar();
            if (select == 'n' || select == 'N')
            {
                mysql_close(mysql);
                break;
            }
        }
        mysql_close(mysql);
    }
}

int SearchBook() // 查找图书信息
{
    MYSQL mysql_connect;
    MYSQL *mysql = mysql_init(&mysql_connect);
    if (mysql == NULL)
    {
        printf("init error\n");
        exit(1);
    }
    mysql = mysql_real_connect(mysql, "localhost", "root", NULL, "db_book", 3306, NULL, 0);
    if (mysql == NULL)
    {
        printf("connect error");
        exit(1);
    }

    system("clear");
    MYSQL_RES *result; // 结果变量
    MYSQL_ROW row;     // 行变量

    printf("请输入要查询的图书编号\n");
    int iNum;
    scanf("%d", &iNum);
    getchar();
    char dest[500] = {" "};
    sprintf(dest, "select * from Book where iNum=%d;", iNum);
    if (mysql_query(mysql, dest) != 0)
    {
        printf("该图书信息不存在\n");
    }
    else
    {
        printf("该图书信息存在，是否显示?Y|N\n");
        char select;
        scanf("%c", &select);
        getchar();
        if (select == 'y' || select == 'Y')
        {
            printf("\t               ***** 显示图书信息 *****                   \n");
            printf("图书编号   图书名     图书作者    图书出版社    图书库存量\n");
            printf(" -------------------------------------------------------- \n");

            result = mysql_store_result(mysql);
            if (result == NULL)
            {
                printf("res error:%s", mysql_error(mysql));
                exit(1);
            }
            int num = mysql_num_rows(result);
            int count = mysql_field_count(mysql);
            // printf("一共%d行,%d列", num, count);
            // 输出行列
            for (int i = 0; i < num; i++)
            {
                row = mysql_fetch_row(result);
                for (int j = 0; j < count; j++)
                {
                    printf("%s           ", row[j]);
                }
                printf("\n");
            }
            mysql_free_result(result);
        }
    }

    mysql_close(mysql);
}

void DeleteBook() // 删除图书信息
{
    MYSQL mysql_connect;
    MYSQL *mysql = mysql_init(&mysql_connect);
    if (mysql == NULL)
    {
        printf("init error\n");
        exit(1);
    }
    mysql = mysql_real_connect(mysql, "localhost", "root", NULL, "db_book", 3306, NULL, 0);
    if (mysql == NULL)
    {
        printf("connect error");
        exit(1);
    }

    system("clear");
    MYSQL_RES *result; // 结果变量
    MYSQL_ROW row;     // 行变量
    char ch;
    // 查询删除的图书是否存在
    int iNum;
    printf("请输入图书编号\n");
    scanf("%d", &iNum);
    getchar();
    char dest[500] = {" "};

    sprintf(dest, "SELECT * FROM Book WHERE iNum = %d;", iNum);
    if (mysql_query(mysql, dest) != 0)
    {
        printf("\n%d好图书不存在，查询失败\n", iNum);
    }
    else
    {
        result = mysql_store_result(mysql);
        if (mysql_num_rows(result) != 0)
        {
            printf("\t图书数据存在,是否显示数据(y|n)\n");
            scanf("%c", &ch);
            getchar();
            if (ch == 'y' || ch == 'Y')
            {
                printf("\t               ***** 显示图书信息 *****                   \n");
                printf("图书编号   图书名     图书作者    图书出版社    图书库存量\n");
                printf(" -------------------------------------------------------- \n");
                int num = mysql_num_rows(result);
                int count = mysql_field_count(mysql);
                // printf("一共%d行,%d列", num, count);
                // 输出行列
                for (int i = 0; i < num; i++)
                {
                    row = mysql_fetch_row(result);
                    for (int j = 0; j < count; j++)
                    {
                        printf("%s           ", row[j]);
                    }
                    printf("\n");
                }

                // while ((row = mysql_fetch_field(result)))
                //{
                //     fprintf(stdout, "\t  %-7s%-7s %-7s %-7s%-7s \n", row[0], row[1], row[2], row[3], row[4]);
                // }

                printf("\t是否删除该数据(Y|N)\n");
                char dest1[500] = {" "};
                scanf("%c", &ch);
                getchar();
                if (ch == 'y' || ch == 'Y')
                {
                    sprintf(dest1, "DELETE FROM Book WHERE iNum = %d;", iNum);
                    if (mysql_query(mysql, dest1) != 0)
                    {
                        printf("\t删除记录失败\n");
                    }
                    else
                    {
                        printf("\t%d号图书信息删除成功\n", iNum);
                    }
                }
            }
        }
        else
        {
            printf("未查找到相关记录\n");
        }
    }
    mysql_free_result(result);
    // 删除之前需要保存删除的图书编号
    mysql_close(mysql);
}

void ShowModifyBookMenu()
{
    printf("\n\n\n");
    printf("\t\t\t\t0.退出\n");
    // printf("\t\t\t\t1.编号\n");
    printf("\t\t\t\t1.书名\n");
    printf("\t\t\t\t2.作者\n");
    printf("\t\t\t\t3.出版社\n");
    printf("\t\t\t\t4.库存量\n");
    printf("\t\t\t请输入你要修改的信息（1-4）");
}

void ModifyBook() // 修改图书信息
{
    system("clear");

    MYSQL mysql_connect;
    MYSQL *mysql = mysql_init(&mysql_connect);
    if (mysql == NULL)
    {
        printf("init error\n");
        exit(1);
    }
    mysql = mysql_real_connect(mysql, "localhost", "root", NULL, "db_book", 3306, NULL, 0);
    if (mysql == NULL)
    {
        printf("connect error");
        exit(1);
    }

    MYSQL_RES *result; // 结果变量
    MYSQL_ROW row;     // 行变量
    int iNum = 0;

    char ch;

    printf("开始修改记录:\n");
    printf("请输入要修改的图书编号\n");
    scanf("%d", &iNum);
    getchar();
    char dest[500] = {" "};
    sprintf(dest, "SELECT * FROM Book WHERE iNum = %d;", iNum);
    if (mysql_query(mysql, dest) != 0)
    {
        printf("\n%d好图书不存在，查询失败\n", iNum);
    }
    else
    {
        result = mysql_store_result(mysql);
        if (mysql_num_rows(result) != 0)
        {
            printf("\t图书数据存在,是否显示数据(y/n)\n");
            scanf("%c", &ch);
            getchar();
            if (ch == 'y' || ch == 'Y')
            {
                printf("\t               ***** 显示图书信息 *****                   \n");
                printf("图书编号   图书名     图书作者    图书出版社    图书库存量\n");
                printf(" -------------------------------------------------------- \n");
                int num = mysql_num_rows(result);
                int count = mysql_field_count(mysql);
                // printf("一共%d行,%d列", num, count);
                // 输出行列
                for (int i = 0; i < num; i++)
                {
                    row = mysql_fetch_row(result);
                    for (int j = 0; j < count; j++)
                    {
                        printf("%s           ", row[j]);
                    }
                    printf("\n");
                }
                mysql_free_result(result);
            }

            printf("\t是否修改(y|n)\n");
            scanf("%c", &ch);
            getchar();
            if (ch == 'y' || ch == 'Y')
            {
                ShowModifyBookMenu();
                char choose;
                int iNum1 = iNum;
                scanf("%d", &choose);
                getchar();
                iNum = iNum1;
                printf("%d\n", iNum);
                while (choose)
                {
                    switch (choose)
                    {
                    case 1:
                        printf("请输入修改后的名字\n");
                        char acName[15];
                        scanf("%s", acName);
                        getchar();
                        memset(dest, 0, 500);
                        sprintf(dest, "UPDATE Book SET acName = '%s' WHERE iNum = %d;", acName, iNum);
                        if (mysql_query(mysql, dest) != 0)
                        {
                            printf("\t修改失败\n");
                        }
                        else
                        {
                            mysql_commit(mysql); // 提交修改
                            printf("\t修改成功\n");
                        }
                        break;
                    case 2:
                        printf("请输入修改后的作者\n");
                        char acAuthor[15];
                        scanf("%s", acAuthor);
                        getchar();
                        memset(dest, 0, 500);
                        sprintf(dest, "UPDATE Book SET acAuthor = '%s' WHERE iNum = %d;", acAuthor, iNum);
                        if (mysql_query(mysql, dest) != 0)
                        {
                            printf("\t修改失败\n");
                        }
                        else
                        {
                            mysql_commit(mysql); // 提交修改

                            printf("\t修改成功\n");
                        }
                        break;
                    case 3:
                        printf("请输入修改后的出版社\n");
                        char acPress[15];
                        scanf("%s", acPress);
                        getchar();
                        memset(dest, 0, 500);
                        sprintf(dest, "UPDATE Book SET acPress = '%s' WHERE iNum = %d;", acPress, iNum);
                        if (mysql_query(mysql, dest) != 0)
                        {
                            printf("\t修改失败\n");
                        }
                        else
                        {
                            mysql_commit(mysql); // 提交修改

                            printf("\t修改成功\n");
                        }
                        break;
                    case 4:
                        printf("请输入修改后的库存量\n");
                        char iAmout[15];
                        scanf("%s", iAmout);
                        getchar();
                        memset(dest, 0, 500);
                        sprintf(dest, "UPDATE Book SET iAmout = %d WHERE iNum = %d;", iAmout, iNum);
                        if (mysql_query(mysql, dest) != 0)
                        {
                            printf("\t修改失败\n");
                        }
                        else
                        {
                            mysql_commit(mysql); // 提交修改

                            printf("\t修改成功\n");
                        }
                        break;
                    default:
                        printf("请输入正确的数字");
                        break;
                    }
                    printf("\n\t\t\t按任意键返回子菜菜单\n");
                    getchar();
                    system("clear");
                    ShowModifyBookMenu();
                    scanf("%d", &choose);
                    getchar();
                }
            }
        }
        else
        {
            printf("未查找到相关记录\n");
        }
    }
    mysql_close(mysql);
}

void ManagerBook()
{
    int choose;
    ShowBookMenu();
    scanf("%d", &choose);
    getchar();
    while (choose)
    {
        switch (choose)
        {
        case 1:
            ShowBook();
            break;
        case 2:
            AddBook(); // 增加图书信息
            break;
        case 3:
            SearchBook(); // 查找图书信息
            break;
        case 4:
            DeleteBook();
            break;
        case 5:
            ModifyBook();
            break;
        default:
            printf("please put in right num\n");
            break;
        }
        printf("\n\t\t\t按任意键返回子菜菜单\n");
        // sleep(2);
        getchar();
        ShowBookMenu();
        scanf("%d", &choose);
        getchar();
    }
}

void ShowReaderMenu() // 显示读者管理菜单
{
    system("clear");
    printf("\n\n\n\n");
    printf("\t\t\t|****************************************|\n");
    printf("\t\t\t|*******欢迎进入读者管理菜单*************|\n");
    printf("\t\t\t|*************0.返回读者管理*************|\n");
    printf("\t\t\t|*************1.显示读者信息*************|\n");
    printf("\t\t\t|*************2.增加读者信息*************|\n");
    printf("\t\t\t|*************3.查询读者信息*************|\n");
    printf("\t\t\t|*************4.删除读者信息*************|\n");
    printf("\t\t\t|*************5.修改读者信息*************|\n");
    printf("\t\t\t|****************************************|\n");
    printf("\n");
    printf("\t\t\t请选择:0-5\n");
}

void ShowModifyReaderMenu() // 显示修改菜单
{
    printf("\n\n\n");
    printf("\t\t\t0.返回上级菜单\n");
    printf("\t\t\t1.编号\n");
    printf("\t\t\t2.姓名\n");
    printf("\t\t\t3.性别\n");
    printf("\t\t\t4.最大可借阅量\n");
    printf("\t\t\t5.当前可借量\n");
    printf("\t\t\t6.借阅列表\n");
    printf("\t\t\t请输入修改的信息\n");
}

void ShowReader()
{
    MYSQL mysql_connect;
    MYSQL *mysql = mysql_init(&mysql_connect);
    if (mysql == NULL)
    {
        printf("init error\n");
        exit(1);
    }
    mysql = mysql_real_connect(mysql, "localhost", "root", NULL, "db_book", 3306, NULL, 0);
    if (mysql == NULL)
    {
        printf("connect error");
        exit(1);
    }

    system("clear");

    MYSQL_RES *result;
    MYSQL_ROW row;
    char *sql = "SELECT * FROM Reader;";
    if (mysql_query(mysql, sql) != 0)
    {
        printf("querry error:%s\n", mysql_error(mysql));
        exit(3);
    }

    result = mysql_store_result(mysql);
    if (result == NULL)
    {
        printf("res err:%s\n", mysql_error(mysql));
        exit(1);
    }

    int num = mysql_num_rows(result);

    int count = mysql_field_count(mysql);
    printf("book书有%d行,%d列\n", num, count);

    // 行列的信息输出还可以见下面删除的输出方法(列直接输出,因为没有几列)
    for (int i = 0; i < num; i++)
    {
        row = mysql_fetch_row(result);
        for (int j = 0; j < count; j++)
        {
            printf("%s   ", row[j]);
        }
        printf("\n");
    }
    mysql_free_result(result);
    mysql_close(mysql);
}

void AddReader() // 新增图书信息
{
    MYSQL mysql_connect;
    MYSQL *mysql = mysql_init(&mysql_connect);
    if (mysql == NULL)
    {
        printf("init error\n");
        exit(1);
    }
    mysql = mysql_real_connect(mysql, "localhost", "root", NULL, "db_book", 3306, NULL, 0);
    if (mysql == NULL)
    {
        printf("connect error");
        exit(1);
    }
    system("clear");

    char dest[500] = {" "};
    int iNum; // 图书编号
    char acName[15];
    char acSex[15];
    char iMax;
    int iAmout;

    char ch;
    while (1)
    {

        printf("\t 读者编号:");
        scanf("%d", &iNum);
        printf("\t 读者姓名:");
        scanf("%s", acName);
        printf("\t 读者性别:");
        scanf("%s", acSex);
        /* printf("\t 最大可借阅量:");
         scanf("%d", &iMax);
         printf("\t 已经借阅量:");
         scanf("%d", &iAmout);
         getchar();*/
        // 将键盘的数据往数据库中输入
        getchar();
        sprintf(dest, "INSERT INTO Reader (iNum,acName,acSex)VALUES (%d,'%s','%s');", iNum, acName, acSex);
        if (mysql_query(mysql, dest) != 0)
        {
            printf("query error:%s\n", mysql_error(mysql));
            exit(3);
        }
        else
        {
            printf("\t 插入成功\n");
            memset(dest, 0, 500);
            printf("是否继续插入数据?Y|N\n");
            char select;
            scanf("%c", &select);
            getchar();
            if (select == 'n' || select == 'N')
            {
                mysql_close(mysql);
                break;
            }
        }
    }
    mysql_close(mysql);
}

int SearchReader() // 查找读者信息
{
    MYSQL mysql_connect;
    MYSQL *mysql = mysql_init(&mysql_connect);
    if (mysql == NULL)
    {
        printf("init error\n");
        exit(1);
    }
    mysql = mysql_real_connect(mysql, "localhost", "root", NULL, "db_book", 3306, NULL, 0);
    if (mysql == NULL)
    {
        printf("connect error");
        exit(1);
    }

    system("clear");
    MYSQL_RES *result; // 结果变量
    MYSQL_ROW row;     // 行变量

    printf("请输入要查询的读者编号\n");
    int iNum;
    scanf("%d", &iNum);
    getchar();
    char dest[500] = {" "};
    sprintf(dest, "select * from Reader where iNum=%d;", iNum);

    if (mysql_query(mysql, dest) != 0)
    {
        printf("query error:%s\n", mysql_error(mysql));
    }

    result = mysql_store_result(mysql);
    if (result == NULL)
    {
        printf("res error:%s", mysql_error(mysql));
        exit(1);
    }

    int num = mysql_num_rows(result);

    int count = mysql_field_count(mysql);
    // printf("一共%d行，%d列", num, count);

    // 输出行列
    for (int i = 0; i < num; i++)
    {
        row = mysql_fetch_row(result);
        for (int j = 0; j < count; j++)
        {
            printf("%s    ", row[j]);
        }
        printf("\n");
    }
    mysql_free_result(result);
    mysql_close(mysql);
}

void DeleteReader() // 删除图书信息
{
    MYSQL mysql_connect;
    MYSQL *mysql = mysql_init(&mysql_connect);
    if (mysql == NULL)
    {
        printf("init error\n");
        exit(1);
    }
    mysql = mysql_real_connect(mysql, "localhost", "root", NULL, "db_book", 3306, NULL, 0);
    if (mysql == NULL)
    {
        printf("connect error");
        exit(1);
    }
    system("clear");

    MYSQL_RES *result; // 结果变量
    MYSQL_ROW row;     // 行变量
    char ch;
    // 查询删除的图书是否存在
    int iNum;
    printf("请输入读者编号\n");
    scanf("%d", &iNum);
    getchar();
    char dest[500] = {" "};

    sprintf(dest, "SELECT * FROM Reader WHERE iNum = %d;", iNum);
    if (mysql_query(mysql, dest) != 0)
    {
        printf("\n%d好图书不存在，查询失败\n", iNum);
    }
    else
    {
        result = mysql_store_result(mysql);
        if (mysql_num_rows(result) != 0)
        {
            printf("\t读者数据存在,是否显示数据(y/n)\n");
            scanf("%c", &ch);
            getchar();
            if (ch == 'y' || ch == 'Y')
            {
                printf("\t               ***** 显示读者信息 *****                   \n");
                printf("读者编号   读者姓名     读者性别    最大可借阅量    当前借阅量    已经借阅的图书列表\n");
                printf(" -------------------------------------------------------- \n");
                int num = mysql_num_rows(result);
                int count = mysql_field_count(mysql);
                // printf("一共%d行,%d列", num, count);
                // 输出行列
                for (int i = 0; i < num; i++)
                {
                    row = mysql_fetch_row(result);
                    for (int j = 0; j < count; j++)
                    {
                        printf("%s           ", row[j]);
                    }
                    printf("\n");
                }
                printf("\t是否删除该数据(Y|N)\n");
                char dest1[500] = {" "};
                int iNum1 = iNum;
                scanf("%c", &ch);
                getchar();
                iNum = iNum1;
                if (ch == 'y' || ch == 'Y')
                {
                    sprintf(dest1, "DELETE FROM Reader WHERE  iNum = %d;", iNum);
                    if (mysql_query(mysql, dest1) != 0)
                    {
                        printf("\t删除记录失败\n");
                    }
                    else
                    {
                        printf("\t%d号图书信息删除成功\n", iNum);
                    }
                }
            }
        }
        else
        {
            printf("未查找到相关记录\n");
        }
    }

    mysql_close(mysql);
}

void ModifyReader() // 修改图书信息
{
    MYSQL mysql_connect;
    MYSQL *mysql = mysql_init(&mysql_connect);
    if (mysql == NULL)
    {
        printf("init error\n");
        exit(1);
    }
    mysql = mysql_real_connect(mysql, "localhost", "root", NULL, "db_book", 3306, NULL, 0);
    if (mysql == NULL)
    {
        printf("connect error");
        exit(1);
    }
    system("clear");

    MYSQL_RES *result; // 结果变量
    MYSQL_ROW row;     // 行变量
    int iNum;
    char ch;
    printf("开始修改记录:\n");
    printf("请输入要修改的读者编号\n");
    scanf("%d", &iNum);
    getchar();
    char dest[500] = {" "};
    sprintf(dest, "SELECT * FROM Reader WHERE iNum = %d;", iNum);
    if (mysql_query(mysql, dest) != 0)
    {
        printf("\n%d号读者信息不存在，查询失败\n", iNum);
    }
    else
    {
        result = mysql_store_result(mysql);
        if (mysql_num_rows(result) != 0)
        {
            printf("\t读者数据存在,是否显示数据\n");
            scanf("%c", &ch);
            getchar();
            if (ch == 'y' || ch == 'Y')
            {
                printf("\t               ***** 显示图书信息 *****                   \n");
                printf("\t读者编号   读者姓名     读者性别    最大可借阅量    当前借阅量    已经借阅的图书列表\n");
                printf("\t -------------------------------------------------------- \n");

                int num = mysql_num_rows(result);
                int count = mysql_field_count(mysql);
                // printf("一共%d行,%d列", num, count);
                // 输出行列
                for (int i = 0; i < num; i++)
                {
                    row = mysql_fetch_row(result);
                    for (int j = 0; j < count; j++)
                    {
                        printf("%s           ", row[j]);
                    }
                    printf("\n");
                }
            }
        }
        else
        {
            printf("未查找到相关记录\n");
            exit(2);
        }
    }

    printf("\t是否修改(y|n)\n");
    scanf("%c", &ch);
    getchar();
    if (ch == 'y' || ch == 'Y')
    {
        ShowModifyReaderMenu();
        char choose;
        int iNum1 = iNum;
        scanf("%d", &choose);
        getchar();
        iNum = iNum1;
        while (choose)
        {
            switch (choose)
            {
            case 1:
                printf("请输入修改后的名字\n");
                char acName[15];
                scanf("%s", acName);
                getchar();
                memset(dest, 0, 500);
                sprintf(dest, "UPDATE Reader SET acName = '%s' WHERE iNum = %d;", acName, iNum);
                if (mysql_query(mysql, dest) != 0)
                {
                    printf("\t修改失败\n");
                }
                else
                {
                    printf("\t修改成功\n");
                }
                break;
            case 2:
                printf("请输入修改后的性别\n");
                char acSex[15];
                scanf("%s", acSex);
                getchar();
                memset(dest, 0, 500);
                sprintf(dest, "UPDATE Reader SET acSex = '%s' WHERE iNum = %d;", acSex, iNum);
                if (mysql_query(mysql, dest) != 0)
                {
                    printf("\t修改失败\n");
                }
                else
                {
                    printf("\t修改成功\n");
                }
                break;
            case 3:
                printf("请输入修改后的最大可借阅量\n");
                int iMax;
                scanf("%d", iMax);
                getchar();
                memset(dest, 0, 500);
                sprintf(dest, "UPDATE Reader SET iMax = '%d' WHERE iNum = %d;", iMax, iNum);
                if (mysql_query(mysql, dest) != 0)
                {
                    printf("\t修改失败\n");
                }
                else
                {
                    printf("\t修改成功\n");
                }
                break;
            case 4:
                printf("请输入修改后的当前借阅量\n");
                int iAmout;
                scanf("%d", iAmout);
                getchar();
                memset(dest, 0, 500);
                sprintf(dest, "UPDATE Reader SET iAmout = '%d' WHERE iNum = %d;", iAmout, iNum);
                if (mysql_query(mysql, dest) != 0)
                {
                    printf("\t修改失败\n");
                }
                else
                {
                    printf("\t修改成功\n");
                }
                break;
            default:
                printf("请输入正确的数字");
                break;
            }
            printf("\n\t\t\t按任意键返回子菜单\n");
            getchar();
            system("clear");
            ShowModifyReaderMenu();
            scanf("%d", &choose);
            getchar();
        }
    }

    mysql_close(mysql);
}

/*
UPDATE Reader R
JOIN Borrow B ON R.iNum = B.iNum
SET R.BookId = B.iBookNum;设置Reader的BookId
*/

/*
UPDATE Reader R
JOIN Borrow B ON R.iNum = B.iNum
SET R.iAmount = (SELECT COUNT(iBookNum) FROM Borrow WHERE iNum = R.iNum);
设置Reader的iAmount
 */

void ManagerReader() // 读者管理模块
{

    int choose;
    ShowReaderMenu();
    scanf("%d", &choose);
    getchar();
    while (choose)
    {
        switch (choose)
        {
        case 1:
            ShowReader(); // 显示读者信息
            break;
        case 2:
            AddReader(); // 增加读者信息
            break;
        case 3:
            SearchReader(); // 查找读者信息
            break;
        case 4:
            DeleteReader(); // 删除读者信息
            break;
        case 5:
            ModifyReader(); // 修改读者信息
            break;
        default:
            printf("\t\t\t请输入正确的数字\n\t\t");
            break;
        }
        // printf("\t\t\t按任意键继续\n");
        //_getch();
        printf("\t\t\t按任意键返回子菜单\n");
        // sleep(2);
        getchar();
        ShowReaderMenu();
        scanf("%d", &choose);
        getchar();
    }
}

void ShowBorrowReturnMenu()
{
    system("clear");
    printf("\n\n\n");
    printf("\t\t\t***********************************************\n");
    printf("\t\t\t*************欢迎进入借书还书登记子菜单********\n");
    printf("\t\t\t*************0.返回上级菜单********************\n");
    printf("\t\t\t*************1.借书****************************\n");
    printf("\t\t\t*************2.还书****************************\n");
    printf("\t\t\t***********************************************\n");
    printf("\n\t\t\t请选择:\n");
}

void BorrowBook()
{
    system("clear");

    MYSQL mysql_connect;
    MYSQL *mysql = mysql_init(&mysql_connect);
    if (mysql == NULL)
    {
        printf("init error\n");
        exit(1);
    }
    mysql = mysql_real_connect(mysql, "localhost", "root", NULL, "db_book", 3306, NULL, 0);
    if (mysql == NULL)
    {
        printf("connect error");
        exit(1);
    }

    int iNum;
    printf("请输入借书者的编号\n");
    scanf("%d", &iNum);
    getchar();
    char dest[500] = {" "};
    // sprintf(dest, "SELECT * FROM Reader WHERE iNum = %d;", iNum);
    // if (mysql_query(mysql, dest) != 0)
    // {
    //    printf("\t查询失败,该读者不存在\n");
    //}
    // printf("该读者存在，请输入要借的图书编号\n");
    int iBookNum;
    printf("请输入图书编号\n");
    scanf("%d", &iBookNum);
    getchar();
    // 图书库存量--
    // memset(dest, 0, 500);
    sprintf(dest, "UPDATE Book SET iAmout = iAmout-1 WHERE iNum = %d;", iBookNum);
    if (mysql_query(mysql, dest) != 0)
    {
        printf("\t借书失败,该图书不存在\n");
        printf("%s\n", mysql_error(mysql));
        printf("%s\n", dest);
    }

    memset(dest, 0, 500);
    sprintf(dest, "INSERT INTO Borrow VALUES(%d,%d);", iNum, iBookNum);
    if (mysql_query(mysql, dest) != 0)
    {
        printf("\t借书失败\n");
        printf("%s\n", mysql_error(mysql));
        printf("%s\n", dest);
    }
    else
    {
        printf("成功借书\n");
    }

    mysql_close(mysql);
}

void ReturnBook() // 还书
{
    system("clear");

    MYSQL mysql_connect;
    MYSQL *mysql = mysql_init(&mysql_connect);
    if (mysql == NULL)
    {
        printf("init error\n");
        exit(1);
    }
    mysql = mysql_real_connect(mysql, "localhost", "root", NULL, "db_book", 3306, NULL, 0);
    if (mysql == NULL)
    {
        printf("connect error");
        exit(1);
    }

    int iNum;
    printf("请输入还书者的编号\n");
    scanf("%d", &iNum);
    getchar();
    char dest[500] = {" "};
    // sprintf(dest, "SELECT * FROM Reader WHERE iNum = %d;", iNum);
    // if (mysql_query(mysql, dest) != 0)
    // {
    //    printf("\t查询失败,该读者不存在\n");
    //}
    // printf("该读者存在，请输入要借的图书编号\n");
    int iBookNum;
    printf("请输入图书编号\n");
    scanf("%d", &iBookNum);
    getchar();
    // 图书库存量--
    // memset(dest, 0, 500);
    sprintf(dest, "UPDATE Book SET iAmout = iAmout+1 WHERE iNum = %d;", iBookNum);
    if (mysql_query(mysql, dest) != 0)
    {
        printf("\t还书失败,该图书不存在\n");
        printf("%s\n", mysql_error(mysql));
        printf("%s\n", dest);
    }

    memset(dest, 0, 500);
    sprintf(dest, "DELETE FROM Borrow WHERE iNum = %d and iBookNum=%d;", iNum, iBookNum);
    if (mysql_query(mysql, dest) != 0)
    {
        printf("\t还书失败\n");
        printf("%s\n", mysql_error(mysql));
        printf("%s\n", dest);
    }
    else
    {
        printf("成功还书\n");
    }

    mysql_close(mysql);
}

void ManagerBorrowReturnBook() // 借书还书模块
{
    int choose;
    ShowBorrowReturnMenu(); // 显示借书还书界面
    scanf("%d", &choose);
    getchar();
    while (choose)
    {
        switch (choose)
        {
        case 1:
            BorrowBook();
            break;
        case 2:
            ReturnBook();
            break;
        default:
            printf("\t\t\t请输入正确的数字(0-2)\n");
        }
        printf("\t\t\t按任意键返回子菜单\n");
        getchar();
        // sleep(2);
        ShowBorrowReturnMenu();
        scanf("%d", &choose);
        getchar();
    }
}

int main()
{

    ShowMainMenu(); // 显示主函数的界面
    int choose;
    scanf("%d", &choose);
    while (choose)
    {
        switch (choose)
        {
        case 1:
            ManagerBook(); // 图书管理模块
            break;
        case 2:
            ManagerReader(); // 读者管理模块
            break;
        case 3:
            ManagerBorrowReturnBook(); // 借书还书系统
            break;
        default:
            printf("\t\t\t请输入正确的数字");
            printf("\n\t\t\t程序将在3秒后跳转到主菜单");
            sleep(3);
            break;
        }
        ShowMainMenu();
        scanf("%d", &choose);
        getchar();
    }

    return 0;
}
