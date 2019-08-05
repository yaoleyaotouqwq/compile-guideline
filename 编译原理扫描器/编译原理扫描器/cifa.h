#ifndef CIFA_H
#define CIFA_H

#define	 BEGIN	1
#define	 END	2
#define	 VAR	3
#define	 INTEGER	4
#define	 WHILE	5
#define	 DO	6
#define	 THEN	7
#define	 IF	8
#define	 ELSE	9
#define	 EQUAL	10
#define	ASSIGNMENT	11
#define	ADD	12
#define	MULTIPLY	13
#define	 SMALLER	14
#define	BIGER	15
#define	 SMALLER_EQUAL	16
#define	 BIGER_EQUAL	17
#define	LEFT_BRACKET	18
#define	RIGHT_BRACKET	19
#define	NOT_EQUAL	20
#define	POINT	21
#define	BEAN	22
#define	BUBBLE	23
#define	SPLIT	24
#define ID 25
#define INT 26

typedef struct {
	int num;
	std::string type;
}word_list;

int outthe_cifa_list(word_list *words);

#endif