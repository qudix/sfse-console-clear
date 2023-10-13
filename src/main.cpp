void ClearHistory_Thread(RE::Scaleform::Ptr<RE::IMenu> a_menu)
{
	// UI thread tasks aren't available yet
	std::this_thread::sleep_for(std::chrono::milliseconds(15));
	a_menu->menuObj.Invoke("ClearHistory");
}

bool ClearHistory(const RE::SCRIPT_PARAMETER*, const char*, RE::TESObjectREFR*, RE::TESObjectREFR*, RE::Script*, RE::ScriptLocals*, float*, std::uint32_t*)
{
	const auto ui = RE::UI::GetSingleton();
	for (const auto menu : ui->menuStack) {
		if (menu->menuName == "Console") {
			std::thread(ClearHistory_Thread, menu).detach();
			break;
		}
	}

	return true;
}

void OnMenu(RE::IMenu* a_menu)
{
	static bool hooked{ false };
	if (a_menu->menuName == "Console" && !hooked) {
		if (auto command = RE::Script::LocateConsoleCommand("ClearConsole")) {
			command->executeFunction = ClearHistory;

			logs::info("hooked '{}' console command", command->shortName);

			hooked = true;
		}
	}
}

SFSEPluginLoad(const SFSE::LoadInterface* a_sfse)
{
	SFSE::Init(a_sfse);

	const auto plugin = SFSE::PluginVersionData::GetSingleton();
	logs::info("{} {}", plugin->GetPluginName(), plugin->GetPluginVersion());

	if (const auto menu = SFSE::GetMenuInterface())
		menu->Register(OnMenu);

	return true;
}
