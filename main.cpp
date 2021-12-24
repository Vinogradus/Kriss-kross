//#define _CRT_SECURE_NO_WARNINGS
#define hei 37 //Высота
#define wid 100 //Ширина
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include "windows.h"
#include <time.h>

int peresdat = 0, index = 0, net = 0, nnn = 0, savei = 0;
int ploshadsave = 1000;

int mi_x;
int mi_y;

struct Slovo
{
	char *element;
	int x;
	int y;
	int etalonx;
	int etalony;
	int orient;//0 - горизонтально, 1 - верт. 1000 - слово не стоит
	int etalonorient;
	int peres;
	int dlin;
	struct Slovo *next;
};

struct Slovo *NList = NULL;


void obnul(struct Slovo *out)
{
	while (out != NULL)
	{
		out->orient = 1000;
		out->peres = 0;
		out = out->next;
	}
}

void obnulone(struct Slovo *out)
{
	out->orient = 1000;
	out->peres = 0;
}



void PROVERKA(struct Slovo *outn, struct Slovo *stoit)//проверка на пересечения с выставленными словами
{
	struct Slovo *prover = NList;
	int prov = 0;
	int x1 = 0, x2 = 0;
	int y1 = 0, y2 = 0;
	int strlo = outn->dlin;
	while (prov != index)
	{
		if (prover->orient != 1000 && prover != outn && prover != stoit)
		{
			int h = 0, k = 0;
			int bok = 0;
			int krest = 0;
			x1 = prover->x; y1 = prover->y;
			int x1left = prover->x - prover->orient; int y1left= prover->y + (prover->orient+1)%2;
			int x1right = prover->x + prover->orient; int y1right = prover->y - (prover->orient + 1) % 2;
			x2 = outn->x; y2 = outn->y;
			int strlp = prover->dlin;


			while (h != strlp)
			{
				if (x1 == x2 && y1 == y2)
				{
					if (prover->element[h] == outn->element[k]) { krest++; outn->peres++; }
					else { net++; return; }
				}
				if (x2 == x1left && y2 == y1left)bok++;
				if (x2 == x1right && y2 == y1right)bok++;
				k++;
				x2 = x2 + (outn->orient + 1) % 2;
				y2 = y2 + outn->orient;
				if (k == strlo)
				{
					k = 0;
					x2 = outn->x; y2 = outn->y;
					h++;
					x1 = x1 + (prover->orient + 1) % 2;
					y1 = y1 + prover->orient;
					x1left = x1 - prover->orient; y1left = y1 + (prover->orient + 1) % 2;
					x1right = x1 + prover->orient; y1right = y1 - (prover->orient + 1) % 2;
				}
			}
			if(bok!=0 && krest == 0)
			{
				net++; return;
			}

		}
		prover = prover->next;
		prov++;
	}
}

void OTSECH(int chslov)
{
    int ploshad=1, xmin=1000, ymin=1000, xmax=-1000, ymax=-1000;
    struct Slovo *podschet = NList;
            while (podschet!=NULL)
            {
                if(podschet->orient!=1000)
                {
                    int xx=podschet->x+((podschet->dlin-1)*((podschet->orient+1)%2));
                    int yy=podschet->y+((podschet->dlin-1)*podschet->orient);
                    if(xx > xmax){ xmax=xx;}
                    if(yy > ymax){ ymax=yy;}
                    if(podschet->x<xmin)xmin=podschet->x;
                    if(podschet->y<ymin)ymin=podschet->y;
                }
                podschet = podschet->next;
            }
    int x=xmax-xmin, y=ymax-ymin;
    ploshad=x*y;
    if(ploshad>ploshadsave) net++;
}

void postanovka(struct Slovo *out)
{
    int ii=0, jj=0;
    struct Slovo *stoit = NList;
    int strlout = out->dlin;
    int strlsto = stoit->dlin;

    while(1)
    {
    while(out->element[jj]!=stoit->element[ii])//цикл поиска места
    {
        jj++;
        if(jj+1==strlout)
        {
            jj=0;
            ii++;
            if(ii+1==strlsto)
            {
                ii=0;
                do{
                    stoit = stoit->next;
                    if(stoit==NULL){return;}//если для слова неt места, прекращаем конкретно эту ступень
                }while (stoit->orient == 1000);
                strlsto = stoit->dlin;
            }
        }
    }

    out->orient = (stoit->orient + 1) % 2;//ставим ориентацию и заодно обозначаем, что слово уже стоит
    out->peres++;
    if (out->orient == 1)//ставим координаты
        {
            out->x = stoit->x + ii;
            out->y = stoit->y - jj;
        }
    else
        {
            out->x = stoit->x - jj;
            out->y = stoit->y + ii;
        }
    PROVERKA(out, stoit);  //проверка на пересечения с другими выставленными словами
    if(net==0 && peresdat!=0)OTSECH(nnn+1);
    if(net)
    {
        //очищаем
        net=0;
        obnulone(out);
        //переходим на следующую интерацию цикла поиска места
        jj++;
        if(jj+1==strlout)
        {
            jj=0;
            ii++;
            if(ii+1==strlsto)
            {
                ii=0;
                do{
                    stoit = stoit->next;
                    if(stoit==NULL){return;}
                }while (stoit->orient == 1000);
                strlsto = stoit->dlin;
            }
        }
    }
    else
    {
        int n=nnn;
        nnn++;
        if(nnn==index)//если выставлены все слова, подсчитываем площадь
        {
            peresdat=1;
            int xmin=1000, ymin=1000, xmax=-1000, ymax=-1000;
            struct Slovo *podschet = NList;
            while (podschet!=NULL)
            {
                podschet->etalonorient=podschet->orient;
                podschet->etalonx=podschet->x;
                podschet->etalony=podschet->y;

                    int xx=podschet->x+((podschet->dlin-1)*((podschet->orient+1)%2));
                    int yy=podschet->y+((podschet->dlin-1)*podschet->orient);
                    if(xx > xmax){ xmax=xx;}
                    if(yy > ymax){ ymax=yy;}
                    if(podschet->x<xmin)xmin=podschet->x;
                    if(podschet->y<ymin)ymin=podschet->y;
                podschet = podschet->next;
            }
            int x=xmax-xmin, y=ymax-ymin;
            mi_x = xmin;
            mi_y = ymin;
            ploshadsave=x*y;

        }
        else //иначе ищем новое слово для постановки и колдуем с ним
        {
            struct Slovo *sled = NList->next;
            while (sled != NULL)
                {
                    while (sled->orient != 1000)
                      {
                          sled = sled->next;
                          if(sled==NULL)break;
                      }
                    if(sled==NULL)break;
                    postanovka(sled);
                    //очищаем после рекурсии
                    nnn=n+1;
                    obnulone(sled);
                    sled = sled->next;
                }
        }
        //очищаем выставленное слово
        nnn=n;
        obnulone(out);
        //переходим на следующую итерацию цикла поиска места
        jj++;
        if(jj+1==strlout)
        {
            jj=0;
            ii++;
            if(ii+1==strlsto)
            {
                ii=0;
                do{
                    stoit = stoit->next;
                    if(stoit==NULL){return;}
                }while (stoit->orient == 1000);
                strlsto = stoit->dlin;
            }
        }
    }
    }

}


//ОФОРМЛЕНИЕ
///Рисование клетки
void cell(int mass[hei][wid], int x, int y, int dir, int hatch) //0 - горизонтально, 1 - вертикально
{
	mass[x][y + 1] = mass[x][y + 2] = mass[x][y + 3] = mass[x + 2][y + 1] = mass[x + 2][y + 2] = mass[x + 2][y + 3] = 205;
	mass[x + 1][y] = mass[x + 1][y + 4] = 186;

	if (mass[x][y] == hatch)
		mass[x][y] = 201;
	else if (mass[x][y] == 200)
		mass[x][y] = 204;
	else if (mass[x][y] == 203 && mass[x - 1][y] == 186)
		mass[x][y] = 206;
	else if (mass[x][y] == 187 && dir == 0)
		mass[x][y] = 203;
	else if (mass[x][y] == 201 && dir == 0 && mass[x][y - 1] != hatch)
		mass[x][y] = 203;
	else if (mass[x][y] == 204 && dir == 0 && mass[x - 1][y] != hatch && mass[x][y - 1] != hatch && mass[x][y - 1] != 186)
		mass[x][y] = 206;
	else if (mass[x][y] == 188)
		mass[x][y] = 206;
	else if (mass[x][y] == 202 && dir == 1)
		mass[x][y] = 206;
	else if (mass[x][y] == 201 && dir == 1 && mass[x - 1][y] != hatch)
		mass[x][y] = 204;
	else if (mass[x][y] == 185)
		mass[x][y] = 206;

	if (mass[x][y + 4] == hatch)
		mass[x][y + 4] = 187;
	else if (mass[x][y + 4] == 188 && dir == 0)
		mass[x][y + 4] = 203;
	else if (mass[x][y + 4] == 188 && dir == 1)
		mass[x][y + 4] = 185;
	else if (mass[x][y + 4] == 185 && dir == 0)
		mass[x][y + 4] = 206;
	else if (mass[x][y + 4] == 202 || mass[x][y + 4] == 200)
		mass[x][y + 4] = 206;
	else if (mass[x][y + 4] == 203 && mass[x - 1][y + 4] == 186)
		mass[x][y + 4] = 206;

	if (mass[x + 2][y] == hatch)
		mass[x + 2][y] = 200;
	else if (mass[x + 2][y] == 188 && dir == 0)
		mass[x + 2][y] = 202;
	else if (mass[x + 2][y] == 187)
		mass[x + 2][y] = 206;
	else if (mass[x + 2][y] == 185 && dir == 0)
		mass[x + 2][y] = 206;
	else if ((mass[x + 2][y] == 202 || mass[x + 2][y] == 203) && dir == 1 && mass[x + 3][y] == 186)
		mass[x + 2][y] = 206;
	else if (mass[x + 2][y] == 204 && dir == 0 && mass[x + 2][y - 1] != hatch && mass[x + 2][y - 1] != 186)
		mass[x + 2][y] = 206;
	else if (mass[x + 2][y] == 203)
		mass[x + 2][y] = 206;
	else if (mass[x + 2][y] == 201)
		mass[x + 2][y] = 204;


	if (mass[x + 2][y + 4] == hatch)
		mass[x + 2][y + 4] = 188;
	else if (mass[x + 2][y + 4] == 202 && dir == 1)
		mass[x + 2][y + 4] = 206;
	else if (mass[x + 2][y + 4] == 203 && dir == 1)
		mass[x + 2][y + 4] = 206;
	else if (mass[x + 2][y + 4] == 187/* && dir == 1*/)
		mass[x + 2][y + 4] = 185;
	else if (mass[x + 2][y + 4] == 203 && mass[x + 2][y + 3] != hatch)
		mass[x + 2][y + 4] = 206;
	else if (mass[x + 2][y + 4] == 200 && dir == 0)
		mass[x + 2][y + 4] = 202;

	mass[x + 1][y + 1] = mass[x + 1][y + 2] = mass[x + 1][y + 3] = ' ';

}

///Рисование линии из клеток для слова
void line(int mass[hei][wid], int x, int y, int len, int direction, int hatch)
{
	if (direction) {//Вертикаль
		for (int i = x; i < len + x; i++)
			cell(mass, i * 2 + 1, y * 4 + 1, direction, hatch);
	}
	else {//Горизонталь
		for (int i = y; i < len + y; i++)
			cell(mass, x * 2 + 1, i * 4 + 1, direction, hatch);
	}
}

///Заполнение линии из клеток словом
void line_with_word(int mass[hei][wid], char word[], int x, int y, int dir)
{
	if (dir) {
		for (int i = x; i < strlen(word) + x - 1; i++) {
			mass[2 * i + 2][4 * y + 3] = word[i - x];
		}
	}
	else {
		for (int i = y; i < strlen(word) + y - 1; i++)
			mass[2 * x + 2][4 * i + 3] = word[i - y];
	}
}

///Вовод поля
void print(int mass[hei][wid])
{
	for (int i = 0; i < hei; i++) {
		for (int j = 0; j < wid; j++)
			printf("%c", mass[i][j]);
		printf("\n");
	}
}

///Заполнение всего поля определенным сиволом
void filling(int mass[hei][wid], int symbol)
{
	for (int i = 0; i < hei; i++) {
		for (int j = 0; j < wid; j++)
			mass[i][j] = symbol;
	}
}

///Рисование рамки
void frame(int mass[hei][wid])
{
	for (int i = 0; i < hei; i++) {
		mass[i][0] = 186;
		mass[i][wid - 1] = 186;
	}
	for (int i = 0; i < wid; i++) {
		mass[0][i] = 205;
		mass[hei - 1][i] = 205;
	}
	mass[0][0] = 201;
	mass[0][wid - 1] = 187;
	mass[hei - 1][0] = 200;
	mass[hei - 1][wid - 1] = 188;
}


///Приветствующий экран
void K(int mass[hei][wid], int x, int y)
{
	line(mass, x, y, 5, 1, 0);
	line(mass, x + 2, y, 2, 0, 0);
	line(mass, x + 1, y + 2, 1, 0, 0);
	line(mass, x + 3, y + 2, 1, 0, 0);
	line(mass, x + 0, y + 3, 1, 0, 0);
	line(mass, x + 4, y + 3, 1, 0, 0);
}
void P(int mass[hei][wid], int x, int y)
{
	line(mass, x, y, 5, 1, 0);
	line(mass, x, y, 3, 0, 0);
	line(mass, x + 2, y, 3, 0, 0);
	line(mass, x, y + 2, 3, 1, 0);
}
void N(int mass[hei][wid], int x, int y)
{
	line(mass, x, y, 5, 1, 0);
	line(mass, x + 2, y, 4, 0, 0);
	line(mass, x + 1, y + 2, 2, 0, 0);
	line(mass, x + 3, y, 2, 0, 0);
	line(mass, x, y + 3, 5, 1, 0);
}
void C(int mass[hei][wid], int x, int y)
{
	line(mass, x, y, 5, 1, 0);
	line(mass, x, y, 3, 0, 0);
	line(mass, x + 4, y, 3, 0, 0);
}
void O(int mass[hei][wid], int x, int y)
{
	line(mass, x, y, 5, 1, 0);
	line(mass, x, y, 4, 0, 0);
	line(mass, x + 4, y, 4, 0, 0);
	line(mass, x, y + 3, 5, 1, 0);
}
void kriss(int mass[hei][wid], int x, int y)
{
	K(mass, x, y);
	P(mass, x, y + 5);
	N(mass, x, y + 9);
	C(mass, x, y + 14);
	C(mass, x, y + 18);
	line(mass, x + 7, y + 9, 4, 0, 0);
	K(mass, x + 10, y);
	P(mass, x + 10, y + 5);
	O(mass, x + 10, y + 9);
	C(mass, x + 10, y + 14);
	C(mass, x + 10, y + 18);
}

void lin()
{
	for (int i = 0; i < 50; i++)
		printf("%c", 205);
	printf("\n");
}


int main()
{
	int field[hei][wid] = { 0 };//Игровое поле
	int hatch = 0;
	frame(field); kriss(field, 1, 1);
	system("pause");
	print(field);
	Sleep(900);
	system("cls");//Очищает экран консоли

	struct Slovo *data = NULL;
	FILE *fp;
	if ((fp = fopen("words.txt", "r")) == NULL)
	{
		printf("Could not open file");
		getchar();
		return 0;
	}
	char *str;
	int size = 49, i = 0;
	/*unsigned*/ char c;
	c = fgetc(fp);
	printf("Words to create a scheme: \n");
	while (!feof(fp)) {
		printf("%c", c);
		c = fgetc(fp);
	}
	lin(); fclose(fp);
	int choose = 0;
	Sleep(700);

	printf("Press...\n 1 - if you want to change words\n 0 - continue with this words\nYour choose: ");
	scanf("%d", &choose);
	while ((choose != 1) && (choose != 0)) {
		printf("Incorrect! Please try again\nYour choose: ");
		scanf("%d", &choose);
	}
	lin();
	if (choose == 1) {
		printf("When you want to finish entering words press 0\n");
		char word[20] = { 0 };
		fp = fopen("words.txt", "w");
		printf("word: ");
		scanf("%s", word);
		while (word[0] != '0') {
			fputs(word, fp);
			putc('\n', fp);
			printf("word: ");
			scanf("%s", word);
		}
		fclose(fp); lin();
	}

	fp = fopen("words.txt", "r");
	c = fgetc(fp);
	if (c == EOF || c == ' ' || c == '\n') {
		printf("Empty file\n");
		lin();
		return 0;
	}
	fseek(fp, 0, SEEK_SET);
	printf("Creating...\n");

	int start_time = clock();

	int bolshoe = 0;
	struct Slovo *stoitword = data;
	while (!feof(fp))
	{
		str = (char*)malloc(size * sizeof(char));
		if (fgets(str, 49, fp))
		{
			index++;
			//data = adding(data, str);
			int jjj = strlen(str);

			if (data == NULL)
            {
                    data = (struct Slovo*)malloc(sizeof(struct Slovo));
                    data->next = NULL;

                    data->element = (char*)malloc(size * sizeof(char));
                    strcpy(data->element, str);
                    data->element = (char*)realloc(str, jjj * sizeof(char*));
                    data->dlin = jjj;
                    NList = data;
            }
            else {
                    data->next = (struct Slovo*)malloc(sizeof(struct Slovo));
                    data = data->next;
                    data->next = NULL;

                    data->element = (char*)malloc(size * sizeof(char));
                    strcpy(data->element, str);
                    data->element = (char*)realloc(str, jjj * sizeof(char*));
                    data->dlin = jjj;
                }

			if ( jjj > bolshoe)
		    {
			    bolshoe = jjj;
			    stoitword = data;
		    }
		}
	}

	//int start_time = clock();


	data = NList;
	if(stoitword != NList)
    {
        while(stoitword != data->next) data=data->next;
        data->next = stoitword->next;
        stoitword->next = NList;
        NList = stoitword;
    }

	obnul(NList);
	stoitword->x = 0;
	stoitword->y = 0;
	stoitword->orient = 0;
	nnn = 1;

	struct Slovo *postavit = NList->next;
	while (postavit != NULL)
        {
            int ii=0, jj=0;
            bool logi = 0;

            int strlout = postavit->dlin;
            int strlsto = NList->dlin;

    while(1)
    {
        while(postavit->element[jj]!=NList->element[ii])
        {
            jj++;
            if(jj+1==strlout)
                {
                    jj=0;
                    ii++;
                    if(ii+1==strlsto)
                        {
                            ii=0;
                            logi = 1;
                            break;
                        }
                }
        }
        if(logi) break;

    postavit->orient = (NList->orient + 1) % 2;//ставим ориентацию и заодно обозначаем, что слово уже стоит
    postavit->peres++;
    if (postavit->orient == 1)//ставим координаты
        {
            postavit->x = NList->x + ii;
            postavit->y = NList->y - jj;
        }
    else
        {
            postavit->x = NList->x - jj;
            postavit->y = NList->y + ii;
        }

        nnn++;
        if(nnn==index)
        {
            peresdat=1;
            stoitword = NList;
            while (stoitword!=NULL)
            {
                stoitword->etalonorient=stoitword->orient;
                stoitword->etalonx=stoitword->x;
                stoitword->etalony=stoitword->y;
                stoitword = stoitword->next;
            }
        }
        else
        {
            stoitword = NList->next;
            while (stoitword != NULL)
                {
                    while (stoitword->orient != 1000)
                      {
                          stoitword = stoitword->next;
                          if(stoitword==NULL)break;
                      }
                    if(stoitword==NULL)break;

                    postanovka(stoitword);

                    nnn=2;
                    obnulone(stoitword);
                    stoitword = stoitword->next;
                }
        }

        nnn=1;
        obnulone(postavit);

        jj++;
            if(jj+1==strlout)
                {
                    jj=0;
                    ii++;
                    if(ii+1==strlsto)
                        {
                            ii=0;
                            break;
                        }
                }
    }
            postavit = postavit->next;
        }





	//int end_time = clock();

	if (peresdat)
	{
		data = NList;
		while (data != NULL)
	    {
		  data->etalonx = data->etalonx - mi_x;
		  data->etalony = data->etalony - mi_y;
		  data = data->next;
	    }
		struct Slovo *dat = NList;
		printf("Scheme: \n");
		hatch = 176;
		filling(field, hatch);//Заполняем поле "штриховкой"
		frame(field);//Рисуем рамку
		for (int i = 0; i < index; i++) {
			line(field, dat->etalony, dat->etalonx, dat->dlin - 1, dat->etalonorient, hatch);
			dat = dat->next;
		}
		print(field);
		dat = NList;
		for (int i = 0; i < index; i++) {
			line_with_word(field, NList->element, NList->etalony, NList->etalonx, NList->etalonorient);
			NList = NList->next;
		}

		int end_time = clock();

		printf("\nComleted scheme:\n");
		print(field);
		lin();
		unsigned int search_time = end_time - start_time;
		printf("Time: %u millisconds\n", search_time);
	}
	else
		printf("It is impossible to make a crossword with these words\n");
	system("pause");
	return 0;
}
