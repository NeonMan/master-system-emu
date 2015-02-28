#include "unity.h"
#include "unity_fixture.h"

// --------------------
// --- Declarations ---
// --------------------
//Declare each group name
TEST_GROUP(group_name);

//Declare every data structured required by the tests.
//Test declarations are usually either 'static' or 'extern'
// [EMPTY]

// ------------------------
// --- Setup & Teardown ---
// ------------------------
///This function is called **before** every test in test group.
TEST_SETUP(group_name){
}

///This function is called **after** every test in test group.
TEST_TEAR_DOWN(group_name){
}

// ------------------
// --- Test cases ---
// ------------------
///This is a test example. Will succeed.
TEST(group_name, test_name_1){
    TEST_ASSERT_EQUAL(10, 10);
}

// -------------------
// --- Test runner ---
// -------------------
TEST_GROUP_RUNNER(group_name){
    RUN_TEST_CASE(group_name, test_name_1);
}

// ----------------------
// --- Helpers & Main ---
// ----------------------
//Helper 'run all' function
static void RunAllTests(void){
    RUN_TEST_GROUP(group_name);
}

//Main
int main(int argc, const char** argv){
    return UnityMain(argc, argv, RunAllTests);
}
