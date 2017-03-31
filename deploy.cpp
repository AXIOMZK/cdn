#include "deploy.h"
#include "MCMF.h"
#define T     1      //初始温度
#define EPS   1e-8    //终止温度
#define DELTA 0.98    //温度衰减率
#define LIMIT 1   //概率选择上限
#define OLOOP 5    //外循环次数
#define ILOOP 5   //内循环次数
using namespace std;
//C++整数规划+模拟退火方案


int isExit = 1;//定时器标志
//通过调用alarm来设置计时器，然后继续做别的事情。当计时器计时到0时，信号发送，处理函数被调用。

void timer(int sig)
{
    isExit = 0;
}

void deploy_server(char *topo[MAX_EDGE_NUM], int line_num, char *filename)
{
    //执行定时器函数
    signal(SIGALRM, timer);
    alarm(80); //定时80s

    double TotalNeed;//所有消费节点总需求
    int SeverCost;
    stringstream read(topo[0]);
    unsigned long links, consumer_nodes, network_nodes;//链路数，消费节点数，网络节点数
    read >> network_nodes >> links >> consumer_nodes;
    //单台服务器成本
    SeverCost = atoi(topo[2]);

    vector<vector<LinkInfo>> Nets(network_nodes, vector<LinkInfo>(network_nodes));
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
        Nets[end_node][start_node].total_bandwidth = total_bandwidth;
        Nets[end_node][start_node].remaining_bandwidth = total_bandwidth;
        Nets[end_node][start_node].network_hire = network_hire;
    }

    vector<ResumeInfo> Consumers(consumer_nodes);//vector序号为消费节点编号
    TotalNeed = 0;//消费节点总需求
    for (unsigned long j = 5 + links; j < 5 + links + consumer_nodes; ++j)
    {
        int num, node_NO, need_bandwidth;
        read.str("");
        read << topo[j];
        read >> num >> node_NO >> need_bandwidth;
        Consumers[num].need_bandwidth = need_bandwidth;
        Consumers[num].node_NO = node_NO;
        TotalNeed += need_bandwidth;
    }


    //TODO:接入MCMF类
    MCMF mcmf;
    //初始化
    mcmf.setConsumersAndNets(Consumers, Nets);
    mcmf.setSeverCostAndTotalNeed(SeverCost, TotalNeed);


    auto curSeverNo = mcmf.getSeverNo();

//    auto newServerNo = mcmf.getNewServe(curSeverNo);

    //传入服务器编号
    mcmf.setServers(curSeverNo);

    mcmf.mainFunction();//主方法

    mcmf.getBestPath();//输出标准格式最优路径

    cout<<endl<<mcmf.getTotalCost()<<endl;
    auto newSever=curSeverNo;
    for (int l = 0; l <1000000 ; ++l)
    {
        mcmf.setServers(curSeverNo);

        mcmf.mainFunction();//主方法

        mcmf.getBestPath();//输出标准格式最优路径

        cout<<endl<<mcmf.getTotalCost()<<endl;
//        newSever = mcmf.getNewServe(curSeverNo);
//        curSeverNo=newSever;
//        double dE = mcmf.evaluateCost(newSever) - mcmf.evaluateCost(curSeverNo);
    }


    //模拟退火

//    double t = T;
//    srand((unsigned int) time(NULL));
////    auto curSeverNo = mcmf.getSeverNo();
//    auto newSever = curSeverNo;
//    auto bestSever = curSeverNo;
//
//    int P_L = 0;
//    int P_F = 0;
//    while (1)       //外循环，主要更新参数t，模拟退火过程
//    {
//        for (int i = 0; i < ILOOP; i++) //内循环，寻找在一定温度下的最优值
//        {
//            newSever = mcmf.getNewServe(curSeverNo);
//            double dE = mcmf.evaluateCost(newSever) - mcmf.evaluateCost(curSeverNo);
//            if (dE < 0)   //如果找到更优值，直接更新
//            {
//                curSeverNo = newSever;
//                P_L = 0;
//                P_F = 0;
//            } else
//            {
//                double rd = rand() / (RAND_MAX + 1.0);
//                if (exp(dE / t) > rd && exp(dE / t) < 1)   //如果找到比当前更差的解，以一定概率接受该解，并且这个概率会越来越小
//                    curSeverNo = newSever;
//                P_L++;
//            }
//            if (P_L > LIMIT)
//            {
//                P_F++;
//                break;
//            }
//        }
//        if (mcmf.evaluateCost(curSeverNo) < mcmf.evaluateCost(newSever))
//            bestSever = curSeverNo;
//        if (P_F > OLOOP || t < EPS)
//            break;
//        t *= DELTA;
//    }
//
//
//    mcmf.setServers(bestSever);
//
//    mcmf.mainFunction();//主方法
//
//    mcmf.getBestPath();//输出标准格式最优路径
//
//    cout<<endl<<mcmf.getTotalCost()<<endl;


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
//    cout << topo_file;
    write_result(topo_file, filename);
}



