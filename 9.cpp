#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

struct TOKEN
{
  string t;
  int m;
} ;
struct QT
{
  string w;//操作符
  struct TOKEN word1;//左操作数
  struct TOKEN word2;//右操作数
  struct TOKEN result;//结果
} ;



//全局变量
int i=1;
int ERR=0;
vector<string> words;
vector<int> line;
vector<int> row;

int flag2=0;
void deleted(string &strr);
void Block();
void Decls();
void Statements();
void Decls1();
void Decl();
void Names();
void Name();
void Names1();
void Stmts1();
void Stmt();
void ELSE();
void Bool();

void E();
void E1();
void T();
void T1();
void F();

void newt();//生成结果符号(t1,t2...)
void quat(string ww);//生成四元式
struct TOKEN word;
struct TOKEN sem[10];//存放操作数
int i_sem;//操作数TOKEN的下标
struct QT qt[30];//存放四元式
int num=0;
int order=1;
vector<int> whiletag1;
vector<int> whiletag2;
vector<int> iftag;

void newt()
{
    word.t="t";
    word.m=order++;
}

void quat(string ww)
{
    string c;
    TOKEN ex;
    if(ww=="+"||ww=="-"||ww=="*"||ww=="/")
    {
        qt[num].w=ww;
        qt[num].word1=sem[i_sem-1];
        qt[num].word2=sem[i_sem];
        newt();
        qt[num].result=word;
        i_sem--;
        i_sem--;
        sem[++i_sem]=word;
    }
    else if(ww=="=")
    {
        qt[num].w=ww;
        qt[num].word1=sem[i_sem];
        ex.t="-";
        ex.m=0;
        qt[num].word2=ex;
        qt[num].result=sem[i_sem-1];
        i_sem--;
        i_sem--;
    }
    else if(ww==">"||ww==">="||ww=="<"
            ||ww=="<="||ww=="=="||ww=="!=")
    {
        qt[num].w="j"+ww;
        qt[num].word1=sem[i_sem-1];
        qt[num].word2=sem[i_sem];
        ex.t="goto";
        ex.m=0;
        qt[num].result=ex;
        i_sem--;
        i_sem--;
    }
    else if(ww=="if j")
    {
        qt[num].w="jmp";
        ex.t="-";
        ex.m=0;
        qt[num].word1=ex;
        qt[num].word2=ex;
        ex.t="goto else";
        ex.m=0;
        qt[num].result=ex;
    }
    else if(ww=="while j")
    {
        qt[num].w="jmp";
        ex.t="-";
        ex.m=0;
        qt[num].word1=ex;
        qt[num].word2=ex;
        ex.t="goto}";
        ex.m=0;
        qt[num].result=ex;
    }
    else if(ww=="loop while j")
    {
        qt[num].w="jmp";
        ex.t="-";
        ex.m=0;
        qt[num].word1=ex;
        qt[num].word2=ex;
        ex.t="goto while";
        ex.m=0;
        qt[num].result=ex;
    }
    num++;
}

void Block()
{
    if(words[i]=="{")
    {
        ++i;
        Decls();
        Statements();
        if(words[i]=="}")
        {
             ++i;
        }
        else
        {
            ERR=1;
            row.push_back(line[i]);
        }

    }
    else
    {
        row.push_back(line[i]);
        ERR=1;
    }

}
void Decls()//消除左递归
{
   Decls1();
}
void Decls1()
{
    if(words[i]=="int")
    {
        Decl();
        Decls1();
    }
}
void Decl()
{
    if(words[i]=="int")
    {
        ++i;
        Names();
        if(words[i]==";")
        {
            ++i;
        }
        else
        {
            row.push_back(line[i]);
            ERR=1;
        }
    }
}
void Names()//消除左递归
{
    Name();
    Names1();
}
void Names1()
{
    if(words[i]==",")
    {
        ++i;
        Name();
        Names1();
    }
}
void Name()
{
    if(isalpha(words[i][0])||words[i][0]=='_')
    {
        ++i;
    }
    else
    {
        row.push_back(line[i]);
        ERR=1;
    }
}
void Statements()
{
    Stmts1();
}
void Stmts1()
{
    if(words[i]=="if"||words[i]=="while"||isalpha(words[i][0])||words[i][0]=='_'||words[i]=="{")
    {
        Stmt();
        Stmts1();
    }
    else if(words[i]!="}")
    {
        ERR=1;
        row.push_back(line[i]);
    }

}
void Stmt()
{
    string flag;
    if(words[i]=="if")
    {
        ++i;
        if(words[i]=="(")
        {
            ++i;
            Bool();
            if(words[i]==")")
            {
                ++i;
                quat("if j");
                Stmt();
                iftag.push_back(num);
                ELSE();
            }
            else
            {
                ERR=1;
                row.push_back(line[i]);
            }

        }
        else
        {
            row.push_back(line[i]);
            ERR=1;
        }

    }
    else if(words[i]=="while")
    {
        ++i;
        if(words[i]=="(")
        {
            ++i;
            whiletag1.push_back(num);
            Bool();
            if(words[i]==")")
            {
                ++i;
                quat("while j");
                Stmt();
                quat("loop while j");
                whiletag2.push_back(num);
            }
            else
            {
                row.push_back(line[i]);
                ERR=1;
            }

        }
        else
        {
            ERR=1;
            row.push_back(line[i]);
        }
    }
    else if(isalpha(words[i][0])||words[i][0]=='_')
    {
        word.t=words[i];
        word.m=0;
        sem[++i_sem]=word;
        ++i;
        if(words[i]=="=")
        {
            flag=words[i];
            ++i;
            E();
            quat(flag);
            if(words[i]==";")
            {
                ++i;
            }
            else
            {
                row.push_back(line[i]);
                ERR=1;
            }
        }
        else
        {
            ERR=1;
            row.push_back(line[i]);
        }
    }
    else if(words[i]=="{")
        Block();
    else
    {
        row.push_back(line[i]);
        ERR=1;
    }
}
void Bool()
{
    string flag;
    E();
    if(words[i]==">"||words[i]==">="||words[i]=="<"
       ||words[i]=="<="||words[i]=="=="||words[i]=="!=")
       {
            flag=words[i];
            ++i;

       }
    else
    {
        row.push_back(line[i]);
        ERR=1;
    }
    E();
    quat(flag);
}
void ELSE()
{
    if(words[i]=="else")
    {
        ++i;
        Stmt();
    }
}

void E()
{
    T();
    E1();
}
void E1()
{
    string flag;
    if(words[i]=="+"||words[i]=="-")
    {
        flag=words[i];
        ++i;
        T();
        quat(flag);
        E1();
    }
}
void T()
{
    F();

    T1();
}
void T1()
{
    string flag;
    if(words[i]=="*"||words[i]=="/")
    {
        flag=words[i];
        ++i;
        F();
        quat(flag);
        T1();
    }
}
void F()
{
     if(words[i]=="(")
      {
          ++i;
          E();
          if(words[i]==")")
             ++i;
          else
          {
             row.push_back(line[i]);
             ERR=1;
          }
      }
      else if(isdigit(words[i][0])||isalpha(words[i][0])||words[i][0]=='_')
      {
          word.t=words[i];
          word.m=0;
	      sem[++i_sem]=word;
          ++i;
      }
      else
      {
        row.push_back(line[i]);
        ERR=1;
      }
}

int splitword(vector<string>  &words)//分词
{
    int linenum=0;
    ifstream in;
    in.open("input1.txt",ios::in);
    if(!in)
    {
        cout << "打开文件出错！" <<endl;
        return -1;
    }
	string s;
	string temp="";
    while(getline(in,s))//逐行读入
    {
		int k=0;
		int flag=0;
		if(s[k]=='#')
            continue;
		while(s[k]!=0)//字符串不为空时
		{
			if(isalpha(s[k]))
            {
				if(s[k] >= 'A' && s[k] <= 'Z' )  s[k] -= -32;//如果此字符为大写字母，则转换为小写
				if(flag!= 1&&flag!= 2&&flag!=3)//flag为0可分词（出现空格），flag为4时出现其他字符进行分词
                {

                        words.push_back(temp);
                        line.push_back(linenum);
					    temp = "";

				}
				temp += s[k];
				flag = 1;//此时不能进行分词
			}
			else if(isdigit(s[k]))
            {
				if(flag != 1 && flag != 2&&flag!=3)
				{
					words.push_back(temp);
					line.push_back(linenum);
					temp = "";
				}
				temp += s[k];
				flag = 2;//此时不能进行分词
			}
			else if(s[k]=='_')
            {
				if(flag != 1 && flag != 2&&flag!=3)
				{
					words.push_back(temp);
					line.push_back(linenum);
					temp = "";
				}
				temp += s[k];
				flag = 3;//flag为1,2，3时不能进行分词，因为字母数字和下划线是组成标志符的元素
			}
			else if(s[k] == ' ' || s[k] == '\t')//词与词之间出现间隔，空格不计入分词结果
            {
				flag = 0;
			}
			else
            {
				if(flag != 4||s[k]!='=')//出现了(,[等一系列字符，进行分词.其中连等看为一个整体
                {
					words.push_back(temp);
					line.push_back(linenum);
					temp = "";

				}
				if(s[k]=='"'&&s[k+1]!='"')
                    break;
				temp += s[k];
				flag = 4;
			}
			k++;
		}
		linenum++;
    }
	in.close();
	words.push_back("}");
	line.push_back(linenum);
    words.push_back("#");
	return 0;
}


void  deleted(string &strr)
{
	int id;
	for(id=0;strr[id]!=0;id++)
    {
		if(strr[id]=='/'&&strr[id+1]=='/')//删除单行注释
        {
			strr[id]=0;
			strr[id+1]=0;
			for(int jd=id+2;strr[jd]!=0;jd++)
            {
                strr[jd]=0;
            }
			break;
		}
		if(strr[id]=='/'&&strr[id+1]=='*')//删除多行注释
        {
            flag2=1;
			strr[id]=0;
			strr[id+1]=0;
			for(int jd=id+2;strr[jd]!=0;jd++)
            {
                if(strr[jd]=='*'&&strr[jd+1]=='/')
                {
                     flag2=0;
                }
                strr[jd]=0;
            }
			break;
		}
		if(flag2==1)
        {
             if(!(strr[id]=='*'&&strr[id+1]=='/'))
             {
                 strr[id]=0;
             }
             else
             {
                 strr[id]=0;
                 strr[id+1]=0;
                 flag2=0;
                 break;
             }

        }
	}
}

int main()
{
    ifstream in;
    ofstream out;
    string str;
    int id;
    in.open("input.txt",ios::in);
    if(!in)
    {
       cout<<"Open the file failure..."<<endl;
       return -1;
    }
    out.open("input1.txt");
    while(getline(in,str))
    {
        deleted(str);
        out<<str<<endl;
    }
    splitword(words);
    int wh,wh2,iff;
    Block();
    if(ERR==0&&words[i]=="#")//输入的字符串被处理到最后
    {
        cout<<"success"<<endl;
        wh=0,wh2=0,iff=0;
        for (int ii=0;ii<num;ii++)
        {
              cout<<"("<<ii+1<<")";
              cout<<" ( "<<qt[ii].w;
              if (qt[ii].word1.t!="t")
                  cout<<" , "<<qt[ii].word1.t;
              else
	              cout<<" , "<<qt[ii].word1.t<<qt[ii].word1.m;
              if (qt[ii].word2.t!="t")
	              cout<<" , "<<qt[ii].word2.t;
              else
	              cout<<" , "<<qt[ii].word2.t<<qt[ii].word2.m;
              if(qt[ii].result.t=="goto")
                  cout<<" ,  ("<<ii+3<<") )"<<endl;
              else if(qt[ii].result.t=="goto while")
              {
                  cout<<" ,  ("<<whiletag1[wh2]+1<<") )"<<endl;
                  wh++;
              }
              else if(qt[ii].result.t=="goto}")
              {
                  cout<<" ,  ("<<whiletag2[wh]+1<<") )"<<endl;
                  wh++;
              }
              else if(qt[ii].result.t=="goto else")
              {
                  cout<<" ,  ("<<iftag[iff]+1<<") )"<<endl;
                  iff++;
              }
              else if(qt[ii].result.t!="t")
                  cout<<" ,  "<<qt[ii].result.t<<")"<<endl;
              else
                  cout<<" ,  "<<qt[ii].result.t<<qt[ii].result.m<<")"<<endl;

        }
    }
    else
    {
        cout<<"error"<<endl;
        cout<<"错误行号:"<<row[0]<<endl;
    }
    return 0;
}

