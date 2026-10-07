#pragma once
enum class NoteType {
	Note, Exam
};
NoteType charToType(char c);
char typeToChar(NoteType t);