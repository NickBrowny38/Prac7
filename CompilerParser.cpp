#include "CompilerParser.h"

/**
 * Constructor for the CompilerParser
 * @param tokens A linked list of tokens to be parsed
 */
CompilerParser::CompilerParser(std::list<Token*> tokens) {
    this->tokens = tokens;
}

/**
 * Generates a parse tree for a single program
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileProgram() {
    // create passtree
    ParseTree* tree = new ParseTree("class", "");

    // add keyword class
    tree->addChild(mustBe("keyword", "class"));

    // add identifier
    tree->addChild(mustBe("identifier", current()->getValue()));

    // add open bracket
    tree->addChild(mustBe("symbol", "{"));

    // add close bracket
    tree->addChild(mustBe("symbol", "}"));

    return tree;
}

/**
 * Generates a parse tree for a single class
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClass() {
    // create passtree
    ParseTree* tree = new ParseTree("class", "");

    // add keyword class
    tree->addChild(mustBe("keyword", "class"));

    // add identifier
    tree->addChild(mustBe("identifier", current()->getValue()));

    // add open bracket
    tree->addChild(mustBe("symbol", "{"));

    // add variable decleration
    tree->addChild(mustBe("classVarDec", current()->getValue()));

    // add close bracket
    tree->addChild(mustBe("symbol", "}"));

    return tree;
}

/**
 * Generates a parse tree for a static variable declaration or field declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClassVarDec() {
    // create passtree
    ParseTree* tree = new ParseTree("classVarDec", "");

    // add keyword static
    if (current()->getValue() == "static"){
        tree->addChild(mustBe("keyword", "static"));
    }
    else if (current()->getValue() == "field"){
        tree->addChild(mustBe("keyword", "field"));
    }
    else{
        throw ParseException();
    }

    // add keyword int
    tree->addChild(mustBe("keyword", "int"));

    // add identifier
    tree->addChild(mustBe("identifier", current()->getValue()));

    // add semi-colon
    tree->addChild(mustBe("symbol", ";"));

    return tree;
}

/**
 * Generates a parse tree for a method, function, or constructor
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutine() {
    return NULL;
}

/**
 * Generates a parse tree for a subroutine's parameters
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileParameterList() {
    return NULL;
}

/**
 * Generates a parse tree for a subroutine's body
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutineBody() {
    return NULL;
}

/**
 * Generates a parse tree for a subroutine variable declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileVarDec() {
    return NULL;
}

/**
 * Generates a parse tree for a series of statements
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileStatements() {
    return NULL;
}

/**
 * Generates a parse tree for a let statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileLet() {
    return NULL;
}

/**
 * Generates a parse tree for an if statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileIf() {
    return NULL;
}

/**
 * Generates a parse tree for a while statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileWhile() {
    return NULL;
}

/**
 * Generates a parse tree for a do statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileDo() {
    return NULL;
}

/**
 * Generates a parse tree for a return statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileReturn() {
    return NULL;
}

/**
 * Generates a parse tree for an expression
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpression() {
    return NULL;
}

/**
 * Generates a parse tree for an expression term
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileTerm() {
    return NULL;
}

/**
 * Generates a parse tree for an expression list
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpressionList() {
    return NULL;
}

/**
 * Advance to the next token
 */
void CompilerParser::next(){
    tokens.pop_front();
    return;
}

/**
 * Return the current token
 * @return the Token
 */
Token* CompilerParser::current(){
    return tokens.front();
}

/**
 * Check if the current token matches the expected type and value.
 * @return true if a match, false otherwise
 */
bool CompilerParser::have(std::string expectedType, std::string expectedValue){
    if (current()->getType() == expectedType && current()->getValue() == expectedValue){
        return true;
    }
    else{
        return false;
    }
}

/**
 * Check if the current token matches the expected type and value.
 * If so, advance to the next token, returning the current token, otherwise throw a ParseException.
 * @return the current token before advancing
 */
Token* CompilerParser::mustBe(std::string expectedType, std::string expectedValue){
    if (have(expectedType, expectedValue) == true){
        Token* curr = new Token(expectedType, expectedValue);
        next();
        return curr;
    }
    else{
        throw ParseException();
    }
}

/**
 * Definition of a ParseException
 * You can use this ParseException with `throw ParseException();`
 */
const char* ParseException::what() {
    return "An Exception occurred while parsing!";
}
