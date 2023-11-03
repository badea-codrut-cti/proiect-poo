#include <cstring>
#include <iostream>
#include "./ext/webview/webview.h"

using namespace std;

int main() {
    webview_t w = webview_create(0, NULL);
    webview_set_title(w, "Basic Example");
    webview_set_size(w, 480, 320, WEBVIEW_HINT_NONE);
    webview_set_html(w, "Thanks for using webview!");
    webview_run(w);
    webview_destroy(w);
    return 0;
}