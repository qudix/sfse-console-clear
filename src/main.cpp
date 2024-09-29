bool ClearHistory(const RE::SCRIPT_PARAMETER*, const char*, RE::TESObjectREFR*, RE::TESObjectREFR*, RE::Script*, RE::ScriptLocals*, float*, std::uint32_t*)
{
	const auto ui = RE::UI::GetSingleton();
	if (const auto menu = ui->GetMenu(RE::Console::MENU_NAME))
		// UI thread tasks aren't available yet
		std::thread([](RE::Scaleform::Ptr<RE::IMenu> a_menu) {
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
			a_menu->menuObj.Invoke("ClearHistory");
		}, menu).detach();

	return true;
}

void OnMessage(SFSE::MessagingInterface::Message* a_msg)
{
	if (a_msg->type == SFSE::MessagingInterface::kPostDataLoad) {
		if (auto command = RE::Script::LocateConsoleCommand("ClearConsole")) {
			command->executeFunction = ClearHistory;

			logs::info("hooked '{}' console command", command->shortName);
		}
	}
}

SFSEPluginLoad(const SFSE::LoadInterface* a_sfse)
{
	SFSE::Init(a_sfse);

	if (const auto message = SFSE::GetMessagingInterface())
		message->RegisterListener(OnMessage);

	return true;
}
