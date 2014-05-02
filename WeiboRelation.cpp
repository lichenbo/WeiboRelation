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
	// ��һ��map�ṹ����ͼ���ڽӱ��ʾ������һ��stringΪ��ע�ţ��ڶ���set<string>Ϊ����ע�ߵļ��ϡ�
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
	
	// �ж�ĳ����ע���Ƿ����map��
	bool hasKey(string id)
	{
		return graph_container.count(id) > 0;
	}

	// ��ͼ����Ӷ��㡣��map�����һ���ˣ���ʼ������ע��Ϊ�ռ�
	void addVertex(string id) 
	{
		// Key does not exist
		if (!hasKey(id))
			graph_container[id] = set<string>();
	}

	// ��ͼ����ӱߡ���˫�����Ѿ���ͼ�е�ǰ���£�����һ���ˣ�from���ı���ע�߼�������ӵڶ����ˣ�to��
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

	// �õ�ͼ�е����ж��㣨����map�е�����key����vertex����������
	vector<string> getKeys()
	{
		vector<string> keys;
		for (map<string, set<string> >::iterator itr = graph_container.begin(); itr != graph_container.end(); itr++)
		{
			keys.push_back(itr->first);
		}
		return keys;
	}

	// ���������֮��Ĺ�ϵ����ʵ�����Ȱ���������ӵ�ͼ�Ȼ�����һ����
	void makeRelation(string from, string to)
	{
		addVertex(from);
		addVertex(to);
		addEdge(from,to);
	}

	// �����С��12�����·���������㷨����Floyd��
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
			// step������������Ϣ��Ҳ�������·����Ϣ
			map<string,int> step;
			step[from] = 0;
			// ��i������Floyd�㷨�еı��
			for (int i = 0; i < 12 ;i++)	// i���ѭ��12�Σ��������12��������֮��û�й�ϵ���ͷ���0
			{
				// ��iterator������map
				for (map<string,int>::iterator it = step.begin(); it!= step.end(); it++)
				{
					// Ѱ�ҿ�ʼ��ŵĵ�
					if (it->second == i)
					{
						// ��һ��Ҫ��ŵĵ�浽next_step���map��ȥ
						set<string> next_step = graph_container[it->first];
						for (set<string>::iterator setit = next_step.begin(); setit!=next_step.end(); setit++)
						{
							// ���ν��б�ţ����Ϊi+1
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
			// ����12�ε�ѭ�����Ϊ0
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
		// ������
		while (!weibo_data.eof())
		{
			weibo_data >> from >> to;
			g->makeRelation(from, to);
			cout << "processed " << count << "\t" << from << "\t" << to << endl;
			count++;
		}

		vector<string> keys = g->getKeys();
		count = 0;
		// ��ÿ�����˶����в���
		for (vector<string>::iterator vitr1 = keys.begin(); vitr1 != keys.end(); vitr1++)
		{
			for (vector<string>::iterator vitr2 = keys.begin(); vitr2 != keys.end(); vitr2++)
			{
				int shortestPath = g->getShortestPathBelow12(*vitr1, *vitr2);
				cout << "shortest path from " << *vitr1 << " to " << *vitr2 << " is " << g->getShortestPathBelow12(*vitr1, *vitr2) << endl;
				if (shortestPath > 0)
				{
					count++; // С��12�������м���
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

