#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Khai báo một biến khóa toàn cục
pthread_key_t key;

// Hàm hủy tài nguyên khi luồng kết thúc
void destructor(void *ptr) {
    free(ptr); // Giải phóng bộ nhớ
    printf("Memory freed\n");
}

// Hàm thực thi của luồng
void *thread_func(void *arg) {
    int *thread_data = malloc(sizeof(int));
    *thread_data = (int)(long)arg;

    // Gắn dữ liệu đặc biệt của luồng với khóa
    pthread_setspecific(key, thread_data);

    // Truy xuất và in dữ liệu đặc biệt
    int *retrieved_data = (int *)pthread_getspecific(key);
    printf("Thread %d: specific data = %d\n", *retrieved_data, *retrieved_data);

    return NULL;
}

int main() {
    pthread_t threads[3];

    // Tạo khóa và gán hàm hủy
    pthread_key_create(&key, destructor);

    // Tạo các luồng
    for (long i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void *)i);
    }

    // Chờ các luồng kết thúc
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    // Xóa khóa
    pthread_key_delete(key);

    return 0;
}
