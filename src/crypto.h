/***********************************************************************
**
** Implementation of the Skein crypto functions.
**
** Source code author: Ergun Coruh, 2015.
**
** This algorithm and source code is released to the public domain.
************************************************************************/

#ifndef CRYPTO_H
#define CRYPTO_H

#include <node.h>
#include <node_object_wrap.h>
#include <node_buffer.h>
#include "skeincrypto.h"

class Crypto : public node::ObjectWrap {
public:
   static void Init(v8::Handle<v8::Object> exports);

private: 
   explicit Crypto();
   ~Crypto();
   static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
   static v8::Persistent<v8::Function> constructor;
   static void Echo(const v8::FunctionCallbackInfo<v8::Value>& args);
   static void CalcHash(const v8::FunctionCallbackInfo<v8::Value>& args);
   static void SetHash(const v8::FunctionCallbackInfo<v8::Value>& args);
   static void GetHash(const v8::FunctionCallbackInfo<v8::Value>& args);
   static void Encrypt(const v8::FunctionCallbackInfo<v8::Value>& args);
   static void Decrypt(const v8::FunctionCallbackInfo<v8::Value>& args);

   SKCipher* ctx; 
}; 

#endif