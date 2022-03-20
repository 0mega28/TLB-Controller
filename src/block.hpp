#pragma once

#include <stdint.h>

#define BLOCK_NOT_ACCESSED __UINT64_MAX__
#define BLOCK_NOT_VALID __UINT64_MAX__

class Block
{
private:
	uint64_t page_number;
	uint64_t frame_number;

	uint64_t last_access;

	bool is_valid;

public:
	Block();
	~Block();

	void set(uint64_t page_number, uint64_t frame_number,
		 uint64_t last_access, bool is_valid);

	void set_page_number(uint64_t page_number);
	void set_frame_number(uint64_t frame_number);
	void set_last_access(uint64_t last_access);
	void set_block_validity(bool is_valid);

	uint64_t get_page_number();
	uint64_t get_frame_number();
	uint64_t get_last_access();
	bool get_block_validity();

	Block get_clone();
};

Block::Block()
{
	this->page_number = 0;
	this->frame_number = 0;
	this->last_access = 0;
	this->is_valid = 0;
}

Block::~Block() {}

void Block::set(uint64_t page_number, uint64_t frame_number,
		uint64_t last_access, bool is_valid)
{
	this->page_number = page_number;
	this->frame_number = frame_number;
	this->last_access = last_access;
	this->is_valid = is_valid;
}

void Block::set_page_number(uint64_t page_number)
{
	this->page_number = page_number;
}

void Block::set_frame_number(uint64_t frame_number)
{
	this->frame_number = frame_number;
}

void Block::set_last_access(uint64_t last_access)
{
	this->last_access = last_access;
}

void Block::set_block_validity(bool is_valid)
{
	this->is_valid = is_valid;
}

uint64_t Block::get_page_number()
{
	return this->page_number;
}

uint64_t Block::get_frame_number()
{
	return this->frame_number;
}

uint64_t Block::get_last_access()
{
	return this->last_access;
}

bool Block::get_block_validity()
{
	return this->is_valid;
}

Block Block::get_clone()
{
	Block clone_block;
	clone_block.set(this->page_number, this->frame_number,
			this->last_access, this->is_valid);
	return clone_block;
}
