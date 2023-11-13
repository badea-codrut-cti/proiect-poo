#include <cstring>
#include <functional>
#include <iostream>
#include <webview.h>
#include "./build/index.html.h"
#include <string>

using namespace std;

std::string test(std::string a) {
    cout << a << "\n";
    
    return a;
}

int main() {
    webview::webview w(false, nullptr);
    w.set_title("Autodragan");
    w.set_size(800, 600, 0);
    string content(reinterpret_cast<char*>(index_html), index_html_len);
    w.set_html(content);
    w.bind("ceva", std::function<std::string(std::string)>(test));
    w.run();
    return 0;

}