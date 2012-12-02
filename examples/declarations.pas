program declarations;

var i:integer;

function test(x:integer; y:real):integer;
var z:integer;
begin
  x:= 1;
  y:= 2;
  z := 1;
  test := x + y + z;
end;

procedure cp(x:integer; z:real);
var y:integer;
begin
    x:=1;
    z:=2;
    z := x - y;
end;

begin
  i := 1 +2 +3;
end.

