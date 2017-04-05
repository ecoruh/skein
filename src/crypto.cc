    
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

static std::string getString (Local<String> str) {
  char* buf = new char[str->Utf8Length() + 1];
  str->WriteUtf8(buf);
  std::string value = reinterpret_cast<char*>(buf);
  delete[] buf;
  return value;
}

void Crypto::Encrypt (const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  
  if (args.Length() < 3) {
    isolate->ThrowException(Exception::Error(
      String::NewFromUtf8(isolate, "There must be 3 arguments")));
    return;
  }

  if (!args[0]->IsObject() || !args[1]->IsString() || !args[2]->IsFunction()) {
    isolate->ThrowException(Exception::Error(
      String::NewFromUtf8(isolate, "Expected a buffer, a string and a function")));
    return;
  }

  Local<Object> hashObj = args[0]->ToObject();
  Local<String> clearTextStr = args[1]->ToString();
  const unsigned argc = 2;
  Local<Function> cb = Local<Function>::Cast(args[2]);

  if (!clearTextStr->Length()) { 
    Local<Value> argv[argc] = {   
      Exception::Error(String::NewFromUtf8(isolate, "The 2nd argument clear text must have non-zero length")), 
      Undefined(isolate) 
    };
    cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
    return;
  }

  char* hash  = Buffer::Data(hashObj);
  size_t length = Buffer::Length(hashObj);

  if (length != SKEIN_BYTES) {
    Local<Value> argv[argc] = {   
      Exception::Error(String::NewFromUtf8(isolate, "The hash buffer argument's length must be 64 bytes")), 
      Undefined(isolate) 
    };
    cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
    return;
  }

  SKCipher cipher;
  sk_init(&cipher, (unsigned char*)hash, SKEIN_BITS);

  std::string clearText = getString(clearTextStr);
  size_t size = clearText.length() + 1;
  unsigned char* cipherText = new unsigned char[size];

  sk_encrypt(&cipher, (char*)clearText.c_str(), size, (char*)cipherText, size);

  Local<Value> argv[argc] = { 
    Undefined(isolate), 
    ArrayBuffer::New(isolate, (char*)cipherText, size) };
  delete [] cipherText;
  cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
}

void Crypto::Decrypt (const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.Length() < 3) {
    isolate->ThrowException(Exception::Error(
      String::NewFromUtf8(isolate, "There must be 3 arguments")));
    return;
  }

  if (!args[0]->IsObject() || !args[1]->IsObject() || !args[2]->IsFunction()) {
    isolate->ThrowException(Exception::Error(
      String::NewFromUtf8(isolate, "Expected a buffer, a string and a function")));
    return;
  }

  Local<Object> hashObj = args[0]->ToObject();
  Local<Object> cipherObj = args[1]->ToObject();
  char* cipherData  = Buffer::Data(cipherObj);
  size_t cipherSize = Buffer::Length(cipherObj);
  const unsigned argc = 2;
  Local<Function> cb = Local<Function>::Cast(args[2]);

  if (!cipherSize) { 
    Local<Value> argv[argc] = {   
      Exception::Error(String::NewFromUtf8(isolate, "The 2nd argument cipher text must have non-zero length")), 
      Undefined(isolate) 
    };
    cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
    return;
  }

  char* hash  = Buffer::Data(hashObj);
  size_t length = Buffer::Length(hashObj);

  if (length != SKEIN_BYTES) {
    Local<Value> argv[argc] = {   
      Exception::Error(String::NewFromUtf8(isolate, "The hash buffer argument's length must be 64 bytes")), 
      Undefined(isolate) 
    };
    cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
    return;
  }

  SKCipher cipher;
  sk_init(&cipher, (unsigned char*)hash, SKEIN_BITS);

  char* clearText = new char[cipherSize];

  sk_decrypt(&cipher, cipherData, cipherSize, clearText, cipherSize);

  Local<Value> argv[argc] = { 
    Undefined(isolate), 
    String::NewFromUtf8(isolate, clearText) };
  delete [] clearText;  
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
    
  const unsigned argc = 2;
  Local<Value> argv[argc] = { Undefined(isolate), ArrayBuffer::New(isolate, (char*)data, (size_t)SKEIN_BYTES) };
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
   Local<Value> argv[argc] = { ArrayBuffer::New(isolate, data, length) };
   cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
}

