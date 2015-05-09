
#include "crypto.h"
#include <string>

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
   NODE_SET_PROTOTYPE_METHOD(tpl, "hash", Hash);
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

static std::string getString(v8::Local<v8::String> str)
{
   char* buf = new char[str->Length() + 1];
   str->WriteUtf8(buf);
   std::string value = reinterpret_cast<char*>(buf);
   delete[] buf;
   return value;
}

void Crypto::Hash(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.Length() < 1) {
    isolate->ThrowException(Exception::Error(
        String::NewFromUtf8(isolate, "There must be one string argument for password")));
    return;
  }

  if (!args[0]->IsString()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Argument type should be a string")));
    return;
  }

  std::string password = getString(args[0]->ToString());

  unsigned char* data = sk_get_key(password.c_str(), SKEIN_BITS);

  Local<Object> actualBuffer = Buffer::New(isolate, (char*)data, (size_t)SKEIN_BYTES);
  args.GetReturnValue().Set(actualBuffer);
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

