//
// Created by 张琨 on 2017/3/29.
//

#include "MCMF.h"

void MCMF::getServers(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &SeverNo)
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


void MCMF::getConsumersAndNets(const vector<ResumeInfo> &Consumers, const vector<vector<LinkInfo>> &Nets)
{
    this->Consumers = Consumers;
    this->Nets = Nets;
    consumer_nodes = Consumers.size();
    network_nodes = Nets.size();
//    mapscost(network_nodes + 2,vector<int >(network_nodes + 2));
}

MCMF::MCMF():mapscost(network_nodes + 2,vector<int >(network_nodes + 2)),mapswidth(network_nodes + 2,vector<int >(network_nodes + 2)) {

}
