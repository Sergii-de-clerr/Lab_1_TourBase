#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <clocale>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <ctime>
#include <stdio.h>
#include <string.h>
#include <map>

#pragma warning(disable : 4996)
const int l_P_I_B = 32;
const int const_Is_Del = 16;

using namespace std;

struct Tourists
{
	int Tourist_code;
	char P_I_B[l_P_I_B];
	int Pasport_code;
	int Tour_code;
	int City_code;
	int PtrFirstVoucher = -1;
};

struct Voucher
{
	int Voucher_code;
	int Tour_code;
	int Hotel_code;
	int Tourist_code;
	int PtrNextVoucher = -1;
};

Tourists ReadTourists(int i_n)
{
	FILE* out;
	Tourists Tr;
	out = fopen("master_file.txt", " r ");
	fseek(out, (i_n * sizeof(Tourists)) + const_Is_Del, SEEK_SET);
	fread(&Tr, sizeof(Tourists), 1, out);
	fclose(out);
	return Tr;
}

void WriteTourists(int i_n, Tourists tr)
{
	FILE* out;
	out = fopen("master_file.txt", " r+ ");
	fseek(out, (i_n * sizeof(Tourists)) + const_Is_Del, SEEK_SET);
	fwrite(&tr, sizeof(Tourists), 1, out);
	fclose(out);
}

void DeleteTourists(int i_n)
{
	unsigned char Is_Delete[const_Is_Del];
	FILE* out;
	out = fopen("master_file.txt", " r+ ");
	fread(&Is_Delete, const_Is_Del, 1, out);
	Is_Delete[i_n / 8] ^= (1 << (7 - i_n % 8));
	fseek(out, 0, SEEK_SET);
	fwrite(&Is_Delete, const_Is_Del, 1, out);
	fclose(out);
}

int AddTourists(Tourists tr)
{
	unsigned char Is_Delete[const_Is_Del];
	int schet = 0;

	FILE* out;
	out = fopen("master_file.txt", " r+ ");
	fread(&Is_Delete, const_Is_Del, 1, out);
	//Is_Delete[i_n / 8] ^= (1 << (7 - i_n % 8));
	int i = 0;

	for (; i < 16; i++)
	{
		if (Is_Delete[i] != 0)
		{
			break;
		}
		schet += 8;
	}

	if (schet != 128)
	{
		for (int j = 0; j < 8; j++)
		{
			schet++;
			if (Is_Delete[i] >> (7 - j) & 1)
			{
				Is_Delete[i] ^= (1 << (7 - j));
				break;
			}
		}
	}
	else
	{
		schet = 0;
	}
	schet--;

	fseek(out, 0, SEEK_SET);
	fwrite(&Is_Delete, const_Is_Del, 1, out);
	fclose(out);
	return schet;
}

Voucher ReadVoucher(int i_n)
{
	FILE* out;
	Voucher Vc;
	out = fopen("slave_file.txt", " r ");
	fseek(out, (i_n * sizeof(Voucher)) + const_Is_Del, SEEK_SET);
	fread(&Vc, sizeof(Voucher), 1, out);
	fclose(out);
	return Vc;
}

void WriteVoucher(int i_n, Voucher vc)
{
	FILE* out;
	out = fopen("slave_file.txt", " r+ ");
	fseek(out, (i_n * sizeof(Voucher)) + const_Is_Del, SEEK_SET);
	fwrite(&vc, sizeof(Voucher), 1, out);
	fclose(out);
}

void DeleteVoucher(int i_n)
{
	unsigned char Is_Delete[const_Is_Del];
	FILE* out;
	out = fopen("slave_file.txt", " r+ ");
	fread(&Is_Delete, const_Is_Del, 1, out);
	Is_Delete[i_n / 8] ^= (1 << (7 - i_n % 8));
	fseek(out, 0, SEEK_SET);
	fwrite(&Is_Delete, const_Is_Del, 1, out);
	fclose(out);
}

int AddVoucher(Voucher vc)
{
	unsigned char Is_Delete[const_Is_Del];
	int schet = 0;

	FILE* out;
	out = fopen("slave_file.txt", " r+ ");
	fread(&Is_Delete, const_Is_Del, 1, out);
	//Is_Delete[i_n / 8] ^= (1 << (7 - i_n % 8));
	int i = 0;

	for (; i < 16; i++)
	{
		if (Is_Delete[i] != 0)
		{
			break;
		}
		schet += 8;
	}

	if (schet != 128)
	{
		for (int j = 0; j < 8; j++)
		{
			schet++;
			if (Is_Delete[i] >> (7 - j) & 1)
			{
				Is_Delete[i] ^= (1 << (7 - j));
				break;
			}
		}
	}
	else
	{
		schet = 0;
	}
	schet--;

	fseek(out, 0, SEEK_SET);
	fwrite(&Is_Delete, const_Is_Del, 1, out);
	fclose(out);
	return schet;
}

//void setPtrVoucher(
//	Tourists T_Array[], 
//	map<int, int> Id_Tourists, 
//	Voucher V_Array[], 
//	map<int, int> Id_Voucher, 
//	Voucher New)
//{
//	Voucher Vc;
//	int i_n = 0;
//	if (T_Array[Id_Tourists[New.Tourist_code]].PtrFirstVoucher == -1)
//		T_Array[Id_Tourists[New.Tourist_code]].PtrFirstVoucher = Id_Voucher[New.Voucher_code];
//	else
//	{
//		Vc = V_Array[T_Array[Id_Tourists[New.Tourist_code]].PtrFirstVoucher];
//		while (Vc.PtrNextVoucher != -1)
//			Vc = V_Array[Vc.PtrNextVoucher];
//		V_Array[Id_Voucher[Vc.Voucher_code]].PtrNextVoucher = Id_Voucher[New.Voucher_code];
//	}
//}

void setPtrVoucher(
	map<int, int> Id_Tourists,
	map<int, int> Id_Voucher,
	Voucher New)
{
	if (Id_Tourists.count(New.Tourist_code) == 0)
		return;

	Tourists Tr = ReadTourists(Id_Tourists[New.Tourist_code]);
	Voucher Vc;
	int i_n = 0;

	if (Tr.PtrFirstVoucher == -1)
	{
		Tr.PtrFirstVoucher = Id_Voucher[New.Voucher_code];
		WriteTourists(Id_Tourists[New.Tourist_code], Tr);
	}
	else
	{
		Vc = ReadVoucher(Tr.PtrFirstVoucher);
		while (Vc.PtrNextVoucher != -1)
			Vc = ReadVoucher(Vc.PtrNextVoucher);
		Vc.PtrNextVoucher = Id_Voucher[New.Voucher_code];
		WriteVoucher(Id_Voucher[Vc.Voucher_code], Vc);
	}
}

void delPtrVoucher(
	map<int, int> Id_Tourists,
	map<int, int> Id_Voucher,
	Voucher New)
{
	if (Id_Tourists.count(New.Tourist_code) == 0)
		return;

	Tourists Tr = ReadTourists(Id_Tourists[New.Tourist_code]);
	Voucher Vc;
	int i_n = 0;

	if (Tr.PtrFirstVoucher == Id_Voucher[New.Voucher_code])
	{
		Tr.PtrFirstVoucher = New.PtrNextVoucher;
		WriteTourists(Id_Tourists[New.Tourist_code], Tr);
	}
	else
	{
		Vc = ReadVoucher(Tr.PtrFirstVoucher);
		while (Vc.PtrNextVoucher != Id_Voucher[New.Voucher_code])
		{
			Vc = ReadVoucher(Vc.PtrNextVoucher);
		}
		Vc.PtrNextVoucher = New.PtrNextVoucher;
		WriteVoucher(Id_Voucher[Vc.Voucher_code], Vc);
	}
}

void default_i()
{
	Tourists T_Array[50];
	Voucher V_Array[50];
	map<int, int> Id_Pasp_Tourists;
	map<int, int> Id_Tourists;
	map<int, int> Id_Voucher;
	unsigned char Is_Delete[const_Is_Del];
	FILE* out;

	T_Array[0].Tourist_code = 1; strcpy(T_Array[0].P_I_B, "Назарчук Сергій Олегович");		T_Array[0].Pasport_code = 9654;
	T_Array[0].Tour_code = 7;	T_Array[0].City_code = 1;
	T_Array[1].Tourist_code = 2; strcpy(T_Array[1].P_I_B, "Симоненко Василь Андрійович");	T_Array[1].Pasport_code = 2057;
	T_Array[1].Tour_code = 1; 	T_Array[1].City_code = 3;
	T_Array[2].Tourist_code = 3; strcpy(T_Array[2].P_I_B, "Шевченко Тарас Григорович");		T_Array[2].Pasport_code = 3214;
	T_Array[2].Tour_code = 5; 	T_Array[2].City_code = 6;
	T_Array[3].Tourist_code = 4; strcpy(T_Array[3].P_I_B, "Стасюк Констянтин Русланович"); 	T_Array[3].Pasport_code = 5296;
	T_Array[3].Tour_code = 4; 	T_Array[3].City_code = 8;
	T_Array[4].Tourist_code = 5; strcpy(T_Array[4].P_I_B, "Недзельський Ярослав Петрович"); T_Array[4].Pasport_code = 8251;
	T_Array[4].Tour_code = 9; 	T_Array[4].City_code = 2;


	for (int i = 0; i < 5; i++)
		Id_Pasp_Tourists[T_Array[i].Pasport_code] = i;

	out = fopen("id_pasport_master_file.txt", " w ");

	for (auto f : Id_Pasp_Tourists)
	{
		fwrite(&f.first, sizeof(int), 1, out);
		fwrite(&f.second, sizeof(int), 1, out);
	}

	fclose(out);

	for (int i = 0; i < 5; i++)
		Id_Tourists[T_Array[i].Tourist_code] = i;

	out = fopen("id_master_file.txt", " w ");

	for (auto f : Id_Tourists)
	{
		fwrite(&f.first, sizeof(int), 1, out);
		fwrite(&f.second, sizeof(int), 1, out);
	}

	fclose(out);

	V_Array[0].Voucher_code = 1; 	V_Array[0].Tour_code = 1; 	V_Array[0].Hotel_code = 5; V_Array[0].Tourist_code = 1;
	Id_Voucher[V_Array[0].Voucher_code] = 0;
	V_Array[1].Voucher_code = 2; 	V_Array[1].Tour_code = 1; 	V_Array[1].Hotel_code = 5; V_Array[1].Tourist_code = 2;
	Id_Voucher[V_Array[1].Voucher_code] = 1;
	V_Array[2].Voucher_code = 3; 	V_Array[2].Tour_code = 1; 	V_Array[2].Hotel_code = 5; V_Array[2].Tourist_code = 4;
	Id_Voucher[V_Array[2].Voucher_code] = 2;
	V_Array[3].Voucher_code = 4; 	V_Array[3].Tour_code = 1; 	V_Array[3].Hotel_code = 5; 	V_Array[3].Tourist_code = 4;
	Id_Voucher[V_Array[3].Voucher_code] = 3;

	memset(Is_Delete, 0, sizeof(Is_Delete));


	out = fopen("master_file.txt", " w ");

	fwrite(Is_Delete, sizeof(unsigned char), const_Is_Del, out);

	for (int i = 0; i < 5; i++)
		fwrite(&T_Array[i], sizeof(Tourists), 1, out);

	fclose(out);

	memset(Is_Delete, 0, sizeof(Is_Delete));

	out = fopen("slave_file.txt", " w ");

	fwrite(Is_Delete, sizeof(unsigned char), const_Is_Del, out);

	for (int i = 0; i < 4; i++)
		fwrite(&V_Array[i], sizeof(Voucher), 1, out);

	fclose(out);

	out = fopen("id_slave_file.txt", " w ");

	for (auto f : Id_Voucher)
	{
		fwrite(&f.first, sizeof(int), 1, out);
		fwrite(&f.second, sizeof(int), 1, out);
	}

	fclose(out);

	setPtrVoucher(Id_Tourists, Id_Voucher, V_Array[0]);
	setPtrVoucher(Id_Tourists, Id_Voucher, V_Array[1]);
	setPtrVoucher(Id_Tourists, Id_Voucher, V_Array[2]);
	setPtrVoucher(Id_Tourists, Id_Voucher, V_Array[3]);
}

void ut_m(map<int, int>& Id_Tourists);
void ut_s(map<int, int>& Id_Voucher);

void default_maps(
	map<int, int>& Id_Tourists,
	map<int, int>& Id_Pasp_Tourists,
	map<int, int>& Id_Voucher,
	int& max_t, int& max_v)
{
	FILE* out;

	out = fopen("id_master_file.txt", " r ");

	//fread(&first, sizeof(int), 1, out);

	for (int i = 0; i < 5; i++)
	{
		int first = 0;
		fread(&first, sizeof(int), 1, out);
		int second = 0;
		fread(&second, sizeof(int), 1, out);
		Id_Tourists[first] = second;
	}

	fclose(out);

	out = fopen("id_pasport_master_file.txt", " r ");

	for (int i = 0; i < 5; i++)
	{
		int first = 0;
		fread(&first, sizeof(int), 1, out);
		int second = 0;
		fread(&second, sizeof(int), 1, out);
		Id_Pasp_Tourists[first] = second;
	}

	fclose(out);

	out = fopen("id_slave_file.txt", " r ");

	for (int i = 0; i < 4; i++)
	{
		int first = 0;
		fread(&first, sizeof(int), 1, out);
		int second = 0;
		fread(&second, sizeof(int), 1, out);
		Id_Voucher[first] = second;
	}

	fclose(out);
}

void get_m(
	map<int, int>& Id_Tourists,
	map<int, int>& Id_Pasp_Tourists, 
	map<int, int>& Id_Voucher)
{
	int p_c = 0, i_n = 0;
	printf("Введіть код паспорту: ");
	scanf("%d", &p_c);

	FILE* out;
	Tourists Tr;

	if (Id_Pasp_Tourists.count(p_c))
		i_n = Id_Pasp_Tourists[p_c];
	else
	{
		printf("Туриста з таким паспортним кодом не існує\n");
		return;
	}

	out = fopen("master_file.txt", " r ");
	fseek(out, (i_n * sizeof(Tourists)) + const_Is_Del, SEEK_SET);
	fread(&Tr, sizeof(Tourists), 1, out);
	fclose(out);

	printf(".___________.______________________________.____________.________._________.\n");
	printf("|Код туриста|             П.І.Б.           |Код Паспорта|Код туру|Код міста|\n");
	printf("|===========|==============================|============|========|=========|\n");

	printf("|%11d|%30s|%12d|%8d|%9d|\n",
		Tr.Tourist_code,
		Tr.P_I_B,
		Tr.Pasport_code,
		Tr.Tour_code,
		Tr.City_code);

	printf(".___________.______________________________.____________.________._________.\n");

	if (Tr.PtrFirstVoucher == -1)
	{
		printf("У туриста %s немає путівок\n", Tr.P_I_B);
		return;
	}

	Voucher Vc;
	out = fopen("slave_file.txt", " r ");
	int tmp = Tr.PtrFirstVoucher;

	printf("\n");
	printf(".____________.____________.____________.____________.\n");
	printf("|Код  ваучеру|  Код туру  | Код готеля |Код  туриста|\n");
	printf("|============|============|============|============|\n");

	while (tmp != -1)
	{
		fseek(out, (tmp * sizeof(Voucher)) + const_Is_Del, SEEK_SET);
		fread(&Vc, sizeof(Voucher), 1, out);
		printf("|%12d|%12d|%12d|%12d|\n",
			Vc.Voucher_code,
			Vc.Tour_code,
			Vc.Hotel_code,
			Vc.Tourist_code);
		tmp = Vc.PtrNextVoucher;
	}

	printf(".____________.____________.____________.____________.\n");

	fclose(out);
}

void get_s(
	map<int, int>& Id_Tourists,
	map<int, int>& Id_Pasp_Tourists,
	map<int, int>& Id_Voucher)
{
	int v_c = 0, i_n = 0;
	printf("Введіть код ваучеру: ");
	scanf("%d", &v_c);

	FILE* out;
	Voucher Vc;

	if (Id_Voucher.count(v_c))
		i_n = Id_Voucher[v_c];
	else
	{
		printf("Ваучера з таким кодом не існує\n");
		return;
	}

	out = fopen("slave_file.txt", " r ");
	fseek(out, (i_n * sizeof(Voucher)) + const_Is_Del, SEEK_SET);
	fread(&Vc, sizeof(Voucher), 1, out);
	fclose(out);

	printf("\n");
	printf(".____________.____________.____________.____________.\n");
	printf("|Код  ваучеру|  Код туру  | Код готеля |Код  туриста|\n");
	printf("|============|============|============|============|\n");

	fread(&Vc, sizeof(Voucher), 1, out);
	printf("|%12d|%12d|%12d|%12d|\n",
		Vc.Voucher_code,
		Vc.Tour_code,
		Vc.Hotel_code,
		Vc.Tourist_code);

	printf(".____________.____________.____________.____________.\n");

	Tourists Tr;

	if (Id_Tourists.count(Vc.Tourist_code))
		i_n = Id_Tourists[Vc.Tourist_code];

	out = fopen("master_file.txt", " r ");
	fseek(out, (i_n * sizeof(Tourists)) + const_Is_Del, SEEK_SET);
	fread(&Tr, sizeof(Tourists), 1, out);
	fclose(out);

	printf(".___________.______________________________.____________.________._________.\n");
	printf("|Код туриста|             П.І.Б.           |Код Паспорта|Код туру|Код міста|\n");
	printf("|===========|==============================|============|========|=========|\n");

	printf("|%11d|%30s|%12d|%8d|%9d|\n",
		Tr.Tourist_code,
		Tr.P_I_B,
		Tr.Pasport_code,
		Tr.Tour_code,
		Tr.City_code);

	printf(".___________.______________________________.____________.________._________.\n");
}

void delete_m(
	map<int, int>& Id_Tourists,
	map<int, int>& Id_Pasp_Tourists,
	map<int, int>& Id_Voucher)
{
	FILE* out;

	Tourists tr;
	int i_n = 0;
	printf("Введіть код туриста, якого хочете видалити: ");
	scanf("%d", &tr.Tourist_code);

	if (Id_Tourists.count(tr.Tourist_code))
		i_n = Id_Tourists[tr.Tourist_code];
	else
	{
		printf("Туриста з таким кодом не існує\n");
		return;
	}

	tr = ReadTourists(i_n);
	Voucher vc;

	while (tr.PtrFirstVoucher != -1)
	{
		vc = ReadVoucher(tr.PtrFirstVoucher);
		delPtrVoucher(Id_Tourists, Id_Voucher, vc);
		DeleteVoucher(Id_Voucher[vc.Voucher_code]);
		Id_Voucher.erase(vc.Voucher_code);
		tr = ReadTourists(i_n);
	}

	DeleteTourists(i_n);

	Id_Tourists.erase(tr.Tourist_code);
	Id_Pasp_Tourists.erase(tr.Pasport_code);
}

void delete_s(
	map<int, int>& Id_Tourists,
	map<int, int>& Id_Pasp_Tourists,
	map<int, int>& Id_Voucher)
{
	FILE* out;

	Voucher vc;
	int i_n = 0;
	printf("Введіть код ваучеру, який хочете видалити: ");
	scanf("%d", &vc.Voucher_code);

	if (Id_Voucher.count(vc.Voucher_code))
		i_n = Id_Voucher[vc.Voucher_code];
	else
	{
		printf("Ваучера з таким кодом не існує\n");
		return;
	}

	delPtrVoucher(Id_Tourists, Id_Voucher, vc);
	DeleteVoucher(i_n);

	Id_Voucher.erase(vc.Voucher_code);
}

void insert_m(
	map<int, int>& Id_Tourists,
	map<int, int>& Id_Pasp_Tourists,
	map<int, int>& Id_Voucher)
{
	FILE* out;

	Tourists tr;
	int i_n = 0;
	printf("Введіть код туриста, якого хочете змінити: ");
	scanf("%d", &tr.Tourist_code);

	if (Id_Tourists.count(tr.Tourist_code))
		i_n = Id_Tourists[tr.Tourist_code];
	else
	{
		printf("Туриста з таким кодом не існує\n");
		return;
	}

	tr = ReadTourists(i_n);

	printf(".___________.______________________________.____________.________._________.\n");
	printf("|Код туриста|             П.І.Б.           |Код Паспорта|Код туру|Код міста|\n");
	printf("|===========|==============================|============|========|=========|\n");

	printf("|%11d|%30s|%12d|%8d|%9d|\n",
		tr.Tourist_code,
		tr.P_I_B,
		tr.Pasport_code,
		tr.Tour_code,
		tr.City_code);
	char name[10], surname[10], pobatkovi[10];

	printf(".___________.______________________________.____________.________._________.\n");

	int pos = Id_Pasp_Tourists[tr.Pasport_code];
	Id_Pasp_Tourists.erase(tr.Pasport_code);

	printf("Введіть П.І.Б. туриста: ");
	scanf("%s %s %s", name, surname, pobatkovi);
	strcpy(tr.P_I_B, name);
	strcat(tr.P_I_B, " ");
	strcat(tr.P_I_B, surname);
	strcat(tr.P_I_B, " ");
	strcat(tr.P_I_B, pobatkovi);
	//strcpy(tr.P_I_B, strcat(strcat(name, " "), strcat(strcat(surname, " "), pobatkovi)));
	printf("Введіть код паспорта туриста: ");
	scanf("%d", &tr.Pasport_code);
	printf("Введіть код туру туриста: ");
	scanf("%d", &tr.Tour_code);
	printf("Введіть код міста туриста: ");
	scanf("%d", &tr.City_code);

	Id_Pasp_Tourists[tr.Pasport_code] = pos;

	printf(".___________.______________________________.____________.________._________.\n");
	printf("|Код туриста|             П.І.Б.           |Код Паспорта|Код туру|Код міста|\n");
	printf("|===========|==============================|============|========|=========|\n");

	printf("|%11d|%30s|%12d|%8d|%9d|\n",
		tr.Tourist_code,
		tr.P_I_B,
		tr.Pasport_code,
		tr.Tour_code,
		tr.City_code);

	printf(".___________.______________________________.____________.________._________.\n");

	WriteTourists(i_n, tr);
}

void insert_s(
	map<int, int>& Id_Tourists,
	map<int, int>& Id_Pasp_Tourists,
	map<int, int>& Id_Voucher)
{
	FILE* out;

	Voucher vc;
	int i_n = 0;
	printf("Введіть код ваучеру, який хочете змінити: ");
	scanf("%d", &vc.Voucher_code);

	if (Id_Voucher.count(vc.Voucher_code))
		i_n = Id_Voucher[vc.Voucher_code];
	else
	{
		printf("Ваучера з таким кодом не існує\n");
		return;
	}

	vc = ReadVoucher(i_n);

	printf(".____________.____________.____________.____________.\n");
	printf("|Код  ваучеру|  Код туру  | Код готеля |Код  туриста|\n");
	printf("|============|============|============|============|\n");

	printf("|%12d|%12d|%12d|%12d|\n",
		vc.Voucher_code,
		vc.Tour_code,
		vc.Hotel_code,
		vc.Tourist_code);

	printf(".____________.____________.____________.____________.\n");

	delPtrVoucher(Id_Tourists, Id_Voucher, vc);

	printf("Введіть код туру ваучеру: ");
	scanf("%d", &vc.Tour_code);
	printf("Введіть код готелю ваучеру: ");
	scanf("%d", &vc.Hotel_code);
	printf("Введіть код туриста ваучеру: ");
	scanf("%d", &vc.Tourist_code);

	if (Id_Tourists.count(vc.Tourist_code) == 0)
	{
		while (true)
		{
			printf("Туриста з таким кодом не існує. Введіть код туриста ваучеру: ");
			scanf("%d", &vc.Tourist_code);
			if (Id_Tourists.count(vc.Tourist_code) == 1)
			{
				break;
			}
		}
	}

	printf(".____________.____________.____________.____________.\n");
	printf("|Код  ваучеру|  Код туру  | Код готеля |Код  туриста|\n");
	printf("|============|============|============|============|\n");

	printf("|%12d|%12d|%12d|%12d|\n",
		vc.Voucher_code,
		vc.Tour_code,
		vc.Hotel_code,
		vc.Tourist_code);

	printf(".____________.____________.____________.____________.\n");

	vc.PtrNextVoucher = -1;
	setPtrVoucher(Id_Tourists, Id_Voucher, vc);

	WriteVoucher(i_n, vc);
}

void add_m(
	map<int, int>& Id_Tourists,
	map<int, int>& Id_Pasp_Tourists,
	map<int, int>& Id_Voucher, int &max_t)
{
	max_t++;
	FILE* out;

	Tourists tr;

	char name[10], surname[10], pobatkovi[10];

	tr.Tourist_code = max_t;
	printf("Введіть П.І.Б. туриста: ");
	scanf("%s %s %s", name, surname, pobatkovi);
	strcpy(tr.P_I_B, name);
	strcat(tr.P_I_B, " ");
	strcat(tr.P_I_B, surname);
	strcat(tr.P_I_B, " ");
	strcat(tr.P_I_B, pobatkovi);
	printf("Введіть код паспорта туриста: ");
	scanf("%d", &tr.Pasport_code);
	printf("Введіть код туру туриста: ");
	scanf("%d", &tr.Tour_code);
	printf("Введіть код міста туриста: ");
	scanf("%d", &tr.City_code);

	int schet = AddTourists(tr);

	if (schet == -1)
	{
		schet = Id_Tourists.size();
	}

	WriteTourists(schet, tr);
	Id_Tourists[tr.Tourist_code] = schet;
	Id_Pasp_Tourists[tr.Pasport_code] = schet;
}

void add_s(
	map<int, int>& Id_Tourists,
	map<int, int>& Id_Pasp_Tourists,
	map<int, int>& Id_Voucher, int& max_v)
{
	max_v++;
	FILE* out;

	Voucher vc;

	char name[10], surname[10], pobatkovi[10];

	vc.Voucher_code = max_v;
	printf("Введіть код туру ваучеру: ");
	scanf("%d", &vc.Tour_code);
	printf("Введіть код готелю ваучеру: ");
	scanf("%d", &vc.Hotel_code);
	printf("Введіть код туриста ваучеру: ");
	scanf("%d", &vc.Tourist_code);

	int schet = AddVoucher(vc);

	if (schet == -1)
	{
		schet = Id_Voucher.size();
	}

	WriteVoucher(schet, vc);
	Id_Voucher[vc.Voucher_code] = schet;
	setPtrVoucher(Id_Tourists, Id_Voucher, vc);
}

void ut_m(map<int, int>& Id_Tourists)
{
	Tourists Tr;
	FILE* out;
	out = fopen("master_file.txt", " r ");

	printf(".___________.______________________________.____________.________._________.\n");
	printf("|Код туриста|             П.І.Б.           |Код Паспорта|Код туру|Код міста|\n");
	printf("|===========|==============================|============|========|=========|\n");

	for (auto f : Id_Tourists)
	{
		fseek(out, const_Is_Del + f.second * sizeof(Tourists), SEEK_SET);
		fread(&Tr, sizeof(Tourists), 1, out);
		printf("|%11d|%30s|%12d|%8d|%9d|\n",
			Tr.Tourist_code,
			Tr.P_I_B,
			Tr.Pasport_code,
			Tr.Tour_code,
			Tr.City_code);
	}

	printf(".___________.______________________________.____________.________._________.\n");
}

void ut_s(map<int, int>& Id_Voucher)
{
	Voucher Vc;
	FILE* out;
	out = fopen("slave_file.txt", " r ");

	printf(".____________.____________.____________.____________.\n");
	printf("|Код  ваучеру|  Код туру  | Код готеля |Код  туриста|\n");
	printf("|============|============|============|============|\n");

	for (auto f : Id_Voucher)
	{
		fseek(out, const_Is_Del + f.second * sizeof(Voucher), SEEK_SET);
		fread(&Vc, sizeof(Voucher), 1, out);
		printf("|%12d|%12d|%12d|%12d|\n",
			Vc.Voucher_code,
			Vc.Tour_code,
			Vc.Hotel_code,
			Vc.Tourist_code);
	}

	printf(".____________.____________.____________.____________.\n");
}

void menu(
	map<int, int>& Id_Tourists,
	map<int, int>& Id_Pasp_Tourists,
	map<int, int>& Id_Voucher,
	int& max_t, int& max_v)
{
	int i = 0;
	while (true)
	{
		printf("Облік Туристів та Путівок\n\n");
		printf("1.  Читиння заданого запису та заданих підзаписів в таблиці туристів\n");
		printf("2.  Читиння заданого запису та заданих підзаписів в таблиці путівок\n");
		printf("3.  Вилучення заданого запису в таблиці туристів\n");
		printf("4.  Вилучення заданого запису в таблиці путівок\n");
		printf("5.  Оновлення значення заданого поля в таблиці туристів\n");
		printf("6.  Оновлення значення заданого поля в таблиці путівок\n");
		printf("7.  Внесення запису в таблицю туристів\n");
		printf("8.  Внесення запису в таблицю путівок\n");
		printf("9.  Підрахунок кількості записів в таблицю туристів\n");
		printf("10. Підрахунок кількості записів в таблицю путівок\n");
		printf("11. Утиліта для читання всіх полів в таблиці туристів\n");
		printf("12. Утиліта для читання всіх полів в таблиці путівок\n");
		printf("13. Вихід\n\n");
		printf("Введіть номер команди:");
		scanf("%d", &i);
		if (i == 1)
		{
			system("cls");
			get_m(Id_Tourists, Id_Pasp_Tourists, Id_Voucher);
			system("pause");
			system("cls");
		}
		else if (i == 2)
		{
			system("cls");
			get_s(Id_Tourists, Id_Pasp_Tourists, Id_Voucher);
			system("pause");
			system("cls");
		}
		else if (i == 3)
		{
			system("cls");
			delete_m(Id_Tourists, Id_Pasp_Tourists, Id_Voucher);
			system("pause");
			system("cls");
		}
		else if (i == 4)
		{
			system("cls");
			delete_s(Id_Tourists, Id_Pasp_Tourists, Id_Voucher);
			system("pause");
			system("cls");
		}
		else if (i == 5)
		{
			system("cls");
			insert_m(Id_Tourists, Id_Pasp_Tourists, Id_Voucher);
			system("pause");
			system("cls");
		}
		else if (i == 6)
		{
			system("cls");
			insert_s(Id_Tourists, Id_Pasp_Tourists, Id_Voucher);
			system("pause");
			system("cls");
		}
		else if (i == 7)
		{
			system("cls");
			add_m(Id_Tourists, Id_Pasp_Tourists, Id_Voucher, max_t);
			system("pause");
			system("cls");
		}
		else if (i == 8)
		{
			system("cls");
			add_s(Id_Tourists, Id_Pasp_Tourists, Id_Voucher, max_v);
			system("pause");
			system("cls");
		}
		else if (i == 9)
		{
			Tourists tr;
			Voucher vc;
			system("cls");
			printf("Кількість записів в таблиці туристів = %d\n\n", Id_Tourists.size());
			int i;
			for (auto f : Id_Tourists)
			{
				i = 0;
				tr = ReadTourists(f.second);
				if (tr.PtrFirstVoucher != -1)
				{
					tr = ReadTourists(f.second);
					vc = ReadVoucher(tr.PtrFirstVoucher);
					while (vc.PtrNextVoucher != -1)
					{
						vc = ReadVoucher(vc.PtrNextVoucher);
						i++;
					}
					i++;
				}
				printf("У туриста %32s %2d путівок\n", tr.P_I_B, i);
			}
			system("pause");
			system("cls");
		}
		else if (i == 10)
		{
			system("cls");
			printf("Кількість записів в таблиці ваучерів = %d\n", Id_Voucher.size());
			system("pause");
			system("cls");
		}
		else if (i == 11)
		{
			system("cls");
			ut_m(Id_Tourists);
			system("pause");
			system("cls");
		}
		else if (i == 12)
		{
			system("cls");
			ut_s(Id_Voucher);
			system("pause");
			system("cls");
		}
		else if (i == 13)
		{
			system("pause");
			return;
		}
		else
		{
			printf("Такої команди не існує!\n");
			system("pause");
			system("cls");
		}
	}
}

void write_maps(
	map<int, int>& Id_Tourists,
	map<int, int>& Id_Pasp_Tourists,
	map<int, int>& Id_Voucher)
{
	FILE* out;

	out = fopen("id_master_file.txt", " w ");

	for (auto f : Id_Tourists)
	{
		fwrite(&f.first, sizeof(int), 1, out);
		fwrite(&f.second, sizeof(int), 1, out);
	}

	fclose(out);

	out = fopen("id_pasport_master_file.txt", " w ");

	for (auto f : Id_Pasp_Tourists)
	{
		fwrite(&f.first, sizeof(int), 1, out);
		fwrite(&f.second, sizeof(int), 1, out);
	}

	fclose(out);

	out = fopen("id_master_file.txt", " w ");

	for (auto f : Id_Tourists)
	{
		fwrite(&f.first, sizeof(int), 1, out);
		fwrite(&f.second, sizeof(int), 1, out);
	}

	fclose(out);
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");
	map<int, int> Id_Tourists;
	map<int, int> Id_Pasp_Tourists;
	map<int, int> Id_Voucher;
	int max_t = 5, max_v = 4;
	default_i();
	default_maps(
		Id_Tourists,
		Id_Pasp_Tourists,
		Id_Voucher,
		max_t, max_v);
	menu(
		Id_Tourists,
		Id_Pasp_Tourists,
		Id_Voucher,
		max_t, max_v);
	write_maps(
		Id_Tourists,
		Id_Pasp_Tourists,
		Id_Voucher);
}