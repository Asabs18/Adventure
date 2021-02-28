#pragma once

//all includes & defines for colors
#define ESC "\x1B"
#define ANSI_CODE_RED			"\x1b[31m"
#define ANSI_CODE_BLACK			ESC"[30m"
#define ANSI_CODE_BWHITE		ESC"[47m"
#define ANSI_CODE_RESET         ESC"[00m"
#define ANSI_CODE_BOLD          ESC"[1m"
#define ANSI_CODE_BGREEN		ESC"[102m"
#define ANSI_CODE_GREEN			ESC"[92m"
#define ANSI_CODE_BBLACK		ESC"[40m"
#define ANSI_CODE_WHITE		ESC"[37m"

#define GREEN(string)			ANSI_CODE_GREEN string
#define BGREEN(string)			ANSI_CODE_BGREEN string
#define BLACK(string)            ANSI_CODE_BLACK   string
#define BWHITE(string)			ANSI_CODE_BWHITE string
#define RED(string)				ANSI_CODE_RED	ANSI_CODE_BOLD string
#define BBLACK(string)			ANSI_CODE_BBLACK	string
#define WHITE(string)			ANSI_CODE_WHITE	string
#define RESET(string)			ANSI_CODE_RESET string