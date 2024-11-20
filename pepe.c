int count = 1;
int terminated = 1;

extern void printInteger(int x);

int main() {
    while (count < 10 && !terminated)
    {
        printInteger(count);
        count = count + 1;
    }
}