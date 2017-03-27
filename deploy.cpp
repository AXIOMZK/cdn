#include "deploy.h"

using namespace std;
//C++整数规划+模拟退火方案

int isExit = 1;//定时器标志
//通过调用alarm来设置计时器，然后继续做别的事情。当计时器计时到0时，信号发送，处理函数被调用。

void timer(int sig)
{
    isExit = 0;
}

//服务器总成本
int getServerCost(vector<SeverNoAndAroundBandwidth> &v)
{
    return SeverCost * (int) v.size();
}

//得到新服务器编号(变异)

set<SeverNoAndAroundBandwidth> getNewServe(set<SeverNoAndAroundBandwidth> &oldServe)
{
    int size= (int) oldServe.size();
    auto newServe = oldServe;
    srand((unsigned int) time(NULL));
    int flag = 1 + rand() % 5;
    if (flag == 1)
    {
        //随机删除一个服务器
        int pos = (int) (rand() % newServe.size());
        auto it = newServe.begin();
        while (pos--) it++;
        newServe.erase(it);

    } else if (flag == 2)
    {
        //优先删除所能提供带宽最小的服务器
        newServe.erase(newServe.begin());

    } else if (flag == 3)
    {
        //随机添加一个服务器
        int pos = (int) (rand() % newServe.size());

        auto it = newServe.begin();

    } else if (flag == 4)
    {


    } else
    {

    }
}

struct Bandwidth_From_Small_To_Big
{
    bool operator()(const SeverNoAndAroundBandwidth &left, const SeverNoAndAroundBandwidth &right) const
    {
        return (left.ServeAroundBandwidth < right.ServeAroundBandwidth);
    }
};

void deploy_server(char *topo[MAX_EDGE_NUM], int line_num, char *filename)
{
    //执行定时器函数
    signal(SIGALRM, timer);
    alarm(80); //定时80s

    stringstream read(topo[0]);
    read >> network_nodes >> links >> consumer_nodes;
    maxServer = consumer_nodes;
    SeverCost = atoi(topo[2]);

    for (unsigned long i = 4; i < 4 + links; ++i)
    {
        double start_node, end_node;
        int total_bandwidth, network_hire;
        read.str("");
        read << topo[i];
        read >> start_node >> end_node >> total_bandwidth >> network_hire;
        Nets[start_node][end_node].total_bandwidth = total_bandwidth;
        Nets[start_node][end_node].remaining_bandwidth = total_bandwidth;
        Nets[start_node][end_node].network_hire = network_hire;
    }
    for (unsigned long j = 5 + links; j < 5 + links + consumer_nodes; ++j)
    {
        int num, node_NO, need_bandwidth;
        read.str("");
        read << topo[j];
        read >> num >> node_NO >> need_bandwidth;
        Consumers[num].need_bandwidth = need_bandwidth;
        Consumers[num].node_NO = node_NO;
    }

    //得到服务器节点可能的最大输出带宽
    int AroundBandwidth = 0;
    for (int i = 0; i < network_nodes; ++i)
    {
        for (int j = 0; j < network_nodes; ++j)
        {
            AroundBandwidth += Nets[i][j].total_bandwidth;
        }
        ServeAroundBandwidth.push_back(AroundBandwidth);
    }

    //服务器初始编号，即直连方案编号，外加一个参考可能提供带宽量,按带宽从小到大排序
    set<SeverNoAndAroundBandwidth,Bandwidth_From_Small_To_Big> SeverNo;
    for (int l = 0; l < Consumers.size(); ++l)
    {
        SeverNoAndAroundBandwidth pair;
        pair.ServerNo = l;
        pair.ServeAroundBandwidth = ServeAroundBandwidth[l];
        SeverNo.insert(pair);
    }


    read.str("");
    stringstream &results = read;
    results << consumer_nodes << "\n";
    for (unsigned long k = 0; k < consumer_nodes; ++k)
    {
        results << "\n" << Consumers[k].node_NO << " " << k << " " << Consumers[k].need_bandwidth;
    }
    const string &strtemp = results.str();
    char *topo_file = (char *) strtemp.c_str();



//    char* topo_file=(char *)results.str().c_str();//这是错误的
//streamstring在调用str()时，会返回临时的string对象。而因为是临时的对象，所以它在整个表达式结束后将会被析构。
    //   如果需要进一步操作string对象，先把其值赋给一个string变量后再操作。
    //alarm定时器，捕捉SIGALRM信号
    //捕捉函数用 signal(SIGALRM, funcPtr);
    // 需要输出的内容
//    char *topo_file = (char *) "17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";

    // 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
    write_result(topo_file, filename);

}


