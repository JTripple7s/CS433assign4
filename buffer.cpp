/**
 * Assignment 4: Producer Consumer Problem
 * @file buffer.cpp
 * @author Joel Burlingame
 * @brief Implementation file for the buffer class
 * @version 0.1
 */

 #include "buffer.h"
 #include <iostream>
 
 /**
  * @brief Construct a new Buffer object with the given size
  * @param size The maximum capacity of the buffer
  */
 Buffer::Buffer(int size) {
     this->size = size;
     pthread_mutex_init(&mutex, NULL);
     pthread_cond_init(&full, NULL);
     pthread_cond_init(&empty, NULL);
 }
 
 /**
  * @brief Destroy the Buffer object and clean up resources
  */
 Buffer::~Buffer() {
     pthread_mutex_destroy(&mutex);
     pthread_cond_destroy(&full);
     pthread_cond_destroy(&empty);
 }
 
 /**
  * @brief Insert an item into the buffer
  * @param item The item to insert
  * @return true if successful, false otherwise
  */
 bool Buffer::insert_item(buffer_item item) {
     pthread_mutex_lock(&mutex);
     
     // Wait while the buffer is full
     while (is_full()) {
         pthread_cond_wait(&full, &mutex);
     }
     
     // Insert the item
     buf.push(item);
     
     // Signal that the buffer is not empty
     pthread_cond_signal(&empty);
     pthread_mutex_unlock(&mutex);
     
     return true;
 }
 
 /**
  * @brief Remove an item from the buffer
  * @param item Pointer to store the removed item
  * @return true if successful, false otherwise
  */
 bool Buffer::remove_item(buffer_item *item) {
     pthread_mutex_lock(&mutex);
     
     // Wait while the buffer is empty
     while (is_empty()) {
         pthread_cond_wait(&empty, &mutex);
     }
     
     // Remove the item
     *item = buf.front();
     buf.pop();
     
     // Signal that the buffer is not full
     pthread_cond_signal(&full);
     pthread_mutex_unlock(&mutex);
     
     return true;
 }
 
 /**
  * @brief Get the maximum capacity of the buffer
  * @return The size of the buffer
  */
 int Buffer::get_size() {
     return size;
 }
 
 /**
  * @brief Get the current number of items in the buffer
  * @return The count of items
  */
 int Buffer::get_count() {
     return buf.size();
 }
 
 /**
  * @brief Check if the buffer is empty
  * @return true if empty, false otherwise
  */
 bool Buffer::is_empty() {
     return buf.empty();
 }
 
 /**
  * @brief Check if the buffer is full
  * @return true if full, false otherwise
  */
 bool Buffer::is_full() {
     return buf.size() >= size;
 }
 
 /**
  * @brief Print the current contents of the buffer
  */
 void Buffer::print_buffer() {
     std::cout << "Buffer: [";
     
     // Create a temporary copy of the queue
     std::queue<buffer_item> temp = buf;
     
     if (!temp.empty()) {
         std::cout << temp.front();
         temp.pop();
         
         while (!temp.empty()) {
             std::cout << ", " << temp.front();
             temp.pop();
         }
     }
     
     std::cout << "]" << std::endl;
 }
 