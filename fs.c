#include "fs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



static struct superblock sb;
static struct inode *inodes;
static struct disk_block *dbs;



void create_fs()
{
	sb.num_inodes = 10;
	sb.num_blocks = 100;
	sb.size_blocks = sizeof(struct disk_block);

	inodes = malloc(sizeof(struct inode) * sb.num_inodes);
	for ( int i = 0; i < sb.num_inodes; ++i ) {
		inodes[i].size = -1;
		inodes[i].first_block = -1;
		strcpy(inodes[i].name, "empty");
	}

	dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);
	for ( int i = 0; i < sb.num_blocks; ++i ) {
		dbs[i].next_block_num = -1;
	}
}



void mount_fs()
{
	FILE *file = fopen(FSFILENAME, "r");

	fread(&sb, sizeof(struct superblock), 1 , file);				// Superblock

	inodes = malloc(sizeof(struct inode) * sb.num_inodes);
	fread(inodes, sizeof(struct inode), sb.num_inodes, file);		// Inodes

	dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);
	fread(dbs, sizeof(struct disk_block), sb.num_blocks, file);		// Disk blocks

	fclose(file);
}



void unmount_fs()
{
	sb.num_inodes = sb.num_blocks = sb.size_blocks = 0;
	free(inodes);
	free(dbs);
}



void sync_fs()
{
	FILE *file = fopen(FSFILENAME, "w");

	fwrite(&sb, sizeof(struct superblock), 1 , file);				// Superblock
	fwrite(inodes, sizeof(struct inode), sb.num_inodes, file);		// Inodes
	fwrite(dbs, sizeof(struct disk_block), sb.num_blocks, file);	// Disk blocks

	fclose(file);
}



void print_fs()
{
	puts("Superblock info");
	printf("\tNumber of inodes: %d\n", sb.num_inodes);
	printf("\tNumber of blocks: %d\n", sb.num_blocks);
	printf("\tSize of blocks: %d\n", sb.size_blocks);

	puts("Inodes");
	for ( int i = 0; i < sb.num_inodes; ++i )
		printf("\tName: %s,  Size: %d,  First block: %d\n",
			   inodes[i].name, inodes[i].size, inodes[i].first_block);

	puts("Blocks");
	for ( int i = 0; i < sb.num_blocks; ++i )
		printf("\tBlock number: %d,  Next block: %d\n",
			   i, dbs[i].next_block_num);
}



static int find_empty_inode()
{
	for ( int i = 0; i < sb.num_inodes; ++i )
		if ( inodes[i].first_block == -1 )  return i;
	return -1;
}



static int find_empty_block()
{
	for ( int i = 0; i < sb.num_blocks; ++i )
		if ( dbs[i].next_block_num == -1 )  return i;
	return -1;
}



int create_file(char name[])
{
	int in = find_empty_inode();
	int block = find_empty_block();

	inodes[in].size = 0;
	inodes[in].first_block = block;
	strcpy(inodes[in].name, name);
	dbs[block].next_block_num = -2;

	return in;
}



static void shorten_file(int bn)
{
	int nbn = dbs[bn].next_block_num;
	if ( nbn >= 0 )  shorten_file(nbn);
	dbs[bn].next_block_num = -1;
}



void set_file_size(int filenum, int size)
{
	int num = (size + BLOCKSIZE - 1) / BLOCKSIZE;
	int bn = inodes[filenum].first_block;

	inodes[filenum].size = size;

	// Enlarge the file if necessary
	while ( --num > 0 ) {
		int next_num = dbs[bn].next_block_num;
		if ( next_num == -2 ) {
			int empty = find_empty_block();
			dbs[bn].next_block_num = empty;
			dbs[empty].next_block_num = -2;
		}
		bn = dbs[bn].next_block_num;
	}

	// Shorten the file if necessary
	shorten_file(bn);
	dbs[bn].next_block_num = -2;
}



static int get_block_num(int file, int offset)
{
	int bn = inodes[file].first_block;
	while ( offset-- > 0 )
		bn = dbs[bn].next_block_num;
	return bn;
}



void write_byte(int filenum, int pos, char data)
{
	int relative_block = pos / BLOCKSIZE;
	int bn = get_block_num(filenum, relative_block);
	int offset = pos % BLOCKSIZE;
	dbs[bn].data[offset] = data;
}
