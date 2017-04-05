//
// Created by 张琨 on 2017/3/29.
//

//重构函数／


#include "MCMF.h"


void MCMF::setServers(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &SeverNo)
{
    set<int> SeverNum;
    PRINT("\n===================\n");
    for (auto it = SeverNo.begin(); it != SeverNo.end(); ++it)
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
            if(!isExit)return;
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
    setTotalServerCost(SeverNo);
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
    this->Consumers = Consumers;
    this->Nets = Nets;
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
}

MCMF::MCMF(const vector<ResumeInfo> &Consumers, const vector<vector<LinkInfo>> &Nets, int SeverCost, double TotalNeed)
{
    setConsumersAndNets(Consumers, Nets);
    setSeverCostAndTotalNeed(SeverCost, TotalNeed);
}

set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big>
MCMF::getNewServe1(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &oldServe)
{
    auto newServe = oldServe;
    if (newServe.size() > minSeverNum)
    {
        newServe.erase(newServe.begin());
        //temp=newServe;
    }
    return newServe;
}





//得到新服务器编号(变异)
set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big>
MCMF::getNewServe(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &oldServe, bool flag)
{

   // set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big>
   // temp;
    //非递归版本
    auto newServe = oldServe;
    auto old_size = consumer_nodes;
//    srand((unsigned int) time(NULL));此处不需要加
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
    if (old_size > 300)
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

    } else if (old_size > 100)
    {
        //中型数据
        //随机删除一个服务器,小型数据
        a1 = 20;
        //随机添加一个服务器,小型数据
        a2 = 40;
        //随机按比例删除服务器，中大型数据
        a3 = 50;
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

        //随机删除一个服务器,小型数据
        a1 = 20;
        //随机添加一个服务器,小型数据
        a2 = 40;
        //随机按比例删除服务器，中大型数据
        a3 = 65;
        //随机按比例增加服务器，中大型数据
        a4 = 85;
        //优先删除所能提供带宽最小的服务器
        a5 = 85;
        //优先添加所能提供带宽最大的服务器
        a6 = 85;
        //完全产生新服务器
        a7 = 95;
        //随机添加t1个服务器,再随机删除t2个服务器
        if (flag) {
//随机删除一个服务器,小型数据
            a1 = 30;
            //随机添加一个服务器,小型数据
            a2 = 60;
            //随机按比例删除服务器，中大型数据
            a3 = 60;
            //随机按比例增加服务器，中大型数据
            a4 = 60;
            //优先删除所能提供带宽最小的服务器
            a5 = 60;
            //优先添加所能提供带宽最大的服务器
            a6 = 90;
            //完全产生新服务器
            a7 = 93;
            //随机添加t1个服务器,再随机删除t2个服务器
        }
    } else
    {
        //小型数据
        //随机删除一个服务器,小型数据
        a1 = 20;
        //随机添加一个服务器,小型数据
        a2 = 40;
        //随机按比例删除服务器，中大型数据
        a3 = 50;
        //随机按比例增加服务器，中大型数据
        a4 = 60;
        //优先删除所能提供带宽最小的服务器
        a5 = 80;
        //优先添加所能提供带宽最大的服务器
        a6 = 90;
        //完全产生新服务器
        a7 = 95;
        //随机添加t1个服务器,再随机删除t2个服务器
        //a7~100
    }


    while (newServe.size() > maxServerNum || t_provide < TotalNeed)
    {
        if (!isExit)return newServe;
        double flag = 100.0 * ((double) rand()) / (RAND_MAX + 0.0001);

//        cout<<flag<<endl;
        if (flag >= 0 && flag < a1)
        {
            //随机删除一个服务器,小型数据
            if (newServe.size() > minSeverNum)
            {
                int pos = (int) (rand() % newServe.size());
//                cout<<pos<<endl;
                auto it = newServe.begin();
                while (pos--) it++;
                newServe.erase(it);

                //再删除一个
                int pos1 = (int) (rand() % newServe.size());
//                cout<<pos<<endl;
                auto it1 = newServe.begin();
                while (pos1--) it1++;
                newServe.erase(it1);
            }
           // cout<<"删除一个           111111111"<<endl;

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
                newServe.insert(pair);

                int pos1 = (int) (rand() % network_nodes);
                SeverNoAndAroundBandwidth pair1;
                pair1.ServerNo = pos1;
                pair1.ServeAroundBandwidth = ServeAroundBandwidth[pos1];
                newServe.insert(pair1);
            }
          //  cout<<"添加一个    222"<<endl;
        } else if (flag >= a2 && flag < a3)
        {
            //随机按比例删除服务器，中大型数据
            double ra = (double) (8 + rand() % 8) / 100.0;//8%-16%
            //基数
            int base = (int) (ra * consumer_nodes);
            //实际删除数
            int countDel = 1 + (int) ((double) base * (0.0001 + newServe.size() - minSeverNum) /
                                      (0.0001 + consumer_nodes - minSeverNum));

            while (countDel-- && newServe.size() > minSeverNum)
            {
                int pos = (int) (rand() % newServe.size());
                auto it = newServe.begin();
                while (pos--) it++;
                newServe.erase(it);
            }

          //  cout<<"随机按比例删除服务器    3"<<endl;

        } else if (flag >= a3 && flag < a4)
        {
            //随机按比例增加服务器，中大型数据
            double ra = (double) (8 + rand() % 8) / 100.0;//8%-16%
            //基数
            int base = (int) (ra * consumer_nodes);
            //实际增加数
            int countAdd = 1 + (int)
                    ((double) base *
                     (1.0 - (0.0001 + newServe.size() - minSeverNum) /
                            (0.0001 + consumer_nodes - minSeverNum)));

            while (countAdd-- && newServe.size() < maxServerNum)
            {
                unsigned long temp_size = newServe.size();
                while (newServe.size() == temp_size)
                {
                    int pos = (int) (rand() % network_nodes);
                    SeverNoAndAroundBandwidth pair;
                    pair.ServerNo = pos;
                    pair.ServeAroundBandwidth = ServeAroundBandwidth[pos];
                    newServe.insert(pair);
                }
            }

          //  cout<<"随机按比例添加服务器    44444"<<endl;
        } else if (flag >= a4 && flag < a5)
        {
            //优先删除所能提供带宽最小的服务器
            if (newServe.size() > minSeverNum)
            {
                newServe.erase(newServe.begin());
                 //temp=newServe;
            }
            cout<<"优先删除所能提供带宽最小的服务器    99999999"<<endl;
        }
/*        else if (flag == 4)
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
           /* unsigned long temp_size = newServe.size();
            auto it = AllNodeAroundBandwidth.rbegin();
            while (newServe.size() == temp_size)
            {
                newServe.insert(*(it++));
            }*/
            //随机删除一个
            int pos = (int) (rand() % network_nodes);
            SeverNoAndAroundBandwidth pair;
            pair.ServerNo = pos;
            pair.ServeAroundBandwidth = ServeAroundBandwidth[pos];
            newServe.insert(pair);
            //随机增加一个
            int pos1 = (int) (rand() % network_nodes);
            SeverNoAndAroundBandwidth pair1;
            pair1.ServerNo = pos1;
            pair1.ServeAroundBandwidth = ServeAroundBandwidth[pos1];
            newServe.insert(pair1);


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
            unsigned long temp_size = (int) newServe.size();
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
            while (newServe.size() != temp_size + addCount)
            {
                int pos = (int) (rand() % network_nodes);
                SeverNoAndAroundBandwidth pair;
                pair.ServerNo = pos;
                pair.ServeAroundBandwidth = ServeAroundBandwidth[pos];
                newServe.insert(pair);
            }
            //删除操作
            while (delCount--)
            {
                int pos = (int) (rand() % newServe.size());
//                cout << "pos:" << pos << "--" << newServe.size() << endl;
                auto it = newServe.begin();
                while (pos--) it++;
                newServe.erase(it);
            }

        }
        t_provide = 0;
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
    auto temp = ServeAroundBandwidth;
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
        pair.ServeAroundBandwidth = ServeAroundBandwidth[Consumers[l].node_NO];
        SeverNo.insert(pair);
    }
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
    widthvalues.clear();
    serverfit.clear();
    preVertex.clear();
    array.clear();
    vector<int>().swap(values);
    vector<int>().swap(widthvalues);
    vector<double>().swap(serverfit);
    vector<int>().swap(preVertex);
    vector<int>().swap(array);
    values.resize((unsigned long) maxpoint);
    widthvalues.resize((unsigned long) maxpoint);
    serverfit.resize((unsigned long) maxpoint);
    preVertex.resize((unsigned long) maxpoint);
    array.resize((unsigned long) maxpoint);
    int index = 0;
    while (!stop)
    {
        // cout <<" 距离是："<<mapswidth[5][0]<<","<<mapswidth[0][3]<<endl;
//两种情况终止循环1.流量都已经满足2.选择了Int_max

//TODO:Dijkstra函数
        Dijkstra();
        if(!isExit)
            return;
        index = maxpoint - 1;
        vector<int> trace;
        trace.push_back(index);
        array[0] = index;
        //记录array，用于计算最小带宽。
        int i = 0;
        while (preVertex[index] != -1)
        {
            if(!isExit)
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
        //printvalues();
        //
    }
    stop = false;//用完后复原
    //获得服务器价值比例
    setServerFit();

    //遍历结束后输出总费用

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
        int widthdir=0;
        for (int i = 0; i < array.size(); ++i) {
            if(array[i]==maxpoint-2) {

                widthdir = array[i-1];
                break;
            }

        }


        //保存服务器输出流量价值
        //int serverdir=array[array.size()-2];
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
        //cout<<"values[dir]__"<<values[dir]<<"dir__"<<dir<<endl;
        widthvalues[widthdir] = widthvalues[widthdir] + distance[maxpoint - 1] * minwidth;
       // cout<<"widthvalues[widthdir]__"<<widthvalues[widthdir]<<"widthdir__"<<widthdir<<endl;

        if (isenough())
        {
            PRINT("\n需求已满足!\n");
//                  cout << "需求已满足";
        }
    }

}


//输出每个消费节点在流量满足的情况下，需要的路径费用
void MCMF::printvalues()
{   //int count=0;
    //TODO：values有问题！！！！！！！！
    for (int i = 0; i < network_nodes; i++)
    {
        if (values[i] != 0)
        {
           // count+=values[i];
           // cout << "values[i]___" << values[i] << "   i:" << i << endl;
            PRINT("消费节点:%d,累计路径费用是:%d\n", i, values[i]);
        }
//            cout << "消费节点" << i << ":" << "累计路径费用是：" << values[i] << endl;
    }

//    int widthcount=0;
//    for (int i = 0; i < network_nodes; i++)
//    {
//        if (widthvalues[i] != 0)
//        { widthcount+=widthvalues[i];
//        cout << "widthvalues[i]___" << widthvalues[i] << "   i:" << i << endl;
//        }
//        PRINT("消费节点:%d,累计路径费用是:%d\n", i, values[i]);
////            cout << "消费节点" << i << ":" << "累计路径费用是：" << values[i] << endl;
//    }
//    if(count==widthcount){
//        cout<<"ggggggggggggggggggghgggg"<<endl;
//    }
//    else
//    { cout<<"count______"<<count<<endl;
//       cout<<"widthcount___________"<<widthcount<<endl;
//    }
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

void MCMF::setServerFit() {
    double widthcount=0;

    for (int i = 0; i < network_nodes; i++)
    {
        if (widthvalues[i] != 0) {
            widthcount += widthvalues[i];
        }
    }

    for (int i = 0; i < network_nodes; i++)
    {
        if (widthvalues[i] != 0) {
            serverfit[i]=widthvalues[i]/widthcount;

        }
    }

}


void MCMF::setBestPath(vector<vector<int>>&tpaths)
{
    paths=tpaths;
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
    if(!isExit)
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
    read << sizeLinks << "\n";
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
            read << "\n" << Consumers[k].node_NO << " " << k << " " << s;
        }
    }

    return read.str();
}


