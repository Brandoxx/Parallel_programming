#include "mpi_computing.h"

/**
 * @brief This function sends the results to the main process.
 * @details This function sends the results to the main process.
 * @param count The number of elements to send.
 * @param numbers The array to send.
*/
void sendResult(int64_t count, array_number_t* numbers);

/**
 * @brief MPImapping function.
 * @details MPImapping function.
 * @param numbers The array of numbers.
 */
void dynamicMpiMapping(array_number_t* numbers);

/**
 * @brief Receives the result from the other processes
 * 
 * @param count Number of elements in the number array
 * @param countPerProcessArray Count assigned to each process
 * @param numbers Numbers array
 * @param start The start index of each process
 */
void receiveResults(int64_t count, int64_t* countPerProcessArray,
 array_number_t* numbers, int64_t* start);

/**
 * @brief Create a Datatype object for the struct number_t
 *      This is needed for MPI to be able to send and receive the struct
 * @return MPI_Datatype  The datatype for the struct number_t
 */
MPI_Datatype createDatatype();  // Create a new datatype for the struct

MPI_Datatype createDatatype() {
    MPI_Datatype datatype = MPI_DATATYPE_NULL;
    MPI_Datatype types[6] = {MPI_UINT64_T, MPI_C_BOOL, MPI_C_BOOL, MPI_C_BOOL,
     MPI_UINT64_T, MPI_INT64_T};
    int blocklen[6] = {1, 1, 1, 1, 1, 1};
    MPI_Aint offsets[6];
    offsets[0] = offsetof(number_t, number);
    offsets[1] = offsetof(number_t, is_negative);
    offsets[2] = offsetof(number_t, is_out_of_range);
    offsets[3] = offsetof(number_t, is_even);
    offsets[4] = offsetof(number_t, sumNumber);
    offsets[5] = offsetof(number_t, sums);
    MPI_Type_create_struct(6, blocklen, offsets, types, &datatype);
    MPI_Type_commit(&datatype);
    return datatype;
}

int mainProcess() {
    int error = EXIT_SUCCESS;
    array_number_t* numbers = (array_number_t*) malloc(sizeof(array_number_t));
    error = array_number_init(numbers);
    if (error == EXIT_SUCCESS) {
        error = read_numbers(numbers);
        if (error == EXIT_SUCCESS) {
            // call dynamicMapping(Numbers)
            array_prime_numbers_t* prime_numbers = (array_prime_numbers_t*)
                malloc(sizeof(array_prime_numbers_t));
            if (prime_numbers) {
                array_prime_numbers_init(prime_numbers);
                error = prime_numbers_calculation(numbers->maxNumber,
                prime_numbers);
                if (error == EXIT_SUCCESS) {
                    dynamicMpiMapping(numbers);
                    // error = print_result(numbers);
                }
            }
        }
    }
    free(numbers);
    return error;
}

void aloneProcess(uint64_t threadNumber) {
    array_number_t* numbers = (array_number_t*) malloc(sizeof(array_number_t));
    if (numbers) {
        array_number_init(numbers);
        read_numbers(numbers);
        array_prime_numbers_t* prime_numbers = (array_prime_numbers_t*)
            malloc(sizeof(array_prime_numbers_t));
        if (prime_numbers) {
            array_prime_numbers_init(prime_numbers);
            prime_numbers_calculation(numbers->maxNumber, prime_numbers);
            calculateGoldbach(numbers, prime_numbers, threadNumber);
           // print_result(numbers);
            array_prime_numbers_destroy(prime_numbers);
            free(prime_numbers);
        }
    }
    free(numbers);
}

int calcProcess(uint64_t threadNumber, int rank) {
    (void) rank;  // Avoid unused parameter warning
    int error = EXIT_SUCCESS;  // Initialize error to success
    uint64_t maxNumber = 0;  // Initialize maxNumber to 0
    MPI_Bcast(&maxNumber, 1, MPI_UINT64_T, 0, MPI_COMM_WORLD);  // Receive
    // maxNumber from main process
    array_prime_numbers_t* prime_numbers = (array_prime_numbers_t*)
        malloc(sizeof(array_prime_numbers_t));  // Allocate memory for
    // prime_numbers
    if (prime_numbers) {  // If memory was allocated
        array_prime_numbers_init(prime_numbers);  // Initialize
        error = prime_numbers_calculation(maxNumber, prime_numbers);  // Call
        // prime_numbers_calculation
        // Receive the numbers to calculate
        if (error == EXIT_SUCCESS) {
            int start = 0;  // Initialize start to 0
            MPI_Recv(&start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);  // Receive start from main process
            int64_t count = 0;  // Initialize count to 0
            MPI_Recv(&count, 1, MPI_INT64_T, 0, 0, MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);  // Receive count from main process
            array_number_t* numbers = (array_number_t*) malloc(sizeof(
                array_number_t));  // Allocate memory for numbers
            if (numbers) {  // If memory was allocated
                array_number_init(numbers);  // Initialize numbers
                for (int64_t i = 0; i  < count; i++) {  // For each number
                    // to calculate
                    number_t number = {0};  // Initialize number to 0
                    MPI_Recv(&number, 1, createDatatype(), 0, 0,
                        MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // Receive
                    // number from main process
                    array_number_append(numbers, number);  // Append
                    // number to numbers
                }
                calculateGoldbach(numbers, prime_numbers, threadNumber);
                // Call calculateGoldbach
                sendResult(count, numbers);  // Send the result to main process
            }
        }
    }
    return error;
}

void sendResult(int64_t count, array_number_t* numbers) {
    for (int64_t i = 0; i < count; i++) {  // For each number
        // calculated
        array_sums_t* sums = numbers->elements[i].sums;  // Get
        // sums
        numbers->elements[i].sums = NULL;  // Set sums to NULL
        MPI_Send(&numbers->elements[i], 1, createDatatype(), 0, 0,
            MPI_COMM_WORLD);  // Send number to main process
        if (!numbers->elements[i].is_out_of_range) {  // If number
            // is not out of range
            if (numbers->elements[i].is_negative) {  // If number.
                // is_negative
                for (int64_t j = 0; j < sums->count; j++) {  // For
                    // each sum
                    if (numbers->elements[i].is_even) {  // If
                        // number is even
                        MPI_Rsend(sums->elements[j], 2,
                            MPI_UINT64_T, 0, 0, MPI_COMM_WORLD);
                            // Send sum to main process
                    } else {  // Same as above, but for odd numbers
                        MPI_Rsend(sums->elements[j], 3,
                            MPI_UINT64_T, 0, 0, MPI_COMM_WORLD);
                    }
                }
            }
        }
    }
}

void dynamicMpiMapping(array_number_t* numbers) {
    uint64_t maxNumber = numbers->maxNumber;
    int64_t count = numbers->count;  // Number of numbers to calculate
    int size;  // Number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Get the number of processes
    MPI_Bcast(&maxNumber, 1, MPI_UINT64_T, 0, MPI_COMM_WORLD);  // Send the max
    // number to all processes
    // Send the numbers to calculate to all ranks
    int64_t countPerProcess = count / (size - 1);  // Number of numbers to
    // calculate per process
    int64_t rest = count % (size - 1);  // Rest of the division
    int64_t* start = calloc(size, sizeof(int64_t));  // Start index of the
    // numbers to calculate per process
    int64_t* countPerProcessArray = (int64_t*) malloc(sizeof(int64_t) *
        (size - 1));  // Number of numbers to calculate per process
    int64_t end = 0;  // End index of the numbers to calculate per process
    for (int i = 1; i < size; i++) {  // Send the numbers to calculate to all
        start[i-1] = end;  // ranks
        end = start[i-1] + countPerProcess;  // Calculate the end index
        countPerProcessArray[i-1] = end - start[i-1];  // Save the number of
        // numbers to calculate per process
        if (rest > 0) {  // If there is a rest, add one to the number of numbers
            // to calculate per process
            end++;  // and the end index
            countPerProcessArray[i-1]++;  // and the number of numbers to
            rest--;  // decrement the rest
        }
        MPI_Send(&start[i-1], 1, MPI_INT, i, 0, MPI_COMM_WORLD);  // Send the
        // start index
        MPI_Send(&countPerProcessArray[i-1], 1, MPI_INT64_T, i, 0,
         MPI_COMM_WORLD);  // Send the number of numbers to calculate
        for (int j = start[i-1]; j < end; j++) {  // Send the numbers to
            // calculate
            MPI_Send(&numbers->elements[j], 1, createDatatype(), i, 0,
                     MPI_COMM_WORLD);  // Send the numbers to calculate
        }
    }
    receiveResults(count, countPerProcessArray, numbers, start);
    free(start);
}

void receiveResults(int64_t count, int64_t* countPerProcessArray,
 array_number_t* numbers, int64_t* start) {
    int64_t received = 0;
    // Receive the calculated numbers
    while (received < count) {  // While not all numbers are received
        MPI_Status status;  // Status of the received message
        MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);  // Probe for
        // an incoming message from any process.
        int source = status.MPI_SOURCE;  // Get the rank of the sender
        int64_t arrayCount = countPerProcessArray[source - 1];  // Get the
        // count of the array
        for (int i = 0; i < arrayCount; i++) {  // Receive the numbers
            number_t number = {0};  // Create a new number
            MPI_Recv(&number, 1, createDatatype(), source, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);  // Receive the number
            if (number.is_negative) {  // If the number is negative
                array_sums_t* sums = malloc(sizeof(array_sums_t));  // Create
                // a new array for the sums
                array_sums_init(sums);  // Initialize the array
                for (uint64_t j = 0; j < number.sumNumber; j++) {  // Receive
                // the sums
                    if (number.is_even) {  // If the number is even
                        uint64_t* sum = malloc(sizeof(uint64_t) * 2);  // Create
                        // a new array for the sum
                        MPI_Recv(sum, 2, MPI_UINT64_T, source, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // Receive the
                        // sum
                        array_sums_append(sums, sum);  // Append the sum to
                        // the array
                    } else {  // If the number is odd
                        uint64_t* sum = malloc(sizeof(uint64_t) * 3);  // Create
                        // a new array for the sum
                        MPI_Recv(sum, 3, MPI_UINT64_T, source, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // Receive the
                        // sum
                        array_sums_append(sums, sum);  // Append the sum to
                        // the array
                    }
                }
                number.sums = sums;  // Set the sums of the number
            }
            numbers->elements[start[source - 1] + i] = number;  // Set the
            // number
            received++;  // Increase the received count
        }
    }
}
