int strcmp(char * s1, char * s2)
{
	int i;
	int s1_len = strlen(s1);
	int s2_len = strlen(s2);
	if (s1_len != s2_len)
		return s1_len - s2_len;
	for (i = 0; i < s1_len; i++)
		if (s1[i] != s2[i])
			return s1[i] > s2[i] ? 1 : -1;
	return 0;
}

int strlen(char * s)
{
	int i = 0;
	while (s[i++]);
	return i;
}
