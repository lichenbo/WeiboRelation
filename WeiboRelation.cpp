// WeiboRelation.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;


class DirectedGraph {
private:
	// 用一个map结构保存图（邻接表表示），第一个string为关注着，第二个set<string>为被关注者的集合。
	map<string, set<string> > graph_container;

	map<pair<string,string>,int> shortestPathBelow12;

	bool hasResult(string from, string to)
	{
		return shortestPathBelow12.count(pair<string, string>(from, to)) > 0;
	}
	int getResult(string from, string to)
	{
		return shortestPathBelow12[pair<string, string>(from, to)];
	}
	void setResult(string from, string to, int result)
	{
		shortestPathBelow12[pair<string, string>(from, to)] = result;
	}
	
	// 判断某个关注者是否存在map中
	bool hasKey(string id)
	{
		return graph_container.count(id) > 0;
	}

	// 在图中添加顶点。向map中添加一个人，初始化被关注者为空集
	void addVertex(string id) 
	{
		// Key does not exist
		if (!hasKey(id))
			graph_container[id] = set<string>();
	}

	// 在图中添加边。在双方都已经在图中的前提下，将第一个人（from）的被关注者集合中添加第二个人（to）
	void addEdge(string from, string to) 
	{
		if (hasKey(from) && hasKey(to)) 
		{
			graph_container[from].insert(to);
		} 
		else 
		{
			cout << "key or value does not exist while adding edge" << endl;
			exit(-1);
		}
	}

public:

	// 得到图中的所有顶点（就是map中的所有key）的vertex（向量）。
	vector<string> getKeys()
	{
		vector<string> keys;
		for (map<string, set<string> >::iterator itr = graph_container.begin(); itr != graph_container.end(); itr++)
		{
			keys.push_back(itr->first);
		}
		return keys;
	}

	// 添加两个人之间的关系，其实就是先把两个人添加到图里，然后添加一条边
	void makeRelation(string from, string to)
	{
		addVertex(from);
		addVertex(to);
		addEdge(from,to);
	}

	// 计算出小于12的最短路径，具体算法采用Floyd。
	int getShortestPathBelow12RecHelper(string from, string to)
	{
		return getShortestPathBelow12Rec(from, to, 0);
	}
	int getShortestPathBelow12Rec(string from, string to, int tag)
	{
		vector<int> result;
		if (hasResult(from, to))
		{
			return getResult(from, to);
		}
		if (tag == 12)
		{
			return 100;
		}
		if (from == to)
		{
			return tag;
		}
		if (hasKey(from) && hasKey(to))
		{
			set<string> next = graph_container[from];
			for (set<string>::iterator setit = next.begin(); setit != next.end(); setit++)
			{
				int tempResult = getShortestPathBelow12Rec(*setit, to, tag + 1);
				result.push_back(tempResult);
			}
			int min_result = *std::min_element(result.begin(), result.end());
			setResult(from, to, min_result);
			return min_result;
		}
		exit(-1);
	}
	int getShortestPathBelow12(string from, string to)
	{
		if (hasResult(from, to))
		{
			return getResult(from, to); 
		}
		if (hasKey(from) && hasKey(to))
		{
			// step用来保存标号信息，也就是最短路径信息
			map<string,int> step;
			step[from] = 0;
			// 用i来进行Floyd算法中的标号
			for (int i = 0; i < 12 ;i++)	// i最多循环12次，如果超过12或者两者之间没有关系，就返回0
			{
				// 用iterator来遍历map
				for (map<string,int>::iterator it = step.begin(); it!= step.end(); it++)
				{
					// 寻找开始标号的点
					if (it->second == i)
					{
						// 下一步要标号的点存到next_step这个map中去
						set<string> next_step = graph_container[it->first];
						for (set<string>::iterator setit = next_step.begin(); setit!=next_step.end(); setit++)
						{
							// 依次进行标号，标号为i+1
							if ((step.count(*setit) < 1) || step[*setit] > i+1)
							{
								if (*setit == to)
									return i+1;
								step[*setit] = i+1;
								setResult(from,*setit,i+1);
							}
						}
					}
				}
			}
			// 大于12次的循环标号为0
			return 0;
		}
		else 
		{
			cout << "key or value does not exist while finding path" << endl;
			exit(-1);
		}
	}
}; 
int main()
{
	set<int> a;
	ifstream weibo_data("weibo.txt");
	DirectedGraph* g = new DirectedGraph();
	if (weibo_data.is_open())
	{
		string line;
		string from, to;
		int count = 0;
		// 读数据
		while (!weibo_data.eof())
		{
			weibo_data >> from >> to;
			g->makeRelation(from, to);
			cout << "processed " << count << "\t" << from << "\t" << to << endl;
			count++;
		}

		vector<string> keys = g->getKeys();
		count = 0;
		// 对每两个人都进行查找
		for (vector<string>::iterator vitr1 = keys.begin(); vitr1 != keys.end(); vitr1++)
		{
			for (vector<string>::iterator vitr2 = keys.begin(); vitr2 != keys.end(); vitr2++)
			{
				int shortestPath = g->getShortestPathBelow12(*vitr1, *vitr2);
				cout << "shortest path from " << *vitr1 << " to " << *vitr2 << " is " << g->getShortestPathBelow12(*vitr1, *vitr2) << endl;
				if (shortestPath > 0)
				{
					count++; // 小于12的数进行计数
				}
			}
		}
		cout << "Total " << count << " relations connected below 12" << endl;
		cout << "Total " << keys.size() << " people in the network" << endl;
	}
	else 
	{
		cout << "Unable to open file" << endl;
	}
	delete g;
	system("pause");
	return 0;
}

