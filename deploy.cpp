#include "deploy.h"

#define T     3000    //初始温度
#define EPS   1e-8    //终止温度
#define DELTA 0.98    //温度衰减率
#define LIMIT 10000   //概率选择上限
#define OLOOP 1000    //外循环次数
#define ILOOP 15000   //内循环次数

#include "MCMF.h"
using namespace std;
//C++整数规划+模拟退火方案

int isExit = 1;//定时器标志
//通过调用alarm来设置计时器，然后继续做别的事情。当计时器计时到0时，信号发送，处理函数被调用。

void timer(int sig)
{
    isExit = 0;
}

//服务器总成本
int getServerCost(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &v)
{
    return SeverCost * (int) v.size();
}

//判断某点是否在vector中
bool isInVector(const vector<int> vec, const int a)
{
    bool result = false;
    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        if (*it == a)
            result = true;
    }
    return result;
}

//得到新服务器编号(变异)
set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big>
getNewServe(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &oldServe)
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


/*int getTotalLinksCosts()
{

    int distance[maxpoint];
    int preVertex[maxpoint];
    //TODO:int arrays[maxpoint][maxpoint] = {-1};
    int arrays[maxpoint][maxpoint];
    for (int j = 0; j <maxpoint ; ++j)
    {
        for (int i = 0; i < maxpoint; ++i)
        {
            arrays[maxpoint][maxpoint]=-1;
        }
    }
    int array[maxpoint];
    int index = 0;

    while (!stop)
    {

        // cout <<" 距离是："<<mapswidth[5][0]<<","<<mapswidth[0][3]<<endl;
//两种情况终止循环1.流量都已经满足2.选择了Int_max

        Dijkstra(maxpoint, maxpoint - 2, mapscost, distance, preVertex);

        index = maxpoint - 1;
        int k = -1;
        vector<int> trace;
        trace.push_back(index);
        array[0] = index;
        if (preVertex[index] != -1) { k = preVertex[index]; }
        //cout<<k<<endl;
        arrays[k][0] = index;
        //记录array，用于计算最小带宽。
        //记录arrays，保存链路
        int i = 0;
        int m = 0;

        arrays[k][0] = index;
        while (preVertex[index] != -1)
        {
            trace.push_back(preVertex[index]);
            //arrays[k][++i]=preVertex[index];
            i++;
            m++;
            array[i] = preVertex[index];
            arrays[k][m] = preVertex[index];
            //cout<<endl<<"观察数组" <<arrays[k][m]<<"数组别"<<m;
            index = preVertex[index];
        }

        //判断流量是否满足，若满足跳出循环
        //数组排序寻找最小的带宽
        decreaseandprintf(array, trace, distance);
    }
    //遍历结束后输出总费用
    return printvalues();
}*/


void deploy_server(char *topo[MAX_EDGE_NUM], int line_num, char *filename)
{
    //执行定时器函数
    signal(SIGALRM, timer);
    alarm(80); //定时80s

    stringstream read(topo[0]);
    //unsigned long links, consumer_nodes, network_nodes;//链路数，消费节点数，网络节点数
    read >> network_nodes >> links >> consumer_nodes;
    maxServer = consumer_nodes;
    SeverCost = atoi(topo[2]);

    for (unsigned long i = 4; i < 4 + links; ++i)
    {
        double start_node, end_node;
        int total_bandwidth, network_hire;
        read.str("");
        read << topo[i];
        read >> start_node >> end_node >> total_bandwidth >> network_hire;
        Nets[start_node][end_node].total_bandwidth = total_bandwidth;
        Nets[start_node][end_node].remaining_bandwidth = total_bandwidth;
        Nets[start_node][end_node].network_hire = network_hire;
    }
    TotalNeed = 0;//消费节点总需求
    for (unsigned long j = 5 + links; j < 5 + links + consumer_nodes; ++j)
    {
        int num, node_NO, need_bandwidth;
        read.str("");
        read << topo[j];
        read >> num >> node_NO >> need_bandwidth;
        Consumers[num].need_bandwidth = need_bandwidth;
        Consumers[num].node_NO = node_NO;
        TotalNeed += need_bandwidth;
    }

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


    //服务器初始编号，即直连方案编号，外加一个参考可能提供带宽量,按带宽从小到大排序
    set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> SeverNo;
    for (int l = 0; l < Consumers.size(); ++l)
    {
        SeverNoAndAroundBandwidth pair;
        pair.ServerNo = l;
        pair.ServeAroundBandwidth = ServeAroundBandwidth[l];
        SeverNo.insert(pair);
    }

    maxpoint = (int) (network_nodes + 2);



//定义cost邻接矩阵
//    vector<vector<int >> mapscost(network_nodes + 2,vector<int >(network_nodes + 2));
//    vector<vector<int >> mapswidth(network_nodes + 2,vector<int >(network_nodes + 2));
//    int mapscost[network_nodes + 2][network_nodes + 2];
//    int mapswidth[network_nodes + 2][network_nodes + 2];


//TODO:用来存放路径费用，行数应该是消费点个数。我直接设成最大的了。你统计消费点个数后可以改过来。
//    int values[consumer_nodes][1]={0};


    //模拟退火
   /* double t = T;
    srand((unsigned int) time(NULL));
    auto curSever = SeverNo;
    auto newSever = SeverNo;
    auto bestSever = SeverNo;

    int P_L = 0;
    int P_F = 0;
    while (1)       //外循环，主要更新参数t，模拟退火过程
    {
        for (int i = 0; i < ILOOP; i++)    //内循环，寻找在一定温度下的最优值
        {
            newSever = getNewServe(curSever);
            double dE = InitMap(newSever) - InitMap(curSever);
            if (dE < 0)   //如果找到更优值，直接更新
            {
                curSever = newSever;
                P_L = 0;
                P_F = 0;
            } else
            {
                double rd = rand() / (RAND_MAX + 1.0);
                if (exp(dE / t) > rd && exp(dE / t) < 1)   //如果找到比当前更差的解，以一定概率接受该解，并且这个概率会越来越小
                    curSever = newSever;
                P_L++;
            }
            if (P_L > LIMIT)
            {
                P_F++;
                break;
            }
        }
        if (InitMap(curSever) < InitMap(newSever))
            bestSever = curSever;
        if (P_F > OLOOP || t < EPS)
            break;
        t *= DELTA;
    }*/


    read.str("");
    stringstream &results = read;
    results << consumer_nodes << "\n";
    for (unsigned long k = 0; k < consumer_nodes; ++k)
    {
        results << "\n" << Consumers[k].node_NO << " " << k << " " << Consumers[k].need_bandwidth;
    }
    const string &strtemp = results.str();
    char *topo_file = (char *) strtemp.c_str();


//    char* topo_file=(char *)results.str().c_str();//这是错误的
//streamstring在调用str()时，会返回临时的string对象。而因为是临时的对象，所以它在整个表达式结束后将会被析构。
    //   如果需要进一步操作string对象，先把其值赋给一个string变量后再操作。
    //alarm定时器，捕捉SIGALRM信号
    //捕捉函数用 signal(SIGALRM, funcPtr);
    // 需要输出的内容
//    char *topo_file = (char *) "17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";

    // 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
    cout << topo_file;
    write_result(topo_file, filename);

}

/*int InitMap(const set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> &SeverNo)
{
    //消费节点连接的网络节点编号
    vector<int> ConsumerNum;
    for (unsigned long k = 0; k < consumer_nodes; ++k)
    {
        ConsumerNum.push_back(Consumers[k].node_NO);
    }

    //TODO:直连的服务器的编号，需要修改
    vector<int> SeverNum;
    for (auto it = SeverNo.begin(); it != SeverNo.end(); ++it)
    {
        SeverNum.push_back((*it).ServerNo);
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
                if (isInVector(SeverNum, i))
                {
                    //j也是服务器点
                    if (isInVector(SeverNum, j))
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
                        if (isInVector(ConsumerNum, j))
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
                else if (isInVector(ConsumerNum, i))
                {
                    //j是服务器点
                    if (isInVector(SeverNum, j))
                    {
                        //i是服务节点
                        if (isInVector(SeverNum, i))
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
                        if (isInVector(SeverNum, i))
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
                    if (isInVector(SeverNum, j))
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
                    if (isInVector(ConsumerNum, j))
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
    int totalCost = getServerCost(SeverNo) + getTotalLinksCosts();
    return totalCost;
}*/


/*void Dijkstra(const int numOfVertex, const int startVertex, vector<vector<int >> map, int *distance, int *prevVertex)
{
    vector<bool> isInS;                 //是否已经在S集合中
    isInS.reserve(0);
    isInS.assign((unsigned long) numOfVertex, false);   //初始化，所有的节点都不在S集合中


    //每次开始执行后设置为不能执行
    //stop=true;

    *//*初始化distance和prevVertex数组*//*
    for (int i = 0; i < numOfVertex; ++i)
    {
        distance[i] = map[startVertex][i];
        if (map[startVertex][i] < INT_MAX)
            prevVertex[i] = startVertex;
        else
            prevVertex[i] = -1;       //表示还不知道前一个节点是什么
    }
    prevVertex[startVertex] = -1;

    *//*开始使用贪心思想循环处理不在S集合中的每一个节点*//*
    isInS[startVertex] = true;          //开始节点放入S集合中


    int u = startVertex;

    for (int i = 1; i < numOfVertex; i++)      //这里循环从1开始是因为开始节点已经存放在S中了，还有numOfVertex-1个节点要处理
    {

        *//*选择distance最小的一个节点*//*
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


        *//*更新distance*//*

        //如果

        for (int j = 0; j < numOfVertex; j++)
        {
            //


            if (isInS[j] == false && mapscost[u][j] < INT_MAX)
            {
                int temp = distance[u] + mapscost[u][j];
                if (temp < distance[j])
                {
                    distance[j] = temp;
                    prevVertex[j] = u;
                }
            }
        }
    }
}

//判断消费节点与主汇点的带宽是否都变为0了，如果是，说明流量已经满足，可以终止最短路径的寻找了。否则继续寻找
bool isEnough()
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

//输出每个消费节点在流量满足的情况下，需要的路径费用
int printvalues()
{
    int t = 0;
    for (int i = 0; i < maxpoint; i++)
    {
        t += values[i];
        if (values[i] != 0)
            cout << "消费节点" << i << ":" << "累计路径费用是：" << values[i] << endl;
    }
    return t;
}


//每次找到一条最短路径，找到该路径最短带宽尽最大可能输出，输出后减少带宽，依次输出线路节点和线路的路径费用。
void decreaseandprintf(int array[], vector<int> trace, int distance[])
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
    int check=0;
    for (auto it=trace.begin();it!=trace.end();it++)
    {
        if(*it==maxpoint-2){
            check=1;
            break;
        }
    }
    //判断是否还能到达终点
    if (check==0)
    {
        stop = true;
        if (!isEnough())
        {
            cout << "需求未满足" << endl;
            //显示多少节点多少流量未满足,即判断与汇点的带宽
            for (int i = 0; i < maxpoint; i++)
            {
                if (values[i] != 0)
                    cout << "消费节点" << i << ":" << "累计路径费用是：" << values[i] << endl;
            }

        }

        //TODO:选择优化方案


    } else
    {
        if (!isEnough())
        {
            cout << "最小" << minwidth << " -d- ";
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

            //保存费用流方向,array存放的是逆向流
            int dir = array[1];
            //cout<<"dir:"<<dir<<endl;

            //if(stop==false){
            cout << "路径：";
            while (!trace.empty())
            {
                cout << *trace.rbegin() << " -- ";
//                trace.resize(trace.size()-1);
                trace.erase(--trace.end());
            }

            cout << " 距离是：" << distance[maxpoint - 1] << endl;

            //if(min!=INT_MAX){
            cout << "单条路径费用是：" << distance[maxpoint - 1] * minwidth << endl;
            values[dir] = values[dir] + distance[maxpoint - 1] * minwidth;
            //cout<< dir<<"累计路径费用是："<<values[dir]<<endl;
            //}
            //}
            isEnough();
        } else
            //输出各节点损耗
            printvalues();
    }
}*/


