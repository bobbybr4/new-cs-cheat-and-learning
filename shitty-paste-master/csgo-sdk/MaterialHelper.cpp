#include "MaterialHelper.h"

CMaterialHelper* g_pMaterialHelper = nullptr;

typedef void(__thiscall* orgGameFunc_InitKeyValues)(void* thisptr, const char* name);
orgGameFunc_InitKeyValues pInitKeyValues;
void CMaterialHelper::GameFunc_InitKeyValues(KeyValues* pKeyValues, const char* name)
{
	pInitKeyValues = (orgGameFunc_InitKeyValues)(offsets->InitKeyValuesEx);
	pInitKeyValues(pKeyValues, name);
}

typedef void(__thiscall* orgGameFunc_LoadFromBuffer)(void* thisptr, const char* resourceName, const char* pBuffer, /*IBaseFileSystem**/void* pFileSystem, const char* pPathID, void* pfnEvaluateSymbolProc);
orgGameFunc_LoadFromBuffer pLoadFromBuffer;
void CMaterialHelper::GameFunc_LoadFromBuffer(KeyValues* pKeyValues, const char* resourceName, const char* pBuffer, /*IBaseFileSystem**/void* pFileSystem, const char* pPathID, void* pfnEvaluateSymbolProc)
{
	pLoadFromBuffer = (orgGameFunc_LoadFromBuffer)(offsets->LoadFromBufferEx);
	pLoadFromBuffer(pKeyValues, resourceName, pBuffer, pFileSystem, pPathID, pfnEvaluateSymbolProc);
}

IMaterial* CMaterialHelper::CreateMaterial(bool shouldIgnoreZ, bool isLit, bool isWireframe) //credits to ph0ne
{
	static int created = 0;

	static const char tmp[] =
	{
		"\"%s\"\
				\n{\
						\n\t\"$basetexture\" \"vgui/white_additive\"\
								\n\t\"$envmap\" \"\"\
										\n\t\"$model\" \"1\"\
												\n\t\"$flat\" \"1\"\
														\n\t\"$nocull\" \"0\"\
																\n\t\"$selfillum\" \"1\"\
																		\n\t\"$halflambert\" \"1\"\
																				\n\t\"$nofog\" \"0\"\
																						\n\t\"$ignorez\" \"%i\"\
																								\n\t\"$znearer\" \"0\"\
																										\n\t\"$wireframe\" \"%i\"\
																										        \n}\n"
	};

	char* baseType = (isLit == true ? XorStr("VertexLitGeneric") : XorStr("UnlitGeneric"));
	char material[512];
	sprintf_s(material, sizeof(material), tmp, baseType, (shouldIgnoreZ) ? 1 : 0, (isWireframe) ? 1 : 0);

	char name[512];
	sprintf_s(name, sizeof(name), XorStr("#mage_is_gay%i.vmt"), created);
	++created;

	KeyValues* keyValues = new KeyValues(name);
	GameFunc_InitKeyValues(keyValues, baseType);
	GameFunc_LoadFromBuffer(keyValues, name, material, nullptr, NULL, nullptr);

	IMaterial *createdMaterial = Interfaces->MaterialSystem->CreateMaterial(name, keyValues);
	createdMaterial->IncrementReferenceCount();

	return createdMaterial;
}

void CMaterialHelper::ForceMaterial(Color color, IMaterial* material, bool useColor, bool forceMaterial)
{
	if (useColor)
	{
		float temp[3] =
		{
			color.r(),
			color.g(),
			color.b()
		};

		temp[0] /= 255.f;
		temp[1] /= 255.f;
		temp[2] /= 255.f;


		float alpha = color.a();

		Interfaces->RenderView->SetBlend(1.0f);
		Interfaces->RenderView->SetColorModulation(temp);
	}

	if (forceMaterial)
		Interfaces->ModelRender->ForcedMaterialOverride(material);
	else
		Interfaces->ModelRender->ForcedMaterialOverride(NULL);
}

CMaterialHelper::CMaterialHelper()
{
	CoveredLit = CreateMaterial(true);
	OpenLit = CreateMaterial(false);
	CoveredFlat = CreateMaterial(true, false);
	OpenFlat = CreateMaterial(false, false);
	Wire = CreateMaterial(true, true, true);
	AddWire = CreateMaterial(false, true, true);
}