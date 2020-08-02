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
        ultralight::String page_title = args[0].ToString();

        uint32_t pWidth = args[1].ToInteger();
        uint32_t pHeight = args[2].ToInteger();
        ultralight::String new_page = args[3].ToString();

        std::cout << page_title.utf8().data() << std::endl;

        for(auto& layout : layouts) {
            if(layout.active) {
                for(auto& pane : layout._panes) {
                    bool dimsCheck = pane._view->get()->width() == pWidth && pane._view->get()->height() == pHeight;
                    if(pane._view->get()->title().utf8().data() == page_title.utf8().data() && dimsCheck) {
                        pane._view->get()->LoadURL("file:///" + new_page);
                    }
                }
            }
        }

        return JSValue();
    }
}