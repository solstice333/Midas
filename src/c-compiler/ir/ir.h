/** Intermediate Representation (IR) structures and macros
*
* The IR, together with the name table, is the skeleton of the compiler.
* It connects together every stage:
*
* - The parser transforms programs into IR
* - The semantic analysis walks the IR nodes over multiple passes
* - Macro and template expansion happens via IR cloning
* - Generation transforms IR into LLVM IR
*
* The IR is comprised of heterogeneous nodes that share common BasicAstHdr info.
* In some cases, it is possible for several distinct node types to share an 
* identical data structure (e.g., statement expression and return).
*
* This include file will pull in the include files for all node types, including types.
* It also defines the structures needed for semantic analysis passes.
*
 * @file
 *
 * This source file is part of the Cone Programming Language C compiler
 * See Copyright Notice in conec.h
*/

#ifndef ir_h
#define ir_h

#include "../shared/memory.h"
#include "nodes.h"
#include "namespace.h"
typedef struct Name Name;		// ../ast/nametbl.h
typedef struct Lexer Lexer;		// ../parser/lexer.h

#include <llvm-c/Core.h>

#include <stdint.h>

typedef struct PassState PassState;

// Flags found at the type of an AST tag
#define VoidGroup  0x0000   // Statement nodes that return no value
#define ValueGroup 0x4000   // Nodes that return a value
#define TypeGroup  0x8000   // Nodes that define or refer to a type
#define GroupMask  0xC000
#define NamedNode  0x2000   // Node that defines a named item (not nameuse)
#define MethodType 0x1000   // Type that supports methods

// Easy checks on the kind of node it is based on high-level flags
#define isValueNode(node) (((node)->asttype & GroupMask) == ValueGroup)
#define isTypeNode(node) (((node)->asttype & GroupMask) == TypeGroup)
#define isNamedNode(node) ((node)->asttype & NamedNode)
#define isMethodType(node) (isTypeNode(node) && (node)->asttype & MethodType)

// All the possible types for an AstNode
enum AstType {
	// Lexer-only nodes that are *never* found in a program's AST.
	// KeywordNode exists for name table consistency
	KeywordNode = VoidGroup,	// Keyword token (flags is the keyword's token type)

	// Untyped (Basic) AST nodes
	IntrinsicNode,	// Alternative to fndcl block for internal operations (e.g., add)
	ReturnNode,     // Return node
	WhileNode,		// While node
	BreakNode,		// Break node
	ContinueNode,	// Continue node

	// Name usage (we do not know what type of name it is until name resolution pass)
	NameUseTag,  	// Name use node (pre-name resolution)

    ModuleNode = VoidGroup + NamedNode,		// Program (global area)

    // Expression nodes (having value type - or sometimes nullType)
    VarNameUseTag = ValueGroup,  // Variable or Function name use node  
	MbrNameUseTag,	// Member of a type's namespace (property/method)
	ULitNode,		// Integer literal
	FLitNode,		// Float literal
	SLitNode,		// String literal
	AssignNode,		// Assignment expression
	FnCallNode,		// Function+method call or Property access
	SizeofNode,		// Sizeof a type (usize)
	CastNode,		// Cast exp to another type
	AddrNode,		// & (address of) operator
	DerefNode,		// * (pointed at) operator
	NotLogicNode,	// ! / not
	OrLogicNode,	// || / or
	AndLogicNode,	// && / and
	BlockNode,		// Block (list of statements)
	IfNode,			// if .. elif .. else statement

    // Named value node
    VarDclTag = ValueGroup + NamedNode,
    FnDclTag,

    // Unnamed type node
    TypeNameUseTag = TypeGroup, // Type name use node
    FnSigType,	// Also method, closure, behavior, co-routine, thread, ...
    VoidType,	// representing no values, e.g., no return values on a fn

    RefType = TypeGroup + NamedNode,	// Reference
    PtrType,	// Pointer

    IntNbrType = TypeGroup + NamedNode + MethodType,	// Integer
    UintNbrType,	// Unsigned integer
    FloatNbrType,	// Floating point number
    StructType,	// Also interface, trait, tuple, actor, etc.
    ArrayType,	// Also dynamic arrays? SOA?
    PermType,
    AllocType,
};

// ******************************
// AST node traits: header macros and structs that begin all AST node structs
// ******************************

// All AST nodes begin with this header, mostly containing lexer data describing
// where in the source this structure came from (useful for error messages)
// - asttype contains the AstType code
// - flags contains node-specific flags
// - lexer contains -> url (filepath) and -> source
// - srcp points to start of source token
// - linep points to start of line that token begins on
// - linenbr is the source file's line number, starting with 1
#define BasicAstHdr \
	Lexer *lexer; \
	char *srcp; \
	char *linep; \
	uint32_t linenbr; \
	uint16_t asttype; \
	uint16_t flags

enum AstFlags {
	FlagMethProp  = 0x0001,	    // VarDclTag/FnDclTag: Method or Property
	FlagExtern    = 0x0002		// C ABI extern
};

// AstNode is a castable struct for all AST nodes.
typedef struct AstNode {
	BasicAstHdr;
} AstNode;

// Typed Ast Node header, offering access to the node's type info
// - vtype is the value type for an expression (e.g., 'i32')
#define TypedAstHdr \
	BasicAstHdr; \
	AstNode *vtype

// Castable structure for all typed AST nodes
typedef struct TypedAstNode {
	TypedAstHdr;
} TypedAstNode;

// Named Ast Node header, for variable and type declarations
// - namesym points to the global name table entry (holds name string)
// - owner is the namespace node this name belongs to
#define NamedAstHdr \
	TypedAstHdr; \
	Name *namesym; \
	struct NamedAstNode *owner

// Castable structure for all named AST nodes
typedef struct NamedAstNode {
	NamedAstHdr;
} NamedAstNode;


#include "types/type.h"
#include "types/methtype.h"
#include "types/permission.h"
#include "types/fnsig.h"
#include "types/number.h"
#include "types/pointer.h"
#include "types/struct.h"
#include "types/array.h"
#include "types/alloc.h"

#include "stmt/module.h"
#include "stmt/block.h"

#include "exp/nameuse.h"
#include "exp/expr.h"
#include "exp/copyexpr.h"
#include "exp/fncall.h"
#include "exp/fndcl.h"
#include "exp/vardcl.h"
#include "exp/literal.h"

#include "../std/stdlib.h"

// The AST analytical passes performed in between parse and generation
enum Passes {
	// Scope all declared names and resolve all name uses accordingly
	NameResolution,
	// Do return inference and type inference/checks.
	TypeCheck
};

// Context used across all AST semantic analysis passes
typedef struct PassState {
	int pass;				// Passes
	ModuleAstNode *mod;		// Current module
	FnSigAstNode *fnsig;	// The type signature of the function we are within

	int16_t scope;			// The current block scope (0=global, 1=fnsig, 2+=blocks)
	uint16_t flags;
} PassState;

#define PassWithinWhile 0x0001

// Allocate and initialize a new AST node
#define newAstNode(node, aststruct, asttyp) {\
	node = (aststruct*) memAllocBlk(sizeof(aststruct)); \
	node->asttype = asttyp; \
	node->flags = 0; \
	node->lexer = lex; \
	node->srcp = lex->tokp; \
	node->linep = lex->linep; \
	node->linenbr = lex->linenbr; \
}

// Copy lexer info over to another node
#define copyNodeLex(newnode, oldnode) { \
    (newnode)->lexer = (oldnode)->lexer; \
    (newnode)->srcp = (oldnode)->srcp; \
    (newnode)->linep = (oldnode)->linep; \
    (newnode)->linenbr = (oldnode)->linenbr; \
}

void astPrint(char *dir, char *srcfn, AstNode *pgm);
void astPrintNode(AstNode *node);
void astFprint(char *str, ...);
void astPrintNL();
void astPrintIndent();
void astPrintIncr();
void astPrintDecr();

// Run all semantic analysis passes against the AST/IR (after parse and before gen)
void irSemanticPasses(ModuleAstNode **pgm);

// Dispatch a node walk for the current semantic analysis pass
// - pstate is helpful state info for node traversal
// - node is a pointer to pointer so that a node can be replaced
void nodeWalk(PassState *pstate, AstNode **pgm);

#endif