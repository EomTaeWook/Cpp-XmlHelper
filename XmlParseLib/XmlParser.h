#pragma once
#include "XmlNode.h"
#include <xmllite.h>
#include <atlbase.h>
#include <string>
#include"json/json.h"
#include "NS.h"
#pragma comment(lib,"xmllite.lib")
//JSONCPP LIB
#pragma comment( lib , "json_vc71_libmtd.lib")
NS_UTIL_BEGIN
	class XmlParser
	{
	public:
		XmlParser(void);
		virtual ~XmlParser(void);
		bool FindAttributesValue(wstring elementName, wstring attributesKey, wstring value);
		void StreamWrite(const void *pv, ULONG cb, ULONG *pcbWritten);
		//GetNode
		
		XmlNode* GetXmlNode();
		void Clear();
		char* JsonToXml(Json::Value& node);
	private:
		IXmlReader* _pReader;
		IStream* _pStream;
		void ZeroPosition();
		bool GetXmlAttributes(IXmlReader* pReader, wstring attributesKey, wstring value);
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
		::CreateXmlReader(__uuidof(IXmlReader), reinterpret_cast<void**>(&_pStream), 0);
		_pReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit);
		_pReader->SetInput(_pStream);
		LARGE_INTEGER pos;
		pos.QuadPart = 0;
		if (_pStream) _pStream->Seek(pos, STREAM_SEEK_SET, NULL);
	}
NS_UTIL_END