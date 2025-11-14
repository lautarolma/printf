#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdarg.h>

// ❌ Versión INCORRECTA (con UB)
int print_char_bad(int c)
{
    return (write(1, &c, 1));
}

int print_unsigned_bad(unsigned long long n, unsigned int base, char *symbol)
{
    int count = 0;
    if (n >= base)
        count += print_unsigned_bad(n / base, base, symbol);
    count += print_char_bad(symbol[n % base]);
    return (count);
}

int print_signed_BAD(long long n, unsigned int base, char *symbol)
{
    int count = 0;
    if (n < 0)
    {
        count += write(1, "-", 1);
        n = -n;  // ❌ UB cuando n == LLONG_MIN o INT_MIN
    }
    if (n >= (long long)base)
        count += print_signed_BAD(n / base, base, symbol);
    count += print_char_bad(symbol[n % base]);
    return (count);
}

// ✅ Versión CORRECTA (sin UB)
int print_char_good(int c)
{
    return (write(1, &c, 1));
}

int print_unsigned_good(unsigned long long n, unsigned int base, char *symbol)
{
    int count = 0;
    if (n >= base)
        count += print_unsigned_good(n / base, base, symbol);
    count += print_char_good(symbol[n % base]);
    return (count);
}

int print_signed_GOOD(long long n, unsigned int base, char *symbol)
{
    int count = 0;
    unsigned long long abs_n;
    
    if (n < 0)
    {
        count += write(1, "-", 1);
        // ✅ Método correcto: evita UB
        abs_n = ((unsigned long long)-(n + 1)) + 1;
    }
    else
        abs_n = (unsigned long long)n;
    
    count += print_unsigned_good(abs_n, base, symbol);
    return (count);
}

void test_case(const char *description, long long value)
{
    printf("\n========================================\n");
    printf("TEST: %s\n", description);
    printf("Value: %lld (0x%llx)\n", value, (unsigned long long)value);
    printf("========================================\n");
    
    // Test con printf estándar
    printf("printf():        ");
    printf("%lld", value);
    printf("\n");
    
    // Test con versión INCORRECTA
    printf("BAD version:     ");
    fflush(stdout);
    print_signed_BAD(value, 10, "0123456789");
    printf("\n");
    
    // Test con versión CORRECTA
    printf("GOOD version:    ");
    fflush(stdout);
    print_signed_GOOD(value, 10, "0123456789");
    printf("\n");
}

int main(void)
{
    printf("═══════════════════════════════════════════════════════\n");
    printf("  DEMOSTRACIÓN DE UNDEFINED BEHAVIOR EN print_signed\n");
    printf("═══════════════════════════════════════════════════════\n");
    
    printf("\nCompilado con: gcc -Wall -Wextra -Werror -fsanitize=undefined\n");
    printf("Si hay UB, verás warnings del sanitizer\n");
    
    // Test 1: Valor normal negativo (debería funcionar bien)
    test_case("Número negativo normal", -42);
    
    // Test 2: Valor cercano al límite (funciona bien)
    test_case("Cerca de INT_MIN", INT_MIN + 1);
    
    // Test 3: INT_MIN (32 bits) - ¡AQUÍ ESTÁ EL BUG!
    test_case("INT_MIN (caso crítico)", (long long)INT_MIN);
    
    // Test 4: LLONG_MIN (64 bits) - ¡MÁXIMO BUG!
    test_case("LLONG_MIN (caso extremo)", LLONG_MIN);
    
    // Test 5: Valor positivo (control)
    test_case("Número positivo (control)", 2147483647);
    
    printf("\n═══════════════════════════════════════════════════════\n");
    printf("  EXPLICACIÓN DEL BUG\n");
    printf("═══════════════════════════════════════════════════════\n");
    printf("\nINT_MIN  = %d (0x%x)\n", INT_MIN, INT_MIN);
    printf("INT_MAX  = %d (0x%x)\n", INT_MAX, INT_MAX);
    printf("LLONG_MIN = %lld (0x%llx)\n", LLONG_MIN, (unsigned long long)LLONG_MIN);
    printf("LLONG_MAX = %lld (0x%llx)\n", LLONG_MAX, (unsigned long long)LLONG_MAX);
    
    printf("\nCuando intentas hacer: n = -n con INT_MIN:\n");
    printf("  n = -(-%d) = %d ???\n", INT_MIN, INT_MAX);
    printf("  Pero %d NO EXISTE en int (MAX es %d)\n", INT_MAX + 1, INT_MAX);
    printf("  → OVERFLOW → UNDEFINED BEHAVIOR\n");
    
    printf("\nLa versión correcta evita esto haciendo:\n");
    printf("  n = ((unsigned long long)-(n + 1)) + 1\n");
    printf("  Paso 1: n + 1 = %d + 1 = %d ✓\n", INT_MIN, INT_MIN + 1);
    printf("  Paso 2: -(n+1) = %d ✓\n", -(INT_MIN + 1));
    printf("  Paso 3: cast a unsigned ✓\n");
    printf("  Paso 4: +1 = %u ✓\n", (unsigned int)(-(INT_MIN + 1)) + 1);
    
    printf("\n═══════════════════════════════════════════════════════\n");
    printf("  RESUMEN\n");
    printf("═══════════════════════════════════════════════════════\n");
    printf("✓ La versión CORRECTA maneja todos los casos\n");
    printf("✗ La versión INCORRECTA tiene UB con INT_MIN/LLONG_MIN\n");
    printf("\nSi usas -fsanitize=undefined, verás:\n");
    printf("  runtime error: negation of -2147483648 cannot be represented\n");
    printf("  in type 'long long int'\n");
    
    return (0);
}
