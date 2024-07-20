#include<iostream>
#include<fstream>
#include<conio.h>
#include<iomanip>
#include<windows.h>
using namespace std;
const int captured_pieces=20;
enum COLOR { WHITE, BLACK };
struct position
{
	int ri, ci;
};
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void SetClr(int clr)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
}
bool ishorizmove(position Sc, position Dc)
{
	return Sc.ri == Dc.ri;
}
bool isvertimove(position Sc, position Dc)
{
	return Sc.ci == Dc.ci;
}
bool isdiagmove(position Sc, position Dc)
{
	int dc = abs(Dc.ci - Sc.ci);
	int dr = abs(Dc.ri - Sc.ri);
	return dc == dr;
}
bool ishoripc(char** B, position Sc, position Dc)
{
	int cs, ce;
	if (Sc.ci < Dc.ci)
	{
		cs = Sc.ri + 1;
		ce = Dc.ri - 1;
	}
	else
	{
		cs = Dc.ci + 1;
		ce = Sc.ci - 1;
	}
	for (int c = cs; c <= ce; c++)
	{
		if (B[Sc.ci][c] != '-')
			return false;
	}
	return true;




}
bool isvertipc(char** B, position Sc, position Dc)
{
	int rs, re;
	if (Sc.ri < Dc.ri)
	{
		rs = Sc.ri;
		re = Dc.ri;
	}
	else
	{
		rs = Dc.ri;
		re = Sc.ri;
	}
	for (int r = rs + 1; r < re; r++)
	{
		if (B[r][Sc.ci] != '-')
			return false;
	}
	return true;


}
bool isdiagL2Rpc(char** B, position Sc, position Dc)
{
	int dr;
	if (Sc.ri < Dc.ri)
	{
		dr = Dc.ri - Sc.ri - 1;
		for (int i = 1; i <= dr; i++)
		{
			if (B[Sc.ri + i][Sc.ci + i] != '-')
				return false;
		}
	}
	else
	{
		for (int i = 1; i < Sc.ri - Dc.ri; i++)
		{
			if (B[Sc.ri - i][Sc.ci - i] != '-')
				return false;
		}
	}
	return true;

}
bool isdiagR2Lpc(char** B, position Sc, position Dc)
{
	if (Sc.ri < Dc.ri)
	{
		for (int i = 1; i < Dc.ri - Sc.ri; i++)
		{
			if (B[Sc.ri + i][Sc.ci - i] != '-')
				return false;
		}
	}
	else
	{
		for (int i = 1; i < Sc.ri - Dc.ri; i++)
		{
			if (B[Sc.ri - i][Sc.ci + i] != '-')
				return false;
		}
	}
	return true;
}
bool rooklegality(char** B, position Sc, position Dc)
{
	return (ishoripc(B, Sc, Dc) && ishorizmove(Sc, Dc)) || (isvertipc(B, Sc, Dc) && isvertimove(Sc, Dc));
}
bool bishoplegal(char** B, position Sc, position Dc)
{
	return (isdiagmove(Sc, Dc) && isdiagL2Rpc(B, Sc, Dc) || isdiagmove(Sc, Dc) && isdiagR2Lpc(B, Sc, Dc));

}
bool bishoplegalgg(char** B, position Sc, position Dc,int Turn)
{
	if (Turn == WHITE)
	{
		if ((abs(Dc.ri - Sc.ri) == 1) && (abs(Dc.ci - Sc.ci) == 1) && Sc.ri > Dc.ri)
		{
			return (isdiagmove(Sc, Dc) && isdiagL2Rpc(B, Sc, Dc) || isdiagmove(Sc, Dc) && isdiagR2Lpc(B, Sc, Dc));
		}
		return false;

	}
	else if (Turn == BLACK)
	{
		if ((abs(Dc.ri - Sc.ri) == 1) && (abs(Dc.ci - Sc.ci) == 1) && Sc.ri <  Dc.ri)
		{
			return (isdiagmove(Sc, Dc) && isdiagL2Rpc(B, Sc, Dc) || isdiagmove(Sc, Dc) && isdiagR2Lpc(B, Sc, Dc));
		}
		return false;
	}
	else
		return false;
}
bool king(char** B, position Sc, position Dc)
{
	return (abs(Dc.ri - Sc.ri) <= 1) && (abs(Dc.ci - Sc.ci) <= 1) && (rooklegality(B, Sc, Dc) || bishoplegal(B, Sc, Dc));
}
bool promotedrook(char** B, position Sc, position Dc,int Turn)
{
	if (Turn == WHITE && Sc.ri > 2 )
	{
		return rooklegality(B,Sc,Dc);
	}
	if (Turn == WHITE && Sc.ri <= 2)
	{
		return king(B, Sc, Dc) || rooklegality(B, Sc, Dc);
	}
	if (Turn == BLACK && Sc.ri < 6 )
	{
		return rooklegality(B, Sc, Dc);
	}
	if (Turn == BLACK && Sc.ri >= 6)
	{
		return king(B, Sc, Dc) || rooklegality(B, Sc, Dc);
	}
	else
		return false;
}
bool promotedbishop(char** B, position Sc, position Dc, int Turn)
{
	if (Turn == WHITE && Sc.ri > 2)
	{
		return bishoplegal(B, Sc, Dc);
	}
	if (Turn == WHITE && Sc.ri <= 2)
	{
		return king(B, Sc, Dc) || bishoplegal(B, Sc, Dc);
	}
	if (Turn == BLACK && Sc.ri < 6)
	{
		return bishoplegal(B, Sc, Dc);
	}
	if (Turn == BLACK && Sc.ri >= 6)
	{
		return king(B, Sc, Dc) || bishoplegal(B, Sc, Dc);
	}
	else
		return false;
}
bool goldgeneral(char** B, position Sc, position Dc, int turn)
{
	if (turn == WHITE && (abs(Dc.ri - Sc.ri) == 1) || (abs(Dc.ci - Sc.ci) == 1))
	{
		return bishoplegalgg(B, Sc, Dc,turn) || rooklegality(B,Sc,Dc);
	}
	if ((turn == BLACK) && (abs(Dc.ri - Sc.ri) == 1) || (abs(Dc.ci - Sc.ci) == 1))
	{
		return bishoplegalgg(B, Sc, Dc, turn) || rooklegality(B, Sc, Dc);
	}
	else
		return false;
}
bool pawn(char** B, position Sc, position Dc, int Turn)
{

	if (Turn == WHITE && Sc.ri > 2 && (Dc.ri - Sc.ri == -1) && isvertimove(Sc, Dc))
	{
		return isvertipc(B, Sc, Dc);
	}
	if (Turn == WHITE && Sc.ri <= 2)
	{
		return goldgeneral(B, Sc, Dc, Turn);
	}
	if (Turn == BLACK && Sc.ri < 6 && (Dc.ri - Sc.ri == 1) && isvertimove(Sc, Dc))
	{
		return isvertipc(B, Sc, Dc);
	}
	if (Turn == BLACK && Sc.ri >= 6)
	{
		return goldgeneral(B, Sc, Dc, Turn);
	}
	else
		return false;
}
bool knight(char** B, position Sc, position Dc, int Turn)
{
	int dr = abs(Dc.ri - Sc.ri);
	int dc = abs(Dc.ci - Sc.ci);
	if (Turn == WHITE && Sc.ri > 2)
	{
		return ((dr == 2 && dc == 1) /*|| (dr == 1 && dc == 2)*/);
	}
	if (Turn == WHITE && Sc.ri <= 2)
	{
		return goldgeneral(B, Sc, Dc, Turn);
	}
	if (Turn == BLACK && Sc.ri < 6)
	{
		return ((dr == 2 && dc == 1) /*|| (dr == 1 && dc == 2)*/);
	}
	if (Turn == BLACK && Sc.ri >= 6)
	{
		return goldgeneral(B, Sc, Dc, Turn);
	}
	else
		return false;
}
bool lances(char** B, position Sc, position Dc, int Turn)
{
	if (Turn == WHITE && Sc.ri > 2 && Dc.ri<Sc.ri)
	{
		return (isvertipc(B, Sc, Dc) && isvertimove(Sc, Dc));
	}
	if (Turn == WHITE && Sc.ri <= 2)
	{
		return goldgeneral(B, Sc, Dc, Turn);
	}
	if (Turn == BLACK && Sc.ri < 6 && Dc.ri > Sc.ri)
	{
		return (isvertipc(B, Sc, Dc) && isvertimove(Sc, Dc));
	}
	if (Turn == BLACK && Sc.ri >= 6)
	{
		return goldgeneral(B, Sc, Dc, Turn);
	}
	else
		return false;


}
bool silvergeneral(char** B, position Sc, position Dc, int Turn)
{
	if (Turn == WHITE && Sc.ri > 2 && abs(Dc.ri - Sc.ri) <= 1 && abs(Dc.ci - Sc.ci) <= 1)
	{
		return pawn(B, Sc, Dc, Turn)|| bishoplegal(B, Sc, Dc);
	}
	if (Turn == WHITE && Sc.ri <= 2)
	{
		return goldgeneral(B, Sc, Dc, Turn);
	}
	if (Turn == BLACK && Sc.ri < 6 && abs(Dc.ri - Sc.ri) <= 1 && abs(Dc.ci - Sc.ci) <= 1)
	{
		return pawn(B, Sc, Dc, Turn) || bishoplegal(B, Sc, Dc);
	}
	if (Turn == BLACK && Sc.ri >= 6)
	{
		return goldgeneral(B, Sc, Dc, Turn);
	}
	else
		return false;
}
void Print_pawn(float x, float y, float Scale, char Sym)
{
	gotoRowCol(x+1 , y);
	cout << Sym << endl;
	for (int i = 0; i < 3; i++)
	{
		gotoRowCol(x + 3, y+i-1);
		cout << Sym << endl;
	}
	for (float i = 0; i <2* Scale+1; i++)
	{
		gotoRowCol(x + Scale, y + i - Scale);
		cout << Sym;
	}
	for (int i = 0; i < 3 * Scale+1; i++)
	{
		gotoRowCol(x + 4, y+i-Scale-1);
		cout << Sym << endl;
	}



}
void Print_lance(float x, float y, float Scale, char Sym)
{
	gotoRowCol(x , y);
	cout << Sym << endl;
	for (int i = 0; i < Scale; i++)
	{
		gotoRowCol(x + 1, y + i - 1);
		cout << Sym << endl;
	}
	for (int i = 0; i < Scale; i++)
	{
		gotoRowCol(x + 2, y + i - 1);
		cout << Sym << endl;
	}
	for (int i = 0; i < Scale; i++)
	{
		gotoRowCol(x + 3, y + i - 1);
		cout << Sym << endl;
	}
	for (int i = 0; i < 2 * Scale-1 ; i++)
	{
		gotoRowCol(x + 4, y + i-Scale+1 );
		cout << Sym << endl;
	}

}
void Print_bishop(float x, float y, float Scale, char Sym)
{
	gotoRowCol(x, y);
	cout << Sym << endl;
	for (int i = 0; i < Scale ; i++)
	{
		gotoRowCol(x + 1, y + i - Scale + 2);
		cout << Sym << endl;
	}
	for (int i = 0; i < 2 * Scale - 1; i++)
	{
		gotoRowCol(x + 2, y + i - Scale + 1);
		cout << Sym << endl;
	}
	for (int i = 0; i < Scale ; i++)
	{
		gotoRowCol(x + 3, y + i - Scale +2);
		cout << Sym << endl;
	}
	gotoRowCol(x+4, y);
	cout << Sym << endl;
}
void Print_knight(float x, float y, float Scale, char Sym)
{
	gotoRowCol(x, y);
	cout << Sym << endl;
	for (int i = 0; i < Scale+1; i++)
	{
		gotoRowCol(x + 1, y + i - Scale + 2);
		cout << Sym << endl;
	}
	for (int i = 0; i < Scale-1; i++)
	{
		gotoRowCol(x + 2, y + i+2);
		cout << Sym << endl;
	}
	for (int i = 0; i < Scale; i++)
	{
		gotoRowCol(x + 2, y + i - Scale + 1);
		cout << Sym << endl;
	}
	for (int i = 0; i < Scale-1; i++)
	{
		gotoRowCol(x + 3, y + i - Scale+2 );
		cout << Sym << endl;
	}
	gotoRowCol(x + 4, y);
	cout << Sym << endl;
	for (int i = 0; i < 2 * Scale - 1; i++)
	{
		gotoRowCol(x + 5, y + i - Scale + 1);
		cout << Sym << endl;
	}
}
void Print_sg(float x, float y, float Scale, char Sym)
{
	gotoRowCol(x, y);
	cout << Sym << endl;
	for (int i = 0; i < 2*Scale-1; i++)
	{
		gotoRowCol(x + 1, y + i - Scale + 1);
		cout << Sym << endl;
	}
	for (int i = 0; i < Scale; i++)
	{
		gotoRowCol(x + 2, y + i - 1);
		cout << Sym << endl;
	}
	for (int i = 0; i < Scale; i++)
	{
		gotoRowCol(x + 3, y + i - 1);
		cout << Sym << endl;
	}
	for (int i = 0; i < 2 * Scale - 1; i++)
	{
		gotoRowCol(x + 4, y + i - Scale + 1);
		cout << Sym << endl;
	}
	for (int i = 0; i < 2 * Scale +1; i++)
	{
		gotoRowCol(x + 5, y + i - Scale );
		cout << Sym << endl;
	}
}
void Print_gg(float x, float y, float Scale, char Sym)
{

	gotoRowCol(x, y-Scale+1);
	cout << Sym << endl;
	gotoRowCol(x , y+Scale-1);
	cout << Sym << endl;
	for (int i = 0; i < 2 * Scale - 1; i++)
	{
		gotoRowCol(x + 1, y + i - Scale + 1);
		cout << Sym << endl;
	}
	for (int i = 0; i < Scale; i++)
	{
		gotoRowCol(x + 2, y + i - 1);
		cout << Sym << endl;
	}
	for (int i = 0; i < Scale; i++)
	{
		gotoRowCol(x + 3, y + i - 1);
		cout << Sym << endl;
	}
	for (int i = 0; i < 2 * Scale - 1; i++)
	{
		gotoRowCol(x + 4, y + i - Scale + 1);
		cout << Sym << endl;
	}
	for (int i = 0; i < 2 * Scale + 1; i++)
	{
		gotoRowCol(x + 5, y + i - Scale);
		cout << Sym << endl;
	}
}
void Print_king(float x, float y, float Scale, char Sym)
{

	gotoRowCol(x-1, y+Scale );
	cout << Sym << endl;
	gotoRowCol(x-1, y -Scale);
	cout << Sym << endl;
	gotoRowCol(x - 1, y);
	cout << Sym << endl;
	for (int i = 0; i < 2 * Scale + 1; i++)
	{
		gotoRowCol(x, y + i - Scale);
		cout << Sym << endl;
	}
	for (int i = 0; i < 2 * Scale - 1; i++)
	{
		gotoRowCol(x + 1, y + i - Scale + 1);
		cout << Sym << endl;
	}
	for (int i = 0; i < Scale; i++)
	{
		gotoRowCol(x + 2, y + i - 1);
		cout << Sym << endl;
	}
	for (int i = 0; i < Scale; i++)
	{
		gotoRowCol(x + 3, y + i - 1);
		cout << Sym << endl;
	}
	for (int i = 0; i < 2 * Scale - 1; i++)
	{
		gotoRowCol(x + 4, y + i - Scale + 1);
		cout << Sym << endl;
	}
	for (int i = 0; i < 2 * Scale + 1; i++)
	{
		gotoRowCol(x + 5, y + i - Scale);
		cout << Sym << endl;
	}
}
void Print_rook(float x, float y, float Scale, char Sym)
{

	for (int i = 0; i < 3; i++)
	{
		gotoRowCol(x , y+i-1);
		cout << Sym << endl;
	}
	for (int i = 0; i < Scale+1; i++)
	{
		gotoRowCol(x +i-1, y );
		cout << Sym << endl;
	}
	for (int i = 0; i < 3; i++)
	{
		gotoRowCol(x + 2, y + i - 1);
		cout << Sym << endl;
	}

	for (int i = 0; i < 3; i++)
	{
		gotoRowCol(x + 3, y + i - 1);
		cout << Sym << endl;
	}
	
	for (int i = 0; i < 2 * Scale - 1; i++)
	{
		gotoRowCol(x + 4, y + i - Scale + 1);
		cout << Sym << endl;
	}
}
void DrawBox(int sr, int sc, int BRows, int BCols, char sym, int clr, int Turn)
{
	for (int r = 0; r < BRows; r++)
	{
		for (int c = 0; c < BCols; c++)
		{
			SetClr(clr);
			gotoRowCol(sr + r, sc + c);
			cout << char(-37);

		}
	}
	
	if (sym == 'p' )
	{
		SetClr(4);
		Print_pawn(sr + (BRows / 3 - 1), sc + (BCols / 2), 2, -37);
	}
	else if (sym == 'P')
	{
		SetClr(0);
		Print_pawn(sr + (BRows / 3 - 1), sc + (BCols / 2), 2, -37);
	}
	else if (sym == 'r')
	{
		SetClr(4);
		Print_rook(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'R')
	{
		SetClr(0);
		Print_rook(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'l')
	{
		SetClr(4);
		Print_lance(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'L')
	{
		SetClr(0);
		Print_lance(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 's')
	{
		SetClr(4);
		Print_sg(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'S')
	{
		SetClr(0);
		Print_sg(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'g')
	{
		SetClr(4);
		Print_gg(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'G')
	{
		SetClr(0);
		Print_gg(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'k')
	{
		SetClr(4);
		Print_king(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'K')
	{
		SetClr(0);
		Print_king(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'b')
	{
		SetClr(4);
		Print_bishop(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'B')
	{
		SetClr(0);
		Print_bishop(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'n')
	{
		SetClr(4);
		Print_knight(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'N')
	{
		SetClr(0);
		Print_knight(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}

}
void DrawBoard(char** B, int dim, int Brows, int Bcols, int turn)
{
	//system("cls");
	for (int r = 0; r < dim; r++)
	{
		for (int c = 0; c < dim; c++)
		{

			if ((r + c) % 2 == 0)
			{

				DrawBox(r * Brows, c * Bcols, Brows, Bcols, B[r][c], 8, turn);
			}
			else
			{

				DrawBox(r * Brows, c * Bcols, Brows, Bcols, B[r][c], 15, turn);
			}
		}
	}
}
void Selectsrc(position& P, int rows, int cols)
{
	//cout << "Give Coordinate(A1:H8) :";
	//char C;
	//int D;
	//cin >> C >> D;
	//C = toupper(C);

	//P.ri = D - 1;
	//P.ci = C - 'A';
	int r, c;
	getRowColbyLeftClick(r, c);
	P.ri = r / rows;
	P.ci = c / cols;
}
bool isWhitepiece(char sym)
{
	return sym >= 'A' && sym <= 'Z';
}
bool isBlackpiece(char sym)
{
	return sym >= 'a' && sym <= 'z';
}
bool ismypiece(char sym, int Turn)
{
	if (Turn == BLACK)
		return isBlackpiece(sym);
	if (Turn == WHITE)
		return isWhitepiece(sym);

}
bool Validsrc(char** B, position Sc, int dim, int turn)
{
	if (Sc.ri < 0 || Sc.ri >= dim || Sc.ci < 0 || Sc.ci >= dim)
		return false;
	return ismypiece(B[Sc.ri][Sc.ci], turn);
}
bool Validdst(char** B, int Dim, position Sc, int Turn)
{
	if (Sc.ri < 0 || Sc.ri >= Dim || Sc.ci < 0 || Sc.ci >= Dim)
		return false;
	return !ismypiece(B[Sc.ri][Sc.ci], Turn);
}
void printboard(char** Brd, int dim)
{
	for (int r = 0; r < dim; r++)
	{
		for (int c = 0; c < dim; c++)
		{
			cout << Brd[r][c] << " ";
		}
		cout << endl;
	}
}
bool islegalmove(char** B, position Sc, position Dc, int dim, int turn)
{
	switch (B[Sc.ri][Sc.ci])
	{
	case 'p':
	case 'P':
		return pawn(B, Sc, Dc, turn);
	case 'b':
	case 'B':
		return promotedbishop(B, Sc, Dc,turn);
	case 'r':
	case 'R':
		return promotedrook(B, Sc, Dc,turn);
	case 'n':
	case 'N':
		return knight(B, Sc, Dc, turn);
	case 'l':
	case 'L':
		return lances(B, Sc, Dc, turn);
	case 's':
	case 'S':
		return silvergeneral(B, Sc, Dc, turn);
	case 'g':
	case 'G':
		return goldgeneral(B, Sc, Dc, turn);
	case 'k':
	case 'K':
		return king(B, Sc, Dc);
	}
}
void updateboard(char** B, position Sc, position Dc)
{
	char p = B[Sc.ri][Sc.ci];
	B[Dc.ri][Dc.ci] = p;
	B[Sc.ri][Sc.ci] = '-';
}
void nakliupdateboard(char** B, position Sc, position Dc)
{
	char p = B[Sc.ri][Sc.ci];
	B[Dc.ri][Dc.ci] = p;
	B[Sc.ri][Sc.ci] = '-';
}

void updateundoboard(char** B, position Sc, position Dc)
{
	char p = B[Dc.ri][Dc.ci];
	B[Sc.ri][Sc.ci] = p;
	B[Dc.ri][Dc.ci] = '-';
}
void TurnMsg(string PNames[], int turn)
{
	gotoRowCol(25, 82);
	{
		SetClr(1);
		cout << "--------------- ** TURN's MESSAGE ** -----------\n\n\n"<<
	"\t\t\t\t\t\t\t\t\t\t\t"<< PNames[turn] << "\'s Turn: " << endl
		<< "\n\n\t\t\t\t\t\t\t\t\t\t\t-----------------------------------------------\n";
	}
}
void turnchange(int& Turn)
{
	Turn = (Turn + 1) % 2;
}

bool** chighlight(char** B, position Sc, int dim, int turn)
{
	bool** bmap = new bool* [dim];
	for (int bi = 0; bi < dim; bi++)
	{
		bmap[bi] = new bool[dim] {};
	}
	position Dc{};
	for (int ri = 0; ri < dim; ri++)

	{
		for (int ci = 0; ci < dim; ci++)
		{
			Dc.ri = ri, Dc.ci = ci;
			if (Validdst(B, dim, Dc, turn) && islegalmove(B, Sc, Dc, dim, turn))
			{
				bmap[ri][ci] = true;
			}
			
		}
	}
	return bmap;


}
void Drawhighlight(int sr, int sc, int BRows, int BCols, char sym, int clr)
{
	for (int r = 0; r < BRows; r++)
	{
		for (int c = 0; c < BCols; c++)
		{
			if (r == 0 || r == BRows - 1 || c == 0 || c == BCols - 1)
			{
				SetClr(clr);
				gotoRowCol(sr + r, sc + c);
				cout << char(-37);

			}
		}
	}
}
void highlight(char** B,bool** M, int dim, int Brows, int Bcols)
{
	
	for (int r = 0; r < dim; r++)
	{
		for (int c = 0; c < dim; c++)
		{
			if (M[r][c] == true)
			{
					Drawhighlight(r * Brows, c * Bcols, Brows, Bcols, B[r][c], 1);
				
			}
		}
	}
}
void Drawunhighlight(int sr, int sc, int BRows, int BCols, char sym, int clr)
{
	for (int r = 0; r < BRows; r++)
	{
		for (int c = 0; c < BCols; c++)
		{
			if (r == 0 || r == BRows - 1 || c == 0 || c == BCols - 1)
			{
				SetClr(clr);
				gotoRowCol(sr + r, sc + c);
				cout << char(-37);

			}
		}
	}
}

void Unhighlight(char** B, bool** M, int dim, int Brows, int Bcols)
{
	for (int r = 0; r < dim; r++)
	{
		for (int c = 0; c < dim; c++)
		{

			if ((r + c) % 2 == 0)
			{

				Drawunhighlight(r * Brows, c * Bcols, Brows, Bcols, B[r][c], 8);
			}
			else
			{

				Drawunhighlight(r * Brows, c * Bcols, Brows, Bcols, B[r][c], 15);
			}
		}
	}
}
position findking(char **B,int dim,int Turn)
{
	position Dc{};
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			if (B[i][j] == 'K' || B[i][j] == 'k')
			{
				Dc.ri = i, Dc.ci = j;
			}
		}
	}
	return Dc;
	
}
bool check(char** B, int dim, int Turn)
{
	turnchange(Turn);
	position Dc = findking(B, dim, Turn);

	turnchange(Turn);
	position Sc{};
	for (int ri = 0; ri < dim; ri++)
	{
		for (int ci = 0; ci < dim; ci++)
		{
			Sc.ri = ri, Sc.ci = ci;
			if (Validsrc(B, Sc, dim, Turn) && islegalmove(B, Sc, Dc, dim, Turn))
			{
				return true;
			}
		}
	}

}
bool selfcheck(char** B, int dim, int Turn)
{
	turnchange(Turn);
	return check(B, dim, Turn);
}
void save(char **B,position Sc,position Dc)
{
	ofstream wrt("Text2.txt");
	wrt << Sc.ri << " " << Sc.ci << " " << Dc.ri << " " << Dc.ci ;
}
void undo(char** B, position Sc, position Dc)
{
	ifstream rdr("Text2.txt");
	rdr>>Sc.ri>>Sc.ci >> Dc.ri >> Dc.ci;
}
void initialize(char**& B, int& dim, string Pnames[], int& turn)
{
	ifstream rdr("Text.txt");
	/*char A;
	cout << "DO u want to play previous game";
	cin >> A;
	if (A == 'Y')
	{
		save(turn);
	}*/
	//else
	{
		cout << "\nEnter first player name : ";
		cin >> Pnames[0];
		cout << "Enter second player name : ";
		cin >> Pnames[1];
		rdr >> dim;
		turn = WHITE;
		B = new char* [dim];
		for (int r = 0; r < dim; r++)
		{
			B[r] = new char[dim];
		}
		for (int r = 0; r < dim; r++)
		{
			for (int c = 0; c < dim; c++)
				rdr >> B[r][c];
		}
	}

}
void DrawBoxundo(int sr, int sc, int BRows, int BCols, char sym, int clr, int Turn)
{
	for (int r = 0; r < BRows; r++)
	{
		for (int c = 0; c < BCols; c++)
		{
			SetClr(clr);
			gotoRowCol(sr + r, sc + c);
			cout << char(-37);

		}
	}
	gotoRowCol(sr + BRows / 2, sc + BCols / 3);
	SetClr(10);
	cout << "UNDO";
}
void DrawBoardundo(char** B, int dim, int Brows, int Bcols, int turn)
{

	for (int r = 8; r < 9; r++)
	{
		for (int c = 10; c < 11; c++)
		{
			{
				DrawBoxundo(r * Brows, c * Bcols, Brows, Bcols, B[r][c], 2, turn);
			}
		}
	}
}
bool validundo(char** B, int Dim, position Sc, int Turn)
{
	if (Sc.ri < 8 || Sc.ri >= Dim || Sc.ci < 10 || Sc.ci >= 11)
		return false;
	return true;
}
bool Validdstcapture(char** B, int Dim, position Sc, int Turn,char& sym)
{
	if (Sc.ri < 0 || Sc.ri >= Dim || Sc.ci < 0 || Sc.ci >= Dim)
		return false;
	turnchange(Turn);

	sym = B[Sc.ri][Sc.ci];
	if (ismypiece(sym, Turn))
	{
		return sym;
	}
	else
		return false;
}

void svaecapturedpieces(char white_pieces[],char Black_pieces[],int turn,char sym)
{
	if (turn == 0)
	{
		for (int i = 0; i < captured_pieces; i++)
		{
			if (white_pieces[i] == '-')
			{
				white_pieces[i] = sym;
				return;
			}
		}
	}
	else if (turn == 1)
	{
		for (int i = 0; i < captured_pieces; i++)
		{
			if (Black_pieces[i] == '-')
			{
				Black_pieces[i] = sym;
				return;
			}
		}
	}
	
}
bool ismypiecedrop(char sym, int Turn)
{
	if (Turn == BLACK)
		return isBlackpiece(sym);
	if (Turn == WHITE)
		return isWhitepiece(sym);

}
bool Validsrcdrop(char** B, char whitepieces[], position Sc, int dim, int turn)
{
	if (Sc.ri < 1 || Sc.ri >= 3 || Sc.ci < 9 || Sc.ci >= 17)
		return false;
	int index = Sc.ri + Sc.ci + 8;
	B[Sc.ri][Sc.ci] = whitepieces[0];
	return ismypiece(B[Sc.ri][Sc.ci], turn);
}
bool Validdstdrop(char** B, int Dim, position Sc, int Turn)
{
	if (Sc.ri < 0 || Sc.ri >= Dim || Sc.ci < 0 || Sc.ci >=11)
		return false;
	return !ismypiece(B[Sc.ri][Sc.ci], Turn);
}

void printcapturedpieces(char white_pieces[], char Black_pieces[], int turn)
{
	if (turn == 0)
	{
		for (int i = 0; i < captured_pieces; i++)
		{
			gotoRowCol(6*3,12*8 + (8 * i));
			SetClr(4);

			if (white_pieces[i] != '-')
			{
				if (white_pieces[i] == 'p')
				{
					Print_pawn(6 * 3, 12 * 8 + (8 * i), 2, -37);
				}
				else if (white_pieces[i] == 'b')
				{
					Print_bishop(6 * 3, 12 * 8 + (8 * i), 3, -37);
				}
				else if (white_pieces[i] == 'r')
				{
					Print_rook(6 * 3, 12 * 8 + (8 * i), 3, -37);
				}
				else if (white_pieces[i] == 'l')
				{
					Print_lance(6 * 3, 12 * 8 + (8 * i), 3, -37);
				}
				else if (white_pieces[i] == 'n')
				{
					Print_knight(6 * 3, 12 * 8 + (8 * i), 3, -37);
				}
				else if (white_pieces[i] == 's')
				{
					Print_sg(6 * 3, 12 * 8 + (8 * i), 3, -37);
				}
				else if (white_pieces[i] == 'g')
				{
					Print_gg(6 * 3, 12 * 8 + (8 * i), 3, -37);
				}
				else if (white_pieces[i] == 'k')
				{
					Print_king(6 * 3, 12 * 8 + (8 * i), 3, -37);
				}
			}
			
		}
	}
	else if (turn == 1)
	{
		for (int i = 0; i < captured_pieces; i++)
		{
			gotoRowCol(9*5, 12 * 8 + (8 * i));
			SetClr(0);
			if (Black_pieces[i] != '-')
			{
				if (Black_pieces[i] == 'P')
				{
					Print_pawn(9*5, 12 * 8 + (8 * i), 2, -37);
				}
				else if (Black_pieces[i] == 'L')
				{
					Print_lance(9 * 5, 12 * 8 + (8 * i), 3, -37);
				}
				else if (Black_pieces[i] == 'N')
				{
					Print_knight(9 * 5, 12 * 8 + (8 * i), 3, -37);
				}
				else if (Black_pieces[i] == 'S')
				{
					Print_sg(9 * 5, 12 * 8 + (8 * i), 3, -37);
				}
				else if (Black_pieces[i] == 'G')
				{
					Print_gg(9 * 5, 12 * 8 + (8 * i), 3, -37);
				}
				else if (Black_pieces[i] == 'K')
				{
					Print_king(9 * 5, 12 * 8 + (8 * i), 3, -37);
				}
				else if (Black_pieces[i] == 'B')
				{
					Print_bishop(9 * 5, 12 * 8 + (8 * i), 3, -37);
				}
				else if (Black_pieces[i] == 'R')
				{
					Print_rook(9 * 5, 12 * 8 + (8 * i), 3, -37);
				}
			}
		}
	}
}
void DrawBoxforwhitecapture(int sr, int sc, int BRows, int BCols, char sym, int clr, int Turn)
{
	for (int r = BRows; r < 2 * BRows; r++)
	{
		for (int c = BCols; c < 2 * BCols; c++)
		{
			SetClr(clr);
			gotoRowCol(sr + r, sc + c);
			cout << char(-37);

		}
	}
}
void DrawBoardforwhitecapture(char** B, int dim, int Brows, int Bcols, int turn)
{

	//system("cls");
	{
		gotoRowCol(14, 90);
		SetClr(3);
		cout << "White's Captured Pieces";
	}
	for (int r = 1; r < 3; r++)
	{
		for (int c = 9; c < 17; c++)
		{
			DrawBoxforwhitecapture(r * Brows, c * Bcols, Brows, Bcols,B[r][c], 15, turn);
		
		}
	}
}
void DrawBoxforblackcapture(int sr, int sc, int BRows, int BCols, char sym, int clr, int Turn)
{
	for (int r = BRows; r < 2 * BRows; r++)
	{
		for (int c = BCols; c < 2 * BCols; c++)
		{
			SetClr(clr);
			gotoRowCol(sr + r, sc + c);
			cout << char(-37);

		}
	}

	/*if (sym == 'p')
	{
		SetClr(4);
		Print_pawn(sr + (BRows / 3 - 1), sc + (BCols / 2), 2, -37);
	}
	else if (sym == 'P')
	{
		SetClr(0);
		Print_pawn(sr + (BRows / 3 - 1), sc + (BCols / 2), 2, -37);
	}
	else if (sym == 'r')
	{
		SetClr(4);
		Print_rook(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'R')
	{
		SetClr(0);
		Print_rook(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'l')
	{
		SetClr(4);
		Print_lance(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'L')
	{
		SetClr(0);
		Print_lance(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 's')
	{
		SetClr(4);
		Print_sg(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'S')
	{
		SetClr(0);
		Print_sg(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'g')
	{
		SetClr(4);
		Print_gg(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'G')
	{
		SetClr(0);
		Print_gg(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'k')
	{
		SetClr(4);
		Print_king(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'K')
	{
		SetClr(0);
		Print_king(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'b')
	{
		SetClr(4);
		Print_bishop(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'B')
	{
		SetClr(0);
		Print_bishop(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'n')
	{
		SetClr(4);
		Print_knight(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}
	else if (sym == 'N')
	{
		SetClr(0);
		Print_knight(sr + (BRows / 3 - 1), sc + (BCols / 2), 3, -37);
	}*/

}
void DrawBoardforblackcapture(char** B, int dim, int Brows, int Bcols, int turn)
{
	//system("cls");

	gotoRowCol(39, 90);
	SetClr(3);
	cout << "Black's Captured Pieces";
	for (int r = 4; r < 6; r++)
	{
		for (int c = 9; c < 17; c++)
		{

			DrawBoxforblackcapture(r * Brows, c * Bcols, Brows, Bcols, B[r][c], 15, turn);
			
		}
	}
}
void drop(char** &B, position Sc, position Dc,int r,int c)
{
	B[4][2] = 'P';
}
int main()
{
	//srand(time(0));
	int dim, Turn, r = 9, c = 9;
	position Sc, Dc,Uc,Drc{};
	char** B;
	bool** M;
	int i = 0;
	char White_pieces[captured_pieces];
	for (int i = 0; i < 20; i++)
	{
		White_pieces[i] = '-';
	}
	char Black_pieces[captured_pieces];
	for (int i = 0; i < 20; i++)
	{
		Black_pieces[i] = '-';
	}
	string Pnames[2];
	char sym=0;
	initialize(B, dim, Pnames, Turn);
	DrawBoard(B, dim, r, c, Turn);
	DrawBoardforwhitecapture(B, dim, r, c, Turn);
	DrawBoardforblackcapture(B, dim, r, c, Turn);

	while (true)
	{
	
		//DrawBoard(B, dim, r, c,Turn);
		//TurnMsg(Pnames, Turn);
		do {
			do {
				do {
					do
					{
						Selectsrc(Sc, r, c);
						if (Validsrcdrop(B, White_pieces, Sc, dim, Turn))
						{
							Selectsrc(Drc, r, c);
							if (Validdstdrop(B, dim, Drc, Turn))
							{
								updateboard(B, Sc, Drc);
							}
						}

					} while (!Validsrc(B, Sc, dim, Turn));
					M = chighlight(B, Sc, dim, Turn);
					highlight(B, M, dim, r, c);
					Selectsrc(Dc, r, c);
					Unhighlight(B, M, dim, r, c);

				} while (!Validdst(B, dim, Dc, Turn));

				if (Validdstcapture(B, dim, Dc, Turn, sym))
				{
					svaecapturedpieces(White_pieces, Black_pieces, Turn, sym);
					printcapturedpieces(White_pieces, Black_pieces, Turn);

				}
			} while (!M[Dc.ri][Dc.ci]);
		} while (check(B, dim, Turn));
		/*do {
			nakliupdateboard(B, Sc, Dc);
		} while (!selfcheck(B, dim, Turn));
		if (selfcheck(B, dim, Turn))
		{
			updateboard(B, Sc, Dc);
		}
		if (check(B, dim, Turn) == true)
		{
			gotoRowCol(4, 90);
			cout << "DANGER";
		}
		*/
	//	drop(B, Sc, Dc, r, c);
		updateboard(B, Sc, Dc);
		
		save(B, Sc, Dc);
		DrawBoard(B, dim, r, c, Turn);
//		DrawBoardforwhitecapture(White_pieces, Black_pieces, dim, r, c, Turn);

		{
			gotoRowCol(2, 95);
			DrawBoardundo(B, dim, r, c, Turn);
			SetClr(10);
			Selectsrc(Uc, r, c);
		}
		if (validundo(B, dim, Uc, Turn))
		{
			undo(B, Sc, Dc);
			updateundoboard(B, Sc, Dc);
			DrawBoard(B, dim, r, c, Turn);
		}
		else
		{
			turnchange(Turn);
		}
		
	}
	_getch();


}
