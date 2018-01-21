#pragma once
#include "XmlNode.h"
#include <xmllite.h>
#include <atlbase.h>
#include <string>
#include"json/json.h"
#pragma comment(lib,"xmllite.lib")
//JSONCPP LIB
#pragma comment( lib , "json_vc71_libmtd.lib")
class CXmlParser
{
public:
	CXmlParser(void);
	virtual ~CXmlParser(void);
	bool FindAttributesValue(wstring elementName, wstring attributesKey, wstring value);
	void StreamWrite(const void *pv, ULONG cb, ULONG *pcbWritten);
	//GetNode
	//error½Ã nullptr ¹ÝÈ¯
	CXmlNode* GetXmlNode();
	void Clear();
	char* JsonToXml(Json::Value& node);
private:
	IXmlReader* pReader;
	IStream* pStream;
	void ZeroPosition();
	bool GetXmlAttributes(IXmlReader* pReader, wstring attributesKey, wstring value);
	bool GetXmlAttributes(IXmlReader* pReader, CXmlNode* node);
	void JsonToXml(Json::Value& node, IXmlWriter *pWriter);
};

inline void CXmlParser::ZeroPosition()
{
	if (pReader)
	{
		pReader->Release();
		pReader = NULL;
	}
	::CreateXmlReader(__uuidof(IXmlReader), reinterpret_cast<void**>(&pReader), 0);
	pReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit);
	pReader->SetInput(pStream);
	LARGE_INTEGER pos;
	pos.QuadPart = 0;
	if (pStream) pStream->Seek(pos, STREAM_SEEK_SET, NULL);
}

