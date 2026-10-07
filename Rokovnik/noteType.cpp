#include "noteType.h"

NoteType charToType(char c) {
	return c == 'E' ? NoteType::Exam : NoteType::Note;
}
char typeToChar(NoteType t) {
	return t == NoteType::Exam ? 'E' : 'N';
}