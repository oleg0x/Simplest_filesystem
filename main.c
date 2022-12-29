#include "fs.h"
#include <stdio.h>



int main(void)
{
	create_fs();

	int f1 = create_file("file_1");
	set_file_size(f1, 10);
	write_byte(f1, 0, 'A');

	int f2 = create_file("file_2");
	set_file_size(f2, BLOCKSIZE);
	for ( int i = 0; i < BLOCKSIZE; ++i )
		write_byte(f2, i, 'B');

	int f3 = create_file("file_3");
	set_file_size(f3, 700);
	for ( int i = 0; i < 350; ++i )
		write_byte(f3, 2*i, 'C');

	sync_fs();
	print_fs();  puts("");

	unmount_fs();
	print_fs();  puts("");

	mount_fs();

	for ( int i = 1; i < 10; ++i )
		write_byte(f1, i, 'A');

	int f4 = create_file("file_4");
	set_file_size(f4, 100);
	for ( int i = 0; i < 20; ++i )
		write_byte(f4, 5*i, 'D');

	sync_fs();
	print_fs();

	puts("Done.");
}
