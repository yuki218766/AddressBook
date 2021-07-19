#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define PathLong 40
#define ArrayUnit 10
typedef struct pathUnit
{
	char path[PathLong];
	long int time;
} PathUnit;
typedef struct addressUnit
{
	int number;
	char personalname[30];
	char familyname[30];
	char postalcode[10];
	char address[60];
	char Email[60];
	char phoneNumber[20];
	struct addressUnit *prepointer;
	struct addressUnit *forpointer;

} AddressUnit;

#pragma region プロトタイプ宣言
void error(void);
void PathUnitSwap(PathUnit *A, PathUnit *B);
int ReadValue(char massage[]);
void ReadString(char string[], int maxlong, char massage[]);
int inputNumChecker(int checkNum, int min, int max);
void _trim(char *str);
void Add(PathUnit *pathUnit, int length);
void Delite(PathUnit *pathList, int listlong);
int FileOpen(char filename[]); //セーブをしたら１を帰す

AddressUnit NewAddress(void);
void Save(char FileName[], AddressUnit *addressList, int arraylength, int head);
void printAddress(AddressUnit ad);
void reset(void);
#pragma endregion
char FileList[] = "FileList.csv";
int main(void)
{
	printf("Address Book\n");
	//開くファイルを選択
	PathUnit *pathList;
	int filenum, mode;
	char filepath[PathLong];
	char menustr[2][30] = {"%d Add\n", "%d Delete\n"};
	while (1)
	{
		filenum = 0;
		mode = -1;
		FILE *fp_path;
		if ((fp_path = fopen(FileList, "r")) != NULL) /*パスを保存したファイルの呼び出し*/
		{
			int i, j;
			while (fscanf(fp_path, "%s", filepath) != EOF)
			{
				filenum++;
			}
			fclose(fp_path);
			if (filenum != 0)
			{
				printf("Select the number corresponding to the file\n");
				fp_path = fopen(FileList, "r");
				pathList = (PathUnit *)malloc(filenum * sizeof(PathUnit));
				for (i = 0; i < filenum; i++)
				{
					fscanf(fp_path, "%[^,],%ld,\n",
						   pathList[i].path,
						   &pathList[i].time);
				}
				for (i = 0; i < filenum - 1; i++)
				{
					for (j = 0; j < filenum - 1 - i; j++)
					{
						if (pathList[j].time < pathList[j + 1].time)
						{
							PathUnitSwap(&pathList[j], &pathList[j + 1]);
						}
					}
				}
				for (i = 0; i < filenum; i++)
				{
					printf("%d %s\n", i + 1, pathList[i].path);
				}
				printf("\n");
			}
		}
		else
		{
			printf("First start\n");
		}
		for (int i = 0; i < 2; i++)
			if (!(filenum == 0 && i == 1))
				printf(menustr[i], filenum + i + 1);
		printf("\n0 End\n");
		/////
		do
		{
			mode = ReadValue("Select the number from the menu>>");
		} while (inputNumChecker(mode, 0, filenum + 2) != 0);
		/////
		reset();
		if (mode == 0)
			return 0;
		else if (mode == filenum + 1)
			Add(pathList, filenum);
		else if (mode == filenum + 2)
			Delite(pathList, filenum);
		else
		{
			if (FileOpen(pathList[mode - 1].path) == 1) //セーブフラグが立つとセーブする
			{
				pathList[mode - 1].time = time(NULL);
				if ((fp_path = fopen(FileList, "w")) == NULL)
					error();
				for (int i = 0; i < filenum; i++)
				{
					fprintf(fp_path, "%s,%ld,\n", pathList[i].path, pathList[i].time);
				}
				fclose(fp_path);
			}
		}
		free(pathList);
	}
}
void error(void)
{
	printf("error");
	exit(1);
}
void PathUnitSwap(PathUnit *A, PathUnit *B)
{
	PathUnit C;
	C = *A;
	*A = *B;
	*B = C;
}
int ReadValue(char massage[])
{
	char string[100], *_string;
	printf("%s", massage);
	if (fgets(string, 100, stdin) == NULL)
	{
		if (string[99] != '\n')
			while (getchar() != '\n')
				;
		return EOF;
	}
	if (string[0] == '\n')
		return EOF;
	const int num = (int)strtol(string, &_string, 10);
	if (strcmp(string, _string) == 0)
		return EOF;
	return num;
}
void ReadString(char string[], int maxlong, char massage[])
{
	printf("%s", massage);
	if (fgets(string, maxlong, stdin) == NULL)
	{
		if (string[99] != '\n')
			while (getchar() != '\n')
				;
		printf("your input is too long\n");
		ReadString(string, maxlong, massage);
		return;
	}
	_trim(string); //先頭から一番近い'\n'を'\0'に置き換える関数
}
char ReadChar(char massage[])
{
	char string[100];
	printf("%s", massage);
	if (fgets(string, 100, stdin) == NULL)
	{
		if (string[99] != '\n')
			while (getchar() != '\n')
				;
	}
	return string[0];
}
int inputNumChecker(int checkNum, int min, int max)
{
	if (checkNum < min || checkNum > max)
	{
		printf("error:Input value is out of range\n");
		return -1;
	}
	return 0;
}
void _trim(char *str) /*改行コードをカットする*/
{
	char *c;
	c = strchr(str, '\n');
	if (c != NULL)
		*c = '\0';
}
void Add(PathUnit *pathUnit, int length)
{
	char filepath[PathLong];
	ReadString(filepath, PathLong - 5, "Input file path (without an extension)>>");
	strcat(filepath, ".csv");
	if (strcmp(FileList, filepath) == 0)
	{
		printf("you cannot add this file path\n");
		return;
	}
	for (int i = 0; i < length; i++)
	{
		if (strcmp(pathUnit[i].path, filepath) == 0)
		{
			printf("You have already registered the file\n");
			return;
		}
	}
	FILE *fp;
	if ((fp = fopen(FileList, "a")) == NULL)
		error();
	fprintf(fp, "%s,%ld,\n", filepath, time(NULL));
	fclose(fp);
}
void Delite(PathUnit *pathList, int listlong)
{
	printf("Enter 0 if you want to cancel\n");
	int n;
	do
	{
		n = ReadValue("Select the file to delete from the list>>");
	} while (inputNumChecker(n, 0, listlong) != 0);
	if (n == 0)
		return;
	FILE *fp;
	if ((fp = fopen(FileList, "w")) == NULL)
		error();
	char path[PathLong], answer;
	for (int i = 0; i < listlong; i++)
	{
		if (i != n - 1)
			fprintf(fp, "%s,%ld,\n", pathList[i].path, pathList[i].time);
		else
			strcpy(path, pathList[i].path);
	}
	fclose(fp);
	do
	{
		answer = ReadChar("Do you want to delete the file? y/n>>");
	} while (!(answer == 'y' || answer == 'n'));
	if (answer == 'y')
	{
		if (remove(path) == 0)
			printf("done\n");
		else
			printf("error\n");
	}
}
int FileOpen(char filename[]) //セーブをしたら１を帰す
{
	////指定されたファイルを読み取り、配列に格納する。読み取れなかった場合は新規と表示
	AddressUnit *addressList = (AddressUnit *)malloc(ArrayUnit * sizeof(AddressUnit)); //住所録のリストのアドレス
	int addressListLength = 0, head = 0, end, arraylength = ArrayUnit;
	int firstopen = 0;
	int mode;
	int fill = 0, save = 0; //終了時
	for (int i = 0; i < ArrayUnit; i++)
		addressList[i].number = 0;

	FILE *fp;
	if ((fp = fopen(filename, "r")) != NULL)
	{
		char buffer[100];
		int i, line = 0;
		size_t readsize;
		while ((readsize = fread(buffer, 1, 100, fp)) > 0)
		{
			for (i = 0; i < readsize; i++)
				if (buffer[i] == '\n')
					line++;
		}
		fclose(fp);
		if (line == 0)
			firstopen = 1;
		else
			addressListLength = line;
	}
	else
	{
		firstopen = 1;
	}
	if (addressListLength != 0) //Load
	{
		arraylength = addressListLength / ArrayUnit;
		if (addressListLength % ArrayUnit != 0)
			arraylength++;
		arraylength *= ArrayUnit;
		free(addressList);
		addressList = (AddressUnit *)malloc(arraylength * sizeof(AddressUnit));
		for (int i = 0; i < arraylength; i++)
			addressList[i].number = 0;

		if ((fp = fopen(filename, "r")) == NULL)
			error();
		for (int i = 0; i < addressListLength; i++)
		{
			addressList[i].number = 1;
			fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],\n",
				   addressList[i].personalname,
				   addressList[i].familyname,
				   addressList[i].postalcode,
				   addressList[i].address,
				   addressList[i].phoneNumber,
				   addressList[i].Email);
			if (i != 0)
			{
				addressList[i - 1].forpointer = &addressList[i];
				addressList[i].prepointer = &addressList[i - 1];
			}
		}
		end = addressListLength - 1;
		fclose(fp);
	}
	if (firstopen != 0)
	{
		printf("First open\n");
		addressList[0] = NewAddress();
		addressListLength = 1;
		end = 0;
	}
	///メニュー
	while (1)
	{
		printf("1 Add\n");
		printf("2 Delete\n");
		printf("3 Browsing\n");
		printf("4 Save\n");

		printf("\n0 exit\n");
		mode = -1;
		do
		{
			mode = ReadValue("Select the number from the menu>>");
		} while (inputNumChecker(mode, 0, 4) != 0);
		reset();
		if (mode == 0) // 終了
		{
			if (fill != 0) //ファイルに対して変更を行うとtrue
			{
				char a;
				do
				{
					a = ReadChar("I'll close it without saving, do you want to save? y/n>>");
				} while (!(a == 'y' || a == 'n'));
				if (a == 'y')
				{
					Save(filename, addressList, arraylength, head);
					save = 1;
				}
			}
			free(addressList);
			return save; //セーブをしたかどうかを伝える
		}
		if (mode == 1)
		{
			if (addressListLength == arraylength)
			{
				AddressUnit *_ad = (AddressUnit *)malloc(arraylength * sizeof(AddressUnit));
				for (int i = 0; i < arraylength; i++)
					_ad[i] = addressList[i];
				free(addressList);
				arraylength += ArrayUnit;
				addressList = (AddressUnit *)malloc(arraylength * sizeof(AddressUnit));
				for (int i = 0; i < arraylength; i++)
					addressList[i].number = 0;
				for (int i = 0; i < arraylength - ArrayUnit; i++)
					addressList[i] = _ad[i];
				free(_ad);
			}
			int i;
			for (i = 0; i < arraylength; i++)
			{
				if (addressList[i].number == 0)
					break;
			}
			addressList[i] = NewAddress();
			addressListLength++;
			if (addressListLength >= 2)
			{
				addressList[i].prepointer = &addressList[end];
				addressList[end].forpointer = &addressList[i];
			}
			else if (addressListLength == 1)
			{
				head = 0;
			}
			end = i;
			fill++;
		}
		if (mode == 2)
		{
			if (addressListLength == 0)
				printf("There is no Address you can delete\n");
			else
			{
				AddressUnit unit = addressList[head];
				int i = 1, target = -1;
				while (unit.number == 1)
				{
					printf("%d	%s %s\n", i, unit.familyname, unit.personalname);
					if (unit.forpointer == NULL)
					{
						break;
					}
					unit = *unit.forpointer;
					i++;
				}
				printf("\nInput 0 if you want to back\n\n");
				do
				{
					target = ReadValue("Select the number you want to Delete>>");
				} while (inputNumChecker(target, 0, i));
				AddressUnit *_unit;
				if (target != 0)
				{
					_unit = &addressList[head];
					for (int j = 1; j < target; j++)
					{
						_unit = _unit->forpointer;
					}
					_unit->number = 0;

					addressListLength--;
					if (target - 1 == head)
					{
						if (addressListLength != 0)
						{
							AddressUnit *Listhead = addressList[head].forpointer;
							Listhead->number = 2;
							for (int j = 0; j < arraylength; j++)
							{
								if (addressList[j].number == 2)
								{
									Listhead->number = 1;
									head = j;
									break;
								}
							}
						}
					}
					else
					{
						AddressUnit *ad = addressList[target - 1].prepointer;
						ad->forpointer = addressList[target - 1].forpointer;
					}
					if (target - 1 == end)
					{
						if (addressListLength != 0)
						{
							AddressUnit *Listend = addressList[end].prepointer;
							Listend->number = 2;
							for (int j = 0; j < arraylength; j++)
							{
								if (addressList[j].number == 2)
								{
									Listend->number = 1;
									end = j;
									break;
								}
							}
						}
					}
					else
					{
						AddressUnit *ad = addressList[target - 1].forpointer;
						ad->prepointer = addressList[target - 1].prepointer;
					}
					fill++;
				}
			}
		}
		if (mode == 3)
		{
			if (addressListLength == 0)
				printf("There are no Address\n");
			else
			{
				int change;
				addressList[head].prepointer = NULL;
				addressList[end].forpointer = NULL;
				AddressUnit unit = addressList[head];
				while (1)
				{
					reset();
					printAddress(unit);
					do
					{
						change = ReadValue("next:(1) back:(2) end:(0)>>");
					} while (inputNumChecker(change, 0, 2));
					if (change == 1 && unit.forpointer != NULL)
					{
						if (unit.forpointer->number != 0)
							unit = *unit.forpointer;
					}
					else if (change == 2 && unit.prepointer != NULL)
					{
						if (unit.prepointer->number != 0)
							unit = *unit.prepointer;
					}
					else if (change == 0)
						break;
				}
				reset();
			}
		}
		if (mode == 4)
		{
			Save(filename, addressList, arraylength, head);
			fill = 0;
			save = 1;
		}
	}
}
////
AddressUnit NewAddress(void)
{
	//コンマが入らないようにする必要性がある
	AddressUnit ret;
	ret.number = 1;
	ret.prepointer = NULL;
	ret.forpointer = NULL;

	ReadString(ret.personalname, 30, "Input person name >>");
	ReadString(ret.familyname, 30, "Input family name >>");
	ReadString(ret.postalcode, 10, "Input postal code >>");
	ReadString(ret.address, 60, "Input address >>");
	ReadString(ret.phoneNumber, 20, "Input phone number >>");
	ReadString(ret.Email, 60, "Input E-mail address >>");
	return ret;
}
void Save(char FileName[], AddressUnit *addressList, int arraylength, int head)
{
	AddressUnit unit = addressList[head];
	FILE *fp;
	if ((fp = fopen(FileName, "w")) == NULL)
		error();

	for (int i = 0; i < arraylength; i++)
	{
		if (unit.number == 0)
			break;
		fprintf(fp, "%s,%s,%s,%s,%s,%s,\n",
				unit.personalname,
				unit.familyname,
				unit.postalcode,
				unit.address,
				unit.phoneNumber,
				unit.Email);
		if (unit.forpointer == NULL)
			break;
		unit = *unit.forpointer;
	}
	fclose(fp);
}
void printAddress(AddressUnit ad)
{
	printf("familyname	%s\n", ad.familyname);
	printf("personalname	%s\n", ad.personalname);
	printf("postalcode	%s\n", ad.postalcode);
	printf("address	%s\n", ad.address);
	printf("phone number	%s\n", ad.phoneNumber);
	printf("E-mail	%s\n", ad.Email);
}
void reset(void)
{
	system("reset");
}