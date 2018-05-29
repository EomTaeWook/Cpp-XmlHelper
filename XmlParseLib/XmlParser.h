#pragma once
#include "NS.h"
#include "XmlNode.h"
#include <atlbase.h>
#include <string>
#include <xmllite.h>
#include "json/json.h"
#if _WIN64
#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v7.0A\\Lib\\x64\\xmllite.lib")
#else
#pragma comment(lib,"xmllite.lib")
#endif
//JSONCPP LIB
#pragma comment( lib , "json_vc71_libmtd.lib")
NS_XML_BEGIN
class XmlParser
{
public:
	XmlParser()
	{
		::CreateStreamOnHGlobal(0, TRUE, &_pStream);
		_pReader = NULL;
	}
	virtual ~XmlParser()
	{
		if (_pStream)_pStream->Release();
		if (_pReader)_pReader->Release();
	}
	bool FindAttributesValue(std::wstring elementName, std::wstring attributesKey, std::wstring value);
	void StreamWrite(const void *pv, ULONG cb, ULONG *pcbWritten);
	XmlNode* GetXmlNode();
	void Clear();
	char* JsonToXml(Json::Value& node);
private:
	IXmlReader* _pReader;
	IStream* _pStream;
	void ZeroPosition();
	bool GetXmlAttributes(IXmlReader* pReader, std::wstring attributesKey, std::wstring value);
	bool GetXmlAttributes(IXmlReader* pReader, XmlNode* node);
	void JsonToXml(Json::Value& node, IXmlWriter *pWriter);
};

inline void XmlParser::ZeroPosition()
{
	if (_pReader)
	{
		_pReader->Release();
		_pReader = NULL;
	}
	CreateXmlReader(__uuidof(IXmlReader), reinterpret_cast<void**>(&_pReader), 0);
	_pReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit);
	_pReader->SetInput(_pStream);
	LARGE_INTEGER pos;
	pos.QuadPart = 0;
	if (_pStream) _pStream->Seek(pos, STREAM_SEEK_SET, NULL);
}
NS_XML_END