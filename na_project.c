#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#define MAX_EXPR 512
#define MAX_ITER 1000
#define EPS 1.0e-10
#define IS_BAD(v) ((v) != (v) || (v) > 1.0e300 || (v) < -1.0e300)
#define H_DIFF 1.0e-7
#define PI_VAL 3.14159265358979323846
#define E_VAL 2.71828182845904523546
typedef enum eNodeType {
    ND_NUMBER,
    ND_VARIABLE,
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_POW,
    ND_NEG,
    ND_SIN,
    ND_COS,
    ND_TAN,
    ND_ASIN,
    ND_ACOS,
    ND_ATAN,
    ND_EXP,
    ND_LN,
    ND_LOG,
    ND_SQRT,
    ND_ABS
} NodeType;
typedef struct sExprNode {
    NodeType type;
    double number;
    struct sExprNode *left;
    struct sExprNode *right;
} ExprNode;
typedef struct sParseState {
    char str[MAX_EXPR];
    int pos;
    int len;
} ParseState;
ExprNode* newNode(NodeType t){
    ExprNode *nd = (ExprNode *)malloc(sizeof(ExprNode));
    if (nd == NULL) { 
		fputs("ERROR: Memory could not be allocated.\n", stderr); 
		exit(1); 
	}
    nd->type   = t;
    nd->number = 0.0;
    nd->left   = NULL;
    nd->right  = NULL;
    return nd;
}
void freeNode(ExprNode *nd){
    if (nd == NULL) { 
		return; 
	}
    freeNode(nd->left);
    freeNode(nd->right);
    free(nd);
}
static void skipWs(ParseState *p){
    while (p->pos < p->len && p->str[p->pos] == ' ') { 
		p->pos++; 
	}
}
static char peekCh(ParseState *p){
    return (p->pos < p->len) ? p->str[p->pos] : '\0';
}
static int hasPrefix(ParseState *p, const char *s){
    int i;
    int n = (int)strlen(s);
    if (p->pos + n > p->len) { 
		return 0; 
	}
    for (i = 0; i < n; i++) {
        if (p->str[p->pos + i] != s[i]) { 
			return 0; 
		}
    }
    return 1;
}
static int isFactorStart(ParseState *p){
    char c;
    if (p->pos >= p->len) { 
		return 0; 
	}
    c = p->str[p->pos];
    return (isdigit((unsigned char)c) || isalpha((unsigned char)c) || c == '(');
}
static ExprNode* parseExpr (ParseState *p);
static ExprNode* parseTerm (ParseState *p);
static ExprNode* parsePow (ParseState *p);
static ExprNode* parseUnary (ParseState *p);
static ExprNode* parseAtom (ParseState *p);
static ExprNode* parseAtom(ParseState *p){
    ExprNode *nd, *arg, *base;
    char buf[64];
    int i;
	skipWs(p);
    if (hasPrefix(p, "pi")) {
        p->pos += 2;
        nd = newNode(ND_NUMBER); 
		nd->number = PI_VAL; 
		return nd;
    }
    if (hasPrefix(p, "asin")) {
        p->pos += 4; 
		skipWs(p);
        if (peekCh(p) == '(') { 
			p->pos++; 
		}
        arg = parseExpr(p); 
		skipWs(p);
        if (peekCh(p) == ')') { 
			p->pos++; 
		}
        nd = newNode(ND_ASIN); 
		nd->left = arg; 
		return nd;
    }
    if (hasPrefix(p, "acos")) {
        p->pos += 4; 
		skipWs(p);
        if (peekCh(p) == '(') { 
			p->pos++; 
		}
        arg = parseExpr(p); 
		skipWs(p);
        if (peekCh(p) == ')') { 
			p->pos++; 
		}
        nd = newNode(ND_ACOS); 
		nd->left = arg; 
		return nd;
    }
    if (hasPrefix(p, "atan")) {
        p->pos += 4; 
		skipWs(p);
        if (peekCh(p) == '(') { 
			p->pos++; 
		}
        arg = parseExpr(p); 
		skipWs(p);
        if (peekCh(p) == ')') { 
			p->pos++; 
		}
        nd = newNode(ND_ATAN); 
		nd->left = arg; 
		return nd;
    }
    if (hasPrefix(p, "sqrt")) {
        p->pos += 4; 
		skipWs(p);
        if (peekCh(p) == '(') { 
			p->pos++; 
		}
        arg = parseExpr(p); 
		skipWs(p);
        if (peekCh(p) == ')') { 
			p->pos++; 
		}
        nd = newNode(ND_SQRT); 
		nd->left = arg; 
		return nd;
    }
    if (hasPrefix(p, "sin")) {
        p->pos += 3; 
		skipWs(p);
        if (peekCh(p) == '(') { 
			p->pos++;
		}
        arg = parseExpr(p); 
		skipWs(p);
        if (peekCh(p) == ')') { 
			p->pos++; 
		}
        nd = newNode(ND_SIN); 
		nd->left = arg; 
		return nd;
    }
    if (hasPrefix(p, "cos")) {
        p->pos += 3; 
		skipWs(p);
        if (peekCh(p) == '(') { 
			p->pos++; 
		}
        arg = parseExpr(p); 
		skipWs(p);
        if (peekCh(p) == ')') { 
			p->pos++; 
		}
        nd = newNode(ND_COS); 
		nd->left = arg; 
		return nd;
    }
    if (hasPrefix(p, "tan")) {
        p->pos += 3; 
		skipWs(p);
        if (peekCh(p) == '(') { 
			p->pos++; 
		}
        arg = parseExpr(p); 
		skipWs(p);
        if (peekCh(p) == ')') { 
			p->pos++; 
		}
        nd = newNode(ND_TAN); 
		nd->left = arg; 
		return nd;
    }
    if (hasPrefix(p, "exp")) {
        p->pos += 3; 
		skipWs(p);
        if (peekCh(p) == '(') { 
			p->pos++; 
		}
        arg = parseExpr(p); 
		skipWs(p);
        if (peekCh(p) == ')') { 
			p->pos++; 
		}
        nd = newNode(ND_EXP); 
		nd->left = arg; 
		return nd;
    }
    if (hasPrefix(p, "log")) {
        p->pos += 3; 
		skipWs(p);
        if (peekCh(p) == '_') {
            p->pos++; 
			skipWs(p);
            if (isdigit((unsigned char)peekCh(p)) || peekCh(p) == '.') {
                i = 0;
                while (p->pos < p->len && (isdigit((unsigned char)p->str[p->pos]) || p->str[p->pos] == '.')){
                    buf[i++] = p->str[p->pos++];
                }
                buf[i] = '\0';
                base = newNode(ND_NUMBER); 
				base->number = atof(buf);
            } else if (peekCh(p) == 'x') {
                p->pos++;
                base = newNode(ND_VARIABLE);
            } else if (peekCh(p) == '(') {
                p->pos++;
                base = parseExpr(p); 
				skipWs(p);
                if (peekCh(p) == ')') { 
					p->pos++; 
				}
            } else {
                base = newNode(ND_NUMBER); 
				base->number = 10.0;
            }
        } else {
            base = newNode(ND_NUMBER); 
			base->number = 10.0;
        }
        skipWs(p);
        if (peekCh(p) == '(') { 
			p->pos++; 
		}
        arg = parseExpr(p); 
		skipWs(p);
        if (peekCh(p) == ')') { 
			p->pos++; 
		}
        nd = newNode(ND_LOG); 
		nd->left = base; 
		nd->right = arg; 
		return nd;
    }
    if (hasPrefix(p, "ln")) {
        p->pos += 2; 
		skipWs(p);
        if (peekCh(p) == '(') { 
			p->pos++; 
		}
        arg = parseExpr(p); 
		skipWs(p);
        if (peekCh(p) == ')') { 
			p->pos++; 
		}
        nd = newNode(ND_LN); 
		nd->left = arg; 
		return nd;
    }
    if (hasPrefix(p, "abs")) {
        p->pos += 3; 
		skipWs(p);
        if (peekCh(p) == '(') { 
			p->pos++; 
		}
        arg = parseExpr(p); 
		skipWs(p);
        if (peekCh(p) == ')') { 
			p->pos++; 
		}
        nd = newNode(ND_ABS); 
		nd->left = arg; 
		return nd;
    }
    if (peekCh(p) == 'e' && (p->pos + 1 >= p->len || !isalpha((unsigned char)p->str[p->pos + 1]))){
        p->pos++;
        nd = newNode(ND_NUMBER); 
		nd->number = E_VAL; 
		return nd;
    }
    if (peekCh(p) == 'x') {
        p->pos++;
        return newNode(ND_VARIABLE);
    }
    if (peekCh(p) == '(') {
        p->pos++;
        nd = parseExpr(p); 
		skipWs(p);
        if (peekCh(p) == ')') { 
			p->pos++; 
		}
        return nd;
    }
	if (isdigit((unsigned char)peekCh(p)) || peekCh(p) == '.') {
        i = 0;
        while (p->pos < p->len && (isdigit((unsigned char)p->str[p->pos]) || p->str[p->pos] == '.')){
            buf[i++] = p->str[p->pos++];
        }
        buf[i] = '\0';
        nd = newNode(ND_NUMBER); 
		nd->number = atof(buf); 
		return nd;
    }
    if (p->pos < p->len) { 
		p->pos++; 
	}
    nd = newNode(ND_NUMBER); 
	nd->number = 0.0; 
	return nd;
}
static ExprNode* parseUnary(ParseState *p){
    ExprNode *nd, *operand;
    skipWs(p);
    if (peekCh(p) == '-') {
        p->pos++;
        operand = parsePow(p);
        nd = newNode(ND_NEG); 
		nd->left = operand; 
		return nd;
    }
    if (peekCh(p) == '+') { 
		p->pos++; 
	}
    return parsePow(p);
}
static ExprNode* parsePow(ParseState *p){
    ExprNode *base, *exp, *nd;
    base = parseAtom(p); 
	skipWs(p);
    if (peekCh(p) == '^') {
        p->pos++; 
		skipWs(p);
        exp = parseUnary(p);
        nd = newNode(ND_POW); 
		nd->left = base; 
		nd->right = exp; 
		return nd;
    }
    return base;
}
static ExprNode* parseTerm(ParseState *p){
    ExprNode *left, *right, *nd;
    char op;
    left = parseUnary(p); 
	skipWs(p);
    while (peekCh(p) == '*' || peekCh(p) == '/' || isFactorStart(p)) {
        if (peekCh(p) == '*' || peekCh(p) == '/') {
            op = p->str[p->pos++]; 
			skipWs(p);
            right = parseUnary(p);
            nd = newNode(op == '*' ? ND_MUL : ND_DIV);
        } else {
            right = parseUnary(p);
            nd = newNode(ND_MUL);
        }
        nd->left = left; 
		nd->right = right; 
		left = nd;
        skipWs(p);
    }
    return left;
}
static ExprNode* parseExpr(ParseState *p){
    ExprNode *left, *right, *nd;
    char op;
    left = parseTerm(p); 
	skipWs(p);
    while (peekCh(p) == '+' || peekCh(p) == '-') {
        op = p->str[p->pos++];
        right = parseTerm(p);
        nd = newNode(op == '+' ? ND_ADD : ND_SUB);
        nd->left = left; 
		nd->right = right; 
		left = nd;
        skipWs(p);
    }
    return left;
}
ExprNode* buildTree(const char *s){
    ParseState p;
    strncpy(p.str, s, MAX_EXPR - 1);
    p.str[MAX_EXPR - 1] = '\0';
    p.pos = 0;
    p.len = (int)strlen(p.str);
    return parseExpr(&p);
}
double evalAt(ExprNode *nd, double x){
    double l, r, b, a;
    if (nd == NULL) { 
		return 0.0; 
	}
    switch (nd->type) {
    	case ND_NUMBER:   
			return nd->number;
    	case ND_VARIABLE: 
			return x;
    	case ND_ADD:      
			return evalAt(nd->left, x) + evalAt(nd->right, x);
    	case ND_SUB:      
			return evalAt(nd->left, x) - evalAt(nd->right, x);
    	case ND_MUL:      
			return evalAt(nd->left, x) * evalAt(nd->right, x);
    	case ND_DIV:
        	r = evalAt(nd->right, x);
        	if (fabs(r) < EPS) { 
				fputs("WARNING: Dividing by zero\n", stderr); 
				return 0.0; 
			}
        	return evalAt(nd->left, x) / r;
    	case ND_POW:
        	l = evalAt(nd->left, x); 
			r = evalAt(nd->right, x);
        	return pow(l, r);
    	case ND_NEG:  
			return -evalAt(nd->left, x);
    	case ND_SIN:  
			return sin (evalAt(nd->left, x));
    	case ND_COS:  
			return cos (evalAt(nd->left, x));
    	case ND_TAN:  
			return tan (evalAt(nd->left, x));
    	case ND_ASIN: 
			return asin(evalAt(nd->left, x));
    	case ND_ACOS: 
			return acos(evalAt(nd->left, x));
    	case ND_ATAN: 
			return atan(evalAt(nd->left, x));
    	case ND_EXP:  
			return exp (evalAt(nd->left, x));
    	case ND_LN:
        	l = evalAt(nd->left, x);
        	if (l <= 0.0) { 
				fputs("WARNING: Invalid for ln\n", stderr); 
				return 0.0; 
			}
        	return log(l);
    	case ND_SQRT:
        	l = evalAt(nd->left, x);
        	if (l < 0.0) { 
				fputs("WARNING: sqrt is negative\n", stderr); 
				return 0.0; 
			}
        	return sqrt(l);
    	case ND_ABS: 
			return fabs(evalAt(nd->left, x));
    	case ND_LOG:
        	b = evalAt(nd->left, x);
        	a = evalAt(nd->right, x);
        	if (a <= 0.0 || b <= 0.0 || fabs(b - 1.0) < EPS) {
            	fputs("WARNING: log invalid base\n", stderr); 
				return 0.0;
        	}
        	return log(a) / log(b);
    	default: 
			return 0.0;
    }
}
ExprNode* readFunc(void){
    char buf[MAX_EXPR];
    int  len;
    printf("  f(x) = ");
    if (fgets(buf, MAX_EXPR, stdin) != NULL) {
        len = (int)strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') { 
			buf[len - 1] = '\0'; 
		}
    } else {
        buf[0] = '\0';
    }
    return buildTree(buf);
}
double** matAlloc(int r, int c){
    double **m;
    int      i;
    m = (double **)malloc(r * sizeof(double *));
    if (m == NULL) {
		fputs("ERROR: malloc\n", stderr); 
		exit(1); 
	}
    for (i = 0; i < r; i++) {
        m[i] = (double *)malloc(c * sizeof(double));
        if (m[i] == NULL) { 
			fputs("ERROR: malloc\n", stderr); 
			exit(1); 
		}
    }
    return m;
}

void matFree(double **m, int r){
    int i;
    for (i = 0; i < r; i++) { 
		free(m[i]); 
	}
    free(m);
}

void matPrint(double **m, int r, int c)
{
    int i, j;
    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) { 
			printf("  %12.6f", m[i][j]); 
		}
        printf("\n");
    }
}
void runBisection(void){
    ExprNode *f;
    double a, b, c, fa, fb, fc, tol;
    int iter, conv;
    printf("\n=== 1. BISECTION METHOD ===\n");
    printf("  Enter a function. (example: x^3-x-2 , sin(x)-x/2):\n");
    f = readFunc();
    printf("  Left border  a = ");
	scanf("%lf", &a);
    printf("  Right border  b = "); 
	scanf("%lf", &b);
    printf("  Tolerance    = "); 
	scanf("%lf", &tol);
    getchar();
    fa = evalAt(f, a);
    fb = evalAt(f, b);
    if (fa * fb > 0.0) {
        printf("  ERROR: f(a)*f(b) > 0, The root may not be in this range!\n");
        freeNode(f);
		return;
    }
    printf("\n  %5s  %14s  %14s  %14s  %14s\n", "Iter", "a", "b", "c=(a+b)/2", "f(c)");
    printf("  -----  --------------  -------------- --------------  --------------\n");
    iter = 0; 
	conv = 0; 
	c = a; 
	fc = fa;
    while (iter < MAX_ITER && !conv) {
        c  = (a + b) / 2.0;
        fc = evalAt(f, c);
        printf("  %5d  %14.8f  %14.8f  %14.8f  %14.8f\n", iter + 1, a, b, c, fc);
        if (fabs(fc) < tol || (b - a) / 2.0 < tol) {
            conv = 1;
        } else {
            fa = evalAt(f, a);
            if (fa * fc < 0.0) { 
				b = c; 
			} else { 
				a = c; 
			}
            iter++;
        }
    }
    if (!conv) { 
		printf("  WARNING: Maximum iterations (%d) reached, convergence not achieved!\n", MAX_ITER); 
	}
    printf("\n  Root  x    = %.10f\n", c);
    printf("  f(root)    = %.4e\n",   fc);
    printf("  Iteration = %d\n",     iter + 1);
    freeNode(f);
}
void runRegulaFalsi(void){
    ExprNode *f;
    double a, b, c, fa, fb, fc, tol;
    int iter, conv;
    printf("\n=== 2. REGULA-FALSI METHOD ===\n");
    printf("  Enter a function:\n");
    f = readFunc();
    printf("  Left Border  a = "); 
	scanf("%lf", &a);
    printf("  Right Border  b = "); 
	scanf("%lf", &b);
    printf("  Tolerance     = "); 
	scanf("%lf", &tol);
    getchar();
    fa = evalAt(f, a);
    fb = evalAt(f, b);
    if (fa * fb > 0.0) {
        printf("  ERROR: f(a)*f(b) > 0!\n");
        freeNode(f); 
		return;
    }
    printf("\n  %5s  %14s  %14s  %14s  %14s\n", "Iter", "a", "b", "c", "f(c)");
    printf("  -----  --------------  -------------- --------------  --------------\n");
    iter = 0;
	conv = 0; 
	c = a; 
	fc = fa;
    while (iter < MAX_ITER && !conv) {
        fa = evalAt(f, a);
        fb = evalAt(f, b);
        c  = (a * fb - b * fa) / (fb - fa);
        fc = evalAt(f, c);
        printf("  %5d  %14.8f  %14.8f  %14.8f  %14.8f\n", iter + 1, a, b, c, fc);
        if (fabs(fc) < tol) {
            conv = 1;
        } else {
            if (fa * fc < 0.0) { 
				b = c; 
			} else { 
				a = c; 
			}
            iter++;
        }
    }
    if (!conv) { 
		printf("  WARNING: Maximum iterations (%d) reached, convergence not achieved!\n", MAX_ITER); 
	}
    printf("\n  Root  x    = %.10f\n", c);
    printf("  f(root)    = %.4e\n", fc);
    printf("  Iteration = %d\n", iter + 1);
    freeNode(f);
}
static double centralDeriv(ExprNode *f, double x){
    double h = H_DIFF;
    return (evalAt(f, x + h) - evalAt(f, x - h)) / (2.0 * h);
}
void runNewtonRaphson(void){
    ExprNode *f;
    double x0, x1, fx, fpx, tol;
    int iter, conv;
    printf("\n=== 3. NEWTON-RAPHSON METHOD ===\n");
    printf("  Enter a function:\n");
    f = readFunc();
    printf("  Beginning x0 = "); 
	scanf("%lf", &x0);
    printf("  Tolerance     = "); 
	scanf("%lf", &tol);
    getchar();
    printf("\n  %5s  %15s  %15s  %15s\n", "Iter", "x", "f(x)", "f'(x)");
    printf("  -----  ---------------  ---------------  ---------------\n");
    iter = 0; 
	conv = 0; 
	x1 = x0;
    while(iter < MAX_ITER && !conv){
        fx  = evalAt(f, x0);
        fpx = centralDeriv(f, x0);
        printf("  %5d  %15.8f  %15.8f  %15.8f\n", iter + 1, x0, fx, fpx);
        if(fabs(fpx) < EPS){
            printf("  WARNING: f'(x0) ~ 0, stopped.\n"); 
			conv = 1;
        } else{
            x1 = x0 - fx / fpx;
            if (fabs(x1 - x0) < tol){ 
			    x0 = x1; conv = 1; 
		    } else{ 
			    x0 = x1; iter++;   
            }
		}
    }
    if(!conv){ 
		printf("  WARNING: Maximum iterations (%d) reached, convergence not achieved!\n", MAX_ITER); 
	}
    printf("\n  Root  x    = %.10f\n", x1);
    printf("  f(root)    = %.4e\n", evalAt(f, x1));
    printf("  Iteration = %d\n", iter + 1);
    freeNode(f);
}
void runMatrixInverse(void){
    double **aug;
    double  *tmp;
    double piv, fac, maxV;
    int n, i, j, k, mr;
    printf("\n=== 4. NxN MATRIX INVERSE (GAUSS-JORDAN) ===\n");
    printf("  Matrix size N = "); 
	scanf("%d", &n); 
	getchar();
    if (n <= 0) {
		printf("  Invalid N.\n"); 
		return; 
	}
    aug = matAlloc(n, 2 * n);
    printf("  Enter matrix A. (%dx%d):\n", n, n);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("   A[%d][%d] = ", i + 1, j + 1); 
			scanf("%lf", &aug[i][j]);
        }
    }
    getchar();
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            aug[i][j + n] = (i == j) ? 1.0 : 0.0;
        }
    }
    i = 0;
    while (i < n) {
        mr = i; 
		maxV = fabs(aug[i][i]);
        k = i + 1;
        while (k < n) {
            if (fabs(aug[k][i]) > maxV) { 
				maxV = fabs(aug[k][i]); 
				mr = k; 
			}
            k++;
        }
        if (mr != i) { 
			tmp = aug[i]; 
			aug[i] = aug[mr]; 
			aug[mr] = tmp; 
		}
        piv = aug[i][i];
        if (fabs(piv) < EPS) {
            printf("  ERROR: The matrix is ??singular! It cannot be inverted.\n");
            matFree(aug, n);
			return;
        }
        for (j = 0; j < 2 * n; j++) { 
			aug[i][j] /= piv; 
		}
        k = 0;
        while (k < n) {
            if (k != i) {
                fac = aug[k][i];
                for (j = 0; j < 2 * n; j++) { 
					aug[k][j] -= fac * aug[i][j]; 
				}
            }
            k++;
        }
        i++;
    }
    printf("\n  A^(-1) =\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) { 
			printf("  %12.6f", aug[i][j + n]); 
		}
        printf("\n");
    }
    matFree(aug, n);
}
void runLU(void){
    double **A, **L, **U, *b, *y, *x;
    double sum;
    int n, i, j, k, ok;
    printf("\n=== 5. LU DECOMPOSÝTÝON (CHOLESKY / ALU METHOD) ===\n");
    printf("  Number of equations N = "); 
	scanf("%d", &n); 
	getchar();
    if (n <= 0) { 
		printf("  Invalid N.\n"); 
		return; }
    A = matAlloc(n, n);
    L = matAlloc(n, n);
    U = matAlloc(n, n);
    b = (double *)malloc(n * sizeof(double));
    y = (double *)malloc(n * sizeof(double));
    x = (double *)malloc(n * sizeof(double));
    if (b == NULL || y == NULL || x == NULL) {
        fputs("ERROR: malloc\n", stderr); 
		exit(1);
    }

    printf("  Coefficient matrix A (%dx%d):\n", n, n);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("   A[%d][%d] = ", i + 1, j + 1); 
			scanf("%lf", &A[i][j]);
        }
    }
    printf("  Right side b: \n");
    for (i = 0; i < n; i++) {
        printf("   b[%d] = ", i + 1); scanf("%lf", &b[i]);
    }
    getchar();
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) { 
			L[i][j] = 0.0; 
			U[i][j] = 0.0; 
		}
        L[i][i] = 1.0;
    }
    ok = 1; 
	k = 0;
    while (k < n && ok) {
        j = k;
        while (j < n) {
            sum = 0.0;
            i = 0;
            while (i < k) { 
				sum += L[k][i] * U[i][j]; 
				i++; 
			}
            U[k][j] = A[k][j] - sum;
            j++;
        }
        if (fabs(U[k][k]) < EPS) {
            printf("  ERROR: Singular matrix!\n"); 
			ok = 0;
        } else {
            i = k + 1;
            while (i < n) {
                sum = 0.0;
                j = 0;
                while (j < k) { 
					sum += L[i][j] * U[j][k]; 
					j++; 
				}
                L[i][k] = (A[i][k] - sum) / U[k][k];
                i++;
            }
        }
        k++;
    }

    if (ok) {
        printf("\n  Matrix L:\n"); 
		matPrint(L, n, n);
        printf("\n  Matrix U:\n");
		matPrint(U, n, n);
        i = 0;
        while (i < n) {
            sum = b[i];
            j = 0;
            while (j < i) { 
				sum -= L[i][j] * y[j]; 
				j++;
			}
            y[i] = sum;
            i++;
        }
        i = n - 1;
        while (i >= 0) {
            sum = y[i];
            j = i + 1;
            while (j < n) { 
				sum -= U[i][j] * x[j]; 
				j++; 
			}
            if (fabs(U[i][i]) < EPS) {
                printf("  ERROR: Singular (replacement).\n"); 
				ok = 0; 
				i = -1;
            } else {
                x[i] = sum / U[i][i];
                i--;
            }
        }
        if (ok) {
            printf("\n  Solution x:\n");
            for (i = 0; i < n; i++) { 
				printf("  x[%d] = %.8f\n", i + 1, x[i]); 
			}
        }
    }
    matFree(A, n); 
	matFree(L, n); 
	matFree(U, n);
    free(b); 
	free(y); 
	free(x);
}
void runGaussSeidel(void){
    double **A, *b, *x, *xOld;
    double sum, tol, maxErr;
    int n, i, j, iter, conv;
    printf("\n=== 6. GAUSS-SEIDEL METHOD === \n");
    printf("  Denklem sayisi N = "); 
	scanf("%d", &n); 
	getchar();
    if (n <= 0) { 
		printf("  Invalid N.\n"); 
		return; 
	}
    A = matAlloc(n, n);
    b = (double *)malloc(n * sizeof(double));
    x = (double *)malloc(n * sizeof(double));
    xOld = (double *)malloc(n * sizeof(double));
    if (b == NULL || x == NULL || xOld == NULL) {
        fputs("ERROR: malloc\n", stderr); 
		exit(1);
    }
    printf("  Coefficient matrix A (%dx%d):\n", n, n);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("   A[%d][%d] = ", i + 1, j + 1);
			scanf("%lf", &A[i][j]);
        }
    }
    printf("  Right side b:\n");
    for (i = 0; i < n; i++) {
        printf("    b[%d] = ", i + 1); 
		scanf("%lf", &b[i]);
    }
    printf("  Tolerance  = "); 
	scanf("%lf", &tol);
    getchar();
    printf("\n  NOTE: For good convergence, the matrix diagonal must be dominant. \n");
    for (i = 0; i < n; i++) { 
		x[i] = 0.0; 
		xOld[i] = 0.0; 
	}
    iter = 0; 
	conv = 0;
    while (iter < MAX_ITER && !conv) {
        int divDetected = 0;
        for (i = 0; i < n; i++) { 
			xOld[i] = x[i]; 
		}
        for (i = 0; i < n; i++) {
            if (fabs(A[i][i]) < EPS) {
                printf("  ERROR: A[%d][%d] ~ 0!\n", i + 1, i + 1);
                matFree(A, n); 
				free(b); 
				free(x); 
				free(xOld); 
				return;
            }
            sum = b[i];
            for (j = 0; j < n; j++) {
                if (j != i) { 
					sum -= A[i][j] * x[j]; 
				}
            }
            x[i] = sum / A[i][i];
            if (IS_BAD(x[i])) { 
				divDetected = 1; 
			}
        }
        if (divDetected) {
            printf("  ERROR: Divergence! Matrix diagonal must be dominant (|A_ii| > total |A_ij|).\n");
            matFree(A, n); 
			free(b); free(x); 
			free(xOld); 
			return;
        }
        maxErr = 0.0;
        for (i = 0; i < n; i++) {
            if (fabs(x[i] - xOld[i]) > maxErr) { 
				maxErr = fabs(x[i] - xOld[i]); 
			}
        }
        printf(" Iter %4d |", iter + 1);
        for (i = 0; i < n; i++) { 
			printf("  x[%d]=%-11.6f", i + 1, x[i]); 
		}
        printf(" Error=%.2e\n", maxErr);

        if (maxErr < tol) { 
			conv = 1; 
		} else { 
			iter++; 
		}
    }
    if (!conv) { 
		printf(" WARNING: Maximum iterations (%d) reached! Isn't the matrix diagonally dominant?\n", MAX_ITER); 
	}
    printf("\n  Solution:\n");
    for (i = 0; i < n; i++) { 
		printf("  x[%d] = %.8f\n", i + 1, x[i]); 
	}
    printf(" Iteration = %d\n", iter + 1);
    matFree(A, n); 
	free(b); 
	free(x); 
	free(xOld);
}
void runNumericalDerivative(void){
    ExprNode *f;
    double xv, h, fx, fxph, fxmh;
    double ileri, geri, merkezi;
    printf("\n=== 7. NUMERICAL DERIVATIVE === \n");
    printf("  Enter a function:\n");
    f = readFunc();
    printf("  x value = "); 
	scanf("%lf", &xv);
    printf("  Step  h  = "); 
	scanf("%lf", &h);
    getchar();
    fx = evalAt(f, xv);
    fxph = evalAt(f, xv + h);
    fxmh = evalAt(f, xv - h);
    ileri = (fxph - fx) / h;
    geri = (fx  - fxmh) / h;
    merkezi = (fxph - fxmh) / (2.0 * h);
    printf("\n  f(x)  = %.10f\n", fx);
    printf("  f(x+h) = %.10f\n",  fxph);
    printf("  f(x-h) = %.10f\n",  fxmh);
    printf("\n  ---- Derivative Results (x = %g, h = %g) ---- \n", xv, h);
    printf("  Forward Difference f'(x) = %.10f\n", ileri);
    printf("  Back    Difference f'(x) = %.10f\n", geri);
    printf("  Central Difference f'(x) = %.10f\n", merkezi);
    freeNode(f);
}
void runSimpson(void){
    ExprNode *f;
    double a, b, h13, h38, r13, r38, xi, sum;
    int n13, n38, i;
    printf("\n=== 8. SIMPSON METHOD (1/3 and 3/8) === \n");
    printf("  Enter a function:\n");
    f = readFunc();
    printf(" Lower limit a = "); 
	scanf("%lf", &a);
    printf(" Upper limit b = "); 
	scanf("%lf", &b);
    printf(" The number of intervals for 1/3 is N (it must be even). = "); scanf("%d", &n13);
    if (n13 <= 0) { 
		n13 = 2; 
	}
    if (n13 % 2 != 0) { 
		n13++; 
		printf("  1/3: Rounded to N=%d.\n", n13); 
	}
    h13 = (b - a) / n13;
    sum = evalAt(f, a) + evalAt(f, b);
    i = 1;
    while (i < n13) {
        xi = a + i * h13;
        if (i % 2 == 0) { 
			sum += 2.0 * evalAt(f, xi); 
		} else { 
			sum += 4.0 * evalAt(f, xi); 
		}
        i++;
    }
    r13 = (h13 / 3.0) * sum;
    printf("  The number of intervals for 3/8 is N (which must be a multiple of 3) = "); 
	scanf("%d", &n38);
    if (n38 <= 0) { 
		n38 = 3; 
	}
    if (n38 % 3 != 0) { 
		n38 += (3 - n38 % 3); printf("  3/8: Rounded to N=%d\n", n38); 
	}
    h38 = (b - a) / n38;
    sum = evalAt(f, a) + evalAt(f, b);
    i = 1;
    while (i < n38) {
        xi = a + i * h38;
        if (i % 3 == 0) { 
			sum += 2.0 * evalAt(f, xi); 
		} else { sum += 3.0 * evalAt(f, xi); }
        i++;
    }
    r38 = (3.0 * h38 / 8.0) * sum;
    getchar();
    printf("\n ---- Simpson Results ----\n");
    printf(" Simpson 1/3  (N=%2d) : integral = %.10f\n", n13, r13);
    printf(" Simpson 3/8  (N=%2d) : integral = %.10f\n", n38, r38);
    freeNode(f);
}
void runTrapez(void){
    ExprNode *f;
    double a, b, h, result, sum;
    int n, i;
    printf("\n=== 9. TRAPEZ METHOD ===\n");
    printf(" Enter a function:\n");
    f = readFunc();
    printf(" Lower limit a          = "); 
	scanf("%lf", &a);
    printf(" Upper limit b          = "); 
	scanf("%lf", &b);
    printf(" Number of Intervals N  = "); 
	scanf("%d",  &n);
    getchar();
    if (n <= 0) { 
		printf("  Gecersiz N.\n");
		freeNode(f); 
		return; 
	}
    h = (b - a) / n;
    sum = evalAt(f, a) + evalAt(f, b);
    i = 1;
    while (i < n) {
        sum += 2.0 * evalAt(f, a + i * h);
        i++;
    }
    result = (h / 2.0) * sum;
    printf("\n Trapez Result (N=%d) : integral = %.10f\n", n, result);
    freeNode(f);
}
void runGregoryNewton(void){
    double **table;
    double *xd, *yd;
    double xq, result, prod;
    int n, i, j;
    printf("\n=== 10. GREGORY-NEWTON INTERPOLATION === \n");
    printf(" Number of data points n = "); 
	scanf("%d", &n);
    if (n <= 0) { 
		printf("  Invalid n.\n"); getchar(); 
		return; 
	}
    xd = (double *)malloc(n * sizeof(double));
    yd = (double *)malloc(n * sizeof(double));
    table = matAlloc(n, n);
    if (xd == NULL || yd == NULL) { 
		fputs("ERROR: malloc\n", stderr); 
		exit(1);
	}
    printf("  Enter the data points:\n");
    i = 0;
    while (i < n) {
        printf("  x[%d] = ", i); 
		scanf("%lf", &xd[i]);
        printf("  y[%d] = ", i); 
		scanf("%lf", &yd[i]);
        i++;
    }
    printf("  Interpolation point x = "); 
	scanf("%lf", &xq);
    getchar();
    i = 0;
    while (i < n) { 
		table[i][0] = yd[i]; 
		i++; 
	}
    j = 1;
    while (j < n) {
        i = 0;
        while (i < n - j) {
            table[i][j] = (table[i + 1][j - 1] - table[i][j - 1]) / (xd[i + j] - xd[i]);
            i++;
        }
        j++;
    }
    printf("\n Divided Difference Table:\n");
    printf(" %-10s  %-14s", "x_i", "f[.]");
    j = 1;
    while (j < n) { 
		printf("  %-14s", "Delta"); 
		j++; 
	}
    printf("\n");
    i = 0;
    while (i < n) {
        printf("  %-10.4f", xd[i]);
        j = 0;
        while (j < n - i) { 
			printf("  %-14.6f", table[i][j]); 
			j++; 
		}
        printf("\n");
        i++;
    }
    result = table[0][0];
    prod   = 1.0;
    j = 1;
    while (j < n) {
        prod *= (xq - xd[j - 1]);
        result += table[0][j] * prod;
        j++;
    }
    printf("\n  p(%g) = %.10f\n", xq, result);
    free(xd);
	free(yd); 
	matFree(table, n);
}
static void printMenu(void){
    printf("\n");
    printf(" +===================================================+\n");
    printf(" |       NUMERICAL ANALYSIS END-OF-TERM PROJECT      |\n");
    printf(" +===================================================+\n");
    printf(" |   1  - Bisection Method                           |\n");
    printf(" |   2  - Regula-Falsi Method                        |\n");
    printf(" |   3  - Newton-Raphson Method                      |\n");
    printf(" |   4  - NxN Matrix Inverse                         |\n");
    printf(" |   5  - LU Decomposition (Cholesky/ALU)            |\n");
    printf(" |   6  - Gauss-Seidel Method                        |\n");
    printf(" |   7  - Numerical Derivative (Forward/Back/Central)|\n");
    printf(" |   8  - Simpson Method (1/3 and 3/8)               |\n");
    printf(" |   9  - Trapeze Method                             |\n");
    printf(" |  10  - Gregory-Newton Interpolation               |\n");
    printf(" |   0  - Exit                                       |\n");
    printf(" +===================================================+\n");
    printf(" Your Choice: ");
}
int main(void){
    int choice;
    int running;
    running = 1;
    while (running) {
        printMenu();
        if (scanf("%d", &choice) != 1) { 
			getchar(); 
			choice = -1; 
		}
        getchar();
        switch (choice){
        	case 1:  
				runBisection();           
				break;
        	case 2:  
				runRegulaFalsi();         
				break;
        	case 3:  
				runNewtonRaphson();       
				break;
        	case 4:  
				runMatrixInverse();       
				break;
        	case 5:  
				runLU();                  
				break;
        	case 6:  
				runGaussSeidel();         
				break;
        	case 7:  
				runNumericalDerivative(); 
				break;
        	case 8:  
				runSimpson();             
				break;
        	case 9:  
				runTrapez();              
				break;
        	case 10: 
				runGregoryNewton();       
				break;
        	case 0:  
				running = 0;              
				break;
        	default: 
				printf("  Invalid choice. \n"); 
				break;
        }
        if (running) {
            printf("\n [Press Enter to continue.... ]");
            getchar();
        }
    }
    printf(" The program is ending.... \n");
    return 0;
}
