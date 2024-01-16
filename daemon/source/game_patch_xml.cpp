#include "../extern/mxml/mxml.h"
#include "game_patch_xml_cfg.hpp"
#include <stdint.h>

#include "game_patch_memory.hpp"

// Include the `game_patch_fliprate_list.xml` as a symbol
__asm__(
	".intel_syntax noprefix\n"
	".section .data\n"
	"DefaultXml_FliprateList:\n"
	".incbin \".." XML_PATH_LIST "\"\n");

extern "C" char DefaultXml_FliprateList[];

int makeDefaultXml_List(void)
{
	printf("%s", DefaultXml_FliprateList);
	FILE *f = fopen(XML_PATH_LIST, "rb");
	if (!f)
	{
		FILE *new_f = fopen(XML_PATH_LIST, "w");
		printf("%s", DefaultXml_FliprateList);
		fputs(DefaultXml_FliprateList, new_f);
		fflush(new_f);
		fclose(new_f);
	}
	return 0;
}

extern "C" void *malloc(size_t);
extern "C" void free(void *);

int Xml_parseTitleID(const char *titleId)
{
	makeDefaultXml_List();
	char *buffer = NULL;
	uint64_t length = 0;
	FILE *f = fopen(XML_PATH_LIST, "rb");
	// int ret = 0;
	printf("File: " XML_PATH_LIST "\n");
	if (f)
	{
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = (char *)malloc(length);
		if (buffer)
		{
			fread(buffer, 1, length, f);
		}
		fclose(f);
	}
	mxml_node_t *tree = mxmlLoadString(NULL, buffer, MXML_OPAQUE_CALLBACK);

	if (tree == NULL)
	{
		return 1;
	}

	mxml_node_t *titleIDNode = mxmlFindElement(tree, tree, "TitleID", NULL, NULL, MXML_DESCEND);

	if (titleIDNode != NULL)
	{
		mxml_node_t *idNode = mxmlFindElement(titleIDNode, tree, "ID", NULL, NULL, MXML_DESCEND);

		while (idNode != NULL)
		{
			const char *idValue = mxmlGetOpaque(idNode);

			if (idValue != NULL)
			{
				// printf("TitleID: %s\n", idValue);
				if (strncmp(titleId, idValue, __builtin_strlen("CUSAxxxxx")) == 0)
				{
					printf("%s match !!\n", titleId);
				}
			}
			idNode = mxmlFindElement(idNode, titleIDNode, "ID", NULL, NULL, MXML_NO_DESCEND);
		}
	}

	if (buffer)
	{
		free(buffer);
	}
	if (tree)
	{
		mxmlDelete(tree);
	}
	return 0;
}

int simple_get_bool(const char *val)
{
	if (val == NULL || val[0] == 0)
	{
		return 0;
	}
	if (
		val[0] == '1' ||
		startsWith(val, "on") ||
		startsWith(val, "true") ||
		startsWith(val, "On") ||
		startsWith(val, "True"))
	{
		return 1;
	}
	else if (
		val[0] == '0' ||
		startsWith(val, "off") ||
		startsWith(val, "false") ||
		startsWith(val, "Off") ||
		startsWith(val, "False"))
	{
		return 0;
	}
	return 0;
}

#undef startsWith

int makeDefaultXml_Cfg(void)
{
	FILE *f = fopen(XML_PATH, "rb");
	if (!f)
	{
		FILE *new_f = fopen(XML_PATH, "w");
		// This is so bad but we want to generate a default config with the same key
		// that is used for comparisons
#define SET_XML_KEY(key, state) "    <" key ">" #state "</" key ">\n"
#define XML_DATA ""                                                                                       \
				 "<cfg>\n    "                                                                            \
				 "<!-- Supported key value: `true`, `false`, `1`, `0` -->\n" SET_XML_KEY(GR1_120HzKey, 1) \
					 SET_XML_KEY(GR2_120HzKey, 0)                                                         \
					 SET_XML_KEY(BB_60FPSKey, 1)                                   		                  \
					 SET_XML_KEY(BB_MBKey, 0)                                    		                  \
					 SET_XML_KEY(BB_CAKey, 0) "</cfg>\n"
		printf(XML_DATA);
		fputs(XML_DATA, new_f);
		fflush(new_f);
		fclose(new_f);
#undef XML_DATA
#undef SET_XML_KEY
	}
	return 0;
}

int parseXML(const char *xml_key)
{
	makeDefaultXml_Cfg();
	char *buffer = NULL;
	uint64_t length = 0;
	FILE *f = fopen(XML_PATH, "rb");
	int ret = 0;

	if (f)
	{
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = (char *)malloc(length);
		if (buffer)
		{
			fread(buffer, 1, length, f);
		}
		fclose(f);
	}
	else
	{
		return -2;
	}
	mxml_node_t *tree = mxmlLoadString(NULL, buffer, MXML_OPAQUE_CALLBACK);
	if (tree == NULL)
	{
		printf("Failed to parse XML.\n");
		return 0;
	}

	mxml_node_t *xml_element = mxmlFindElement(tree, tree, xml_key, NULL, NULL, MXML_DESCEND);
	if (xml_element)
	{
		const char *value = mxmlGetOpaque(xml_element);
		if (value)
		{
			ret = simple_get_bool(value);
			printf("%s: (%s) 0x%08x\n", xml_key, value, ret);
		}
	}
	if (buffer)
	{
		free(buffer);
	}
	if (tree)
	{
		mxmlDelete(tree);
	}
	return ret;
}
