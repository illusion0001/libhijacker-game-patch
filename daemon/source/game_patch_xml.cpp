#include "../extern/mxml/mxml.h"
#include "game_patch_xml_cfg.hpp"
#include <stdint.h>

#include "game_patch_memory.hpp"
#include "notify.hpp"

// Include the `game_patch_fliprate_list.xml` as a symbol
__asm__(
	".intel_syntax noprefix\n"
	".section .data\n"
	"DefaultXml_FliprateList:\n"
	".incbin \".." XML_PATH_LIST "\"\n");

extern "C" const char DefaultXml_FliprateList[];

int makeDefaultXml_List()
{
	FILE *f = fopen(XML_PATH_LIST, "rb");
	if (!f)
	{
		FILE *new_f = fopen(XML_PATH_LIST, "w");
		// Print default data to TTY
		printf("%s\n", DefaultXml_FliprateList);
		fputs(DefaultXml_FliprateList, new_f);
		fflush(new_f);
		fclose(new_f);
		printf_notification("Created default config file:\n" XML_PATH_LIST);
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
	printf("File: " XML_PATH_LIST " exist at 0x%p\n", f);
	if (f)
	{
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = (char *)malloc(length);
		if (buffer)
		{
			fread(buffer, 1, length, f);
			printf("Memory at 0x%p\n", buffer);
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

// This is so bad but we want to generate a default config with the same key
// that is used for comparisons
#define SET_XML_KEY(key, value) "    <" key ">" #value "</" key ">\n"
const char DefaultCfgData[] = ""
				 "<cfg>\n"
				 "    <!-- Supported key value: `true`, `false`, `1`, `0` -->\n"
					SET_XML_KEY(GR1_EN, 0)
					SET_XML_KEY(GR2_60HzKey, 0)
					SET_XML_KEY(BB_60FPSKey, 1)
					SET_XML_KEY(BB_MBKey, 0)
					SET_XML_KEY(BB_CAKey, 0)
					SET_XML_KEY(BB_DebugCameraKey, 0)
					SET_XML_KEY(BB_ColorBorder, 0)
					SET_XML_KEY(BB_Vsync, 0)
					SET_XML_KEY(BB_1280_720, 0)
					SET_XML_KEY(SOTC_DebugMenu, 0)
					SET_XML_KEY(TO1886_60FPS, 1)
					SET_XML_KEY(TO1886_16_9, 0)
					SET_XML_KEY(Big4R_MainDebugMenu, 0)
					SET_XML_KEY(Big4R_TLLDebugMenu, 0)
					SET_XML_KEY(DemonSouls_UnlockFPS, 0)
					SET_XML_KEY(DemonSouls_DebugMenu, 0)
					SET_XML_KEY(Driveclub_60FPS, 1)
					SET_XML_KEY(Driveclub_DLC_Unlock, 1)
					SET_XML_KEY(TLG_60FPS, 0)
					SET_XML_KEY(t2ps4_109_1080p_in120Hz, 0)
					SET_XML_KEY(BPR_103_120Hz, 0)
					SET_XML_KEY(BPR_103_1080p, 1)
					SET_XML_KEY(BPR_103_SkipLogos, 1)
					SET_XML_KEY(BPR_103_SkipIntroVideo, 0)
					"</cfg>\n";
#undef SET_XML_KEY

int makeDefaultXml_Cfg()
{
	FILE *f = fopen(XML_PATH, "rb");
	if (!f)
	{
		FILE *new_f = fopen(XML_PATH, "w");
		// Print the default data to TTY
		printf("%s\n", DefaultCfgData);
		fputs(DefaultCfgData, new_f);
		fflush(new_f);
		fclose(new_f);
		printf_notification("Created default config file:\n" XML_PATH);
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

	printf("File " XML_PATH " exist at 0x%p\n", f);
	if (f)
	{
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = (char *)malloc(length);
		if (buffer)
		{
			fread(buffer, 1, length, f);
			printf("Memory at 0x%p\n", buffer);
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
