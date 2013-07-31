int CheckCRC(unsigned char* buf)
{
	int t=0;
	for(int i=0;i<16;i++)
		t|=(buf[i]<<i);
	return t;
}
