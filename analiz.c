#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


char alinanMetin[10000];
char duzgunMetin[10000];
int aImlec = 0;
int dImlec = 0;
int hata[10];
int parantez[10000];
int fazlaCiftTirnakVarMi = 0;

// dizileri ve degiskenleri temizler
int degerTemizle(){
	int i;
	for (i = 0; i < 10000; i++)
	{
		alinanMetin[i] = '\0';
	}
	for (i = 0; i < 10000; i++)
	{
		duzgunMetin[i] = '\0';
	}
	for (i = 0; i < 10; i++)
	{
		hata[i] = '\0';
	}
	for (i = 0; i < 10000; i++)
	{
		parantez[i] = '\0';
	}
	aImlec = 0;
	dImlec = 0;
	fazlaCiftTirnakVarMi = 0;
}
///////////////////////////////////////////////////////////////////////

// degiskenleri sifirlar, ekrani temizler, menuyu yazar.
int anaMenu(){
	degerTemizle();
	system("cls || clear");
	printf("1\t->Konsoldan oku\n2\t->Dosyadan oku\n3\t->Cikis\n\n");

	switch (getchar())
	{
	case '1':
		printf("Metni giriniz:");
		konsoldanOku();
		break;
	case '2':
		dosyadanOku();
		break;
	case '3':
		return 0;
	default:
		anaMenu();
		break;
	}

}
////////////////////////////////////////////////////////////////////////////


// Konsoldan Okur ve alinanMetin dizisine yazar.
int konsoldanOku(){
	char enter = '0';
	int k = 0;
	gets(alinanMetin);
	while (enter != '\r' && enter != '\n') {
		enter = getchar();
		if (enter == '\n')
		{
			break;
		}
		alinanMetin[k] = enter;
		k++;


	}
	metniAldim();
}
//////////////////////////////////////////////////////////////////////////

// Dosyadan Okur ve alinanMetin dizisine yazar.
int dosyadanOku(){
	FILE *fptr;

	if ((fptr = fopen("oku.txt", "r")) == NULL)
	{
		printf("Dosya Okunamadi!");
		exit(1);
	}

	fscanf(fptr, "%[^\n]", alinanMetin);
	fclose(fptr);
	metniAldim();

	return 0;
}
//////////////////////////////////////////////////////////////////////////

// Alinan Metni Goster ve Secenek Sun
int metniAldim(){
	system("cls||clear");
	int i;
	for (i = 0; i < strlen(alinanMetin); i++)
	{
		printf("%c", alinanMetin[i]);
	}

	printf("\n1\t->Incele ve Hatalari Duzelt\n2\t->Incele, Duzelt ve Hatalari Raporla (HTML)\n");
	parantezIsle();
	int secenek;
	scanf("%d", &secenek);
	switch (secenek)
	{
	case 1:
		system("cls||clear");
		inceleVeDuzelt();

		raporla(1);
		break;
	case 2:
		system("cls||clear");
		inceleVeDuzelt();

		raporla(1);
		raporla(2);
	default:
		break;
	}
}
//////////////////////////////////////////////////////////////////////////

//#####################################################INCELEME
//!!!!!!!!!!!!!HATALAR
//hata[0]	-> Buyuk Harf Hatasi
//hata[1]	-> Fazla Bosluk Hatasi
//hata[2]	-> Bosluk Yok Hatasi
//hata[3]	-> Parantez Hatasi
//hata[4]	-> Cift Tirnak Hatasi
//hata[5]	-> Cumle Sonu Hatasi
//\!!!!!!!!!!!!!!!!!!!!

// Parantezlerin yazilip yazilmayacagini belirle
int parantezIsle(){
	int i, j;
	int tirnakSay = 0;
	for (i = 0; i < 10000; i++)
	{
		if (alinanMetin[i] == '(')
		{
			for (j = i + 1; j < 10000 - i; j++)
			{
				if (alinanMetin[j] == ')'&& parantez[j] != 1)
				{
					parantez[i] = 1;
					parantez[j] = 1;
					break;
				}
			}
		}
	}

	for (i = 0; i < 10000; i++)
	{
		if (alinanMetin[i] == '"')
		{
			tirnakSay++;
		}
	}
	if (tirnakSay % 2 == 0)
	{
		fazlaCiftTirnakVarMi = 0;
	}
	else
	{
		fazlaCiftTirnakVarMi = 1;
	}
}
///////////////////////////////////////////////////////////////

// Buyuk Harf Inceleme
int buyukHarfIncele(int x){
	if (alinanMetin[x] == '"' || alinanMetin[x] == '(' || alinanMetin[x] == ')')
	{
		if (alinanMetin[x] == '(' || alinanMetin[x] == ')')
		{
			if (parantezIncele(x) == 1)
			{
				duzgunMetin[dImlec] = alinanMetin[x];
				dImlec++;
			}
		}

		if (alinanMetin[x] == '"')
		{
			if (ciftTirnakIncele() == 1)
			{
				duzgunMetin[dImlec] = alinanMetin[x];
				dImlec++;
			}
		}


		buyukHarfIncele(x + 1);
	}
	else if (isupper(alinanMetin[x]))
	{
		duzgunMetin[dImlec] = alinanMetin[x];
		aImlec++;
		dImlec++;
		return aImlec;	// bir sonraki karaktere gec
	}
	else
	{
		hata[0]++;	// Buyuk harf hatasi
		duzgunMetin[dImlec] = toupper(alinanMetin[x]); //metni buyut
		aImlec++;
		dImlec++;
		return aImlec; // bir sonraki karaktere gec
	}
}
//////////////////////////////////////////////////////////////////////////////////////

// Nokta, Soru Isareti ve Unlem Incele
int noktaIncele(int x){
	if (isspace(alinanMetin[x + 1]))
	{
		aImlec = x + 1;
		aImlec = boslukIncele(aImlec); // sonraki imlecte 1 bosluk olsun, fazlasi varsa silinsin, imlec bosluktan sonrakinde olsun, imlec degeri donsun.
		return buyukHarfIncele(aImlec);	// Bosluktan sonraki harf buyuk olsun ve imlec buyuk harften sonrakinde olsun.
	}
	else
	{
		aImlec = boslukIncele(aImlec);// sonraki imlecte 1 bosluk olsun ve imlec boslukta olsun.
		aImlec = buyukHarfIncele(aImlec);// Bosluktan sonraki harf buyuk olsun ve imlec buyuk harften sonrakinde olsun.
		return aImlec;
	}
}



int soruIsaretiIncele(int x){
	if (isspace(alinanMetin[x + 1]))
	{
		aImlec = x + 1;
		aImlec = boslukIncele(aImlec); // sonraki imlecte 1 bosluk olsun, fazlasi varsa silinsin, imlec bosluktan sonrakinde olsun, imlec degeri donsun.
		return buyukHarfIncele(aImlec);	// Bosluktan sonraki harf buyuk olsun ve imlec buyuk harften sonrakinde olsun.
	}
	else
	{
		aImlec = boslukIncele(aImlec);// sonraki imlecte 1 bosluk olsun ve imlec boslukta olsun.
		aImlec = buyukHarfIncele(aImlec);// Bosluktan sonraki harf buyuk olsun ve imlec buyuk harften sonrakinde olsun.
		return aImlec;
	}
}




int unlemIncele(int x){
	if (isspace(alinanMetin[x + 1]))
	{
		aImlec = x + 1;
		aImlec = boslukIncele(aImlec); // sonraki imlecte 1 bosluk olsun, fazlasi varsa silinsin, imlec bosluktan sonrakinde olsun, imlec degeri donsun.
		return buyukHarfIncele(aImlec);	// Bosluktan sonraki harf buyuk olsun ve imlec buyuk harften sonrakinde olsun.
	}
	else
	{
		aImlec = boslukIncele(aImlec);// sonraki imlecte 1 bosluk olsun ve imlec boslukta olsun.
		aImlec = buyukHarfIncele(aImlec);// Bosluktan sonraki harf buyuk olsun ve imlec buyuk harften sonrakinde olsun.
		return aImlec;
	}
}
/////////////////////////////////////////////////////////////////////////

// Virgul Incele
int virgulIncele(int x){
	if (isspace(alinanMetin[x + 1]))
	{
		aImlec = x + 1;
		boslukIncele(aImlec); // sonraki imlecte 1 bosluk olsun, fazlasi varsa silinsin ve imlec boslukta olsun.
	}
	else
	{
		boslukIncele(aImlec);// sonraki imlecte 1 bosluk olsun ve imlec boslukta olsun.
	}
}
////////////////////////////////////////////////////////////////////////

// Birden Fazla Bosluk Incele
int boslukIncele(int x){
	if (isspace(alinanMetin[x]))
	{
		if (isspace(alinanMetin[x + 1]))
		{
			aImlec++; //imleci arttir
			hata[1]++;	// Fazla bosluk hatasi
			boslukIncele(aImlec);//Fazla bosluk algilandi bir sonrakini kontrol et.
		}
		else
		{
			duzgunMetin[dImlec] = alinanMetin[aImlec];
			dImlec++;// Duzgun imleci artir
			aImlec++; //Alinan imleci artir
			return aImlec;
		}
	}
	else
	{
		hata[2]++;	// Bosluk yok hatasi
		duzgunMetin[dImlec] = ' ';
		dImlec++;
		aImlec++;
		return aImlec;
	}
}
////////////////////////////////////////////////////////////////////////

// Parantez ve Cift Tirnak Incele
int parantezIncele(int x){
	if (parantez[x] == 1)
	{
		duzgunMetin[dImlec] = alinanMetin[aImlec];
		dImlec++;
		aImlec++;
	}
	else
	{
		hata[3]++;
		aImlec++;
	}
}
int ciftTirnakIncele(){
	int i;
	for (i = aImlec + 1; i < 10000; i++)
	{
		if (alinanMetin[i] == '"')
		{
			duzgunMetin[dImlec] = alinanMetin[aImlec];
			aImlec++;
			dImlec++;
			return 0;
		}

	}
	if (fazlaCiftTirnakVarMi == 1)
	{
		hata[4]++;	// Cift tirnak hatasi
		aImlec++;
	}
	else
	{
		duzgunMetin[dImlec] = alinanMetin[aImlec];
		aImlec++;
		dImlec++;
	}
}
///////////////////////////////////////////////////////////////////////

//\#############################################################

// Inceleyip bulunan hatalari duzeltir ve ekrana basar.
int inceleVeDuzelt(){
	int i;
	for (i = 0; i < strlen(alinanMetin); i = aImlec)
	{
		if (i == 0)
		{
			if (isupper(alinanMetin[0]))
			{
				duzgunMetin[dImlec] = alinanMetin[aImlec];
				dImlec++;
				aImlec++;
			}
			else
			{
				hata[0]++;	// Buyuk harf hatasi
				duzgunMetin[dImlec] = toupper(alinanMetin[aImlec]);
				dImlec++;
				aImlec++;
			}
		}
		else if (aImlec == strlen(alinanMetin) - 1)
		{
			if (alinanMetin[aImlec] == '.' || alinanMetin[aImlec] == '!' || alinanMetin[aImlec] == '?')
			{
				duzgunMetin[dImlec] = alinanMetin[aImlec];
				return 0;
			}
			else if (alinanMetin[i] == '"')
			{
				ciftTirnakIncele();
				hata[5]++;	//Cumle sonu noktalamasiz bitmis hatasi
				duzgunMetin[dImlec] = '.';
			}
			else if (alinanMetin[i] == '(' || alinanMetin[i] == ')')
			{
				parantezIncele(aImlec);
				hata[5]++;	//Cumle sonu noktalamasiz bitmis hatasi
				duzgunMetin[dImlec] = '.';
			}
			else
			{
				duzgunMetin[dImlec] = alinanMetin[aImlec];
				aImlec++;
				dImlec++;
				hata[5]++;	//Cumle sonu noktalamasiz bitmis hatasi
				duzgunMetin[dImlec] = '.';

				return 0;
			}
		}
		else if (alinanMetin[i] == '"')
		{
			ciftTirnakIncele();
		}
		else if (alinanMetin[i] == '.')
		{
			duzgunMetin[dImlec] = alinanMetin[aImlec];
			dImlec++;
			aImlec = noktaIncele(i);
		}
		else if (alinanMetin[i] == '?')
		{
			duzgunMetin[dImlec] = alinanMetin[aImlec];
			dImlec++;
			aImlec = soruIsaretiIncele(i);
		}
		else if (alinanMetin[i] == '!')
		{
			duzgunMetin[dImlec] = alinanMetin[aImlec];
			dImlec++;
			aImlec = unlemIncele(i);
		}
		else if (alinanMetin[i] == ',' || alinanMetin[i] == ';')
		{
			duzgunMetin[dImlec] = alinanMetin[aImlec];
			dImlec++;
			aImlec = virgulIncele(i);
		}
		else if (alinanMetin[i] == ' ')
		{
			aImlec = boslukIncele(i);
		}
		else if (alinanMetin[i] == '(' || alinanMetin[i] == ')')
		{
			parantezIncele(aImlec);
		}
		else
		{
			duzgunMetin[dImlec] = alinanMetin[aImlec];
			dImlec++;
			aImlec++;
		}


	}
	return 1;
}
/////////////////////////////////////////////////////////////////////////

int raporla(int x){

	if (x == 1)	//Ekrana bas.
	{
		int space = 0;
		int i;
		for (i = 0; i < strlen(duzgunMetin); i++)
		{
			printf("%c", duzgunMetin[i]);
			if (duzgunMetin[i] == ' ')
			{
				space++;
			}
		}
		printf("\n\n\nKarakter Sayisi: %d\n", strlen(duzgunMetin));
		printf("Kelime Sayisi: %d\n\n", space + 1);
		printf("\n--------------##HATALAR##-----------------\n");
		printf("Buyuk Harf Hatasi:\t%d\n", hata[0]);
		printf("Fazla Bosluk Hatasi:\t%d\n", hata[1]);
		printf("Bosluk Yok Hatasi:\t%d\n", hata[2]);
		printf("Parantez Hatasi:\t%d\n", hata[3]);
		printf("Cift Tirnak Hatasi:\t%d\n", hata[4]);
		printf("Cumle Sonu Hatasi:\t%d\n", hata[5]);


	}
	if (x == 2)	//HTML Raporu olustur.
	{
		FILE *fp;
		fp = fopen("analiz-incelemeraporu.html", "w+");
		fprintf(fp, "<!doctype html>		<html>		<head>		<meta charset = \"utf-8\">		<title>Analiz - Inceleme Raporu</title>		<style>			p{		font - size:12px;		font - weight:bold;	opacity:0.7;	}p:hover{opacity:1;}  i{  color:red;}  </style>	  </head>	  <body><h3>%s</h3>	  	  <p style = \"font-size:18px;margin:0px;color:red;opacity:1;\">Hatalar</p>	  <hr>	  <p>&THORN; Buyuk Harf Hatasi : &ensp; <i>%d</i></p>	  <p>&THORN; Fazla Bosluk Hatasi : &ensp; <i>%d</i></p>	  <p>&THORN; Bosluk Yok Hatasi : &ensp; <i>%d</i></p>	  <p>&THORN; Parantez Hatasi : &ensp; <i>%d</i></p>	  <p>&THORN; Cift Tirnak Hatasi : &ensp; <i>%d</i></p>	  <p>&THORN; Cumle Sonu Hatasi : &ensp; <i>%d</i></p>	  	  </body>	  </html>	  ", duzgunMetin, hata[0], hata[1], hata[2], hata[3], hata[4], hata[5]);
		fclose(fp);
		printf("\n\aHTML Raporu Olusturuldu.\n");
	}
	return 0;
}



int main(){
	anaMenu();

	return 0;
}