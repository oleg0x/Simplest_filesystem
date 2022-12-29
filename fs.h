#define BLOCKSIZE 512
#define FSFILENAME "fs_data"

// Meta information about filesystem
struct superblock {
	int num_inodes;					// Number of inodes
	int num_blocks;					// Number of disk blocks
	int size_blocks;				// Size of disk blocks
};

struct inode {
	int size;
	int first_block;
	char name[16];
};

// File allocation table approach
struct disk_block {
	int next_block_num;
	char data[BLOCKSIZE];
};

void create_fs();					// Initialize new filesystem
void mount_fs();					// Load a filesystem
void unmount_fs();					// Detach the filesystem
void sync_fs();						// Write the filesystem

void print_fs();					// Print info about filesystem

int create_file(char name[]);		// Return file descriptor
// void delete_file(char name[]);
void set_file_size(int filenum, int size);
void write_byte(int filenum, int pos, char data);
// char read_byte(int filenum, int pos);

// Based on: youtube drdelhart
