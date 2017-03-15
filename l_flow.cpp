#include<iostream>
#include<fstream>
#include<vector>
#include<string.h>
#include<algorithm>
#include<queue>
#include<time.h>
#include<stdio.h>
using namespace std;

bool BELLMAN_FORD(int **map, vector<int> &path, int num_p, int cur, int dst, int **cost = NULL , int **c = NULL, int **f = NULL){
	int * bf_visited = new int[num_p]();
	int * bf_map = new int[num_p]();
	int * bf_cost = new int[num_p]();
	memset(bf_map, -1, num_p*sizeof(int));
	for(int i=0; i<num_p; i++){
		if(cost[0][i]>0)
			bf_cost[i]=cost[0][i];
		else
			bf_cost[i]=9999999;
	}

	for(int i=0; i<num_p; i++){//loop for nodes' number times
		for(int j=0; j<num_p; j++){
			for(int k=0; k<num_p; k++){
				if(map[j][k]>0 && f[j][k]<c[j][k] && bf_cost[k]>bf_cost[j]+cost[j][k]){
					bf_map[k] = j;
					bf_cost[k] = bf_cost[j]+cost[j][k];
				}
			}
		}
	}

	if(bf_map[dst]=-1)//if dst node's pre node is not exsit
		return false;
	else{
		int trace = dst;
		while(trace!=-1){
			path.push_back(trace);
			trace = bf_map[trace];
		}
		reverse(path.begin(),path.end());
		return true;
	}
}

bool BFS(int **map, vector<int> &path, int num_p, int cur, int dst){//find a path from src to dst with BFS
	int * bfs_map = new int[num_p]();//traceback path, "bfs_map[i] == j" means that ith node's pre node is jth node
	int * bfs_flag = new int[num_p]();//visited node flag
	queue<int> bfs_que;
	bfs_que.push(cur);
	bfs_map[cur]=-1;
	int top_node;
	while( !bfs_que.empty() ){
		top_node = bfs_que.front();
		for(int i=0; i<num_p; i++){
			if( map[top_node][i]>0 && bfs_flag[i]==0){
				bfs_map[i]=top_node;//update current node's prefix node
				bfs_que.push(i);
			}
		}
		bfs_que.pop();
		bfs_flag[top_node]=1;
		if(top_node==dst){//find a traceback path and store it in "path"
			int trace=dst;
			while(bfs_map[trace]!=-1){//-1 is the pre node of node 0
				path.push_back(trace);
				trace = bfs_map[trace];
			}
			path.push_back(0);
			reverse(path.begin(),path.end());//reverse to make sure the path begining with node 0
			return true;
		}
	}
}

bool DFS(int **map, vector<int> &path, int num_p, int cur, int dst){//find a path from src to dst with DFS
	bool flag = false;
	if(find(path.begin(), path.end(), cur)!=path.end())//avoid loop
		return false;
	path.push_back(cur);
	if(cur!=dst){
		for(int i=0; i<num_p; i++){
			if(map[cur][i]>0){
				flag = DFS(map,path,num_p,i,dst);
				if(flag){
					return true;
				}
			}
		}
		if(flag==false){
			path.pop_back();
			return false;
		}
	}
	else{
		return true;
	}
}

void get_resi(int **c,int **f,int **rg, int num){//获取残留网络
	for(int i=0; i<num; i++){
		for(int j=0; j<num; j++){
			if( rg[i][j]>0 && c[i][j]>0){
				rg[i][j]=c[i][j]-f[i][j];
				rg[j][i]=f[i][j];
			}
		}
	}
}

void status(int **input, int num, const char* str){
	cout<<str<<":"<<endl;
	for(int i=0; i<num; i++){
		for(int j=0; j<num; j++){
			cout<<input[i][j]<<" ";
		}
		cout<<endl;
	}
}

int main(int argc , int **argv){
	ifstream fin("data.txt",ios::in);
	int nodes;
	int **f , **c, **rg;//current flow, capacity, residual graph
	vector<int> cur_path;
	if(!fin.is_open()){
		cout<<"file open error!\n";
		return 0;
	}
	fin>>nodes;//get nodes' number
	f = new int*[nodes];
	c = new int*[nodes];
	rg = new int*[nodes];
	for(int i=0; i<nodes; i++){
		f[i]=new int[nodes];
		c[i]=new int[nodes];
		rg[i]=new int[nodes];
		memset(f[i],0,nodes*sizeof(int));
		memset(c[i],0,nodes*sizeof(int));
		memset(rg[i],0,nodes*sizeof(int));
	}

	int a=0,b=0,weight=0;
	while(!fin.eof()){//inicialize capacity
		fin>>a>>b>>weight;
		c[a][b]=weight;
		rg[a][b]=weight;
	}
	get_resi(c,f,rg,nodes);
	int min_c;
	clock_t start = clock();
	// while(DFS(rg, cur_path, nodes, 0 ,nodes-1)){
	while(BFS(rg, cur_path, nodes, 0 ,nodes-1)){
	cout<<"cur path : ";
		for(int i=0; i<cur_path.size(); i++){
			cout<<cur_path[i]<<" ";
		}
		cout<<endl;

		min_c=rg[cur_path[0]][cur_path[1]];
		for(int i=0; i<cur_path.size()-1; i++){//find the min weight sub_path in the found path
			if( rg[cur_path[i]][cur_path[i+1]]<min_c ){
				min_c = rg[cur_path[i]][cur_path[i+1]];
			}
		}
		for(int i=0; i<cur_path.size()-1; i++){
			if(c[cur_path[i]][cur_path[i+1]] > 0)
				f[cur_path[i]][cur_path[i+1]] += min_c;
			else
				f[cur_path[i+1]][cur_path[i]] -= min_c;
		}
		get_resi(c,f,rg,nodes);
		while(cur_path.size()>0)
			cur_path.pop_back();
	}
	double diff = (clock()-start)/CLOCKS_PER_SEC;

	int sum=0;
	for(int i=0; i<nodes; i++){
		if(f[0][i]>0)
			sum += f[0][i];
	}
	cout<<"max flow : "<<sum<<endl;
//	cout<<diff<<endl;
	printf("time cost : %lf\n",diff);
	return 0;
}
