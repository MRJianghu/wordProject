/*
 * Copyright (c) 2023 The Regents of Northwest Minzu University.
 * All rights reserved.
 * @Author: MRJianghu
 * @Email: jianghu2021@petalmail.com
 * @Date: 2023-06-03 08:01:11
 * @LastEditors: MRJianghu
 * @LastEditTime: 2023-06-06 15:36:01
 * @Description:
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
// #include "sequence_stack.h"
#define MAX_SIZE 1024
#define WORD_SIZE 30
#define SAVE_WORDS "./.w_save.txt"
#define SAVE_LECTURE "./.lecture_save.txt"
#define DIR_WORD_ROOT "../word_dir/"
#define DIR_WORD_ROOT_LEN 12
#define BUFFER_SIZE 70

void init();
void load();
bool use_find(char *word);
void judge();
void load();
bool is_invisable(char ch);
void judge_word(char *words);
void judge_file(char *file_name);
void reset();
bool find(char word[]);
void run_judge(char *src_sentence);
FILE *file_open(const char *file_name, const char *ways);
void revocation();
void info();

/*The read word struct*/
typedef struct word_stack
{
	unsigned int w_top;
	char w_stack[WORD_SIZE];
	bool is_correct;
} word;

// struct word_stack_s
// {
// 	SequenctStack stack;
// 	bool is_correct;
// };

typedef struct stence
{
	FILE *m_words;
	FILE *m_whole_lecture;
	unsigned int error_count;
} stence;

struct buffer
{
	unsigned int length;
	struct word_stack m_buffer_[BUFFER_SIZE];
};

stence m_stence;
char *m_argv[3];
struct buffer _word_in_buffer;
struct buffer _whole_in_buffer;

/*dd
 *Certify the mode
 *-f judge a file
 *-s judge a sentence or a word
 * */
int main(int argc, char *argv[])
{
	for (int i = 0; i < argc; i++)
	{
		m_argv[i] = argv[i];
	}
	init();
	/*judge file*/
	judge();
	info();
	revocation();
	return 0;
}

/*TODO
 *this fuction shuld pass argument by adress
 **/
/*should use pointer*/
void reset_buffer(struct buffer *bf)
{
	bf->length = 0;
}

void init()
{
	m_stence.error_count = 0;
	m_stence.m_words = file_open(SAVE_WORDS, "w");

	m_stence.m_whole_lecture = file_open(SAVE_LECTURE, "w+");
	reset_buffer(&_word_in_buffer);
	reset_buffer(&_whole_in_buffer);
}

FILE *file_open(const char *file_name, const char *ways)
{
	FILE *src_w_fp = fopen(file_name, ways);
	if (src_w_fp == NULL)
	{
		printf("Process %s exit.\n Error: unable to open file %s\n", m_argv[0], file_name);
		exit(-1);
	}
	return src_w_fp;
}

void info()
{
	printf("There are %d error in your lecture.\n", m_stence.error_count);
	/**/
	// fclose(m_stence.m_whole_lecture);
	fseek(m_stence.m_whole_lecture, 0, SEEK_SET);
	// m_stence.m_whole_lecture = file_open(SAVE_LECTURE, "r");
	char ch;
	while ((ch = fgetc(m_stence.m_whole_lecture)) != EOF)
	{
		putchar(ch);
	}
	putchar('\n');
}

void delete_file(char *file_name)
{
	if (remove(file_name) == 0)
	{
		printf("File %s deleted successfully.\n", file_name);
	}
	else
	{
		printf("Error: unable to delete file %s\n", file_name);
		exit(-1);
	}
}

void revocation()
{
	fclose(m_stence.m_words);

	fclose(m_stence.m_whole_lecture);
}

bool is_invisable(char ch)
{
	if (0 <= ch && ch <= 127)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void judge_word(char *src_sentence)
{
	run_judge(src_sentence);
}

void judge_file(char *file_name)
{
	FILE *in_fp;
	in_fp = file_open(file_name, "r");
	char src_sentence[MAX_SIZE] = {0};
	while (fgets(src_sentence, MAX_SIZE, in_fp) != NULL)
	{
		run_judge(src_sentence);
		memset(src_sentence, 0, MAX_SIZE);
	}
}

void judge_t_f(struct word_stack *m_word)
{
	if (m_word->w_top <= 1)
	{
		if (is_invisable(m_word->w_stack[0]))
		{
			m_word->is_correct = true;
		}
		else
		{
			m_word->is_correct = false;
			m_stence.error_count++;
		}
	}
	else
	{
		if (!use_find(m_word->w_stack))
		{
			m_word->is_correct = false;
			m_stence.error_count++;
		}
		else
		{
			m_word->is_correct = true;
		}
	}
}

void fflush_buffer(int finish)
{
	if (_whole_in_buffer.length >= BUFFER_SIZE || finish)
	{
		for (unsigned int i = 0; i < _whole_in_buffer.length; i++)
		{
			word _word = _whole_in_buffer.m_buffer_[i];
			if (_word.is_correct == false)
			{
				fprintf(m_stence.m_whole_lecture, "\x1b[31m%s", _word.w_stack);
			}
			else
			{
				fprintf(m_stence.m_whole_lecture, "\x1b[37m%s", _word.w_stack);
			}

			/*store in m_words*/
			fprintf(m_stence.m_words, "%s\t%s\n", _word.w_stack,
					_word.is_correct == true ? "ture" : "false");
		}
		reset_buffer(&_whole_in_buffer);
	}
}

void reset_word(struct word_stack *word)
{
	word->is_correct = true;
	memset(word->w_stack, 0, WORD_SIZE);
	word->w_top = 0;
}

void run_judge(char *src_sentence)
{
	word m_word;
	reset_word(&m_word);
	char ch;
	/*collect the words into the buffer*/
	for (int i = 0;;)
	{
		ch = src_sentence[i++];
		if (isalpha(ch))
		{
			m_word.w_stack[m_word.w_top++] = ch;
		}
		else
		{
			if (m_word.w_top > 0)
			{
				fflush_buffer(0);
				judge_t_f(&m_word);
				_whole_in_buffer.m_buffer_[_whole_in_buffer.length++] = m_word;
				reset_word(&m_word);
			}
			/*the end of sentence*/
			if (ch == '\0')
			{
				fflush_buffer(1);
				break;
			}

			char ch_s[2] = {0};
			ch_s[0] = ch;
			memcpy(m_word.w_stack, ch_s, 2);
			_whole_in_buffer.m_buffer_[_whole_in_buffer.length++] = m_word;
			reset_word(&m_word);
		}
	}
}

bool use_find(char *word)
{
	if (strcmp("\n", word) == 0)
	{
		return true;
	}
	if (!find(word))
	{
		return false;
	}
	else
	{
		return true;
	}
}

char *to_low(const char *src_word)
{
	int len = strlen(src_word);
	char *n_word = (char *)malloc(len + 1);
	memset(n_word, 0, len + 1);
	for (int j = 0; src_word[j] != '\0'; j++)
	{
		n_word[j] = tolower(src_word[j]);
	}
	return n_word;
}

void m_str_cat(const char *arr_1, const char *arr_2, char *dest_arr)
{
	int index = 0;
	char ch;
	for (int i = 0; (ch = arr_1[i]) != '\0'; i++)
	{
		dest_arr[index++] = ch;
	}
	for (int i = 0; ((dest_arr[index++] = arr_2[i]) != '\0'); i++)
	{
	}
}
/*This is a static function mean that find word form dictionary is static.
 * Use a static variable count_line to signify where we find in dictionary
 * */
bool find(char src_word[])
{
	char *word = to_low(src_word);
	char dir_word[WORD_SIZE] = {0};

	char file_end_name[] = ".a.txt";
	file_end_name[1] = tolower(src_word[0]);

	int name_len = strlen(file_end_name) + DIR_WORD_ROOT_LEN + 1;
	char file_name[name_len];
	m_str_cat(DIR_WORD_ROOT, file_end_name, file_name);

	FILE *src_file = file_open(file_name, "r");

	for (; (fgets(dir_word, WORD_SIZE, src_file) != NULL);)
	{
		/*The words is too long if the last value of buffer is not '\0' */
		if (dir_word[WORD_SIZE - 1] != '\0')
		{
			printf("The word is too long\n");
			exit(-1);
		}

		// /*use '\0' to replace the '\n' */
		char *_w = strrchr(dir_word, '\n');
		if (_w != NULL)
		{
			*_w = '\0';
		}

		if (strcmp(word, dir_word) == 0)
		{
			return true;
		}
		memset(dir_word, 0, WORD_SIZE);
	}
	fclose(src_file);
	free(word);
	return false;
}

void judge()
{
	/*file*/
	if (strcmp(m_argv[1], "-f") == 0)
	{
		judge_file(m_argv[2]);
	}
	else if (strcmp(m_argv[1], "-w") == 0)
	{
		judge_word(m_argv[2]);
	}
}
