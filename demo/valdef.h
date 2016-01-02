//define the extraction type value


#ifndef   DFPOINT_H__
#define   DFPOINT_H__



#define		PI							3.1415926

///定义画笔的颜色
#define		SEED_PT_COLOR				RGB(0,255,255)
#define		DRAW_PEN_COLOR				RGB(255,0,0)
#define		XOR_PEN_COLOR				RGB(255,0,255)///RGB(200,120,60)



typedef struct {
	char MainDirPath[MAX_PATH];
	char CtrlPath[MAX_PATH];
	char MainDirName[MAX_PATH];
	char BlkName[MAX_PATH];
	int PhotoScale;
	float ResolutionMM;
	int ImageScale;
	int ImageBase;
	float ConInterval;
	float DemInterval;
}BLKDAT;//zhangl



typedef struct {
	int Rows;
	int Cols;
	float PixelSize;
	int ImageColor;
}SPTFILE;//ZHANGL


typedef struct {
	char MdlPath[MAX_PATH];
	char MdlName[MAX_PATH];
	char MdlDatPath[MAX_PATH];
	char LeftImage[MAX_PATH];
	char RightImage[MAX_PATH];
	int  EpiRows;
	int  EpiCols;
	int  EpiRowsL;
	int  EpiColsL;
	int  EpiRowsR;
	int  EpiColsR;
	int  MatchWidth;
	int  MatchHeight;
	int  XInterval;
	int  YInterval;
	float Overlap;
	char tmp[MAX_PATH];
	char product[MAX_PATH];
	SPTFILE sptfile;
}MDLDAT;//zhangl

struct record
{
	record *next;
	DWORD dwID;
	float flX;
	float flY;
	float flZ;
};//zhangl

struct PcfPoints{
	PcfPoints *next;
	int	nID;
	float flXL;
	float flYL;
	float flXR;
	float flYR;
};//ZHANGL


#endif

