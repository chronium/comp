int **test;

void *malloc(int size);

int main() {
    test = malloc(64);
    return 0;
}