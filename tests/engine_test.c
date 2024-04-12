#include "board.h"
#include <stdio.h>

int main()
{
	int ret;
	ret |= board_init();
	board_pwm_set_freq(200);
    ret |= board_start();
	printf("ret =  %d\n", ret);
	delay(1000);

	printf("Initialized!\n");
	float i1, i2;
	int power = 0;
	while (1)
	{
		scanf("%d", &power);
		if (power < 0 || power > 100)
			break;
		board_pwm_set_pw(14, 1000+10*power);
		board_pwm_set_pw(15, 1000+10*power);
	}
	

	printf("Resetting...\n");
	board_pwm_set_value(0, 0);
	board_pwm_set_value(1, 0);
	printf("Reset!\n");
	return 0;
}
