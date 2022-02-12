// dllmain.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
#include "../ToxicAIO/plugin_sdk/plugin_sdk.hpp"

PLUGIN_NAME("ToxicAIO");
SUPPORTED_CHAMPIONS(champion_id::Kennen);

#include "kennen.h"

PLUGIN_API bool on_sdk_load(plugin_sdk_core* plugin_sdk_good)
{
	DECLARE_GLOBALS(plugin_sdk_good);

	switch (myhero->get_champion())
	{
	case champion_id::Kennen:
		kennen::load();
		break;
	default:
		console->print("[ToxicAIO] Champion is currently not supported", myhero->get_model_cstr());
		return false;
	}

	console->print("[ToxicAIO] Champion loaded successfully Have Fun", myhero->get_model_cstr());
	return true;
}

PLUGIN_API void on_sdk_unload()
{
	switch (myhero->get_champion())
	{
	case champion_id::Kennen:
		kennen::unload();
		break;
	default:
		break;
	}
}