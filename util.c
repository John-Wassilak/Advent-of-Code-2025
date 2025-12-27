#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

//////////////////////  Curl stuff ////////////////////////////////////////////

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,
                                  void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

char *pull_input(char *url) {
    CURLcode res;
    struct MemoryStruct *chunk;
    char *return_value;
    CURL *curl_handle;

    chunk = (struct MemoryStruct *)malloc(sizeof(struct MemoryStruct));
    chunk->memory = malloc(1); /* grown as needed by the realloc above */
    chunk->size = 0;           /* no data at this point */

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)chunk);
    curl_easy_setopt(curl_handle, CURLOPT_COOKIE, getenv("AOC_COOKIE"));

    printf("calling: %s\n", url);
    res = curl_easy_perform(curl_handle);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        free(chunk->memory);
        return_value = NULL;
    } else {
        return_value = chunk->memory;
    }

    free(chunk);
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();

    return return_value;
}
