#include "ScriptedTriggersUpdater.h"
#include "Log.h"



void updateIdeologyScriptedTriggers(HoI4::ScriptedTriggers& scriptedTriggers,
	 const std::set<std::string>& majorIdeologies);
void updateElectionsScriptedTriggers(HoI4::ScriptedTriggers& scriptedTriggers,
	 const std::set<std::string>& majorIdeologies);
void updateLawsWarSupportTriggers(HoI4::ScriptedTriggers& scriptedTriggers,
	 const std::set<std::string>& majorIdeologies);
void updateNationalFocusTriggers(HoI4::ScriptedTriggers& scriptedTriggers,
	 const std::set<std::string>& majorIdeologies);
void HoI4::updateScriptedTriggers(ScriptedTriggers& scriptedTriggers, const std::set<std::string>& majorIdeologies)
{
	Log(LogLevel::Info) << "\tUpdating scripted triggers";
	updateIdeologyScriptedTriggers(scriptedTriggers, majorIdeologies);
	updateElectionsScriptedTriggers(scriptedTriggers, majorIdeologies);
	updateLawsWarSupportTriggers(scriptedTriggers, majorIdeologies);
	updateNationalFocusTriggers(scriptedTriggers, majorIdeologies);
}


void updateIdeologyScriptedTriggers(HoI4::ScriptedTriggers& scriptedTriggers,
	 const std::set<std::string>& majorIdeologies)
{
	std::string body = "= {\n";
	body += "\tOR = {\n";
	for (const auto& ideology: majorIdeologies)
	{
		if (ideology == "neutrality")
		{
			continue;
		}
		body += "\t\tAND = {\n";
		body += "\t\t\thas_government = " + ideology + "\n";
		body += "\t\t\tROOT = {\n";
		body += "\t\t\t\tOR = {\n";
		for (const auto& secondIdeology: majorIdeologies)
		{
			if ((secondIdeology == ideology) || (secondIdeology == "neutrality"))
			{
				continue;
			}
			body += "\t\t\t\t\thas_government = " + secondIdeology + "\n";
		}
		body += "\t\t\t\t}\n";
		body += "\t\t\t}\n";
		body += "\t\t}\n";
	}
	body += "\t}\n";
	body += "}";

	scriptedTriggers.replaceIdeologyScriptedTrigger("is_enemy_ideology", body);
}



void updateElectionsScriptedTriggers(HoI4::ScriptedTriggers& scriptedTriggers,
	 const std::set<std::string>& majorIdeologies)
{
	std::string supportBody = "= {\n";
	for (const auto& ideology: majorIdeologies)
	{
		if (ideology == "neutrality")
		{
			continue;
		}
		if (ideology == "democratic")
		{
			supportBody += "\tdemocratic > 0.65\n";
		}
		else
		{
			supportBody += "\t" + ideology + " < 0.18\n";
		}
	}
	supportBody += "}";

	scriptedTriggers.replaceElectionsScriptedTrigger("can_lose_democracy_support", supportBody);
}


std::string getHasUnsupportedManpowerLawBody(const std::set<std::string>& majorIdeologies);
std::string getHasExcessiveArmySizeBody(const std::set<std::string>& majorIdeologies);
void updateLawsWarSupportTriggers(HoI4::ScriptedTriggers& scriptedTriggers,
	 const std::set<std::string>& majorIdeologies)
{
	std::map<std::string_view, std::string> replacements;
	replacements.insert(
		 std::make_pair("has_unsupported_manpower_law", getHasUnsupportedManpowerLawBody(majorIdeologies)));
	replacements.insert(std::make_pair("has_excessive_army_size", getHasExcessiveArmySizeBody(majorIdeologies)));
	scriptedTriggers.replaceLawsWarSupportTriggers(replacements);
}


std::string getHasUnsupportedManpowerLawBody(const std::set<std::string>& majorIdeologies)
{
	std::string unsupportedManpowerBody = "= {\n";
	unsupportedManpowerBody += "\tif = {\n";
	unsupportedManpowerBody += "\t\tlimit = {\n";
	unsupportedManpowerBody += "\t\t\thas_idea = limited_conscription\n";
	unsupportedManpowerBody += "\t\t}\n";
	unsupportedManpowerBody += "\t\thas_idea = limited_conscription\n";
	unsupportedManpowerBody += "\t\thas_war_support < 0.1\n";
	unsupportedManpowerBody += "\t}\n";
	unsupportedManpowerBody += "\telse_if = {\n";
	unsupportedManpowerBody += "\t\tlimit = {\n";
	unsupportedManpowerBody += "\t\t\thas_idea = extensive_conscription\n";
	unsupportedManpowerBody += "\t\t}\n";
	unsupportedManpowerBody += "\t\thas_idea = extensive_conscription\n";
	unsupportedManpowerBody += "\t\thas_war_support < 0.2\n";
	for (const auto& majorIdeology: majorIdeologies)
	{
		if ((majorIdeology == "democratic") || (majorIdeology == "neutrality"))
		{
			continue;
		}

		unsupportedManpowerBody += "\t\tNOT = { has_government = " + majorIdeology + " }\n";
	}
	unsupportedManpowerBody += "\t}\n";
	unsupportedManpowerBody += "\telse_if = {\n";
	unsupportedManpowerBody += "\t\tlimit = {\n";
	unsupportedManpowerBody += "\t\t\thas_idea = service_by_requirement\n";
	unsupportedManpowerBody += "\t\t}\n";
	unsupportedManpowerBody += "\t\thas_idea = service_by_requirement\n";
	unsupportedManpowerBody += "\t\thas_war_support < 0.6\n";
	for (const auto& majorIdeology: majorIdeologies)
	{
		if ((majorIdeology == "democratic") || (majorIdeology == "neutrality"))
		{
			continue;
		}

		unsupportedManpowerBody += "\t\tNOT = { has_government = " + majorIdeology + " }\n";
	}
	unsupportedManpowerBody += "\t}\n";
	unsupportedManpowerBody += "\telse_if = {\n";
	unsupportedManpowerBody += "\t\tlimit = {\n";
	unsupportedManpowerBody += "\t\t\thas_idea = all_adults_serve\n";
	unsupportedManpowerBody += "\t\t}\n";
	unsupportedManpowerBody += "\t\thas_idea = all_adults_serve\n";
	unsupportedManpowerBody += "\t}\n";
	unsupportedManpowerBody += "\telse_if = {\n";
	unsupportedManpowerBody += "\t\tlimit = {\n";
	unsupportedManpowerBody += "\t\t\thas_idea = scraping_the_barrel\n";
	unsupportedManpowerBody += "\t\t}\n";
	unsupportedManpowerBody += "\t\thas_idea = scraping_the_barrel\n";
	unsupportedManpowerBody += "\t}\n";
	unsupportedManpowerBody += "\telse = {\n";
	unsupportedManpowerBody += "\t\talways = no\n";
	unsupportedManpowerBody += "\t}\n";
	unsupportedManpowerBody += "}\n";
	return unsupportedManpowerBody;
}


std::string getHasExcessiveArmySizeBody(const std::set<std::string>& majorIdeologies)
{
	std::string body = "= {\n";
	body += "\tOR = {\n";
	body += "\t\tAND = {\n";
	body += "\t\t\tnum_of_factories < 25\n";
	body += "\t\t\thas_army_manpower = { size = 250000 }\n";
	body += "\t\t\thas_manpower < 1\n";
	if (majorIdeologies.contains("communism"))
	{
		body += "\t\t\tNOT = {\n";
		body += "\t\t\t\thas_government = communism\n";
		body += "\t\t\t}\n";
	}
	if (majorIdeologies.contains("radical"))
	{
		body += "\t\t\tNOT = {\n";
		body += "\t\t\t\thas_government = radical\n";
		body += "\t\t\t}\n";
	}
	body += "\t\t}\n";
	body += "\t\tAND = {\n";
	body += "\t\tnum_of_factories < 50\n";
	body += "\t\thas_army_manpower = { size = 500000 }\n";
	body += "\t\thas_manpower < 1\n";
	if (majorIdeologies.contains("communism"))
	{
		body += "\t\t\tNOT = {\n";
		body += "\t\t\t\thas_government = communism\n";
		body += "\t\t\t}\n";
	}
	if (majorIdeologies.contains("radical"))
	{
		body += "\t\t\tNOT = {\n";
		body += "\t\t\t\thas_government = radical\n";
		body += "\t\t\t}\n";
	}
	body += "\t\t}\n";
	body += "\t\tAND = {\n";
	body += "\t\t\tnum_of_factories < 75\n";
	body += "\t\t\thas_army_manpower = { size = 750000 }\n";
	body += "\t\t\thas_manpower < 1\n";
	if (majorIdeologies.contains("communism"))
	{
		body += "\t\t\tNOT = {\n";
		body += "\t\t\t\thas_government = communism\n";
		body += "\t\t\t}\n";
	}
	if (majorIdeologies.contains("radical"))
	{
		body += "\t\t\tNOT = {\n";
		body += "\t\t\t\thas_government = radical\n";
		body += "\t\t\t}\n";
	}
	body += "\t\t}\n";
	body += "\t\tAND = {\n";
	body += "\t\t\tnum_of_factories < 100\n";
	body += "\t\t\thas_army_manpower = { size = 1000000 }\n";
	body += "\t\t\thas_manpower < 1\n";
	if (majorIdeologies.contains("communism"))
	{
		body += "\t\t\tNOT = {\n";
		body += "\t\t\t\thas_government = communism\n";
		body += "\t\t\t}\n";
	}
	if (majorIdeologies.contains("radical"))
	{
		body += "\t\t\tNOT = {\n";
		body += "\t\t\t\thas_government = radical\n";
		body += "\t\t\t}\n";
	}
	body += "\t\t}\n";
	if (majorIdeologies.contains("democratic"))
	{
		body += "\t\tAND = {\n";
		body += "\t\t\thas_army_manpower = { size = 2500000 }\n";
		body += "\t\t\thas_manpower < 1\n";
		body += "\t\t\thas_government = democratic\n";
		body += "\t\t}\n";
	}
	if (majorIdeologies.contains("fascism"))
	{
		body += "\t\tAND = {\n";
		body += "\t\t\thas_army_manpower = { size = 3000000 }\n";
		body += "\t\t\thas_manpower < 1\n";
		body += "\t\t\thas_government = fascism\n";
		body += "\t\t}\n";
	}
	if (majorIdeologies.contains("absolutist"))
	{
		body += "\t\tAND = {\n";
		body += "\t\t\thas_army_manpower = { size = 3000000 }\n";
		body += "\t\t\thas_manpower < 1\n";
		body += "\t\t\thas_government = absolutist\n";
		body += "\t\t}\n";
	}
	body += "\t}\n";
	body += "}\n";
	return body;
}

void updateNationalFocusTriggers(HoI4::ScriptedTriggers& scriptedTriggers,
	 const std::set<std::string>& majorIdeologies)
{
	if (majorIdeologies.contains("communism"))
	{
		HoI4::ScriptedTrigger NFTrigger("potential_communist_puppet_target");
		std::string body = "= {\n";
		body += "\tcustom_trigger_tooltip = {\n";
		body += "\t\ttooltip = potential_communist_puppet_target_tooltip\n";
		body += "\t\thidden_trigger = {\n";
		body += "\t\t\tROOT = { has_government = communism }\n";
		body += "\t\t\tNOT = { has_government = communism }\n";
		body += "\t\t}\n";
		body += "\t}\n";
		body += "}";
		NFTrigger.setBody(body);

		scriptedTriggers.addNationalFocusTrigger(NFTrigger);
	}

	if (majorIdeologies.contains("fascism"))
	{
		HoI4::ScriptedTrigger NFTrigger("potential_fascist_annex_target");
		std::string body = "= {\n";
		body += "\tcustom_trigger_tooltip = {\n";
		body += "\t\ttooltip = potential_fascist_annex_target_tooltip\n";
		body += "\t\thidden_trigger = {\n";
		body += "\t\t\tROOT = { has_government = fascism }\n";
		body += "\t\t\tNOT = { has_government = fascism }\n";
		body += "\t\t}\n";
		body += "\t}\n";
		body += "}";
		NFTrigger.setBody(body);

		scriptedTriggers.addNationalFocusTrigger(NFTrigger);
	}

	if (majorIdeologies.contains("radical"))
	{
		HoI4::ScriptedTrigger NFTrigger("potential_radical_annex_target");
		std::string body = "= {\n";
		body += "\tcustom_trigger_tooltip = {\n";
		body += "\t\ttooltip = potential_radical_annex_target_tooltip\n";
		body += "\t\thidden_trigger = {\n";
		body += "\t\t\tROOT = { has_government = radical }\n";
		body += "\t\t\tNOT = { has_government = radical }\n";
		body += "\t\t}\n";
		body += "\t}\n";
		body += "}";
		NFTrigger.setBody(body);

		scriptedTriggers.addNationalFocusTrigger(NFTrigger);
	}
}