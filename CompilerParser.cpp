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

    while (current() != NULL && have("keyword", current()->getValue())){
        // add variable decleration
        if (have("keyword", "static") || have("keyword", "field")){
            tree->addChild(compileClassVarDec());
        }
        else if(have("keyword", "function") || have("keyword", "constructor") || have("keyword", "method")){
            tree->addChild(compileSubroutine());
        }
        else{
            throw ParseException();
        }
    }

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
    if (have("keyword", "static")){
        tree->addChild(mustBe("keyword", "static"));
    }
    else if (current()->getValue() == "field"){
        tree->addChild(mustBe("keyword", "field"));
    }
    else{
        throw ParseException();
    }

    // add type variable
    tree->addChild(mustBe("keyword", current()->getValue()));

    // add identifier
    tree->addChild(mustBe("identifier", current()->getValue()));

    while (current() != NULL && have("symbol", ",")){
        tree->addChild(mustBe("symbol", ","));
        tree->addChild(mustBe("identifier", current()->getValue()));
    }

    // add semi-colon
    tree->addChild(mustBe("symbol", ";"));

    return tree;
}

/**
 * Generates a parse tree for a method, function, or constructor
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutine() {
    // create passtree
    ParseTree* tree = new ParseTree("subroutine", "");

    // add keyword type
    if (have("keyword", "function")){
        tree->addChild(mustBe("keyword", "function"));
    }
    else if (have("keyword", "constructor")){
        tree->addChild(mustBe("keyword", "constructor"));
    }
    else if (have("keyword", "method")){
        tree->addChild(mustBe("keyword", "method"));
    }
    else{
        throw ParseException();
    }

    // add keyword func type
    if (have("keyword", current()->getValue())){
        tree->addChild(mustBe("keyword", current()->getValue()));
    }
    else if (have("identifier", current()->getValue())){
        tree->addChild(mustBe("identifier", current()->getValue()));
    }
    else{
        throw ParseException();
    }

    // add identifier
    tree->addChild(mustBe("identifier", current()->getValue()));

    // add open bracket
    tree->addChild(mustBe("symbol", "("));

    // add parameter list
    tree->addChild(compileParameterList());

    // add close bracket
    tree->addChild(mustBe("symbol", ")"));

    // add subroutine body
    tree->addChild(compileSubroutineBody());

    return tree;
}

/**
 * Generates a parse tree for a subroutine's parameters
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileParameterList() {
    // create passtree
    ParseTree* tree = new ParseTree("parameterList", "");

    if (have("keyword", current()->getValue())){
        // add variable type
        tree->addChild(mustBe("keyword", current()->getValue()));

        // add identifier
        tree->addChild(mustBe("identifier", current()->getValue()));
    }

    while (current() != NULL && have("symbol", ",")){
        tree->addChild(mustBe("symbol", ","));
        tree->addChild(mustBe("keyword", current()->getValue()));
        tree->addChild(mustBe("identifier", current()->getValue()));
    }

    return tree;
}

/**
 * Generates a parse tree for a subroutine's body
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutineBody() {
    // create passtree
    ParseTree* tree = new ParseTree("subroutineBody", "");

    // add open bracket
    tree->addChild(mustBe("symbol", "{"));
    
    // variable declerations
    if (have("varDec", current()->getValue())){
        tree->addChild(compileVarDec());
    }// statements
    else if(have("statements", current()->getValue())){
        tree->addChild(compileStatements());
    }
    
    // add body
    while (current() != NULL && have("symbol", ";")){
        tree->addChild(mustBe("symbol", ";"));
        
        // variable declerations
        if (have("varDec", current()->getValue())){
            tree->addChild(compileVarDec());
        }// statements
        else if(have("statements", current()->getValue())){
            tree->addChild(compileStatements());
        }
    }

    // add close bracket
    tree->addChild(mustBe("symbol", "}"));

    return tree;
}

/**
 * Generates a parse tree for a subroutine variable declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileVarDec() {
    // create passtree
    ParseTree* tree = new ParseTree("varDec", "");

    // add keyword var
    tree->addChild(mustBe("keyword", "var"));

    // add type variable
    tree->addChild(mustBe("keyword", current()->getValue()));

    // add identifier
    tree->addChild(mustBe("identifier", current()->getValue()));

    while (current() != NULL && have("symbol", ",")){
        tree->addChild(mustBe("symbol", ","));
        tree->addChild(mustBe("identifier", current()->getValue()));
    }

    // add semi-colon
    tree->addChild(mustBe("symbol", ";"));

    return tree;
}

/**
 * Generates a parse tree for a series of statements
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileStatements() {
    // create passtree
    ParseTree* tree = new ParseTree("statements", "");

    // first statement
    if (have("letStatement", current()->getValue())){
        tree->addChild(compileLet());
    }// statements
    else if(have("ifStatement", current()->getValue())){
        tree->addChild(compileIf());
    }
    else if(have("whileStatement", current()->getValue())){
        tree->addChild(compileWhile());
    }
    else if(have("doStatement", current()->getValue())){
        tree->addChild(compileDo());
    }
    else if(have("returnStatement", current()->getValue())){
        tree->addChild(compileReturn());
    }
    
    // subsequent statements
    while (current() != NULL && have("symbol", ";")){
        tree->addChild(mustBe("symbol", ";"));
        
        // statements types
        if (have("letStatement", current()->getValue())){
            tree->addChild(compileLet());
        }
        else if(have("ifStatement", current()->getValue())){
            tree->addChild(compileIf());
        }
        else if(have("whileStatement", current()->getValue())){
            tree->addChild(compileWhile());
        }
        else if(have("doStatement", current()->getValue())){
            tree->addChild(compileDo());
        }
        else if(have("returnStatement", current()->getValue())){
            tree->addChild(compileReturn());
        }
    }

    return tree;
}

/**
 * Generates a parse tree for a let statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileLet() {
    // create passtree
    ParseTree* tree = new ParseTree("letStatement", "");

    // add keyword let
    tree->addChild(mustBe("keyword", "let"));

    // add variable
    tree->addChild(mustBe("identifier", current()->getValue()));

    if (have("symbol", "[")){
        tree->addChild(mustBe("symbol", "["));
        // add expression 1
        tree->addChild(compileExpression());
        tree->addChild(mustBe("symbol", "]"));
    }

    // add '='
    tree->addChild(mustBe("symbol", "="));

    // add expression 2
    tree->addChild(compileExpression());

    // add semi-colon
    tree->addChild(mustBe("symbol", ";"));

    return tree;
}

/**
 * Generates a parse tree for an if statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileIf() {
    // create passtree
    ParseTree* tree = new ParseTree("ifStatement", "");

    // add keyword if
    tree->addChild(mustBe("keyword", "if"));

    // add open bracket
    tree->addChild(mustBe("symbol", "("));

    // add expression
    tree->addChild(compileExpression());

    // add close bracket
    tree->addChild(mustBe("symbol", ")"));

    // add open bracket
    tree->addChild(mustBe("symbol", "{"));

    // add statements
    tree->addChild(compileStatements());

    // add closed brackets
    tree->addChild(mustBe("symbol", "}"));

    // else functionality
    while (current() != NULL && have("keyword", "else")){
        // add open bracket
        tree->addChild(mustBe("symbol", "{"));

        // add statements
        tree->addChild(compileStatements());

        // add closed brackets
        tree->addChild(mustBe("symbol", "}"));
    }

    return tree;
}

/**
 * Generates a parse tree for a while statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileWhile() {
    // create passtree
    ParseTree* tree = new ParseTree("whileStatement", "");

    // add keyword while
    tree->addChild(mustBe("keyword", "while"));

    // add open bracket
    tree->addChild(mustBe("symbol", "("));

    // add expression
    tree->addChild(compileExpression());

    // add close bracket
    tree->addChild(mustBe("symbol", ")"));

    // add open bracket
    tree->addChild(mustBe("symbol", "{"));

    // add statements
    tree->addChild(compileStatements());

    // add closed brackets
    tree->addChild(mustBe("symbol", "}"));

    return tree;
}

/**
 * Generates a parse tree for a do statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileDo() {
    // create passtree
    ParseTree* tree = new ParseTree("doStatement", "");

    // add keyword do
    tree->addChild(mustBe("keyword", "do"));

    // add expression
    tree->addChild(compileExpression());

    // add semi-colon
    tree->addChild(mustBe("symbol", ";"));

    return tree;
}

/**
 * Generates a parse tree for a return statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileReturn() {
    // create passtree
    ParseTree* tree = new ParseTree("returnStatement", "");

    // add keyword rerturn
    tree->addChild(mustBe("keyword", "return"));

    if (have("expression", current()->getValue())){
        // add expression
        tree->addChild(compileExpression());
    }

    // add semi-colon
    tree->addChild(mustBe("symbol", ";"));

    return tree;
}

/**
 * Generates a parse tree for an expression
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpression() {
    // create passtree
    ParseTree* tree = new ParseTree("expression", "");

    if (have("keyword", "skip")){
        // add keyword skip
        tree->addChild(mustBe("keyword", "skip"));
    }
    else{
        // add term
        tree->addChild(current());
    }

    return tree;
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
