# 整体思路：

* 先解决CDN位置确定时的最小费用最大流问题

 > 使用MCMF(最小费用最大流)算法，加入超级源点和超级汇点的思想:超级源点连接设定好的CDN（为了方便，接下来服务器都会被简称为CDN），超级汇点连接所有的消费节点所连接的网络节点，并且保证所有消费节点连接到超级汇点的链路带宽即为其带宽需求。至此，当确定某几个节点为CDN时，可以求出最大流为且只能为固定值时的最小费用问题，所以接下来我们只需要想方法来确定CDN位置。


* 先模拟退火，然后再对优解做遗传进化
> 采用从直连解法为种子，进行模拟退火，启发式搜索，在退火过程中加大抖动力度，让搜索范围更全局，而非陷入某个局部最优。之后，将模拟退火的解作为祖先，进行交叉变异，遗传进化，快速迭代出更优秀的解。


## 数据结构设计
```C++

 //某网络节点作为服务器时的最大提供带宽
 //序号为服务器所连的节点号，值为评估带宽
    vector<int> ServeAroundBandwidth;

 //所有节点的评估带宽存储
 //插入时，级按带宽权重排序，以便后续退火出优解提供更好的候选节点
    set<SeverNoAndAroundBandwidth, Bandwidth_From_Small_To_Big> AllNodeAroundBandwidth;

 //消费节点连接的网络节点编号
    set<int> ConsumerNum;

 //网络节点编号索引相连消费节点带宽
    map<int, int> NodesLinkConsumerNeed;

 //网络节点编号索引相连消费节点序号
    map<int, int> NodesLinkConsumerNO;

struct SeverNoAndAroundBandwidth
{
    int ServerNo;
    int ServeAroundBandwidth;
};

struct server
{
    vector<int> serverNO;//一共net_node,bit来对染色体进行编码
    double bandwidths;
    int num_of_sercers;
    double fit;//适应值
    double rfit = 0;//相对的fit值，即所占的百分比
    double cfit = 0;//积累概率
    int cost = INT_MAX;

    bool operator<(const server &m) const
    {
        return fit < m.fit;
    }
};

struct SeverSetAndCost
{
    set<int> SetSeverNO;//服务器编号集合
    int cost;//对应的成本
};

struct Bandwidth_From_Small_To_Big
{
    bool operator()(const SeverNoAndAroundBandwidth &left, const SeverNoAndAroundBandwidth &right) const
    {
        return (left.ServeAroundBandwidth < right.ServeAroundBandwidth);
    }
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


```


## 模拟退火算法流程
![avatar](./模拟退火算法流程.jpeg)

## 退火策略

* 1、先对数据量（节点数量）进行分类，分为大、中、小三类
* 2、各个类型下，进行不同的退火策略，具体策略及其执行概率见下方
```C++
if (consumer_nodes > 300)
    {
//大型数据
//随机删除一个服务器,小型数据
        a1 = 35;
//随机添加一个服务器,小型数据
        a2 = 35;
//随机按比例删除服务器，中大型数据
        a3 = 100;
//随机按比例增加服务器，中大型数据
        a4 = 100;
//优先删除所能提供带宽最小的服务器
        a5 = 100;
//优先添加所能提供带宽最大的服务器
        a6 = 100;
//完全产生新服务器
        a7 = 100;
//随机添加t1个服务器,再随机删除t2个服务器
        a8 = 100;
//添加一个最大服务器,再删除最小的服务器
//a8~100

    } else if (consumer_nodes > 100)
    {
//中型数据
//随机删除一个服务器,小型数据
        a1 = 70;
//随机添加一个服务器,小型数据
        a2 = 100;
//随机按比例删除服务器，中大型数据
        a3 = 100;
//随机按比例增加服务器，中大型数据
        a4 = 100;
//优先删除所能提供带宽最小的服务器
        a5 = 100;
//优先添加所能提供带宽最大的服务器
        a6 = 100;
//完全产生新服务器
        a7 = 100;
//随机添加t1个服务器,再随机删除t2个服务器
        a8 = 100;
//添加一个最大服务器,再删除最小的服务器
//a8~100
    } else
    {
//小型数据
//随机删除一个服务器,小型数据
        a1 = 70;
//随机添加一个服务器,小型数据
        a2 = 100;
//随机按比例删除服务器，中大型数据
        a3 = 100;
//随机按比例增加服务器，中大型数据
        a4 = 100;
//优先删除所能提供带宽最小的服务器
        a5 = 100;
//优先添加所能提供带宽最大的服务器
        a6 = 100;
//完全产生新服务器
        a7 = 100;
//随机添加t1个服务器,再随机删除t2个服务器
        a8 = 100;
//添加一个最大服务器,再删除最小的服务器
//a8~100
    }
```

## 退火后的优化解进行遗传进化，快速出解





