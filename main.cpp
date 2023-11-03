#include <cstring>
#include <iostream>
#include "./ext/webview/webview.h"
#include "./build/index.html.h"
#include <string>

using namespace std;

int main() {
    webview_t w = webview_create(0, NULL);
    webview_set_title(w, "Basic Example");
    webview_set_size(w, 480, 320, WEBVIEW_HINT_NONE);
    string content(reinterpret_cast<char*>(assets_index_html), assets_index_html_len);
    webview_set_html(w, content.c_str());
    webview_run(w);
    webview_destroy(w);
    return 0;
}