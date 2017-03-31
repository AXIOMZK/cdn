#include "deploy.h"
#include "MCMF.h"
#include "lib_time.h"

int main(int argc, char *argv[])
{
    print_time("Begin");
    char *topo[MAX_EDGE_NUM];
    int line_num;

//    char *topo_file = argv[1];
    char *topo_file = (char *) "/Users/guzi/Desktop/HUAWEI_Code_Craft_2017_Preliminary_Contest_Question/HUAWEI_Code_Craft_2017_Preliminary_Contest_Question_zh/case_example/case0.txt";

    line_num = read_file(topo, MAX_EDGE_NUM, topo_file);

    printf("line num is :%d \n", line_num);
    if (line_num == 0)
    {
        printf("Please input valid topo file.\n");
        return -1;
    }

//    char *result_file = argv[2];
    char *result_file = (char *) "/Users/guzi/Desktop/case0Result.txt";

    deploy_server(topo, line_num, result_file);

    release_buff(topo, line_num);

    print_time("End");

	return 0;
}

