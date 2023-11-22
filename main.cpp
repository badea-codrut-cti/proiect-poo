#include <functional>
#include <webview.h>
#include "./build/index.html.h"
#include "./src/topologie/workspace.h"
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
    string content(reinterpret_cast<char*>(index_html), index_html_len);
    w.set_html(content);
    w.bind("wDeviceAdd", std::function<std::string(std::string)>(ceva));
    w.run();
    return 0;

}