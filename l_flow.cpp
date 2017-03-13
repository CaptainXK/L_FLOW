#include<iostream>
#include<fstream>
#include<vector>
#include<string.h>
#include<algorithm>
using namespace std;

bool DFS(int **map, vector<int> &path, int num_p, int cur, int dst){//find a path from src to dst
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

void get_resi(int **c,int **f,int **rg, int num){
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

int main(){
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
//	status(c,nodes,"c");
//	status(f,nodes,"f");
//	status(rg,nodes,"rg");
	int min_c;
	while(DFS(rg, cur_path, nodes, 0 ,nodes-1)){
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
//		status(c,nodes,"c");
//		status(f,nodes,"f");
//		status(rg,nodes,"rg");
		while(cur_path.size()>0)
			cur_path.pop_back();
	}
	int sum=0;
	for(int i=0; i<nodes; i++){
		if(f[0][i]>0)
			sum += f[0][i];
	}
	cout<<"max flow : "<<sum<<endl;
	return 0;	
}