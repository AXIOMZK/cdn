//
// Created by 张琨 on 2017/3/29.
//

//重构函数
#include "MCMF.h"

void MCMF::setServers(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &SeverNo)
{

    //TODO:直连的服务器的编号，需要修改
    set<int> SeverNum;

    for (auto it = SeverNo.begin(); it != SeverNo.end(); ++it)
    {
        SeverNum.insert((*it).ServerNo);
        cout<<(*it).ServerNo<<endl;
    }

    //TODO:反复调用是否要做处理
    mapswidth.clear();
    mapscost.clear();
    vector<vector<int>>().swap(mapswidth);
    vector<vector<int>>().swap(mapscost);

    mapscost.resize(network_nodes + 2, vector<int>(network_nodes + 2));
    mapswidth.resize(network_nodes + 2, vector<int>(network_nodes + 2));

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
    }
    setTotalServerCost(SeverNo);
    //TODO：清空paths
    paths.clear();
    vector<vector<int>>().swap(paths);
//    for (int l = 0; l < network_nodes + 2; ++l)
//    {
//        for (int i = 0; i < network_nodes + 2; ++i)
//        {
//            cout << mapswidth[l][i] << " ";
//        }
//        cout << endl;
//    }
//    cout <<"---------"<< mapswidth[7][38] << " " << mapscost[7][38] << endl;
//    cout <<"---------"<< Nets[7][38].total_bandwidth << " " << Nets[7][38].network_hire << endl;
//    cout << mapswidth[4][11] << " " << mapscost[4][11] << endl;
//    cout << mapswidth[43][44] << " " << mapscost[43][44] << endl;
//    cout << mapswidth[7][5] << " " << mapscost[7][5] << endl;
//    cout << mapswidth[13][14] << " " << mapscost[13][14] << endl;
//    cout << mapswidth[50][13] << " " << mapscost[50][13] << endl;
//    cout << mapswidth[51][38] << " " << mapscost[51][38] << endl;


}


void MCMF::setConsumersAndNets(const vector<ResumeInfo> &Consumers, const vector<vector<LinkInfo>> &Nets)
{
    this->Consumers = Consumers;
    this->Nets = Nets;
    consumer_nodes = Consumers.size();
    network_nodes = Nets.size();
    maxServer = consumer_nodes;
    maxpoint = (int) (network_nodes + 2);

    for (unsigned long k = 0; k < consumer_nodes; ++k)
    {
        ConsumerNum.insert(Consumers[k].node_NO);
        NodesLinkConsumerNeed[Consumers[k].node_NO] = Consumers[k].need_bandwidth;
        NodesLinkConsumerNO[Consumers[k].node_NO] = (int) k;
    }

    setServeAroundBandwidth();
}

MCMF::MCMF()
{
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
    if (newServe.size() > maxServer)
    {
//        return oldServe;
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
    {
//        return oldServe;
        newServe = getNewServe(oldServe);
        return newServe;
    }
    return newServe;
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
    }
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
        index = maxpoint - 1;
        vector<int> trace;
        trace.push_back(index);
        array[0] = index;
        //记录array，用于计算最小带宽。
        int i = 0;
        while (preVertex[index] != -1)
        {
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
    printvalues();
    setBestPath();//仅仅只能调用一次
}

void MCMF::Dijkstra(
//        const int numOfVertex,    /*节点数目*/
//        const int startVertex,    /*源节点*/
//        int map[][maxpoint],          /*有向图邻接矩阵*/
//       vector<int> distance,            /*各个节点到达源节点的距离*/
//       vector<int> prevVertex           /*各个节点的前一个节点*/
)
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
    int minwidth = INT_MAX;
    //数组是逆向排序的
    for (int j = (int) trace.size() - 2; j > 0; j--)
    {//cout << array[j]<< array[j-1]<<" -- ";
        if (mapswidth[array[j]][array[j - 1]] < minwidth)
        {
            minwidth = mapswidth[array[j]][array[j - 1]];
            //cout << mapswidth[j][j-1]<<" -- ";
        }
    }
    int check = 0;
    for (auto it = trace.begin(); it != trace.end(); it++)
    {
        if (*it == maxpoint - 2)
        {
            check = 1;
            break;
        }
    }
    //判断是否还能到达终点
    if (check == 0)
    {
        stop = true;
        if (!isenough())
        {
            cout << "需求未满足" << endl;
            //显示多少节点多少流量未满足,即判断与汇点的带宽
            for (int i = 0; i < consumer_nodes; i++)
            {
                if (values[i] != 0)
                    cout << "消费节点" << i << ":" << "累计路径费用是：" << values[i] << endl;
            }
        }

        //选择优化方案

    } else
    {

        if (!isenough())
        {
            cout << "最小" << minwidth;
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
            cout << "路径：";

            vector<int> temPath;
            while (!trace.empty())
            {
                cout << *trace.rbegin() << " -- ";
                temPath.push_back(*trace.rbegin());
//                trace.resize(trace.size()-1);
                trace.erase(--trace.end());
            }
            temPath.push_back(minwidth);
            paths.push_back(temPath);
            cout << " 距离是：" << distance[maxpoint - 1] << endl;

            //if(min!=INT_MAX){
            cout << "单条路径费用是：" << distance[maxpoint - 1] * minwidth << endl;

            values[dir] = values[dir] + distance[maxpoint - 1] * minwidth;

            isenough();
        } else
            //输出各节点损耗
            printvalues();
    }
}

//输出每个消费节点在流量满足的情况下，需要的路径费用
void MCMF::printvalues()
{
    //TODO：values有问题！！！！！！！！
    // TODO：consumer_nodes换成了network_nodes
    for (int i = 0; i < network_nodes; i++)
    {
        if (values[i] != 0)
            cout << "消费节点" << i << ":" << "累计路径费用是：" << values[i] << endl;
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
        cout << "需求已满足" << endl;
        enough = true;
    }
    return enough;
}

void MCMF::setBestPath()
{
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
    return (getLinksTotalCost() + getServerTotalCost());
}

int MCMF::evaluateCost(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &v)
{
    setServers(v);
    mainFunction();//主方法
    int Tcost=getTotalCost();
    cout<<"总成本:"<<Tcost<<endl;
    return Tcost;
}

string MCMF::getBestPath()
{
    stringstream read;
    int sizeLinks = (int) paths.size();
    read << sizeLinks << "\n";

    for (int i = 0; i < sizeLinks; ++i)
    {
        read << "\n";
        for (int j = 0; j < paths[i].size(); ++j)
        {
            read << paths[i][j];
            if (j != paths[i].size() - 1)
                read << " ";
        }
    }

    cout << read.str();
    return std::string();
}

