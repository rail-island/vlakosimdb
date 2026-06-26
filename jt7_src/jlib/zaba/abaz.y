%start module

%token tNUMBER tID tSTRING tREGEXP
%token kIF kELSE kDO kWHILE kFUNCTION kFOR kIN kVAR kNEW kMODULE
%token kSYSTEM kTHIS kRETURN kTRY kCATCH kSELECT kSWITCH kCASE kDEFAULT
%token kFINALLY kCLASS kREADONLY kREADWRITE kEXTENDS kBREAK kCONTINUE kCONSTRUCTOR
%token kPROPERTY kGET kSET kLIBRARY kPROGRAM kDELEGATE
%token kINC kDEC kGETELEM kGETPROP kLT kGT kLET kGET kEQ kNEQ kTESTEQ
%token kADDASSIGN kSUBASSIGN kMULASSIGN kDIVASSIGN kMODASSIGN
%token kAND kOR kNOT kFALSE kTRUE kNULL kTHROW kIMPORT kUSING kOPERATOR
%token kDIRECTLY kINDIRECTLY kOPTION kREQUIRES kYIELD kDETACH kIS kINSPECT
%token kINCLUDE kINCLUDABLE

%right '=' kADDASSIGN kSUBASSIGN kMULASSIGN kDIVASSIGN kMODASSIGN
%left kOR
%left kAND
%left kNOT
%left kIS
%nonassoc kLT kGT kLET kGET kEQ kNEQ kTESTEQ
%left '&'
%left '+' '-'
%left '*' '/' '%'
%%

exprelem : tNUMBER | tID | tSTRING | tREGEXP ;
exprelem : kSYSTEM {$$=new P_PushSys(); };
exprelem : kTHIS {$$=new P_PushThis(); };
exprelem : kFALSE {$$=new P_PushFalse(); };
exprelem : kTRUE {$$=new P_PushTrue(); };
exprelem : kNULL {$$=new P_PushNull(); };
exprelem : '(' comexpr ')' {$$=new P_ParExpr(retype(Expr,$2)); };
exprelem : '(' expr ')' {$$=new P_ParExpr(retype(Expr,$2)); };
exprelem : '[' params ']' {$$=new P_InlineArray(retype(Params,$2)); };

puredefexpr : puredefexpr '.' tID {$$=new P_Defer(retype(Expr,$1),retype(Id,$3));  };
puredefexpr : tID ;
puredefexpr : kSYSTEM {$$=new P_PushSys(); };
puredefexpr : kTHIS {$$=new P_PushThis(); };

defoper : tID ;
defoper : tSTRING ;

storeitem : tID '=' expr {$$=new P_StoreItem(retype(Id,$1),retype(Expr,$3)); };
storeitem : tSTRING '=' expr {$$=new P_StoreItem(retype(String,$1),retype(Expr,$3)); };

storeitems : storeitems ',' storeitem {retype(StoreItems,$1)->additem(retype(StoreItem,$3)); };
storeitems : storeitem {$$=new P_StoreItems();retype(StoreItems,$$)->additem(retype(StoreItem,$1)); };

defexpr : exprelem ;
defexpr : defexpr '.' tID {$$=new P_Defer(retype(Expr,$1),retype(Id,$3) ); };
defexpr : defexpr '[' params ']' {$$=new P_Index(retype(Expr,$1),retype(Params,$3));  };
defexpr : kNEW puredefexpr '(' params ')' {$$=new P_New(retype(Expr,$2),retype(Params,$4)); };
defexpr : defexpr '(' params ')' {$$=new P_Call(retype(Expr,$1),retype(Params,$3)); };
defexpr : kINC defexpr {$$=new P_PreUn(retype(Expr,$2),"inc"); };
defexpr : kDEC defexpr {$$=new P_PreUn(retype(Expr,$2),"dec"); };
defexpr : defexpr kINC {$$=new P_PostUn(retype(Expr,$1),"inc"); };
defexpr : defexpr kDEC {$$=new P_PostUn(retype(Expr,$1),"dec"); };
defexpr : defexpr kGETELEM defoper {$$=new P_DefOp(retype(Expr,$1),retype(Node,$3),"elem"); };
defexpr : defexpr kGETPROP defoper {$$=new P_DefOp(retype(Expr,$1),retype(Node,$3),"prop"); };
defexpr : kDELEGATE tID '(' expr ')' {$$=new P_Delegate(retype(Id,$2),retype(Expr,$4)); };
defexpr : kNEW '(' storeitems ')' {$$=new P_NewStore(retype(StoreItems,$3)); };
defexpr : kDETACH puredefexpr '.' tID '(' params ')' {$$=new P_Detach(retype(Expr,$2),retype(Id,$4),retype(Params,$6)); };

comexpr : block {$$=new P_ComExpr(retype(Command,$1)); };
comexpr : '{' expr '}' {$$=new P_RetComExpr(retype(Expr,$2)); };

param : comexpr ;
param : expr ;

params : params ',' param {(retype(Params,$1))->addparam(retype(Expr,$3)); };
params : param {$$=new P_Params(retype(Expr,$1)); };
params : {$$=new P_Params(); };

priexpr : priexpr '+' priexpr {$$=new P_Operator(retype(Expr,$1),retype(Expr,$3),"add"); };
priexpr : priexpr '-' priexpr {$$=new P_Operator(retype(Expr,$1),retype(Expr,$3),"sub"); };
priexpr : priexpr '*' priexpr {$$=new P_Operator(retype(Expr,$1),retype(Expr,$3),"mul"); };
priexpr : priexpr '/' priexpr {$$=new P_Operator(retype(Expr,$1),retype(Expr,$3),"div"); };
priexpr : priexpr '%' priexpr {$$=new P_Operator(retype(Expr,$1),retype(Expr,$3),"mod"); };
priexpr : priexpr '&' priexpr {$$=new P_Operator(retype(Expr,$1),retype(Expr,$3),"cat"); };

priexpr : '-' priexpr %prec '*' {$$=new P_Minus(retype(Expr,$2)); };

priexpr : priexpr kGT priexpr {$$=new P_Operator(retype(Expr,$1),retype(Expr,$3),"gt"); };
priexpr : priexpr kLT priexpr {$$=new P_Operator(retype(Expr,$1),retype(Expr,$3),"lt"); };
priexpr : priexpr kGET priexpr {$$=new P_Operator(retype(Expr,$1),retype(Expr,$3),"get"); };
priexpr : priexpr kLET priexpr {$$=new P_Operator(retype(Expr,$1),retype(Expr,$3),"let"); };
priexpr : priexpr kEQ priexpr {$$=new P_Operator(retype(Expr,$1),retype(Expr,$3),"eq"); };
priexpr : priexpr kNEQ priexpr {$$=new P_Operator(retype(Expr,$1),retype(Expr,$3),"neq"); };
priexpr : priexpr kTESTEQ priexpr {$$=new P_TestEq(retype(Expr,$1),retype(Expr,$3)); };

priexpr : priexpr kIS priexpr {$$=new P_Is(retype(Expr,$1),retype(Expr,$3)); };

priexpr : priexpr '=' priexpr {$$=new P_Assign(retype(Expr,$1),retype(Expr,$3)); };
priexpr : priexpr kADDASSIGN priexpr {$$=new P_AssignOp(retype(Expr,$1),retype(Expr,$3),"add"); };
priexpr : priexpr kSUBASSIGN priexpr {$$=new P_AssignOp(retype(Expr,$1),retype(Expr,$3),"sub"); };
priexpr : priexpr kMULASSIGN priexpr {$$=new P_AssignOp(retype(Expr,$1),retype(Expr,$3),"mul"); };
priexpr : priexpr kDIVASSIGN priexpr {$$=new P_AssignOp(retype(Expr,$1),retype(Expr,$3),"div"); };
priexpr : priexpr kMODASSIGN priexpr {$$=new P_AssignOp(retype(Expr,$1),retype(Expr,$3),"mod"); };

priexpr : defexpr ;

priexpr : priexpr kAND priexpr {$$=new P_And(retype(Expr,$1),retype(Expr,$3)); };
priexpr : priexpr kOR priexpr {$$=new P_Or(retype(Expr,$1),retype(Expr,$3)); };
priexpr : kNOT priexpr {$$=new P_Not(retype(Expr,$2)); };

condexpr : priexpr '?' priexpr ':' priexpr {$$=new P_CondExpr(retype(Expr,$1),retype(Expr,$3),retype(Expr,$5)); };
condexpr : priexpr ;

expr : condexpr ;

varexpr : expr {$$=new P_SimpleVarExpr(retype(Expr,$1)); };
varexpr : kVAR tID '=' expr {$$=new P_IdVarExpr(retype(Id,$2),retype(Expr,$4)); };

commandelem : expr ';' {$$=new P_ExprCom(retype(Expr,$1)); };
commandelem : kIF '(' varexpr ')' command {$$=new P_If(retype(VarExpr,$3),retype(Command,$5)); };
commandelem : kIF '(' varexpr ')' command kELSE command {$$=new P_IfElse(retype(VarExpr,$3),retype(Command,$5),retype(Command,$7)); };
commandelem : kFOR '(' varexpr ';' expr ';' expr ')' command {$$=new P_For(retype(VarExpr,$3),retype(Expr,$5),retype(Expr,$7),retype(Command,$9)); };
commandelem : kFOR '(' tID kIN expr ')' command {$$=new P_ForEach(retype(Id,$3),retype(Expr,$5),retype(Command,$7)); };
commandelem : kWHILE '(' varexpr ')' command {$$=new P_While(retype(VarExpr,$3),retype(Command,$5)); };
commandelem : kDO command '(' expr ')' {$$=new P_DoWhile(retype(Expr,$4),retype(Command,$2)); };
commandelem : kRETURN ';' {$$=new P_RetVoid(); };
commandelem : kRETURN expr ';' {$$=new P_RetValue(retype(Expr,$2)); };
commandelem : kYIELD ';' {$$=new P_YieldVoid(); };
commandelem : kYIELD expr ';' {$$=new P_YieldValue(retype(Expr,$2)); };
commandelem : kTRY block kCATCH '(' tID ')' block {$$=new P_TryCatch(retype(Block,$2),retype(Id,$5),retype(Block,$7)); };
commandelem : kTRY block kFINALLY block {$$=new P_TryFinally(retype(Block,$2),retype(Block,$4)); };
commandelem : kTRY block kCATCH '(' tID ')' block kFINALLY block {$$=new P_TryFinally(new P_TryCatch(retype(Block,$2),retype(Id,$5),retype(Block,$7)),retype(Block,$9)); };
commandelem : kBREAK ';' {$$=new P_Break(); };
commandelem : kCONTINUE ';' {$$=new P_Continue(); };
commandelem : kBREAK tID ';' {$$=new P_Break(retype(Id,$2)); };
commandelem : kCONTINUE tID ';' {$$=new P_Continue(retype(Id,$2)); };
commandelem : kTHROW expr ';' {$$=new P_Throw(retype(Expr,$2)); };
commandelem : block ;
commandelem : select ;
commandelem : inspect ;
commandelem : switch ;
commandelem : kDIRECTLY commandelem {$$=new P_Directly(true,retype(Command,$2)); };
commandelem : kINDIRECTLY commandelem {$$=new P_Directly(false,retype(Command,$2)); };
commandelem : optioncommand ;

optioncommand : kOPTION kDIRECTLY ';' {$$=new P_DirectCommand(true); };
optioncommand : kOPTION kINDIRECTLY ';' {$$=new P_DirectCommand(false); };

selectitems : selectitems selectitem {retype(SelectItems,$1)->additem(retype(SelectItem,$2)); };
selectitems : {$$=new P_SelectItems(); };

selectitem : expr ':' commandelem {$$=new P_SelectItem(retype(Expr,$1),retype(Command,$3)); };

inspectitems : inspectitems inspectitem {retype(InspectItems,$1)->additem(retype(InspectItem,$2)); };
inspectitems : {$$=new P_InspectItems(); };

inspectitem : expr ':' commandelem {$$=new P_InspectItem(retype(Expr,$1),retype(Command,$3)); };

switchitems : switchitems switchitem {retype(SwitchItems,$1)->additem(retype(SwitchItem,$2)); }
switchitems : {$$=new P_SwitchItems(); }

switchitem : kCASE expr ':' {$$=new P_SwitchCase(retype(Expr,$3)); };
switchitem : commandelem {$$=new P_SwitchCommand(retype(Command,$1)); }
switchitem : kDEFAULT ':' {$$=new P_SwitchDefault(); };

selectelse : {$$=NULL; };
selectelse : kELSE command {$$=new P_SelectElse(retype(Command,$2)); };

inspectelse : {$$=NULL; };
inspectelse : kELSE command {$$=new P_InspectElse(retype(Command,$2)); };

inspect : kINSPECT '(' expr ')' '{' inspectitems inspectelse '}' {$$=new P_Inspect(retype(Expr,$3),retype(InspectItems,$6),retype(InspectElse,$7)); };

select : kSELECT '(' expr ')' '{' selectitems selectelse '}' {$$=new P_Select(retype(Expr,$3),retype(SelectItems,$6),retype(SelectElse,$7)); };

switch : kSWITCH '(' expr ')' '{' switchitems '}' {$$=new P_Switch(retype(Expr,$3),retype(SwitchItems,$6)); };

vardefs : vardefs ',' vardef {(retype(VarDefs,$1))->addvardef(retype(VarDef,$3)); };
vardefs : vardef {$$=new P_VarDefs(retype(VarDef,$1)); };

vardef : tID '=' expr {$$=new P_VarDefInit(retype(Id,$1),retype(Expr,$3)); };
vardef : tID {$$=new P_VarDefVoid(retype(Id,$1)); };

varcommandelem : kVAR vardefs ';' {$$=new P_VarCommand(retype(VarDefs,$2)); };

varcommand : varcommandelem ;
varcommand : kDIRECTLY varcommandelem {$$=new P_Directly(true,retype(Command,$2)); };
varcommand : kINDIRECTLY varcommandelem {$$=new P_Directly(false,retype(Command,$2)); };

label : tID ':' ;

novarcommand : commandelem ;
novarcommand : label commandelem {$$=$2;retype(Command,$$)->setlabel(retype(Id,$1)); };

command : varcommand ;
command : novarcommand ;

commands : commands command {(retype(Commands,$1))->addcommand(retype(Command,$2)); };
commands : {$$=new P_Commands(); };

block : '{' commands '}' {$$=new P_Block(retype(Commands,$2)); };
block : '{' commands error {syntaxerror("'}' expected"); };
block : '{' error {syntaxerror("Command expected"); };

readonly : kREADONLY vardefs ';' {$$=new P_Readonly(retype(VarDefs,$2)); };

readwrite : kREADWRITE vardefs ';' {$$=new P_Readwrite(retype(VarDefs,$2)); };

getspec : kGET block {$$=new P_GetPropSpec(retype(Block,$2)); };
getspec : {$$=new P_GetPropSpec(); };

setspec : kSET block {$$=new P_SetPropSpec(retype(Block,$2)); };
setspec : {$$=new P_SetPropSpec(); };

property : kPROPERTY tID getspec setspec {$$=new P_Property(retype(Id,$2),retype(GetPropSpec,$3),retype(SetPropSpec,$4)); };

classelem : novarcommand {$$=new P_ElemCommand(retype(Command,$1)); };
classelem : flddef ;
classelem : funcdef ;
classelem : constructor ;
classelem : classdef ;
classelem : readonly ;
classelem : readwrite ;
classelem : property ;
classelem : import ;
classelem : operdef ;

moduleelem : novarcommand {$$=new P_ElemCommand(retype(Command,$1)); };
moduleelem : includemodule ;
moduleelem : flddef ;
moduleelem : funcdef ;
moduleelem : classdef ;
moduleelem : readonly ;
moduleelem : readwrite ;
moduleelem : property ;
moduleelem : import ;

import : kIMPORT expr ';' {$$=new P_Import(retype(Expr,$2)); };

flddef : kVAR vardefs ';' {$$=new P_FldDef(retype(VarDefs,$2)); };

noemptyarglist : noemptyarglist ',' argument {retype(ArgList,$1)->addarg(retype(Argument,$3)); };
noemptyarglist : argument {$$=new P_ArgList();retype(ArgList,$$)->addarg(retype(Argument,$1)); };
emptyarglist : {$$=new P_ArgList(); };
arglist : noemptyarglist | emptyarglist ;

argument : tID {$$=new P_Argument(retype(Id,$1)); };

funcelems : funcelems command {retype(FuncElems,$1)->addcommand(retype(Command,$2)); };
funcelems : funcelems funcdef {retype(FuncElems,$1)->addfunc(retype(Function,$2)); };
funcelems : {$$=new P_FuncElems(); };

funcdef : kFUNCTION tID '(' arglist ')' '{' funcelems '}' {$$=new P_Function(retype(Id,$2),retype(ArgList,$4),retype(FuncElems,$7)); };

operbody : '{' funcelems '}' {$$=$2; };

operdef : kOPERATOR '+' '(' tID ')' operbody {$$=makefn1("op:add",retype(Id,$4),retype(FuncElems,$6)); };
operdef : kOPERATOR '-' '(' tID ')' operbody {$$=makefn1("op:sub",retype(Id,$4),retype(FuncElems,$6)); };
operdef : kOPERATOR '*' '(' tID ')' operbody {$$=makefn1("op:mul",retype(Id,$4),retype(FuncElems,$6)); };
operdef : kOPERATOR '/' '(' tID ')' operbody {$$=makefn1("op:div",retype(Id,$4),retype(FuncElems,$6)); };
operdef : kOPERATOR '%' '(' tID ')' operbody {$$=makefn1("op:mod",retype(Id,$4),retype(FuncElems,$6)); };

operdef : kOPERATOR kEQ '(' tID ')' operbody {$$=makefn1("op:eq",retype(Id,$4),retype(FuncElems,$6)); };
operdef : kOPERATOR kNEQ '(' tID ')' operbody {$$=makefn1("op:neq",retype(Id,$4),retype(FuncElems,$6)); };
operdef : kOPERATOR kLT '(' tID ')' operbody {$$=makefn1("op:lt",retype(Id,$4),retype(FuncElems,$6)); };
operdef : kOPERATOR kGT '(' tID ')' operbody {$$=makefn1("op:gt",retype(Id,$4),retype(FuncElems,$6)); };
operdef : kOPERATOR kLET '(' tID ')' operbody {$$=makefn1("op:let",retype(Id,$4),retype(FuncElems,$6)); };
operdef : kOPERATOR kGET '(' tID ')' operbody {$$=makefn1("op:get",retype(Id,$4),retype(FuncElems,$6)); };

operget : kGET operbody {$$=new P_OperGet(retype(FuncElems,$2)); };
operget : {$$=NULL;}

operset : kSET operbody {$$=new P_OperSet(retype(FuncElems,$2)); };
operset : {$$=NULL;}

operdef : kOPERATOR kGETPROP '(' tID ')' operget operset {$$=new P_DefOpDef("prop",retype(Id,$4),retype(OperGet,$6),retype(OperSet,$7)); };
operdef : kOPERATOR kGETELEM '(' tID ')' operget operset {$$=new P_DefOpDef("elem",retype(Id,$4),retype(OperGet,$6),retype(OperSet,$7)); };

operdef : kOPERATOR kINC '(' ')' operbody {$$=makefn0("op:inc",retype(FuncElems,$5)); };
operdef : kOPERATOR kDEC '(' ')' operbody {$$=makefn0("op:dec",retype(FuncElems,$5)); };
operdef : kOPERATOR '-' '(' ')' operbody {$$=makefn0("op:minus",retype(FuncElems,$5)); };

operdef : kOPERATOR '[' arglist ']' operget operset {$$=new P_IndexOpDef(retype(ArgList,$3),retype(OperGet,$5),retype(OperSet,$6)); };
operdef : kOPERATOR '(' arglist ')' operbody {$$=new P_Function(new P_Id("op:call"),retype(ArgList,$3),retype(FuncElems,$5)); };

includemodule : kINCLUDE kMODULE tID ';' {$$=new P_IncludeModule(retype(Id,$3)); };

//funcdef : kFUNCTION tID '(' arglist ')' '{' nestedfuncs commands '}' {$$=new P_Function(retype(Id,$2),retype(ArgList,$4),retype(Block,$6)); };

/*
funcdef : kFUNCTION tID '(' arglist ')' error {syntaxerror("block expected"); };
funcdef : kFUNCTION tID '(' arglist error {syntaxerror("')' expected"); };
funcdef : kFUNCTION tID '(' error {syntaxerror("Function argument list expected"); };
funcdef : kFUNCTION tID error {syntaxerror("'(' expected"); };
funcdef : kFUNCTION error {syntaxerror("Identifier expected"); };
*/

constructor : kCONSTRUCTOR '(' arglist ')' block {$$=new P_Constructor(retype(ArgList,$3),retype(Block,$5)); };

classelems : classelems classelem {retype(ClassElems,$1)->addelem(retype(ClassElem,$2)); };
classelems : {$$=new P_ClassElems(); };

moduleelems : moduleelems moduleelem {retype(ModuleElems,$1)->addelem(retype(ModuleElem,$2)); };
moduleelems : {$$=new P_ModuleElems(); };
moduleelems : error {syntaxerror("Unrecognized syntax error"); };

modulename : {$$=new P_ModuleName(NULL,32); };
modulename : kMODULE tID ';' {$$=new P_ModuleName(retype(Id,$2),32); };
modulename : kSYSTEM kMODULE tID ';' {$$=new P_ModuleName(retype(Id,$3),0); };
modulename : kPROGRAM kMODULE tID ';' {$$=new P_ModuleName(retype(Id,$3),48); };
modulename : kLIBRARY kMODULE tID ';' {$$=new P_ModuleName(retype(Id,$3),16); };
modulename : kINCLUDABLE kMODULE tID ';' {$$=new P_ModuleName(retype(Id,$3),-1); };
modulename : kMODULE error {syntaxerror("Missing module name"); };

ancestor : kEXTENDS tID {$$=new P_Ancestor(retype(Id,$2)); };
ancestor : {$$=new P_Ancestor(NULL); };

classdef : kCLASS tID ancestor '{' classelems '}' {$$=new P_Class(retype(Id,$2),retype(Ancestor,$3),retype(ClassElems,$5)); };

usings : {$$=new P_Usings(); };
usings : usings kUSING kLIBRARY tSTRING ';' {retype(Usings,$1)->uselib(retype(String,$4)); };
usings : usings kUSING kMODULE tSTRING ';' {retype(Usings,$1)->usemod(retype(String,$4)); };
usings : usings kREQUIRES kMODULE tID ';' {retype(Usings,$1)->reqmod(retype(Id,$4)); };

module : modulename usings moduleelems {lex_result=new P_Module(retype(ModuleName,$1),retype(Usings,$2),retype(ModuleElems,$3)); };
%%
