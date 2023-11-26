#include <webview.h>
#include "./build/index.html.h"
#include "./src/topologie/workspace.h"
#include "./src/topologie/uihook.h"
#include <string>
#include <utility>

using namespace std;

std::string ceva(std::string str) {
    return Workspace::getWorkspace().WDeviceAddParser(std::move(str));
}
int main() {
    webview::webview w(false, nullptr);
    w.set_title("Autodragan");
    w.set_size(800, 600, 0);
    hookMainWindow(w);
    string content(reinterpret_cast<char*>(index_html), index_html_len);
    w.set_html(content);
    w.run();
    return 0;

}