#include "../include/formula.h"
#include "../include/operations.h"
#include <sstream>
#include <cctype>
#include <stdexcept>

// Convierte "B3" -> row=2, col=1 (base-0)
// Letras = columnas (A=0, B=1, ..., Z=25, AA=26 ...)
// Números = filas (1-indexed -> base-0)
bool parseRef(const std::string& ref, int& row, int& col) {
    if (ref.empty()) return false;
    int i = 0;
    col = 0;

    // leer parte de letras
    while (i < (int)ref.size() && std::isalpha(ref[i])) {
        col = col * 26 + (std::toupper(ref[i]) - 'A' + 1);
        i++;
    }
    if (i == 0 || i == (int)ref.size()) return false;
    col -= 1; // base-0
    
    // leer parte numérica
    std::string rowStr = ref.substr(i);
    for (char c : rowStr) if (!std::isdigit(c)) return false;
    row = std::stoi(rowStr) - 1; // base-0
    return row >= 0 && col >= 0;
}

// Resuelve un operando: puede ser referencia de celda o número literal
static bool resolveOperand(const std::string& token, SparseMatrix& m, double& out) {
    // ¿Es referencia de celda? (empieza con letra)
    if (!token.empty() && std::isalpha(token[0])) {
        int r, c;
        if (!parseRef(token, r, c)) return false;
        std::string val = m.getCell(r, c);
        if (!isNumeric(val)) return false;
        out = std::stod(val);
        return true;
    }
    // ¿Es número literal?
    if (isNumeric(token)) {
        out = std::stod(token);
        return true;
    }
    return false;
}

// Evalúa fórmulas simples: =OPERAND OP OPERAND
// donde OP es +, -, *, /
// No soporta paréntesis ni prioridad de operadores (fuera del scope del proyecto).
std::string evalFormula(const std::string& formula, SparseMatrix& m) {
    if (formula.empty() || formula[0] != '=') return formula;

    std::string expr = formula.substr(1); // quitar el '='

    // Buscar operador de derecha a izquierda para manejar negativos simples
    // Soportamos exactamente una operación binaria
    char op = 0;
    int opPos = -1;

    // Recorrer buscando operador (evitando el '-' inicial que sería número negativo)
    for (int i = (int)expr.size() - 1; i > 0; i--) {
        char c = expr[i];
        if (c == '+' || c == '-' || c == '*' || c == '/') {
            op = c;
            opPos = i;
            break;
        }
    }

    if (opPos <= 0) {
        // Sin operador: solo un operando
        double val;
        std::string token = expr;
        // quitar espacios
        token.erase(0, token.find_first_not_of(' '));
        if (!resolveOperand(token, m, val)) return "ERROR";
        // Formatear: si es entero, sin decimales
        if (val == (long long)val) return std::to_string((long long)val);
        std::ostringstream oss;
        oss << val;
        return oss.str();
    }

    std::string left  = expr.substr(0, opPos);
    std::string right = expr.substr(opPos + 1);

    // quitar espacios
    left.erase(0, left.find_first_not_of(' '));
    left.erase(left.find_last_not_of(' ') + 1);
    right.erase(0, right.find_first_not_of(' '));
    right.erase(right.find_last_not_of(' ') + 1);

    double a, b;
    if (!resolveOperand(left, m, a) || !resolveOperand(right, m, b)) return "ERROR";

    double result;
    switch (op) {
        case '+': result = a + b; break;
        case '-': result = a - b; break;
        case '*': result = a * b; break;
        case '/':
            if (b == 0) return "DIV/0";
            result = a / b;
            break;
        default: return "ERROR";
    }

    // Formatear resultado
    if (result == (long long)result) return std::to_string((long long)result);
    std::ostringstream oss;
    oss << result;
    return oss.str();
}
