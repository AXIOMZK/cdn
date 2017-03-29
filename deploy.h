#ifndef __ROUTE_H__
#define __ROUTE_H__
#include <stack>
#include <cmath>
#include "lib_io.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <unistd.h>
#include <climits>

using namespace std;
//服务器部署编号(连接的节点号)，评估带宽结构体
struct SeverNoAndAroundBandwidth
{
    int ServerNo;
    int ServeAroundBandwidth;
};
//链路信息(双向)
struct LinkInfo
{
    int total_bandwidth = 0;
    int remaining_bandwidth = 0;
    int network_hire = 0;
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
bool stop = false;
int maxpoint;
double TotalNeed;//所有消费节点总需求
int SeverCost;//单台服务器成本
unsigned long maxServer;//最大服务器数，即消费节点数
unsigned long links, consumer_nodes, network_nodes;//链路数，消费节点数，网络节点数
vector<int> ServeAroundBandwidth(network_nodes);//序号为服务器所连的节点号，值为评估带宽
vector<vector<LinkInfo>> Nets(network_nodes, vector<LinkInfo>(network_nodes));
vector<ResumeInfo> Consumers(consumer_nodes);//vector序号为消费节点编号
//vector<vector<int >> mapscost(network_nodes + 2,vector<int >(network_nodes + 2));
//vector<vector<int >> mapswidth(network_nodes + 2,vector<int >(network_nodes + 2));
//vector<int> values(consumer_nodes);

//判断某点是否在vector中
bool isInVector(const vector<int> vec, const int a);

void deploy_server(char *graph[MAX_EDGE_NUM], int edge_num, char *filename);

void timer(int sig);

int getServerCost(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &v);//得到部署服务器的成本
set<SeverNoAndAroundBandwidth,Bandwidth_From_Small_To_Big> getNewServe(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &oldServe);
//模拟退火产生新的服务器编号组合


int InitMap(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &SeverNo);
void Dijkstra(
        const int numOfVertex,    /*节点数目*/
        const int startVertex,    /*源节点*/
        vector<vector<int >>map,          /*有向图邻接矩阵*/
        int *distance,            /*各个节点到达源节点的距离*/
        int *prevVertex           /*各个节点的前一个节点*/
);

//判断消费节点与主汇点的带宽是否都变为0了，如果是，说明流量已经满足，可以终止最短路径的寻找了。否则继续寻找
bool isEnough();

int printvalues();
void decreaseandprintf(int array[], vector<int> trace, int distance[]);
int getTotalLinksCosts();
#endif
