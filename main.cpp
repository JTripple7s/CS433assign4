/**
 * Assignment 4: Producer Consumer Problem
 * @file main.cpp
 * @author Joel Burlingame
 * @brief The main program for the producer consumer problem.
 * @version 0.1
 */

 #include <iostream>
 #include "buffer.h"
 #include <unistd.h>
 #include <pthread.h>
 #include <cstdlib>
 #include <ctime>
 
 using namespace std;
 
 // global buffer object
 Buffer buffer;
 
 // Producer thread function
 void *producer(void *param) {
     // Each producer insert its own ID into the buffer
     buffer_item item = *((int *) param);
 
     while (true) {
         /* sleep for a random period of time */
         usleep(rand() % 1000000);
         
         if (buffer.insert_item(item)) {
             cout << "Producer " << item << ": Inserted item " << item << endl;
             buffer.print_buffer();
         } else {
             cout << "Producer error condition" << endl;
         }
     }
 }
 
 // Consumer thread function
 void *consumer(void *param) {
     buffer_item item;
 
     while (true) {
         /* sleep for a random period of time */
         usleep(rand() % 1000000);
         
         if (buffer.remove_item(&item)) {
             cout << "Consumer Removed item " << item << endl;
             buffer.print_buffer();
         } else {
             cout << "Consumer error condition" << endl;
         }
     }
 }
 
 int main(int argc, char *argv[]) {
     // Initialize random seed
     srand(time(NULL));
     
     // 1. Get command line arguments
     if (argc != 4) {
         cerr << "Usage: " << argv[0] << " <sleep_time> <num_producers> <num_consumers>" << endl;
         return 1;
     }
     
     int sleep_time = atoi(argv[1]);
     int num_producers = atoi(argv[2]);
     int num_consumers = atoi(argv[3]);
     
     cout << "Running with parameters:" << endl;
     cout << "  Sleep time: " << sleep_time << " seconds" << endl;
     cout << "  Number of producers: " << num_producers << endl;
     cout << "  Number of consumers: " << num_consumers << endl;
     
     // 2. Initialize buffer (buffer is already globally declared)
     
     // 3. Create producer threads
     pthread_t producers[num_producers];
     int producer_ids[num_producers];
     
     for (int i = 0; i < num_producers; i++) {
         producer_ids[i] = i + 1;  // Producer IDs start from 1
         if (pthread_create(&producers[i], NULL, producer, &producer_ids[i]) != 0) {
             cerr << "Error creating producer thread " << i + 1 << endl;
             return 1;
         }
     }
     
     // 4. Create consumer threads
     pthread_t consumers[num_consumers];
     
     for (int i = 0; i < num_consumers; i++) {
         if (pthread_create(&consumers[i], NULL, consumer, NULL) != 0) {
             cerr << "Error creating consumer thread " << i + 1 << endl;
             return 1;
         }
     }
     
     // 5. Main thread sleep
     cout << "Main thread sleeping for " << sleep_time << " seconds..." << endl;
     sleep(sleep_time);
     
     // 6. Exit (This is a simple exit and doesn't properly clean up threads)
     cout << "Main thread exiting." << endl;
     return 0;
 }
 