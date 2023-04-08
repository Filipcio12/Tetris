#include "primlib.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGHT gfx_screenHeight()
#define BOARD_WIDTH 10
#define BOARD_HEIGTH 20
#define SQUARE_SIZE (600 / BOARD_HEIGTH)

#define LEFT_X (SCREEN_WIDTH / 2 - SQUARE_SIZE * BOARD_WIDTH / 2)
#define RIGHT_X (SCREEN_WIDTH / 2 + SQUARE_SIZE * BOARD_WIDTH / 2)
#define TOP_Y (SCREEN_HEIGHT - SQUARE_SIZE * BOARD_HEIGTH)
#define BOTTOM_Y (SCREEN_HEIGHT)

#define TEXT_X_POSITION (SCREEN_WIDTH / 2 - 100)
#define TEXT_Y_POSITION (SCREEN_HEIGHT / 2)
#define TEXT_Y_SHIFT 50

int pieces[7 /*kind */][4 /* rotation */][4][4] = {
	/* square */
	{{{2, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{2, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{2, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{2, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
	/* I */

	{{{1, 2, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{1, 0, 0, 0}, {2, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}},
	 {{1, 1, 2, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{1, 0, 0, 0}, {1, 0, 0, 0}, {2, 0, 0, 0}, {1, 0, 0, 0}}},
	/* L */
	{{{1, 0, 0, 0}, {2, 0, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
	 {{1, 2, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{1, 1, 0, 0}, {0, 2, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
	 {{0, 0, 1, 0}, {1, 2, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
	/* L mirrored */
	{{{0, 1, 0, 0}, {0, 2, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
	 {{1, 0, 0, 0}, {1, 2, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{1, 1, 0, 0}, {2, 0, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
	 {{1, 2, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
	/* N */
	{{{0, 1, 0, 0}, {2, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
	 {{1, 2, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{0, 1, 0, 0}, {1, 2, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
	 {{1, 1, 0, 0}, {0, 2, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
	/* N mirrored */
	{{{1, 0, 0, 0}, {2, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
	 {{0, 2, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{1, 0, 0, 0}, {1, 2, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
	 {{0, 1, 1, 0}, {1, 2, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
	/* T */
	{{{1, 0, 0, 0}, {2, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
	 {{1, 2, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{0, 1, 0, 0}, {1, 2, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
	 {{0, 1, 0, 0}, {1, 2, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}};

typedef struct {
	int x1;
	int y1;
	int x2;
	int y2;
	enum color c;
} Square;

typedef struct {
	int xCenter;
	int yCenter;
	int xLeft;
	int yTop;
	int kind;
	int rotation;
	enum color c;
	Square arr[4][4];
} Piece;

void initMatrix(Square matrix[][BOARD_HEIGTH]);
void transformCoordstoIndices(int x, int y, int* i, int* j);
void drawSquare(Square square, enum color c);
void drawSquare(Square square, enum color c);
void drawMatrix(Square matrix[BOARD_WIDTH][BOARD_HEIGTH]);
void findCenter(int arr[][4], int* iCenter, int* jCenter);
void createPieceArray(Piece* piece);
void initPiece(Piece* piece);
void placePiece(Piece* piece, int xCenter, int yCenter);
void drawPiece(Piece* piece);
void drawBoard();
void addToMatrix(Piece* piece, Square matrix[][BOARD_HEIGTH]);
void moveDown(Piece* piece, Piece* next, Square matrix[][BOARD_HEIGTH]);
void moveRight(Piece* piece, Square matrix[][BOARD_HEIGTH]);
void moveLeft(Piece* piece, Square matrix[][BOARD_HEIGTH]);
void rotate(Piece* piece, Square matrix[][BOARD_HEIGTH]);
void drop(Piece* piece, Piece* next, Square matrix[][BOARD_HEIGTH]);
void takeUserInput(Piece* piece, Piece* next, Square matrix[][BOARD_HEIGTH],
				   int* isSpacebarOn, int* isDownOn);
void clearRow(Square matrix[][BOARD_HEIGTH], int y);
void checkRows(Square matrix[][BOARD_HEIGTH]);
int makeEndScreen(Square matrix[][BOARD_HEIGTH]);

int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}

	srand(time(0));
	Square matrix[BOARD_WIDTH][BOARD_HEIGTH];
	initMatrix(matrix);
	Piece fallingPiece, nextPiece;
	initPiece(&fallingPiece);
	placePiece(&fallingPiece, matrix[BOARD_WIDTH / 2][BOARD_HEIGTH - 1].x1,
			   matrix[BOARD_WIDTH / 2][BOARD_HEIGTH - 1].y1);
	initPiece(&nextPiece);
	placePiece(&nextPiece, RIGHT_X + 100, SCREEN_HEIGHT / 2);

	int isSpacebarOn = 0;
	int isDownOn = 0;
	int frameCounter = 0;

	do {
		drawBoard();
		drawMatrix(matrix);
		drawPiece(&fallingPiece);
		drawPiece(&nextPiece);
		takeUserInput(&fallingPiece, &nextPiece, matrix, &isSpacebarOn,
					  &isDownOn);
		isSpacebarOn = gfx_isKeyDown(SDLK_SPACE);
		isDownOn = gfx_isKeyDown(SDLK_DOWN);
		if (frameCounter == 8) {
			moveDown(&fallingPiece, &nextPiece, matrix);
			frameCounter = 0;
		}
		checkRows(matrix);
		gfx_updateScreen();
		frameCounter++;
		SDL_Delay(40);
	} while (gfx_pollkey() != SDLK_ESCAPE && makeEndScreen(matrix));

	return 0;
}

void initMatrix(Square matrix[][BOARD_HEIGTH])
{
	int i;
	for (int j = 0; j < BOARD_HEIGTH; j++)
		for (i = 0; i < BOARD_WIDTH; i++) {
			matrix[i][j].c = BLACK;
			matrix[i][j].x1 = LEFT_X + i * SQUARE_SIZE;
			matrix[i][j].y1 = BOTTOM_Y - SQUARE_SIZE - j * SQUARE_SIZE;
			matrix[i][j].x2 = matrix[i][j].x1 + SQUARE_SIZE;
			matrix[i][j].y2 = matrix[i][j].y1 + SQUARE_SIZE;
		}
}

void transformCoordstoIndices(int x, int y, int* i, int* j)
{
	*i = (x - LEFT_X) / SQUARE_SIZE;
	*j = (BOTTOM_Y - y) / SQUARE_SIZE - 1;
}

void drawSquare(Square square, enum color c)
{
	gfx_filledRect(square.x1 + 1, square.y1 + 1, square.x2 - 1, square.y2 - 1,
				   c);
}

void drawMatrix(Square matrix[BOARD_WIDTH][BOARD_HEIGTH])
{
	int i;
	for (int j = 0; j < BOARD_HEIGTH; j++)
		for (i = 0; i < BOARD_WIDTH; i++)
			drawSquare(matrix[i][j], matrix[i][j].c);
}

void findCenter(int arr[][4], int* iCenter, int* jCenter)
{
	int i;
	for (int j = 0; j < 4; j++)
		for (i = 0; i < 4; i++)
			if (arr[i][j] == 2) {
				*iCenter = i;
				*jCenter = j;
				return;
			}
}

void createPieceArray(Piece* piece)
{
	int i;
	for (int j = 0; j < 4; j++)
		for (i = 0; i < 4; i++) {
			piece->arr[i][j].x1 = piece->xLeft + i * SQUARE_SIZE;
			piece->arr[i][j].y1 = piece->yTop + j * SQUARE_SIZE;
			piece->arr[i][j].x2 = piece->xLeft + (i + 1) * SQUARE_SIZE;
			piece->arr[i][j].y2 = piece->yTop + (j + 1) * SQUARE_SIZE;
			piece->arr[i][j].c = 0;
			if (pieces[piece->kind][piece->rotation][i][j] == 1) {
				piece->arr[i][j].c = piece->c;
			}
			else if (pieces[piece->kind][piece->rotation][i][j] == 2) {
				piece->arr[i][j].c = (piece->c + 3) % 5 + 1;
			}
		}
}

void initPiece(Piece* piece)
{
	piece->kind = rand() % 7;
	piece->rotation = rand() % 4;
	piece->c = rand() % 5 + 1;
}

void placePiece(Piece* piece, int xCenter, int yCenter)
{
	piece->xCenter = xCenter;
	piece->yCenter = yCenter;
	int iCenter, jCenter;
	findCenter(pieces[piece->kind][piece->rotation], &iCenter, &jCenter);
	piece->xLeft = piece->xCenter - iCenter * SQUARE_SIZE;
	piece->yTop = piece->yCenter - jCenter * SQUARE_SIZE;
	createPieceArray(piece);
}

void drawPiece(Piece* piece)
{
	int i;
	for (int j = 0; j < 4; j++)
		for (i = 0; i < 4; i++)
			if (piece->arr[i][j].c != 0)
				drawSquare(piece->arr[i][j], piece->arr[i][j].c);
}

void drawBoard()
{
	gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BLACK);
	gfx_line(LEFT_X - 1, TOP_Y, LEFT_X - 1, BOTTOM_Y, YELLOW);
	gfx_line(RIGHT_X + 1, TOP_Y, RIGHT_X + 1, BOTTOM_Y, YELLOW);
}

void addToMatrix(Piece* piece, Square matrix[][BOARD_HEIGTH])
{
	int i;
	for (int j = 0; j < 4; j++)
		for (i = 0; i < 4; i++)
			if (piece->arr[i][j].c != 0) {
				int iSquare;
				int jSquare;
				transformCoordstoIndices(piece->arr[i][j].x1,
										 piece->arr[i][j].y1, &iSquare,
										 &jSquare);
				if (iSquare > -1 && iSquare < BOARD_WIDTH && jSquare > -1 &&
					jSquare < BOARD_HEIGTH)
					matrix[iSquare][jSquare].c = piece->arr[i][j].c;
			}
}

void moveDown(Piece* piece, Piece* next, Square matrix[][BOARD_HEIGTH])
{
	int i;
	for (int j = 0; j < 4; j++)
		for (i = 0; i < 4; i++)
			if (piece->arr[i][j].c != 0) {
				int iSquare;
				int jSquare;
				transformCoordstoIndices(piece->arr[i][j].x1,
										 piece->arr[i][j].y1, &iSquare,
										 &jSquare);
				if (jSquare == 0 || (jSquare < BOARD_HEIGTH &&
									 matrix[iSquare][jSquare - 1].c != 0)) {
					addToMatrix(piece, matrix);
					drawMatrix(matrix);
					*piece = *next;
					placePiece(piece,
							   matrix[BOARD_WIDTH / 2][BOARD_HEIGTH - 1].x1,
							   matrix[BOARD_WIDTH / 2][BOARD_HEIGTH - 1].y1);
					initPiece(next);
					createPieceArray(next);
					return;
				}
			}

	for (int j = 0; j < 4; j++)
		for (i = 0; i < 4; i++) {
			piece->arr[i][j].y1 += SQUARE_SIZE;
			piece->arr[i][j].y2 += SQUARE_SIZE;
		}
	piece->yCenter += SQUARE_SIZE;
}

void moveRight(Piece* piece, Square matrix[][BOARD_HEIGTH])
{
	int i;
	for (int j = 0; j < 4; j++)
		for (i = 0; i < 4; i++)
			if (piece->arr[i][j].c != 0) {
				int iSquare;
				int jSquare;
				transformCoordstoIndices(piece->arr[i][j].x1,
										 piece->arr[i][j].y1, &iSquare,
										 &jSquare);
				if ((iSquare > -1 && iSquare < BOARD_WIDTH && jSquare > -1 &&
					 jSquare < BOARD_HEIGTH) &&
					(iSquare == BOARD_WIDTH - 1 ||
					 matrix[iSquare + 1][jSquare].c != 0))
					return;
			}

	for (int j = 0; j < 4; j++)
		for (i = 0; i < 4; i++) {
			piece->arr[i][j].x1 += SQUARE_SIZE;
			piece->arr[i][j].x2 += SQUARE_SIZE;
		}
	piece->xCenter += SQUARE_SIZE;
}

void moveLeft(Piece* piece, Square matrix[][BOARD_HEIGTH])
{
	int i;
	for (int j = 0; j < 4; j++)
		for (i = 0; i < 4; i++)
			if (piece->arr[i][j].c != 0) {
				int iSquare;
				int jSquare;
				transformCoordstoIndices(piece->arr[i][j].x1,
										 piece->arr[i][j].y1, &iSquare,
										 &jSquare);
				if ((iSquare > -1 && iSquare < BOARD_WIDTH && jSquare > -1 &&
					 jSquare < BOARD_HEIGTH) &&
					(iSquare == 0 || matrix[iSquare - 1][jSquare].c != 0))
					return;
			}

	for (int j = 0; j < 4; j++)
		for (i = 0; i < 4; i++) {
			piece->arr[i][j].x1 -= SQUARE_SIZE;
			piece->arr[i][j].x2 -= SQUARE_SIZE;
		}
	piece->xCenter -= SQUARE_SIZE;
}

void rotate(Piece* piece, Square matrix[][BOARD_HEIGTH])
{
	piece->rotation = (piece->rotation + 1) % 4;
	int iCenter, jCenter;
	findCenter(pieces[piece->kind][piece->rotation], &iCenter, &jCenter);
	piece->xLeft = piece->xCenter - iCenter * SQUARE_SIZE;
	piece->yTop = piece->yCenter - jCenter * SQUARE_SIZE;
	createPieceArray(piece);

	int i;
	for (int j = 0; j < 4; j++)
		for (i = 0; i < 4; i++)
			if (piece->arr[i][j].c != 0) {

				if (piece->arr[i][j].x1 < LEFT_X ||
					piece->arr[i][j].x2 > RIGHT_X ||
					piece->arr[i][j].y1 < TOP_Y ||
					piece->arr[i][j].y2 > BOTTOM_Y)
					goto go_back;

				int iSquare, jSquare;
				transformCoordstoIndices(piece->arr[i][j].x1,
										 piece->arr[i][j].y1, &iSquare,
										 &jSquare);

				if (matrix[iSquare][jSquare].c != 0)
					goto go_back;
			}
	return;

go_back:
	piece->rotation = (piece->rotation + 3) % 4;
	findCenter(pieces[piece->kind][piece->rotation], &iCenter, &jCenter);
	piece->xLeft = piece->xCenter - iCenter * SQUARE_SIZE;
	piece->yTop = piece->yCenter - jCenter * SQUARE_SIZE;
	createPieceArray(piece);
}

void drop(Piece* piece, Piece* next, Square matrix[][BOARD_HEIGTH])
{
	while (1) {
		int i;
		for (int j = 0; j < 4; j++)
			for (i = 0; i < 4; i++)
				if (piece->arr[i][j].c != 0) {
					int iSquare;
					int jSquare;
					transformCoordstoIndices(piece->arr[i][j].x1,
											 piece->arr[i][j].y1, &iSquare,
											 &jSquare);
					if (jSquare == 0 || (jSquare < BOARD_HEIGTH &&
										 matrix[iSquare][jSquare - 1].c != 0)) {
						addToMatrix(piece, matrix);
						drawMatrix(matrix);
						*piece = *next;
						placePiece(
							piece, matrix[BOARD_WIDTH / 2][BOARD_HEIGTH - 1].x1,
							matrix[BOARD_WIDTH / 2][BOARD_HEIGTH - 1].y1);
						initPiece(next);
						createPieceArray(next);
						return;
					}
				}

		for (int j = 0; j < 4; j++)
			for (i = 0; i < 4; i++) {
				piece->arr[i][j].y1 += SQUARE_SIZE;
				piece->arr[i][j].y2 += SQUARE_SIZE;
			}
		piece->yCenter += SQUARE_SIZE;
	}
}

void takeUserInput(Piece* piece, Piece* next, Square matrix[][BOARD_HEIGTH],
				   int* isSpacebarOn, int* isDownOn)
{
	if (gfx_isKeyDown(SDLK_RIGHT)) {
		moveRight(piece, matrix);
	}
	if (gfx_isKeyDown(SDLK_LEFT)) {
		moveLeft(piece, matrix);
	}
	if (gfx_isKeyDown(SDLK_DOWN) && !*isDownOn) {
		drop(piece, next, matrix);
		*isDownOn = 1;
	}
	if (gfx_isKeyDown(SDLK_SPACE) && !*isSpacebarOn) {
		rotate(piece, matrix);
		*isSpacebarOn = 1;
	}
}

void clearRow(Square matrix[][BOARD_HEIGTH], int y)
{
	for (int i = 0; i < BOARD_WIDTH; i++)
		matrix[i][y].c = 0;

	int i = 0;
	for (int j = y + 1; j < BOARD_HEIGTH; j++)
		for (i = 0; i < BOARD_WIDTH; i++) {
			enum color temp;
			temp = matrix[i][j].c;
			matrix[i][j].c = matrix[i][j - 1].c;
			matrix[i][j - 1].c = temp;
		}
}

void checkRows(Square matrix[][BOARD_HEIGTH])
{
	int i;
	for (int j = 0; j < BOARD_HEIGTH; j++) {
		int isRowFull = 1;
		for (i = 0; i < BOARD_WIDTH; i++)
			if (matrix[i][j].c == 0) {
				isRowFull = 0;
				break;
			}
		if (isRowFull) {
			clearRow(matrix, j);
			j--;
		}
	}
}

int makeEndScreen(Square matrix[][BOARD_HEIGTH])
{
	int isGameOver = 0;
	for (int i = 0; i < BOARD_WIDTH; i++)
		if (matrix[i][BOARD_HEIGTH - 1].c != 0) {
			isGameOver = 1;
			break;
		}

	if (!isGameOver)
		return 1;

	gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BLACK);
	gfx_textout(TEXT_X_POSITION, TEXT_Y_POSITION - TEXT_Y_SHIFT, "Game Over",
				WHITE);
	gfx_textout(TEXT_X_POSITION, TEXT_Y_POSITION, "Press ENTER to REPLAY",
				WHITE);
	gfx_textout(TEXT_X_POSITION, TEXT_Y_POSITION + TEXT_Y_SHIFT,
				"Press ESC to EXIT", WHITE);
	gfx_updateScreen();

	while (1) {
		int input = gfx_getkey();
		if (input == SDLK_ESCAPE)
			return 0;
		if (input == SDLK_RETURN) {
			initMatrix(matrix);
			return 1;
		}
	}
}