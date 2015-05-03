#include <node.h>
#include <v8.h>

using namespace v8;

void SkeinName(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "skein"));
}

void init(Handle<Object> target) {
  NODE_SET_METHOD(target, "name", SkeinName);
}

NODE_MODULE(skein, init);