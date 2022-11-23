#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Problem 1
int check_leap_year (unsigned int y)
{
    /* Check if leap year */
    return (y % 400 == 0) || ((y % 4 == 0) && (y % 100 != 0));
}

int get_month_adjustment(unsigned int m, unsigned int y)
{
    /* Do the month adjustment so we can calculate the day of the week. */
    switch (m) {
        case 1: return check_leap_year(y) ? 0 : 1; 
        case 2: return check_leap_year(y) ? 3 : 4;
        case 3: return 4;
        case 4: return 0;
        case 5: return 2;
        case 6: return 5;
        case 7: return 0;
        case 8: return 3;
        case 9: return 6;
        case 10: return 1;
        case 11: return 4;
        case 12: return 6;
    }
    fprintf(stderr,"bad month %d.\n",m);
    exit(1);
}

unsigned int get_dow(unsigned int m, unsigned int d, unsigned int y)
{
    /* Get the string for the day of the week. */
    unsigned int j = get_month_adjustment(m, y);
    unsigned int n = (y - 1900) + j + d + y/4;
    return n % 7;
}

char *get_dow_name(unsigned int dow)
{
    switch (dow) {
        case 0: return "Sun";
        case 1: return "Mon";
        case 2: return "Tue";
        case 3: return "Wed";
        case 4: return "Thu";
        case 5: return "Fri";
        case 6: return "Sat";
    }
    fprintf(stderr,"invalid day of week %d.\n", dow);
    exit(1);
}

char *get_month_name(unsigned int m) {
    /* Get the string for the name of the month */
    switch (m) {
        case 1: return "Jan";
        case 2: return "Feb";
        case 3: return "Mar";
        case 4: return "Apr";
        case 5: return "May";
        case 6: return "Jun";
        case 7: return "Jul";
        case 8: return "Aug";
        case 9: return "Sep";
        case 10: return "Oct";
        case 11: return "Nov";
        case 12: return "Dec";
    }
    fprintf(stderr,"invalid month %d.\n", m);
    exit(1);
}

int get_days_in_month(int m, int y) 
{
    /* Get the days of the inputted month for the inputted year. */
    switch (m) {
        case 1: 
        case 3: 
        case 5:
        case 7:
        case 8:
        case 10:
        case 12: return 31;
        case 2: return check_leap_year(y) ? 29 : 28;
        case 4: 
        case 6:
        case 9:
        case 11: return 30;
    }
    fprintf(stderr,"bad month %d.\n",m);
    exit(1); 
}

void show_month(unsigned int m, unsigned int y, char *sep)
{
    /* Print out each day of the month */
    char *m_name = get_month_name(m);
    for (int d = 1; d <= get_days_in_month(m, y); d++) {
        unsigned int dow = get_dow(m, d, y);
        char *dow_name = get_dow_name(dow);
        if (!dow) {
            printf("%s\n", sep);
        } 
        printf("%s %s %d, %d\n", dow_name, m_name, d, y);
    }
}

// Problem 2
void print_pm_row(unsigned int n_cols) 
{
    printf("+-+"); // Print first column
    for (int col = 1; col < n_cols; col++) { 
        printf("-+");                   
    }
    printf("\n");
}

void print_pipe_row(unsigned int n_cols) 
{
    printf("| |"); // Print first column
    for (int col = 1; col < n_cols; col++) {
        printf(" |");
    }
    printf("\n");
}

void grid(unsigned int n_rows, unsigned int n_cols) 
{
    if (n_rows == 0 || n_cols == 0) {
        return;
    }
    // Print first grid
    print_pm_row(n_cols);
    print_pipe_row(n_cols);
    print_pm_row(n_cols);
    // Print subsequent grids
    for (int row = 1; row < n_rows; row++) {        
        print_pipe_row(n_cols);
        print_pm_row(n_cols);
    }
}

// Problem 3
unsigned char red(uint32_t color)
{
    return color >> 24;
}

unsigned char green(uint32_t color)
{
    return (color >> 16) & 255;
}

unsigned char blue(uint32_t color)
{
    return (color >> 8) & 255;
}

uint32_t make_color(unsigned char r, unsigned char g, unsigned char b) 
{
    /* Do bit shifting to construct the color from red, green, and blue */
    return (r << 24) + (g << 16) + (b << 8);
}

double luminance(uint32_t color)
{
    // luminance measures brightness
    // luminance of an RGB is 0.2126r + 0.7152g + 0.0722b
    return 0.2126 * red(color) + 0.7152 * green(color) + 0.0722 * blue(color);
}
int brighter(uint32_t color1, uint32_t color2) 
{
    // return true if color1 has greater luminance than color2
    return luminance(color1) > luminance(color2);
}

int dimmer(uint32_t color1, uint32_t color2)
{
    // return true if color1 has lesser luminance than color2
    return luminance(color1) < luminance(color2);
}

uint32_t rgb_negate(uint32_t color)
{
    // subtract each rgb component from 255 to negate
    // for example, the rgb negation of (1,2,3) is (254,253,252)
    unsigned char neg_red = 255 - red(color);
    unsigned char neg_green = 255 - green(color);
    unsigned char neg_blue = 255 - blue(color);
    return make_color(neg_red, neg_green, neg_blue);
}

uint32_t rgb_mid(uint32_t color1, uint32_t color2)
{
    // compute the middle color between color1 and color2, rounding down
    unsigned char red_mid = (red(color1) + red(color2)) / 2;
    unsigned char green_mid = (green(color1) + green(color2)) / 2;
    unsigned char blue_mid = (blue(color1)  + blue(color2)) / 2;
    return make_color(red_mid, green_mid, blue_mid);
}

uint32_t make_gray(unsigned char g)
{
    // construct a color all of whose RGB components are the given byte
    return make_color(g, g, g);
}
int is_gray(uint32_t color)
{
    // test whether a color is gray, that is, whether its RGB bytes are
    //   all the same number
    unsigned char r = red(color);
    unsigned char g = green(color);
    unsigned char b = blue(color);
    return g == r && g == b; 
}

void show_rgb_triple(uint32_t color)
{
    // print a color in "(R,G,B)" form, as in "(255,128,4)"
    printf("(%d,%d,%d)\n",red(color), green(color), blue(color));
}

void show_hex_color(uint32_t color)
{
    // print a color in #RRGGBB hex form, as in "#FF8004"
    // note that printf supports hexadecimal, so this is not as much
    //   work as it might seem
    printf("#%06X\n", color >> 8);
}

int main()
{
    printf("Testing Problem 1\n");
    printf("Running show_month(1, 2022, \"========\")\n");
    show_month(1, 2022, "========");
    printf("Running show_month(3, 2023, \"========\")\n");
    show_month(3, 2023, "========");
    printf("Running show_month(4, 2021, \"========\")\n");
    show_month(4, 2021, "========");
    printf("Running show_month(5, 2029, \"========\")\n");
    show_month(5, 2029, "========");
    printf("Running show_month(6, 2028, \"========\")\n");
    show_month(6, 2028, "========");
    printf("Running show_month(8, 2027, \"========\")\n");
    show_month(8, 2027, "========");
    printf("Running show_month(9, 2026, \"========\")\n");
    show_month(9, 2026, "========");
    printf("Running show_month(10, 2002, \"========\")\n");
    show_month(10, 2002, "========");
    printf("Running show_month(11, 2012, \"========\")\n");
    show_month(11, 2012, "========");
    printf("Running show_month(12, 2032, \"========\")\n");
    show_month(12, 2032, "========");
    printf("Running show_month(7, 1934, \"-----\")\n");
    show_month(7, 1934, "-----");
    printf("Running show_month(1, 1938, \"-----\")\n");
    show_month(1, 1938, "-----");
    printf("Running show_month(2, 1938, \"-----\")\n");
    show_month(2, 1938, "-----");
    printf("Running show_month(2, 2096, \"--AAA--\")\n");
    show_month(2, 2096, "--AAA--");
    printf("Testing Problem 2\n");
    printf("Testing grid(%d,%d).\n",0 ,0);
    grid(0,0);
    printf("Testing grid(%d,%d).\n",0 ,1);
    grid(0,1);
    printf("Testing grid(%d,%d).\n",1 ,0);
    grid(1, 0);
    printf("Testing grid(%d,%d).\n",1 ,1);
    grid(1, 1);
    printf("Testing grid(%d,%d).\n",1 ,5);
    grid(1, 5);
    printf("Testing grid(%d,%d).\n",5 ,1);
    grid(5, 1);
    printf("Testing grid(%d,%d).\n",2 ,3);
    grid(2, 3);
    printf("Testing grid(%d,%d).\n",6 ,4);
    grid(6, 4);
    printf("Testing Problem 3\n");
    uint32_t color = 0xFF800400;
    printf("Color is %08X.\n",color);
    printf("show_hex_color(color) expecting #FF8004\n");
    show_hex_color(color);
    printf("show_rgb_triple(color) expecting (255,128,4)\n");
    show_rgb_triple(color);
    printf("(expecting 255) red(color) = %d.\n", red(color));
    printf("(expecting 128) green(color) = %d.\n", green(color));
    printf("(expecting 4) blue(color) = %d.\n", blue(color));
    printf("(expecting 146.05) luminance(color) = %f.\n", luminance(color));
    
    color = 0x00FA9B00;
    printf("Color is %08X.\n",color);
    printf("show_hex_color(color) expecting #00FA9B\n");
    show_hex_color(color);
    printf("show_rgb_triple(color) expecting (0,250,155)\n");
    show_rgb_triple(color);
    printf("(expecting 0) red(color) = %d.\n", red(color));
    printf("(expecting 250) green(color) = %d.\n", green(color));
    printf("(expecting 155) blue(color) = %d.\n", blue(color));
    printf("(expecting 189.99) luminance(color) = %f.\n", luminance(color));

    color = 0x00000000;
    printf("Color is %08X.\n",color);
    printf("show_hex_color(color) expecting #000000\n");
    show_hex_color(color);
    printf("show_rgb_triple(color) expecting (0,0,0)\n");
    show_rgb_triple(color);
    printf("(expecting 0) red(color) = %d.\n", red(color));
    printf("(expecting 0) green(color) = %d.\n", green(color));
    printf("(expecting 0) blue(color) = %d.\n", blue(color));
    printf("(expecting 0.0) luminance(color) = %f.\n", luminance(color));

    color = 0xFFFFFF00;
    printf("Color is %08X.\n",color);
    printf("show_hex_color(color) expecting #FFFFFF\n");
    show_hex_color(color);
    printf("show_rgb_triple(color) expecting (255,255,255)\n");
    show_rgb_triple(color);
    printf("(expecting 255) red(color) = %d.\n", red(color));
    printf("(expecting 255) green(color) = %d.\n", green(color));
    printf("(expecting 255) blue(color) = %d.\n", blue(color));
    printf("(expecting 255.0) luminance(color) = %f.\n", luminance(color));

    printf("(expecting 0) brighter(0xFF800400,0x00FA9B00) = %d.\n", 
            brighter(0xFF800400,0x00FA9B00));
    printf("(expecting 1) dimmer(0xFF800400,0x00FA9B00) = %d.\n", 
            dimmer(0xFF800400,0x00FA9B00));
    printf("(expecting 1) brighter(0x00FA9B00,0xFF800400) = %d.\n", 
            brighter(0x00FA9B00,0xFF800400));
    printf("(expecting 0) dimmer(0x00FA9B00,0xFF800400) = %d.\n", 
            dimmer(0x00FA9B00,0xFF800400));
    printf("Negating (254,253,252) to (1,2,3) and showing rgb triple: ");
    show_rgb_triple(rgb_negate(0xFEFDFC00));
    printf("Negating (1,2,3) to (254,253,252) and showing rgb triple: ");
    show_rgb_triple(rgb_negate(0x01020300));
    printf("Making gray color make_gray(128) and showing rgb triple: ");
    show_rgb_triple(make_gray(128));
    printf("Making gray color make_gray(0) and showing rgb triple: ");
    show_rgb_triple(make_gray(0));
    printf("Making gray color make_gray(255) and showing rgb triple: ");
    show_rgb_triple(make_gray(255));
    printf("Getting middle color rgb_mid(0x00FA9B00,0xFF800400) " 
           "and showing rgb triple expecting (127,189,79): "); 
    show_rgb_triple(rgb_mid(0x00FA9B00,0xFF800400));
    printf("Getting middle color rgb_mid(0xFFFFFF00,0xFFFFFF00) " 
           "and showing rgb triple expecting (255,255,255): "); 
    show_rgb_triple(rgb_mid(0xFFFFFF00,0xFFFFFF00));
    printf("Getting middle color rgb_mid(0x00000000,0x00000000) " 
           "and showing rgb triple expecting (0,0,0): "); 
    show_rgb_triple(rgb_mid(0x00000000,0x00000000));
    printf("Getting middle color rgb_mid(0xAE0D3F00,0x08BC3000) " 
           "and showing rgb triple expecting (91,100,55): "); 
    show_rgb_triple(rgb_mid(0xAE0D3F00,0x08BC3000));
    printf("Getting middle color rgb_mid(0x11111100,0x11111100) " 
           "and showing rgb triple expecting (17,17,17): "); 
    show_rgb_triple(rgb_mid(0x11111100,0x11111100));
    printf("(expecting 0) is_gray(0xFF00FF00): %d.\n",is_gray(0xFF00FF00));
    printf("(expecting 1) is_gray(0x99999900): %d.\n",is_gray(0x99999900));
    printf("(expecting 0) is_gray(0x0088AA00): %d.\n",is_gray(0x0088AA00));
    printf("show_hex_color(0x99999900).\n");
    show_hex_color(0x99999900);
    printf("show_hex_color(0xF000F000).\n");
    show_hex_color(0xF000F000);
    return 0;
}

