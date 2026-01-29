/*
 * read_binary_file_dynamic.c
 * 
 * Copyright (c) 2026 Miroslaw Staron
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Demonstrates reading and writing binary data with dynamically allocated strings.
 * Uses a two-phase approach: write/read metadata (length, age), then allocate and
 * read/write the actual string data.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Structure to hold person data with dynamically allocated name
typedef struct {
    uint32_t name_len;  // Length of the name string (for binary I/O)
    int32_t  age;       // Age of the person
    char    *name;      // Dynamically allocated name string
} Person;

/*
 * write_person - Write a Person structure to a binary file
 * @f: file pointer (opened in binary write mode)
 * @p: pointer to Person structure to write
 * 
 * Returns: 1 on success, 0 on failure
 * 
 * Format: writes header (name_len, age) followed by name bytes
 * This allows the reader to know how many bytes to allocate
 */
int write_person(FILE *f, const Person *p)
{
    if (!f || !p || !p->name) return 0;

    // 1) Write metadata (header): name length and age
    // Note: We don't write the pointer itself, only the actual data
    if (fwrite(&p->name_len, sizeof(p->name_len), 1, f) != 1) return 0;
    if (fwrite(&p->age,      sizeof(p->age),      1, f) != 1) return 0;

    // 2) Write actual name string data (payload)
    if (p->name_len > 0) {
        if (fwrite(p->name, 1, p->name_len, f) != p->name_len) return 0;
    }

    return 1;
}

/*
 * read_person - Read a Person structure from a binary file
 * @f: file pointer (opened in binary read mode)
 * @out: pointer to Person structure to fill
 * 
 * Returns: 1 on success, 0 on failure or EOF
 * 
 * Process: reads header → allocates memory → reads string data
 */
int read_person(FILE *f, Person *out)
{
    if (!f || !out) return 0;

    // 1) Read metadata (header): name length and age
    if (fread(&out->name_len, sizeof(out->name_len), 1, f) != 1) return 0;
    if (fread(&out->age,      sizeof(out->age),      1, f) != 1) return 0;

    // Sanity check: prevent allocation of unreasonably large strings
    if (out->name_len > 1024 * 1024) return 0;

    // 2) Allocate memory for the string (including null terminator)
    out->name = malloc(out->name_len + 1);
    if (!out->name) return 0;

    // 3) Read the actual string data (payload)
    if (out->name_len > 0) {
        if (fread(out->name, 1, out->name_len, f) != out->name_len) {
            free(out->name);
            return 0;
        }
    }

    // Null-terminate the string
    out->name[out->name_len] = '\0';
    return 1;
}

// Free dynamically allocated memory from a Person structure
void free_person(Person *p)
{
    if (p && p->name) {
        free(p->name);
        p->name = NULL;  // Clear pointer to avoid use-after-free bugs
    }
}

int main(void)
{
    const char *filename = "people.bin";

    /* ---- WRITE SECTION: Create binary file with person data ---- */
    {
        FILE *f = fopen(filename, "wb");
        if (!f) {
            perror("fopen");
            return 1;
        }

        // Create first person
        Person p1 = {
            .name = "John",
            .name_len = 4,
            .age = 25
        };

        // Create second person
        Person p2 = {
            .name = "Miroslaw",
            .name_len = 8,
            .age = 40
        };

        // Write both persons to file
        write_person(f, &p1);
        write_person(f, &p2);

        fclose(f);
        printf("Wrote people to file.\n");
    }

    /* ---- READ SECTION: Read and display binary file data ---- */
    {
        FILE *f = fopen(filename, "rb");
        if (!f) {
            perror("fopen");
            return 1;
        }

        // Read persons one by one until EOF
        while (1) {
            Person p = {0};  // Initialize to zero

            // Attempt to read next person; break if EOF or error
            if (!read_person(f, &p))
                break;

            // Display the person's information
            printf("Read person: name=\"%s\" (len=%u), age=%d\n",
                   p.name, p.name_len, p.age);

            // Clean up allocated memory
            free_person(&p);
        }

        fclose(f);
    }

    return 0;
}
