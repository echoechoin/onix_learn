char message[] = "Hello, kernel!!!";

int main(int argc, char const *argv[])
{
    char *video_memory = (char *)0xb8000;

    for (int i = 0; i < sizeof(message); i++)
    {
        video_memory[i * 2] = message[i];
        video_memory[i * 2 + 1] = 0x0f;
    }
}