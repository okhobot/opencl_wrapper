__kernel void test_kernel(
    __global int *data,
    long data_size
)
{
    long i = get_global_id(0); // getting the cycle index
    if(i>=data_size)return; // checking for going outside the array

    printf("%lld: %d ok \n", i,data[i]);

}
