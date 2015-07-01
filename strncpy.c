#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* x_strncpy(char* dest, char* source, size_t count) {
	char *start = dest;
	char flag = 0;
	size_t source_len = strlen(source);
	
	// In order to avoid breaking memory bounds
	if ( count > strlen(dest) )
		count = strlen(dest);
	
	// We want to determine if it's possible to copy words or dwords instead of bytes
	if (count % 2 == 0)
		flag = 1;

	if (count % 4 == 0)
		flag = 2;

	// Copying...
	__asm 
	{
			cmp count, 0
			je done
			cld
			mov edi, dest
			mov esi, source
			
			// Choosing the size of the data block
			cmp flag, 1
			je words
			cmp flag, 2
			je dwords

	bytes:
			mov ecx, count
			rep movsb
			jmp done

	words:	
			mov ecx, count
			shr ecx, 1
			rep movsw
			jmp done

	dwords:	
			mov ecx, count
			shr ecx, 2
			rep movsd
	done:
	}

	// Padding out with zero's 
	if (count > source_len)		
	{
		count -= source_len;
		dest += source_len;
		__asm 
		{
				cmp count, 0
				je padding_done
			
				mov edi, dest
				mov eax, '\0'	
				cld 

				// Choosing the size of the data block
				cmp flag, 1
				je p_words
				cmp flag, 2
				je p_dwords

		p_bytes:
				mov ecx, count
				rep stosb
				jmp padding_done

		p_words:	
				mov ecx, count
				shr ecx, 1
				rep stosb
				jmp padding_done

		p_dwords:	
				mov ecx, count
				shr ecx, 2
				rep stosb
		padding_done:
		}
	}

	return (start);
}

int main() {
	char Destination[64] = "QWERTYUIOP asdfghjkl ZXCVBNM";
//	char Source[64] = "123456789012345678901234567890";
	char Source[7] = "012345";
	int num = 13;

	puts(Destination);
	x_strncpy(Destination, Source, num);
	puts(Destination);

	return 0;
}
