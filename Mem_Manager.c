
/* memory spaces */
char* dataMarkStart = (char*) 0x00200000;
char* dataMarkEnd = (char*) 0x010FF7FE;
char* dataRealStart = (char*) 0x010FF7FF;
char* dataRealEnd = (char*) 0x01FFEFFE;


int isMemAvailable(char* baseAddress, int size)
{
	for(char* c = baseAddress; c < (baseAddress + size); c++)
	{
		if (*c == 1)
		{
			return 0; //this base address won't work out
		}
	}
	return 1; //this base address will
}

void markMemory(char* baseAddress, int size)
{
	for(char* c = baseAddress; c < baseAddress + size; c++)
	{
		*c = 1;
	}
}

// double check litman's git
char* getMem(unsigned int numBytes)
{
	int dataMapOffset = (int)dataRealStart - (int)dataMarkStart;
	for(char* c = dataMarkStart; c <= dataMarkEnd; c++)
	{
		if(*c == 0)
		{
			if (isMemAvailable(c, numBytes))
			{
				markMemory(c, numBytes);
				return c + dataMapOffset;
			}
		}
	}
	return 0x0; //nothing available found
}

void freeMem(char* baseAddress, int numBytes)
{
	for(int i = 0; i < numBytes; i++)
	{
		*(baseAddress + i) = 0;
	}
}

void unmarkAllMemory()
{
	for(char* c = dataMarkStart; c <= dataMarkEnd; c++)
	{
		*c = 0;
	}
}
