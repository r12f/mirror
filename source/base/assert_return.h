#pragma once

#define ASSERT_RETURN(condition, ret)       \
    if(!(condition)) {                      \
        ATLASSERT(FALSE);                   \
        return ret;                         \
    }

#define ASSERT_RETURN_VOID(condition)       \
    if(!(condition)) {                      \
        ATLASSERT(FALSE);                   \
        return;                             \
    }