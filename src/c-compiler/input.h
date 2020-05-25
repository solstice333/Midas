#ifndef VALE_INPUT_H_
#define VALE_INPUT_H_

#include <string>

#include "readjson.h"

Program* getUserInput() {
  auto programJ =
      R"json(
{"": "Program",
  "interfaces": [],
  "structs": [{"": "Struct", "name": "Tup", "mutability": {"": "Immutable"}, "edges": [], "members": []}],
  "externs": [],
  "functions": [
    {"": "Function",
      "prototype": {"": "Prototype",
        "name": "F(\"main\")",
        "params": [],
        "return": {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}},
      "block": {"": "Block",
        "exprs": [
          {"": "Return",
            "sourceExpr": {"": "Block",
              "exprs": [
                {"": "Stackify",
                  "sourceExpr": {"": "Call",
                    "function": {"": "Prototype",
                      "name": "F(\"+\",[],[R(*,i),R(*,i)])",
                      "params": [
                        {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}},
                        {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}],
                      "return": {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}},
                    "argExprs": [{"": "ConstantI64", "value": 3}, {"": "ConstantI64", "value": 9}]},
                  "local": {"": "Local",
                    "id": {"": "VariableId", "number": 0, "name": {"": "Some", "value": "F(\"main\"):TemplarBlockResultVarName"}},
                    "type": {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}},
                  "name": {"": "Some", "value": "F(\"main\"):TemplarBlockResultVarName"}},
                {"": "Unstackify",
                  "local": {"": "Local",
                    "id": {"": "VariableId", "number": 0, "name": {"": "Some", "value": "F(\"main\"):TemplarBlockResultVarName"}},
                    "type": {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}}}]}}]}},
    {"": "Function",
      "prototype": {"": "Prototype",
        "name": "F(\"+\",[],[R(*,i),R(*,i)])",
        "params": [
          {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}},
          {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}],
        "return": {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}},
      "block": {"": "Block",
        "exprs": [
          {"": "Return",
            "sourceExpr": {"": "Block",
              "exprs": [
                {"": "Stackify",
                  "sourceExpr": {"": "Argument",
                    "resultType": {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}},
                    "argumentIndex": 0},
                  "local": {"": "Local",
                    "id": {"": "VariableId", "number": 0, "name": {"": "Some", "value": "F(\"+\",[],[R(*,i),R(*,i)]):CodeVarName(\"a\")"}},
                    "type": {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}},
                  "name": {"": "Some", "value": "F(\"+\",[],[R(*,i),R(*,i)]):CodeVarName(\"a\")"}},
                {"": "Stackify",
                  "sourceExpr": {"": "Argument",
                    "resultType": {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}},
                    "argumentIndex": 1},
                  "local": {"": "Local",
                    "id": {"": "VariableId", "number": 1, "name": {"": "Some", "value": "F(\"+\",[],[R(*,i),R(*,i)]):CodeVarName(\"b\")"}},
                    "type": {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}},
                  "name": {"": "Some", "value": "F(\"+\",[],[R(*,i),R(*,i)]):CodeVarName(\"b\")"}},
                {"": "Stackify",
                  "sourceExpr": {"": "Call",
                    "function": {"": "Prototype",
                      "name": "F(\"__addIntInt\",[],[R(*,i),R(*,i)])",
                      "params": [
                        {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}},
                        {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}],
                      "return": {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}},
                    "argExprs": [
                      {"": "LocalLoad",
                        "local": {"": "Local",
                          "id": {"": "VariableId", "number": 0, "name": {"": "Some", "value": "F(\"+\",[],[R(*,i),R(*,i)]):CodeVarName(\"a\")"}},
                          "type": {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}},
                        "targetOwnership": {"": "Share"},
                        "localName": "F(\"+\",[],[R(*,i),R(*,i)]):CodeVarName(\"a\")"},
                      {"": "LocalLoad",
                        "local": {"": "Local",
                          "id": {"": "VariableId", "number": 1, "name": {"": "Some", "value": "F(\"+\",[],[R(*,i),R(*,i)]):CodeVarName(\"b\")"}},
                          "type": {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}},
                        "targetOwnership": {"": "Share"},
                        "localName": "F(\"+\",[],[R(*,i),R(*,i)]):CodeVarName(\"b\")"}]},
                  "local": {"": "Local",
                    "id": {"": "VariableId",
                      "number": 2,
                      "name": {"": "Some", "value": "F(\"+\",[],[R(*,i),R(*,i)]):TemplarBlockResultVarName"}},
                    "type": {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}},
                  "name": {"": "Some", "value": "F(\"+\",[],[R(*,i),R(*,i)]):TemplarBlockResultVarName"}},
                {"": "Discard",
                  "sourceExpr": {"": "Unstackify",
                    "local": {"": "Local",
                      "id": {"": "VariableId", "number": 1, "name": {"": "Some", "value": "F(\"+\",[],[R(*,i),R(*,i)]):CodeVarName(\"b\")"}},
                      "type": {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}}}},
                {"": "Discard",
                  "sourceExpr": {"": "Unstackify",
                    "local": {"": "Local",
                      "id": {"": "VariableId", "number": 0, "name": {"": "Some", "value": "F(\"+\",[],[R(*,i),R(*,i)]):CodeVarName(\"a\")"}},
                      "type": {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}}}},
                {"": "Unstackify",
                  "local": {"": "Local",
                    "id": {"": "VariableId",
                      "number": 2,
                      "name": {"": "Some", "value": "F(\"+\",[],[R(*,i),R(*,i)]):TemplarBlockResultVarName"}},
                    "type": {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}}}]}}]}},
    {"": "Function",
      "prototype": {"": "Prototype",
        "name": "F(\"__addIntInt\",[],[R(*,i),R(*,i)])",
        "params": [
          {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}},
          {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}],
        "return": {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}},
      "block": {"": "Block",
        "exprs": [
          {"": "Return",
            "sourceExpr": {"": "ExternCall",
              "function": {"": "Prototype",
                "name": "F(\"__addIntInt\",[],[R(*,i),R(*,i)])",
                "params": [
                  {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}},
                  {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}],
                "return": {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}},
              "argExprs": [
                {"": "Argument", "resultType": {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}, "argumentIndex": 0},
                {"": "Argument", "resultType": {"": "Ref", "ownership": {"": "Share"}, "referend": {"": "Int"}}, "argumentIndex": 1}]}}]}}]}
)json"_json;

  return readProgram(programJ);
}

#endif