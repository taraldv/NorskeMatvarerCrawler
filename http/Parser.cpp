#include "Parser.h"

Parser::Parser(htmlDocPtr doc):doc(doc){}

Parser::~Parser(){
	xmlFreeDoc(doc);
	//delete doc;
}

xmlNodeSetPtr Parser::getRegexNodes(xmlChar *regex){
	//htmlDocPtr doc = getXMLDocFromURL(url);

	/* https://www.w3schools.com/xml/xpath_syntax.asp */
	xmlChar *xpath = regex;

	/* http://xmlsoft.org/html/libxml-xpath.html#xmlXPathContext */
	xmlXPathContextPtr context = xmlXPathNewContext(doc);

	/* http://xmlsoft.org/html/libxml-xpath.html#xmlXPathObject */
	xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);


	xmlXPathFreeContext(context);
	return result->nodesetval;
	//http://xmlsoft.org/html/libxml-xpath.html#xmlNodeSet
}