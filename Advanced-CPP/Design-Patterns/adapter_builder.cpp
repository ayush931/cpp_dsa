/**
 * Adapter pattern -> make an existing interface usable through another one
 *
 * The adapter wraps an incompatible object and translates calls
 * Object adapter -> holds the adaptee (composition)
 * Class adapter -> inherits from both (multiple inheritance, rare)
 *
 * Builder pattern -> construct a complex object step by step
 * Method chaining returns *this by reference
 * build() validates the result and returns the finished object
 *
 * Builder is useful when a constructor would need many parameters or when
 * some fields are optional.
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

// legacy API: draws with corner coordinates
class LegacyRectangle {
   public:
    void draw(int x1, int y1, int x2, int y2) {
        cout << "legacy rectangle (" << x1 << "," << y1 << ") to (" << x2 << ","
             << y2 << ")" << endl;
    }
};

// new interface: draws with position and size
class Shape {
   public:
    virtual ~Shape() {}

    virtual void draw(int x, int y, int width, int height) = 0;
};

class RectangleAdapter : public Shape {
    LegacyRectangle legacy;

   public:
    void draw(int x, int y, int width, int height) override {
        legacy.draw(x, y, x + width, y + height);
    }
};

// builder
class HttpRequest {
   public:
    string method;
    string url;
    vector<string> headers;
    string body;
    int timeoutSeconds = 30;

    void print() {
        cout << method << " " << url << " timeout=" << timeoutSeconds << endl;

        for (string &header : headers) {
            cout << "  " << header << endl;
        }

        if (!body.empty()) {
            cout << "  body: " << body << endl;
        }
    }
};

class HttpRequestBuilder {
    HttpRequest request;

   public:
    HttpRequestBuilder &withMethod(string method) {
        request.method = method;
        return *this;
    }

    HttpRequestBuilder &withUrl(string url) {
        request.url = url;
        return *this;
    }

    HttpRequestBuilder &withHeader(string key, string value) {
        request.headers.push_back(key + ": " + value);
        return *this;
    }

    HttpRequestBuilder &withBody(string body) {
        request.body = body;
        return *this;
    }

    HttpRequestBuilder &withTimeout(int seconds) {
        request.timeoutSeconds = seconds;
        return *this;
    }

    HttpRequest build() {
        if (request.method.empty()) {
            request.method = "GET";
        }
        if (request.url.empty()) {
            request.url = "/";
        }
        if (request.timeoutSeconds <= 0) {
            request.timeoutSeconds = 30;
        }
        return request;
    }
};

int main() {
    RectangleAdapter adapter;
    adapter.draw(10, 20, 30, 40);
    // legacy rectangle (10,20) to (40,60)

    HttpRequest request = HttpRequestBuilder()
                              .withMethod("POST")
                              .withUrl("https://api.example.com/users")
                              .withHeader("Content-Type", "application/json")
                              .withHeader("Authorization", "Bearer token")
                              .withBody("{\"name\":\"alice\"}")
                              .withTimeout(10)
                              .build();

    request.print();

    HttpRequest defaultRequest = HttpRequestBuilder().build();
    defaultRequest.print();  // GET / timeout=30

    return 0;
}

// Special cases: an adapter must not hide errors from the adaptee;
// coordinate conversions can be lossy; builder validation should happen in
// build(), not in every setter; a builder can be reused only if it resets
// state; fluent methods returning a reference must not return a reference
// to a temporary
