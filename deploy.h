#ifndef __ROUTE_H__
#define __ROUTE_H__

#include <cmath>
#include "lib_io.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <unistd.h>

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
int SeverCost;//单台服务器成本
unsigned long maxServer;//最大服务器数，即消费节点数
unsigned long links, consumer_nodes, network_nodes;//链路数，消费节点数，网络节点数
vector<int> ServeAroundBandwidth(network_nodes);//序号为服务器所连的节点号，值为评估带宽
vector<vector<LinkInfo>> Nets(network_nodes, vector<LinkInfo>(network_nodes));
vector<ResumeInfo> Consumers(consumer_nodes);//vector序号为消费节点编号


void deploy_server(char *graph[MAX_EDGE_NUM], int edge_num, char *filename);

void timer(int sig);

int getServerCost(vector<SeverNoAndAroundBandwidth> &v);//得到部署服务器的成本
set<SeverNoAndAroundBandwidth,Bandwidth_From_Small_To_Big> getNewServe(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &oldServe);
//模拟退火产生新的服务器编号组合

#endif
