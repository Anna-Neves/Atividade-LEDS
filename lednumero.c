#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "ws2812.pio.h"

// Definições dos GPIOs
#define LED_RED 13
#define BUTTON_A 5
#define BUTTON_B 6
#define WS2812_PIN 7
#define NUM_PIXELS 25
#define OUT_PIN 7

// Variáveis globais
volatile bool button_a_pressed = false;
volatile bool button_b_pressed = false;
volatile uint32_t debounce_time = 0;
int current_number = 0; // Número atual exibido na matriz de LEDs

// Função para piscar o LED vermelho
void blink_red_led() {
    static bool led_state = false;
    led_state = !led_state;
    gpio_put(LED_RED, led_state);
}

// Função para exibir um número na matriz de LEDs WS2812
static volatile uint32_t ultimo_evento = 0; 

           double ZERO [25] = {
    0.0, 0.8, 0.8, 0.8, 0.0,
    0.0, 0.8, 0.0, 0.8, 0.0,
    0.0, 0.8, 0.0, 0.8, 0.0,
    0.0, 0.8, 0.0, 0.8, 0.0,
    0.0, 0.8, 0.8, 0.8, 0.0
};
               double UM [25] = {
    0.0, 0.8, 0.8, 0.8, 0.0,
    0.0, 0.0, 0.8, 0.0, 0.0,
    0.0, 0.0, 0.8, 0.0, 0.0,
    0.0, 0.0, 0.8, 0.0, 0.0,
    0.0, 0.0, 0.8, 0.0, 0.0
};
  
               double DOIS [25] = {
    0.0, 0.8, 0.8, 0.8, 0.0,
    0.0, 0.0, 0.0, 0.8, 0.0,
    0.0, 0.8, 0.8, 0.8, 0.0,
    0.0, 0.8, 0.0, 0.0, 0.0,
    0.0, 0.8, 0.8, 0.8, 0.0
};

               double TRES [25] = {
    0.0, 0.8, 0.8, 0.8, 0.0,
    0.0, 0.8, 0.0, 0.0, 0.0,
    0.0, 0.8, 0.8, 0.8, 0.0,
    0.0, 0.8, 0.0, 0.0, 0.0,
    0.0, 0.8, 0.8, 0.8, 0.0
};
  
               double QUATRO [25] = {
    0.0, 0.8, 0.0, 0.0, 0.0,
    0.0, 0.8, 0.0, 0.0, 0.0,
    0.0, 0.8, 0.8, 0.8, 0.0,
    0.0, 0.8, 0.0, 0.8, 0.0,
    0.0, 0.8, 0.0, 0.8, 0.0
};

               double CINCO [25] = {
    0.0, 0.8, 0.8, 0.8, 0.0,
    0.0, 0.0, 0.0, 0.8, 0.0,
    0.0, 0.8, 0.8, 0.8, 0.0,
    0.0, 0.8, 0.0, 0.0, 0.0,
    0.0, 0.8, 0.8, 0.8, 0.0
};
 
               double SEIS [25] = {
    0.0, 0.8, 0.8, 0.8, 0.0,
    0.0, 0.8, 0.0, 0.8, 0.0,
    0.0, 0.8, 0.8, 0.8, 0.0,
    0.0, 0.8, 0.0, 0.0, 0.0,
    0.0, 0.8, 0.0, 0.0, 0.0
};
   
               double SETE [25] = {
    0.0, 0.8, 0.0, 0.0, 0.0,
    0.0, 0.8, 0.0, 0.0, 0.0,
    0.0, 0.8, 0.0, 0.0, 0.0,
    0.0, 0.8, 0.0, 0.0, 0.0,
    0.0, 0.8, 0.8, 0.8, 0.0
};
 
               double OITO [25] = {
    0.0, 0.8, 0.8, 0.8, 0.0,
    0.0, 0.8, 0.0, 0.8, 0.0,
    0.0, 0.8, 0.8, 0.8, 0.0,
    0.0, 0.8, 0.0, 0.8, 0.0,
    0.0, 0.8, 0.8, 0.8, 0.0
};

               double NOVE [25] = {
    0.0, 0.8, 0.0, 0.0, 0.0,
    0.0, 0.8, 0.0, 0.0, 0.0,
    0.0, 0.8, 0.8, 0.8, 0.0,
    0.0, 0.8, 0.0, 0.8, 0.0,
    0.0, 0.8, 0.8, 0.8, 0.0
};

volatile uint8_t desenho_atual = 0; 

double* desenhos[] = {ZERO, UM, DOIS, TRES, QUATRO, CINCO, SEIS, SETE, OITO, NOVE};

void display_number(int num) {
    if (num >= 0 && num <= 9) {
        animacao(desenhos[num], pio0, 0, 1.0, 1.0, 1.0); // Branco como cor padrão
    }
}
   
    // Para definir a intensidade:
    uint32_t matrix_rgb(double r, double g, double b) {
    uint8_t R = (uint8_t)(r * 255);
    uint8_t G = (uint8_t)(g * 255);
    uint8_t B = (uint8_t)(b * 255);
    return (G << 16) | (R << 8) | B;
}
    // Rotina para acionar a matriz de LEDs
void animacao(double *desenho, PIO pio, uint sm, double r, double g, double b) {
    for (int i = 0; i < 25; i++) {
        uint32_t cor = matrix_rgb(desenho[i] * r, desenho[i] * g, desenho[i] * b);
        pio_sm_put_blocking(pio, sm, cor);
    }
}

    // Envie os dados para a matriz de LEDs WS2812
    for (int i = 0; i < 25; i++) {
        pio_sm_put_blocking(pio0, 0, colors[i]);
    }


// Função de debouncing para os botões
bool debounce() {
    static uint32_t last_time = 0;
    uint32_t now = time_us_32();
    if ((now - last_time) > 100000) { // Debounce de 100ms
        last_time = now;
        return true;
    }
    return false;
}

// Função de interrupção para o botão A
void button_a_irq(uint gpio, uint32_t events) {
    if (debounce() && gpio == BUTTON_A) {
        button_a_pressed = true;
    }
}

// Função de interrupção para o botão B
void button_b_irq(uint gpio, uint32_t events) {
    if (debounce() && gpio == BUTTON_B) {
        button_b_pressed = true;
    }
}

int main() {
    // Inicialização do hardware
    stdio_init_all();

    // Configuração dos GPIOs do LED vermelho
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);

    // Configuração dos botões
    gpio_init(BUTTON_A);
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_A); // Usar resistor de pull-up interno
    gpio_pull_up(BUTTON_B); // Usar resistor de pull-up interno

    // Configuração das interrupções dos botões
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_a_irq);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &button_b_irq);

    // Inicialização da matriz de LEDs WS2812
    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);

    // Loop principal
    while (true) {
        // Piscar o LED vermelho a 5 Hz
        blink_red_led();
        sleep_ms(100); // Intervalo de 100ms para 5 Hz

        // Verificar se o botão A foi pressionado
        if (button_a_pressed) {
            button_a_pressed = false;
            current_number = (current_number + 1) % 10; // Incrementa o número (0-9)
            display_number(current_number); // Atualiza a matriz de LEDs
        }

        // Verificar se o botão B foi pressionado
        if (button_b_pressed) {
            button_b_pressed = false;
            current_number = (current_number - 1 + 10) % 10; // Decrementa o número (0-9)
            display_number(current_number); // Atualiza a matriz de LEDs
        }
    }

    return 0;
}