#include <node.h>
#include <v8.h>

#include <node.h>
#include "crypto.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
   Crypto::Init(exports);
}

NODE_MODULE(skein, InitAll);