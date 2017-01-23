#include "../base/http_op.h"
#include "./common/check_log.hpp"
#include "./common/assert_wrapper.hpp"
#include "../base/macro.hpp"
#include <string>
#include <stdio.h>

int g_count = 0;

void IFRV_test() {
    IF_FALSE_RETURN_VOID(1 == 2);
    assert(0);
}

void IFRV_test2() {
    IF_FALSE_RETURN_VOID(true);
    g_count++;
}

int *IFRN_test() {
    IF_FALSE_RETURN_NULL(1 == 2);
    assert(0);
    return NULL; /// 消除返回值警告，由于上面有assert，所以返回啥无所谓了~
}

bool IFRF_test() {
    IF_FALSE_RETURN_FALSE(1 == 2);
    assert(0);
    return true;
}

int IFRNO_test() {
    IF_FALSE_RETURN_NEGATIVE_ONE(1 == 2);
    assert(0);
    return 0;
}

std::string IFRVAL_test() {
    IF_FALSE_RETURN_VAL(1 == 2, "false");
    assert(0);
    return std::string();
}

/// ~

void INRV_test() {
    IF_NULL_RETURN_VOID(NULL);
    assert(0);
}

int *INRN_test() {
    IF_NULL_RETURN_NULL(NULL);
    assert(0);
    return NULL;
}

bool INRF_test() {
    IF_NULL_RETURN_FALSE(NULL);
    assert(0);
    return true;
}

int INRNO_test() {
    IF_NULL_RETURN_NEGATIVE_ONE(NULL);
    assert(0);
    return 0;
}

int INRNO_test2() {
    IF_NULL_RETURN_NEGATIVE_ONE(&g_count);
    g_count++;
    return 0;
}

std::string INRVAL_test() {
    IF_NULL_RETURN_VAL(NULL, "NULL");
    assert(0);
    return std::string();
}

/// ~

void INORV_test() {
    IF_NEGATIVE_ONE_RETURN_VOID(-1);
    assert(0);
}

int *INORN_test() {
    IF_NEGATIVE_ONE_RETURN_NULL(-1);
    assert(0);
    return NULL;
}

bool INORF_test() {
    IF_NEGATIVE_ONE_RETURN_FALSE(-1);
    assert(0);
    return true;
}

int INORNO_test() {
    IF_NEGATIVE_ONE_RETURN_NEGATIVE_ONE(-1);
    assert(0);
    return 0;
}

std::string INORVAL_test() {
    IF_NEGATIVE_ONE_RETURN_VAL(-1, "a");
    assert(0);
    return "b";
}

std::string INORVAL_test2() {
    IF_NEGATIVE_ONE_RETURN_VAL(0, "a");
    g_count++;
    return "b";
}

int main() {
    ENTER_TEST;

    IFRV_test();
    assert(IFRN_test() == NULL);
    assert(IFRF_test() == false);
    assert(IFRNO_test() == -1);
    assert(IFRVAL_test() == "false")
    /// 偷懒~
    IFRV_test2();

    INRV_test();
    assert(INRN_test() == NULL);
    assert(INRF_test() == false);
    assert(INRNO_test() == -1);
    assert(INRVAL_test() == "NULL")
    assert(INRNO_test2() == 0);

    INORV_test();
    assert(INORN_test() == NULL);
    assert(INORF_test() == false);
    assert(INORNO_test() == -1);
    assert(INORVAL_test() == "a");
    assert(INORVAL_test2() == "b")

    assert(g_count == 3);

    int *p1 = static_cast<int *>(malloc(sizeof(int)));
    FREE_PTR(p1);
    assert(p1 == NULL);
    int *p2 = static_cast<int *>(malloc(sizeof(int) * 8));
    FREE_PTR(p2);
    assert(p2 == NULL);

    int *p3 = new int;
    DELETE_PTR(p3);
    assert(p3 == NULL);

    int *p4 = new int[8];
    DELETE_ARRAY_PTR(p4);
    assert(p4 == NULL);

    return 0;
}
