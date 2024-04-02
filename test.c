# include <fcntl.h>
# include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len;
	size_t	i;
	size_t	j;
	char	*str;

	len = strlen((char *)s1) + strlen((char *)s2);
	i = 0;
	str = malloc (sizeof(char) * (len + 1));
	if (str == NULL)
		return (str);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		str[i + j] = s2[j];
		j++;
	}
	str [i + j] = 0;
	return (str);
}

int main(int arc, char **argv)
{

	char	*temp;
	temp = ft_strjoin(argv[1], "oh");
	printf("dest = %s\n", temp);
	free(argv[1]);
	argv[1] = temp;
	return (0);
}