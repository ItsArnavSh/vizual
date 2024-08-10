#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <map>
#include <utility>
using namespace std;

enum class TokenType {

    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals.
    IDENTIFIER, STRING, NUMBER, CHARACTER,

    // Keywords.
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,LET,
};

map<std::string, TokenType> tokenMap = {
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"return", TokenType::RETURN},
    {"and", TokenType::AND},
    {"super", TokenType::SUPER},
    {"class", TokenType::CLASS},
    {"for", TokenType::FOR},
    {"fun", TokenType::FUN},
    {"nil", TokenType::NIL},
    {"or", TokenType::OR},
    {"return", TokenType::RETURN},
    {"this", TokenType::THIS},
    {"true", TokenType::TRUE},
    {"var", TokenType::VAR},
    {"while", TokenType::WHILE},
};
class Literal{};
class Token {
    public:
    TokenType type;
    string lexeme;
    int line;
    Literal literal;

public:
    Token(TokenType type, string lexeme, int line, Literal literal)
        : type(type), lexeme(lexeme), line(line), literal(literal) {}
};

class Scanner {
    string source;
    vector<Token> tokens;

public:
    Scanner(string source) : source(source) {}
    

void scanTokens() {
    ifstream file(source);
    string line;
    int li = 0;

    if (file.is_open()) {  // Check if the file is open
        while (getline(file, line)) {  // Read the file line by line
            li++;
            inner:
            for (int i = 0; i < line.size(); i++) {
                switch (line[i]) {
                    case '{': {
                        string lexeme(1, line[i]);
                        Literal literal;
                        Token temp(TokenType::LEFT_BRACE, lexeme, li, literal);
                        tokens.push_back(temp);
                        break;
                    }
                    case '}': {
                        string lexeme(1, line[i]);
                        Literal literal;
                        Token temp(TokenType::RIGHT_BRACE, lexeme, li, literal);
                        tokens.push_back(temp);
                        break;
                    }
                    case '(': {
                        string lexeme(1, line[i]);
                        Literal literal;
                        Token temp(TokenType::LEFT_PAREN, lexeme, li, literal);
                        tokens.push_back(temp);
                        break;
                    }
                    case ')': {
                        string lexeme(1, line[i]);
                        Literal literal;
                        Token temp(TokenType::RIGHT_PAREN, lexeme, li, literal);
                        tokens.push_back(temp);
                        break;
                    
                    }
                    case '.': {
                        string lexeme(1, line[i]);
                        Literal literal;
                        Token temp(TokenType::DOT, lexeme, li, literal);
                        tokens.push_back(temp);
                        break;
                    
                    }
                    case ',': {
                        string lexeme(1, line[i]);
                        Literal literal;
                        Token temp(TokenType::COMMA, lexeme, li, literal);
                        tokens.push_back(temp);
                        break;
                    
                    }
                    case '-': {
                        string lexeme(1, line[i]);
                        Literal literal;
                        Token temp(TokenType::MINUS, lexeme, li, literal);
                        tokens.push_back(temp);
                        break;
                    
                    }
                    case '+': {
                        string lexeme(1, line[i]);
                        Literal literal;
                        Token temp(TokenType::PLUS, lexeme, li, literal);
                        tokens.push_back(temp);
                        break;
                    
                    }
                    case ';': {
                        string lexeme(1, line[i]);
                        Literal literal;
                        Token temp(TokenType::SEMICOLON, lexeme, li, literal);
                        tokens.push_back(temp);
                        break;
                    
                    }
                    case '*': {
                        string lexeme(1, line[i]);
                        Literal literal;
                        Token temp(TokenType::STAR, lexeme, li, literal);
                        tokens.push_back(temp);
                        break;
                    
                    }
                    case '!': {
                        string lexeme(1, line[i]);
                        Literal literal;
                        if(line[i+1]=='='){
                        Token temp(TokenType::BANG_EQUAL, lexeme+'=', li, literal);
                        i++;
                        tokens.push_back(temp);}
                        else{
                          Token temp(TokenType::BANG, lexeme, li, literal);  
                          tokens.push_back(temp);
                        }
                        break;
                    
                    }
                    case '=': {
                        string lexeme(1, line[i]);
                        Literal literal;
                        if(line[i+1]=='='){
                        Token temp(TokenType::EQUAL_EQUAL, lexeme+'=', li, literal);
                        i++;
                        tokens.push_back(temp);}
                        else{
                          Token temp(TokenType::EQUAL, lexeme, li, literal);  
                          tokens.push_back(temp);
                        }
                        break;
                    
                    }
                    case '<': {
                        string lexeme(1, line[i]);
                        Literal literal;
                        if(line[i+1]=='='){
                        Token temp(TokenType::LESS_EQUAL, lexeme+'=', li, literal);
                        i++;
                        tokens.push_back(temp);}
                        else{
                          Token temp(TokenType::LESS, lexeme, li, literal);  
                          tokens.push_back(temp);
                        }
                        break;
                    
                    }
                    case '>': {
                        string lexeme(1, line[i]);
                        Literal literal;
                        if(line[i+1]=='='){
                        Token temp(TokenType::GREATER_EQUAL, lexeme+'=', li, literal);
                        i++;
                        tokens.push_back(temp);}
                        else{
                          Token temp(TokenType::GREATER, lexeme, li, literal);  
                          tokens.push_back(temp);
                        }
                        break;
                    
                    }
                    case '/':{
                        if(line[i+1]!='/')
                        {
                            string lexeme(1, line[i]);
                            Literal literal;
                            Token temp(TokenType::SLASH, lexeme, li, literal);
                            tokens.push_back(temp);
                            break;
                    
                        }
                        else
                        {
                            i = line.size();
                        }
                        break;
                    }
            
                    case '"': {
                    int current = i + 1;
                    
                    while (current < line.size() && line[current] != '"') {
                        current++;
                    }

                    // Check if we exited the loop because we found the closing quote
                    if (current >= line.size() || line[current] != '"') {
                        cerr << "Unterminated String on line " << li << endl;
                        exit(1); // Use exit(1) to indicate an error
                    }

                    // Extract the string literal, excluding the surrounding quotes
                    string lexeme = line.substr(i + 1, current - i - 1);
                    Literal literal; // Assuming you have a way to set the literal value for strings
                    Token temp(TokenType::STRING, lexeme, li, literal);
                    tokens.push_back(temp);

                    // Move the index `i` to the position after the closing quote
                    i = current;
                    break;
}

                    default:
                        if (isalpha(line[i])) {
                            int start = i;
                            int current = i;
                            while (current < line.size() && (isalpha(line[current]) || isdigit(line[current]))) {
                                current++;
                            }
                            string lexeme = line.substr(start, current - start);
                            auto it = tokenMap.find(lexeme);
                            
                            Literal literal;
                            Token temp(it!=tokenMap.end()?tokenMap[lexeme]:TokenType::IDENTIFIER, lexeme, li, literal);
                            tokens.push_back(temp);
                            i = current - 1; // Update i to the last character of the identifier
                        }
                        else if(isdigit(line[i]))
                        {
                             int start = i;
                            int current = i;
                            while (current < line.size() && (isdigit(line[current])||(line[current]=='.'&&isdigit(line[current+1])))){
                                current++;
                            }
                            string lexeme = line.substr(start, current - start);
                            Literal literal;
                            Token temp(TokenType::NUMBER, lexeme, li, literal);
                            tokens.push_back(temp);
                            i = current - 1; // Update i to the last character of the identifier
                        }
                        break;
                }
            }
        }
        file.close();  // Close the file after reading
    } else {
        cerr << "Unable to open file" << endl;
    }
}

    void printTokens()
    {
        for(Token iter : tokens)
        {
            cout << iter.lexeme <<endl;
        }
    }

};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Not the correct syntax for files" << endl;
        return EXIT_FAILURE;
    }
    Scanner lexlox(argv[1]);
    lexlox.scanTokens();
    lexlox.printTokens();
    return EXIT_SUCCESS;
}
