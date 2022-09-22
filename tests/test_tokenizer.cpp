//
// Created by gnilk on 22.09.22.
//
#include <testinterface.h>
#include <vector>
#include <functional>
#include <math.h>
#include <string.h>
#include "../src/tokenizer.h"
#include <string_view>
#include <string.h>
using namespace gnilk;

// test exports
extern "C" {
    DLL_EXPORT int test_tokenizer(ITesting *t);
    DLL_EXPORT int test_tokenizer_single(ITesting *t);
    DLL_EXPORT int test_tokenizer_multi(ITesting *t);
    DLL_EXPORT int test_tokenizer_peek(ITesting *t);
}
int test_tokenizer(ITesting *t) {
    return kTR_Pass;
}

int test_tokenizer_single(ITesting *t) {
    const char *expression = "4+2";
    auto tokenizer = new Tokenizer(expression,"* / + - ( ) , < > ? :");

    static const char *expected[]={
            "4",
            "+",
            "2",
    };

    int idx = 0;
    while(tokenizer->HasMore()) {
        auto next = tokenizer->Next();
        TR_ASSERT(t, strlen(next) == 1);
        TR_ASSERT(t, !strcmp(next, expected[idx]));
        idx++;
    }

    return kTR_Pass;
}

int test_tokenizer_multi(ITesting *t) {
    const char *expression = "4<<2";
    auto tokenizer = new Tokenizer(expression,"<< >> * / + - ( ) , < > ? :");

    while(tokenizer->HasMore()) {
        auto next = tokenizer->Next();
        printf("next: %s\n", next);
    }

    return kTR_Pass;
}

int test_tokenizer_peek(ITesting *t) {
    const char *expression = "4<<2";
    auto tokenizer = new Tokenizer(expression,"<< >> * / + - ( ) , < > ? :");

    auto next = tokenizer->Next();
    TR_ASSERT(t, !strcmp(next, "4"));
    auto peek = tokenizer->Peek();
    TR_ASSERT(t, !strcmp(peek, "<<"));



    return kTR_Pass;
}

