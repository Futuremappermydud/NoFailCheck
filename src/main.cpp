#include "../include/main.hpp"

std::string NoFailText = "<color=#ff0000>NoFail!</color>";
std::string PlayText = "Play";

using namespace il2cpp_utils;

rapidjson::Document& config_doc = Configuration::config;

Il2CppObject* LevelDetail;
Il2CppObject* GameplayMods;
Il2CppObject* PlayButtonObj;
Il2CppObject* PlayButton;
Il2CppObject* TMP;
Il2CppObject* TMPLocalizer;
Il2CppObject* PlayButtonObj_p;
Il2CppObject* PlayButton_p;
Il2CppObject* TMP_p;
Il2CppObject* TMPLocalizer_p;
int ClicksNeeded = 2;
int clicks = 0;

bool NoFail;

struct Config
{
    bool NFT;
    bool Double;
};


MAKE_HOOK_OFFSETLESS(LevelDetailViewController_DidActivate, void, Il2CppObject* self, bool firstActivation, int activationType)
{
    clicks = 0;
    NoFail = *GetPropertyValue<bool>(GameplayMods, "noFail");
    LevelDetail = *GetFieldValue<Il2CppObject*>(self, "_standardLevelDetailView");
    PlayButton = *GetFieldValue<Il2CppObject*>(LevelDetail, "_playButton");
    PlayButtonObj = *GetPropertyValue<Il2CppObject*>(PlayButton, "gameObject");
    if(TMPLocalizer == nullptr)
    {
        TMPLocalizer = *RunMethod<Il2CppObject*>(PlayButtonObj, "GetComponentInChildren", GetSystemType("Polyglot", "LocalizedTextMeshProUGUI"));
        RunMethod("UnityEngine", "Object", "Destroy", TMPLocalizer);
    }
    LevelDetailViewController_DidActivate(self, firstActivation, activationType);
}    
MAKE_HOOK_OFFSETLESS(RefreshContent, void, Il2CppObject* self)
{
    clicks = 0;
    if(TMP == nullptr)
    {
        TMP = *RunMethod<Il2CppObject*>(PlayButtonObj, "GetComponentInChildren", GetSystemType("TMPro", "TextMeshProUGUI"));
    }
    if(NoFail)
    {
        RunMethod(TMP, "set_text", createcsstr(NoFailText));
    }
    else
    {
        RunMethod(TMP, "set_text", createcsstr(PlayText));
    }
    RefreshContent(self);
}

MAKE_HOOK_OFFSETLESS(GetNoFail, void, Il2CppObject* self, Il2CppObject* gameplayModifiers, Il2CppObject* gameplayModifierParams, bool value)
{
    clicks = 0;
    GetNoFail(self, gameplayModifiers, gameplayModifierParams, value);
    GameplayMods = gameplayModifiers;
    NoFail = *GetPropertyValue<bool>(gameplayModifiers, "noFail");
    if(NoFail)
    {
        RunMethod(TMP, "set_text", createcsstr(NoFailText));

    }
    else
    {
        RunMethod(TMP, "set_text", createcsstr(PlayText));
    } 
}
MAKE_HOOK_OFFSETLESS(HandlePlayButtonWasPressed, void, Il2CppObject* self, Il2CppObject* viewController)
{
    if(NoFail)
    {
        if(clicks == ClicksNeeded - 1)
        {
            clicks = 0;
            HandlePlayButtonWasPressed(self, viewController);
        }
        else
        {
            clicks++;
            return;
        } 
    }
    else
    {
        HandlePlayButtonWasPressed(self, viewController);
    }
    
}
/*
void SaveConfig() {
    log(INFO, "Saving Configuration...");
    config_doc.RemoveAllMembers();
    config_doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = config_doc.GetAllocator();
    config_doc.AddMember("No Fail Text Replacement", Config.NFT, allocator);
    config_doc.AddMember("Double Press Required", Config.Double, allocator);
    Configuration::Write();
    log(INFO, "Saved Configuration!");
}

bool LoadConfig() { 
    log(INFO, "Loading Configuration...");
    Configuration::Load();
    bool foundEverything = true;
    if(config_doc.HasMember("SabersStartDiff") && config_doc["SabersStartDiff"].IsBool()){
        Config.NFT = config_doc["SabersStartDiff"].GetBool();    
    }else{
        foundEverything = false;
    }
    if(config_doc.HasMember("Double Press Required") && config_doc["Double Press Required"].IsBool()){
        Config.Double = config_doc["Double Press Required"].GetBool();    
    }else{
        foundEverything = false;
    }
    if(foundEverything){
        log(INFO, "Loaded Configuration!");
        return true;
    }
    return false;
}
*/
extern "C" void load() {
    //if(!LoadConfig())
    //    SaveConfig();
    INSTALL_HOOK_OFFSETLESS(RefreshContent, il2cpp_utils::FindMethodUnsafe("", "StandardLevelDetailView", "RefreshContent", 0));
    INSTALL_HOOK_OFFSETLESS(LevelDetailViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "StandardLevelDetailViewController", "DidActivate", 2));
    INSTALL_HOOK_OFFSETLESS(GetNoFail, il2cpp_utils::FindMethodUnsafe("", "GameplayModifiersModelSO", "SetModifierBoolValue", 3));
    INSTALL_HOOK_OFFSETLESS(HandlePlayButtonWasPressed, il2cpp_utils::FindMethodUnsafe("", "LevelSelectionNavigationController", "HandleLevelDetailViewControllerDidPressPlayButton", 1));
}