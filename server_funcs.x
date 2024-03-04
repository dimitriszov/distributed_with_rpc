struct mult_array {
    double num;
    int arr<>;
};

struct int_array {
    int arr<>;
};

struct double_array {
    double arr<>;
};

struct intpair {
    int num1;
    int num2;
};

program SERVER_PROG {
    version AVG_VERS {
        double AVG(int_array) = 1;
        intpair Max_min(int_array) = 2;
        double_array MULT(mult_array) = 3;
    } = 1;
} = 0x23451111;
