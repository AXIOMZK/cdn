#include "deploy.h"
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;
//你要完成的功能总入口
struct NetsInfo
{
    int total_bandwidth = 0;
    int remaining_bandwidth = 0;
    int network_hire = 0;
};
struct ResumeInfo
{
    int node_NO;
    int need_bandwidth;
};

void deploy_server(char *topo[MAX_EDGE_NUM], int line_num, char *filename)
{
    unsigned long links, consumer_nodes, network_nodes;
    stringstream read(topo[0]);
    read >> network_nodes >> links >> consumer_nodes;
    vector<vector<NetsInfo>> Nets(network_nodes, vector<NetsInfo>(network_nodes));
    vector<ResumeInfo> Consumers(consumer_nodes);
    int cost = atoi(topo[2]);
    for (unsigned long i = 4; i < 4 + links; ++i)
    {
        int start_node, end_node, total_bandwidth, network_hire;
        stringstream read2(topo[i]);
        read2 >> start_node >> end_node >> total_bandwidth >> network_hire;
        Nets[start_node][end_node].total_bandwidth = total_bandwidth;
        Nets[start_node][end_node].remaining_bandwidth = total_bandwidth;
        Nets[start_node][end_node].network_hire = network_hire;
    }
    for (unsigned long j = 5 + links; j < 5 + links + consumer_nodes; ++j)
    {
        int num, node_NO, need_bandwidth;
        stringstream read2(topo[j]);
        read2 >> num >> node_NO >> need_bandwidth;
        Consumers[num].need_bandwidth = need_bandwidth;
        Consumers[num].node_NO = node_NO;
    }

    stringstream results;
    results << consumer_nodes<<"\n";
    for (unsigned long k = 0; k < consumer_nodes; ++k)
    {
        results<<"\n"<<Consumers[k].node_NO<<" "<<k<<" "<<Consumers[k].need_bandwidth;
    }
    char* topo_file=(char *)results.str().c_str();



    // 需要输出的内容
//    char *topo_file = (char *) "17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";

    // 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
    write_result(topo_file, filename);

}
