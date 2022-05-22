/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object* obj);
void freeScope(Scope* scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab* symtab;
Type* intType;
Type* charType;

/******************* Type utilities ******************************/

Type* makeIntType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type* makeCharType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type* makeArrayType(int arraySize, Type* elementType) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type* duplicateType(Type* type) {
	// Type * t = (Type*) malloc(sizeof(Type));
	// t-> typeClass = type->typeClass;
	// t->arraySize = type->arraySize;
	// t->elementType = type->elementType;
  // 	return t;
  // TODO
  switch(type->typeClass){
    case TP_INT:
      return makeIntType();
      break;
    case TP_CHAR:
      return makeCharType();
      break;
    case TP_ARRAY:
      return NULL;
      break;
    default:
      return NULL;
  }
}

int compareType(Type* type1, Type* type2) {
  // TODO
  // return type1->typeClass == type2->typeClass ? 1 : 0;
  if(type1->typeClass == type2->typeClass){
    return 1;
  }
  return 0;
  
}

void freeType(Type* type) {
  // TODO
  if(type->typeClass == TP_ARRAY && type->typeClass != NULL){
    freeType(type->elementType);
  }
  free(type);
}

/******************* Constant utility ******************************/

ConstantValue* makeIntConstant(int i) {
	ConstantValue *c = (ConstantValue*)malloc(sizeof(ConstantValue));
	c->type = TP_INT;
	c->intValue = i;
	return c;
  // TODO
}

ConstantValue* makeCharConstant(char ch) {
  // TODO
  ConstantValue *c = (ConstantValue*)malloc(sizeof(ConstantValue));
  c->type = TP_CHAR;
  c->charValue = ch;
  return c;
}

ConstantValue* duplicateConstantValue(ConstantValue* v) {
  // TODO
  ConstantValue *c = (ConstantValue*)malloc(sizeof(ConstantValue));
  c->type = v->type;
  switch(v->type){
    case TP_INT:
      c->intValue = v->intValue;
      break;
    case TP_CHAR:
      c->charValue = v->charValue;
      break;
    case TP_ARRAY:
      return NULL;
      break;
    default:
      return NULL;
  }
  return c;
}

/******************* Object utilities ******************************/

Scope* createScope(Object* owner, Scope* outer) {
  Scope* scope = (Scope*) malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

Object* createProgramObject(char *programName) {
  Object* program = (Object*) malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes*) malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = createScope(program,NULL);
  symtab->program = program;
  return program;
}

Object* createConstantObject(char *name) {
  // TODO
  Object* constant = (Object*)malloc(sizeof(Object));
  strcpy(constant->name,name);
  constant->kind = OBJ_CONSTANT;
  constant->constAttrs = (ConstantAttributes*)malloc(sizeof(ConstantAttributes));
  constant->constAttrs->value = (ConstantValue*)malloc(sizeof(ConstantValue));
  return constant;
}

Object* createTypeObject(char *name) {
  // TODO
  Object* type = (Object*)malloc(sizeof(Object));
  strcpy(type->name, name);
  type->kind = OBJ_TYPE;
  type->typeAttrs = (TypeAttributes*)malloc(sizeof(TypeAttributes));
  return type;
}

Object* createVariableObject(char *name) {
  // TODO
  Object* var = (Object*)malloc(sizeof(Object));
  strcpy(var->name, name);
  var->kind = OBJ_VARIABLE;
  var->varAttrs = (VariableAttributes*)malloc(sizeof(VariableAttributes));
  var->varAttrs->scope = createScope(var, symtab->currentScope);
  return var;
}

Object* createFunctionObject(char *name) {
  // TODO
  Object* func = (Object*)malloc(sizeof(Object));
  strcpy(func->name, name);
  func->kind = OBJ_FUNCTION;
  func->funcAttrs = (FunctionAttributes*)malloc(sizeof(FunctionAttributes));
  func->funcAttrs->paramList = NULL;
  func->funcAttrs->scope = createScope(func, symtab->currentScope);
  return func;
}

Object* createProcedureObject(char *name) {
  // TODO
  Object* procedure  = (Object*)malloc(sizeof(Object));
  strcpy(procedure->name, name);
  procedure->kind = OBJ_PROCEDURE;
  procedure->progAttrs = (ProcedureAttributes*)malloc(sizeof(ProcedureAttributes));
  procedure->procAttrs->paramList = NULL;
  procedure->procAttrs->scope = createScope(procedure, symtab->currentScope);
  return procedure;
}

Object* createParameterObject(char *name, enum ParamKind kind, Object* owner) {
  // TODO
  Object* paramObj = (Object*)malloc(sizeof(Object));
  strcpy(paramObj->name, name);
  paramObj->kind = OBJ_PARAMETER;
  paramObj->paramAttrs = (ParameterAttributes*)malloc(sizeof(ParameterAttributes));
  paramObj->paramAttrs->kind = kind;
  paramObj->paramAttrs->function = NULL;
  return paramObj;
}

void freeObject(Object* obj) {
  // TODO
  switch(obj->kind){
    case OBJ_PARAMETER:
      freeType(obj->paramAttrs->type);
      free(obj->paramAttrs);
      break;
    case OBJ_PROCEDURE:
      freeObjectList(obj->procAttrs->paramList);
      freeScope(obj->procAttrs);
      break;
    case OBJ_FUNCTION:
      freeObjectList(obj->funcAttrs->paramList);
      freeType(obj->funcAttrs->returnType);
      freeScope(obj->funcAttrs->scope);
      free(obj->funcAttrs);
      break;
    case OBJ_VARIABLE:
      freeType(obj->varAttrs->type);
      freeScope(obj->varAttrs->scope);
      free(obj->varAttrs);
      break;
    case OBJ_TYPE:
      freeType(obj->typeAttrs->actualType);
      free(obj->typeAttrs);
      break;
    case OBJ_CONSTANT:
      free(obj->constAttrs->value);
      free(obj->constAttrs);
      break;
    case OBJ_PROGRAM:
      freeScope(obj->progAttrs->scope);
      free(obj->progAttrs);
      break;
  }
}

void freeScope(Scope* scope) {
  // TODO
  freeObjectList(scope->objList);
  free(scope);
}

void freeObjectList(ObjectNode *objList) {
  // TODO
  if(objList->next != NULL){
    freeObjectList(objList->next);
  }
  freeObject(objList->object);
  free(objList);
}

void freeReferenceList(ObjectNode *objList) {
  // TODO

}

void addObject(ObjectNode **objList, Object* obj) {
  ObjectNode* node = (ObjectNode*) malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL) 
    *objList = node;
  else {
    ObjectNode *n = *objList;
    while (n->next != NULL) 
      n = n->next;
    n->next = node;
  }
}

Object* findObject(ObjectNode *objList, char *name) {
  // TODO
  ObjectNode* p = objList;
  while (p != NULL){
    if(strcmp(p->object->name, name) == 0) return p->object;
  }
  return NULL;  
}

/******************* others ******************************/

void initSymTab(void) {
  Object* obj;
  Object* param;

  symtab = (SymTab*) malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;
  
  obj = createFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEI");
  param = createParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEC");
  param = createParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITELN");
  addObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void) {
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

void enterBlock(Scope* scope) {
  symtab->currentScope = scope;
}

void exitBlock(void) {
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object* obj) {
  if (obj->kind == OBJ_PARAMETER) {
    Object* owner = symtab->currentScope->owner;
    switch (owner->kind) {
    case OBJ_FUNCTION:
      addObject(&(owner->funcAttrs->paramList), obj);
      break;
    case OBJ_PROCEDURE:
      addObject(&(owner->procAttrs->paramList), obj);
      break;
    default:
      break;
    }
  }
 
  addObject(&(symtab->currentScope->objList), obj);
}


