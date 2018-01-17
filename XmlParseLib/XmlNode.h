#pragma once
#include<string>
#include<vector>
#include<map>
using namespace std;

//작성자 엄태욱 2018-01-10
//형태는 Data와 같으나 햇갈릴 위험이 있음으로
class XmlArribute
{
public:
	wstring prefix;
	wstring attributeKey;
	wstring attributeValue;
};
class XmlData
{
public:
	unsigned int depth;
	wstring prefix;
	wstring element;
	wstring elementText;
	std::map <wstring, XmlArribute*> attribute;
public:
	XmlData() {};
	~XmlData()
	{
		for (auto it = attribute.begin(); it != attribute.end(); ++it)
		{
			delete it->second;
		}
		attribute.clear();
	};
};

class CXmlNode
{
private:
	XmlData data;
	vector<CXmlNode*> child;
	CXmlNode* parent;
	CXmlNode* next;
public :
	CXmlNode() 
	{
		parent = nullptr;
		next = nullptr;
	}
	~CXmlNode();
	void Add(CXmlNode* node);
	vector<CXmlNode*>& Node();
	CXmlNode* Next();
	bool HasChildNodes();
	XmlData& GetData();
};
inline vector<CXmlNode*>& CXmlNode::Node()
{
	return child;
}
//형제 노드가 없으면 nullPtr return
inline CXmlNode* CXmlNode::Next()
{
	return next;
}
//자식노드 추가
inline void CXmlNode::Add(CXmlNode* node)
{
	node->parent = this;
	if (child.size() != 0)
	{
		child.back()->next = node;
	}
	child.push_back(node);
}
inline XmlData& CXmlNode::GetData()
{
	return data;
}
inline CXmlNode::~CXmlNode()
{
	for (int i = 0; i < child.size(); i++)
	{
		delete child[i];
	}
	child.clear();
}
inline bool CXmlNode::HasChildNodes()
{
	return child.size() != 0;
}