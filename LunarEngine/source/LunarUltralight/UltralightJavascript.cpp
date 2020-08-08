#include "../../public/LunarUltralight/UltralightManager.h"


namespace LunarGUI {
    using namespace ultralight;
    void UltralightManager::OnDOMReady(ultralight::View* caller,
        uint64_t frame_id,
        bool is_main_frame,
        const ultralight::String& url) {
        ultralight::Ref<ultralight::JSContext> context = caller->LockJSContext();
        ultralight::SetJSContext(context.get());

        ultralight::JSObject global = ultralight::JSGlobalObject();

        //assignments
        global["LogMSG"] = BindJSCallbackWithRetval(&UltralightManager::LogMSG);
        global["GetProjects"] = BindJSCallbackWithRetval(&UltralightManager::GetProjects);
    }

    ultralight::JSValue UltralightManager::LogMSG(const ultralight::JSObject& object, const ultralight::JSArgs& args) {
        if (!args.empty()) {
            ultralight::JSValue msgObject = args[0];
            ultralight::JSString msgJsString = msgObject.ToString();
            ultralight::String msg = msgJsString;
            std::cout << "Leptron Javascript: " << msg.utf8().data() << std::endl;
        }

        return JSValue();
    }

    ultralight::JSValue UltralightManager::SwitchPage(const ultralight::JSObject& object, const ultralight::JSArgs& args) {
        return JSValue();
    }

    ultralight::JSValue UltralightManager::GetProjects(const ultralight::JSObject& object, const ultralight::JSArgs& args) {
        return JSValue(LunarEditor::ProjectManager::getInstance()->Serialize().c_str());
    }
}