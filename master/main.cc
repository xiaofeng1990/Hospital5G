#include <iostream>
#include <string>
#include "xfframework/config.h"
#include "xfframework/logging.h"
#define TEST_CONFIG_FILE "./master/config/config.ini"
#define AGE 15
int main()
{
    std::cout << "hospital 5g project!" << std::endl;
    // XfConfig config;
    // config.Open(TEST_CONFIG_FILE);

    printf("hello world xfframework\n");
    printf(toString(12345));
    printf("\n%d\n", conStr(1, 3));
    printf(toString(XF_DEBUG));
    printf("\n");
    printf(toString(AGE));
    printf("\n%d\n", conStr(AGE, AGE));
    printf("\n");

    std::string exec_path;
    std::string exec_name;
    xflog::GetExecutablePath(exec_path, exec_name);
    std::cout << exec_path << std::endl;
    std::cout << exec_name << std::endl;
    int i;
    // for (i = 0; i < 1024 * 1024; i++)
    // {
    //     XF_LOGT(ERROR, "test", "log test %d\n", i);
    // }
    return 0;
}