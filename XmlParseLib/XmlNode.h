#pragma once
#include<string>
#include<vector>
#include<map>
using namespace std;

//�ۼ��� ���¿� 2018-01-10
//���´� Data�� ������ �ް��� ������ ��������
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
	XmlData _data;
	vector<CXmlNode*> _child;
	CXmlNode* _parent;
	CXmlNode* _next;
public :
	CXmlNode() 
	{
		_parent = nullptr;
		_next = nullptr;
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
	return _child;
}
//���� ��尡 ������ nullPtr return
inline CXmlNode* CXmlNode::Next()
{
	return _next;
}
//�ڽĳ�� �߰�
inline void CXmlNode::Add(CXmlNode* node)
{
	node->_parent = this;
	if (_child.size() != 0)
	{
		_child.back()->_next = node;
	}
	_child.push_back(node);
}
inline XmlData& CXmlNode::GetData()
{
	return _data;
}
inline CXmlNode::~CXmlNode()
{
	for (int i = 0; i < _child.size(); i++)
	{
		delete _child[i];
		_child[i] = NULL;
	}
	_child.clear();
}
inline bool CXmlNode::HasChildNodes()
{
	return _child.size() != 0;
}