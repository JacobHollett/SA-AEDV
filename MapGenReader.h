#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>     /* for _O_TEXT and _O_BINARY */

#define TRUNCATE(name)	name[strlen(name)-1] = '\0'
#define NAMELEN	16	/* Filename length */

enum ST_DIR { East, West };
enum AVE_DIR { North, South };
enum BLDG_TYPE { CHG, STB, BOTH };
enum QUAD { NE, N, NW, E, LBL, W, SE, S, SW };

/* Building record structure */
struct bldg_data
{
int x;
int y;
enum BLDG_TYPE bt;
enum QUAD qd;
};

/* List of valid responses and codes */
struct prompt
{
char* name;
int code;
};

struct prompt bldg_t[] = { {"Charge",CHG}, {"Stable",STB}, {"Both", BOTH} };
struct prompt ew[] = { {"East", East}, {"West", West} };
struct prompt ns[] = { {"North", North}, {"South", South} };
struct prompt bldg_q[] = {
	{"NE",NE}, {"N",N}, {"NW",NW},
	{"E",E}, {"LBL",-1}, {"W",W},
	{"SE",SE}, {"S",S}, {"SW",SW} };

/* Building file descriptor */
FILE* bfd;
