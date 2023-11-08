//########################Asm compapability layer
void HaltCPU();


//#############################Custom Data types
typedef char int_8;
typedef short int_16;
typedef int int_32;
typedef long long int_64;

typedef unsigned char uint_8;
typedef unsigned short uint_16;
typedef unsigned int uint_32;
typedef unsigned long long uint_64;

struct interrupt_frame;

typedef unsigned char bool;

#define NULL ((void*)0)

//#####################################Memory Manager
#define HEAP_SIZE_KB 4
#define MAX_ALL_BLOCKS 256


void* MemAll(int size);
void* MemReal(void* ptr, int new_size, bool keep);
void  Free(void* ptr);
void  InitAll();
int   BlocSize(void* ptr);

//################################Text processor
void AlignText(char* text);
int TextToInt(const char* text, int *outp,  int from);
int Split(const char* text, char* outp,  char spliter, int from);
void SplitIterration(const char* text, char* outp, char spliter, int iter);
int CompareStrings(const char* a, const char* b);
int IntToStr(int inp, char* str);
int Find(const char* str, char ch, int from);
void MinusCollapse(char* str);

//##########################################Math
int_64 DivFlor(int a, int b);
int_64 DivCeil(int a, int b);
int_64 IntDiv(int a, int b);
int_64 Pow(int a, int b);
int_64 Root(int a, int b);
int_64 Abs(int a);
int ProcessString(char* string);
int SimpleProcess(char* string);
int ProcessExp(char* string, int sign);
int VerifyString(char* string);


//##############################################Print
enum ConsoleColor{
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GRAY = 7,
	COLOR_DARK_GRAY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_PINK = 13,
	COLOR_YELLOW = 14,
	COLOR_WHITE = 15,

};

void ClearPrint();
void SetPrintColor(uint_8 foreground, uint_8 background);
void GetCursor(uint_8* x, uint_8* y);
void SetCursor(uint_8 x, uint_8 y);
char GetPrintColor();
void SwitchPrintColor(uint_8 newcol);
void PrintChar(char ch);
void PrintCharSavePos(char ch);
void PrintString(char* string);
void PrintInt(int inp);
void ClearRow(int row_clear);
void PrintNewLine();
void GetMax(uint_8* x, uint_8* y);
void ShiftCursor(int x, int y);
void ColorLoader();

//##################Interrupt processor
#define INTERRUPT_HANDLER __attribute__((interrupt)) void

#define IDT_TYPE_INTR (0xE)
#define IDT_TYPE_TRAP (0xF)
//#define IDT_TYPE_TASK (0x5)

#define IDT_FLAG_RING_0 (0<<5)
#define IDT_FLAG_RING_1 (1<<5)
#define IDT_FLAG_RING_2 (2<<5)
#define IDT_FLAG_RING_3 (3<<5)
#define IDT_FLAG_RING_PRESENT (0x80)

#define GDT_CS (0x8)

struct idt_entry_t {
	uint_16     base_low;      // The lower 16 bits of the ISR's address
	uint_16     kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint_8	    ist;          // The IST in the TSS that the CPU will load into RSP; set to zero for now
	uint_8      attributes;   // Type and attributes; see the IDT page
	uint_16     base_mid;      // The higher 16 bits of the lower 32 bits of the ISR's address
	uint_32     base_high;     // The higher 32 bits of the ISR's address
	uint_32     reserved;     // Set to zero
} __attribute__((packed)) ;


 struct idtr_t {
	uint_16	    limit;
	uint_64	    base;
} __attribute__((packed)) ;


typedef INTERRUPT_HANDLER (*intr_handler)(struct interrupt_frame* frame);

INTERRUPT_HANDLER DefaultIntrHandlr(struct interrupt_frame* frame);
void IntrReg(int num, uint_16 segm_sel, uint_16 flags, intr_handler hndlr);
void IntrFill();
void SetIDT();
void IntrEnable();
void IntrDisable();


//############################Device port manager
uint_8 ReadDataFromPort (uint_16 port); // Чтение из порта

void SetDataToPort (uint_16 port, uint_8 data); // Запись


//###############################keyboard processor
#define PIC1_PORT (0x20)


void KeyboardInit();

void OnKeyEvent(uint_8 event_code);



char GetKey();
char ReadKey();
char* ReadLine();

uint_8 InterpritateCode(uint_8 code, char* symb);


//###############################################Default interrupts

#define PIC1_PORT (0x20)


void RegisterDefaultIDT();

INTERRUPT_HANDLER DivErr(struct interrupt_frame* frame);
INTERRUPT_HANDLER Debugpoint(struct interrupt_frame* frame);
INTERRUPT_HANDLER UnknownInter(struct interrupt_frame* frame);
INTERRUPT_HANDLER Breakpoint(struct interrupt_frame* frame, int_64 instruction);
INTERRUPT_HANDLER Overflow(struct interrupt_frame* frame, int_64 instruction);
INTERRUPT_HANDLER BoundCheck(struct interrupt_frame* frame, int_64 instruction);
INTERRUPT_HANDLER InvalidOpcode(struct interrupt_frame* frame);
INTERRUPT_HANDLER CoprocessorAvailib(struct interrupt_frame* frame);
INTERRUPT_HANDLER DoubleFault(struct interrupt_frame* frame);
INTERRUPT_HANDLER KeyboardInter(struct interrupt_frame* frame);
INTERRUPT_HANDLER InvalidTSS(struct interrupt_frame* frame);
INTERRUPT_HANDLER SegmentNotPresent(struct interrupt_frame* frame);
INTERRUPT_HANDLER StackException(struct interrupt_frame* frame);
INTERRUPT_HANDLER GeneralProtection(struct interrupt_frame* frame);
INTERRUPT_HANDLER PageFault(struct interrupt_frame* frame);
INTERRUPT_HANDLER Reserved(struct interrupt_frame* frame);
INTERRUPT_HANDLER CoprocessorError(struct interrupt_frame* frame);




//#####################################kernell
int kmain(){
    
    ClearPrint();
    IntrFill();
    RegisterDefaultIDT();
    KeyboardInit();
    SetIDT();
    IntrEnable();
    InitAll();
    ColorLoader();
   
  

    PrintString("HELLO USER");
    PrintNewLine();

    char * command;
    char * buff;

    do
    {
        command=ReadLine();


        

        SplitIterration(command, buff, ' ', 0);

        AlignText(buff);
        

        if (CompareStrings(buff, "info")) {
                PrintString("Made by Rock");
                PrintNewLine();
        }
        else if (CompareStrings(buff, "expr")) {
	        int err = 0;
            

	        SplitIterration(command, buff, ' ', 1);
            int cou=0;

            do
            {
                cou++;
            } while (buff[cou]!='\0');
            

            for (int i = cou-1; i >-1; i--)
            {
                buff[i+1]=buff[i];
            }
            buff[0]='(';
            buff[cou+1]=')';
            buff[cou+2]='\0';
	        err= ProcessString(buff);
	        if (err < 0) {
                switch (err)
                {
                case(-1):
                     PrintString("Err. Zero division");
                    PrintNewLine();
                    break;
                case(-2):
                    PrintString("Err. Int overflow");
                     PrintNewLine();
                    break;
                default:
                    break;
                }
		
	        }
	        else {
		         PrintString(buff);
                PrintNewLine();
            }
        }
        else if (CompareStrings(buff, "shutdown")) {
            break;
        }
        else if (CompareStrings(buff, "sysyphus")) {
            
        }
        else {
             PrintString("Command not found? go wash your balls");
                PrintNewLine();
        }

    } while (1);
    
 
    SetPrintColor(COLOR_LIGHT_GRAY, COLOR_BLACK);
    
    
}
    






const static int NUM_COLS=80;
const static int NUM_ROWS=25;

struct ConsChar{
    char symb;
    char color;
};

struct ConsChar* buffer=(struct ConsChar*) 0xb8000;

int col=0;
int row=0;

int CurColor=COLOR_WHITE | COLOR_BLACK<<4;

void ClearRow(int row_clear){
    struct ConsChar mpty;
    mpty.color=CurColor;
    mpty.symb=' ';

    for (int i = 0; i < NUM_COLS; i++)
    {
        buffer[i+NUM_COLS*row_clear]=mpty;
    }
    
    col=0;
    row=row_clear;

}



void ClearPrint()
{
   
    for (int i = 0; i < NUM_ROWS; i++)
    {
        ClearRow(i);
    }
    col=0;
    row=0;

}

void SetPrintColor(uint_8 foreground, uint_8 background)
{
    CurColor=foreground+(background<<4);
}

void GetCursor(uint_8 *x, uint_8 *y)
{
    (*x)=col;
    (*y)=row;
}

void SetCursor(uint_8 x, uint_8 y)
{
    col=x;
    if(x>NUM_COLS-1){
        col=NUM_COLS-1;
    }

    row=y;
    if(y>NUM_ROWS-1){
        row=NUM_ROWS-1;
    }

}

char GetPrintColor()
{
    return CurColor;
}

void SwitchPrintColor(uint_8 newcol)
{
    CurColor=newcol;
}

void PrintChar(char ch)
{
    PrintCharSavePos(ch);

    col++;

    if(col==NUM_COLS){
        
        PrintNewLine();
    }

   
}

void PrintCharSavePos(char ch)
{
    if(ch== '\n'){
        PrintNewLine();
        return;
    }

    struct ConsChar mpty;
    mpty.color=CurColor;
    mpty.symb=ch;


    buffer[col+NUM_COLS*row]=mpty;

}

void PrintString(char *string)
{
    for (int i = 0; 1; i++)
    {
        char ch=(char)string[i];

        if(ch=='\0'){
            return;
        }

        PrintChar(ch);
    }
    

}

void PrintInt(int inp)
{
    int flag=0;
    if(inp<0){
        inp=inp*(-1);
        flag=1;
    }
    char to_str[20];
    to_str[19]='\0';
    int next=inp;
    int cou=1;
    do
    {
        to_str[19-cou]=(next % 10)+48;
        next=(next-(next % 10))/10;
        cou++;
    } while (next>9);
   if(next!=0){
         to_str[19 - cou] = next + 48;
    }
    else {
        cou--;
     }
    for (int i = 0; i <= cou; i++)
    {
        to_str[i]=to_str[19-cou+i];
    }
    
    if(flag){
        PrintChar('-');
    }
    PrintString(to_str);
    
}

void PrintNewLine()
{
    col=0;

    row++;

     if(row<NUM_ROWS-1){
        return;
    }
    else
    {
        for (int i = 1; i < NUM_ROWS; i++)
        {
            for (int g = 0; g < NUM_COLS; g++)
            {
                struct ConsChar MovCh=buffer[g+(NUM_COLS)*i];
                buffer[g+(NUM_COLS)*(i-1)]=MovCh;
            }
            
        }
        ClearRow(NUM_ROWS-2);
        row=NUM_ROWS-2;
        
    }

}

void GetMax(uint_8 *x, uint_8 *y)
{
     (*x)=NUM_COLS;
    (*y)=NUM_ROWS;
}

void ShiftCursor(int x, int y)
{
    int vert_shift=DivFlor(x, NUM_COLS)+y;
    int hor_shift=x % NUM_COLS;

    row+=vert_shift;

    col+=hor_shift;

    if(row<0){
        row=0;
    }else if (row>NUM_ROWS-1)
    {
        row=NUM_ROWS-1;
    }
    

    if(col<0){
        col=NUM_COLS+col;
    }
    else if(col>NUM_COLS-1){
        col=NUM_COLS-1;
    }


    
}


int_64 DivFlor(int a, int b)
{

    int comp_a = a, comp_b = b;
    a = Abs(a);
    b = Abs(b);
    int ret = (a - (a % b)) / b;

    if (comp_a < 0) {
        ret = ret * (-1);
    }


    if (comp_b < 0) {
        ret = ret * (-1);
    }

    return ret;
}

int_64 DivCeil(int a, int b)
{
    int comp_a = a, comp_b = b;
    a = Abs(a);
    b = Abs(b);
    int ret = (a + (b - (a % b))) / b;

    if (comp_a < 0) {
        ret = ret * (-1);
    }


    if (comp_b < 0) {
        ret = ret * (-1);
    }

    return ret;
}

int_64 IntDiv(int a, int b)
{
    if (a % b < b / 2) {
        return DivFlor(a, b);
    }
    else {
        return DivCeil(a, b);
    }
}

int_64 Pow(int a, int b)
{
    if (b < 0) {
        return -1;
    }

    if (b == 0) {
        return 1;
    }

    int ans = a;

    for (int i = 1; i < b; i++)
    {
        ans = ans * a;
    }

    return ans;
}

int_64 Root(int a, int b)
{
    for (int i = 0; i < a; i++)
    {
        if (Pow(i, b) >= a) {
            return i;
        }
    }
}

int_64 Abs(int a)
{
    if (a > 0) {
        return a;
    }
    else {
        a = a * (-1);
        return a;
    }
}

int ProcessString(char* string)
{
     int link = 0;
    int link_end = 0;
    int prior = 0;
    int prior_mem = 0;

    int cou = 0;
    char buff[40];
    int buff_cou = 0;

    int err = 0;

    MinusCollapse(string);

    do
    {


        if (Find(string, '(', 0) == -1) {
            break;
        }


        cou = 0;
        prior_mem = 0;
        prior = 0;
        do
        {
            if (string[cou] == '(') {
                prior++;
                if (prior > prior_mem) {
                    link = cou + 1;
                    prior_mem = prior;
                    buff_cou = 0;
                }
                

            }
            else if (string[cou] == ')')
            {
                if (prior == prior_mem) {
                    link_end = cou;
                }
                prior--;
                if(buff_cou>-1){
                    buff[buff_cou] = '\0';
                }
                
                buff_cou = -1;
            }
            else {
                if (buff_cou > -1) {
                    buff[buff_cou] = string[cou];
                    buff_cou++;
                }
            }
            cou++;
        } while (string[cou]!='\0');
        

        err=SimpleProcess(buff);
        if (err < 0) {
            return err;
        }

        
        cou = 0;
        link--;
        do
        {
            cou++;
        } while (buff[cou] != '\0');

        int size = cou;

        if (size > (link_end - link + 1)) {
            int shift = size - (link_end - link + 1);

            cou = link_end;

            do
            {
                cou++;
            } while (string[cou] != '\0');

            for (int i = cou; i > link_end; i--)
            {
                string[i + shift] = string[i];
            }

        }
        else if (size < (link_end - link + 1)) {
            int shift = (link_end - link + 1) - size;

            do
            {
                cou++;
            } while (string[cou] != '\0');

            for (int i = link_end; i < cou + 1; i++)
            {
                string[i - shift] = string[i];
            }

        }


        cou = 0;

        do
        {
            string[link + cou] = buff[cou];
            cou++;
        } while (buff[cou] != '\0');


    } while (1);

    err = SimpleProcess(string);
    if (err < 0) {
        return err;
    }

    return 0;
}

int SimpleProcess(char* string)
{
    MinusCollapse(string);
    int err = 0;
    int ret = -1;
    do
    {
        ret = Find(string, '^', ret + 1);

        if (ret != -1) {
            err = ProcessExp(string, ret);
            if (err < 0) {
                return err;
            }
            MinusCollapse(string);
            ret = 0;
        }
    } while (ret != -1);

    ret = -1;
    do
    {
        ret = Find(string, 'R', ret + 1);

        if (ret != -1) {
            err = ProcessExp(string, ret);
            if (err < 0) {
                return err;
            }
            MinusCollapse(string);
            ret = 0;
        }
    } while (ret != -1);

    ret = -1;
    do
    {
        ret=Find(string, '*', ret + 1);

        if (ret != -1) {
            err = ProcessExp(string, ret);
            if (err < 0) {
                return err;
            }
            MinusCollapse(string);
            ret = 0;
        }
    } while (ret!=-1);

    ret = -1;
    do
    {
        ret = Find(string, '/', ret + 1);

        if (ret != -1) {
            err=ProcessExp(string, ret);
            if (err <0) {
                return err;
            }
            MinusCollapse(string);
            ret = 0;
        }
    } while (ret != -1);

    ret = 0;
    do
    {

        ret = Find(string, '-', ret + 1);

        if (ret != -1) {
            err = ProcessExp(string, ret);
            if (err < 0) {
                return err;
            }
            MinusCollapse(string);
            ret = 0;
        }
    } while (ret != -1);

    ret = -1;
    do
    {
        ret = Find(string, '+', ret + 1);

        if (ret != -1) {
            err = ProcessExp(string, ret);
            if (err < 0) {
                return err;
            }
            MinusCollapse(string);
            ret = 0;
        }
    } while (ret != -1);

    return 0;
}

int ProcessExp(char* string, int sign)
{
    int a = 0;
    int b = 0;

    int_64 ans=0;

    int start_a=0;
    int end_b = 0;
    int err = 0;
    char operate=string[sign];

    int cou = 0;

    err = TextToInt(string, &b, sign + 1);

    if (err == -1) {
        return -2;
    }
    
    
    cou = sign + 1;

    do
    {
        cou++;
    } while (string[cou] != '\0' && string[cou] > 47 && string[cou] < 58);

    end_b = cou-1;
    cou = sign-1;
    do
    {
        cou--;
    } while (string[cou] > 47 && string[cou] < 58 && cou > -1);

    start_a = cou + 1;

    err = TextToInt(string, &a, start_a);

    if (err == -1) {
        return -2;
    }
    

    if (a < 0) {
        start_a--;
    }

    switch (operate)
    {
        case '+':
            ans = a + b;
            break;
    
        case '-':
            ans = a - b;
            break;
    
        case '*':
            ans = a * b;
            break;

        case '/':
            if (b == 0) {
                return -1;
            }
            ans = IntDiv(a, b);
            break;

        case '^':
            ans = Pow(a, b);
            break;

        case 'R':
            ans = Root(a, b);
            break;
    default:
        break;
    }


    if (ans > 2147483647) {
        return -2;
    }

    if (ans < -2147483647) {
        return -2;
    }

    char buff[10];

    int size=IntToStr(ans, buff);
    
    if (size > (end_b - start_a + 1)) {
        int shift = size-(end_b - start_a + 1);
        
        cou = end_b;

        do
        {
            cou++;
        } while (string[cou]!='\0');

        for (int i = cou; i > end_b; i--)
        {
            string[i + shift] = string[i];
        }

    }
    else if (size < (end_b - start_a + 1)) {
        int shift = (end_b - start_a + 1)-size;

        do
        {
            cou++;
        } while (string[cou] != '\0');

        for (int i = end_b; i < cou+1; i++)
        {
            string[i - shift] = string[i];
        }

    }
    

    cou = 0;

    do
    {
        string[start_a + cou] = buff[cou];
        cou++;
    } while (buff[cou] != '\0');

    return 1;
   
}

int VerifyString(char* string)
{



    return 0;
}


void AlignText(char* text)
{
	int cou = 0;

	do
	{
		if (text[cou] > 64 && text[cou] < 91) {
			text[cou] = text[cou] + 32;
		}
		cou++;
	} while (text[cou]!='\0');
	return;
}

int TextToInt(const char* text, int* outp, int from)
{
	if (text[from] == '-') {
		from++;
	}

	int cou = from;
	int_64 ret = 0;
	do
	{	
		ret = ret * 10;
		ret += text[cou] - 48;

		if (ret > 2147483647) {
			return -1;
		}

		if (ret < -2147483647) {
			return -1;
		}

		cou++;

	} while (text[cou] != '\0' && text[cou]>47 && text[cou]<58);

	if (from > 0) {
		if (text[from-1]=='-') {

			if (from > 1) {
				if (text[from - 2] == '*' || text[from - 2] == '/' || text[from - 2] == '^' || text[from - 2] == 'R') {
					ret = ret * -1;
				}
			}
			else {
				ret = ret * -1;
			}
			
		}
	}

	(*outp) = ret;

	return 0;
}

int Split(const char* text,char* outp, char spliter, int from)
{

	int cou = from;
	do
	{
		outp[cou-from] = text[cou];
		cou++;
	} while (text[cou]!='\0' && text[cou] != spliter);
	outp[cou-from] = '\0';
	return cou;
}

void SplitIterration(const char* text, char* outp, char spliter, int iter)
{
	int last = 0;
	for (int i = 0; i < iter+1; i++)
	{
		last= Split(text, outp, spliter, last);
		last++;

	}
}

int CompareStrings(const char* a, const char* b)
{
	int cou = 0;
	do
	{
		if (a[cou] != b[cou]) {
			return 0;
		}

		cou++;
	} while (a[cou]!='\0' && b[cou] != '\0');

	if (a[cou] != b[cou]) {
		return 0;
	}

	return 1;
}

int IntToStr(int inp, char* str)
{
	int flag = 0;
	if (inp < 0) {
		inp = inp * (-1);
		flag = 1;
	}
	
	str[19] = '\0';
	int next = inp;
	int cou = 1;
	do
	{
		str[19 - cou] = (next % 10) + 48;
		next = (next - (next % 10)) / 10;
		cou++;
	} while (next > 9);
	if (next != 0) {
		str[19 - cou] = next + 48;
	}
	else {
		cou--;
	}
	

	if (flag) {
		for (int i = 1; i <= cou+1; i++)
		{
			str[i] = str[19 - cou + i-1];
		}
		str[0] = '-';
		return cou + 1;
	}
	else {
		for (int i = 0; i <= cou; i++)
		{
			str[i] = str[19 - cou + i];
		}
		return cou;
	}

	return -1;
	
}

int Find(const char* str, char ch, int from)
{
	int cou = from;

	if (str[cou] == '\0') {
		return -1;
	}

	do
	{
		if (str[cou] == ch) {
			return cou;
		}
		cou++;
	} while (str[cou]!='\0');

	return -1;
}

void MinusCollapse(char* str)
{

	int cou = 1;

	int inter_cou = 0;
	do
	{

		if (str[cou] == '-') {
			switch (str[cou-1])
			{
			case '+':
				str[cou - 1] = '-';
				inter_cou = cou;
				do
				{
					str[inter_cou] = str[inter_cou + 1];
					inter_cou++;
				} while (str[inter_cou] != '\0');
				break;

			case '-':
				str[cou - 1] = '+';
				inter_cou = cou;
				do
				{
					str[inter_cou] = str[inter_cou + 1];
					inter_cou++;
				} while (str[inter_cou] != '\0');
				break;

			default:
				cou++;
				break;
			}
		}
		else {
			cou++;
		}
		

	} while (str[cou]!='\0');

}


void HaltCPU(){

   do
   {
      
   } while (1);
   

}


void RegisterDefaultIDT()
{
    IntrReg(0, GDT_CS, IDT_FLAG_RING_PRESENT | IDT_TYPE_INTR, DivErr); // segm_sel=0x8, P=1, DPL=0, Type=Intr
    IntrReg(1, GDT_CS, IDT_FLAG_RING_PRESENT | IDT_TYPE_TRAP, DivErr); // segm_sel=0x8, P=1, DPL=0, Type=Intr
    IntrReg(2, GDT_CS, IDT_FLAG_RING_PRESENT | IDT_TYPE_INTR, UnknownInter); // segm_sel=0x8, P=1, DPL=0, Type=Intr
    IntrReg(3, GDT_CS, IDT_FLAG_RING_PRESENT | IDT_TYPE_TRAP, Breakpoint); // segm_sel=0x8, P=1, DPL=0, Type=Intr
    IntrReg(4, GDT_CS, IDT_FLAG_RING_PRESENT | IDT_TYPE_TRAP, Overflow); // segm_sel=0x8, P=1, DPL=0, Type=Intr
    IntrReg(5, GDT_CS, IDT_FLAG_RING_PRESENT | IDT_TYPE_INTR, BoundCheck); // segm_sel=0x8, P=1, DPL=0, Type=Intr
    IntrReg(6, GDT_CS, IDT_FLAG_RING_PRESENT | IDT_TYPE_INTR, InvalidOpcode); // segm_sel=0x8, P=1, DPL=0, Type=Intr
    IntrReg(7, GDT_CS, IDT_FLAG_RING_PRESENT | IDT_TYPE_INTR, CoprocessorAvailib); // segm_sel=0x8, P=1, DPL=0, Type=Intr
    IntrReg(8, GDT_CS, IDT_FLAG_RING_PRESENT | IDT_TYPE_INTR, DoubleFault); // segm_sel=0x8, P=1, DPL=0, Type=Intr
    IntrReg(9, GDT_CS, IDT_FLAG_RING_PRESENT | IDT_TYPE_TRAP, KeyboardInter); // segm_sel=0x8, P=1, DPL=0, Type=Intr
    IntrReg(10, GDT_CS, IDT_FLAG_RING_PRESENT | IDT_TYPE_INTR, InvalidTSS); // segm_sel=0x8, P=1, DPL=0, Type=Intr
    IntrReg(11, GDT_CS, IDT_FLAG_RING_PRESENT | IDT_TYPE_INTR, SegmentNotPresent); // segm_sel=0x8, P=1, DPL=0, Type=Intr
    IntrReg(12, GDT_CS, IDT_FLAG_RING_PRESENT | IDT_TYPE_INTR, StackException); // segm_sel=0x8, P=1, DPL=0, Type=Intr
    IntrReg(13, GDT_CS, IDT_FLAG_RING_PRESENT | IDT_TYPE_INTR, GeneralProtection); // segm_sel=0x8, P=1, DPL=0, Type=Intr
    IntrReg(14, GDT_CS, IDT_FLAG_RING_PRESENT | IDT_TYPE_INTR, PageFault); // segm_sel=0x8, P=1, DPL=0, Type=Intr
    IntrReg(15, GDT_CS, IDT_FLAG_RING_PRESENT | IDT_TYPE_INTR, Reserved); // segm_sel=0x8, P=1, DPL=0, Type=Intr
    IntrReg(16, GDT_CS, IDT_FLAG_RING_PRESENT | IDT_TYPE_INTR, CoprocessorError); // segm_sel=0x8, P=1, DPL=0, Type=Intr
}

INTERRUPT_HANDLER DivErr(struct interrupt_frame* frame)
{
     
    PrintNewLine();
    char mem=GetPrintColor();
    SetPrintColor(COLOR_RED, COLOR_BLACK);
    PrintString("Division Error.");
    SwitchPrintColor(mem);
     
}

INTERRUPT_HANDLER Debugpoint(struct interrupt_frame* frame)
{
     
    PrintNewLine();
    char mem=GetPrintColor();
    SetPrintColor(COLOR_RED, COLOR_BLACK);
    PrintString("Debug Breakpoint.");
    SwitchPrintColor(mem);
     
}

INTERRUPT_HANDLER UnknownInter(struct interrupt_frame* frame)
{
     
    PrintNewLine();
    char mem=GetPrintColor();
    SetPrintColor(COLOR_RED, COLOR_BLACK);
    PrintString("Unknown Interrupt.");
    SwitchPrintColor(mem);
     
}

INTERRUPT_HANDLER Breakpoint(struct interrupt_frame* frame, int_64 instruction)
{
     
    PrintNewLine();
    char mem=GetPrintColor();
    SetPrintColor(COLOR_RED, COLOR_BLACK);
    PrintString("Breakpoint.");
    SwitchPrintColor(mem);
     
}

INTERRUPT_HANDLER Overflow(struct interrupt_frame* frame, int_64 instruction)
{
     
    PrintNewLine();
    char mem=GetPrintColor();
    SetPrintColor(COLOR_RED, COLOR_BLACK);
    PrintString("Overflow Error.");
    SwitchPrintColor(mem);
     
}

INTERRUPT_HANDLER BoundCheck(struct interrupt_frame* frame, int_64 instruction)
{
     
    PrintNewLine();
    char mem=GetPrintColor();
    SetPrintColor(COLOR_RED, COLOR_BLACK);
    PrintString("Bound Check error.");
    SwitchPrintColor(mem);
     
}

INTERRUPT_HANDLER InvalidOpcode(struct interrupt_frame* frame)
{
     
    PrintNewLine();
    char mem=GetPrintColor();
    SetPrintColor(COLOR_RED, COLOR_BLACK);
    PrintString("Invalid Operation Code.");
    SwitchPrintColor(mem);
     
}

INTERRUPT_HANDLER CoprocessorAvailib(struct interrupt_frame* frame)
{
     
    PrintNewLine();
    char mem=GetPrintColor();
    SetPrintColor(COLOR_RED, COLOR_BLACK);
    PrintString("Coprocessor not found.");
    SwitchPrintColor(mem);
     
}

INTERRUPT_HANDLER DoubleFault(struct interrupt_frame* frame)
{
     
    PrintNewLine();
    char mem=GetPrintColor();
    SetPrintColor(COLOR_RED, COLOR_BLACK);
    PrintString("Double fault.");
    SwitchPrintColor(mem);
     
}

INTERRUPT_HANDLER KeyboardInter(struct interrupt_frame* frame)
{
     
    
    if (ReadDataFromPort(0x64) & 0x01)
    {
        uint_8 scan_code;
        uint_8 state;
        scan_code = ReadDataFromPort(0x60); // Считывание символа с PS/2 клавиатуры
        OnKeyEvent(scan_code);
        
        
    }
    


    SetDataToPort(PIC1_PORT, 0x20);
     
}

INTERRUPT_HANDLER InvalidTSS(struct interrupt_frame* frame)
{
     
    PrintNewLine();
    char mem=GetPrintColor();
    SetPrintColor(COLOR_RED, COLOR_BLACK);
    PrintString("Invalid TSS.");
    SwitchPrintColor(mem);
     
}

INTERRUPT_HANDLER SegmentNotPresent(struct interrupt_frame* frame)
{
     
    PrintNewLine();
    char mem=GetPrintColor();
    SetPrintColor(COLOR_RED, COLOR_BLACK);
    PrintString("Segment Not ptesent.");
    SwitchPrintColor(mem);
     
}

INTERRUPT_HANDLER StackException(struct interrupt_frame* frame)
{
     
    PrintNewLine();
    char mem=GetPrintColor();
    SetPrintColor(COLOR_RED, COLOR_BLACK);
    PrintString("Stack exc.");
    SwitchPrintColor(mem);
     
}

INTERRUPT_HANDLER GeneralProtection(struct interrupt_frame* frame)
{
     
    PrintNewLine();
    char mem=GetPrintColor();
    SetPrintColor(COLOR_RED, COLOR_BLACK);
    PrintString("General Protection Fault.");
    SwitchPrintColor(mem);
     
}

INTERRUPT_HANDLER PageFault(struct interrupt_frame* frame)
{
     
    PrintNewLine();
    char mem=GetPrintColor();
    SetPrintColor(COLOR_RED, COLOR_BLACK);
    PrintString("PageFault.");
    SwitchPrintColor(mem);
     
}

INTERRUPT_HANDLER Reserved(struct interrupt_frame* frame)
{
     
    PrintNewLine();
    char mem=GetPrintColor();
    SetPrintColor(COLOR_RED, COLOR_BLACK);
    PrintString("Reserved?");
    SwitchPrintColor(mem);
     
}

INTERRUPT_HANDLER CoprocessorError(struct interrupt_frame* frame)
{
     
    PrintNewLine();
    char mem=GetPrintColor();
    SetPrintColor(COLOR_RED, COLOR_BLACK);
    PrintString("Soprocessor failature");
    SwitchPrintColor(mem);
     
}


uint_8 ReadDataFromPort (uint_16 port) // Чтение из порта
{
    uint_8 data;
    asm volatile ("inb %w1, %b0" : "=a" (data) : "Nd" (port));
    return data;
}


void SetDataToPort (uint_16 port, uint_8 data) // Запись
{
    asm volatile ("outb %b0, %w1" : : "a" (data), "Nd" (port));
}

struct idt_entry_t g_idt[256]; // Реальная таблица IDT
struct idtr_t g_idtp; // Описатель таблицы для команды lidt


INTERRUPT_HANDLER DefaultIntrHandlr(struct interrupt_frame* frame) {
    

    PrintNewLine();
    char mem=GetPrintColor();
    SetPrintColor(COLOR_RED, COLOR_BLACK);
    PrintString("Called Exsception handler... processor holded.");
    SwitchPrintColor(mem);
    HaltCPU();

}

void IntrReg(int num, uint_16 segm_sel, uint_16 flags, intr_handler hndlr)
{
    uint_64 HndlrAdr=(uint_64)hndlr;

    g_idt[num].base_low=(uint_64)(HndlrAdr & 0xFFFF);
    g_idt[num].kernel_cs=0x08;
    g_idt[num].ist=0;
    g_idt[num].attributes=flags;
    g_idt[num].base_mid=(uint_64)((HndlrAdr >> 16) & 0xFFFF);
    g_idt[num].base_high= (uint_64)((HndlrAdr >> 32) & 0xFFFFFFFF);

}

void IntrFill()
{
    int i;
    int idt_count = sizeof(g_idt) / sizeof(g_idt[0]);
    for(i = 0; i < idt_count; i++){
        IntrReg(i, GDT_CS, IDT_FLAG_RING_PRESENT | IDT_TYPE_INTR, DefaultIntrHandlr); // segm_sel=0x8, P=1, DPL=0, Type=Intr
    }
    
    
   
}

void SetIDT()
{
    int idt_count = sizeof(g_idt) / sizeof(g_idt[0]);
    g_idtp.base = (unsigned int) (&g_idt[0]);
    g_idtp.limit = (sizeof (struct idt_entry_t) * idt_count) - 1;


    asm volatile ("lidt %0" : : "m"(g_idtp)); // load the new IDT

}

void IntrEnable()
{
    PrintString("Interrupts enabled");
    PrintNewLine();
     asm volatile ("sti"); // set the interrupt flag
}

void IntrDisable()
{
    PrintString("Interrupts disabled");
    PrintNewLine();
    asm volatile ("cli"); // set the interrupt flag
}

char CCodes[]={0,27,'1','2','3','4','5','6','7','8','9','0','-','=', 8,
9, 'q','w','e','r','t','y','u','i','o','p','[',']',13,17,
'a','s','d','f','g','h','j','k','l',';', 39 , 96 , 15 ,92
,'z','x','c','v','b','n','m',',','.','/', 14, '*', 17, ' '};

char line[50];
int size=0;

char pressed_key='\0';

int_8 flag_l=0;
int_8 flag_k=0;

bool read_line=0;

bool shift_dwn=0;
bool ctrl_dwn=0;
bool alt_dwn=0;

void KeyboardInit()
{
    SetDataToPort(PIC1_PORT + 1, 0xFF ^ 0x02);
}

void OnKeyEvent(uint_8 event_code)
{   
    uint_8 new_chr='\0';
    uint_8 flg=InterpritateCode(event_code, &new_chr);
    int mem_x=0, mem_y=0;
    int holder=0;
    if(flg){
        pressed_key=new_chr;
        flag_k=1;
        if(read_line){
             if(pressed_key==13){
                line[size]='\0';
                flag_l=1;
                size=0;
                PrintNewLine();
            }
            else
            {


              
                switch (new_chr)
                {
                case 8: //Backspace

                    if(size>0){
                       
                        line[size]=' ';
                        size--;
                        ShiftCursor(-1, 0);
                        PrintCharSavePos(' ');
                    }

                    break;
                
                case 9: //Tab
                    GetCursor(&mem_x, &mem_y);
                    int step=mem_x % 8;

                    if(step==0){
                        step=8;
                    }
                    ShiftCursor(step, 0);

                    break;
                
                case 17: //Ctrl
                    ctrl_dwn=1;
                    break;
                
                case 15: //Shift
                    shift_dwn=1;
                    break;
                
                case 14: //Alt
                    alt_dwn=1;
                    break;
                
                default:
                    

                    if(shift_dwn){
                        switch (new_chr)
                        {
                        case 61:
                            new_chr=43;
                            break;
                        case 48:
                            new_chr=41;
                            break;
                        case 57:
                            new_chr=40;
                            break;
                        case 54:
                            new_chr=94;
                            break;
                        case 56:
                            new_chr=42;
                            break;
                        default:
                             new_chr-=32;
                            break;
                        }
                    }
                    line[size]=new_chr;
                    size++;
                    PrintChar(new_chr);
                    break;
                }

                
            }
        }
    }
    else
    {
        pressed_key=0;
        flag_k=0;

         switch (new_chr)
                {
                case 8: //backspace
                   
                    break;
                
                case 9: //Tab
                   break;
                
                case 17: //Ctrl
                    ctrl_dwn=0;
                    break;
                
                case 15: //Shift
                    shift_dwn=0;
                    break;
                
                case 14: //Alt
                    alt_dwn=0;
                    break;
                
                default:
                
                    break;
                }
    }
}

char GetKey()
{
    return pressed_key;
}

char ReadKey()
{
    do
    {
        
    } while (flag_k==0);
    flag_k=0;
    return pressed_key;
}

char *ReadLine()
{   
 
    size=0;

    read_line=1;
    do
    {
        
    } while (flag_l==0);
    flag_l=0;
    read_line=0;
    return &line[0];
}

uint_8 InterpritateCode(uint_8 code, char* symb)
{
    uint_8 ret=0; 
    if(code<129){
        ret=1;
    }
    else
    {
        ret=0;
        code=code-128;
    }

    (*symb)=CCodes[code];
    return ret;
    
}

void ColorLoader()
{
     PrintString("Chose color: 1-Gray, 2-White, 3-Red, 4-Green, 5-Blue, 6-Black, 7-Yellow");

    char new_col=ReadKey();

    switch (new_col)
    {
    case '1':
        SetPrintColor(COLOR_LIGHT_GRAY, COLOR_BLACK);
        break;
    case '2':
        SetPrintColor(COLOR_WHITE, COLOR_BLACK);
        break;
    case '3':
        SetPrintColor(COLOR_RED, COLOR_BLACK);
        break;
    case '4':
        SetPrintColor(COLOR_GREEN, COLOR_BLACK);
        break;
    case '5':
        SetPrintColor(COLOR_BLUE, COLOR_BLACK);
        break;
    case '6':
        SetPrintColor(COLOR_BLACK, COLOR_BLACK);
        break;
    case '7':
        SetPrintColor(COLOR_YELLOW, COLOR_BLACK);
        break;


    default:
        SetPrintColor(COLOR_CYAN, COLOR_BLACK);
        break;
    }

    ClearPrint();
}

struct AllocatedPointer
{
   void* ptr;
   int size;
   bool state;
};

char heap[HEAP_SIZE_KB*1024];
int allocated_num=0;
struct AllocatedPointer blocks[MAX_ALL_BLOCKS];

void *MemAll(int size)
{
    int best_size=(HEAP_SIZE_KB*1024)+1;
    void* ptr=NULL;
    int shift=0;
    int block_id=-1;

    for (int i = 0; i < allocated_num; i++)
    {
        if(blocks[i].state==0){
            if(best_size>blocks[i].size && blocks[i].size>=size){
                best_size=blocks[i].size;
                ptr=blocks[i].ptr;
                shift=blocks[i].size-size;
                block_id=i;
            }
        }
    }

    if(shift>0){
        blocks[allocated_num].ptr=ptr+size;
        blocks[allocated_num].size=shift;
        blocks[allocated_num].state=0;
        allocated_num++;
    }
    
    blocks[block_id].size=size;
    blocks[block_id].state=1;

   return ptr;
}

void *MemReal(void *ptr, int new_size, bool keep)
{   
    void* new_ptr=MemAll(new_size);

    if(keep){

        

        for (int i = 0; i < allocated_num; i++)
        {
        if(blocks[i].ptr==ptr){
            
            int min=new_size;

            if(blocks[i].size<new_size){
                min=blocks[i].size;
            }

            for (int g = 0; g < min; g++)
            {
                ((char*)(new_ptr))[g]=((char*)(ptr))[g];
            }
            

            break;
        }
        
        }
            

    }
    
    

   return new_ptr;
}


void InitAll()
{
    blocks[0].ptr=heap;
    blocks[0].size=HEAP_SIZE_KB*1024;
    blocks[0].state=0;
    allocated_num=1;
}

int BlocSize(void *ptr)
{   


    for (int i = 0; i < allocated_num; i++)
    {
        if(blocks[i].ptr==ptr){
            return blocks[i].size;
        }
        
    }
            

    return -1;
}
