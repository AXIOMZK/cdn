//
// Created by 张琨 on 2017/3/29.
//

#ifndef CDN_MCMF_H
#define CDN_MCMF_H


#include <vector>
#include <set>
#include <climits>
#include <iostream>

using namespace std;


class MCMF
{
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
public:
    double TotalNeed;//所有消费节点总需求
    int SeverCost;//单台服务器成本
    unsigned long maxServer;//最大服务器数，即消费节点数
    unsigned long consumer_nodes, network_nodes;//链路数，消费节点数，网络节点数
    vector<int> ServeAroundBandwidth;//序号为服务器所连的节点号，值为评估带宽
    vector<vector<LinkInfo>> Nets;
    vector<ResumeInfo> Consumers;//vector序号为消费节点编号
    int maxpoint;
    bool stop = false;

    vector<vector<int >> mapscost;
    vector<vector<int >> mapswidth;
    set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> SeverNo;


    void getConsumersAndNets(const vector<ResumeInfo> &Consumers,const vector<vector<LinkInfo>> &Nets);
    void getServers(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &SeverNo);

    MCMF();
};


#endif //CDN_MCMF_H
