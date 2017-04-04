//
// Created by 张琨 on 2017/3/29.
//

//重构函数


#include "MCMF.h"


void MCMF::setServers(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &SeverNo)
{
    set<int> SeverNum;
    //TODO:服务器转换，加上直连服务器
    auto newServe = SeverNo;
    //取并集合
    for (auto it = SeverDirect.begin(); it != SeverDirect.end(); ++it)
    {
        SeverNoAndAroundBandwidth pair;
        pair.ServerNo = *it;
        pair.ServeAroundBandwidth = ServeAroundBandwidth[*it];
        newServe.insert(pair);
    }


    PRINT("\n===================\n");
    for (auto it = newServe.begin(); it != newServe.end(); ++it)
    {
        SeverNum.insert((*it).ServerNo);
        PRINT("%d\n", (*it).ServerNo);
//        cout << (*it).ServerNo << endl;
    }

    //TODO:反复调用是否要做处理
    mapswidth.clear();
    mapscost.clear();
    vector<vector<int>>().swap(mapswidth);
    vector<vector<int>>().swap(mapscost);

    mapscost.resize(network_nodes + 2, vector<int>(network_nodes + 2));
    mapswidth.resize(network_nodes + 2, vector<int>(network_nodes + 2));

    //新版本(优化直连变换)
    for (int i = 0; i < network_nodes + 2; i++)
    {
        //如果i是服务器也是消费节点，不用计算路径费用
        for (int j = 0; j < network_nodes + 2; j++)
        {
            if (!isExit)return;
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


                    //j也是服务器点
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
                        if (ConsumerNum.count(i))
                        {
                            //mapscost[i][j] = 0;
                            //mapswidth[i][j] = NodesLinkConsumerNeed[i];
                            //修改
                            mapscost[i][j] = INT_MAX;
                            mapswidth[i][j] = 0;

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
                    {   //修改
                        //i也是服务器
                        if (SeverNum.count(i))
                        {
                            mapscost[i][j] = INT_MAX;
                            mapswidth[i][j] = 0;
                        } else
                        {
                            mapscost[i][j] = 0;
                            mapswidth[i][j] = NodesLinkConsumerNeed[i];
                        }
                    }
                        //j是主源点
                    else if (j == network_nodes)
                    {
                        //判断i是否为服务点
                        if (SeverNum.count(i))
                        {   //修改
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
                        //修改
                        //j是服务器点
                        if (SeverNum.count(j))
                        {
                            mapscost[i][j] = INT_MAX;
                            mapswidth[i][j] = 0;
                        } else
                        {
                            mapscost[i][j] = 0;
                            mapswidth[i][j] = NodesLinkConsumerNeed[j];
                        }
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

    //老版本
    /*for (int i = 0; i < network_nodes + 2; i++)
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
                    //j也是服务器点

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
                        if (ConsumerNum.count(i))
                        {
                            mapscost[i][j] = 0;
                            mapswidth[i][j] = NodesLinkConsumerNeed[i];
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
                        mapswidth[i][j] = NodesLinkConsumerNeed[i];
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
                        mapswidth[i][j] = NodesLinkConsumerNeed[j];
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
    }*/
    setTotalServerCost(newServe);
    //TODO：清空paths
    paths.clear();
    vector<vector<int>>().swap(paths);

/*    for (int l = 0; l < network_nodes + 2; ++l)
    {
        for (int i = 0; i < network_nodes + 2; ++i)
        {
            cout << mapswidth[l][i] << " ";
        }
        cout << endl;
    }
    cout <<"---------"<< mapswidth[7][38] << " " << mapscost[7][38] << endl;
    cout <<"---------"<< Nets[7][38].total_bandwidth << " " << Nets[7][38].network_hire << endl;
    cout << mapswidth[4][11] << " " << mapscost[4][11] << endl;
    cout << mapswidth[43][44] << " " << mapscost[43][44] << endl;
    cout << mapswidth[7][5] << " " << mapscost[7][5] << endl;
    cout << mapswidth[13][14] << " " << mapscost[13][14] << endl;
    cout << mapswidth[50][13] << " " << mapscost[50][13] << endl;
    cout << mapswidth[51][38] << " " << mapscost[51][38] << endl;*/

}


void MCMF::setConsumersAndNets(const vector<ResumeInfo> &Consumers, const vector<vector<LinkInfo>> &Nets)
{
//    this->Consumers = Consumers;
    this->Consumers.assign(Consumers.begin(), Consumers.end());
//    this->Nets = Nets;
    this->Nets.assign(Nets.begin(), Nets.end());
    consumer_nodes = Consumers.size();
    network_nodes = Nets.size();
    maxServerNum = consumer_nodes;
    maxpoint = (int) (network_nodes + 2);

    for (unsigned long k = 0; k < consumer_nodes; ++k)
    {
        ConsumerNum.insert(Consumers[k].node_NO);
        NodesLinkConsumerNeed[Consumers[k].node_NO] = Consumers[k].need_bandwidth;
        NodesLinkConsumerNO[Consumers[k].node_NO] = (int) k;
    }

    setServeAroundBandwidth();

    setSeverDirect();

}

MCMF::MCMF(const vector<ResumeInfo> &Consumers, const vector<vector<LinkInfo>> &Nets, int SeverCost, double TotalNeed)
{
    setConsumersAndNets(Consumers, Nets);
    setSeverCostAndTotalNeed(SeverCost, TotalNeed);
}

//得到新服务器编号(变异)
set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big>
MCMF::getNewServe(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &oldServe)
{
    //非递归版本
    auto sizeDirect = SeverDirect.size();
    auto newServe = oldServe;
    int t_provide = 0;//服务器可能的最大提供流量
    //随机删除一个服务器,小型数据
    double a1;
    //随机添加一个服务器,小型数据
    double a2;
    //随机按比例删除服务器，中大型数据
    double a3;
    //随机按比例增加服务器，中大型数据
    double a4;
    //优先删除所能提供带宽最小的服务器
    double a5;
    //优先添加所能提供带宽最大的服务器
    double a6;
    //完全产生新服务器
    double a7;
    //随机添加t1个服务器,再随机删除t2个服务器
    //a7~100
    if (consumer_nodes > 300)
    {
        //大型数据
        //随机删除一个服务器,小型数据
        a1 = 5;
        //随机添加一个服务器,小型数据
        a2 = 10;
        //随机按比例删除服务器，中大型数据
        a3 = 20;
        //随机按比例增加服务器，中大型数据
        a4 = 30;
        //优先删除所能提供带宽最小的服务器
        a5 = 70;
        //优先添加所能提供带宽最大的服务器
        a6 = 90;
        //完全产生新服务器
        a7 = 95;
        //随机添加t1个服务器,再随机删除t2个服务器
        //a7~100

    } else if (consumer_nodes > 100)
    {
        //中型数据
        //随机删除一个服务器,小型数据
        a1 = 30;
        //随机添加一个服务器,小型数据
        a2 = 60;
        //随机按比例删除服务器，中大型数据
        a3 = 60;
        //随机按比例增加服务器，中大型数据
        a4 = 60;
        //优先删除所能提供带宽最小的服务器
        a5 = 80;
        //优先添加所能提供带宽最大的服务器
        a6 = 90;
        //完全产生新服务器
        a7 = 93;
        //随机添加t1个服务器,再随机删除t2个服务器
        //a7~100
    } else
    {
        //小型数据
        //随机删除一个服务器,小型数据
        a1 = 30;
        //随机添加一个服务器,小型数据
        a2 = 60;
        //随机按比例删除服务器，中大型数据
        a3 = 60;
        //随机按比例增加服务器，中大型数据
        a4 = 60;
        //优先删除所能提供带宽最小的服务器
        a5 = 80;
        //优先添加所能提供带宽最大的服务器
        a6 = 90;
        //完全产生新服务器
        a7 = 94;
        //随机添加t1个服务器,再随机删除t2个服务器
        //a7~100
    }
    while (newServe.size() + sizeDirect > maxServerNum || t_provide < TotalNeed)
    {
        if (!isExit)return newServe;
        double flag = 100.0 * ((double) rand()) / (RAND_MAX + 0.0001);

//        cout<<flag<<endl;
        if (flag >= 0 && flag < a1)
        {
            //随机删除一个服务器,小型数据
            if (newServe.size() + sizeDirect > minSeverNum && !newServe.empty())
            {
                int pos = (int) (rand() % newServe.size());
//                cout<<pos<<endl;
                auto it = newServe.begin();
                while (pos--) it++;
                newServe.erase(it);
            } else
            {
                //随机添加一个服务器,小型数据
                unsigned long temp_size = newServe.size();
                while (newServe.size() == temp_size)
                {
                    int pos = (int) (rand() % network_nodes);
                    SeverNoAndAroundBandwidth pair;
                    pair.ServerNo = pos;
                    pair.ServeAroundBandwidth = ServeAroundBandwidth[pos];
                    if (SeverDirect.count(pos))continue;
                    newServe.insert(pair);
                }
            }

        } else if (flag >= a1 && flag < a2)
        {
            //随机添加一个服务器,小型数据
            unsigned long temp_size = newServe.size();
            while (newServe.size() == temp_size)
            {
                int pos = (int) (rand() % network_nodes);
                SeverNoAndAroundBandwidth pair;
                pair.ServerNo = pos;
                pair.ServeAroundBandwidth = ServeAroundBandwidth[pos];
                if (SeverDirect.count(pos))continue;
                newServe.insert(pair);
            }

        } else if (flag >= a2 && flag < a3)
        {
            //随机按比例删除服务器，中大型数据
            double ra = (double) (8 + rand() % 8) / 100.0;//8%-16%
            //基数
            int base = (int) (ra * maxServerNum);
            //实际删除数
            int countDel = 1 + (int) ((double) base * (0.0001 + newServe.size() + sizeDirect - minSeverNum) /
                                      (0.0001 + maxServerNum - minSeverNum));

            while (countDel-- && (newServe.size() + sizeDirect > minSeverNum) && !newServe.empty())
            {
                int pos = (int) (rand() % newServe.size());
                auto it = newServe.begin();
                while (pos--) it++;
                newServe.erase(it);
            }
        } else if (flag >= a3 && flag < a4)
        {
            //随机按比例增加服务器，中大型数据
            double ra = (double) (8 + rand() % 8) / 100.0;//8%-16%
            //基数
            int base = (int) (ra * maxServerNum);
            //实际增加数
            int countAdd = 1 + (int)
                    ((double) base *
                     (1.0 - (0.0001 + newServe.size() + sizeDirect - minSeverNum) /
                            (0.0001 + maxServerNum - minSeverNum)));

            while (countAdd-- && (newServe.size() + sizeDirect < maxServerNum))
            {
                unsigned long temp_size = newServe.size();
                while (newServe.size() == temp_size)
                {
                    int pos = (int) (rand() % network_nodes);
                    SeverNoAndAroundBandwidth pair;
                    pair.ServerNo = pos;
                    pair.ServeAroundBandwidth = ServeAroundBandwidth[pos];
                    if (SeverDirect.count(pos))continue;
                    newServe.insert(pair);
                }
            }
        } else if (flag >= a4 && flag < a5)
        {
            //优先删除所能提供带宽最小的服务器
            if (newServe.size() + sizeDirect > minSeverNum && !newServe.empty())
                newServe.erase(newServe.begin());
            else
            {
                //随机添加一个服务器,小型数据
                unsigned long temp_size = newServe.size();
                while (newServe.size() == temp_size)
                {
                    int pos = (int) (rand() % network_nodes);
                    SeverNoAndAroundBandwidth pair;
                    pair.ServerNo = pos;
                    pair.ServeAroundBandwidth = ServeAroundBandwidth[pos];
                    if (SeverDirect.count(pos))continue;
                    newServe.insert(pair);
                }
            }

        }
            /*else if (flag == 4)
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
            }*/
        else if (flag >= a5 && flag < a6)
        {
            //优先添加所能提供带宽最大的服务器
            unsigned long temp_size = newServe.size();
            auto it = AllNodeAroundBandwidth.rbegin();
            while (newServe.size() == temp_size)
            {
                if (!SeverDirect.count((*it).ServerNo))
                    newServe.insert(*it);
                ++it;
            }

        } else if (flag >= a6 && flag < a7)
        {
            //完全产生新服务器
            newServe.clear();
            //产生的个数,随机产生minSeverNum~maxServerNum的整数
            int t = (int) (minSeverNum + (int) (rand() % (1 + maxServerNum - minSeverNum)));
            while (newServe.size() != t)
            {
                int pos = (int) (rand() % network_nodes);
                SeverNoAndAroundBandwidth pair;
                pair.ServerNo = pos;
                pair.ServeAroundBandwidth = ServeAroundBandwidth[pos];
                if (SeverDirect.count(pos))continue;
                newServe.insert(pair);
            }
        }

            /*else
            {
                //随机删除t1个服务器,再随机添加t2个服务器
                //t1,t2都小于等于newServe.size()
                unsigned long temp_size = (int) newServe.size();
                int t1 = 1 + (int) (rand() % temp_size);
                int t2 = 1 + (int) (rand() % temp_size);
                for (int i = 0; i < t1; ++i)
                {
                    newServe.erase(newServe.begin());
                }

                while (newServe.size() != temp_size - t1 + t2)
                {
                    int pos = (int) (rand() % network_nodes);
                    SeverNoAndAroundBandwidth pair;
                    pair.ServerNo = pos;
                    pair.ServeAroundBandwidth = ServeAroundBandwidth[pos];
                    newServe.insert(pair);
                }
            }*/
        else
        {
            //随机添加t1个服务器,再随机删除t2个服务器
            unsigned long temp_size = (int) newServe.size() + sizeDirect;
            //添加删除后的服务器数,保证服务器数在minSeverNum~maxServerNum之间
            int stdCount = (int) (minSeverNum + (int) (rand() % (1 + maxServerNum - minSeverNum)));
            //随机出一个添加的个数
            int addCount;
            if (stdCount > temp_size)
            {
                //std-temp ~ max-temp

                addCount = (int) (stdCount - temp_size + 1 + (rand() % (maxServerNum + 1 - temp_size)));

            } else
            {
                //0 ~ max-tem
                addCount = (int) (rand() % (maxServerNum + 1 - temp_size)) + 1;
            }
            //删除的个数
            int delCount = (int) (temp_size + addCount - stdCount);
            //TODO:测试
//            cout << endl << temp_size << "+" << addCount << "-" << delCount << "=" << stdCount << endl;
            //添加操作
            while (newServe.size() + sizeDirect != temp_size + addCount)
            {
                int pos = (int) (rand() % network_nodes);
                SeverNoAndAroundBandwidth pair;
                pair.ServerNo = pos;
                pair.ServeAroundBandwidth = ServeAroundBandwidth[pos];
                if (SeverDirect.count(pos))continue;
                newServe.insert(pair);
            }
            //删除操作
            while (delCount-- && !newServe.empty())
            {
                int pos = (int) (rand() % newServe.size());
//                cout << "pos:" << pos << "--" << newServe.size() << endl;
                auto it = newServe.begin();
                while (pos--) it++;
                newServe.erase(it);
            }

        }
        t_provide = DirectBandwidth;
        for (auto item = newServe.begin(); item != newServe.end(); ++item)
        {
            t_provide += (*item).ServeAroundBandwidth;
        }
    }

    return newServe;

    //递归版本
/*    auto newServe = oldServe;
    srand((unsigned int) time(NULL));
    int flag = 1 + rand() % 5;
    if (flag == 1)
    {
        //随机删除一个服务器
        if (newServe.size() > 1)
        {
            int pos = (int) (rand() % newServe.size());
            auto it = newServe.begin();
            while (pos--) it++;
            newServe.erase(it);
        }

    } else if (flag == 2)
    {
        if (newServe.size() > 1)
            //优先删除所能提供带宽最小的服务器
            newServe.erase(newServe.begin());

    } else if (flag == 3)
    {
        //随机添加一个服务器
        unsigned long temp_size = newServe.size();
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
//        if(newServe.size()==1)
//            return newServe;
        //随机删除t1个服务器,再随机添加t2个服务器
        unsigned long temp_size = (int) newServe.size();
        int t1 = 1 + (int) (rand() % newServe.size());
        int t2 = 1 + (int) (rand() % newServe.size());
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
    if (newServe.size() > maxServerNum)
    {
//        return oldServe;
        return getNewServe(oldServe);
    }
    // 2、服务器总供给不小于总需求
    int t_provide = 0;
    for (auto item = newServe.begin(); item != newServe.end(); ++item)
    {
        t_provide += (*item).ServeAroundBandwidth;
    }
    if (t_provide < TotalNeed)
    {
//        return oldServe;
        return getNewServe(oldServe);
    }
    return newServe;*/
}

void MCMF::setTotalServerCost(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &v)
{
    ServerTotalCost = SeverCost * (int) v.size();
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
        SeverNoAndAroundBandwidth pair;
        pair.ServeAroundBandwidth = AroundBandwidth;
        pair.ServerNo = i;
        AllNodeAroundBandwidth.insert(pair);
    }

    //得到服务器评估最小数
    vector<int> temp;
    temp.assign(ServeAroundBandwidth.begin(), ServeAroundBandwidth.end());
    sort(temp.rbegin(), temp.rend());
    int sum = 0;
    int k = 0;
    for (; k < temp.size();)
    {
        sum += temp[k];
        ++k;
        if (sum >= TotalNeed)
            break;
    }
    minSeverNum = (unsigned long) k;
}

set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> MCMF::getSeverNo()
{
    //服务器初始编号，即直连方案编号，外加一个参考可能提供带宽量,按带宽从小到大排序
    set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> SeverNo;
    for (int l = 0; l < Consumers.size(); ++l)
    {
        SeverNoAndAroundBandwidth pair;
        pair.ServerNo = Consumers[l].node_NO;
        //取与必须直连服务器集合的差集合
        if (SeverDirect.count(pair.ServerNo))continue;
        pair.ServeAroundBandwidth = ServeAroundBandwidth[Consumers[l].node_NO];
        SeverNo.insert(pair);
    }
//    set<int>setNew;
//    set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> newServe;
//    //取差集
//    set_difference(setSever.begin(), setSever.end(), SeverDirect.begin(), SeverDirect.end(), inserter(setNew, setNew.begin()));

    return SeverNo;
}


void MCMF::setSeverCostAndTotalNeed(int SeverCost, double TotalNeed)
{
    MCMF::SeverCost = SeverCost;
    MCMF::TotalNeed = TotalNeed;
}

void MCMF::mainFunction()
{//TODO:values初始化大小问题
    values.clear();
    preVertex.clear();
    array.clear();
    vector<int>().swap(values);
    vector<int>().swap(preVertex);
    vector<int>().swap(array);
    values.resize((unsigned long) maxpoint);
    preVertex.resize((unsigned long) maxpoint);
    array.resize((unsigned long) maxpoint);
    int index = 0;
    while (!stop)
    {
        // cout <<" 距离是："<<mapswidth[5][0]<<","<<mapswidth[0][3]<<endl;
//两种情况终止循环1.流量都已经满足2.选择了Int_max

//TODO:Dijkstra函数
        Dijkstra();
        if (!isExit)
            return;
        index = maxpoint - 1;
        vector<int> trace;
        trace.push_back(index);
        array[0] = index;
        //记录array，用于计算最小带宽。
        int i = 0;
        while (preVertex[index] != -1)
        {
            if (!isExit)
                return;
            trace.push_back(preVertex[index]);
            //arrays[k][++i]=preVertex[index];
            i++;
            array[i] = preVertex[index];
            index = preVertex[index];
        }
        //判断流量是否满足，若满足跳出循环
        //数组排序寻找最小的带宽
        decreaseAndPrintf(trace);
    }
    stop = false;//用完后复原
    //遍历结束后输出总费用
//    printvalues();
    //TODO:setPath
//    setBestPath(paths);//仅仅只能调用一次
}

void MCMF::Dijkstra()
{
    distance.resize((unsigned long) maxpoint);
    int numOfVertex = maxpoint;
    int startVertex = maxpoint - 2;
    vector<bool> isInS;                 //是否已经在S集合中
    isInS.reserve(0);
    isInS.assign((unsigned long) numOfVertex, false);   //初始化，所有的节点都不在S集合中


    //每次开始执行后设置为不能执行
    //stop=true;

    /*初始化distance和prevVertex数组*/
    for (int i = 0; i < numOfVertex; ++i)
    {
        if (!isExit)return;
        distance[i] = mapscost[startVertex][i];
        if (mapscost[startVertex][i] < INT_MAX)
            preVertex[i] = startVertex;
        else
            preVertex[i] = -1;       //表示还不知道前一个节点是什么
    }
    preVertex[startVertex] = -1;

    /*开始使用贪心思想循环处理不在S集合中的每一个节点*/
    isInS[startVertex] = true;          //开始节点放入S集合中


    int u = startVertex;

    for (int i = 1; i < numOfVertex; i++)      //这里循环从1开始是因为开始节点已经存放在S中了，还有numOfVertex-1个节点要处理
    {

        /*选择distance最小的一个节点*/
        int nextVertex = u;
        int tempDistance = INT_MAX;
        for (int j = 0; j < numOfVertex; ++j)
        {
            if (!isExit)return;
            if ((isInS[j] == false) && (distance[j] < tempDistance))//寻找不在S集合中的distance最小的节点
            {   //有新点加入时，允许下次执行，否则不再执行
                //stop=false;
                nextVertex = j;
                tempDistance = distance[j];
            }
        }

        isInS[nextVertex] = true;
        u = nextVertex;
        //放入S集合中

        //下一次寻找的开始节点

        //当消费点被收录后，已获得一个最短路径，处理相邻矩阵后，继续下一轮搜索，注意搜索停止条件。
        if (nextVertex == maxpoint - 1) { break; }

        /*更新distance*/
        //如果

        for (int j = 0; j < numOfVertex; j++)
        {
            if (!isExit)return;
            if (isInS[j] == false && mapscost[u][j] < INT_MAX)
            {
                int temp = distance[u] + mapscost[u][j];
                if (temp < distance[j])
                {
                    distance[j] = temp;
                    preVertex[j] = u;
                }
            }
        }
    }
}

void MCMF::decreaseAndPrintf(vector<int> trace)
{
    //先判断是否有路径达到终点
    int check = 0;
    for (auto it = trace.begin(); it != trace.end(); it++)
    {
        if (*it == maxpoint - 2)
        {
            check = 1;
            break;
        }
    }

    if (check == 0)
    {
        stop = true;
        //没有路径达到终点时 判断流量是否已经满足
        if (!isenough())
        {
            PRINT("\n需求未满足\n");
//            cout << "需求未满足" << endl;

//            int count=0;
//            //显示多少节点多少流量未满足,即判断与汇点的带宽
//            for (int m = 0; m < maxpoint; m++)
//            {
//                if (mapswidth[m][maxpoint - 1] != 0) { count++; }
//            }


            //选择优化方案
        } else
        {
            PRINT("\n需求已满足\n");
//            cout << "需求已满足" << endl;
            //输出
//            printvalues();
        }


    } else
    {
        //可以到达最终点
        //寻找最小带宽
        int minwidth = INT_MAX;
        //数组是逆向排序的
        //寻找最小带宽
        for (int j = (int) trace.size() - 2; j > 0; j--)
        {//cout << array[j]<< array[j-1]<<" -- ";
            if (mapswidth[array[j]][array[j - 1]] < minwidth)
            {
                minwidth = mapswidth[array[j]][array[j - 1]];
                //cout << mapswidth[j][j-1]<<" -- ";
            }
        }

        //修改网络带宽
        PRINT("\n路径流量:%d,链路:", minwidth);
//        cout << "最小" << minwidth;
        //缩减带宽，带宽为0，成本设为最大。
        for (int j = (int) trace.size() - 2; j > 0; j--)
        {
            if (mapswidth[array[j]][array[j - 1]] != INT_MAX)
            {
                mapswidth[array[j]][array[j - 1]] = mapswidth[array[j]][array[j - 1]] - minwidth;
                //cout <<"shcuhu"<< mapswidth[array[j]][array[j-1]];
                if (mapswidth[array[j]][array[j - 1]] == 0)
                    mapscost[array[j]][array[j - 1]] = INT_MAX;
            }
        }
        //TODO:dir的大小问题
        //保存费用流方向,array存放的是逆向流
        int dir = array[1];
        //cout<<"dir:"<<dir<<endl;
        //if(stop==false){
//        cout << "路径：";

        vector<int> temPath;
        while (!trace.empty())
        {
            PRINT("--%d", *trace.rbegin());
//            cout << *trace.rbegin() << " -- ";
            temPath.push_back(*trace.rbegin());
//                trace.resize(trace.size()-1);
            trace.erase(--trace.end());
        }
        temPath.push_back(minwidth);
        paths.push_back(temPath);

//        cout << " 距离是：" << distance[maxpoint - 1] << endl;

//        PRINT("\n费用:%d,", distance[maxpoint - 1] * minwidth);

//        cout << "单条路径费用是：" << distance[maxpoint - 1] * minwidth << endl;

        values[dir] = values[dir] + distance[maxpoint - 1] * minwidth;

        if (isenough())
        {
            PRINT("\n需求已满足!\n");
//                  cout << "需求已满足";
        }
    }

}


//输出每个消费节点在流量满足的情况下，需要的路径费用
void MCMF::printvalues()
{
    //TODO：values有问题！！！！！！！！
    for (int i = 0; i < network_nodes; i++)
    {
        if (values[i] != 0);
        PRINT("消费节点:%d,累计路径费用是:%d\n", i, values[i]);
//            cout << "消费节点" << i << ":" << "累计路径费用是：" << values[i] << endl;
    }
}


//判断消费节点与主汇点的带宽是否都变为0了，如果是，说明流量已经满足，可以终止最短路径的寻找了。否则继续寻找
bool MCMF::isenough()
{
    bool enough = false;
    int count = 0;
    for (int m = 0; m < maxpoint; m++)
    {
        if (mapswidth[m][maxpoint - 1] == 0) { count++; }
    }

    if (count == maxpoint)
    {
        stop = true;
        enough = true;
    }
    return enough;
}

void MCMF::setBestPath(vector<vector<int>> &tpaths)
{
    paths = tpaths;
    int sizeLinks = (int) paths.size();
    for (int k = 0; k < sizeLinks; ++k)
    {
        paths[k].erase(paths[k].begin());
        auto it = --(--paths[k].end());
        paths[k].erase(it);
        auto it2 = --(--paths[k].end());
        paths[k].insert(--paths[k].end(), NodesLinkConsumerNO[*(it2)]);
    }
}

int MCMF::getLinksTotalCost()
{
    int temCost = 0;
    for (int i = 0; i < values.size(); ++i)
    {
        temCost += values[i];
    }
    return temCost;
}

int MCMF::getServerTotalCost()
{
    return ServerTotalCost;
}

int MCMF::getTotalCost()
{
    //不满足流量时的惩罚因子
    int countSever = 0;
    //显示多少节点多少流量未满足,即判断与汇点的带宽
    for (int m = 0; m < maxpoint; m++)
    {
        if (mapswidth[m][maxpoint - 1] != 0) { countSever++; }
    }
    countSever *= SeverCost;
    return (getLinksTotalCost() + getServerTotalCost() + countSever);
}

int MCMF::evaluateCost(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &v)
{

    setServers(v);
    mainFunction();//主方法
    if (!isExit)
        return INT_MAX;
    int Tcost = getTotalCost();
    PRINT("总成本:%d", Tcost);
//    cout << "总成本:" << Tcost << endl;
    return Tcost;
}

string MCMF::getBestPath()
{
    stringstream read;
    int sizeLinks = (int) paths.size();
//    read << sizeLinks << "\n";
    read << "\n";
    //TODO:存当前消费节点的获得流量
    vector<int> tem(consumer_nodes, 0);
    for (int i = 0; i < sizeLinks; ++i)
    {
        tem[paths[i][paths[i].size() - 2]] += paths[i][paths[i].size() - 1];
        read << "\n";
        for (int j = 0; j < paths[i].size(); ++j)
        {
            read << paths[i][j];
            if (j != paths[i].size() - 1)
                read << " ";
        }
    }
    //TODO:修正评估错解
    for (int k = 0; k < tem.size(); ++k)
    {
        int s = Consumers[k].need_bandwidth - tem[k];
        if (s > 0)
        {
            sizeLinks++;
            read << "\n" << Consumers[k].node_NO << " " << k << " " << s;
        }
    }
    string result = read.str();
    read.str("");
    read << sizeLinks;
    result = read.str() + result;
    return result;
}

void MCMF::setSeverDirect()
{
    DirectBandwidth = 0;
    for (auto it = Consumers.begin(); it != Consumers.end(); ++it)
    {
        if ((*it).need_bandwidth > ServeAroundBandwidth[(*it).node_NO])
        {
            SeverNoAndAroundBandwidth pair;
            pair.ServeAroundBandwidth = (*it).need_bandwidth;
            pair.ServerNo = (*it).node_NO;
            SeverDirect.insert(pair.ServerNo);
            DirectBandwidth += (*it).need_bandwidth;
        }
    }
}




//TODO:遗传相关

void MCMF::init_popcurrent()   // 初始化 编码
{
    Max_Point = (int) network_nodes;
    // int random ;
    double sumFit = 0;
    // 从种群中的第1个染色体到第n个染色体
    for (int i = 0; i < pro_server.size(); i++)
    {
        server pair = server();
        pair.serverNO.resize((unsigned long) Max_Point, 0);
        // 从染色体的第1个基因位到第n个基因位====编码
        for (auto j = pro_server[i].SetSeverNO.begin(); j != pro_server[i].SetSeverNO.end(); j++)
        {
            pair.serverNO[(*j)] = 1;
        }
        //加上成本
        //TODO:适应度函数
        pair.cost = pro_server[i].cost;
        pair.fit = 50.0 * exp(8.0 * (StdFit - (double)pro_server[i].cost) / StdFit);
        cout<<"cost:"<<pair.cost<<"    fit:"<<pair.fit<<endl;
        popcurrent.push_back(pair);

        //用于轮盘赌选择法
        sumFit += popcurrent[i].fit;
    }

    //计算适应值得百分比，该参数是在用轮盘赌选择法时需要用到的
    for (int k = 0; k < popcurrent.size(); k++)
    {    //染色体适应度的百分比
        //TODO:适应度算错了吧
        popcurrent[k].rfit = (double) popcurrent[k].fit / sumFit;
    }
}


//得到去除必须直连点后的服务器
set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> MCMF::getServerFromBit(const server &singlepopcurrent)
{
    set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> set1;
    for (int i = 0; i < Max_Point; i++)
    {
        if (singlepopcurrent.serverNO[i] == 1)
        {
            SeverNoAndAroundBandwidth pair;
            pair.ServerNo = i;
            //去除必须直连点
            if (SeverDirect.count(i))continue;
            pair.ServeAroundBandwidth
                    = ServeAroundBandwidth[i];
            set1.insert(pair);
        }
    }
    return set1;
}


bool ServerFitFromBigToSmall(const server &m1, const server &m2)
{
    return m2.fit < m1.fit;
}

//根据适应度排序（由大到小）
//新旧解结合筛选
void MCMF::SortAndChoosePopcurrent(int x)          // 函数：选择个体；
{
    /*int i, j;
    server temp;                                // 中间变量

    for (i = 0; i < popcurrent.size() - 1; i++)                           // 根据个体适应度来排序；（冒泡法）
    {
        for (j = 0; j < popcurrent.size() - 1 - i; j++)
        {
            if (popcurrent[j + 1].fit > popcurrent[j].fit)
            {
                temp = popcurrent[j + 1];
                popcurrent[j + 1] = popcurrent[j];
                popcurrent[j] = temp;
            }
        }
    }*/

    vector<server> vecMerge;
    //... vec1,vec2赋值
    sort(popcurrent.begin(), popcurrent.end(), ServerFitFromBigToSmall);
    sort(popnext.begin(), popnext.end(), ServerFitFromBigToSmall);
    vecMerge.resize(popcurrent.size() + popnext.size());
    merge(popcurrent.begin(), popcurrent.end(), popnext.begin(), popnext.end(), vecMerge.begin());

    //取前50
    popcurrent.clear();
    popcurrent.assign(vecMerge.begin(), vecMerge.begin() + x);
    for (int i = 0; i < 50; ++i)
    {
        cout << "fit=" << popcurrent[i].fit << "   cost=" << popcurrent[i].cost << endl;
    }
    popnext.clear();
    double sum = 0;
    for (int j = 0; j < popcurrent.size(); ++j)
    {
        sum += popcurrent[j].fit;
    }
    for (int j = 0; j < popcurrent.size(); ++j)
    {
        popcurrent[j].rfit = popcurrent[j].fit / sum;
    }

}


//基于轮盘赌选择方法，进行基因型的选择
//sum越大，保存下来的概率越大。
void MCMF::randompickup_new()
{

    //计算相对概率，前两项精英至少保留1个
    // popnext.push_back(popcurrent[0]);
    // popnext.push_back(popcurrent[1]);

    //calculate the cumulative fitness,即计算积累概率
    popcurrent[0].cfit = popcurrent[0].rfit;

    for (int men = 1; men < popcurrent.size(); men++)
    {
        popcurrent[men].cfit = popcurrent[men - 1].cfit + popcurrent[men].rfit;
    }
    popnext.clear();
    for (int k = 0; k < popcurrent.size(); ++k)
    {
        double p = ((double) rand()) / (RAND_MAX + 0.00001);
        for (int i = 0; i < popcurrent.size(); ++i)
        {
            if (p < popcurrent[i].cfit)
            {
                popnext.push_back(popcurrent[i]);
                break;
            }
        }
    }
}


//生成ab之间均匀分布的数字。
double MCMF::r8_uniform_ab(double a, double b, int &seed)
{
    {
        int i4_huge = 2147483647;
        int k;
        double value;

        if (seed == 0)
        {
            std::cerr << "\n";
            std::cerr << "R8_UNIFORM_AB - Fatal error!\n";
            std::cerr << "  Input value of SEED = 0.\n";
            exit(1);
        }

        k = seed / 127773;

        seed = 16807 * (seed - k * 127773) - k * 2836;

        if (seed < 0)
        {
            seed = seed + i4_huge;
        }

        value = (double) (seed) * 4.656612875E-10;

        value = a + (b - a) * value;

        return value;
    }
}


void MCMF::crossover()              // 函数：交叉操作；
{

    // 交叉点控制在0到Max_Point-1之间；
    // 随机产生交叉点；
    //TODO:交叉算法是否有问题
    for (int i = 0; i < popnext.size() - 1; i += 2)
    {
        int random = (rand() % (Max_Point - 1) + 1);
        for (int j = 0; j < random; j++)
        {
            int temp1 = popnext[i].serverNO[j];
            int temp2 = popnext[i + 1].serverNO[j];
            popnext[i + 1].serverNO[j] = temp1;   // child 1 cross over
            popnext[i].serverNO[j] = temp2;   // child 2 cross over
        }
    }

    /* for(i =0;i<4; i++)
     {
         popnext[i ].fit= main_value(x(popnext[i]));        // 为新个体计算适应度值；
     }*/
}


void MCMF::mutation()               // 函数：变异操作；
{
    int randommutation;
    int randomlocation;
    //每个染色体有20%概率变异
    //TODO:改为next？
    /*for (int i = 0; i < popcurrent.size(); i++)
    {
        randommutation = rand() % 100;
        if (randommutation <= 20)
        {
            randomlocation = rand() % Max_Point;
            popcurrent[i].serverNO[randomlocation]
                    = popcurrent[i].serverNO[randomlocation] ^ 1;
        }
    }*/
    for (int i = 0; i < popnext.size(); i++)
    {
        randommutation = rand() % 100;
        if (randommutation <= 60)
        {
            int count = 1 + (int) (rand() % (network_nodes / 20));
            int t = 0;
            while (t != count)
            {
                randomlocation = rand() % Max_Point;
                //保证必须直连点不变异
                if (!SeverDirect.count(randomlocation))
                {
                    popnext[i].serverNO[randomlocation]
                            = popnext[i].serverNO[randomlocation] ^ 1;
                    ++t;
                }
            }

        }
    }
}

void MCMF::setPro_server(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &curSever)
{
    /* newSever = mcmf.getNewServe(curSeverNo);
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
 //                        cout<<bestCost<<endl;
         }

         P_L = 0;
         P_F = 0;
     }*/

    auto temSever = curSever;
    for (int j = 0; j < 100; ++j)
    {
        set<int> temp = SeverDirect;
        for (auto i = temSever.begin(); i != temSever.end(); ++i)
        {
            temp.insert((*i).ServerNo);
        }
        SeverSetAndCost pair;
        pair.SetSeverNO = temp;
        pair.cost = evaluateCost(temSever);
        //适应度标准值
        if (j == 0)StdFit = pair.cost;
        pro_server.push_back(pair);
        temSever = getNewGA(temSever);

        /*for (auto k = temp.begin(); k !=temp.end() ; ++k)
        {
            cout<<*k<<" ";
        }
        cout<<endl;*/
    }

}

/* int random ;
 int row ,col, value;
 //srand(time(0));
 random=rand ()%50;  //随机产生到之间的数；
 //变异操作也要遵从一定的概率来进行，一般设置为0到0.5之间
 //
 if(random ==25)                              // random==25的概率只有2%，即变异率为，所以是以小概率进行变异！！
 {
     col=rand ()%6;                            // 随机产生要变异的基因位号；
     row=rand ()%4;                            // 随机产生要变异的染色体号；

     if(popnext [row]. serverNO[col ]==0)             // 1变为；
     {
         popnext[row ].serverNO[ col]=1 ;
     }
     else if (popnext[ row].serverNO [col]==1)        // 0变为；
     {
         popnext[row ].serverNO[ col]=0;
     }
     popnext[row ].fit= main_value(x(popnext[row]));     // 计算变异后的适应度值；
     value=x (popnext[ row]);
     printf("\nMutation occured in popnext[%d] serverNO[%d]:=%d%d%d%d%d%d    value=%d   fitness=%d", row,col ,popnext[ row].serverNO [5],popnext[ row].serverNO [4],popnext[ row].serverNO [3],popnext[ row].serverNO [2],popnext[ row].serverNO [1],popnext[ row].serverNO [0],value, popnext[row ].fit);

     // 输出变异后的新个体；
 }*/


//得到新服务器编号(变异)
set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big>
MCMF::getNewGA(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &oldServe)
{
    //非递归版本
    auto sizeDirect = SeverDirect.size();
    auto newServe = oldServe;
    int t_provide = 0;//服务器可能的最大提供流量
    //随机删除一个服务器,小型数据
    double a1;
    //随机添加一个服务器,小型数据
    double a2;
    //随机按比例删除服务器，中大型数据
    double a3;
    //随机按比例增加服务器，中大型数据
    double a4;
    //优先删除所能提供带宽最小的服务器
    double a5;
    //优先添加所能提供带宽最大的服务器
    double a6;
    //完全产生新服务器
    double a7;
    //随机添加t1个服务器,再随机删除t2个服务器
    //a7~100
    if (consumer_nodes > 300)
    {
        //大型数据
        //随机删除一个服务器,小型数据
        a1 = 10;
        //随机添加一个服务器,小型数据
        a2 = 20;
        //随机按比例删除服务器，中大型数据
        a3 = 30;
        //随机按比例增加服务器，中大型数据
        a4 = 40;
        //优先删除所能提供带宽最小的服务器
        a5 = 70;
        //优先添加所能提供带宽最大的服务器
        a6 = 90;
        //完全产生新服务器
        a7 = 95;
        //随机添加t1个服务器,再随机删除t2个服务器
        //a7~100

    } else if (consumer_nodes > 100)
    {
        //中型数据
        //随机删除一个服务器,小型数据
        a1 = 30;
        //随机添加一个服务器,小型数据
        a2 = 60;
        //随机按比例删除服务器，中大型数据
        a3 = 60;
        //随机按比例增加服务器，中大型数据
        a4 = 60;
        //优先删除所能提供带宽最小的服务器
        a5 = 80;
        //优先添加所能提供带宽最大的服务器
        a6 = 90;
        //完全产生新服务器
        a7 = 93;
        //随机添加t1个服务器,再随机删除t2个服务器
        //a7~100
    } else
    {
        //小型数据
        //随机删除一个服务器,小型数据
        a1 = 30;
        //随机添加一个服务器,小型数据
        a2 = 60;
        //随机按比例删除服务器，中大型数据
        a3 = 60;
        //随机按比例增加服务器，中大型数据
        a4 = 60;
        //优先删除所能提供带宽最小的服务器
        a5 = 80;
        //优先添加所能提供带宽最大的服务器
        a6 = 90;
        //完全产生新服务器
        a7 = 94;
        //随机添加t1个服务器,再随机删除t2个服务器
        //a7~100
    }
    while (newServe.size() + sizeDirect > maxServerNum || t_provide < TotalNeed)
    {
        if (!isExit)return newServe;
        double flag = 100.0 * ((double) rand()) / (RAND_MAX + 0.0001);

//        cout<<flag<<endl;
        if (flag >= 0 && flag < a1)
        {
            //随机删除一个服务器,小型数据
            if (newServe.size() + sizeDirect > minSeverNum && !newServe.empty())
            {
                int pos = (int) (rand() % newServe.size());
//                cout<<pos<<endl;
                auto it = newServe.begin();
                while (pos--) it++;
                newServe.erase(it);
            } else
            {
                //随机添加一个服务器,小型数据
                unsigned long temp_size = newServe.size();
                while (newServe.size() == temp_size)
                {
                    int pos = (int) (rand() % network_nodes);
                    SeverNoAndAroundBandwidth pair;
                    pair.ServerNo = pos;
                    pair.ServeAroundBandwidth = ServeAroundBandwidth[pos];
                    if (SeverDirect.count(pos))continue;
                    newServe.insert(pair);
                }
            }

        } else if (flag >= a1 && flag < a2)
        {
            //随机添加一个服务器,小型数据
            unsigned long temp_size = newServe.size();
            while (newServe.size() == temp_size)
            {
                int pos = (int) (rand() % network_nodes);
                SeverNoAndAroundBandwidth pair;
                pair.ServerNo = pos;
                pair.ServeAroundBandwidth = ServeAroundBandwidth[pos];
                if (SeverDirect.count(pos))continue;
                newServe.insert(pair);
            }

        } else if (flag >= a2 && flag < a3)
        {
            //随机按比例删除服务器，中大型数据
            double ra = (double) (8 + rand() % 8) / 100.0;//8%-16%
            //基数
            int base = (int) (ra * maxServerNum);
            //实际删除数
            int countDel = 1 + (int) ((double) base * (0.0001 + newServe.size() + sizeDirect - minSeverNum) /
                                      (0.0001 + maxServerNum - minSeverNum));

            while (countDel-- && (newServe.size() + sizeDirect > minSeverNum) && !newServe.empty())
            {
                int pos = (int) (rand() % newServe.size());
                auto it = newServe.begin();
                while (pos--) it++;
                newServe.erase(it);
            }
        } else if (flag >= a3 && flag < a4)
        {
            //随机按比例增加服务器，中大型数据
            double ra = (double) (8 + rand() % 8) / 100.0;//8%-16%
            //基数
            int base = (int) (ra * maxServerNum);
            //实际增加数
            int countAdd = 1 + (int)
                    ((double) base *
                     (1.0 - (0.0001 + newServe.size() + sizeDirect - minSeverNum) /
                            (0.0001 + maxServerNum - minSeverNum)));

            while (countAdd-- && (newServe.size() + sizeDirect < maxServerNum))
            {
                unsigned long temp_size = newServe.size();
                while (newServe.size() == temp_size)
                {
                    int pos = (int) (rand() % network_nodes);
                    SeverNoAndAroundBandwidth pair;
                    pair.ServerNo = pos;
                    pair.ServeAroundBandwidth = ServeAroundBandwidth[pos];
                    if (SeverDirect.count(pos))continue;
                    newServe.insert(pair);
                }
            }
        } else if (flag >= a4 && flag < a5)
        {
            //优先删除所能提供带宽最小的服务器
            if (newServe.size() + sizeDirect > minSeverNum && !newServe.empty())
                newServe.erase(newServe.begin());
            else
            {
                //随机添加一个服务器,小型数据
                unsigned long temp_size = newServe.size();
                while (newServe.size() == temp_size)
                {
                    int pos = (int) (rand() % network_nodes);
                    SeverNoAndAroundBandwidth pair;
                    pair.ServerNo = pos;
                    pair.ServeAroundBandwidth = ServeAroundBandwidth[pos];
                    if (SeverDirect.count(pos))continue;
                    newServe.insert(pair);
                }
            }

        } else if (flag >= a5 && flag < a6)
        {
            //优先添加所能提供带宽最大的服务器
            unsigned long temp_size = newServe.size();
            auto it = AllNodeAroundBandwidth.rbegin();
            while (newServe.size() == temp_size)
            {
                if (!SeverDirect.count((*it).ServerNo))
                    newServe.insert(*it);
                ++it;
            }

        } else if (flag >= a6 && flag < a7)
        {
            //完全产生新服务器
            newServe.clear();
            //产生的个数,随机产生minSeverNum~maxServerNum的整数
            int t = (int) (minSeverNum + (int) (rand() % (1 + maxServerNum - minSeverNum)));
            while (newServe.size() != t)
            {
                int pos = (int) (rand() % network_nodes);
                SeverNoAndAroundBandwidth pair;
                pair.ServerNo = pos;
                pair.ServeAroundBandwidth = ServeAroundBandwidth[pos];
                if (SeverDirect.count(pos))continue;
                newServe.insert(pair);
            }
        } else
        {
            //随机添加t1个服务器,再随机删除t2个服务器
            unsigned long temp_size = (int) newServe.size() + sizeDirect;
            //添加删除后的服务器数,保证服务器数在minSeverNum~maxServerNum之间
            int stdCount = (int) (minSeverNum + (int) (rand() % (1 + maxServerNum - minSeverNum)));
            //随机出一个添加的个数
            int addCount;
            if (stdCount > temp_size)
            {
                //std-temp ~ max-temp

                addCount = (int) (stdCount - temp_size + 1 + (rand() % (maxServerNum + 1 - temp_size)));

            } else
            {
                //0 ~ max-tem
                addCount = (int) (rand() % (maxServerNum + 1 - temp_size)) + 1;
            }
            //删除的个数
            int delCount = (int) (temp_size + addCount - stdCount);
            //TODO:测试
//            cout << endl << temp_size << "+" << addCount << "-" << delCount << "=" << stdCount << endl;
            //添加操作
            while (newServe.size() + sizeDirect != temp_size + addCount)
            {
                int pos = (int) (rand() % network_nodes);
                SeverNoAndAroundBandwidth pair;
                pair.ServerNo = pos;
                pair.ServeAroundBandwidth = ServeAroundBandwidth[pos];
                if (SeverDirect.count(pos))continue;
                newServe.insert(pair);
            }
            //删除操作
            while (delCount-- && !newServe.empty())
            {
                int pos = (int) (rand() % newServe.size());
//                cout << "pos:" << pos << "--" << newServe.size() << endl;
                auto it = newServe.begin();
                while (pos--) it++;
                newServe.erase(it);
            }

        }
        t_provide = DirectBandwidth;
        for (auto item = newServe.begin(); item != newServe.end(); ++item)
        {
            t_provide += (*item).ServeAroundBandwidth;
        }
    }

    return newServe;

}

void MCMF::evaluateNextFit()
{

    for (int i = 0; i < popnext.size(); ++i)
    {
        double cost = evaluateCost(getServerFromBit(popnext[i]));
        popnext[i].cost = (int) cost;
        popnext[i].fit =
                50 * exp(8.0 * (StdFit - (double)popnext[i].cost) / StdFit);
        cout << popnext.size()<< "      cost" << cost <<"    fit"<<popnext[i].fit<< endl;
    }
}
