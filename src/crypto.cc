
#include "crypto.h"

using namespace v8;
using namespace node;

Persistent<Function> Crypto::constructor;

Crypto::Crypto() : ctx(NULL) {
}

Crypto::~Crypto() {
}

void Crypto::Init(Handle<Object> exports) {
   Isolate* isolate = Isolate::GetCurrent();

   // Prepare constructor template
   Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
   tpl->SetClassName(String::NewFromUtf8(isolate, "Crypto"));
   tpl->InstanceTemplate()->SetInternalFieldCount(1);

   // Prototype
   NODE_SET_PROTOTYPE_METHOD(tpl, "echo", Echo);
   constructor.Reset(isolate, tpl->GetFunction());
   exports->Set(String::NewFromUtf8(isolate, "Crypto"),
               tpl->GetFunction());
}

void Crypto::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.IsConstructCall()) {
      // Invoked as constructor: `new Crypto(...)`
      Crypto* obj = new Crypto();
      obj->Wrap(args.This());
      args.GetReturnValue().Set(args.This());
  } else {  
      // Invoked as plain function `Crypto(...)`, turn into construct call.
      const int argc = 0;
      Local<Function> cons = Local<Function>::New(isolate, constructor);
      args.GetReturnValue().Set(cons->NewInstance(argc, 0));
  }
}

void Crypto::Echo(const FunctionCallbackInfo<Value>& args) {
   Isolate* isolate = Isolate::GetCurrent();
   HandleScope scope(isolate);

   Local<Object> obj    = args[0]->ToObject();
   char*         data   = Buffer::Data(obj);
   size_t        length = Buffer::Length(obj);

   Local<Object> actualBuffer = Buffer::New(isolate, data, length);
   args.GetReturnValue().Set(actualBuffer);
}

