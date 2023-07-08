
enum RESULT {
    RESULT_SUCCESS,
    RESULT_FAILURE
};

struct TestUnit {
    void (*print_error)(void);
    void (*print_success)(void);
    enum RESULT (*test)(void);
};

// #define CURRENT_TIME time(NULL)
// #define RAND_N 32

// u32 random_int(u32 max);
