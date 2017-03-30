//
// Created by 张琨 on 2017/3/29.
//

#include "MCMF.h"

void MCMF::setServers(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &SeverNo)
{
    //消费节点连接的网络节点编号
    set<int> ConsumerNum;
    for (unsigned long k = 0; k < consumer_nodes; ++k)
    {
        ConsumerNum.insert(Consumers[k].node_NO);
    }

    //TODO:直连的服务器的编号，需要修改
    set<int> SeverNum;
    for (auto it = SeverNo.begin(); it != SeverNo.end(); ++it)
    {
        SeverNum.insert((*it).ServerNo);
    }

    mapscost.resize(network_nodes + 2,vector<int >(network_nodes + 2));
    mapswidth.resize(network_nodes + 2,vector<int >(network_nodes + 2));

    for (int i = 0; i < network_nodes + 2; i++)
    {
        for (int j = 0; j < network_nodes + 2; j++)
        {
            //自身cost=0
            if (i == j)
            {
                mapscost[i][j] = INT_MAX;
                mapswidth[i][j] = 0;
            } else
            {
                //如果i是服务器点
                if (SeverNum.count(i))
                {
                    //j也是服务器点11

                    if (SeverNum.count(j))
                    {
                        mapscost[i][j] = INT_MAX;
                        mapswidth[i][j] = 0;
                    }
                        //j是主源点
                    else if (j == network_nodes)
                    {
                        mapscost[i][j] = 0;
                        mapswidth[i][j] = INT_MAX;
                    }
                        //j是主汇点
                    else if (j == network_nodes + 1)
                    {
                        //判断i是否为消费点
                        if (ConsumerNum.count(j))
                        {
                            mapscost[i][j] = 0;
                            mapswidth[i][j] = Consumers[i].need_bandwidth;
                        }
                            //i不是消费节点
                        else
                        {
                            mapscost[i][j] = INT_MAX;
                            mapswidth[i][j] = 0;
                        }

                    } else
                    {
                        mapscost[i][j] = Nets[i][j].network_hire;
                        mapswidth[i][j] = Nets[i][j].total_bandwidth;
                    }
                }
                    //i是消费节点
                else if (ConsumerNum.count(i))
                {
                    //j是服务器点
                    if (SeverNum.count(j))
                    {
                        //i是服务节点
                        if (SeverNum.count(i))
                        {
                            mapscost[i][j] = INT_MAX;
                            mapswidth[i][j] = 0;
                        } else
                        {
                            mapscost[i][j] = Nets[i][j].network_hire;
                            mapswidth[i][j] = Nets[i][j].total_bandwidth;
                        }
                    }
                        //j是主汇点
                    else if (j == network_nodes + 1)
                    {
                        mapscost[i][j] = 0;
                        mapswidth[i][j] = Consumers[i].need_bandwidth;
                    }
                        //j是主源点
                    else if (j == network_nodes)
                    {
                        //判断i是否为服务点
                        if (SeverNum.count(i))
                        {
                            mapscost[i][j] = 0;
                            mapswidth[i][j] = INT_MAX;
                        }
                            //i不是服务节点
                        else
                        {
                            mapscost[i][j] = INT_MAX;
                            mapswidth[i][j] = 0;
                        }

                    } else
                    {
                        mapscost[i][j] = Nets[i][j].network_hire;
                        mapswidth[i][j] = Nets[i][j].total_bandwidth;
                    }
                }

                    //i是主源点
                else if (i == network_nodes)
                {
                    //j是服务点
                    if (SeverNum.count(j))
                    {
                        mapscost[i][j] = 0;
                        mapswidth[i][j] = INT_MAX;
                    } else
                    {
                        mapscost[i][j] = INT_MAX;
                        mapswidth[i][j] = 0;
                    }
                }

                    //i是主汇点
                else if (i == network_nodes + 1)
                {
                    //j是消费点
                    if (ConsumerNum.count(j))
                    {
                        mapscost[i][j] = 0;
                        mapswidth[i][j] = Consumers[i].need_bandwidth;
                    } else
                    {
                        mapscost[i][j] = INT_MAX;
                        mapswidth[i][j] = 0;
                    }
                }
                    //普通节点
                else
                {
                    //j是主源点或主汇点
                    if (j == network_nodes || j == network_nodes + 1)
                    {
                        mapscost[i][j] = INT_MAX;
                        mapswidth[i][j] = 0;
                    } else
                    {
                        mapscost[i][j] = Nets[i][j].network_hire;
                        mapswidth[i][j] = Nets[i][j].total_bandwidth;
                    }
                }
            }
        }
    }
    for (int l = 0; l <network_nodes + 2 ; ++l)
    {
        for (int i = 0; i < network_nodes + 2; ++i)
        {
            cout<<mapswidth[l][i]<<" ";
        }
        cout<<endl;
    }
}


void MCMF::setConsumersAndNets(const vector<ResumeInfo> &Consumers, const vector<vector<LinkInfo>> &Nets)
{
    this->Consumers = Consumers;
    this->Nets = Nets;
    consumer_nodes = Consumers.size();
    network_nodes = Nets.size();
}

MCMF::MCMF():mapscost(network_nodes + 2,vector<int >(network_nodes + 2)),mapswidth(network_nodes + 2,vector<int >(network_nodes + 2)) {

}

//得到新服务器编号(变异)
set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big>
MCMF::getNewServe(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &oldServe)
{
//    int size = (int) oldServe.size();
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
        unsigned long temp_size =  newServe.size();
        while (newServe.size() == temp_size)
        {
            int pos = (int) (rand() % network_nodes);
            SeverNoAndAroundBandwidth pair;
            pair.ServerNo = pos;
            pair.ServeAroundBandwidth = ServeAroundBandwidth[pos];
            newServe.insert(pair);
        }
    } else if (flag == 4)
    {
        //随机添加一个服务器,再删除最小的服务器
        unsigned long temp_size = (int) newServe.size();
        while (newServe.size() == temp_size)
        {
            int pos = (int) (rand() % network_nodes);
            SeverNoAndAroundBandwidth pair;
            pair.ServerNo = pos;
            pair.ServeAroundBandwidth = ServeAroundBandwidth[pos];
            newServe.insert(pair);
        }
        newServe.erase(newServe.begin());
    } else
    {
        //随机删除t1个服务器,再随机添加t2个服务器
        unsigned long temp_size = (int) newServe.size();
        int t1 = (int) (rand() % newServe.size());
        int t2 = (int) (rand() % newServe.size());
        for (int i = 0; i < t1; ++i)
        {
            newServe.erase(newServe.begin());
        }

        while (newServe.size() == temp_size - t1 + t2)
        {
            int pos = (int) (rand() % network_nodes);
            SeverNoAndAroundBandwidth pair;
            pair.ServerNo = pos;
            pair.ServeAroundBandwidth = ServeAroundBandwidth[pos];
            newServe.insert(pair);
        }
    }
    //分支限界：
    // 1、服务器总数不超过消费节点数
    if (newServe.size() > maxServer)
    {
        newServe = getNewServe(oldServe);
        return newServe;
    }
    // 2、服务器总供给不小于总需求
    int t_provide = 0;
    for (auto item = newServe.begin(); item != newServe.end(); ++item)
    {
        t_provide += (*item).ServeAroundBandwidth;
    }
    if (t_provide < TotalNeed)
        newServe = getNewServe(oldServe);
    return newServe;
}

int MCMF::getTotalServerCost(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &v)
{
    return SeverCost * (int) v.size();
}


void MCMF::setServeAroundBandwidth()
{
    //得到服务器节点可能的最大输出带宽
    int AroundBandwidth = 0;
    for (int i = 0; i < network_nodes; ++i)
    {
        for (int j = 0; j < network_nodes; ++j)
        {
            AroundBandwidth += Nets[i][j].total_bandwidth;
        }
        //vector<int> ServeAroundBandwidth(network_nodes);//序号为服务器所连的节点号，值为评估带宽
        ServeAroundBandwidth.push_back(AroundBandwidth);
    }
}

set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> MCMF::getSeverNo()
{
    //服务器初始编号，即直连方案编号，外加一个参考可能提供带宽量,按带宽从小到大排序
    set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> SeverNo;
    for (int l = 0; l < Consumers.size(); ++l)
    {
        SeverNoAndAroundBandwidth pair;
        pair.ServerNo = l;
        pair.ServeAroundBandwidth = ServeAroundBandwidth[l];
        SeverNo.insert(pair);
    }
    return SeverNo;
}
