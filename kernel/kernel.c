void main()
{
    char* video_mem = (char*) 0xB8000;
    *video_mem = 'A';
}
