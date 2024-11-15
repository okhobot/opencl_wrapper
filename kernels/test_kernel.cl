__kernel void test_kernel(
    __global int *data,
    int data_size
)
{
    int i = get_global_id(0); // getting the cycle index
    if(i>=data_size)return; // checking for going outside the array

    printf("%d: %d ok \n", i,data[i]);

}
