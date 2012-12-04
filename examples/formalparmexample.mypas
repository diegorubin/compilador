program teste;

var testvarint: integer;
{ #in the symbol table:
   || SYMBOL     ||OBJTYPE ||DATATYPE|| FORMPARMS || TYPE#1 || TYPE#2 || TYPE#3  || TYPE#4
   ||            ||        ||        ||  OFFSET   ||        ||        ||         ||
   ||  ...       ||  ...   ||  ...   ||    ...    ||  ...   ||  ...   ||  ...    || ...    
1  ||"testvarint"|| global ||long(4) || 
}
    testvardbl: real;
{ #in the symbol table:
   || SYMBOL     ||OBJTYPE ||DATATYPE || FORMPARMS || TYPE#1 || TYPE#2 || TYPE#3  || TYPE#4
   ||            ||        ||         ||  OFFSET   ||        ||        ||         ||
   ||  ...       ||  ...   ||  ...    ||    ...    ||  ...   ||  ...   ||  ...    || ...    
2  ||"testvardbl"|| global ||double(8)||
}

procedure A(x,y:integer; z,t: real);
{

in the object code:

    .type A,@function
A:
    pushl %ebp
    movl %esp,%ebp
    
in the symbol table:

   || SYMBOL || OBJTYPE   ||DATATYPE|| FORMPARMS || TYPE#1 || TYPE#2 || TYPE#3  || TYPE#4
   ||        ||           ||        ||  OFFSET   ||        ||        ||         ||
   ||  ...   ||  ...      ||  ...   ||    ...    ||  ...   ||  ...   ||  ...    || ...    
n  ||   "A"  || procedure ||  void  ||    3      || long(4)|| long(4)||double(8)||double(8)
n+1||   "x"  ||   parm    || long(4)||    8
n+2||   "y"  ||   parm    || long(4)|| 4 + 8 = 12||
n+3||   "z"  ||   parm    || double || 4 + 12= 16||
n+4||   "t"  ||   parm    || double || 8 + 16= 24||

}
  var i,j: integer;
{
n+5||   "i"  ||  local    || long(4)|| -4
n+6||   "j"  ||  local    || long(4)|| -4-4=-8
}
      w,v: real;
{
n+7||   "w"  ||  local    || double || -8-8=-16
n+8||   "v"  ||  local    || double || -16-8=-24
}
  begin
    i := x + y;
    {
    #in the object code:
    
    addl $-24,%esp
    
    movl 8(%ebp),%eax
    addl 12(%ebp),%eax
    movl %eax,-4(%ebp)
    }
    j := x - y;
    {
    #in the object code:
    
    movl 8(%ebp),%eax
    subl 12(%ebp),%eax
    movl %eax,-8(%ebp)
    }  
    w := z * t;
    {
    #in the object code:
    
    fmovq 16(%ebp),%st0
    fmulq 24(%ebp),%st0
    fmovl %st0,-4(%ebp)
    }
    v := z / t;
    {
    #in the object code:
    
    fmovq 16(%ebp),%st0
    fdivq 24(%ebp),%st0
    fmovl %st0,-4(%ebp)
    }
    
  end;
  {
  #in the code:
  
    movl %ebp,%esp
    popl %ebp
    ret
  
  }
{
in the symbol table:

symtab_release(n+1, n+8)

}
function B(N:integer): integer;
{

in the object code:

    .type B,@function
B:
    pushl %ebp
    movl %esp,%ebp
    
in the symbol table:

   || SYMBOL || OBJTYPE   ||DATATYPE|| FORMPARMS || TYPE#1 || TYPE#2 || TYPE#3  || TYPE#4
   ||        ||           ||        ||  OFFSET   ||        ||        ||         ||
   ||  ...   ||  ...      ||  ...   ||    ...    ||  ...   ||  ...   ||  ...    || ...    
n  ||   "A"  || procedure ||  void  ||    3      || long(4)|| long(4)||double(8)||double(8)
n+1||   "B"  || function  || long(4)||    2      || long(4)|| long(4)
n+2||   "N"  ||   parm    || long(4)||    8      ||

}
  begin
    if (N < 2) then
    {
      movl 8(%ebp),%eax
      pushl %eax
      movl $2,%eax
      cmpl (%esp),%eax
      jge .L1
    }
      begin
        B:= 1
        {       
        movl $1,%eax
        jmp B.conclusion
        }
      end
    else
      {
      jmp .L2
.L1:
      }
      B := N * B(N - 1)
      {
      #in the object code:
      
      movl 8(%ebp),%eax
      pushl %eax
      
      mvl 8(%ebp),%eax
      subl $1,%eax
      
      pushl %eax
      call B
      addl $4,%esp
      
      mull (%esp)
      
      jmp B.conclusion
.L2:
      }
  end;
  {
B.conclusion:
    movl %ebp,%esp
    popl %ebp
    ret
   }
{
in the symbol table:

symtab_release(n+2, n+2)

}

{#in the object code:
    .type _start,@function
    .global _start
_start:
}
begin

end.
{
#_start.success:
    movl $0,%ebx
    movl $1,%eax
    int 0x80
}
