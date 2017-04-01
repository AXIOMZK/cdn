//
// Created by 张琨 on 2017/3/29.
//


#ifndef CDN_MCMF_H
#define CDN_MCMF_H

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

class MCMF
{
private:
    vector<int> ServeAroundBandwidth;//序号为服务器所连的节点号，值为评估带宽
    int maxpoint;
    bool stop = false;
    vector<vector<int> > mapscost;
    vector<vector<int> > mapswidth;
//    set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> SeverNo;//直连方案服务器信息

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
    vector<vector<int> > paths;

    //消费节点连接的网络节点编号
    set<int> ConsumerNum;
    //网络节点编号索引相连消费节点带宽
    map<int, int> NodesLinkConsumerNeed;
    //网络节点编号索引相连消费节序号
    map<int, int> NodesLinkConsumerNO;

public:
    MCMF(const vector<ResumeInfo> &Consumers, const vector<vector<LinkInfo>> &Nets,int SeverCost, double TotalNeed);

    void setConsumersAndNets(const vector<ResumeInfo> &Consumers, const vector<vector<LinkInfo> > &Nets);

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

    void setBestPath();//输出标准答案格式

    string getBestPath();

    //得到部署服务器的成本
    void setTotalServerCost(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &v);

    int getLinksTotalCost();

    int getServerTotalCost();

    int getTotalCost();

    int evaluateCost(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &sever);


};


#endif //CDN_MCMF_H
