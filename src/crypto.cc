
#include "crypto.h"
#include <string>

using namespace v8;
using namespace node;

Persistent<Function> Crypto::constructor;

Crypto::Crypto() : ctx(NULL) {
}

Crypto::~Crypto() {
}

void Crypto::Init (Handle<Object> exports) {
   Isolate* isolate = Isolate::GetCurrent();

   // Prepare constructor template
   Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
   tpl->SetClassName(String::NewFromUtf8(isolate, "Crypto"));
   tpl->InstanceTemplate()->SetInternalFieldCount(1);

   // Prototype
   NODE_SET_PROTOTYPE_METHOD(tpl, "echo", Echo);
   NODE_SET_PROTOTYPE_METHOD(tpl, "calcHash", CalcHash);
   NODE_SET_PROTOTYPE_METHOD(tpl, "setHash", SetHash);
   NODE_SET_PROTOTYPE_METHOD(tpl, "getHash", GetHash);
   NODE_SET_PROTOTYPE_METHOD(tpl, "encrypt", Encrypt);
   NODE_SET_PROTOTYPE_METHOD(tpl, "decrypt", Decrypt);

   constructor.Reset(isolate, tpl->GetFunction());
   exports->Set(String::NewFromUtf8(isolate, "Crypto"),
               tpl->GetFunction());
}

void Crypto::New (const FunctionCallbackInfo<Value>& args) {
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

static std::string getString (v8::Local<v8::String> str) {
  char* buf = new char[str->Length() + 1];
  str->WriteUtf8(buf);
  std::string value = reinterpret_cast<char*>(buf);
  delete[] buf;
  return value;
}

void Crypto::Encrypt (const FunctionCallbackInfo<Value>& args) {

}

void Crypto::Decrypt (const FunctionCallbackInfo<Value>& args) {

}

void Crypto::SetHash (const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.Length() < 1 || !args[0]->IsObject()) {
    isolate->ThrowException(Exception::Error(
        String::NewFromUtf8(isolate, "There must be one Buffer argument for hash")));
    return;
  }

  Local<Object> obj = args[0]->ToObject();
  char* data  = Buffer::Data(obj);
  size_t length = Buffer::Length(obj);

  if (length != 64) {
    isolate->ThrowException(Exception::Error(
        String::NewFromUtf8(isolate, "The Buffer argument's length must be 64 bytes")));
    return;
  }

  Crypto* crp = ObjectWrap::Unwrap<Crypto>(args.Holder());
  delete crp->ctx;
  crp->ctx = new SKCipher();

  sk_init(crp->ctx, (const unsigned char*)data, SKEIN_BITS);

  return;
}

void Crypto::GetHash (const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.Length() < 1 || !args[0]->IsFunction()) {
    isolate->ThrowException(Exception::Error(
      String::NewFromUtf8(isolate, "There must be one function argument")));
    return;
  }
  
  const unsigned argc = 2;
  Local<Function> cb = Local<Function>::Cast(args[0]);

  Crypto* crp = ObjectWrap::Unwrap<Crypto>(args.Holder());
  if (crp->ctx == NULL) {
    Local<Value> argv[argc] = { 
      Exception::Error(String::NewFromUtf8(isolate, "The hash must be set first. Use setHash")), 
      Undefined(isolate) };
      cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
    return;
  }

  Local<Value> argv[argc] = { 
    Undefined(isolate), 
    Buffer::New(isolate, (char*)crp->ctx->key, SKEIN_BYTES) };
  cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
}

void Crypto::CalcHash (const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.Length() < 2) {
    isolate->ThrowException(Exception::Error(
      String::NewFromUtf8(isolate, "There must be two arguments")));
    return;
  }

  if (!args[0]->IsString() || !args[1]->IsFunction()) {
    isolate->ThrowException(Exception::Error(
      String::NewFromUtf8(isolate, "Expected a string and a function")));
    return;
  }

  std::string password = getString(args[0]->ToString());
  unsigned char* data = sk_get_key(password.c_str(), SKEIN_BITS);
  Local<Object> actualBuffer = Buffer::New(isolate, (char*)data, (size_t)SKEIN_BYTES);
  
  const unsigned argc = 2;
  Local<Value> argv[argc] = { Undefined(isolate), Buffer::New(isolate, (char*)data, (size_t)SKEIN_BYTES) };
  Local<Function> cb = Local<Function>::Cast(args[1]);
  cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
}

void Crypto::Echo (const FunctionCallbackInfo<Value>& args) {
   Isolate* isolate = Isolate::GetCurrent();
   HandleScope scope(isolate);

   Local<Object> obj    = args[0]->ToObject();
   char*         data   = Buffer::Data(obj);
   size_t        length = Buffer::Length(obj);

   Local<Function> cb = Local<Function>::Cast(args[1]);
   const unsigned argc = 1;
   Local<Value> argv[argc] = { Buffer::New(isolate, data, length) };
   cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
}

