#ifndef TRANSLATE_H_
#define TRANSLATE_H_

class GlobalState {
public:
  std::unordered_map<std::string, LLVMValueRef> functions;
};

class FunctionState {
public:
  LLVMValueRef containingFunc;
  std::unordered_map<int, LLVMValueRef> localAddrByLocalId;
  int nextBlockNumber = 1;
  LLVMBuilderRef builder;

  FunctionState(
      LLVMValueRef containingFunc_,
      LLVMBuilderRef builder_) :
      containingFunc(containingFunc_),
      builder(builder_) {}
};

LLVMTypeRef translateType(Reference* referenceM) {
  if (referenceM->ownership == Ownership::SHARE && dynamic_cast<Int*>(referenceM->referend) != nullptr) {
    return LLVMInt64Type();
  } else {
    assert(false);
    return nullptr;
  }
}

std::vector<LLVMTypeRef> translateTypes(std::vector<Reference*> referencesM) {
  std::vector<LLVMTypeRef> result;
  for (auto referenceM : referencesM) {
    result.push_back(translateType(referenceM));
  }
  return result;
}

LLVMValueRef translateExpression(
    GlobalState* globalState,
    FunctionState* functionState,
    Expression* expr);
LLVMValueRef translateCall(
    GlobalState* globalState,
    FunctionState* functionState,
    Call* call);
LLVMValueRef translateExternCall(
    GlobalState* globalState,
    FunctionState* functionState,
    ExternCall* expr);


LLVMValueRef translateExternCall(
    GlobalState* globalState,
    FunctionState* functionState,
    ExternCall* call) {
  auto name = call->function->name->name;
  if (name == "F(\"__addIntInt\",[],[R(*,i),R(*,i)])") {
    assert(call->argExprs.size() == 2);
    return LLVMBuildAdd(
        functionState->builder,
        translateExpression(globalState, functionState, call->argExprs[0]),
        translateExpression(globalState, functionState, call->argExprs[1]),
        "add");
  } else if (name == "F(\"__addFloatFloat\",[],[R(*,f),R(*,f)])") {
    // VivemExterns.addFloatFloat
    assert(false);
  } else if (name == "F(\"panic\")") {
    // VivemExterns.panic
    assert(false);
  } else if (name == "F(\"__multiplyIntInt\",[],[R(*,i),R(*,i)])") {
    assert(call->argExprs.size() == 2);
    return LLVMBuildMul(
        functionState->builder,
        translateExpression(globalState, functionState, call->argExprs[0]),
        translateExpression(globalState, functionState, call->argExprs[1]),
        "mul");
  } else if (name == "F(\"__subtractIntInt\",[],[R(*,i),R(*,i)])") {
    // VivemExterns.subtractIntInt
    assert(false);
  } else if (name == "F(\"__addStrStr\",[],[R(*,s),R(*,s)])") {
    // VivemExterns.addStrStr
    assert(false);
  } else if (name == "F(\"__getch\")") {
    // VivemExterns.getch
    assert(false);
  } else if (name == "F(\"__lessThanInt\",[],[R(*,i),R(*,i)])") {
    // VivemExterns.lessThanInt
    assert(false);
  } else if (name == "F(\"__greaterThanOrEqInt\",[],[R(*,i),R(*,i)])") {
    // VivemExterns.greaterThanOrEqInt
    assert(false);
  } else if (name == "F(\"__eqIntInt\",[],[R(*,i),R(*,i)])") {
    // VivemExterns.eqIntInt
    assert(false);
  } else if (name == "F(\"__eqBoolBool\",[],[R(*,b),R(*,b)])") {
    // VivemExterns.eqBoolBool
    assert(false);
  } else if (name == "F(\"__print\",[],[R(*,s)])") {
    // VivemExterns.print
    assert(false);
  } else if (name == "F(\"__not\",[],[R(*,b)])") {
    // VivemExterns.not
    assert(false);
  } else if (name == "F(\"__castIntStr\",[],[R(*,i)])") {
    // VivemExterns.castIntStr
    assert(false);
  } else if (name == "F(\"__and\",[],[R(*,b),R(*,b)])") {
    // VivemExterns.and
    assert(false);
  } else if (name == "F(\"__mod\",[],[R(*,i),R(*,i)])") {
    // VivemExterns.mod
    assert(false);
  } else {
    assert(false);
  }
}

std::vector<LLVMValueRef> translateExpressions(
    GlobalState* globalState,
    FunctionState* functionState,
    std::vector<Expression*> exprs) {
  auto result = std::vector<LLVMValueRef>{};
  result.reserve(exprs.size());
  for (auto expr : exprs) {
    result.push_back(translateExpression(globalState, functionState, expr));
  }
  return result;
}

LLVMValueRef translateCall(
    GlobalState* globalState,
    FunctionState* functionState,
    Call* call) {
  auto funcIter = globalState->functions.find(call->function->name->name);
  assert(funcIter != globalState->functions.end());
  auto funcL = funcIter->second;

  auto argExprsL = translateExpressions(globalState, functionState, call->argExprs);
  return LLVMBuildCall(functionState->builder, funcL, argExprsL.data(), argExprsL.size(), "");
}

LLVMValueRef translateExpression(
    GlobalState* globalState,
    FunctionState* functionState,
    Expression* expr) {
  if (auto constantI64 = dynamic_cast<ConstantI64*>(expr)) {
    return LLVMConstInt(LLVMInt64Type(), constantI64->value, false);
  } else if (auto discard = dynamic_cast<Discard*>(expr)) {
    std::cout << "do some cleanup here" << std::endl;
    LLVMValueRef empty[1] = {};
    return LLVMConstArray(LLVMInt64Type(), empty, 0);
  } else if (auto ret = dynamic_cast<Return*>(expr)) {
    return LLVMBuildRet(functionState->builder, translateExpression(globalState, functionState, ret->sourceExpr));
  } else if (auto stackify = dynamic_cast<Stackify*>(expr)) {
    assert(functionState->localAddrByLocalId.count(stackify->local->id->number) == 0);
    auto name = std::string("r") + std::to_string(stackify->local->id->number);
    auto valueToStore = translateExpression(globalState, functionState, stackify->sourceExpr);
    auto localAddr = LLVMBuildAlloca(functionState->builder, translateType(stackify->local->type), name.c_str());
    functionState->localAddrByLocalId.emplace(stackify->local->id->number, localAddr);
    LLVMBuildStore(functionState->builder, valueToStore, localAddr);
    LLVMValueRef empty[1] = {};
    return LLVMConstArray(LLVMInt64Type(), empty, 0);
  } else if (auto localLoad = dynamic_cast<LocalLoad*>(expr)) {
    auto localAddrIter = functionState->localAddrByLocalId.find(localLoad->local->id->number);
    assert(localAddrIter != functionState->localAddrByLocalId.end());
    auto localAddr = localAddrIter->second;
    return LLVMBuildLoad(functionState->builder, localAddr, localLoad->localName.c_str());
  } else if (auto unstackify = dynamic_cast<Unstackify*>(expr)) {
    auto localAddrIter = functionState->localAddrByLocalId.find(unstackify->local->id->number);
    assert(localAddrIter != functionState->localAddrByLocalId.end());
    auto localAddr = localAddrIter->second;
    return LLVMBuildLoad(functionState->builder, localAddr, "");
  } else if (auto call = dynamic_cast<Call*>(expr)) {
    return translateCall(globalState, functionState, call);
  } else if (auto externCall = dynamic_cast<ExternCall*>(expr)) {
    return translateExternCall(globalState, functionState, externCall);
  } else if (auto argument = dynamic_cast<Argument*>(expr)) {
    return LLVMGetParam(functionState->containingFunc, argument->argumentIndex);
  } else if (auto block = dynamic_cast<Block*>(expr)) {
    auto exprs = translateExpressions(globalState, functionState, block->exprs);
    assert(!exprs.empty());
    return exprs.back();
  } else {
    std::string name = typeid(*expr).name();
    std::cout << name << std::endl;
    assert(false);
  }
  assert(false);
}

// Returns block and the result expression.
std::tuple<LLVMBasicBlockRef, LLVMValueRef>
translateBlock(
    GlobalState* globalState,
    FunctionState* functionState,
    Block* block) {

  int blockNumber = functionState->nextBlockNumber++;
  auto blockName = std::string("block") + std::to_string(blockNumber);
  LLVMBasicBlockRef blockL = LLVMAppendBasicBlock(functionState->containingFunc, blockName.c_str());

  LLVMPositionBuilderAtEnd(functionState->builder, blockL);

  LLVMValueRef lastResult;
  assert(!block->exprs.empty());

  for (int i = 0; i < block->exprs.size(); i++) {
    auto expr = block->exprs[i];

    auto result = translateExpression(globalState, functionState, expr);

    if (i < block->exprs.size() - 1) {
      assert(dynamic_cast<Discard *>(expr) != nullptr ||
          dynamic_cast<Return *>(expr) != nullptr ||
          dynamic_cast<Stackify *>(expr) != nullptr);
    } else {
      lastResult = result;
    }
  }

  return std::make_tuple(blockL, lastResult);
}

LLVMValueRef declareFunction(
    GlobalState* globalState,
    LLVMModuleRef mod,
    Function* functionM) {

  auto paramTypesL = translateTypes(functionM->prototype->params);
  auto returnTypeL = translateType(functionM->prototype->returnType);
  auto nameL = functionM->prototype->name->name;

  LLVMTypeRef functionTypeL = LLVMFunctionType(returnTypeL, paramTypesL.data(), paramTypesL.size(), 0);
  LLVMValueRef functionL = LLVMAddFunction(mod, nameL.c_str(), functionTypeL);

  assert(globalState->functions.count(functionM->prototype->name->name) == 0);
  globalState->functions.emplace(functionM->prototype->name->name, functionL);

  return functionL;
}

void translateFunction(
    GlobalState* globalState,
    LLVMModuleRef mod,
    Function* functionM) {

  auto functionIter = globalState->functions.find(functionM->prototype->name->name);
  assert(functionIter != globalState->functions.end());
  LLVMValueRef functionL = functionIter->second;

  auto localAddrByLocalId = std::unordered_map<int, LLVMValueRef>{};

  LLVMBuilderRef builder = LLVMCreateBuilder();
  FunctionState functionState(functionL, builder);

  auto [block, blockResult] =
  translateBlock(globalState, &functionState, functionM->block);
}

#endif