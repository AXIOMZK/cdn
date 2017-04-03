
#include "deploy.h"
#include "MCMF.h"


using namespace std;

//C++整数规划+模拟退火方案


//通过调用alarm来设置计时器，然后继续做别的事情。当计时器计时到0时，信号发送，处理函数被调用。

void timer(int sig)
{
    isExit = 0;
}

void deploy_server(char *topo[MAX_EDGE_NUM], int line_num, char *filename)
{
//    //执行定时器函数
//    signal(SIGALRM, timer);
//    alarm(86); //定时80s

    int TotalNeed;//所有消费节点总需求
    int SeverCost;
    stringstream read(topo[0]);
    unsigned long links, consumer_nodes, network_nodes;//链路数，消费节点数，网络节点数
    read >> network_nodes >> links >> consumer_nodes;
    //单台服务器成本
    SeverCost = atoi(topo[2]);

    vector<vector<LinkInfo>> Nets(network_nodes, vector<LinkInfo>(network_nodes));
    for (unsigned long i = 4; i < 4 + links; ++i)
    {
        int start_node, end_node;
        int total_bandwidth, network_hire;
        read.str("");
        read << topo[i];
        read >> start_node >> end_node >> total_bandwidth >> network_hire;
        Nets[start_node][end_node].total_bandwidth = total_bandwidth;
        Nets[start_node][end_node].network_hire = network_hire;
        Nets[end_node][start_node].total_bandwidth = total_bandwidth;
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

    if (links > 4000)
    {
        //TODO:直连方案(大数据直接输出直连)
        read.str("");
        stringstream &results = read;
        results << consumer_nodes << "\n";
        for (unsigned long k = 0; k < consumer_nodes; ++k)
        {
            results << "\n" << Consumers[k].node_NO << " " << k << " " << Consumers[k].need_bandwidth;
        }
        const string &strDirect = results.str();

        char *topo_file = (char *) strDirect.c_str();
        write_result(topo_file, filename);
    } else
    {

        //TODO:接入msmf类
        //初始化
        MCMF mcmf(Consumers, Nets, SeverCost, TotalNeed);


/*        //传入服务器编号
        auto curSeverNo=mcmf.getSeverNo();

        mcmf.setServers(curSeverNo);

        mcmf.mainFunction();//主方法

        mcmf.getBestPath();//输出标准格式最优路径
//        PRINT("%lf\n", mcmf.getTotalCost());
        cout << endl << mcmf.getTotalCost() << endl;
        auto newSever = curSeverNo;
        for (int l = 0; l < 1000; ++l)
        {
            newSever = mcmf.getNewServe(curSeverNo);
            double dE = mcmf.evaluateCost(newSever) - mcmf.evaluateCost(curSeverNo);
            curSeverNo = newSever;
            cout<<dE<<endl;
        }*/

        double T;     //初始温度
        double EPS;     //终止温度
        double DELTA;     //温度衰减率
        int LIMIT;    //概率选择上限
        int OLOOP;      //外循环次数
        int ILOOP;      //内循环次数

        double p0;   //退火接受参数

        if (links > 2000)
        {
            //执行定时器函数
            signal(SIGALRM, timer);
            alarm(86); //定时80s
            T = 1000;     //初始温度
            EPS = 1e-9;     //终止温度
            DELTA = 0.98;     //温度衰减率
            LIMIT = 10;     //概率选择上限
            OLOOP = 20000;      //外循环次数
            ILOOP = 1000;      //内循环次数
            p0 = 1.0;
        } else if (links > 1000)
        {
            //执行定时器函数
            signal(SIGALRM, timer);
            alarm(88); //定时80s
            T = 1000;     //初始温度
            EPS = 1e-9;     //终止温度
            DELTA = 0.98;     //温度衰减率
            LIMIT = 10;     //概率选择上限
            OLOOP = 20000;      //外循环次数
            ILOOP = 1000;      //内循环次数
            p0 = 1.0;
        } else
        {
            //执行定时器函数
            signal(SIGALRM, timer);
            alarm(89); //定时80s
//            T = 650;         //初始温度
//            EPS = 1e-9;      //终止温度
//            DELTA = 0.95;    //温度衰减率
//            LIMIT = 20;      //概率选择上限
//            OLOOP = 300;      //外循环次数
//            ILOOP = 400;      //内循环次数
//            p0=1.22;
            T = 900;         //初始温度
            EPS = 1e-9;      //终止温度
            DELTA = 0.98;    //温度衰减率
            LIMIT = 33;      //概率选择上限
            OLOOP = 500;      //外循环次数
            ILOOP = 450;      //内循环次数
            p0 = 1.2;
        }

        //TODO:模拟退火


        double t = T;
        int P_L = 0;
        int P_F = 0;
        srand((unsigned int) time(NULL));

        auto curSeverNo = mcmf.getSeverNo();
        auto newSever = curSeverNo;

        auto bestSever1 = curSeverNo;//局部最优
        //    auto bestSever2 = curSeverNo;//全局最优

        int bestCost = mcmf.evaluateCost(curSeverNo);
        int maxCost = bestCost;
        int curCost = bestCost;//当前的费用
        auto bestPath=mcmf.paths;//保存最优路径

        while (isExit)       //外循环，主要更新参数t，模拟退火过程
        {
//            cout<<"==========================P_F:"<<P_F<<endl;
            for (int i = 0; i < ILOOP; i++) //内循环，寻找在一定温度下的最优值
            {
                if (!isExit)break;
                newSever = mcmf.getNewServe(curSeverNo);
                int newCost = mcmf.evaluateCost(newSever);
                double dE = newCost - curCost;
                if (dE < 0)   //如果找到更优值，直接更新
                {
                    curSeverNo = newSever;
                    curCost = newCost;
//                    cout<<newCost<<endl;
                    if (newCost < bestCost)
                    {
                        bestCost=newCost;
                        bestSever1 = newSever;
                        bestPath=mcmf.paths;
                    }

                    P_L = 0;
                    P_F = 0;
                } else
                {
                    double rd = rand() / (RAND_MAX + 1.0);
                    if (exp(dE / t) > rd && exp(dE / t) < p0)
                        //如果找到比当前更差的解，以一定概率接受该解，并且这个概率会越来越小
                    {
                        curSeverNo = newSever;
                        curCost = newCost;
//                        cout<<"BadCost="<<curCost<<endl;
                    }
                    P_L++;
//                    cout<<"              P_L="<<P_L<<endl;
                }
                if (P_L > LIMIT)
                {
                    P_F++;
//                    P_L=0;//TODO:是否要加？
                    break;
                }
            }
            //        if (mcmf.evaluateCost(curSeverNo) < mcmf.evaluateCost(newSever))
            //            bestSever2 = curSeverNo;
            if (P_F > OLOOP || t < EPS)
                break;
            t *= DELTA;
        }

        PRINT("\n======================================\n最优解\n");

//        mcmf.setServers(bestSever1);

//        mcmf.mainFunction();//主方法
        mcmf.setBestPath(bestPath);
        PRINT("\n%s\n", mcmf.getBestPath().c_str());//输出标准格式最优路径
        //    cout << mcmf.getBestPath();
        PRINT("\n总成本:%d/%d\n", bestCost, maxCost);

        printf("\n总成本:%d/%d\n", bestCost, maxCost);
        //    cout << endl << mcmf.getTotalCost() << endl;

        const string &strtemp = mcmf.getBestPath();
        char *topo_file = (char *) strtemp.c_str();
        write_result(topo_file, filename);
    }

//    char* topo_file=(char *)results.str().c_str();//这是错误的
//streamstring在调用str()时，会返回临时的string对象。而因为是临时的对象，所以它在整个表达式结束后将会被析构。
    //   如果需要进一步操作string对象，先把其值赋给一个string变量后再操作。
    //alarm定时器，捕捉SIGALRM信号
    //捕捉函数用 signal(SIGALRM, funcPtr);
    // 需要输出的内容
//    char *topo_file = (char *) "17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";

    // 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)

}



