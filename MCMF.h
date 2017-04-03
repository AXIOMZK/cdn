//
// Created by 张琨 on 2017/3/29.
//

#ifndef CDN_MCMF_H
#define CDN_MCMF_H

#include "global.h"
#include <map>
#include <vector>
#include <set>
#include <climits>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>

#ifdef _DEBUG
#define PRINT   printf
#else
#define PRINT(...)
#endif

using namespace std;


struct SeverNoAndAroundBandwidth
{
    int ServerNo;
    int ServeAroundBandwidth;
};
//链路信息(双向)
struct LinkInfo
{
    int total_bandwidth = 0;
    int network_hire = INT_MAX;
};
//消费节点信息结构体
struct ResumeInfo
{
    int node_NO;//消费节点连接的网络节点编号
    int need_bandwidth;//需求带宽
};

struct Bandwidth_From_Small_To_Big
{
    bool operator()(const SeverNoAndAroundBandwidth &left, const SeverNoAndAroundBandwidth &right) const
    {
        return (left.ServeAroundBandwidth < right.ServeAroundBandwidth);
    }
};

struct SeverSetAndCost
{
    set<int> SetSeverNO;//服务器编号集合
    int cost;//对应的成本
};

struct pro_serverFromSmallToBig
{
    bool operator()(const SeverSetAndCost &left, const SeverSetAndCost &right) const
    {
        return (left.cost < right.cost);
    }
};

struct server
{
    vector<int> serverNO;//一共net_node,bit来对染色体进行编码

    int fit = 0;//适应值
    double rfit = 0;//相对的fit值，即所占的百分比
    double cfit = 0;//积累概率
};

class MCMF
{
private:
    //某网络节点作为服务器时的最大提供带宽
    vector<int> ServeAroundBandwidth;//序号为服务器所连的节点号，值为评估带宽
    //所以节点的评估带宽存储
    set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> AllNodeAroundBandwidth;
    int maxpoint;
    bool stop = false;
    vector<vector<int> > mapscost;
    vector<vector<int> > mapswidth;
    set<int> SeverDirect;//消费节点必须直连的服务器节点信息
    int DirectBandwidth;//SeverDirect总评估带宽
    double TotalNeed;//所有消费节点总需求
    int SeverCost;//单台服务器成本
    int ServerTotalCost;//服务器总成本
    unsigned long maxServerNum;//最大服务器数，即消费节点数
    unsigned long minSeverNum;//最小服务器数
    vector<vector<LinkInfo> > Nets;
    vector<ResumeInfo> Consumers;//vector序号为消费节点编号
    unsigned long consumer_nodes, network_nodes;//链路数，消费节点数，网络节点数

    vector<int> distance;/*各个节点到达源节点的距离*/
    vector<int> preVertex;/*各个节点的前一个节点*/
    vector<int> array;
    vector<int> values;


    //消费节点连接的网络节点编号
    set<int> ConsumerNum;
    //网络节点编号索引相连消费节点带宽
    map<int, int> NodesLinkConsumerNeed;
    //网络节点编号索引相连消费节点序号
    map<int, int> NodesLinkConsumerNO;

public:
    vector<vector<int> > paths;

    MCMF(const vector<ResumeInfo> &Consumers, const vector<vector<LinkInfo>> &Nets, int SeverCost, double TotalNeed);

    void setConsumersAndNets(const vector<ResumeInfo> &Consumers, const vector<vector<LinkInfo>> &Nets);

    void setServers(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &SeverNo);

    void setSeverCostAndTotalNeed(int SeverCost, double TotalNeed);

    //模拟退火产生新的服务器编号组合
    set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big>
    getNewServe(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &oldServe);


    void setServeAroundBandwidth();

    set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> getSeverNo();

    void mainFunction();

    bool isenough();

    void Dijkstra();

    void decreaseAndPrintf(vector<int> trace);

    void printvalues();

    void setBestPath(vector<vector<int>> &tpaths);//输出标准答案格式

    string getBestPath();

    //得到部署服务器的成本
    void setTotalServerCost(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &v);

    int getLinksTotalCost();

    int getServerTotalCost();

    int getTotalCost();

    int evaluateCost(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &sever);

    void setSeverDirect();












    //遗传相关

    vector<SeverSetAndCost> pro_server;//服务器编号集群未编码
    void setPro_server(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &curSever);

    int Max_Point;//二进制网络节点数

    //  server popcurrent [PopulationSize];    // 初始种群规模
    vector<server> popcurrent;

    vector<server> popnext;    // 更新后种群规模仍为；

    void init_popcurrent();//将十组Server转化为十组基因，进行种群的初始化

    //将个体二进制字节流转化为服务器编号，用于计算目标函数
    vector<int> getServerFromBit(const server &singlepopcurrent);


    //根据个体的适应度进行排序
    void SortAndChoosePopcurrent();//选择操作

    // 基于概率分布的轮盘法选择
    void randompickup_new();

    void crossover();//交叉操作

    void mutation();//突变


    double r8_uniform_ab(double a, double b, int &seed);//生成a~b之间均匀分布的数字

    set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big>
    getNewGA(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &oldServe);
};


#endif //CDN_MCMF_H
