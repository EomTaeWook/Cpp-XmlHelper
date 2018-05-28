#include "XmlParser.h"
//JSONCPP LIB ERROR
#pragma warning(disable:4996)
NS_XML_BEGIN
XmlParser::XmlParser(void)
{
	::CreateStreamOnHGlobal(0, TRUE, &_pStream);
	_pReader = NULL;
}

XmlParser::~XmlParser()
{
	if (_pStream)_pStream->Release();
	if (_pReader)_pReader->Release();
}
void XmlParser::StreamWrite(const void *pv, ULONG cb,ULONG *pcbWritten)
{
	_pStream->Write(pv, cb, pcbWritten);
}
bool XmlParser::GetXmlAttributes(IXmlReader* pReader, std::wstring attributesKey, std::wstring pvalue)
{
	HRESULT hr = pReader->MoveToFirstAttribute();
	std::wstring m_attributesKey=L"";
	unsigned int cwchPrefix;
	if (S_OK != hr)
		return false;
	while(TRUE)
	{
		if (!pReader->IsDefault())
		{
			LPCWSTR prefix;
			LPCWSTR localName;
			LPCWSTR value;
			if (pReader->GetPrefix(&prefix, &cwchPrefix) != S_OK) return false;
			if (pReader->GetLocalName(&localName, NULL) != S_OK) return false;
			if(pReader->GetValue(&value, NULL) != S_OK) return false;
			if (cwchPrefix > 0)
			{
				m_attributesKey.append(prefix);
				m_attributesKey.append(L":");
				m_attributesKey.append(localName);
			}
			else
			{
				m_attributesKey.append(localName);
			}
                 
			if(m_attributesKey == attributesKey)
			{
				if(value == pvalue) return true;
				else return false;
			}
		}
		if (S_OK != pReader->MoveToNextAttribute())
			break;
	}
	return false;
}
bool XmlParser::FindAttributesValue(std::wstring elementName, std::wstring attributesKey, std::wstring value)
{
	ZeroPosition();

	XmlNodeType nodeType;
	std::wstring m_elementName=L"";
	while (S_OK == (_pReader->Read(&nodeType)))
	{
		switch (nodeType)
		{
		case XmlNodeType_Element:
			{
				if (_pReader->IsEmptyElement() )
				{
					break;
				}
				LPCWSTR prefix;
				LPCWSTR localName;
				UINT prefix_size;
				if (_pReader->GetPrefix(&prefix, &prefix_size)  != S_OK) return false;
				if(_pReader->GetLocalName(&localName, NULL) != S_OK)return false;
				if(prefix_size>0) 
				{
					m_elementName.append(prefix);
					m_elementName.append(L":");
					m_elementName.append(localName);
				}
				else 
				{
					m_elementName.append(localName);
				}
				if(m_elementName == elementName)
				{
					if(GetXmlAttributes(_pReader, attributesKey, value))
						return true;
				}
			}
			break;
					
		case XmlNodeType_EndElement:
			{
				LPCWSTR prefix;
				LPCWSTR localName;
				UINT prefix_size;
				if(_pReader->GetPrefix(&prefix, &prefix_size) != S_OK) return false;
				if(_pReader->GetLocalName(&localName, NULL) != S_OK) return false;
			}
			break;
		case XmlNodeType_Text:
		case XmlNodeType_CDATA:
		case XmlNodeType_Whitespace:
		case XmlNodeType_DocumentType:
		case XmlNodeType_Comment:
		case XmlNodeType_ProcessingInstruction:
			break;
		}
	}
	return false;
}
bool XmlParser::GetXmlAttributes(IXmlReader* pReader, XmlNode* node)
{
	HRESULT hr = pReader->MoveToFirstAttribute();
	if (S_OK != hr)
		return true;
	std::wstring mapKey;
	while (TRUE)
	{
		if (!pReader->IsDefault())
		{
			LPCWSTR prefix;
			LPCWSTR localName;
			LPCWSTR value;
			UINT prefix_size;
			if (pReader->GetPrefix(&prefix, &prefix_size) != S_OK) return false;
			if (pReader->GetLocalName(&localName, NULL) != S_OK) return false;
			if (pReader->GetValue(&value, NULL) != S_OK) return false;
			XmlArribute* attribute = new XmlArribute() ;
			attribute->prefix = prefix;
			attribute->attributeKey = localName;
			attribute->attributeValue = value;
			if(prefix_size == 0)
			{
				node->GetData().attribute.insert(std::make_pair(localName, attribute));
			}
			else
			{
				mapKey = prefix;
				mapKey.append(L":");
				mapKey.append(localName);
				node->GetData().attribute.insert(std::make_pair(mapKey, attribute));
				mapKey.clear();
			}
		}
		if (S_OK != pReader->MoveToNextAttribute())
			break;
	}
	return true;
}
void XmlParser::Clear()
{
	if(_pStream)
	{
		_pStream->Release();
	}
	::CreateStreamOnHGlobal(0, TRUE, &_pStream);
	ZeroPosition();
}
XmlNode* XmlParser::GetXmlNode()
{
	ZeroPosition();

	XmlNodeType nodeType;
			
	std::vector<XmlNode*> nodeStack;
	XmlNode* root = new XmlNode();
	nodeStack.push_back(root);
	try
	{
		while (S_OK == (_pReader->Read(&nodeType)))
		{
			switch (nodeType)
			{
			case XmlNodeType_Element:
			{
				if (_pReader->IsEmptyElement() )
				{
					break;
				}
				LPCWSTR prefix;
				LPCWSTR localName;
				if (_pReader->GetPrefix(&prefix, NULL) != S_OK) throw std::exception("Xml Element Prefix Parse Error");
				if (_pReader->GetLocalName(&localName, NULL) != S_OK) throw std::exception("Xml Element LocalName Parse Error");
				XmlNode* node = new XmlNode();
				_pReader->GetDepth(&node->GetData().depth);
				node->GetData().prefix = prefix;
				node->GetData().element = localName;
				nodeStack.push_back(node);
				if (!GetXmlAttributes(_pReader, node)) throw std::exception("Xml Attribute Parse Error");
				break;
			}
			case XmlNodeType_Text:
				{
					LPCWSTR value;
					if (_pReader->GetValue(&value, NULL) != S_OK) throw std::exception("Xml Text Parse Error");
					nodeStack.back()->GetData().elementText = value;
					break;
				}
			case XmlNodeType_EndElement:
			{
				LPCWSTR prefix;
				LPCWSTR localName;
				if (_pReader->GetPrefix(&prefix, NULL) != S_OK) throw std::exception("Xml EndElement Prefix Parse Error");
				if (_pReader->GetLocalName(&localName, NULL) != S_OK) throw std::exception("Xml EndElement LocalName Parse Error");
				XmlNode* node = nodeStack.back();
				nodeStack.pop_back();
				nodeStack.back()->Add(node);
				break;
			}
			case XmlNodeType_XmlDeclaration:
			case XmlNodeType_CDATA:
			case XmlNodeType_Whitespace:
			case XmlNodeType_DocumentType:
			case XmlNodeType_Comment:
			case XmlNodeType_ProcessingInstruction:
				break;
			}
		}
	}
	catch(std::exception ex)
	{
		while(!nodeStack.empty())
		{
			delete nodeStack.back();
			nodeStack.pop_back();
		}
		return nullptr;
	}
	return root;
}

void XmlParser::JsonToXml(Json::Value& node, IXmlWriter *pWriter)
{
	std::wstring key;
	std::wstring value;
	std::string tmp;
	if (node.isArray())
	{
		for (int i = 0; i < node.size(); i++)
		{
			JsonToXml(node[i], pWriter);
		}
	}
	else
	{
		for (auto it = node.begin(); it != node.end(); ++it)
		{
			tmp = it.key().asString();
			key.assign(tmp.begin(), tmp.end());
			if (tmp.front() == '@')
			{
				tmp = (*it).asString();
				value.assign(tmp.begin(), tmp.end());
				pWriter->WriteAttributeString(L"", key.substr(1, key.length() - 1).c_str(), L"", value.c_str());
			}
			else
			{
				pWriter->WriteStartElement(NULL, key.c_str(), NULL);
				if (it->size() > 0)
				{
					JsonToXml(*it, pWriter);
				}
				else
				{
					tmp = (*it).asString();
					value.assign(tmp.begin(), tmp.end());
					pWriter->WriteString(value.c_str());
				}
				pWriter->WriteEndElement();
			}
		}
	}
}

char* XmlParser::JsonToXml(Json::Value& node)
{
	IStream* pStream;
	::CreateStreamOnHGlobal(0, TRUE, &pStream);
	IXmlWriter *pWriter = NULL;
	CreateXmlWriter(__uuidof(IXmlWriter), reinterpret_cast<void**>(&pWriter), NULL);
	pWriter->SetProperty(XmlWriterProperty_Indent, TRUE);
	pWriter->SetOutput(pStream);
	pWriter->WriteStartDocument(XmlStandalone_Yes);
	JsonToXml(node, pWriter);
	pWriter->WriteEndDocument();
	pWriter->Flush();
	LARGE_INTEGER l;
	l.QuadPart = 0;
	pStream->Seek(l, 0, NULL);
	HGLOBAL hg;
	GetHGlobalFromStream(pStream, &hg);
	int buffsize = GlobalSize(hg);
	char *pBuff = new char[buffsize];
	ULONG ulBytesRead;
	pStream->Read(pBuff, buffsize, &ulBytesRead);
	pWriter->Release();
	pStream->Release();
	pWriter = NULL;
	pStream = NULL;
	return pBuff;
}
NS_XML_END