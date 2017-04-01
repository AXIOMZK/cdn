#ifndef __ROUTE_H__
#define __ROUTE_H__

#include <cmath>
#include "lib_io.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <unistd.h>
#include <climits>
#include <signal.h>
using namespace std;

//服务器部署编号(连接的节点号)，评估带宽结构体
struct SeverNoAndAroundBandwidth;
//链路信息(双向)
struct LinkInfo;
//消费节点信息结构体
struct ResumeInfo;
struct Bandwidth_From_Small_To_Big;

void deploy_server(char *graph[MAX_EDGE_NUM], int edge_num, char *filename);

void timer(int sig);

#endif

