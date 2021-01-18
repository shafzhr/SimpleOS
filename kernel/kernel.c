void kernel_main()
{
    short* video_mem = (short*) 0xB8000;
    video_mem[1] = 0x0F << 8 | 'H';
}
