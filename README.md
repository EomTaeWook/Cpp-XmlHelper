# Cpp-XmlHelper

XML SAX Parser

Convert Json TO XML

XmlParser xmlParser;

xmlParser.StreamWrite(string, string.size(), &returnCallbackSize);

unique_ptr<Helper::Xml::XmlNode>pRootNode(xmlParser.GetXmlNode()); 



Convert Json To Xml

Json은 Jsoncpp를 사용하며 Xml은 Xmllite 사용

Json::Value node
unique_ptr<char[]>p_Xml(xmlParser.JsonToXml(node));
